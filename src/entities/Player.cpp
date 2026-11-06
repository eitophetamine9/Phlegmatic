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
    willOfAtlasTimer = 0.0f;
    willOfAtlasCooldown = 0.0f;



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
    RelocateCompanionToPlayer();

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
    willOfAtlasTimer = 0.0f;
    willOfAtlasCooldown = 0.0f;

    comboStep = 1;

    comboTimer = 0.0f;

    shockwaveActive = false;

    shockwaveTimer = 0.0f;

    spawnedProjectiles.clear();

}



void Player::Revive(Vector2 startPos, float hpPercent) {

    position = startPos;
    RelocateCompanionToPlayer();

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
    willOfAtlasTimer = 0.0f;
    willOfAtlasCooldown = 0.0f;

    comboStep = 1;

    comboTimer = 0.0f;

    shockwaveActive = false;

    shockwaveTimer = 0.0f;

}



void Player::SetWeapon(WeaponType newWeapon) {
    currentWeapon = newWeapon;
    daggerLightAttackCounter = 0;

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
            pendingAoEDamage = 65.0f;
            pendingAoERadius = 220.0f;
            pendingAoEStun = 1.8f;
            pendingAoEArmorShred = 0.30f;
            // Unstoppable phalanx forward rush
            Vector2 forward = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
            position = Vector2Add(position, Vector2Scale(forward, 180.0f));
            armorSkillShieldHp = 50.0f; // 4s Fortification barrier
            SoundFX::PlayParrySuccess();
            particles.AddScreenShake(10.0f, 0.35f);
            particles.SpawnFloatingText(position, "PHALANX VANGUARD CHARGE!", Color{ 255, 120, 60, 255 }, 26.0f);
            particles.SpawnSparks(position, Color{ 245, 80, 50, 255 }, 40, 260.0f);
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

void Player::RelocateCompanionToPlayer() {
    float sideAngle = (facingAngle - 90.0f) * DEG2RAD;
    Vector2 offset = { cosf(sideAngle) * 36.0f, sinf(sideAngle) * 36.0f };
    companion.position = Vector2Add(position, offset);
    companion.attackCooldown = 0.0f;
    if (currentTotem == TotemType::TOTEM_MIST) {
        mistFairy.active = true;
        mistFairy.position = { position.x - 32.0f, position.y - 20.0f };
    }
}

void Player::SetTotem(TotemType newTotem) {
    currentTotem = newTotem;
    inventory.UnlockTotem(newTotem);
    inventory.SetEquippedTotem(newTotem);
    if (newTotem == TotemType::TOTEM_MIST) {
        mistFairy.active = true;
        mistFairy.healOrbTimer = 14.0f; mistFairy.flutterTime = 0.0f;
        mistFairy.position = { position.x - 30.0f, position.y - 30.0f };
    } else {
        mistFairy.active = false;
    }
    if (newTotem == TotemType::ELEMENTAL_CORE_TOTEM) {
        if (!companion.active && companion.respawnTimer <= 0.0f) {
            companion.active = true;
            companion.health = companion.maxHealth;
            RelocateCompanionToPlayer();
        }
    } else {
        companion.active = false;
    }
    defense = GetDefense();
    will = GetWill();
    SoundFX::PlayEquipWeapon();
}

float Player::GetTotemDamageMultiplier() const {
    if (currentTotem == TotemType::TOTEM_INSANITY) {
        float maxHp = GetMaxHealth();
        if (maxHp > 0.0f && (health / maxHp) <= 0.40f) {
            return 2.35f; // Low-health blood frenzy: surges from +65% (1.65f) to +135% (2.35f) more damage
        }
        return 1.65f; // Baseline: +65% overall damage
    }
    return GetTotemStats(currentTotem).damageMultiplier;
}

float Player::GetMaxHealth() const {
    float bonus = GetHelmetStats(equippedHelmet).maxHpBonus
                + GetChestplateStats(equippedChestplate).maxHpBonus
                + GetLeggingsStats(equippedLeggings).maxHpBonus
                + GetTotemStats(currentTotem).maxHpBonus;
    if (HasFullArmorSet(ArmorSet::VANGUARD_BASTION)) {
        bonus += 50.0f; // +50 Max HP
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
        speedMult *= 1.25f; // +25% Move Speed
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



    // Elemental Core Totem & Companion Update
    if (elementalProjectileCooldown > 0.0f) {
        elementalProjectileCooldown -= dt;
    }

    if (currentTotem == TotemType::ELEMENTAL_CORE_TOTEM) {
        if (!companion.active) {
            companion.respawnTimer -= dt;
            if (companion.respawnTimer <= 0.0f) {
                companion.active = true;
                companion.health = companion.maxHealth;
                companion.respawnTimer = 0.0f;
                RelocateCompanionToPlayer();
                particles.SpawnFloatingText(position, "MINI GOLEM RECONSTRUCTED!", Color{ 120, 245, 180, 255 }, 24.0f);
                particles.SpawnSparks(companion.position, Color{ 255, 170, 50, 255 }, 25, 180.0f);
                SoundFX::PlayParrySuccess();
            }
        }
    } else {
        companion.active = false;
    }

    // Totem of Mist & Mist Fairy Companion Update
    if (currentTotem == TotemType::TOTEM_MIST) {
        if (!mistFairy.active) {
            mistFairy.active = true;
            mistFairy.healOrbTimer = 14.0f;
            mistFairy.flutterTime = 0.0f;
            mistFairy.position = { position.x - 32.0f, position.y - 20.0f };
        }

        mistFairy.flutterTime += dt;

        // Dynamic hovering anchor orbiting near player
        float hoverAngle = mistFairy.flutterTime * 2.2f;
        Vector2 targetFairyPos = {
            position.x + cosf(hoverAngle) * 36.0f,
            position.y - 22.0f + sinf(hoverAngle * 1.5f) * 12.0f
        };

        float distToPlayer = Vector2Distance(mistFairy.position, position);
        if (distToPlayer > 260.0f || (mistFairy.position.x == 0.0f && mistFairy.position.y == 0.0f)) {
            mistFairy.position = targetFairyPos;
        } else {
            mistFairy.position = Vector2Lerp(mistFairy.position, targetFairyPos, fminf(1.0f, 6.5f * dt));
        }

        if (fmodf(mistFairy.flutterTime, 0.22f) < dt) {
            particles.SpawnSparks(mistFairy.position, Color{ 110, 245, 225, 150 }, 1, 15.0f);
        }

        mistFairy.healOrbTimer -= dt;
        if (mistFairy.healOrbTimer <= 0.0f) {
            mistFairy.healOrbTimer = 14.0f;
            MistHealingOrb orb;
            orb.position = mistFairy.position;
            orb.lifetime = 20.0f;
            orb.healAmount = 30.0f;
            orb.active = true;
            mistHealingOrbs.push_back(orb);
            particles.SpawnSparks(orb.position, Color{ 90, 255, 200, 255 }, 18, 90.0f);
            particles.SpawnFloatingText(orb.position, "MIST HEALING ORB!", Color{ 110, 255, 215, 255 }, 20.0f);
        }
    } else {
        mistFairy.active = false;
    }

    // Active Mist Healing Orbs decay & pickup collision
    for (auto it = mistHealingOrbs.begin(); it != mistHealingOrbs.end(); ) {
        if (it->active) {
            it->lifetime -= dt;
            if (it->lifetime <= 0.0f) {
                it->active = false;
                it = mistHealingOrbs.erase(it);
                continue;
            }
            float d = Vector2Distance(position, it->position);
            if (d <= radius + 36.0f) {
                Heal(it->healAmount);
                particles.SpawnFloatingText(position, "+30 HP (Mist Orb)", Color{ 90, 255, 180, 255 }, 22.0f);
                particles.SpawnSparks(it->position, Color{ 120, 255, 220, 255 }, 20, 140.0f);
                SoundFX::PlayParrySuccess();
                it->active = false;
                it = mistHealingOrbs.erase(it);
                continue;
            }
        }
        ++it;
    }

    // Crusader's Totem & Will of Atlas Update
    if (willOfAtlasCooldown > 0.0f) {
        willOfAtlasCooldown -= dt;
    }
    if (willOfAtlasTimer > 0.0f) {
        willOfAtlasTimer -= dt;
        if (fmodf(willOfAtlasTimer, 0.16f) < dt) {
            particles.SpawnSparks(position, Color{ 255, 220, 80, 220 }, 2, 45.0f);
        }
    } else if (currentTotem == TotemType::CRUSADER_TOTEM && health > 0.0f && health <= maxHealth * 0.20f && willOfAtlasCooldown <= 0.0f) {
        willOfAtlasTimer = 4.0f;
        willOfAtlasCooldown = 60.0f;
        SoundFX::PlayParrySuccess();
        SoundFX::PlayCrusaderClash();
        particles.SpawnFloatingText(position, "WILL OF ATLAS! (4s INVULNERABLE)", Color{ 255, 220, 80, 255 }, 24.0f);
        particles.SpawnSparks(position, Color{ 255, 230, 90, 255 }, 35, 180.0f);
        particles.AddScreenShake(5.0f, 0.25f);
    }

    // Shadowstrider Smoke Mist Zones duration decay (7-second lifetime)
    for (auto it = smokeMistZones.begin(); it != smokeMistZones.end(); ) {
        if (it->active) {
            it->duration -= dt;
            if (it->duration <= 0.0f) {
                it->active = false;
                it = smokeMistZones.erase(it);
                continue;
            }
        }
        ++it;
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
            currentDashSpeed *= 1.10f; // Rebalanced dash distance
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

            currentAttack.damage = (counterBuffTimer > 0.0f ? (spinBase * 1.5f) : spinBase) * GetTotemDamageMultiplier();

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

                float baseDmg = (stats.upgradeLevel >= 2) ? 58.0f : ((stats.upgradeLevel >= 1) ? 50.0f : 42.0f);

                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();

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

                float baseDmg = (stats.upgradeLevel >= 2) ? 40.0f : ((stats.upgradeLevel >= 1) ? 35.0f : 30.0f);

                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();

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
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
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

                float baseDmg = (stats.upgradeLevel >= 1) ? 42.0f : 34.0f;

                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();

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

            } else if (currentWeapon == WeaponType::SUN_CLAYMORE) {

                currentAttack.range = stats.heavyRange;
                currentAttack.arcAngle = stats.heavyArc;
                float baseDmg = stats.heavyDamage;
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                currentAttack.knockback = 420.0f * stats.poiseMultiplier;
                currentAttack.defensePierce = stats.defensePierceBonus;
                currentAttack.duration = stats.heavyDuration;
                currentAttack.timer = stats.heavyDuration;
                currentAttack.hasShockwave = true;
                currentAttack.shockwavePos = { position.x + forwardDir.x * 60.0f, position.y + forwardDir.y * 60.0f };
                currentAttack.shockwaveRadius = 120.0f;

                SoundFX::PlayHeavyAttack();
                SoundFX::PlayCrusaderClash();
                particles.AddScreenShake(8.0f, 0.25f);
                particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 175, 40, 255 });
                particles.SpawnSparks(position, Color{ 255, 215, 80, 255 }, 32, 220.0f);
                if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
                    currentAttack.hasShockwave = true;
                    currentAttack.shockwaveRadius = 150.0f;
                    currentAttack.damage += 30.0f;
                    particles.SpawnFloatingText(position, "EARTH RUPTURE +30", Color{ 255, 180, 60, 255 }, 22.0f);
                }
                velocity.x += forwardDir.x * 120.0f;
                velocity.y += forwardDir.y * 120.0f;

            } else if (currentWeapon == WeaponType::WINDRUNNER_RECURVE) {
                Vector2 spawnPos = { position.x + forwardDir.x * (radius + 18.0f), position.y + forwardDir.y * (radius + 18.0f) };
                Vector2 projVel = { forwardDir.x * 850.0f, forwardDir.y * 850.0f };
                float baseDmg = stats.heavyDamage;
                float finalDmg = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();

                Projectile proj(spawnPos, projVel, 13.0f, finalDmg, ProjectileType::GALE_ARROW, true, 0.50f);
                proj.SetPiercing(true);
                spawnedProjectiles.push_back(proj);

                velocity.x -= forwardDir.x * 160.0f;
                velocity.y -= forwardDir.y * 160.0f;

                SoundFX::PlayHeavyAttack();
                particles.AddScreenShake(6.0f, 0.20f);
                particles.SpawnSlashTrail(position, forwardDir, Color{ 80, 240, 210, 255 });
                particles.SpawnSparks(spawnPos, Color{ 120, 255, 230, 255 }, 24, 180.0f);
            } else if (currentWeapon == WeaponType::STORM_GLAIVE) {

                currentAttack.range = stats.heavyRange;
                currentAttack.arcAngle = stats.heavyArc;
                float baseDmg = stats.heavyDamage;
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                currentAttack.knockback = 300.0f * stats.poiseMultiplier;
                currentAttack.defensePierce = stats.defensePierceBonus;
                currentAttack.duration = stats.heavyDuration;
                currentAttack.timer = stats.heavyDuration;

                SoundFX::PlayHeavyAttack();
                particles.AddScreenShake(5.0f, 0.16f);
                particles.SpawnSlashTrail(position, forwardDir, Color{ 100, 245, 230, 255 });
                particles.SpawnSparks(position, Color{ 140, 255, 240, 255 }, 24, 180.0f);
                velocity.x += forwardDir.x * 240.0f;
                velocity.y += forwardDir.y * 240.0f;

            } else {

                currentAttack.range = stats.heavyRange;

                currentAttack.arcAngle = stats.heavyArc;

                currentAttack.damage = (counterBuffTimer > 0.0f ? (stats.heavyDamage * 1.5f) : stats.heavyDamage) * GetTotemDamageMultiplier();

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

            // Check for Stage 2 & Stage 3 follow-ups
            if (currentAttack.type == AttackType::HEAVY && heavyStage == 1) {
                Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
                WeaponStats stats = GetEquippedWeaponStats();

                if (currentWeapon == WeaponType::MACE) {
                    // War Mace Heavy Stage 2: Seismic Tremor (360 AoE with 25% defense pierce and knockdown stagger!)
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = false;
                    currentAttack.range = 150.0f;
                    currentAttack.arcAngle = 360.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 50.0f : ((stats.upgradeLevel >= 1) ? 42.0f : 36.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 460.0f;
                    currentAttack.duration = 0.22f;
                    currentAttack.timer = 0.22f;
                    currentAttack.hasHit = false;
                    currentAttack.defensePierce = 0.25f + stats.defensePierceBonus;
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
                    // Crusader Blade Heavy Stage 2: Shock Thrust (linear piercing box interrupting enemies)
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.range = 150.0f;
                    currentAttack.width = 48.0f;
                    currentAttack.isLineBox = true;
                    currentAttack.arcAngle = 0.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 32.0f : ((stats.upgradeLevel >= 1) ? 28.0f : 24.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 300.0f * stats.poiseMultiplier;
                    currentAttack.duration = 0.18f;
                    currentAttack.timer = 0.18f;
                    currentAttack.hasHit = false;

                    velocity.x += forwardDir.x * 220.0f;
                    velocity.y += forwardDir.y * 220.0f;

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
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
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
                    // Truth Seeker Heavy Stage 2: Sacred Judgment Pillar (100% defense pierce beam + debuff cleanse)
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = true;
                    currentAttack.circleRadius = 90.0f;
                    currentAttack.circleCenter = { position.x + forwardDir.x * 90.0f, position.y + forwardDir.y * 90.0f };
                    currentAttack.range = 170.0f;
                    currentAttack.arcAngle = 0.0f;
                    currentAttack.isLineBox = false;
                    float baseDmg = (stats.upgradeLevel >= 1) ? 46.0f : 38.0f;
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 380.0f;
                    currentAttack.defensePierce = 1.0f; // 100% DEFENSE PIERCE!
                    currentAttack.duration = 0.22f;
                    currentAttack.timer = 0.22f;
                    currentAttack.hasHit = false;

                    CleanseDebuffs(particles); // Cleanses player debuffs!

                    SoundFX::PlayTranceActivate();
                    particles.AddScreenShake(8.0f, 0.26f);
                    particles.SpawnSparks(currentAttack.circleCenter, Color{ 255, 240, 100, 255 }, 35, 220.0f);
                } else {
                    currentAttack.type = AttackType::NONE;
                    heavyStage = 1;
                }
            } else if (currentAttack.type == AttackType::HEAVY && heavyStage == 2) {
                Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
                WeaponStats stats = GetEquippedWeaponStats();

                if (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS) {
                    // Dual Daggers Heavy Stage 3: Vault Kick Disengage (100% poise stagger, backward leap)
                    heavyStage = 3;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.weapon = WeaponType::DAGGERS;
                    currentAttack.isLineBox = true;
                    currentAttack.range = 75.0f;
                    currentAttack.width = 46.0f;
                    currentAttack.arcAngle = 0.0f;
                    float kickBase = (stats.upgradeLevel >= 2) ? 16.0f : ((stats.upgradeLevel >= 1) ? 14.0f : 12.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (kickBase * 1.5f) : kickBase) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 200.0f;
                    currentAttack.duration = 0.14f;
                    currentAttack.timer = 0.14f;
                    currentAttack.hasHit = false;

                    velocity.x -= forwardDir.x * 380.0f;
                    velocity.y -= forwardDir.y * 380.0f;

                    SoundFX::PlayDash();
                    particles.AddScreenShake(3.0f, 0.10f);
                    particles.SpawnSlashTrail(position, Vector2Scale(forwardDir, -1.0f), Color{ 100, 255, 180, 220 });
                } else if (currentWeapon == WeaponType::SWORD) {
                    // Crusader Blade Heavy Stage 3: Radiant Execution (downward holy cleave with +50% stagger punish)
                    heavyStage = 3;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.weapon = WeaponType::SWORD;
                    currentAttack.isLineBox = true;
                    currentAttack.range = 135.0f;
                    currentAttack.width = 56.0f;
                    currentAttack.arcAngle = 0.0f;
                    float execBase = (stats.upgradeLevel >= 2) ? 46.0f : ((stats.upgradeLevel >= 1) ? 39.0f : 34.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (execBase * 1.5f) : execBase) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 360.0f * stats.poiseMultiplier;
                    currentAttack.duration = 0.22f;
                    currentAttack.timer = 0.22f;
                    currentAttack.hasHit = false;

                    if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
                    currentAttack.hasShockwave = true;
                    currentAttack.shockwaveRadius = 150.0f;
                    currentAttack.damage += 30.0f;
                    particles.SpawnFloatingText(position, "EARTH RUPTURE +30", Color{ 255, 180, 60, 255 }, 22.0f);
                }
                velocity.x += forwardDir.x * 120.0f;
                    velocity.y += forwardDir.y * 120.0f;

                    SoundFX::PlayCrusaderClash();
                    particles.AddScreenShake(7.0f, 0.22f);
                    particles.SpawnSparks(position, Color{ 255, 230, 90, 255 }, 28, 200.0f);
                } else if (currentWeapon == WeaponType::HALBERD) {
                    // Royal Halberd Heavy Stage 3: Flurry of 3 consecutive rapid stabs (each deals 30% of base weapon damage)
                    heavyStage = 3;
                    halberdStabCount = 1;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isLineBox = true;
                    currentAttack.range = 165.0f;
                    currentAttack.width = 46.0f;
                    currentAttack.arcAngle = 0.0f;
                    float baseDmg = stats.lightDamage * 0.30f;
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 180.0f;
                    currentAttack.defensePierce = 0.20f + stats.defensePierceBonus;
                    currentAttack.duration = 0.09f;
                    currentAttack.timer = 0.09f;
                    currentAttack.hasHit = false;

                    velocity.x += forwardDir.x * 110.0f;
                    velocity.y += forwardDir.y * 110.0f;

                    SoundFX::PlayLightAttack();
                    particles.SpawnSparks(Vector2{ position.x + forwardDir.x * 90.0f, position.y + forwardDir.y * 90.0f }, Color{ 220, 245, 255, 255 }, 10, 130.0f);
                } else {
                    currentAttack.type = AttackType::NONE;
                    heavyStage = 1;
                }
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

                    float baseDmg = stats.lightDamage * 0.30f;
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
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
        float effectiveDashStamina = stats.dashStaminaCost;
        if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER)) {
            effectiveDashStamina *= 0.70f; // -30% Dash Stamina Cost
        }

        if (stamina < effectiveDashStamina) {
            ModifyComposure(-15.0f); // Out of stamina dash penalty
            particles.SpawnFloatingText(position, "EXHAUSTED", Color{ 240, 100, 100, 200 }, 16.0f);
            return;
        }

        stamina -= effectiveDashStamina;
    }



    dashCooldown = stats.dashCooldown;

    dashTimer = Config::DASH_DURATION;

    isDashing = true;

    iFrameTimer = HasFullArmorSet(ArmorSet::SHADOWSTRIDER) ? 0.35f : 0.25f; // Extended 0.35s I-Frame window!



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

    // Daggers light attack combo: every 5th strike consumes 14 stamina to balance rapid flurry
    if (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS) {
        daggerLightAttackCounter++;
        if (daggerLightAttackCounter >= 5) {
            daggerLightAttackCounter = 0;
            if (!isInPhlegmaticTrance) {
                float daggerCost = (currentWeapon == WeaponType::ZEPHYR_DAGGERS) ? 10.0f : 14.0f;
                if (stamina < daggerCost) {
                    ModifyComposure(-15.0f);
                    particles.SpawnFloatingText(position, "EXHAUSTED", Color{ 240, 100, 100, 200 }, 16.0f);
                    return;
                }
                stamina -= daggerCost;
                particles.SpawnFloatingText(position, "5-STRIKE EXERTION (-14 STAMINA)", Color{ 255, 200, 100, 220 }, 15.0f);
            }
        }
    } else if (!isInPhlegmaticTrance) {
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

    float baseLightDmg = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;

    // Critical hit chance & multiplier from Mist Fairy & Shadowstrider
    float critChance = 0.05f;
    float critMult = stats.critMultiplier;
    if (currentTotem == TotemType::TOTEM_MIST) {
        critChance += 0.20f;
        critMult += 0.35f;
    }
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER) && 
        (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS || currentWeapon == WeaponType::WINDRUNNER_RECURVE)) {
        critChance += 0.15f;
    }
    if ((rand() % 100) < (int)(critChance * 100.0f)) {
        baseLightDmg *= critMult;
        particles.SpawnFloatingText(position, "CRITICAL HIT!", Color{ 255, 230, 80, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 255, 220, 80, 255 }, 14, 130.0f);
    }

    // Armor set weapon synergies
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE) && (currentWeapon == WeaponType::TRUTH_SEEKER || currentWeapon == WeaponType::SWORD)) {
        baseLightDmg *= 1.25f;
    }
    if (HasFullArmorSet(ArmorSet::COMMONERS_SCRAP) && currentWeapon == WeaponType::SWORD) {
        lightCooldown *= 0.85f;
    }
    if (HasFullArmorSet(ArmorSet::VANGUARD_BASTION) && (currentWeapon == WeaponType::HALBERD || currentWeapon == WeaponType::STORM_GLAIVE)) {
        currentAttack.range += 25.0f;
    }

    currentAttack.damage = baseLightDmg * GetTotemDamageMultiplier();

    currentAttack.comboStep = comboStep;

    currentAttack.hasShockwave = false;

    currentAttack.isFrontalCircle = false;

    currentAttack.defensePierce = 0.0f;



    // Windrunner's Recurve: Light Attack fires rapid wind arrow (windburst AoE on impact)
    if (currentWeapon == WeaponType::WINDRUNNER_RECURVE) {
        Vector2 spawnPos = { position.x + forwardDir.x * (radius + 16.0f), position.y + forwardDir.y * (radius + 16.0f) };
        Vector2 projVel = { forwardDir.x * 780.0f, forwardDir.y * 780.0f };
        Projectile proj(spawnPos, projVel, 7.0f, currentAttack.damage, ProjectileType::GALE_ARROW, true, 0.20f);
        proj.SetPiercing(false);
        spawnedProjectiles.push_back(proj);
        particles.SpawnSparks(spawnPos, Color{ 110, 245, 220, 255 }, 12, 140.0f);
        SoundFX::PlayLightAttack();
    }

    // Elemental Core Totem: Infinitely Piercing Projectile on Light Attack (3s cooldown, 54 damage)
    if (currentTotem == TotemType::ELEMENTAL_CORE_TOTEM && elementalProjectileCooldown <= 0.0f) {
        Vector2 spawnPos = { position.x + forwardDir.x * (radius + 14.0f), position.y + forwardDir.y * (radius + 14.0f) };
        Vector2 projVel = { forwardDir.x * 580.0f, forwardDir.y * 580.0f };
        Projectile proj(spawnPos, projVel, 11.0f, 54.0f, ProjectileType::ELEMENTAL_BOLT, true, 0.25f);
        proj.SetPiercing(true);
        spawnedProjectiles.push_back(proj);
        elementalProjectileCooldown = 3.0f;
        particles.SpawnSparks(spawnPos, Color{ 255, 140, 50, 255 }, 22, 170.0f);
        SoundFX::PlayCrusaderClash();
    }



    if (currentWeapon == WeaponType::MACE) {

        // War Mace Light: Frontal circular smack

        currentAttack.isFrontalCircle = true;

        currentAttack.circleRadius = 50.0f;

        currentAttack.circleCenter = { position.x + forwardDir.x * 55.0f, position.y + forwardDir.y * 55.0f };

        float baseLightDmg = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;

    // Critical hit chance & multiplier from Mist Fairy & Shadowstrider
    float critChance = 0.05f;
    float critMult = stats.critMultiplier;
    if (currentTotem == TotemType::TOTEM_MIST) {
        critChance += 0.20f;
        critMult += 0.35f;
    }
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER) && 
        (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS || currentWeapon == WeaponType::WINDRUNNER_RECURVE)) {
        critChance += 0.15f;
    }
    if ((rand() % 100) < (int)(critChance * 100.0f)) {
        baseLightDmg *= critMult;
        particles.SpawnFloatingText(position, "CRITICAL HIT!", Color{ 255, 230, 80, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 255, 220, 80, 255 }, 14, 130.0f);
    }

    // Armor set weapon synergies
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE) && (currentWeapon == WeaponType::TRUTH_SEEKER || currentWeapon == WeaponType::SWORD)) {
        baseLightDmg *= 1.25f;
    }
    if (HasFullArmorSet(ArmorSet::COMMONERS_SCRAP) && currentWeapon == WeaponType::SWORD) {
        lightCooldown *= 0.85f;
    }
    if (HasFullArmorSet(ArmorSet::VANGUARD_BASTION) && (currentWeapon == WeaponType::HALBERD || currentWeapon == WeaponType::STORM_GLAIVE)) {
        currentAttack.range += 25.0f;
    }

    currentAttack.damage = baseLightDmg * GetTotemDamageMultiplier();

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

        float baseLightDmg = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;

    // Critical hit chance & multiplier from Mist Fairy & Shadowstrider
    float critChance = 0.05f;
    float critMult = stats.critMultiplier;
    if (currentTotem == TotemType::TOTEM_MIST) {
        critChance += 0.20f;
        critMult += 0.35f;
    }
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER) && 
        (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS || currentWeapon == WeaponType::WINDRUNNER_RECURVE)) {
        critChance += 0.15f;
    }
    if ((rand() % 100) < (int)(critChance * 100.0f)) {
        baseLightDmg *= critMult;
        particles.SpawnFloatingText(position, "CRITICAL HIT!", Color{ 255, 230, 80, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 255, 220, 80, 255 }, 14, 130.0f);
    }

    // Armor set weapon synergies
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE) && (currentWeapon == WeaponType::TRUTH_SEEKER || currentWeapon == WeaponType::SWORD)) {
        baseLightDmg *= 1.25f;
    }
    if (HasFullArmorSet(ArmorSet::COMMONERS_SCRAP) && currentWeapon == WeaponType::SWORD) {
        lightCooldown *= 0.85f;
    }
    if (HasFullArmorSet(ArmorSet::VANGUARD_BASTION) && (currentWeapon == WeaponType::HALBERD || currentWeapon == WeaponType::STORM_GLAIVE)) {
        currentAttack.range += 25.0f;
    }

    currentAttack.damage = baseLightDmg * GetTotemDamageMultiplier();

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

            float baseLightDmg = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;

    // Critical hit chance & multiplier from Mist Fairy & Shadowstrider
    float critChance = 0.05f;
    float critMult = stats.critMultiplier;
    if (currentTotem == TotemType::TOTEM_MIST) {
        critChance += 0.20f;
        critMult += 0.35f;
    }
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER) && 
        (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS || currentWeapon == WeaponType::WINDRUNNER_RECURVE)) {
        critChance += 0.15f;
    }
    if ((rand() % 100) < (int)(critChance * 100.0f)) {
        baseLightDmg *= critMult;
        particles.SpawnFloatingText(position, "CRITICAL HIT!", Color{ 255, 230, 80, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 255, 220, 80, 255 }, 14, 130.0f);
    }

    // Armor set weapon synergies
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE) && (currentWeapon == WeaponType::TRUTH_SEEKER || currentWeapon == WeaponType::SWORD)) {
        baseLightDmg *= 1.25f;
    }
    if (HasFullArmorSet(ArmorSet::COMMONERS_SCRAP) && currentWeapon == WeaponType::SWORD) {
        lightCooldown *= 0.85f;
    }
    if (HasFullArmorSet(ArmorSet::VANGUARD_BASTION) && (currentWeapon == WeaponType::HALBERD || currentWeapon == WeaponType::STORM_GLAIVE)) {
        currentAttack.range += 25.0f;
    }

    currentAttack.damage = baseLightDmg * GetTotemDamageMultiplier();

            currentAttack.knockback = 200.0f;



            SoundFX::PlayLightAttack();

            particles.SpawnSlashTrail(position, forwardDir, Color{ 120, 230, 255, 255 });

        }

    } else if (currentWeapon == WeaponType::SUN_CLAYMORE) {

        currentAttack.range = stats.lightRange;
        currentAttack.arcAngle = stats.lightArc;
        float baseLightDmg = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;

    // Critical hit chance & multiplier from Mist Fairy & Shadowstrider
    float critChance = 0.05f;
    float critMult = stats.critMultiplier;
    if (currentTotem == TotemType::TOTEM_MIST) {
        critChance += 0.20f;
        critMult += 0.35f;
    }
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER) && 
        (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS || currentWeapon == WeaponType::WINDRUNNER_RECURVE)) {
        critChance += 0.15f;
    }
    if ((rand() % 100) < (int)(critChance * 100.0f)) {
        baseLightDmg *= critMult;
        particles.SpawnFloatingText(position, "CRITICAL HIT!", Color{ 255, 230, 80, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 255, 220, 80, 255 }, 14, 130.0f);
    }

    // Armor set weapon synergies
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE) && (currentWeapon == WeaponType::TRUTH_SEEKER || currentWeapon == WeaponType::SWORD)) {
        baseLightDmg *= 1.25f;
    }
    if (HasFullArmorSet(ArmorSet::COMMONERS_SCRAP) && currentWeapon == WeaponType::SWORD) {
        lightCooldown *= 0.85f;
    }
    if (HasFullArmorSet(ArmorSet::VANGUARD_BASTION) && (currentWeapon == WeaponType::HALBERD || currentWeapon == WeaponType::STORM_GLAIVE)) {
        currentAttack.range += 25.0f;
    }

    currentAttack.damage = baseLightDmg * GetTotemDamageMultiplier();
        currentAttack.knockback = 260.0f;
        currentAttack.defensePierce = stats.defensePierceBonus;
        SoundFX::PlayLightAttack();
        particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 175, 40, 255 });
        particles.SpawnSparks(Vector2{ position.x + forwardDir.x * 50.0f, position.y + forwardDir.y * 50.0f }, Color{ 255, 215, 80, 255 }, 12, 120.0f);

    } else if (currentWeapon == WeaponType::STORM_GLAIVE) {

        currentAttack.isLineBox = true;
        currentAttack.range = stats.lightRange;
        currentAttack.width = 46.0f;
        currentAttack.arcAngle = 0.0f;
        float baseLightDmg = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;

    // Critical hit chance & multiplier from Mist Fairy & Shadowstrider
    float critChance = 0.05f;
    float critMult = stats.critMultiplier;
    if (currentTotem == TotemType::TOTEM_MIST) {
        critChance += 0.20f;
        critMult += 0.35f;
    }
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER) && 
        (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS || currentWeapon == WeaponType::WINDRUNNER_RECURVE)) {
        critChance += 0.15f;
    }
    if ((rand() % 100) < (int)(critChance * 100.0f)) {
        baseLightDmg *= critMult;
        particles.SpawnFloatingText(position, "CRITICAL HIT!", Color{ 255, 230, 80, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 255, 220, 80, 255 }, 14, 130.0f);
    }

    // Armor set weapon synergies
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE) && (currentWeapon == WeaponType::TRUTH_SEEKER || currentWeapon == WeaponType::SWORD)) {
        baseLightDmg *= 1.25f;
    }
    if (HasFullArmorSet(ArmorSet::COMMONERS_SCRAP) && currentWeapon == WeaponType::SWORD) {
        lightCooldown *= 0.85f;
    }
    if (HasFullArmorSet(ArmorSet::VANGUARD_BASTION) && (currentWeapon == WeaponType::HALBERD || currentWeapon == WeaponType::STORM_GLAIVE)) {
        currentAttack.range += 25.0f;
    }

    currentAttack.damage = baseLightDmg * GetTotemDamageMultiplier();
        currentAttack.knockback = 210.0f;
        currentAttack.defensePierce = stats.defensePierceBonus;
        SoundFX::PlayLightAttack();
        particles.SpawnSlashTrail(position, forwardDir, Color{ 100, 245, 230, 255 });
        particles.SpawnSparks(Vector2{ position.x + forwardDir.x * 60.0f, position.y + forwardDir.y * 60.0f }, Color{ 140, 255, 240, 255 }, 10, 110.0f);

    } else {

        currentAttack.knockback = 130.0f;

        SoundFX::PlayLightAttack();

        particles.SpawnSlashTrail(position, forwardDir, Color{ 80, 220, 255, 255 });

    }



    currentAttack.damage *= GetTotemDamageMultiplier();



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



    if (currentWeapon == WeaponType::DAGGERS || currentWeapon == WeaponType::ZEPHYR_DAGGERS) {

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

        currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();

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

    bool isHeavyWeaponWindup = isHeavyWindingUp && (
        currentWeapon == WeaponType::SUN_CLAYMORE ||
        currentWeapon == WeaponType::MACE ||
        currentWeapon == WeaponType::HALBERD ||
        currentWeapon == WeaponType::STORM_GLAIVE ||
        HasFullArmorSet(ArmorSet::VANGUARD_BASTION)
    );

    if (isInPhlegmaticTrance || (HasStoicPoise() && isHeavyWindingUp) || isHeavyWeaponWindup) {
        particles.SpawnFloatingText(position, "HYPER-ARMOR DEFLECT!", Color{ 255, 215, 80, 255 }, 24.0f);
        particles.SpawnSparks(position, Color{ 255, 185, 60, 255 }, 14, 120.0f);
        SoundFX::PlayParrySuccess();
        return;
    }

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

    // Evasion bonus when inside an active Smoke Mist zone
    bool insideSmoke = false;
    for (const auto& zone : smokeMistZones) {
        if (zone.active && Vector2Distance(position, zone.position) <= zone.radius) {
            insideSmoke = true;
            break;
        }
    }
    if (insideSmoke && (rand() % 100) < 40) {
        ModifyComposure(10.0f);
        particles.SpawnFloatingText(position, "MIST EVADED!", Color{ 180, 120, 255, 255 }, 20.0f);
        particles.SpawnSparks(position, Color{ 160, 90, 240, 220 }, 14, 90.0f);
        return false;
    }

    // Crusader's Totem: Will of Atlas emergency crisis trigger check
    if (currentTotem == TotemType::CRUSADER_TOTEM && willOfAtlasCooldown <= 0.0f && willOfAtlasTimer <= 0.0f && health > 0.0f) {
        float projectedHp = health - (amount * (100.0f / (100.0f + GetDefense())));
        if (projectedHp <= maxHealth * 0.20f) {
            willOfAtlasTimer = 4.0f;
            willOfAtlasCooldown = 60.0f;
            SoundFX::PlayParrySuccess();
            SoundFX::PlayCrusaderClash();
            particles.SpawnFloatingText(position, "WILL OF ATLAS! (4s INVULNERABLE)", Color{ 255, 220, 80, 255 }, 24.0f);
            particles.SpawnSparks(position, Color{ 255, 230, 90, 255 }, 35, 180.0f);
            particles.AddScreenShake(5.0f, 0.25f);
            return false; // Prevent damage from reducing below threshold on trigger
        }
    }

    // Invulnerable during Dash, Dagger Lunge, or Active I-Frames
    if (isDashing || isDaggerLunging || HasInvulnerability()) {

        ModifyComposure(15.0f); // Evasive Dash through hitbox

        particles.SpawnFloatingText(position, "EVADED", Color{ 120, 240, 255, 255 }, 18.0f);

        // Shadowstrider: 15% chance to spawn an AoE Smoke Mist on perfect evade
        if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER) && (rand() % 100) < 15) {
            SmokeMistZone smoke;
            smoke.position = position;
            smoke.radius = 95.0f;
            smoke.duration = 7.0f;
            smoke.tickTimer = 0.0f;
            smoke.active = true;
            smokeMistZones.push_back(smoke);
            particles.SpawnSparks(position, Color{ 160, 90, 255, 255 }, 30, 180.0f);
            particles.SpawnFloatingText(position, "SMOKE MIST BURST!", Color{ 180, 120, 255, 255 }, 24.0f);
            SoundFX::PlayHitImpact();
        }

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

    // Totem of Insanity: suffers x1.5 incoming damage penalty from all enemy strikes
    if (currentTotem == TotemType::TOTEM_INSANITY) {
        finalAmount *= 1.50f;
    }

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

    // Hyper-armor on heavy weapon windups: reduce damage taken by 30% during startup
    bool isHeavyWeaponWindup = isHeavyWindingUp && (
        currentWeapon == WeaponType::SUN_CLAYMORE ||
        currentWeapon == WeaponType::MACE ||
        currentWeapon == WeaponType::HALBERD ||
        currentWeapon == WeaponType::STORM_GLAIVE ||
        HasFullArmorSet(ArmorSet::VANGUARD_BASTION)
    );
    if (isHeavyWeaponWindup) {
        finalAmount *= 0.70f;
        particles.SpawnSparks(position, Color{ 255, 185, 60, 255 }, 8, 80.0f);
    }

    // Basalt Golem armor poise impact feedback
    if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
        particles.SpawnSparks(position, Color{ 160, 140, 120, 240 }, 12, 90.0f);
        particles.AddScreenShake(3.5f, 0.15f);
    }

    health -= finalAmount;

    if (health < 0.0f) health = 0.0f;



    SoundFX::PlayHitImpact();

    particles.AddScreenShake(6.0f, 0.18f);

    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 15, 160.0f);

    particles.SpawnFloatingText(position, "-" + std::to_string((int)finalAmount), Color{ 255, 60, 60, 255 }, 22.0f);



    float knockbackSpeed = 240.0f;
    if (HasStoicPoise() || (isHeavyWindingUp && (isHeavyWeaponWindup || HasFullArmorSet(ArmorSet::VANGUARD_BASTION)))) {
        knockbackSpeed = 0.0f;
    } else if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
        knockbackSpeed = 70.0f; // High density basalt poise absorbs 70% knockback
    } else if (HasStoicPoise()) {
        knockbackSpeed = 60.0f;
    }

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





