#include "Player.hpp"
#include "PlayerRenderer.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/AssetManager.hpp"
#include "raymath.h"
#include <cmath>

Player::Player() {
    position = { 1200.0f, 900.0f };
    velocity = { 0.0f, 0.0f };
    radius = Config::PLAYER_RADIUS;
    facingAngle = 0.0f;
    currentWeapon = WeaponType::SWORD;

    health = Config::PLAYER_MAX_HEALTH;
    maxHealth = Config::PLAYER_MAX_HEALTH;
    stamina = Config::PLAYER_MAX_STAMINA;
    maxStamina = Config::PLAYER_MAX_STAMINA;
    defense = 15.0f; // Base player armor (mitigates ~13% raw damage)

    lightCooldown = 0.0f;
    heavyCooldown = 0.0f;
    parryCooldown = 0.0f;
    parryTimer = 0.0f;
    isParrying = false;
    counterBuffTimer = 0.0f;

    dashCooldown = 0.0f;
    dashTimer = 0.0f;
    isDashing = false;
    dashDir = { 0.0f, 0.0f };

    isDaggerLunging = false;
    daggerLungeTimer = 0.0f;
    daggerLungeDir = { 0.0f, 0.0f };

    heavyWindupTimer = 0.0f;
    isHeavyWindingUp = false;
    heavyStage = 1;
    heavyStageTimer = 0.0f;
    walkCycle = 0.0f;

    comboStep = 1;
    comboTimer = 0.0f;
    heavyTargetPos = { 0.0f, 0.0f };
    shockwaveActive = false;
    shockwaveTimer = 0.0f;
    shockwaveRadius = 0.0f;
    shockwavePos = { 0.0f, 0.0f };
    spawnedProjectiles.clear();
}

void Player::Init(Vector2 startPos) {
    position = startPos;
    health = maxHealth;
    stamina = maxStamina;
    velocity = { 0.0f, 0.0f };
    currentAttack.type = AttackType::NONE;
    isParrying = false;
    isDashing = false;
    isDaggerLunging = false;
    isHeavyWindingUp = false;
    heavyStage = 1;
    heavyStageTimer = 0.0f;
    counterBuffTimer = 0.0f;
    comboStep = 1;
    comboTimer = 0.0f;
    shockwaveActive = false;
    shockwaveTimer = 0.0f;
    spawnedProjectiles.clear();
}

void Player::Revive(Vector2 startPos, float hpPercent) {
    position = startPos;
    health = maxHealth * hpPercent;
    stamina = maxStamina;
    velocity = { 0.0f, 0.0f };
    currentAttack.type = AttackType::NONE;
    isParrying = false;
    isDashing = false;
    isHeavyWindingUp = false;
    counterBuffTimer = 0.0f;
    comboStep = 1;
    comboTimer = 0.0f;
    shockwaveActive = false;
    shockwaveTimer = 0.0f;
}

void Player::SetWeapon(WeaponType newWeapon) {
    currentWeapon = newWeapon;
    inventory.UnlockWeapon(newWeapon);
    if (newWeapon == WeaponType::TRUTH_SEEKER) {
        SoundFX::PlayTruthSeekerResonance();
    } else {
        SoundFX::PlayEquipWeapon();
    }
}

void Player::Update(float dt, const GameMap& map, Vector2 mouseWorldPos, ParticleSystem& particles) {
    if (!IsAlive()) return;

    // Face towards mouse unless locked in dash
    if (!isDashing) {
        float dx = mouseWorldPos.x - position.x;
        float dy = mouseWorldPos.y - position.y;
        facingAngle = atan2f(dy, dx) * RAD2DEG;
    }

    // Cooldown updates
    if (lightCooldown > 0.0f) lightCooldown -= dt;
    if (heavyCooldown > 0.0f) heavyCooldown -= dt;
    if (parryCooldown > 0.0f) parryCooldown -= dt;
    if (dashCooldown > 0.0f) dashCooldown -= dt;
    if (counterBuffTimer > 0.0f) counterBuffTimer -= dt;

    // Stamina regeneration
    if (!isParrying && !isHeavyWindingUp && !isDashing) {
        stamina = fminf(maxStamina, stamina + Config::STAMINA_REGEN_RATE * dt);
    }

    // Dash update (Q Key)
    if (isDashing) {
        dashTimer -= dt;
        velocity.x = dashDir.x * Config::DASH_SPEED;
        velocity.y = dashDir.y * Config::DASH_SPEED;

        particles.SpawnDashTrail(position, dashDir, Color{ 100, 220, 255, 180 });

        if (dashTimer <= 0.0f) {
            isDashing = false;
            velocity.x *= 0.3f;
            velocity.y *= 0.3f;
        }
    }

    // Parry update (F Key)
    if (isParrying) {
        parryTimer -= dt;
        if (parryTimer <= 0.0f) {
            isParrying = false;
        }
    }

    // Combo timer decay
    if (comboTimer > 0.0f) {
        comboTimer -= dt;
        if (comboTimer <= 0.0f) {
            comboStep = 1;
        }
    }

    // Shockwave timer decay
    if (shockwaveTimer > 0.0f) {
        shockwaveTimer -= dt;
        if (shockwaveTimer <= 0.0f) {
            shockwaveActive = false;
        }
    }

    // Daggers Lunge Update
    if (isDaggerLunging) {
        daggerLungeTimer -= dt;
        velocity.x = daggerLungeDir.x * 660.0f; // Buffed forward lunge speed
        velocity.y = daggerLungeDir.y * 660.0f;

        // Keep lunge attack origin and angle synchronized with moving player
        currentAttack.origin = position;
        currentAttack.angle = facingAngle;

        particles.SpawnSparks(position, Color{ 140, 240, 200, 200 }, 2, 40.0f);
        particles.SpawnSlashTrail(position, daggerLungeDir, Color{ 80, 255, 170, 180 });

        if (daggerLungeTimer <= 0.0f) {
            isDaggerLunging = false;
            heavyStage = 2;
            // Instantaneous 360 whirlwind spin attack!
            currentAttack.type = AttackType::HEAVY;
            currentAttack.weapon = WeaponType::DAGGERS;
            currentAttack.origin = position;
            currentAttack.angle = facingAngle;
            currentAttack.range = 95.0f; // Buffed whirlwind range
            currentAttack.arcAngle = 360.0f;
            currentAttack.isLineBox = false;
            currentAttack.damage = counterBuffTimer > 0.0f ? (68.0f * 1.5f) : 68.0f; // Buffed spin damage
            currentAttack.knockback = 280.0f;
            currentAttack.duration = 0.12f;
            currentAttack.timer = 0.12f;
            currentAttack.hasHit = false;
            currentAttack.defensePierce = 0.0f;
            currentAttack.isFrontalCircle = false;
            currentAttack.hasShockwave = false;

            SoundFX::PlayCrusaderClash();
            particles.AddScreenShake(4.5f, 0.14f);
            particles.SpawnSparks(position, Color{ 100, 255, 180, 255 }, 28, 200.0f);
        }
    }

    // Heavy attack windup
    if (isHeavyWindingUp) {
        heavyWindupTimer -= dt;
        if (currentWeapon == WeaponType::TRUTH_SEEKER) {
            particles.SpawnSparks(position, Color{ 255, 220, 60, 220 }, 2, 50.0f);
        } else if (currentWeapon == WeaponType::MACE) {
            particles.SpawnSparks(position, Color{ 255, 140, 40, 200 }, 2, 45.0f);
        } else {
            particles.SpawnSparks(position, Color{ 255, 200, 60, 200 }, 1, 40.0f);
        }

        if (heavyWindupTimer <= 0.0f) {
            isHeavyWindingUp = false;
            WeaponStats stats = GetWeaponStats(currentWeapon);
            heavyStage = 1;

            Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };

            currentAttack.type = AttackType::HEAVY;
            currentAttack.weapon = currentWeapon;
            currentAttack.origin = position;
            currentAttack.angle = facingAngle;
            currentAttack.hasHit = false;
            currentAttack.defensePierce = 0.0f;
            currentAttack.hasShockwave = false;
            currentAttack.isFrontalCircle = false;

            if (currentWeapon == WeaponType::MACE) {
                // War Mace Heavy Stage 1: Giant Frontal Smack
                currentAttack.isFrontalCircle = true;
                currentAttack.circleRadius = 85.0f;
                currentAttack.circleCenter = { position.x + forwardDir.x * 70.0f, position.y + forwardDir.y * 70.0f };
                currentAttack.range = 155.0f;
                currentAttack.arcAngle = 180.0f;
                currentAttack.damage = counterBuffTimer > 0.0f ? (75.0f * 1.5f) : 75.0f;
                currentAttack.knockback = 380.0f;
                currentAttack.duration = 0.16f;
                currentAttack.timer = 0.16f;

                SoundFX::PlayHeavyAttack();
                particles.AddScreenShake(7.0f, 0.20f);
                particles.SpawnSparks(currentAttack.circleCenter, Color{ 255, 160, 50, 255 }, 24, 180.0f);
            } else if (currentWeapon == WeaponType::SWORD) {
                // Broadsword Heavy Stage 1: Wide Slash
                currentAttack.range = 125.0f;
                currentAttack.arcAngle = 160.0f;
                currentAttack.damage = counterBuffTimer > 0.0f ? (42.0f * 1.5f) : 42.0f;
                currentAttack.knockback = 240.0f;
                currentAttack.duration = 0.16f;
                currentAttack.timer = 0.16f;

                SoundFX::PlayHeavyAttack();
                particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 190, 60, 255 });
                velocity.x += forwardDir.x * 180.0f;
                velocity.y += forwardDir.y * 180.0f;
            } else if (currentWeapon == WeaponType::TRUTH_SEEKER) {
                // Truth Seeker Heavy Stage 1: Full 360 circle AoE swing 1
                currentAttack.range = 135.0f;
                currentAttack.arcAngle = 360.0f;
                currentAttack.damage = counterBuffTimer > 0.0f ? (60.0f * 1.5f) : 60.0f;
                currentAttack.knockback = 360.0f;
                currentAttack.duration = 0.18f;
                currentAttack.timer = 0.18f;

                // Simultaneously shoot 1 holy projectile with 10% defense pierce
                Vector2 spawnPos = { position.x + forwardDir.x * (radius + 14.0f), position.y + forwardDir.y * (radius + 14.0f) };
                Vector2 projVel = { forwardDir.x * 580.0f, forwardDir.y * 580.0f };
                spawnedProjectiles.emplace_back(spawnPos, projVel, 9.0f, 45.0f, ProjectileType::HOLY_BEAM, true, 0.10f);

                SoundFX::PlayHeavyAttack();
                SoundFX::PlayCrusaderClash();
                particles.AddScreenShake(6.5f, 0.20f);
                particles.SpawnSparks(position, Color{ 255, 230, 80, 255 }, 28, 200.0f);
            } else {
                currentAttack.range = stats.heavyRange;
                currentAttack.arcAngle = stats.heavyArc;
                currentAttack.damage = counterBuffTimer > 0.0f ? (stats.heavyDamage * 1.5f) : stats.heavyDamage;
                currentAttack.knockback = 260.0f;
                currentAttack.duration = stats.heavyDuration;
                currentAttack.timer = stats.heavyDuration;

                SoundFX::PlayHeavyAttack();
                particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 180, 50, 255 });
                velocity.x += forwardDir.x * 200.0f;
                velocity.y += forwardDir.y * 200.0f;
            }
        }
    }

    // Current attack duration & multi-stage attack transitions
    if (currentAttack.type != AttackType::NONE) {
        currentAttack.timer -= dt;
        currentAttack.origin = position;
        if (currentAttack.timer <= 0.0f) {
            // Check for Stage 2 follow-ups
            if (currentAttack.type == AttackType::HEAVY && heavyStage == 1) {
                Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };

                if (currentWeapon == WeaponType::MACE) {
                    // War Mace Heavy Stage 2: Followup full 360 AoE with 25% defense pierce!
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = false;
                    currentAttack.range = 150.0f;
                    currentAttack.arcAngle = 360.0f;
                    currentAttack.damage = counterBuffTimer > 0.0f ? (80.0f * 1.5f) : 80.0f;
                    currentAttack.knockback = 460.0f;
                    currentAttack.duration = 0.22f;
                    currentAttack.timer = 0.22f;
                    currentAttack.hasHit = false;
                    currentAttack.defensePierce = 0.25f; // IGNORES 25% DEFENSE!
                    currentAttack.hasShockwave = true;
                    currentAttack.shockwavePos = position;
                    currentAttack.shockwaveRadius = 150.0f;

                    shockwaveActive = true;
                    shockwaveTimer = 0.38f;
                    shockwaveRadius = 150.0f;
                    shockwavePos = position;

                    SoundFX::PlayCrusaderClash();
                    particles.AddScreenShake(9.0f, 0.28f);
                    particles.SpawnSparks(position, Color{ 255, 120, 30, 255 }, 35, 240.0f);
                } else if (currentWeapon == WeaponType::SWORD) {
                    // Broadsword Heavy Stage 2: Followup upward shock slash (line/box AoE)
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.range = 135.0f;
                    currentAttack.width = 50.0f;
                    currentAttack.isLineBox = true;
                    currentAttack.arcAngle = 0.0f;
                    currentAttack.damage = counterBuffTimer > 0.0f ? (48.0f * 1.5f) : 48.0f;
                    currentAttack.knockback = 300.0f;
                    currentAttack.duration = 0.18f;
                    currentAttack.timer = 0.18f;
                    currentAttack.hasHit = false;

                    SoundFX::PlayCrusaderClash();
                    particles.AddScreenShake(5.0f, 0.16f);
                    particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 230, 110, 255 });
                    particles.SpawnSparks(position, Color{ 255, 200, 60, 255 }, 16, 160.0f);
                } else if (currentWeapon == WeaponType::TRUTH_SEEKER) {
                    // Truth Seeker Heavy Stage 2: Second consecutive 360 full circle AoE swing!
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.range = 145.0f;
                    currentAttack.arcAngle = 360.0f;
                    currentAttack.damage = counterBuffTimer > 0.0f ? (65.0f * 1.5f) : 65.0f;
                    currentAttack.knockback = 380.0f;
                    currentAttack.duration = 0.20f;
                    currentAttack.timer = 0.20f;
                    currentAttack.hasHit = false;
                    currentAttack.defensePierce = 0.0f;

                    SoundFX::PlayHeavyAttack();
                    particles.AddScreenShake(7.0f, 0.22f);
                    particles.SpawnSparks(position, Color{ 255, 240, 100, 255 }, 30, 220.0f);
                } else {
                    currentAttack.type = AttackType::NONE;
                    heavyStage = 1;
                }
            } else {
                currentAttack.type = AttackType::NONE;
                heavyStage = 1;
            }
        }
    }

    // Normal movement when not dashing
    if (!isDashing) {
        Vector2 moveDir = { 0.0f, 0.0f };
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    moveDir.y -= 1.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  moveDir.y += 1.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  moveDir.x -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDir.x += 1.0f;

        float currentSpeed = Config::PLAYER_SPEED;
        if (isParrying) currentSpeed *= 0.35f;
        if (isHeavyWindingUp) currentSpeed *= 0.25f;
        if (currentAttack.type == AttackType::LIGHT) currentSpeed *= 0.65f;

        if (moveDir.x != 0.0f || moveDir.y != 0.0f) {
            moveDir = Vector2Normalize(moveDir);
            velocity.x = moveDir.x * currentSpeed;
            velocity.y = moveDir.y * currentSpeed;
            walkCycle += dt * 10.0f;
        } else {
            velocity.x *= 0.80f;
            velocity.y *= 0.80f;
        }
    }

    // Collision
    Vector2 nextPos = { position.x + velocity.x * dt, position.y + velocity.y * dt };
    position = map.ResolveCollision(position, nextPos, radius);
}

void Player::TriggerDash(ParticleSystem& particles) {
    if (isDashing) return;
    if (dashCooldown > 0.0f) return;

    WeaponStats stats = GetWeaponStats(currentWeapon);
    if (stamina < stats.dashStaminaCost) return;

    stamina -= stats.dashStaminaCost;
    dashCooldown = stats.dashCooldown;
    dashTimer = Config::DASH_DURATION;
    isDashing = true;

    // Movement direction or facing direction
    Vector2 inputDir = { 0.0f, 0.0f };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    inputDir.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  inputDir.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  inputDir.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) inputDir.x += 1.0f;

    if (inputDir.x != 0.0f || inputDir.y != 0.0f) {
        dashDir = Vector2Normalize(inputDir);
    } else {
        dashDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
    }

    SoundFX::PlayDash();
}

void Player::TriggerLightAttack(ParticleSystem& particles) {
    if (currentWeapon == WeaponType::NONE) return;
    if (isDashing || isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;

    WeaponStats stats = GetWeaponStats(currentWeapon);
    // Daggers light attack costs 0 stamina
    if (currentWeapon != WeaponType::DAGGERS) {
        if (stamina < stats.lightStamina) return;
        stamina -= stats.lightStamina;
    }
    lightCooldown = stats.lightCooldown;

    // Truth Seeker 2-hit combo tracking
    if (currentWeapon == WeaponType::TRUTH_SEEKER) {
        if (comboTimer > 0.0f && comboStep == 1) {
            comboStep = 2;
        } else {
            comboStep = 1;
        }
        comboTimer = stats.lightDuration + 0.45f;
    } else {
        comboStep = 1;
    }

    Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };

    currentAttack.type = AttackType::LIGHT;
    currentAttack.weapon = currentWeapon;
    currentAttack.origin = position;
    currentAttack.angle = facingAngle;
    currentAttack.range = stats.lightRange;
    currentAttack.arcAngle = stats.lightArc;
    currentAttack.damage = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;
    currentAttack.comboStep = comboStep;
    currentAttack.hasShockwave = false;
    currentAttack.isFrontalCircle = false;
    currentAttack.defensePierce = 0.0f;

    if (currentWeapon == WeaponType::MACE) {
        // War Mace Light: Frontal circular smack
        currentAttack.isFrontalCircle = true;
        currentAttack.circleRadius = 50.0f;
        currentAttack.circleCenter = { position.x + forwardDir.x * 55.0f, position.y + forwardDir.y * 55.0f };
        currentAttack.damage = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;
        currentAttack.range = 105.0f;
        currentAttack.knockback = 280.0f;

        SoundFX::PlayLightAttack();
        particles.AddScreenShake(3.5f, 0.12f);
        particles.SpawnSparks(currentAttack.circleCenter, Color{ 255, 170, 60, 255 }, 12, 100.0f);
    } else if (currentWeapon == WeaponType::TRUTH_SEEKER) {
        if (comboStep == 2) {
            // Hit 2: Upward radiant slice (line/box AoE, smaller than broadsword)
            currentAttack.isLineBox = true;
            currentAttack.range = 105.0f;
            currentAttack.width = 40.0f;
            currentAttack.arcAngle = 0.0f;
            currentAttack.damage = counterBuffTimer > 0.0f ? (36.0f * 1.5f) : 36.0f;
            currentAttack.knockback = 260.0f;

            SoundFX::PlayLightAttack();
            particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 230, 90, 255 });
            particles.SpawnSparks(position, Color{ 140, 220, 255, 255 }, 12, 140.0f);
        } else {
            // Hit 1: Original golden horizontal sweep
            currentAttack.damage = counterBuffTimer > 0.0f ? (46.0f * 1.5f) : 46.0f;
            currentAttack.knockback = 200.0f;

            SoundFX::PlayLightAttack();
            particles.SpawnSlashTrail(position, forwardDir, Color{ 120, 230, 255, 255 });
        }
    } else {
        currentAttack.knockback = 130.0f;
        SoundFX::PlayLightAttack();
        particles.SpawnSlashTrail(position, forwardDir, Color{ 80, 220, 255, 255 });
    }

    currentAttack.duration = stats.lightDuration;
    currentAttack.timer = stats.lightDuration;
    currentAttack.hasHit = false;

    velocity.x += forwardDir.x * 110.0f;
    velocity.y += forwardDir.y * 110.0f;
}

void Player::TriggerHeavyAttack(ParticleSystem& particles, Vector2 targetPos) {
    if (currentWeapon == WeaponType::NONE) return;
    if (isDashing || isParrying || isHeavyWindingUp || isDaggerLunging || currentAttack.type != AttackType::NONE) return;
    if (heavyCooldown > 0.0f) return;

    WeaponStats stats = GetWeaponStats(currentWeapon);
    if (stamina < stats.heavyStamina) return;

    stamina -= stats.heavyStamina;
    heavyCooldown = stats.heavyCooldown;

    Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };

    if (currentWeapon == WeaponType::DAGGERS) {
        // Dual Daggers: Buffed high-speed piercing lunge forward!
        isDaggerLunging = true;
        daggerLungeTimer = 0.20f;
        daggerLungeDir = forwardDir;
        isHeavyWindingUp = false;
        heavyStage = 1;

        // Stage 1 Attack: Forward Piercing Lunge deals damage!
        currentAttack.type = AttackType::HEAVY;
        currentAttack.weapon = WeaponType::DAGGERS;
        currentAttack.origin = position;
        currentAttack.angle = facingAngle;
        currentAttack.range = 85.0f;
        currentAttack.width = 48.0f;
        currentAttack.isLineBox = true;
        currentAttack.arcAngle = 0.0f;
        currentAttack.damage = counterBuffTimer > 0.0f ? (42.0f * 1.5f) : 42.0f;
        currentAttack.knockback = 180.0f;
        currentAttack.duration = 0.20f;
        currentAttack.timer = 0.20f;
        currentAttack.hasHit = false;
        currentAttack.defensePierce = 0.0f;
        currentAttack.isFrontalCircle = false;
        currentAttack.hasShockwave = false;

        SoundFX::PlayDash();
        particles.AddScreenShake(2.5f, 0.10f);
        particles.SpawnSlashTrail(position, forwardDir, Color{ 80, 255, 170, 220 });
        particles.SpawnSparks(position, Color{ 80, 240, 180, 255 }, 14, 130.0f);
        return;
    }

    isHeavyWindingUp = true;
    heavyWindupTimer = stats.heavyWindup;
    heavyStage = 1;

    if (targetPos.x != 0.0f || targetPos.y != 0.0f) {
        float tdx = targetPos.x - position.x;
        float tdy = targetPos.y - position.y;
        float tdist = sqrtf(tdx * tdx + tdy * tdy);
        float maxRange = 175.0f;
        if (tdist > maxRange) {
            heavyTargetPos = { position.x + (tdx / tdist) * maxRange, position.y + (tdy / tdist) * maxRange };
        } else {
            heavyTargetPos = targetPos;
        }
    } else {
        heavyTargetPos = { position.x + forwardDir.x * 135.0f, position.y + forwardDir.y * 135.0f };
    }
}

void Player::TriggerParry(ParticleSystem& particles) {
    if (isDashing || isParrying || isHeavyWindingUp || isDaggerLunging || currentAttack.type != AttackType::NONE) return;
    if (parryCooldown > 0.0f || stamina < Config::PARRY_STAMINA_COST) return;

    stamina -= Config::PARRY_STAMINA_COST;
    isParrying = true;
    parryTimer = Config::PARRY_WINDOW;
    parryCooldown = Config::PARRY_COOLDOWN;

    SoundFX::PlayParryMiss();
}

void Player::OnParrySuccessful(ParticleSystem& particles) {
    isParrying = false;
    parryTimer = 0.0f;
    parryCooldown = 0.2f;
    counterBuffTimer = Config::COUNTER_BUFF_DURATION;
    stamina = fminf(maxStamina, stamina + 35.0f);

    SoundFX::PlayParrySuccess();
    particles.SpawnParryBurst(position);
    particles.SpawnFloatingText(position, "PERFECT PARRY!", Color{ 255, 230, 80, 255 }, 26.0f);
}

bool Player::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles, float defensePierce) {
    // Invulnerable during Dash & Dagger Lunge
    if (isDashing || isDaggerLunging) {
        particles.SpawnFloatingText(position, "EVADED", Color{ 120, 240, 255, 255 }, 18.0f);
        return false;
    }

    // Parried
    if (isParrying) {
        OnParrySuccessful(particles);
        return false;
    }

    // Apply Player Defense with diminishing returns
    float effDef = defense * fmaxf(0.0f, 1.0f - defensePierce);
    float damageFactor = 100.0f / (100.0f + effDef);
    float finalAmount = amount * damageFactor;

    health -= finalAmount;
    if (health < 0.0f) health = 0.0f;

    SoundFX::PlayHitImpact();
    particles.AddScreenShake(6.0f, 0.18f);
    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 15, 160.0f);
    particles.SpawnFloatingText(position, "-" + std::to_string((int)finalAmount), Color{ 255, 60, 60, 255 }, 22.0f);

    velocity.x += knockbackDir.x * 240.0f;
    velocity.y += knockbackDir.y * 240.0f;
    return true;
}

void Player::Heal(float amount) {
    health = fminf(maxHealth, health + amount);
}

void Player::Draw() const {
    PlayerRenderer::Draw(*this);
}

