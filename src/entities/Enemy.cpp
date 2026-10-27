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
    will = 20.0f;
    quenchedTimer = 0.0f;
    shatteredTimer = 0.0f;

    switch (type) {
        case EnemyType::SLASHER:
            humor = HumorType::CHOLERIC;
            radius = 20.0f;
            maxHealth = 80.0f;
            defense = 8.0f; // Light armor (~7% reduction)
            will = 10.0f;
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
            humor = HumorType::MELANCHOLIC;
            radius = 30.0f;
            maxHealth = 190.0f;
            defense = 32.0f; // Heavy plate armor (~24% reduction)
            will = 50.0f;
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
            humor = HumorType::SANGUINE;
            radius = 18.0f;
            maxHealth = 65.0f;
            defense = 12.0f; // Reinforced vanguard (~11% reduction)
            will = 30.0f;
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
            humor = HumorType::SANGUINE;
            radius = 18.0f;
            maxHealth = 60.0f;
            defense = 6.0f; // Fragile leather (~5% reduction)
            will = 15.0f;
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
            humor = HumorType::CHOLERIC;
            radius = 20.0f;
            maxHealth = 75.0f;
            defense = 10.0f; // Occult robes (~9% reduction)
            will = 20.0f;
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
            humor = HumorType::CHOLERIC;
            radius = 36.0f;
            maxHealth = 500.0f;
            defense = 26.0f; // Imperial regalia (~21% reduction)
            will = 60.0f;
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
            humor = HumorType::MELANCHOLIC;
            radius = 28.0f;
            maxHealth = 520.0f; // Formidable Chapter 1 High-Altar Boss
            defense = 30.0f;    // 1% power remnant crusader armor
            will = 65.0f;       // High will poise
            moveSpeed = 115.0f;
            attackRange = 175.0f;
            kitingRange = 0.0f;
            windupDuration = 0.58f;
            strikeDuration = 0.20f;
            recoveryDuration = 0.55f;
            stunDuration = 1.6f;
            telegraph.shape = TelegraphShape::LINE;
            telegraph.radius = 175.0f;
            telegraph.width = 54.0f;
            telegraph.warningColor = Color{ 255, 215, 80, 190 };
            bossAttackPhase = 0;
            isPerformingMeleeSmite = true;
            break;

        case EnemyType::KNIGHT_LENON_BOSS:
            humor = HumorType::SANGUINE;
            radius = 28.0f;
            maxHealth = 640.0f; // High HP courtyard martial sparring champion
            defense = 32.0f;    // Tempered Crusader plate
            will = 70.0f;       // Resolute vanguard captain poise
            moveSpeed = 125.0f;
            attackRange = 170.0f;
            kitingRange = 0.0f;
            windupDuration = 0.48f;
            strikeDuration = 0.20f;
            recoveryDuration = 0.45f;
            stunDuration = 1.4f;
            telegraph.shape = TelegraphShape::LINE;
            telegraph.radius = 170.0f;
            telegraph.width = 52.0f;
            telegraph.warningColor = Color{ 255, 215, 80, 190 };
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
        case EnemyType::ZIL_BOSS: return "ZIL'S FRAGMENT";
        case EnemyType::KNIGHT_LENON_BOSS: return "KNIGHT LENON - VANGUARD CAPTAIN";
    }
    return "UNKNOWN";
}

std::string Enemy::GetHumorName() const {
    switch (humor) {
        case HumorType::PHLEGMATIC: return "PHLEGMATIC [WATER]";
        case HumorType::CHOLERIC: return "CHOLERIC [FIRE]";
        case HumorType::MELANCHOLIC: return "MELANCHOLIC [EARTH]";
        case HumorType::SANGUINE: return "SANGUINE [BLOOD]";
    }
    return "UNKNOWN";
}

Color Enemy::GetHumorColor() const {
    switch (humor) {
        case HumorType::PHLEGMATIC: return Color{ 140, 230, 255, 255 }; // Ice cyan
        case HumorType::CHOLERIC: return Color{ 255, 140, 40, 255 };    // Amber flame
        case HumorType::MELANCHOLIC: return Color{ 190, 110, 255, 255 };// Deep violet earth
        case HumorType::SANGUINE: return Color{ 255, 60, 75, 255 };     // Crimson blood
    }
    return WHITE;
}

void Enemy::Quench(ParticleSystem& particles) {
    quenchedTimer = 5.0f;
    SoundFX::PlayParrySuccess();
    particles.SpawnFloatingText(Vector2{ position.x, position.y - 30.0f }, "QUENCHED! (-30% SPEED)", Color{ 140, 230, 255, 255 }, 24.0f);
    particles.SpawnSparks(position, Color{ 140, 230, 255, 255 }, 25, 160.0f);
}

void Enemy::ShatterShell(ParticleSystem& particles) {
    shatteredTimer = 4.0f;
    SoundFX::PlayCrusaderClash();
    particles.AddScreenShake(5.0f, 0.18f);
    particles.SpawnFloatingText(Vector2{ position.x, position.y - 30.0f }, "SHELL SHATTERED! (-35% DEF)", Color{ 200, 130, 255, 255 }, 24.0f);
    particles.SpawnSparks(position, Color{ 180, 100, 240, 255 }, 28, 180.0f);
}

void Enemy::Update(float dt, Vector2 playerPos, const GameMap& map, ParticleSystem& particles, std::vector<Projectile>& outProjectiles) {
    if (state == EnemyState::DEAD) return;

    // Humoral status effect timers
    if (quenchedTimer > 0.0f) {
        quenchedTimer -= dt;
        particles.SpawnSparks(position, Color{ 140, 230, 255, 140 }, 1, 15.0f);
    }
    if (shatteredTimer > 0.0f) {
        shatteredTimer -= dt;
        particles.SpawnSparks(position, Color{ 190, 120, 245, 140 }, 1, 15.0f);
    }

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
                    // Truth Seeker 4-phase combat pattern with telegraphed follow-up cleave:
                    if (bossAttackPhase == 0) {
                        // Phase 0: Upward Radiant Slice (Line / Box AoE)
                        windupDuration = 0.58f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::LINE;
                        telegraph.radius = 175.0f;
                        telegraph.width = 54.0f;
                        telegraph.warningColor = Color{ 255, 215, 80, 190 };
                        isPerformingMeleeSmite = true;
                    } else if (bossAttackPhase == 1) {
                        // Phase 1: Dawnfire 360 Tremor Sweep (Circle AoE)
                        windupDuration = 0.65f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::CIRCLE;
                        telegraph.radius = 155.0f;
                        telegraph.warningColor = Color{ 255, 175, 45, 190 };
                        isPerformingMeleeSmite = true;
                    } else if (bossAttackPhase == 2) {
                        // Phase 2: Holy Beam Blast (Radiant Piercing Beam Projectile)
                        windupDuration = 0.52f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::LINE;
                        telegraph.radius = 350.0f;
                        telegraph.width = 24.0f;
                        telegraph.warningColor = Color{ 120, 230, 255, 210 };
                        isPerformingMeleeSmite = false; // Projectile deals damage!
                    } else {
                        // Phase 3: Telegraphed Follow-up Cleave (Damaging Melee Sweep!)
                        windupDuration = 0.42f; // Dedicated telegraph reaction window
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::CONE;
                        telegraph.radius = 165.0f;
                        telegraph.arcAngle = 140.0f;
                        telegraph.warningColor = Color{ 255, 190, 50, 200 };
                        isPerformingMeleeSmite = true; // Melee hitbox deals damage!
                    }
                } else if (type == EnemyType::KNIGHT_LENON_BOSS) {
                    // Lenon Vanguard 3-phase high-intensity combat pattern:
                    if (bossAttackPhase == 0) {
                        // Royal Greatsword Lunge (Line / Box AoE)
                        windupDuration = 0.48f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::LINE;
                        telegraph.radius = 170.0f;
                        telegraph.width = 52.0f;
                        telegraph.warningColor = Color{ 255, 215, 80, 190 };
                        isPerformingMeleeSmite = true;
                    } else if (bossAttackPhase == 1) {
                        // Golden Whirlwind Slash (Circle AoE)
                        windupDuration = 0.55f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::CIRCLE;
                        telegraph.radius = 150.0f;
                        telegraph.warningColor = Color{ 255, 185, 45, 190 };
                        isPerformingMeleeSmite = true;
                    } else {
                        // Crusader's Piercing Thrust (Narrow Line AoE with extra range)
                        windupDuration = 0.45f;
                        stateTimer = windupDuration;
                        telegraph.shape = TelegraphShape::LINE;
                        telegraph.radius = 185.0f;
                        telegraph.width = 44.0f;
                        telegraph.warningColor = Color{ 255, 235, 100, 210 };
                        isPerformingMeleeSmite = true;
                    }
                }
            } else {
                // Approach player
                Vector2 moveDir = Vector2Normalize({ dx, dy });
                float currentSpeed = moveSpeed * (IsQuenched() ? 0.70f : 1.0f);
                velocity.x = moveDir.x * currentSpeed;
                velocity.y = moveDir.y * currentSpeed;
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
                    float rad = facingAngle * DEG2RAD;
                    Vector2 forwardDir = { cosf(rad), sinf(rad) };

                    if (bossAttackPhase == 0) {
                        // Phase 0: Truth Seeker Upward Radiant Slice (Line / Box strike)
                        particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 220, 90, 255 });
                        particles.AddScreenShake(4.5f, 0.15f);
                        SoundFX::PlayCrusaderClash();
                    } else if (bossAttackPhase == 1) {
                        // Phase 1: Truth Seeker Dawnfire 360 Tremor Sweep
                        particles.SpawnSparks(position, Color{ 255, 185, 50, 255 }, 24, 160.0f);
                        particles.AddScreenShake(6.0f, 0.20f);
                        SoundFX::PlayHeavyAttack();
                    } else if (bossAttackPhase == 2) {
                        // Phase 2: Truth Seeker Holy Beam Projectile
                        Vector2 spawnPos = { position.x + forwardDir.x * (radius + 10.0f), position.y + forwardDir.y * (radius + 10.0f) };
                        Vector2 projVel = { forwardDir.x * 420.0f, forwardDir.y * 420.0f };
                        outProjectiles.emplace_back(spawnPos, projVel, 9.0f, 24.0f, ProjectileType::HOLY_BEAM, false);
                        SoundFX::PlayProjectileShoot();
                        particles.SpawnSparks(spawnPos, Color{ 140, 235, 255, 255 }, 16, 140.0f);
                        particles.AddScreenShake(3.5f, 0.12f);
                    } else {
                        // Phase 3: Telegraphed Follow-up Melee Cleave
                        particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 190, 50, 255 });
                        particles.SpawnSparks(position, Color{ 255, 220, 80, 255 }, 22, 160.0f);
                        particles.AddScreenShake(6.0f, 0.20f);
                        SoundFX::PlayCrusaderClash();
                    }
                    // Cycle boss attack phase across 4 distinct stages
                    bossAttackPhase = (bossAttackPhase + 1) % 4;
                } else if (type == EnemyType::KNIGHT_LENON_BOSS) {
                    float rad = facingAngle * DEG2RAD;
                    Vector2 forwardDir = { cosf(rad), sinf(rad) };

                    if (bossAttackPhase == 0) {
                        // Phase 0: Royal Greatsword Lunge
                        particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 220, 80, 255 });
                        particles.AddScreenShake(4.5f, 0.15f);
                        SoundFX::PlayCrusaderClash();
                    } else if (bossAttackPhase == 1) {
                        // Phase 1: Golden Whirlwind
                        particles.SpawnSparks(position, Color{ 255, 190, 50, 255 }, 22, 145.0f);
                        particles.AddScreenShake(5.5f, 0.18f);
                        SoundFX::PlayHeavyAttack();
                    } else {
                        // Phase 2: Piercing Thrust
                        particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 240, 130, 255 });
                        particles.AddScreenShake(4.0f, 0.12f);
                        SoundFX::PlayLightAttack();
                    }
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
    float baseStun = 2.2f;
    if (humor == HumorType::CHOLERIC) {
        Quench(particles);
        baseStun *= 2.0f; // 2.0x stagger duration on Quenching Choleric frenzy!
    }
    float staggerDuration = CalculateStaggerDuration(baseStun, will, false);
    stateTimer = staggerDuration;
    hasDamagedPlayer = true;
    velocity = { 0.0f, 0.0f };

    particles.SpawnFloatingText({ position.x, position.y - 25.0f }, "STAGGERED!", Color{ 255, 200, 60, 255 }, 24.0f);
    particles.SpawnFloatingText({ position.x, position.y - 5.0f }, "DEFENSE BROKEN!", Color{ 255, 120, 60, 255 }, 16.0f);
}

bool Enemy::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles, float defensePierce) {
    if (state == EnemyState::DEAD) return false;

    if (state == EnemyState::STUNNED) {
        amount *= 1.5f;
        particles.SpawnFloatingText({ position.x, position.y - 30.0f }, "CRIT!", Color{ 255, 220, 0, 255 }, 20.0f);
    }

    // Apply Enemy Defense formula with defense pierce and Melancholic shell shatter
    float activeDef = defense;
    if (IsShattered()) {
        activeDef *= 0.65f; // -35% armor on shattered defense shell!
    }
    float effDef = activeDef * fmaxf(0.0f, 1.0f - defensePierce);
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

    if (attack.isLineBox) {
        float rad = attack.angle * DEG2RAD;
        Vector2 dir = { cosf(rad), sinf(rad) };
        Vector2 normal = { -dir.y, dir.x };
        Vector2 toEnemy = { position.x - attack.origin.x, position.y - attack.origin.y };
        float forwardDist = toEnemy.x * dir.x + toEnemy.y * dir.y;
        float lateralDist = fabsf(toEnemy.x * normal.x + toEnemy.y * normal.y);
        return (forwardDist >= -radius && forwardDist <= (attack.range + radius) &&
                lateralDist <= (attack.width * 0.5f + radius));
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

