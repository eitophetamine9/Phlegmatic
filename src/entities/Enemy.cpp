#include "Enemy.hpp"
#include "EnemyRenderer.hpp"
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
    isPerformingMeleeSmite = false;

    switch (type) {
        case EnemyType::SLASHER:
            radius = 20.0f;
            maxHealth = 80.0f;
            defense = 8.0f; // Light armor (~7% reduction)
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
            defense = 32.0f; // Heavy plate armor (~24% reduction)
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
            defense = 12.0f; // Reinforced vanguard (~11% reduction)
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
            defense = 6.0f; // Fragile leather (~5% reduction)
            moveSpeed = 150.0f;
            attackRange = 360.0f;
            kitingRange = 200.0f;
            windupDuration = 0.52f;
            strikeDuration = 0.12f;
            recoveryDuration = 0.65f;
            stunDuration = 1.8f;
            telegraph.shape = TelegraphShape::LINE;
            telegraph.radius = attackRange;
            telegraph.width = 8.0f;
            telegraph.warningColor = Color{ 255, 120, 40, 180 };
            break;

        case EnemyType::PYROMANCER:
            radius = 20.0f;
            maxHealth = 75.0f;
            defense = 10.0f; // Occult robes (~9% reduction)
            moveSpeed = 125.0f;
            attackRange = 320.0f;
            kitingRange = 170.0f;
            windupDuration = 0.68f;
            strikeDuration = 0.14f;
            recoveryDuration = 0.70f;
            stunDuration = 2.0f;
            telegraph.shape = TelegraphShape::PROJECTILE_SPREAD;
            telegraph.radius = attackRange;
            telegraph.spreadCount = 3;
            telegraph.spreadAngle = 0.30f;
            telegraph.warningColor = Color{ 190, 70, 255, 180 };
            break;

        case EnemyType::INQUISITOR_BOSS:
            radius = 36.0f;
            maxHealth = 500.0f;
            defense = 26.0f; // Imperial regalia (~21% reduction)
            moveSpeed = 85.0f;
            attackRange = 420.0f;
            kitingRange = 0.0f;
            windupDuration = 0.70f;
            strikeDuration = 0.22f;
            recoveryDuration = 0.70f;
            stunDuration = 1.4f;
            telegraph.shape = TelegraphShape::CIRCLE;
            telegraph.radius = 180.0f;
            telegraph.warningColor = Color{ 255, 180, 40, 170 };
            break;

        case EnemyType::ZIL_BOSS:
            radius = 28.0f;
            maxHealth = 380.0f;
            defense = 28.0f; // Corrupted crusader armor (~22% reduction)
            moveSpeed = 105.0f;
            attackRange = 175.0f;
            kitingRange = 0.0f;
            windupDuration = 0.70f;
            strikeDuration = 0.20f;
            recoveryDuration = 0.65f;
            stunDuration = 1.6f;
            telegraph.shape = TelegraphShape::CONE;
            telegraph.arcAngle = 125.0f;
            telegraph.radius = 115.0f;
            telegraph.warningColor = Color{ 255, 170, 40, 180 };
            bossAttackPhase = 0;
            isPerformingMeleeSmite = true;
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
        case EnemyType::ZIL_BOSS: return "ZIL THE CORRUPTED CRUSADER";
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
                        // Nova phase (Pure ranged 12-way radial star burst)
                        windupDuration = 0.72f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::PROJECTILE_SPREAD;
                        telegraph.radius = 280.0f;
                        telegraph.spreadCount = 12;
                        telegraph.spreadAngle = 0.5236f;
                        telegraph.warningColor = Color{ 255, 190, 40, 180 };
                        isPerformingMeleeSmite = false;
                    } else if (bossAttackPhase == 1) {
                        // Arcane Beam phase (Pure ranged linear beam)
                        windupDuration = 0.52f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::LINE;
                        telegraph.radius = 460.0f;
                        telegraph.width = 36.0f;
                        telegraph.warningColor = Color{ 220, 50, 90, 170 };
                        isPerformingMeleeSmite = false;
                    } else {
                        // Teleport & Smite phase (Melee shockwave + shards)
                        windupDuration = 0.70f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::CIRCLE;
                        telegraph.radius = 190.0f;
                        telegraph.warningColor = Color{ 255, 60, 60, 180 };
                        isPerformingMeleeSmite = true;
                    }
                } else if (type == EnemyType::ZIL_BOSS) {
                    if (bossAttackPhase == 0) {
                        // Heavy Crusader Cleave (High-arc cone, parriable melee)
                        windupDuration = 0.75f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::CONE;
                        telegraph.arcAngle = 125.0f;
                        telegraph.radius = 120.0f;
                        telegraph.warningColor = Color{ 255, 160, 40, 180 };
                        isPerformingMeleeSmite = true;
                    } else if (bossAttackPhase == 1) {
                        // Shield Bash Charge (Linear charge)
                        windupDuration = 0.65f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::LINE;
                        telegraph.radius = 240.0f;
                        telegraph.width = 54.0f;
                        telegraph.warningColor = Color{ 255, 100, 30, 190 };
                        isPerformingMeleeSmite = true;
                    } else {
                        // Corrupted Flame Wave (3 flame projectiles)
                        windupDuration = 0.60f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::PROJECTILE_SPREAD;
                        telegraph.spreadCount = 3;
                        telegraph.spreadAngle = 0.25f;
                        telegraph.radius = 320.0f;
                        telegraph.warningColor = Color{ 255, 90, 40, 200 };
                        isPerformingMeleeSmite = false; // damage comes from flame projectiles
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
                    // Fire high-speed piercing bolt (fast, smooth parry window)
                    float rad = facingAngle * DEG2RAD;
                    Vector2 dir = { cosf(rad), sinf(rad) };
                    Vector2 spawnPos = { position.x + dir.x * (radius + 8.0f), position.y + dir.y * (radius + 8.0f) };
                    Vector2 projVel = { dir.x * 520.0f, dir.y * 520.0f };

                    outProjectiles.emplace_back(spawnPos, projVel, 6.0f, 20.0f, ProjectileType::BOLT, false);
                    SoundFX::PlayProjectileShoot();
                    particles.SpawnSparks(spawnPos, Color{ 255, 140, 50, 255 }, 6, 80.0f);
                } else if (type == EnemyType::PYROMANCER) {
                    // Fire 3 void orbs in a fan spread
                    float baseRad = facingAngle * DEG2RAD;
                    float angles[3] = { baseRad - 0.30f, baseRad, baseRad + 0.30f };

                    for (int i = 0; i < 3; i++) {
                        Vector2 dir = { cosf(angles[i]), sinf(angles[i]) };
                        Vector2 spawnPos = { position.x + dir.x * (radius + 6.0f), position.y + dir.y * (radius + 6.0f) };
                        Vector2 projVel = { dir.x * 290.0f, dir.y * 290.0f };
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
                            Vector2 projVel = { dir.x * 290.0f, dir.y * 290.0f };
                            outProjectiles.emplace_back(spawnPos, projVel, 9.0f, 22.0f, ProjectileType::NOVA_STAR, false);
                        }
                        SoundFX::PlayProjectileShoot();
                        particles.AddScreenShake(4.0f, 0.15f);
                        particles.SpawnSparks(position, Color{ 255, 200, 50, 255 }, 24, 200.0f);
                    } else if (bossAttackPhase == 1) {
                        // Arcane Beam: 3 rapid beam shards in tight spread
                        float baseRad = facingAngle * DEG2RAD;
                        float angles[3] = { baseRad - 0.08f, baseRad, baseRad + 0.08f };
                        for (int i = 0; i < 3; i++) {
                            Vector2 dir = { cosf(angles[i]), sinf(angles[i]) };
                            Vector2 spawnPos = { position.x + dir.x * (radius + 10.0f), position.y + dir.y * (radius + 10.0f) };
                            Vector2 projVel = { dir.x * 580.0f, dir.y * 580.0f };
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
                            Vector2 projVel = { dir.x * 320.0f, dir.y * 320.0f };
                            outProjectiles.emplace_back(spawnPos, projVel, 7.0f, 18.0f, ProjectileType::BEAM_SHARD, false);
                        }
                        SoundFX::PlayHeavyAttack();
                    }

                    // Cycle boss attack phase
                    bossAttackPhase = (bossAttackPhase + 1) % 3;
                } else if (type == EnemyType::ZIL_BOSS) {
                    if (bossAttackPhase == 0) {
                        // Heavy Cleave: visual slash trail + screen shake
                        float rad = facingAngle * DEG2RAD;
                        particles.SpawnSlashTrail(position, { cosf(rad), sinf(rad) }, Color{ 255, 170, 40, 255 });
                        particles.AddScreenShake(5.0f, 0.18f);
                        SoundFX::PlayCrusaderClash();
                    } else if (bossAttackPhase == 1) {
                        // Shield Charge: dash forward
                        float rad = facingAngle * DEG2RAD;
                        velocity = { cosf(rad) * 620.0f, sinf(rad) * 620.0f };
                        particles.SpawnSlashTrail(position, { cosf(rad), sinf(rad) }, Color{ 255, 120, 40, 255 });
                        particles.AddScreenShake(4.5f, 0.15f);
                        SoundFX::PlayDash();
                        SoundFX::PlayCrusaderClash();
                    } else {
                        // Corrupted Flame Wave: release 3 corrupted flame projectiles
                        float baseRad = facingAngle * DEG2RAD;
                        float angles[3] = { baseRad - 0.25f, baseRad, baseRad + 0.25f };
                        for (int i = 0; i < 3; i++) {
                            Vector2 dir = { cosf(angles[i]), sinf(angles[i]) };
                            Vector2 spawnPos = { position.x + dir.x * (radius + 8.0f), position.y + dir.y * (radius + 8.0f) };
                            Vector2 projVel = { dir.x * 340.0f, dir.y * 340.0f };
                            outProjectiles.emplace_back(spawnPos, projVel, 8.0f, 22.0f, ProjectileType::VOID_ORB, false);
                        }
                        SoundFX::PlayProjectileShoot();
                        particles.SpawnSparks(position, Color{ 220, 60, 200, 255 }, 14, 120.0f);
                        particles.AddScreenShake(3.5f, 0.12f);
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
                isPerformingMeleeSmite = false;
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

bool Enemy::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles, float defensePierce) {
    if (state == EnemyState::DEAD) return false;

    if (state == EnemyState::STUNNED) {
        amount *= 1.5f;
        particles.SpawnFloatingText({ position.x, position.y - 30.0f }, "CRIT!", Color{ 255, 220, 0, 255 }, 20.0f);
    }

    // Apply Enemy Defense formula with defense pierce
    float effDef = defense * fmaxf(0.0f, 1.0f - defensePierce);
    float damageFactor = 100.0f / (100.0f + effDef);
    float finalAmount = amount * damageFactor;

    health -= finalAmount;
    SoundFX::PlayHitImpact();
    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 12, 120.0f);
    particles.SpawnFloatingText(position, "-" + std::to_string((int)finalAmount), Color{ 255, 240, 240, 255 }, 20.0f);

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

    if (attack.isFrontalCircle) {
        float dx = position.x - attack.circleCenter.x;
        float dy = position.y - attack.circleCenter.y;
        float maxDist = radius + attack.circleRadius;
        return (dx * dx + dy * dy) <= (maxDist * maxDist);
    }

    float dx = position.x - attack.origin.x;
    float dy = position.y - attack.origin.y;
    float distSq = dx * dx + dy * dy;
    float maxDist = attack.range + radius;

    if (distSq > maxDist * maxDist) return false;

    if (attack.arcAngle >= 360.0f) return true;

    float hitAngle = atan2f(dy, dx) * RAD2DEG;
    float diff = fabsf(hitAngle - attack.angle);
    while (diff > 180.0f) diff = 360.0f - diff;

    return diff <= (attack.arcAngle * 0.5f + 15.0f);
}

void Enemy::DrawTelegraph() const {
    EnemyRenderer::DrawTelegraph(*this);
}

void Enemy::Draw() const {
    EnemyRenderer::Draw(*this);
}

