#include "CollisionSystem.hpp"
#include "LootDrop.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "Projectile.hpp"
#include "../core/Particles.hpp"
#include "../core/SoundFX.hpp"
#include "raymath.h"
#include <cmath>

void CollisionSystem::CheckCombatCollisions(Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Camera2D camera, std::vector<LootDrop>* outLootDrops) {
    // 0. Armor skill pending AoE resolution
    if (player.pendingArmorSkillAoE) {
        player.pendingArmorSkillAoE = false;
        for (auto& enemy : enemies) {
            if (!enemy.IsAlive()) continue;
            float dist = Vector2Distance(player.GetPosition(), enemy.GetPosition());
            if (dist <= player.pendingAoERadius) {
                Vector2 kDir = Vector2Normalize(Vector2Subtract(enemy.GetPosition(), player.GetPosition()));
                if (kDir.x == 0.0f && kDir.y == 0.0f) kDir = { 0.0f, -1.0f };
                if (enemy.TakeDamage(player.pendingAoEDamage, kDir, particles, player.pendingAoEArmorShred)) {
                    if (outLootDrops) {
                        LootDrop::SpawnFromEnemy(enemy, *outLootDrops);
                    }
                }
                if (player.pendingAoEStun > 0.0f) {
                    enemy.ApplyStagger(player.pendingAoEStun);
                }
            }
        }
    }

    // Basalt Pillars obstacle & projectile blocking
    for (const auto& pillar : player.basaltPillars) {
        if (!pillar.active) continue;
        for (auto& enemy : enemies) {
            if (!enemy.IsAlive()) continue;
            float d = Vector2Distance(pillar.position, enemy.GetPosition());
            float minD = pillar.radius + enemy.GetRadius();
            if (d < minD && d > 0.01f) {
                Vector2 pushDir = Vector2Scale(Vector2Subtract(enemy.GetPosition(), pillar.position), 1.0f / d);
                enemy.SetPosition(Vector2Add(pillar.position, Vector2Scale(pushDir, minD)));
            }
        }
        for (auto& proj : projectiles) {
            if (proj.IsActive() && !proj.IsFromPlayer()) {
                if (CheckCollisionCircles(proj.GetPosition(), proj.GetRadius(), pillar.position, pillar.radius)) {
                    proj.Destroy();
                    particles.SpawnSparks(proj.GetPosition(), Color{ 160, 150, 140, 255 }, 10, 100.0f);
                }
            }
        }
    }

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
                // Melancholic shell shatter: War Mace or Heavy attacks shatter brittle black-bile shells
                if (enemy.GetHumor() == HumorType::MELANCHOLIC && !enemy.IsShattered()) {
                    if (attack.weapon == WeaponType::MACE || attack.type == AttackType::HEAVY) {
                        enemy.ShatterShell(particles);
                    }
                }

                // Weapon Heavy Attack Specific On-Hit Effects
                float attackDamage = attack.damage;
                if (attack.type == AttackType::HEAVY) {
                    if (attack.weapon == WeaponType::DAGGERS) {
                        if (player.GetHeavyStage() == 1) {
                            // Stage 1: Shadow Puncture inflicts bleed
                            enemy.ApplyBleed(2.5f);
                            particles.SpawnFloatingText(enemy.GetPosition(), "BLEED INFLICTED!", Color{ 255, 60, 60, 255 }, 18.0f);
                        } else if (player.GetHeavyStage() == 2) {
                            // Stage 2: Backstab Cross-Slash (+40% bonus if enemy is bleeding or hit from rear)
                            float enemyAngle = enemy.GetFacingAngle();
                            float diff = fabsf(player.GetFacingAngle() - enemyAngle);
                            while (diff > 180.0f) diff = 360.0f - diff;
                            bool isBackstab = (diff < 65.0f);
                            if (enemy.IsBleeding() || isBackstab) {
                                attackDamage *= 1.40f;
                                particles.SpawnFloatingText(enemy.GetPosition(), isBackstab ? "BACKSTAB CRIT! +40%" : "HEMORRHAGE PUNISH! +40%", Color{ 255, 230, 80, 255 }, 22.0f);
                                particles.SpawnSparks(enemy.GetPosition(), Color{ 255, 210, 40, 255 }, 20, 180.0f);
                            }
                        } else if (player.GetHeavyStage() == 3) {
                            // Stage 3: Vault Kick (100% poise stagger)
                            enemy.ApplyStagger(1.2f);
                            particles.SpawnFloatingText(enemy.GetPosition(), "STAGGERED!", Color{ 120, 255, 180, 255 }, 20.0f);
                        }
                    } else if (attack.weapon == WeaponType::MACE) {
                        if (player.GetHeavyStage() == 1) {
                            // Stage 1: Overhead Skull Crusher inflicts Armor Fracture (-20% def for 4s)
                            enemy.ApplyArmorFracture(4.0f);
                            particles.SpawnFloatingText(enemy.GetPosition(), "ARMOR FRACTURE! (-20% DEF)", Color{ 255, 160, 40, 255 }, 20.0f);
                        } else if (player.GetHeavyStage() == 2) {
                            // Stage 2: Seismic Tremor knocks flat / heavy stagger
                            enemy.ApplyStagger(1.5f);
                            particles.SpawnFloatingText(enemy.GetPosition(), "KNOCKDOWN!", Color{ 255, 120, 30, 255 }, 22.0f);
                        }
                    } else if (attack.weapon == WeaponType::SWORD) {
                        if (player.GetHeavyStage() == 1) {
                            // Stage 1: Crescent Cleave restores +12 Composure
                            player.ModifyComposure(12.0f);
                            particles.SpawnFloatingText(player.GetPosition(), "+12 COMPOSURE", Color{ 120, 220, 255, 255 }, 18.0f);
                        } else if (player.GetHeavyStage() == 2) {
                            // Stage 2: Shock Thrust interrupts enemy windup
                            enemy.ApplyStagger(0.7f);
                            particles.SpawnFloatingText(enemy.GetPosition(), "INTERRUPTED!", Color{ 255, 220, 90, 255 }, 18.0f);
                        } else if (player.GetHeavyStage() == 3) {
                            // Stage 3: Radiant Execution (+50% damage bonus if enemy is staggered/vulnerable)
                            if (enemy.GetState() == EnemyState::STUNNED) {
                                attackDamage *= 1.50f;
                                particles.SpawnFloatingText(enemy.GetPosition(), "RADIANT SUNDER! +50%", Color{ 255, 240, 110, 255 }, 24.0f);
                                particles.SpawnSparks(enemy.GetPosition(), Color{ 255, 240, 120, 255 }, 25, 220.0f);
                            }
                        }
                    } else if (attack.weapon == WeaponType::TRUTH_SEEKER) {
                        if (player.GetHeavyStage() == 1) {
                            // Stage 1: Dawnfire Ring applies Solar Burn
                            enemy.ApplySolarBurn(3.0f);
                            particles.SpawnFloatingText(enemy.GetPosition(), "SOLAR BURN!", Color{ 255, 210, 50, 255 }, 20.0f);
                        } else if (player.GetHeavyStage() == 2) {
                            // Stage 2: Sacred Judgment Pillar 100% defense pierce
                            particles.SpawnFloatingText(enemy.GetPosition(), "JUDGMENT! (100% PIERCE)", Color{ 255, 255, 180, 255 }, 24.0f);
                            particles.SpawnSparks(enemy.GetPosition(), Color{ 255, 240, 140, 255 }, 25, 220.0f);
                        }
                    }
                }

                Vector2 knockOrigin = (attack.hasShockwave && !enemy.CheckHit(attack)) ? attack.shockwavePos : player.GetPosition();
                Vector2 knockbackDir = Vector2Normalize({
                    enemy.GetPosition().x - knockOrigin.x,
                    enemy.GetPosition().y - knockOrigin.y
                });
                if (knockbackDir.x == 0.0f && knockbackDir.y == 0.0f) knockbackDir = { 0.0f, -1.0f };
                if (enemy.TakeDamage(attackDamage, knockbackDir, particles, attack.defensePierce)) {
                    if (outLootDrops) {
                        LootDrop::SpawnFromEnemy(enemy, *outLootDrops);
                    }
                }
                hitAny = true;
            }
        }

        if (hitAny) {
            player.SetAttackHit();
            if (attack.type == AttackType::HEAVY) {
                particles.TriggerHitStop(0.05f);
                float rad = attack.angle * DEG2RAD;
                particles.AddDirectionalShake({ cosf(rad), sinf(rad) }, 8.0f, 0.22f);
            } else {
                particles.AddScreenShake(2.5f, 0.12f);
            }
        }
    }

    Vector2 playerPos = player.GetPosition();
    float playerRadius = player.GetRadius();
    float dt = GetFrameTime();

    // Patient Stance: standing unhit while nearby enemy telegraphs gives +8.0f/sec composure
    if (player.IsAlive() && !player.IsAttacking() && !player.IsDashing() && !player.IsDaggerLunging() && !player.IsStaggered() && !player.IsInPhlegmaticTrance()) {
        bool enemyTelegraphingNear = false;
        for (const auto& enemy : enemies) {
            if (!enemy.IsAlive()) continue;
            if (enemy.IsTelegraphing()) {
                float dx = playerPos.x - enemy.GetPosition().x;
                float dy = playerPos.y - enemy.GetPosition().y;
                if ((dx * dx + dy * dy) <= (220.0f * 220.0f)) {
                    enemyTelegraphingNear = true;
                    break;
                }
            }
        }
        if (enemyTelegraphingNear) {
            player.ModifyComposure(8.0f * dt);
        }
    }

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
            else if (enemy.GetType() == EnemyType::ZIL_BOSS) baseDamage = 34.0f;
            else if (enemy.GetType() == EnemyType::KNIGHT_LENON_BOSS) baseDamage = 34.0f;

            bool tookDamage = player.TakeDamage(baseDamage, knockbackDir, particles);
            if (!tookDamage && player.IsParrying()) {
                enemy.OnParried(particles);
                particles.TriggerHitStop(0.06f);
                particles.AddDirectionalShake({ -knockbackDir.x, -knockbackDir.y }, 7.5f, 0.20f);
                if (enemy.GetHumor() == HumorType::CHOLERIC) {
                    player.ModifyComposure(30.0f); // Quenching Choleric Frenzy composure bonus!
                }
            } else if (tookDamage) {
                // Sanguine bleed: unmitigated strikes apply arterial hemorrhage unless in Stoic Poise
                if (enemy.GetHumor() == HumorType::SANGUINE) {
                    player.ApplyBleed(3.0f, particles);
                }

                // Heavy and crushing boss attacks apply stagger to player if unparried
                bool isHeavyAttack = (enemy.GetType() == EnemyType::BRUTE ||
                                      enemy.GetType() == EnemyType::CHARGER ||
                                      enemy.GetType() == EnemyType::INQUISITOR_BOSS ||
                                      enemy.GetType() == EnemyType::ZIL_BOSS ||
                                      enemy.GetType() == EnemyType::KNIGHT_LENON_BOSS);
                if (isHeavyAttack) {
                    player.ModifyComposure(-25.0f); // Unmitigated heavy enemy impact!
                    particles.TriggerHitStop(0.04f);
                    particles.AddDirectionalShake(knockbackDir, 8.5f, 0.25f);
                    if (!player.HasStaggerImmunity()) {
                        player.ApplyStagger(1.0f, particles);
                    }
                }
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

            // In Phlegmatic Trance, automatic projectile reflection in 120px radius!
            if (player.IsInPhlegmaticTrance() && dist <= 120.0f) {
                Vector2 aimDir = Vector2Normalize({ -proj.GetVelocity().x, -proj.GetVelocity().y });
                if (Vector2Length(aimDir) < 0.1f) aimDir = { 1.0f, 0.0f };
                Vector2 reflectedVel = { aimDir.x * 720.0f, aimDir.y * 720.0f };
                proj.Reflect(reflectedVel);
                SoundFX::PlayProjectileReflect();
                particles.SpawnSparks(proj.GetPosition(), Color{ 140, 230, 255, 255 }, 20, 180.0f);
                particles.SpawnFloatingText(playerPos, "TRANCE REFLECT!", Color{ 140, 230, 255, 255 }, 22.0f);
                continue;
            }

            if (dist <= playerRadius + proj.GetRadius()) {
                if (player.IsDashing() || player.IsDaggerLunging() || player.HasInvulnerability()) {
                    // Dash / Dagger Lunge invulnerability frames: Evaded!
                    player.ModifyComposure(15.0f); // Evasive Dash through projectile hitbox
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
                    player.ModifyComposure(25.0f); // Timed parry reflection bonus
                    if (proj.GetType() == ProjectileType::VOID_ORB || proj.GetType() == ProjectileType::NOVA_STAR || proj.GetType() == ProjectileType::BEAM_SHARD) {
                        player.ModifyComposure(10.0f); // Extra quenching bonus on fiery/arcane projectiles
                    }
                    SoundFX::PlayProjectileReflect();
                    particles.SpawnSparks(proj.GetPosition(), Color{ 80, 240, 255, 255 }, 18, 160.0f);
                    particles.SpawnFloatingText(playerPos, "PROJECTILE REFLECTED!", Color{ 80, 240, 255, 255 }, 22.0f);
                    particles.TriggerHitStop(0.045f);
                    particles.AddDirectionalShake(aimDir, 5.5f, 0.16f);
                } else {
                    // Hit player
                    Vector2 knockDir = Vector2Normalize(proj.GetVelocity());
                    player.TakeDamage(proj.GetDamage(), knockDir, particles);
                    if (proj.GetType() == ProjectileType::BOLT) {
                        player.ApplyBleed(3.0f, particles);
                    }
                    proj.Destroy();
                }
            }
        } else {
            // Player projectile vs Enemies (with infinite piercing support)
            for (size_t eIdx = 0; eIdx < enemies.size(); ++eIdx) {
                auto& enemy = enemies[eIdx];
                if (!enemy.IsAlive()) continue;

                float dx = enemy.GetPosition().x - proj.GetPosition().x;
                float dy = enemy.GetPosition().y - proj.GetPosition().y;
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist <= enemy.GetRadius() + proj.GetRadius()) {
                    if (!proj.HasHitEnemy((int)eIdx)) {
                        proj.RecordHitEnemy((int)eIdx);
                        Vector2 knockDir = Vector2Normalize(proj.GetVelocity());
                        if (enemy.TakeDamage(proj.GetDamage(), knockDir, particles, proj.GetDefensePierce())) {
                            if (outLootDrops) {
                                LootDrop::SpawnFromEnemy(enemy, *outLootDrops);
                            }
                        }
                        particles.SpawnSparks(proj.GetPosition(), Color{ 255, 140, 50, 255 }, 18, 160.0f);
                        particles.SpawnFloatingText(enemy.GetPosition(), "PIERCING BOLT! -" + std::to_string((int)proj.GetDamage()), Color{ 255, 175, 45, 255 }, 22.0f);

                        if (!proj.IsPiercing()) {
                            proj.Destroy();
                            break;
                        }
                    }
                }
            }
        }
    }

    // Friendly Mini Elemental Golem Companion combat & movement
    if (player.GetTotem() == TotemType::ELEMENTAL_CORE_TOTEM && player.GetCompanion().active) {
        auto& comp = player.GetCompanion();
        if (comp.attackCooldown > 0.0f) comp.attackCooldown -= dt;

        Enemy* nearestEnemy = nullptr;
        float nearestDist = 320.0f;
        for (auto& enemy : enemies) {
            if (!enemy.IsAlive()) continue;
            float d = Vector2Distance(comp.position, enemy.GetPosition());
            if (d < nearestDist) {
                nearestDist = d;
                nearestEnemy = &enemy;
            }
        }

        if (nearestEnemy) {
            Vector2 toEnemy = Vector2Subtract(nearestEnemy->GetPosition(), comp.position);
            Vector2 dir = Vector2Normalize(toEnemy);
            if (nearestDist > 32.0f) {
                comp.position = Vector2Add(comp.position, Vector2Scale(dir, 165.0f * dt));
            } else if (comp.attackCooldown <= 0.0f) {
                comp.attackCooldown = 1.0f;
                Vector2 knock = dir;
                if (nearestEnemy->TakeDamage(comp.damage, knock, particles, 0.15f)) {
                    if (outLootDrops) LootDrop::SpawnFromEnemy(*nearestEnemy, *outLootDrops);
                }
                particles.SpawnSparks(comp.position, Color{ 100, 255, 180, 255 }, 18, 140.0f);
                particles.SpawnFloatingText(nearestEnemy->GetPosition(), "GOLEM SLAM! -14", Color{ 100, 245, 180, 255 }, 20.0f);
                SoundFX::PlayHitImpact();
            }
        } else {
            float distToPlayer = Vector2Distance(comp.position, player.GetPosition());
            if (distToPlayer > 60.0f) {
                Vector2 toPlayer = Vector2Normalize(Vector2Subtract(player.GetPosition(), comp.position));
                comp.position = Vector2Add(comp.position, Vector2Scale(toPlayer, 175.0f * dt));
            }
        }

        for (auto& enemy : enemies) {
            if (!enemy.IsAlive() || !enemy.IsInStrike() || !enemy.HasMeleeHitbox()) continue;
            if (Vector2Distance(comp.position, enemy.GetPosition()) <= (comp.radius + enemy.GetRadius() + 15.0f)) {
                float effDef = comp.defense;
                float baseDmg = 18.0f;
                if (enemy.GetType() == EnemyType::BRUTE) baseDmg = 38.0f;
                else if (enemy.GetType() == EnemyType::CHARGER) baseDmg = 22.0f;
                else if (enemy.GetType() == EnemyType::ORE_GOLEM) baseDmg = 26.0f;
                else if (enemy.GetType() == EnemyType::ELEMENTAL_GOLEM) baseDmg = 34.0f;
                else if (enemy.GetType() == EnemyType::MINI_ELEMENTAL_GOLEM) baseDmg = 18.0f;
                else if (enemy.GetType() == EnemyType::INQUISITOR_BOSS) baseDmg = 45.0f;
                else if (enemy.GetType() == EnemyType::ZIL_BOSS) baseDmg = 34.0f;
                else if (enemy.GetType() == EnemyType::KNIGHT_LENON_BOSS) baseDmg = 34.0f;
                float dmg = baseDmg * (100.0f / (100.0f + effDef));
                comp.health -= dmg;
                particles.SpawnFloatingText(comp.position, "-" + std::to_string((int)dmg), Color{ 255, 120, 80, 255 }, 18.0f);
                particles.SpawnSparks(comp.position, Color{ 180, 160, 140, 255 }, 12, 120.0f);
                if (comp.health <= 0.0f) {
                    comp.active = false;
                    comp.respawnTimer = 60.0f;
                    particles.SpawnFloatingText(comp.position, "COMPANION SHATTERED! (60s Respawn)", Color{ 255, 90, 90, 255 }, 24.0f);
                    particles.SpawnSparks(comp.position, Color{ 255, 140, 50, 255 }, 35, 200.0f);
                    SoundFX::PlayHitImpact();
                    break;
                }
            }
        }
    }
}
