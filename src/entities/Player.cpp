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
    equippedHelmet = HelmetType::NONE;
    equippedChestplate = ChestplateType::NONE;
    equippedLeggings = LeggingsType::NONE;
    armorSkillCooldown = 0.0f;
    armorSkillMaxCooldown = 30.0f;
    armorSkillActiveTimer = 0.0f;
    armorSkillShieldHp = 0.0f;
    basaltShellCooldown = 0.0f;
    speedBuffTimer = 0.0f;
    speedBuffAmount = 0.0f;
    shadowDecoy = { false, { 0.0f, 0.0f }, 0.0f, 3.5f };
    basaltPillars.clear();
    pendingArmorSkillAoE = false;
    pendingAoEDamage = 0.0f;
    pendingAoERadius = 0.0f;
    pendingAoEStun = 0.0f;
    pendingAoEArmorShred = 0.0f;

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
    switch (newArmor) {
        case ArmorType::COMMONERS_SCRAP:
            equippedHelmet = HelmetType::COMMONERS_HOOD;
            equippedChestplate = ChestplateType::COMMONERS_TUNIC;
            equippedLeggings = LeggingsType::COMMONERS_BREECHES;
            break;
        case ArmorType::CRUSADER_PLATE:
            equippedHelmet = HelmetType::CRUSADER_HELM;
            equippedChestplate = ChestplateType::CRUSADER_CUIRASS;
            equippedLeggings = LeggingsType::CRUSADER_GREAVES;
            break;
        case ArmorType::VANGUARD_MAIL:
            equippedHelmet = HelmetType::VANGUARD_GREATHELM;
            equippedChestplate = ChestplateType::VANGUARD_CHEST;
            equippedLeggings = LeggingsType::VANGUARD_GREAVES;
            break;
        case ArmorType::SHADOW_LEATHER:
            equippedHelmet = HelmetType::SHADOW_COWL;
            equippedChestplate = ChestplateType::SHADOW_GARB;
            equippedLeggings = LeggingsType::SHADOW_LEGGINGS;
            break;
        case ArmorType::NONE:
        default:
            equippedHelmet = HelmetType::NONE;
            equippedChestplate = ChestplateType::NONE;
            equippedLeggings = LeggingsType::NONE;
            break;
    }
    inventory.UnlockArmor(newArmor);
    inventory.SetEquippedHelmet(equippedHelmet);
    inventory.SetEquippedChestplate(equippedChestplate);
    inventory.SetEquippedLeggings(equippedLeggings);
    inventory.SetEquippedArmor(newArmor);
    maxHealth = GetMaxHealth();
    if (health > maxHealth) health = maxHealth;
    defense = GetDefense();
    will = GetWill();
    SoundFX::PlayEquipWeapon();
}

void Player::SetHelmet(HelmetType type) {
    equippedHelmet = type;
    inventory.UnlockHelmet(type);
    inventory.SetEquippedHelmet(type);
    maxHealth = GetMaxHealth();
    if (health > maxHealth) health = maxHealth;
    defense = GetDefense();
    will = GetWill();
    SoundFX::PlayEquipWeapon();
}

void Player::SetChestplate(ChestplateType type) {
    equippedChestplate = type;
    inventory.UnlockChestplate(type);
    inventory.SetEquippedChestplate(type);
    maxHealth = GetMaxHealth();
    if (health > maxHealth) health = maxHealth;
    defense = GetDefense();
    will = GetWill();
    SoundFX::PlayEquipWeapon();
}

void Player::SetLeggings(LeggingsType type) {
    equippedLeggings = type;
    inventory.UnlockLeggings(type);
    inventory.SetEquippedLeggings(type);
    maxHealth = GetMaxHealth();
    if (health > maxHealth) health = maxHealth;
    defense = GetDefense();
    will = GetWill();
    SoundFX::PlayEquipWeapon();
}

ArmorSet Player::GetActiveArmorSet() const {
    return GetMatchingArmorSet(equippedHelmet, equippedChestplate, equippedLeggings);
}

bool Player::HasFullArmorSet(ArmorSet set) const {
    if (set == ArmorSet::NONE) return false;
    return GetActiveArmorSet() == set;
}

void Player::TriggerArmorSkill(ParticleSystem& particles) {
    ArmorSet set = GetActiveArmorSet();
    if (set == ArmorSet::NONE) return;
    if (armorSkillCooldown > 0.0f) return;

    ArmorSetInfo info = GetArmorSetInfo(set);
    armorSkillMaxCooldown = info.skillCooldown;
    armorSkillCooldown = info.skillCooldown;

    switch (set) {
        case ArmorSet::COMMONERS_SCRAP: {
            speedBuffTimer = 4.0f;
            speedBuffAmount = 0.35f;
            pendingArmorSkillAoE = true;
            pendingAoEDamage = 15.0f;
            pendingAoERadius = 180.0f;
            pendingAoEStun = 1.5f;
            pendingAoEArmorShred = 0.0f;
            SoundFX::PlayParrySuccess();
            particles.AddScreenShake(4.0f, 0.2f);
            particles.SpawnFloatingText(position, "DUST BURST!", Color{ 220, 190, 140, 255 }, 24.0f);
            particles.SpawnSparks(position, Color{ 200, 170, 110, 255 }, 30, 200.0f);
            break;
        }
        case ArmorSet::CRUSADER_PLATE: {
            armorSkillShieldHp = 60.0f;
            armorSkillActiveTimer = 6.0f;
            SoundFX::PlayTranceActivate();
            particles.AddScreenShake(6.0f, 0.25f);
            particles.SpawnFloatingText(position, "DAWNFIRE AEGIS!", Color{ 255, 230, 90, 255 }, 26.0f);
            particles.SpawnSparks(position, Color{ 255, 225, 70, 255 }, 35, 220.0f);
            break;
        }
        case ArmorSet::VANGUARD_BASTION: {
            pendingArmorSkillAoE = true;
            pendingAoEDamage = 50.0f;
            pendingAoERadius = 200.0f;
            pendingAoEStun = 1.8f;
            pendingAoEArmorShred = 0.25f;
            SoundFX::PlayParrySuccess();
            particles.AddScreenShake(10.0f, 0.35f);
            particles.SpawnFloatingText(position, "SEISMIC QUAKE SLAM!", Color{ 255, 120, 60, 255 }, 26.0f);
            particles.SpawnSparks(position, Color{ 200, 80, 50, 255 }, 40, 260.0f);
            break;
        }
        case ArmorSet::SHADOWSTRIDER: {
            shadowDecoy.active = true;
            shadowDecoy.position = position;
            shadowDecoy.timer = 3.5f;
            shadowDecoy.duration = 3.5f;
            iFrameTimer = 0.5f;
            Vector2 forward = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
            position = Vector2Add(position, Vector2Scale(forward, 70.0f));
            SoundFX::PlayDash();
            particles.SpawnFloatingText(position, "SHADOW DECOY!", Color{ 160, 100, 255, 255 }, 24.0f);
            particles.SpawnSparks(shadowDecoy.position, Color{ 140, 80, 240, 255 }, 35, 180.0f);
            break;
        }
        case ArmorSet::FORGED_SCRAP_IRON: {
            armorSkillActiveTimer = 6.0f;
            SoundFX::PlayEquipWeapon();
            particles.AddScreenShake(5.0f, 0.2f);
            particles.SpawnFloatingText(position, "IRONCLAD SPIKE WALL!", Color{ 230, 200, 140, 255 }, 24.0f);
            particles.SpawnSparks(position, Color{ 200, 210, 230, 255 }, 25, 200.0f);
            break;
        }
        case ArmorSet::BASALT_GOLEM: {
            basaltPillars.clear();
            Vector2 forward = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
            Vector2 right = { -forward.y, forward.x };
            Vector2 centerPillar = Vector2Add(position, Vector2Scale(forward, 65.0f));
            Vector2 leftPillar = Vector2Add(centerPillar, Vector2Scale(right, -45.0f));
            Vector2 rightPillar = Vector2Add(centerPillar, Vector2Scale(right, 45.0f));

            basaltPillars.push_back({ true, leftPillar, 24.0f, 6.0f });
            basaltPillars.push_back({ true, centerPillar, 26.0f, 6.0f });
            basaltPillars.push_back({ true, rightPillar, 24.0f, 6.0f });

            pendingArmorSkillAoE = true;
            pendingAoEDamage = 55.0f;
            pendingAoERadius = 160.0f;
            pendingAoEStun = 1.2f;
            pendingAoEArmorShred = 0.15f;

            SoundFX::PlayParrySuccess();
            particles.AddScreenShake(9.0f, 0.3f);
            particles.SpawnFloatingText(position, "BASALT SPIRE WALL!", Color{ 255, 160, 50, 255 }, 26.0f);
            particles.SpawnSparks(centerPillar, Color{ 245, 140, 40, 255 }, 40, 220.0f);
            break;
        }
        default:
            break;
    }
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
    float bonus = GetHelmetStats(equippedHelmet).maxHpBonus
                + GetChestplateStats(equippedChestplate).maxHpBonus
                + GetLeggingsStats(equippedLeggings).maxHpBonus;
    if (HasFullArmorSet(ArmorSet::VANGUARD_BASTION)) {
        bonus += 40.0f;
    }
    return Config::PLAYER_MAX_HEALTH + bonus;
}

float Player::GetDefense() const {
    float raw = 15.0f
              + GetHelmetStats(equippedHelmet).defense
              + GetChestplateStats(equippedChestplate).defense
              + GetLeggingsStats(equippedLeggings).defense;
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE)) {
        raw *= 1.20f;
    } else if (HasFullArmorSet(ArmorSet::FORGED_SCRAP_IRON)) {
        raw += 18.0f;
    } else if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
        raw += 35.0f;
    }
    if (currentTotem == TotemType::ELEMENTAL_CORE_TOTEM) {
        raw += 25.0f;
    }
    return raw * GetTotemStats(currentTotem).defenseMultiplier;
}

float Player::GetWill() const {
    float totalWill = 20.0f
                    + GetHelmetStats(equippedHelmet).willBonus
                    + GetChestplateStats(equippedChestplate).willBonus
                    + GetLeggingsStats(equippedLeggings).willBonus
                    + GetTotemStats(currentTotem).willBonus;
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE)) {
        totalWill += 25.0f;
    }
    return totalWill;
}

float Player::GetSpeed() const {
    float speedMult = GetHelmetStats(equippedHelmet).speedModifier
                    * GetChestplateStats(equippedChestplate).speedModifier
                    * GetLeggingsStats(equippedLeggings).speedModifier
                    * GetTotemStats(currentTotem).speedMultiplier;
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER)) {
        speedMult *= 1.15f;
    } else if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
        speedMult *= 0.90f;
    }
    float base = Config::PLAYER_SPEED * speedMult;
    if (speedBuffTimer > 0.0f) {
        base *= (1.0f + speedBuffAmount);
    }
    return base;
}

float Player::GetStaminaRegenRate() const {
    float rate = Config::STAMINA_REGEN_RATE
               * GetHelmetStats(equippedHelmet).staminaRegenModifier
               * GetChestplateStats(equippedChestplate).staminaRegenModifier
               * GetLeggingsStats(equippedLeggings).staminaRegenModifier
               * GetTotemStats(currentTotem).staminaRegenMultiplier;
    if (HasFullArmorSet(ArmorSet::COMMONERS_SCRAP)) {
        rate *= 1.15f;
    }
    if (IsDiscomposed()) {
        rate *= 0.80f;
    } else if (HasStoicPoise()) {
        rate *= 1.20f;
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



    // Armor Skill activation via [V]
    if (IsKeyPressed(KEY_V)) {
        TriggerArmorSkill(particles);
    }

    if (armorSkillCooldown > 0.0f) {
        armorSkillCooldown -= dt;
        if (armorSkillCooldown < 0.0f) armorSkillCooldown = 0.0f;
    }

    if (armorSkillActiveTimer > 0.0f) {
        armorSkillActiveTimer -= dt;
        if (armorSkillActiveTimer <= 0.0f) {
            armorSkillActiveTimer = 0.0f;
            if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE) && armorSkillShieldHp > 0.0f) {
                pendingArmorSkillAoE = true;
                pendingAoEDamage = 45.0f;
                pendingAoERadius = 180.0f;
                pendingAoEStun = 1.5f;
                particles.SpawnFloatingText(position, "DAWNFIRE DETONATION!", Color{ 255, 220, 80, 255 }, 26.0f);
                particles.SpawnSparks(position, Color{ 255, 230, 90, 255 }, 30, 240.0f);
                armorSkillShieldHp = 0.0f;
            }
        }
    }

    if (basaltShellCooldown > 0.0f) {
        basaltShellCooldown -= dt;
        if (basaltShellCooldown < 0.0f) basaltShellCooldown = 0.0f;
    }

    if (speedBuffTimer > 0.0f) {
        speedBuffTimer -= dt;
        if (speedBuffTimer <= 0.0f) {
            speedBuffTimer = 0.0f;
            speedBuffAmount = 0.0f;
        }
    }

    if (shadowDecoy.active) {
        shadowDecoy.timer -= dt;
        particles.SpawnSparks(shadowDecoy.position, Color{ 140, 80, 220, 140 }, 1, 30.0f);
        if (shadowDecoy.timer <= 0.0f) {
            shadowDecoy.active = false;
            pendingArmorSkillAoE = true;
            pendingAoEDamage = 40.0f;
            pendingAoERadius = 160.0f;
            pendingAoEStun = 1.0f;
            particles.SpawnFloatingText(shadowDecoy.position, "DECOY BURST!", Color{ 180, 110, 255, 255 }, 24.0f);
            particles.SpawnSparks(shadowDecoy.position, Color{ 160, 90, 240, 255 }, 30, 220.0f);
            SoundFX::PlayHitImpact();
        }
    }

    for (auto it = basaltPillars.begin(); it != basaltPillars.end(); ) {
        if (it->active) {
            it->timer -= dt;
            if (it->timer <= 0.0f) {
                particles.SpawnSparks(it->position, Color{ 140, 130, 120, 200 }, 10, 80.0f);
                it = basaltPillars.erase(it);
                continue;
            }
        }
        ++it;
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

        float currentDashSpeed = Config::DASH_SPEED;
        if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER)) {
            currentDashSpeed *= 1.25f;
        }
        velocity.x = dashDir.x * currentDashSpeed;
        velocity.y = dashDir.y * currentDashSpeed;



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

            } else if (currentWeapon == WeaponType::HALBERD) {
                // Royal Halberd Heavy Stage 1: Long Lunge Thrust with Invulnerability Frames (i-frames)
                heavyStage = 1;
                halberdStabCount = 0;
                iFrameTimer = 0.22f; // Invulnerability window during forward lunge!

                currentAttack.isLineBox = true;
                currentAttack.range = 175.0f;
                currentAttack.width = 52.0f;
                currentAttack.arcAngle = 0.0f;

                float baseDmg = (stats.upgradeLevel >= 2) ? 36.0f : ((stats.upgradeLevel >= 1) ? 32.0f : 28.0f);
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
                currentAttack.knockback = 340.0f * stats.poiseMultiplier;
                currentAttack.defensePierce = 0.15f + stats.defensePierceBonus;
                currentAttack.duration = 0.20f;
                currentAttack.timer = 0.20f;
                currentAttack.hasHit = false;

                velocity.x += forwardDir.x * 320.0f;
                velocity.y += forwardDir.y * 320.0f;

                SoundFX::PlayHeavyAttack();
                particles.AddScreenShake(5.5f, 0.18f);
                particles.SpawnSlashTrail(position, forwardDir, Color{ 140, 210, 255, 255 });
                particles.SpawnSparks(position, Color{ 180, 230, 255, 255 }, 18, 160.0f);
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

                } else if (currentWeapon == WeaponType::HALBERD) {
                    // Royal Halberd Heavy Stage 2: Wide 220-degree sweeping cleave AoE
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isLineBox = false;
                    currentAttack.range = 155.0f;
                    currentAttack.arcAngle = 220.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 38.0f : ((stats.upgradeLevel >= 1) ? 33.0f : 28.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
                    currentAttack.knockback = 320.0f * stats.poiseMultiplier;
                    currentAttack.defensePierce = 0.10f + stats.defensePierceBonus;
                    currentAttack.duration = 0.20f;
                    currentAttack.timer = 0.20f;
                    currentAttack.hasHit = false;

                    velocity.x += forwardDir.x * 140.0f;
                    velocity.y += forwardDir.y * 140.0f;

                    SoundFX::PlayCrusaderClash();
                    particles.AddScreenShake(6.5f, 0.18f);
                    particles.SpawnSlashTrail(position, forwardDir, Color{ 160, 215, 255, 255 });
                    particles.SpawnSparks(position, Color{ 160, 210, 255, 255 }, 22, 170.0f);
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

            } else if (currentAttack.type == AttackType::HEAVY && heavyStage == 2 && currentWeapon == WeaponType::HALBERD) {
                // Royal Halberd Heavy Stage 3: Flurry of 3 consecutive rapid stabs (each deals 30% of base weapon damage)
                heavyStage = 3;
                halberdStabCount = 1;
                Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
                WeaponStats stats = GetEquippedWeaponStats();

                currentAttack.type = AttackType::HEAVY;
                currentAttack.isLineBox = true;
                currentAttack.range = 165.0f;
                currentAttack.width = 46.0f;
                currentAttack.arcAngle = 0.0f;

                float baseDmg = stats.lightDamage * 0.30f; // 30% of base weapon damage!
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
                currentAttack.knockback = 180.0f;
                currentAttack.defensePierce = 0.20f + stats.defensePierceBonus;
                currentAttack.duration = 0.09f;
                currentAttack.timer = 0.09f;
                currentAttack.hasHit = false;

                velocity.x += forwardDir.x * 110.0f;
                velocity.y += forwardDir.y * 110.0f;

                SoundFX::PlayLightAttack();
                particles.SpawnSparks(Vector2{ position.x + forwardDir.x * 90.0f, position.y + forwardDir.y * 90.0f }, Color{ 220, 245, 255, 255 }, 10, 130.0f);
            } else if (currentAttack.type == AttackType::HEAVY && heavyStage == 3 && currentWeapon == WeaponType::HALBERD) {
                if (halberdStabCount < 3) {
                    halberdStabCount++;
                    Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
                    WeaponStats stats = GetEquippedWeaponStats();

                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isLineBox = true;
                    currentAttack.range = 165.0f;
                    currentAttack.width = 46.0f;
                    currentAttack.arcAngle = 0.0f;

                    float baseDmg = stats.lightDamage * 0.30f; // 30% of base weapon damage!
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemStats(currentTotem).damageMultiplier;
                    currentAttack.knockback = 180.0f;
                    currentAttack.defensePierce = 0.20f + stats.defensePierceBonus;
                    currentAttack.duration = 0.09f;
                    currentAttack.timer = 0.09f;
                    currentAttack.hasHit = false;

                    velocity.x += forwardDir.x * 110.0f;
                    velocity.y += forwardDir.y * 110.0f;

                    SoundFX::PlayLightAttack();
                    particles.SpawnSparks(Vector2{ position.x + forwardDir.x * 100.0f, position.y + forwardDir.y * 100.0f }, Color{ 220, 245, 255, 255 }, 12, 140.0f);
                } else {
                    currentAttack.type = AttackType::NONE;
                    heavyStage = 1;
                    halberdStabCount = 0;
                }
            } else {
                currentAttack.type = AttackType::NONE;
                heavyStage = 1;
                halberdStabCount = 0;
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
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER)) {
        dashCooldown *= 0.65f;
    }

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

    } else if (currentWeapon == WeaponType::HALBERD) {

        // Royal Halberd Light: Extended armor-piercing linear thrust (LineBox)

        currentAttack.isLineBox = true;

        currentAttack.range = stats.lightRange;

        currentAttack.width = 44.0f;

        currentAttack.arcAngle = 0.0f;

        currentAttack.damage = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;

        currentAttack.knockback = 220.0f;

        currentAttack.defensePierce = 0.10f + stats.defensePierceBonus;



        SoundFX::PlayLightAttack();

        particles.SpawnSlashTrail(position, forwardDir, Color{ 170, 215, 255, 255 });

        particles.SpawnSparks(Vector2{ position.x + forwardDir.x * 60.0f, position.y + forwardDir.y * 60.0f }, Color{ 220, 240, 255, 255 }, 12, 120.0f);

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

    // Basalt Golem: 1 free heavy hit absorbed every 20s
    if (HasFullArmorSet(ArmorSet::BASALT_GOLEM) && basaltShellCooldown <= 0.0f) {
        basaltShellCooldown = 20.0f;
        particles.SpawnFloatingText(position, "PRIMORDIAL CARAPACE ABSORBED!", Color{ 245, 175, 45, 255 }, 24.0f);
        particles.SpawnSparks(position, Color{ 180, 160, 140, 255 }, 25, 180.0f);
        SoundFX::PlayParrySuccess();
        return false;
    }



    // Apply Player Defense with diminishing returns

    timeSinceCombat = 0.0f;

    float effDef = GetDefense() * fmaxf(0.0f, 1.0f - defensePierce);

    float damageFactor = 100.0f / (100.0f + effDef);

    float finalAmount = amount * damageFactor;

    // Crusader Plate Dawnfire Aegis Shield
    if (armorSkillShieldHp > 0.0f) {
        float absorbed = fminf(armorSkillShieldHp, finalAmount);
        armorSkillShieldHp -= absorbed;
        finalAmount -= absorbed;
        particles.SpawnFloatingText(position, "AEGIS SHIELD -" + std::to_string((int)absorbed), Color{ 255, 230, 90, 255 }, 22.0f);
        if (armorSkillShieldHp <= 0.0f) {
            armorSkillShieldHp = 0.0f;
            armorSkillActiveTimer = 0.0f;
            pendingArmorSkillAoE = true;
            pendingAoEDamage = 45.0f;
            pendingAoERadius = 180.0f;
            pendingAoEStun = 1.5f;
            particles.SpawnFloatingText(position, "DAWNFIRE DETONATION!", Color{ 255, 220, 80, 255 }, 26.0f);
            particles.SpawnSparks(position, Color{ 255, 230, 90, 255 }, 30, 240.0f);
        }
        if (finalAmount <= 0.0f) return false;
    }

    // Forged Scrap Iron: reflect 40% damage during Ironclad Spike Wall
    if (armorSkillActiveTimer > 0.0f && HasFullArmorSet(ArmorSet::FORGED_SCRAP_IRON)) {
        particles.SpawnFloatingText(position, "SPIKE REFLECT 40%!", Color{ 240, 210, 150, 255 }, 20.0f);
        particles.SpawnSparks(position, Color{ 230, 220, 210, 255 }, 15, 150.0f);
    }

    health -= finalAmount;

    if (health < 0.0f) health = 0.0f;



    SoundFX::PlayHitImpact();

    particles.AddScreenShake(6.0f, 0.18f);

    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 15, 160.0f);

    particles.SpawnFloatingText(position, "-" + std::to_string((int)finalAmount), Color{ 255, 60, 60, 255 }, 22.0f);



    float knockbackSpeed = (HasStoicPoise() || (isHeavyWindingUp && HasFullArmorSet(ArmorSet::VANGUARD_BASTION))) ? 0.0f : (HasStoicPoise() ? 60.0f : 240.0f);

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





