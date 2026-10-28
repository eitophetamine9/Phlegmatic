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
    currentArmor = ArmorType::NONE;
    currentTotem = TotemType::NONE;

    maxHealth = GetMaxHealth();
    health = maxHealth;
    stamina = Config::PLAYER_MAX_STAMINA;
    maxStamina = Config::PLAYER_MAX_STAMINA;
    composure = 50.0f;
    isInPhlegmaticTrance = false;
    tranceTimer = 0.0f;
    defense = GetDefense();
    will = GetWill();

    lightCooldown = 0.0f;
    heavyCooldown = 0.0f;
    parryCooldown = 0.0f;
    parryTimer = 0.0f;
    isParrying = false;
    counterBuffTimer = 0.0f;
    potionCooldown = 0.0f;
    elementalProjectileCooldown = 0.0f;

    staggerTimer = 0.0f;
    staggerImmunityTimer = 0.0f;
    bleedTimer = 0.0f;
    bleedTickTimer = 0.0f;

    dashCooldown = 0.0f;
    dashTimer = 0.0f;
    isDashing = false;
    dashDir = { 0.0f, 0.0f };
    iFrameTimer = 0.0f;

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
    timeSinceCombat = 10.0f;
    spawnedProjectiles.clear();
}

void Player::Init(Vector2 startPos) {
    position = startPos;
    maxHealth = GetMaxHealth();
    health = maxHealth;
    stamina = maxStamina;
    composure = 50.0f;
    isInPhlegmaticTrance = false;
    tranceTimer = 0.0f;
    timeSinceCombat = 10.0f;
    defense = GetDefense();
    will = GetWill();
    velocity = { 0.0f, 0.0f };
    currentAttack.type = AttackType::NONE;
    isParrying = false;
    isDashing = false;
    isDaggerLunging = false;
    isHeavyWindingUp = false;
    heavyStage = 1;
    heavyStageTimer = 0.0f;
    counterBuffTimer = 0.0f;
    potionCooldown = 0.0f;
    staggerTimer = 0.0f;
    staggerImmunityTimer = 0.0f;
    bleedTimer = 0.0f;
    bleedTickTimer = 0.0f;
    iFrameTimer = 0.0f;
    comboStep = 1;
    comboTimer = 0.0f;
    shockwaveActive = false;
    shockwaveTimer = 0.0f;
    spawnedProjectiles.clear();
}

void Player::Revive(Vector2 startPos, float hpPercent) {
    position = startPos;
    maxHealth = GetMaxHealth();
    health = maxHealth * hpPercent;
    stamina = maxStamina;
    composure = 50.0f;
    isInPhlegmaticTrance = false;
    tranceTimer = 0.0f;
    defense = GetDefense();
    will = GetWill();
    velocity = { 0.0f, 0.0f };
    currentAttack.type = AttackType::NONE;
    isParrying = false;
    isDashing = false;
    isHeavyWindingUp = false;
    counterBuffTimer = 0.0f;
    potionCooldown = 0.0f;
    staggerTimer = 0.0f;
    staggerImmunityTimer = 0.0f;
    bleedTimer = 0.0f;
    bleedTickTimer = 0.0f;
    iFrameTimer = 0.0f;
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

void Player::SetArmor(ArmorType newArmor) {
    currentArmor = newArmor;
    inventory.UnlockArmor(newArmor);
    inventory.SetEquippedArmor(newArmor);
    maxHealth = GetMaxHealth();
    if (health > maxHealth) health = maxHealth;
    defense = GetDefense();
    will = GetWill();
    SoundFX::PlayEquipWeapon();
}

void Player::SetTotem(TotemType newTotem) {
    currentTotem = newTotem;
    inventory.UnlockTotem(newTotem);
    inventory.SetEquippedTotem(newTotem);
    defense = GetDefense();
    will = GetWill();
    SoundFX::PlayEquipWeapon();
}

float Player::GetMaxHealth() const {
    return Config::PLAYER_MAX_HEALTH + GetArmorStats(currentArmor).maxHpBonus;
}

float Player::GetDefense() const {
    float raw = 15.0f + GetArmorStats(currentArmor).defense;
    if (currentTotem == TotemType::ELEMENTAL_CORE_TOTEM) {
        raw += 25.0f; // High defense fortification
    }
    return raw * GetTotemStats(currentTotem).defenseMultiplier;
}

float Player::GetWill() const {
    float totalWill = 20.0f + GetArmorStats(currentArmor).willBonus + GetTotemStats(currentTotem).willBonus;
    return totalWill;
}

float Player::GetSpeed() const {
    return Config::PLAYER_SPEED * GetArmorStats(currentArmor).speedModifier * GetTotemStats(currentTotem).speedMultiplier;
}

float Player::GetStaminaRegenRate() const {
    float rate = Config::STAMINA_REGEN_RATE * GetArmorStats(currentArmor).staminaRegenModifier * GetTotemStats(currentTotem).staminaRegenMultiplier;
    if (IsDiscomposed()) {
        rate *= 0.80f; // -20% stamina regen speed
    } else if (HasStoicPoise()) {
        rate *= 1.20f; // +20% stamina regen speed
    }
    return rate;
}

void Player::ModifyComposure(float delta) {
    if (isInPhlegmaticTrance) return; // Locked during Trance
    composure = fmaxf(0.0f, fminf(100.0f, composure + delta));
}

bool Player::TriggerPhlegmaticTrance(ParticleSystem& particles) {
    if (isInPhlegmaticTrance || composure < 100.0f) return false;
    isInPhlegmaticTrance = true;
    tranceTimer = 4.0f;
    SoundFX::PlayTranceActivate();
    particles.AddScreenShake(7.0f, 0.25f);
    particles.SpawnFloatingText(position, "PHLEGMATIC TRANCE!", Color{ 140, 230, 255, 255 }, 28.0f);
    particles.SpawnSparks(position, Color{ 120, 220, 255, 255 }, 35, 220.0f);
    return true;
}

void Player::ApplyBleed(float duration, ParticleSystem& particles) {
    if (HasStoicPoise()) {
        particles.SpawnFloatingText(position, "BLEED RESISTED!", Color{ 140, 240, 255, 255 }, 20.0f);
        return;
    }
    bleedTimer = duration;
    bleedTickTimer = 0.8f;
    particles.SpawnFloatingText(position, "BLEEDING!", Color{ 255, 60, 80, 255 }, 22.0f);
    particles.SpawnSparks(position, Color{ 220, 30, 45, 255 }, 14, 100.0f);
}

void Player::Update(float dt, const GameMap& map, Vector2 mouseWorldPos, ParticleSystem& particles) {
    if (!IsAlive()) return;

    // Face towards mouse unless locked in dash
    if (!isDashing) {
        float dx = mouseWorldPos.x - position.x;
        float dy = mouseWorldPos.y - position.y;
        facingAngle = atan2f(dy, dx) * RAD2DEG;
    }

    // Cooldown and buff updates
    if (lightCooldown > 0.0f) lightCooldown -= dt;
    if (heavyCooldown > 0.0f) heavyCooldown -= dt;
    if (parryCooldown > 0.0f) parryCooldown -= dt;
    if (dashCooldown > 0.0f) dashCooldown -= dt;
    if (counterBuffTimer > 0.0f) counterBuffTimer -= dt;
    if (potionCooldown > 0.0f) potionCooldown -= dt;
    if (iFrameTimer > 0.0f) iFrameTimer -= dt;
    if (staggerImmunityTimer > 0.0f) staggerImmunityTimer -= dt;

    // Sanguine Bleed tick update
    if (bleedTimer > 0.0f) {
        bleedTimer -= dt;
        bleedTickTimer -= dt;
        particles.SpawnSparks(position, Color{ 200, 25, 40, 180 }, 1, 25.0f);
        if (bleedTickTimer <= 0.0f) {
            bleedTickTimer = 0.8f;
            health = fmaxf(1.0f, health - 3.0f); // Bleed tick damage
            particles.SpawnFloatingText(position, "-3 Bleed", Color{ 255, 50, 70, 240 }, 16.0f);
        }
    }

    // Stagger state update
    if (staggerTimer > 0.0f) {
        staggerTimer -= dt;
        if (staggerTimer <= 0.0f) {
            staggerTimer = 0.0f;
            staggerImmunityTimer = 1.8f; // 1.8s anti-stunlock grace window!
        }
    }

    // Elemental Core Totem projectile cooldown
    if (elementalProjectileCooldown > 0.0f) {
        elementalProjectileCooldown -= dt;
    }

    // Health Potion consumption via [1] or [H] (7-second cooldown)
    if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_H)) {
        UseHealthPotion(particles);
    }

    // Phlegmatic Trance activation via [SPACE] when composure is full (100%)
    if (IsKeyPressed(KEY_SPACE) && IsTranceReady()) {
        TriggerPhlegmaticTrance(particles);
    }

    // Phlegmatic Trance countdown & aura
    if (isInPhlegmaticTrance) {
        tranceTimer -= dt;
        particles.SpawnSparks(position, Color{ 160, 230, 255, 180 }, 1, 30.0f);
        if (tranceTimer <= 0.0f) {
            isInPhlegmaticTrance = false;
            tranceTimer = 0.0f;
            composure = 50.0f; // Resets to baseline upon trance expiry
            particles.SpawnFloatingText(position, "TRANCE EXPIRED", Color{ 180, 210, 240, 220 }, 20.0f);
        }
    }

    // Out of combat health regeneration (recovers health when off combat for 4+ seconds)
    timeSinceCombat += dt;
    if (timeSinceCombat >= 4.0f && health < GetMaxHealth()) {
        float regenRate = 8.0f; // +8 HP/sec out of combat
        health = fminf(GetMaxHealth(), health + regenRate * dt);
    }

    // Stamina regeneration
    if (!isParrying && !isHeavyWindingUp && !isDashing && staggerTimer <= 0.0f) {
        stamina = fminf(maxStamina, stamina + GetStaminaRegenRate() * dt);
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
            ModifyComposure(-12.0f); // Whiffed parry composure penalty
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
            WeaponStats dStats = GetEquippedWeaponStats();
            float spinBase = (dStats.upgradeLevel >= 2) ? 54.0f : ((dStats.upgradeLevel >= 1) ? 46.0f : 38.0f);
            currentAttack.damage = (counterBuffTimer > 0.0f ? (spinBase * 1.5f) : spinBase) * GetTotemStats(currentTotem).damageMultiplier;
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
            WeaponStats stats = GetEquippedWeaponStats();
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
                float baseDmg = (stats.upgradeLevel >= 2) ? 72.0f : ((stats.upgradeLevel >= 1) ? 62.0f : 52.0f);
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
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
                float baseDmg = (stats.upgradeLevel >= 2) ? 54.0f : ((stats.upgradeLevel >= 1) ? 48.0f : 42.0f);
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
                currentAttack.knockback = 240.0f * stats.poiseMultiplier;
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
                float baseDmg = (stats.upgradeLevel >= 1) ? 52.0f : 42.0f;
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
                currentAttack.knockback = 360.0f;
                currentAttack.duration = 0.18f;
                currentAttack.timer = 0.18f;

                // Simultaneously shoot 1 holy projectile with 10% defense pierce
                Vector2 spawnPos = { position.x + forwardDir.x * (radius + 14.0f), position.y + forwardDir.y * (radius + 14.0f) };
                Vector2 projVel = { forwardDir.x * 580.0f, forwardDir.y * 580.0f };
                spawnedProjectiles.emplace_back(spawnPos, projVel, 9.0f, stats.holyBeamDamage, ProjectileType::HOLY_BEAM, true, 0.10f);

                SoundFX::PlayHeavyAttack();
                SoundFX::PlayCrusaderClash();
                particles.AddScreenShake(6.5f, 0.20f);
                particles.SpawnSparks(position, Color{ 255, 230, 80, 255 }, 28, 200.0f);
            } else {
                currentAttack.range = stats.heavyRange;
                currentAttack.arcAngle = stats.heavyArc;
                currentAttack.damage = (counterBuffTimer > 0.0f ? (stats.heavyDamage * 1.5f) : stats.heavyDamage) * GetTotemStats(currentTotem).damageMultiplier;
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
                WeaponStats stats = GetEquippedWeaponStats();

                if (currentWeapon == WeaponType::MACE) {
                    // War Mace Heavy Stage 2: Followup full 360 AoE with 20% defense pierce!
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = false;
                    currentAttack.range = 150.0f;
                    currentAttack.arcAngle = 360.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 56.0f : ((stats.upgradeLevel >= 1) ? 50.0f : 44.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
                    currentAttack.knockback = 460.0f;
                    currentAttack.duration = 0.22f;
                    currentAttack.timer = 0.22f;
                    currentAttack.hasHit = false;
                    currentAttack.defensePierce = 0.20f + stats.defensePierceBonus; // IGNORES 20% (+10% on Tier 2) DEFENSE!
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
                    float baseDmg = (stats.upgradeLevel >= 2) ? 36.0f : ((stats.upgradeLevel >= 1) ? 32.0f : 28.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
                    currentAttack.knockback = 300.0f * stats.poiseMultiplier;
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
                    float baseDmg = (stats.upgradeLevel >= 1) ? 44.0f : 36.0f;
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
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

    // Normal movement when not dashing or dagger lunging
    if (staggerTimer > 0.0f) {
        // Stagger stumble: rapid posture deceleration
        velocity.x *= 0.70f;
        velocity.y *= 0.70f;
    } else if (!isDashing && !isDaggerLunging) {
        Vector2 moveDir = { 0.0f, 0.0f };
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    moveDir.y -= 1.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  moveDir.y += 1.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  moveDir.x -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDir.x += 1.0f;

        float currentSpeed = GetSpeed();
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

    WeaponStats stats = GetEquippedWeaponStats();
    if (!isInPhlegmaticTrance) {
        if (stamina < stats.dashStaminaCost) {
            ModifyComposure(-15.0f); // Out of stamina dash penalty
            particles.SpawnFloatingText(position, "EXHAUSTED", Color{ 240, 100, 100, 200 }, 16.0f);
            return;
        }
        stamina -= stats.dashStaminaCost;
    }

    dashCooldown = stats.dashCooldown;
    dashTimer = Config::DASH_DURATION;
    isDashing = true;
    iFrameTimer = 0.25f; // Reliable 0.25s I-Frame window!

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
    if (staggerTimer > 0.0f) return;
    if (isDashing || isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;

    timeSinceCombat = 0.0f;
    WeaponStats stats = GetEquippedWeaponStats();
    // Daggers light attack costs 0 stamina, and Phlegmatic Trance grants zero stamina cost
    if (!isInPhlegmaticTrance && currentWeapon != WeaponType::DAGGERS) {
        if (stamina < stats.lightStamina) {
            ModifyComposure(-15.0f); // Out of stamina attack penalty
            particles.SpawnFloatingText(position, "EXHAUSTED", Color{ 240, 100, 100, 200 }, 16.0f);
            return;
        }
        stamina -= stats.lightStamina;
    }
    lightCooldown = stats.lightCooldown * GetTotemStats(currentTotem).cooldownMultiplier;

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

    // Elemental Core Totem: Projectile on Light Attack (5s cooldown)
    if (currentTotem == TotemType::ELEMENTAL_CORE_TOTEM && elementalProjectileCooldown <= 0.0f) {
        Vector2 spawnPos = { position.x + forwardDir.x * (radius + 14.0f), position.y + forwardDir.y * (radius + 14.0f) };
        Vector2 projVel = { forwardDir.x * 560.0f, forwardDir.y * 560.0f };
        spawnedProjectiles.emplace_back(spawnPos, projVel, 10.0f, 32.0f, ProjectileType::ELEMENTAL_BOLT, true, 0.20f);
        elementalProjectileCooldown = 5.0f;
        particles.SpawnSparks(spawnPos, Color{ 255, 140, 50, 255 }, 18, 140.0f);
        SoundFX::PlayCrusaderClash();
    }

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
            float baseDmg = (stats.upgradeLevel >= 1) ? 30.0f : 24.0f;
            currentAttack.damage = counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg;
            currentAttack.knockback = 260.0f;

            SoundFX::PlayLightAttack();
            particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 230, 90, 255 });
            particles.SpawnSparks(position, Color{ 140, 220, 255, 255 }, 12, 140.0f);
        } else {
            // Hit 1: Original golden horizontal sweep
            currentAttack.damage = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;
            currentAttack.knockback = 200.0f;

            SoundFX::PlayLightAttack();
            particles.SpawnSlashTrail(position, forwardDir, Color{ 120, 230, 255, 255 });
        }
    } else {
        currentAttack.knockback = 130.0f;
        SoundFX::PlayLightAttack();
        particles.SpawnSlashTrail(position, forwardDir, Color{ 80, 220, 255, 255 });
    }

    currentAttack.damage *= GetTotemStats(currentTotem).damageMultiplier;

    currentAttack.duration = stats.lightDuration;
    currentAttack.timer = stats.lightDuration;
    currentAttack.hasHit = false;

    velocity.x += forwardDir.x * 110.0f;
    velocity.y += forwardDir.y * 110.0f;
}

void Player::TriggerHeavyAttack(ParticleSystem& particles, Vector2 targetPos) {
    if (currentWeapon == WeaponType::NONE) return;
    if (staggerTimer > 0.0f) return;
    if (isDashing || isParrying || isHeavyWindingUp || isDaggerLunging) return;
    if (currentAttack.type != AttackType::NONE && currentWeapon != WeaponType::DAGGERS) return;
    if (heavyCooldown > 0.0f) return;

    timeSinceCombat = 0.0f;
    WeaponStats stats = GetEquippedWeaponStats();
    if (!isInPhlegmaticTrance) {
        if (stamina < stats.heavyStamina) {
            ModifyComposure(-15.0f); // Out of stamina heavy attack penalty
            particles.SpawnFloatingText(position, "EXHAUSTED", Color{ 240, 100, 100, 200 }, 16.0f);
            return;
        }
        stamina -= stats.heavyStamina;
    }
    heavyCooldown = stats.heavyCooldown * GetTotemStats(currentTotem).cooldownMultiplier;

    Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };

    if (currentWeapon == WeaponType::DAGGERS) {
        // Dual Daggers: Balanced high-speed piercing lunge forward!
        isDaggerLunging = true;
        daggerLungeTimer = 0.20f;
        daggerLungeDir = forwardDir;
        velocity.x = forwardDir.x * 660.0f;
        velocity.y = forwardDir.y * 660.0f;
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
        float baseDmg = (stats.upgradeLevel >= 2) ? (30.0f * stats.critMultiplier) : ((stats.upgradeLevel >= 1) ? 26.0f : 22.0f);
        currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
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

float Player::GetMaxParryCooldown() const {
    return Config::PARRY_COOLDOWN;
}

void Player::TriggerParry(ParticleSystem& particles) {
    if (staggerTimer > 0.0f) return;
    if (isDashing || isParrying || isHeavyWindingUp || isDaggerLunging || currentAttack.type != AttackType::NONE) return;
    if (parryCooldown > 0.0f || stamina < Config::PARRY_STAMINA_COST) return;

    stamina -= Config::PARRY_STAMINA_COST;
    isParrying = true;
    parryTimer = Config::PARRY_WINDOW;
    parryCooldown = Config::PARRY_COOLDOWN;
    timeSinceCombat = 0.0f;

    SoundFX::PlayParryMiss();
}

void Player::OnParrySuccessful(ParticleSystem& particles) {
    isParrying = false;
    parryTimer = 0.0f;
    parryCooldown = 0.2f;
    counterBuffTimer = Config::COUNTER_BUFF_DURATION;
    stamina = fminf(maxStamina, stamina + 35.0f);
    ModifyComposure(25.0f); // Timed parry deflection composure bonus
    timeSinceCombat = 0.0f;

    SoundFX::PlayParrySuccess();
    particles.SpawnParryBurst(position);
    particles.SpawnFloatingText(position, "PERFECT PARRY!", Color{ 255, 230, 80, 255 }, 26.0f);
}

void Player::ApplyStagger(float baseStunDuration, ParticleSystem& particles) {
    if (staggerImmunityTimer > 0.0f || HasInvulnerability() || isDashing) return;
    if (isInPhlegmaticTrance || (HasStoicPoise() && isHeavyWindingUp)) return; // Stoic poise hyper-armor & trance invulnerability

    float duration = CalculateStaggerDuration(baseStunDuration, GetWill(), true);
    if (IsDiscomposed()) {
        duration *= 1.15f; // Discomposed tier: +15% stagger duration
    }
    staggerTimer = duration;
    isHeavyWindingUp = false;
    isParrying = false;
    currentAttack.type = AttackType::NONE;
    particles.SpawnFloatingText(position, "STAGGERED!", Color{ 255, 200, 60, 255 }, 24.0f);
    particles.SpawnSparks(position, Color{ 255, 200, 60, 255 }, 14, 130.0f);
    SoundFX::PlayHitImpact();
}

bool Player::UseHealthPotion(ParticleSystem& particles) {
    if (potionCooldown > 0.0f) {
        particles.SpawnFloatingText(position, "POTION ON COOLDOWN", Color{ 200, 200, 200, 220 }, 16.0f);
        return false;
    }
    if (inventory.GetHealthPotionCount() <= 0) {
        particles.SpawnFloatingText(position, "NO POTIONS", Color{ 240, 100, 100, 220 }, 16.0f);
        return false;
    }
    if (health >= GetMaxHealth()) {
        particles.SpawnFloatingText(position, "HP FULL", Color{ 140, 240, 180, 220 }, 16.0f);
        return false;
    }

    if (inventory.ConsumeHealthPotion()) {
        potionCooldown = 7.0f; // 7-second cooldown
        Heal(45.0f);
        SoundFX::PlayHeal();
        particles.SpawnSparks(position, Color{ 80, 255, 140, 255 }, 25, 140.0f);
        particles.SpawnFloatingText(position, "+45 HP", Color{ 80, 255, 140, 255 }, 24.0f);
        return true;
    }
    return false;
}

bool Player::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles, float defensePierce) {
    // Invulnerable during Dash, Dagger Lunge, or Active I-Frames
    if (isDashing || isDaggerLunging || HasInvulnerability()) {
        ModifyComposure(15.0f); // Evasive Dash through hitbox
        particles.SpawnFloatingText(position, "EVADED", Color{ 120, 240, 255, 255 }, 18.0f);
        return false;
    }

    // Parried
    if (isParrying) {
        OnParrySuccessful(particles);
        return false;
    }

    // Apply Player Defense with diminishing returns
    timeSinceCombat = 0.0f;
    float effDef = GetDefense() * fmaxf(0.0f, 1.0f - defensePierce);
    float damageFactor = 100.0f / (100.0f + effDef);
    float finalAmount = amount * damageFactor;

    health -= finalAmount;
    if (health < 0.0f) health = 0.0f;

    SoundFX::PlayHitImpact();
    particles.AddScreenShake(6.0f, 0.18f);
    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 15, 160.0f);
    particles.SpawnFloatingText(position, "-" + std::to_string((int)finalAmount), Color{ 255, 60, 60, 255 }, 22.0f);

    float knockbackSpeed = HasStoicPoise() ? 60.0f : 240.0f; // Stoic Poise resists 75% knockback
    velocity.x += knockbackDir.x * knockbackSpeed;
    velocity.y += knockbackDir.y * knockbackSpeed;
    return true;
}

void Player::Heal(float amount) {
    health = fminf(GetMaxHealth(), health + amount);
}

void Player::Draw() const {
    PlayerRenderer::Draw(*this);
}


