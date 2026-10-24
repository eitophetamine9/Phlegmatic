#include "CollisionSystem.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "Projectile.hpp"
#include "../core/Particles.hpp"
#include "../core/SoundFX.hpp"
#include "raymath.h"
#include <cmath>

void CollisionSystem::CheckCombatCollisions(Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Camera2D camera) {
    // 1. Player melee attack vs enemies
    if (player.IsAttacking() && !player.GetCurrentAttack().hasHit) {
        const auto& attack = player.GetCurrentAttack();
        bool hitAny = false;

        for (auto& enemy : enemies) {
            if (!enemy.IsAlive()) continue;

            bool hit = enemy.CheckHit(attack);

            // Also check Truth Seeker holy shockwave area
            if (!hit && attack.hasShockwave) {
                float sdx = enemy.GetPosition().x - attack.shockwavePos.x;
                float sdy = enemy.GetPosition().y - attack.shockwavePos.y;
                float sdistSq = sdx * sdx + sdy * sdy;
                float maxSafe = attack.shockwaveRadius + enemy.GetRadius();
                if (sdistSq <= maxSafe * maxSafe) {
                    hit = true;
                }
            }

            if (hit) {
                Vector2 knockOrigin = (attack.hasShockwave && !enemy.CheckHit(attack)) ? attack.shockwavePos : player.GetPosition();
                Vector2 knockbackDir = Vector2Normalize({
                    enemy.GetPosition().x - knockOrigin.x,
                    enemy.GetPosition().y - knockOrigin.y
                });
                if (knockbackDir.x == 0.0f && knockbackDir.y == 0.0f) knockbackDir = { 0.0f, -1.0f };
                enemy.TakeDamage(attack.damage, knockbackDir, particles, attack.defensePierce);
                hitAny = true;
            }
        }

        if (hitAny) {
            player.SetAttackHit();
            particles.AddScreenShake(attack.type == AttackType::HEAVY ? 6.0f : 2.5f, 0.12f);
        }
    }

    Vector2 playerPos = player.GetPosition();
    float playerRadius = player.GetRadius();

    // 2. Enemy melee strikes vs player (ONLY pure melee strikes, NOT ranged attacks!)
    for (auto& enemy : enemies) {
        if (!enemy.IsAlive() || !enemy.IsInStrike()) continue;
        if (!enemy.HasMeleeHitbox()) continue; // Skip ranged enemies; their projectiles deal damage!

        TelegraphIndicator tel = enemy.GetTelegraph();
        bool playerHit = false;

        if (tel.shape == TelegraphShape::CONE) {
            float dx = playerPos.x - enemy.GetPosition().x;
            float dy = playerPos.y - enemy.GetPosition().y;
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist <= tel.radius + playerRadius) {
                float hitAngle = atan2f(dy, dx) * RAD2DEG;
                float diff = fabsf(hitAngle - tel.angle);
                while (diff > 180.0f) diff = 360.0f - diff;
                if (diff <= (tel.arcAngle * 0.5f + 12.0f)) {
                    playerHit = true;
                }
            }
        } else if (tel.shape == TelegraphShape::CIRCLE) {
            float dx = playerPos.x - enemy.GetPosition().x;
            float dy = playerPos.y - enemy.GetPosition().y;
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist <= tel.radius + playerRadius) {
                playerHit = true;
            }
        } else if (tel.shape == TelegraphShape::LINE) {
            float rad = tel.angle * DEG2RAD;
            Vector2 dir = { cosf(rad), sinf(rad) };
            Vector2 normal = { -dir.y, dir.x };
            Vector2 toPlayer = { playerPos.x - enemy.GetPosition().x, playerPos.y - enemy.GetPosition().y };
            float projDist = toPlayer.x * dir.x + toPlayer.y * dir.y;
            float perpDist = fabsf(toPlayer.x * normal.x + toPlayer.y * normal.y);
            if (projDist >= -playerRadius && projDist <= tel.radius + playerRadius && perpDist <= (tel.width * 0.5f + playerRadius)) {
                playerHit = true;
            }
        }

        if (playerHit) {
            enemy.MarkDamagedPlayer();
            Vector2 knockbackDir = Vector2Normalize({
                playerPos.x - enemy.GetPosition().x,
                playerPos.y - enemy.GetPosition().y
            });

            float baseDamage = 18.0f;
            if (enemy.GetType() == EnemyType::BRUTE) baseDamage = 38.0f;
            else if (enemy.GetType() == EnemyType::CHARGER) baseDamage = 22.0f;
            else if (enemy.GetType() == EnemyType::INQUISITOR_BOSS) baseDamage = 45.0f;
            else if (enemy.GetType() == EnemyType::ZIL_BOSS) baseDamage = 32.0f;
            else if (enemy.GetType() == EnemyType::KNIGHT_LENON_BOSS) baseDamage = 30.0f;

            bool tookDamage = player.TakeDamage(baseDamage, knockbackDir, particles);
            if (!tookDamage && player.IsParrying()) {
                enemy.OnParried(particles);
            }
        }
    }

    // 3. Projectile Collisions
    for (auto& proj : projectiles) {
        if (!proj.IsActive()) continue;

        if (!proj.IsFromPlayer()) {
            // Enemy projectile vs Player
            float dx = playerPos.x - proj.GetPosition().x;
            float dy = playerPos.y - proj.GetPosition().y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist <= playerRadius + proj.GetRadius()) {
                if (player.IsDashing() || player.IsDaggerLunging()) {
                    // Dash / Dagger Lunge invulnerability frames: Evaded!
                    particles.SpawnFloatingText(playerPos, "EVADED", Color{ 120, 240, 180, 255 }, 20.0f);
                    proj.Destroy();
                } else if (player.IsParrying()) {
                    // F Parry reflection!
                    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
                    Vector2 aimDir = Vector2Normalize({ mouseWorld.x - playerPos.x, mouseWorld.y - playerPos.y });
                    if (Vector2Length(aimDir) < 0.1f) {
                        aimDir = Vector2Normalize({ -proj.GetVelocity().x, -proj.GetVelocity().y });
                    }
                    Vector2 reflectedVel = { aimDir.x * 680.0f, aimDir.y * 680.0f };
                    proj.Reflect(reflectedVel);
                    SoundFX::PlayProjectileReflect();
                    particles.SpawnSparks(proj.GetPosition(), Color{ 80, 240, 255, 255 }, 18, 160.0f);
                    particles.SpawnFloatingText(playerPos, "PROJECTILE REFLECTED!", Color{ 80, 240, 255, 255 }, 22.0f);
                    particles.AddScreenShake(3.5f, 0.12f);
                } else {
                    // Hit player
                    Vector2 knockDir = Vector2Normalize(proj.GetVelocity());
                    player.TakeDamage(proj.GetDamage(), knockDir, particles);
                    proj.Destroy();
                }
            }
        } else {
            // Reflected Player projectile vs Enemies
            for (auto& enemy : enemies) {
                if (!enemy.IsAlive()) continue;

                float dx = enemy.GetPosition().x - proj.GetPosition().x;
                float dy = enemy.GetPosition().y - proj.GetPosition().y;
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist <= enemy.GetRadius() + proj.GetRadius()) {
                    Vector2 knockDir = Vector2Normalize(proj.GetVelocity());
                    enemy.TakeDamage(proj.GetDamage(), knockDir, particles, proj.GetDefensePierce());
                    particles.SpawnSparks(proj.GetPosition(), Color{ 100, 230, 255, 255 }, 14, 130.0f);
                    particles.SpawnFloatingText(enemy.GetPosition(), "COUNTER HIT!", Color{ 255, 220, 40, 255 }, 20.0f);
                    proj.Destroy();
                    break;
                }
            }
        }
    }
}
