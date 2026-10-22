#include "Enemy.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/AssetManager.hpp"
#include "raymath.h"
#include <cmath>

Enemy::Enemy(EnemyType type, Vector2 startPos) : type(type), position(startPos) {
    velocity = { 0.0f, 0.0f };
    state = EnemyState::IDLE_CHASE;
    stateTimer = 0.0f;
    hasDamagedPlayer = false;
    warnedSound = false;
    facingAngle = 0.0f;
    bossAttackPhase = 0;
    burstShotsRemaining = 0;
    burstIntervalTimer = 0.0f;
    kitingRange = 0.0f;

    switch (type) {
        case EnemyType::SLASHER:
            radius = 20.0f;
            maxHealth = 80.0f;
            moveSpeed = 165.0f;
            attackRange = 115.0f;
            windupDuration = 0.85f;
            strikeDuration = 0.16f;
            recoveryDuration = 0.65f;
            stunDuration = 2.0f;
            telegraph.shape = TelegraphShape::CONE;
            telegraph.arcAngle = 95.0f;
            telegraph.radius = attackRange;
            telegraph.warningColor = Color{ 255, 75, 45, 140 };
            break;

        case EnemyType::BRUTE:
            radius = 30.0f;
            maxHealth = 190.0f;
            moveSpeed = 100.0f;
            attackRange = 150.0f;
            windupDuration = 1.25f;
            strikeDuration = 0.22f;
            recoveryDuration = 0.90f;
            stunDuration = 2.4f;
            telegraph.shape = TelegraphShape::CIRCLE;
            telegraph.radius = attackRange;
            telegraph.warningColor = Color{ 255, 40, 70, 160 };
            break;

        case EnemyType::CHARGER:
            radius = 18.0f;
            maxHealth = 65.0f;
            moveSpeed = 190.0f;
            attackRange = 240.0f;
            windupDuration = 0.70f;
            strikeDuration = 0.18f;
            recoveryDuration = 0.55f;
            stunDuration = 1.8f;
            telegraph.shape = TelegraphShape::LINE;
            telegraph.radius = attackRange;
            telegraph.width = 50.0f;
            telegraph.warningColor = Color{ 255, 160, 20, 150 };
            break;

        case EnemyType::ARCHER:
            radius = 18.0f;
            maxHealth = 60.0f;
            moveSpeed = 150.0f;
            attackRange = 380.0f;
            kitingRange = 220.0f;
            windupDuration = 0.80f;
            strikeDuration = 0.14f;
            recoveryDuration = 0.85f;
            stunDuration = 1.8f;
            telegraph.shape = TelegraphShape::LINE;
            telegraph.radius = attackRange;
            telegraph.width = 8.0f;
            telegraph.warningColor = Color{ 255, 120, 40, 180 };
            break;

        case EnemyType::PYROMANCER:
            radius = 20.0f;
            maxHealth = 75.0f;
            moveSpeed = 125.0f;
            attackRange = 320.0f;
            kitingRange = 180.0f;
            windupDuration = 1.05f;
            strikeDuration = 0.18f;
            recoveryDuration = 0.95f;
            stunDuration = 2.0f;
            telegraph.shape = TelegraphShape::CONE;
            telegraph.radius = attackRange;
            telegraph.arcAngle = 55.0f;
            telegraph.warningColor = Color{ 190, 70, 255, 160 };
            break;

        case EnemyType::INQUISITOR_BOSS:
            radius = 36.0f;
            maxHealth = 500.0f;
            moveSpeed = 85.0f;
            attackRange = 420.0f;
            kitingRange = 0.0f;
            windupDuration = 1.10f;
            strikeDuration = 0.25f;
            recoveryDuration = 0.75f;
            stunDuration = 1.4f;
            telegraph.shape = TelegraphShape::CIRCLE;
            telegraph.radius = 180.0f;
            telegraph.warningColor = Color{ 255, 180, 40, 170 };
            break;
    }

    health = maxHealth;
}

std::string Enemy::GetName() const {
    switch (type) {
        case EnemyType::SLASHER: return "CULTIST SLASHER";
        case EnemyType::BRUTE: return "IRON BRUTE";
        case EnemyType::CHARGER: return "SHADOW CHARGER";
        case EnemyType::ARCHER: return "DEATHSEEKER ARCHER";
        case EnemyType::PYROMANCER: return "VOID PYROMANCER";
        case EnemyType::INQUISITOR_BOSS: return "THE GRAND INQUISITOR";
    }
    return "UNKNOWN";
}

void Enemy::Update(float dt, Vector2 playerPos, const GameMap& map, ParticleSystem& particles, std::vector<Projectile>& outProjectiles) {
    if (state == EnemyState::DEAD) return;

    float dx = playerPos.x - position.x;
    float dy = playerPos.y - position.y;
    float distToPlayer = sqrtf(dx * dx + dy * dy);

    // Aim calculation
    if (state == EnemyState::IDLE_CHASE || state == EnemyState::TELEGRAPH_WINDUP) {
        if (state == EnemyState::TELEGRAPH_WINDUP) {
            float targetAngle = atan2f(dy, dx) * RAD2DEG;
            float angleDiff = targetAngle - facingAngle;
            while (angleDiff > 180.0f) angleDiff -= 360.0f;
            while (angleDiff < -180.0f) angleDiff += 360.0f;
            facingAngle += angleDiff * dt * (type == EnemyType::ARCHER ? 4.5f : 3.5f);
        } else {
            facingAngle = atan2f(dy, dx) * RAD2DEG;
        }
    }

    switch (state) {
        case EnemyState::IDLE_CHASE: {
            // Kiting check for ranged enemies
            if (kitingRange > 0.0f && distToPlayer < kitingRange) {
                // Back away from player
                Vector2 moveDir = Vector2Normalize({ dx, dy });
                velocity.x = -moveDir.x * (moveSpeed * 0.9f);
                velocity.y = -moveDir.y * (moveSpeed * 0.9f);
            } else if (distToPlayer <= attackRange * 0.95f) {
                // Ready to attack
                state = EnemyState::TELEGRAPH_WINDUP;
                stateTimer = windupDuration;
                hasDamagedPlayer = false;
                warnedSound = false;
                velocity = { 0.0f, 0.0f };

                // Setup Boss attack phase telegraph
                if (type == EnemyType::INQUISITOR_BOSS) {
                    if (bossAttackPhase == 0) {
                        // Nova phase
                        telegraph.shape = TelegraphShape::CIRCLE;
                        telegraph.radius = 180.0f;
                        telegraph.warningColor = Color{ 255, 190, 40, 160 };
                    } else if (bossAttackPhase == 1) {
                        // Arcane Beam phase
                        telegraph.shape = TelegraphShape::LINE;
                        telegraph.radius = 460.0f;
                        telegraph.width = 36.0f;
                        telegraph.warningColor = Color{ 220, 50, 90, 170 };
                    } else {
                        // Teleport & Smite phase
                        telegraph.shape = TelegraphShape::CIRCLE;
                        telegraph.radius = 190.0f;
                        telegraph.warningColor = Color{ 255, 60, 60, 180 };
                    }
                }
            } else {
                // Approach player
                Vector2 moveDir = Vector2Normalize({ dx, dy });
                velocity.x = moveDir.x * moveSpeed;
                velocity.y = moveDir.y * moveSpeed;
            }
            break;
        }

        case EnemyState::TELEGRAPH_WINDUP: {
            stateTimer -= dt;
            float progress = 1.0f - (stateTimer / windupDuration);
            telegraph.progress = fmaxf(0.0f, fminf(1.0f, progress));
            telegraph.origin = position;
            telegraph.angle = facingAngle;

            if (!warnedSound && progress > 0.1f) {
                SoundFX::PlayTelegraphWarning();
                warnedSound = true;
            }

            if (progress > 0.7f && GetRandomValue(0, 3) == 0) {
                particles.SpawnSparks(position, telegraph.warningColor, 1, 30.0f);
            }

            // Boss Teleport windup fx
            if (type == EnemyType::INQUISITOR_BOSS && bossAttackPhase == 2 && progress > 0.75f && distToPlayer > 130.0f) {
                // Pre-smite teleport right next to player!
                particles.SpawnSparks(position, Color{ 160, 60, 255, 255 }, 20, 140.0f);
                float angleOff = (float)GetRandomValue(0, 360) * DEG2RAD;
                position = { playerPos.x + cosf(angleOff) * 90.0f, playerPos.y + sinf(angleOff) * 90.0f };
                position = map.ResolveCollision(position, position, radius);
                particles.SpawnSparks(position, Color{ 255, 200, 60, 255 }, 20, 140.0f);
                telegraph.origin = position;
            }

            if (stateTimer <= 0.0f) {
                state = EnemyState::STRIKE;
                stateTimer = strikeDuration;
                hasDamagedPlayer = false;

                if (type == EnemyType::CHARGER) {
                    float rad = facingAngle * DEG2RAD;
                    velocity = { cosf(rad) * 600.0f, sinf(rad) * 600.0f };
                    particles.SpawnSlashTrail(position, { cosf(rad), sinf(rad) }, Color{ 255, 140, 40, 255 });
                } else if (type == EnemyType::BRUTE) {
                    particles.AddScreenShake(5.0f, 0.18f);
                    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 20, 180.0f);
                } else if (type == EnemyType::SLASHER) {
                    float rad = facingAngle * DEG2RAD;
                    particles.SpawnSlashTrail(position, { cosf(rad), sinf(rad) }, Color{ 255, 70, 70, 255 });
                } else if (type == EnemyType::ARCHER) {
                    // Fire high-speed piercing bolt
                    float rad = facingAngle * DEG2RAD;
                    Vector2 dir = { cosf(rad), sinf(rad) };
                    Vector2 spawnPos = { position.x + dir.x * (radius + 8.0f), position.y + dir.y * (radius + 8.0f) };
                    Vector2 projVel = { dir.x * 580.0f, dir.y * 580.0f };

                    outProjectiles.emplace_back(spawnPos, projVel, 6.0f, 20.0f, ProjectileType::BOLT, false);
                    SoundFX::PlayProjectileShoot();
                    particles.SpawnSparks(spawnPos, Color{ 255, 140, 50, 255 }, 6, 80.0f);
                } else if (type == EnemyType::PYROMANCER) {
                    // Fire 3 void orbs in a fan spread
                    float baseRad = facingAngle * DEG2RAD;
                    float angles[3] = { baseRad - 0.32f, baseRad, baseRad + 0.32f };

                    for (int i = 0; i < 3; i++) {
                        Vector2 dir = { cosf(angles[i]), sinf(angles[i]) };
                        Vector2 spawnPos = { position.x + dir.x * (radius + 6.0f), position.y + dir.y * (radius + 6.0f) };
                        Vector2 projVel = { dir.x * 320.0f, dir.y * 320.0f };
                        outProjectiles.emplace_back(spawnPos, projVel, 8.0f, 16.0f, ProjectileType::VOID_ORB, false);
                    }
                    SoundFX::PlayProjectileShoot();
                    particles.SpawnSparks(position, Color{ 190, 70, 255, 255 }, 12, 100.0f);
                } else if (type == EnemyType::INQUISITOR_BOSS) {
                    if (bossAttackPhase == 0) {
                        // Nova: 12-way radial star burst
                        for (int i = 0; i < 12; i++) {
                            float starAngle = ((float)i * 30.0f) * DEG2RAD;
                            Vector2 dir = { cosf(starAngle), sinf(starAngle) };
                            Vector2 spawnPos = { position.x + dir.x * (radius + 8.0f), position.y + dir.y * (radius + 8.0f) };
                            Vector2 projVel = { dir.x * 320.0f, dir.y * 320.0f };
                            outProjectiles.emplace_back(spawnPos, projVel, 9.0f, 22.0f, ProjectileType::NOVA_STAR, false);
                        }
                        SoundFX::PlayProjectileShoot();
                        particles.AddScreenShake(4.0f, 0.15f);
                        particles.SpawnSparks(position, Color{ 255, 200, 50, 255 }, 24, 200.0f);
                    } else if (bossAttackPhase == 1) {
                        // Arcane Beam: 3 rapid beam shards in tight spread
                        float baseRad = facingAngle * DEG2RAD;
                        float angles[3] = { baseRad - 0.10f, baseRad, baseRad + 0.10f };
                        for (int i = 0; i < 3; i++) {
                            Vector2 dir = { cosf(angles[i]), sinf(angles[i]) };
                            Vector2 spawnPos = { position.x + dir.x * (radius + 10.0f), position.y + dir.y * (radius + 10.0f) };
                            Vector2 projVel = { dir.x * 640.0f, dir.y * 640.0f };
                            outProjectiles.emplace_back(spawnPos, projVel, 7.0f, 26.0f, ProjectileType::BEAM_SHARD, false);
                        }
                        SoundFX::PlayProjectileShoot();
                        particles.AddScreenShake(5.0f, 0.15f);
                        particles.SpawnSparks(position, Color{ 255, 60, 90, 255 }, 18, 160.0f);
                    } else {
                        // Cataclysmic Smite: ground tremor + 8 shard shockwaves
                        particles.AddScreenShake(8.0f, 0.25f);
                        particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 30, 240.0f);
                        for (int i = 0; i < 8; i++) {
                            float shardAngle = ((float)i * 45.0f) * DEG2RAD;
                            Vector2 dir = { cosf(shardAngle), sinf(shardAngle) };
                            Vector2 spawnPos = { position.x + dir.x * (radius + 6.0f), position.y + dir.y * (radius + 6.0f) };
                            Vector2 projVel = { dir.x * 350.0f, dir.y * 350.0f };
                            outProjectiles.emplace_back(spawnPos, projVel, 7.0f, 18.0f, ProjectileType::BEAM_SHARD, false);
                        }
                        SoundFX::PlayHeavyAttack();
                    }

                    // Cycle boss attack phase
                    bossAttackPhase = (bossAttackPhase + 1) % 3;
                }
            }
            break;
        }

        case EnemyState::STRIKE: {
            stateTimer -= dt;
            if (type != EnemyType::CHARGER) {
                velocity.x *= 0.85f;
                velocity.y *= 0.85f;
            }
            if (stateTimer <= 0.0f) {
                state = EnemyState::RECOVERY;
                stateTimer = recoveryDuration;
                velocity = { 0.0f, 0.0f };
            }
            break;
        }

        case EnemyState::RECOVERY: {
            stateTimer -= dt;
            velocity.x *= 0.85f;
            velocity.y *= 0.85f;
            if (stateTimer <= 0.0f) {
                state = EnemyState::IDLE_CHASE;
            }
            break;
        }

        case EnemyState::STUNNED: {
            stateTimer -= dt;
            velocity.x *= 0.80f;
            velocity.y *= 0.80f;
            if (stateTimer <= 0.0f) {
                state = EnemyState::IDLE_CHASE;
            }
            break;
        }

        case EnemyState::DEAD:
            break;
    }

    Vector2 nextPos = { position.x + velocity.x * dt, position.y + velocity.y * dt };
    position = map.ResolveCollision(position, nextPos, radius);
}

void Enemy::OnParried(ParticleSystem& particles) {
    state = EnemyState::STUNNED;
    stateTimer = stunDuration;
    hasDamagedPlayer = true;
    velocity = { 0.0f, 0.0f };

    particles.SpawnFloatingText({ position.x, position.y - 25.0f }, "STAGGERED!", Color{ 255, 230, 80, 255 }, 24.0f);
    particles.SpawnFloatingText({ position.x, position.y - 5.0f }, "DEFENSE BROKEN!", Color{ 255, 120, 60, 255 }, 16.0f);
}

bool Enemy::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles) {
    if (state == EnemyState::DEAD) return false;

    if (state == EnemyState::STUNNED) {
        amount *= 1.5f;
        particles.SpawnFloatingText({ position.x, position.y - 30.0f }, "CRIT!", Color{ 255, 220, 0, 255 }, 20.0f);
    }

    health -= amount;
    SoundFX::PlayHitImpact();
    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 12, 120.0f);
    particles.SpawnFloatingText(position, "-" + std::to_string((int)amount), Color{ 255, 240, 240, 255 }, 20.0f);

    float knockMult = (type == EnemyType::INQUISITOR_BOSS) ? 40.0f : ((type == EnemyType::BRUTE) ? 80.0f : 180.0f);
    velocity.x += knockbackDir.x * knockMult;
    velocity.y += knockbackDir.y * knockMult;

    if (health <= 0.0f) {
        health = 0.0f;
        state = EnemyState::DEAD;
        SoundFX::PlayEnemyDeath();
        particles.SpawnSparks(position, Color{ 200, 40, 40, 255 }, 25, 220.0f);
        particles.SpawnFloatingText({ position.x, position.y - 10.0f }, type == EnemyType::INQUISITOR_BOSS ? "BOSS SLAIN" : "DEFEATED", Color{ 255, 100, 100, 255 }, 24.0f);
        return true;
    }

    return false;
}

bool Enemy::CheckHit(const AttackInfo& attack) const {
    if (state == EnemyState::DEAD || attack.type == AttackType::NONE) return false;

    float dx = position.x - attack.origin.x;
    float dy = position.y - attack.origin.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > attack.range + radius) return false;

    float hitAngle = atan2f(dy, dx) * RAD2DEG;
    float diff = fabsf(hitAngle - attack.angle);
    while (diff > 180.0f) diff = 360.0f - diff;

    return diff <= (attack.arcAngle * 0.5f + 15.0f);
}

void Enemy::DrawTelegraph() const {
    if (state != EnemyState::TELEGRAPH_WINDUP && state != EnemyState::STRIKE) return;

    float alphaProgress = (state == EnemyState::STRIKE) ? 0.95f : telegraph.progress;
    Color activeOutline = Color{ 255, 230, 90, 240 };
    Color dangerFill = telegraph.warningColor;
    dangerFill.a = (unsigned char)(dangerFill.a * alphaProgress);

    switch (telegraph.shape) {
        case TelegraphShape::CONE: {
            float startAngle = telegraph.angle - telegraph.arcAngle * 0.5f;
            float endAngle = telegraph.angle + telegraph.arcAngle * 0.5f;

            DrawCircleSectorLines(position, telegraph.radius, startAngle, endAngle, 20, activeOutline);
            float currentRadius = telegraph.radius * (0.3f + 0.7f * telegraph.progress);
            DrawCircleSector(position, currentRadius, startAngle, endAngle, 20, dangerFill);

            float rad1 = startAngle * DEG2RAD;
            float rad2 = endAngle * DEG2RAD;
            Vector2 p1 = { position.x + cosf(rad1) * telegraph.radius, position.y + sinf(rad1) * telegraph.radius };
            Vector2 p2 = { position.x + cosf(rad2) * telegraph.radius, position.y + sinf(rad2) * telegraph.radius };
            DrawLineEx(position, p1, 2.0f, activeOutline);
            DrawLineEx(position, p2, 2.0f, activeOutline);
            break;
        }

        case TelegraphShape::CIRCLE: {
            DrawCircleLines((int)position.x, (int)position.y, telegraph.radius, activeOutline);
            DrawCircleLines((int)position.x, (int)position.y, telegraph.radius + 2.0f, Color{ 255, 60, 60, 180 });

            float currentRadius = telegraph.radius * telegraph.progress;
            DrawCircle((int)position.x, (int)position.y, currentRadius, dangerFill);
            break;
        }

        case TelegraphShape::LINE: {
            float rad = telegraph.angle * DEG2RAD;
            Vector2 dir = { cosf(rad), sinf(rad) };
            Vector2 normal = { -dir.y, dir.x };
            float halfWidth = telegraph.width * 0.5f;

            Vector2 c1 = { position.x - normal.x * halfWidth, position.y - normal.y * halfWidth };
            Vector2 c2 = { position.x + normal.x * halfWidth, position.y + normal.y * halfWidth };
            Vector2 c3 = { c2.x + dir.x * telegraph.radius, c2.y + dir.y * telegraph.radius };
            Vector2 c4 = { c1.x + dir.x * telegraph.radius, c1.y + dir.y * telegraph.radius };

            DrawLineEx(c1, c2, 2.0f, activeOutline);
            DrawLineEx(c2, c3, 2.0f, activeOutline);
            DrawLineEx(c3, c4, 2.0f, activeOutline);
            DrawLineEx(c4, c1, 2.0f, activeOutline);

            float fillLength = telegraph.radius * telegraph.progress;
            Vector2 f3 = { c2.x + dir.x * fillLength, c2.y + dir.y * fillLength };
            Vector2 f4 = { c1.x + dir.x * fillLength, c1.y + dir.y * fillLength };
            DrawTriangle(c1, c2, f3, dangerFill);
            DrawTriangle(c1, f3, f4, dangerFill);
            break;
        }
    }
}

void Enemy::Draw() const {
    if (state == EnemyState::DEAD) return;

    Color bodyColor = Color{ 160, 40, 50, 255 };
    if (type == EnemyType::BRUTE) {
        bodyColor = Color{ 110, 30, 45, 255 };
    } else if (type == EnemyType::CHARGER) {
        bodyColor = Color{ 180, 80, 30, 255 };
    } else if (type == EnemyType::ARCHER) {
        bodyColor = Color{ 40, 110, 85, 255 }; // Forest stealth hunter
    } else if (type == EnemyType::PYROMANCER) {
        bodyColor = Color{ 95, 30, 140, 255 }; // Occult purple void
    } else if (type == EnemyType::INQUISITOR_BOSS) {
        bodyColor = Color{ 60, 20, 35, 255 }; // Imperial dark crimson & gold
    }

    bool isStunned = (state == EnemyState::STUNNED);
    AssetManager::DrawEnemySilhouette(position, facingAngle, bodyColor, radius, isStunned);

    // Boss special crown and aura
    if (type == EnemyType::INQUISITOR_BOSS) {
        float time = (float)GetTime() * 4.0f;
        float auraPulse = radius + 8.0f + 3.0f * sinf(time);
        DrawCircleLines((int)position.x, (int)position.y, auraPulse, Color{ 255, 215, 60, 140 });

        // Boss runic crown spikes
        float rad = facingAngle * DEG2RAD;
        Vector2 dir = { cosf(rad), sinf(rad) };
        Vector2 normal = { -dir.y, dir.x };
        Vector2 crownCenter = { position.x + dir.x * 8.0f, position.y + dir.y * 8.0f };
        DrawCircleV(crownCenter, 6.0f, Color{ 255, 215, 60, 255 });
        DrawCircleV({ crownCenter.x - normal.x * 9.0f, crownCenter.y - normal.y * 9.0f }, 4.0f, Color{ 255, 180, 40, 240 });
        DrawCircleV({ crownCenter.x + normal.x * 9.0f, crownCenter.y + normal.y * 9.0f }, 4.0f, Color{ 255, 180, 40, 240 });
    }

    // Overhead Health Bar (for normal enemies; boss has master top bar)
    if (type != EnemyType::INQUISITOR_BOSS) {
        const float barWidth = radius * 2.2f;
        const float barHeight = 5.0f;
        float barX = position.x - barWidth * 0.5f;
        float barY = position.y - radius - 16.0f;

        DrawRectangle((int)barX, (int)barY, (int)barWidth, (int)barHeight, Color{ 20, 20, 26, 220 });
        float healthPercent = fmaxf(0.0f, health / maxHealth);
        Color hpColor = (state == EnemyState::STUNNED) ? Color{ 255, 220, 40, 255 } : Color{ 220, 50, 60, 255 };
        DrawRectangle((int)barX, (int)barY, (int)(barWidth * healthPercent), (int)barHeight, hpColor);
        DrawRectangleLines((int)barX, (int)barY, (int)barWidth, (int)barHeight, Color{ 40, 45, 60, 200 });
    }
}
