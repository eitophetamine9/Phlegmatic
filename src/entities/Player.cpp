#include "Player.hpp"
#include "Enemy.hpp"
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
    pendingAoEChill = 0.0f;
    pendingAoEStunNonBossOnly = false;
    frostNovaVisualTimer = 0.0f;

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

    frostbiteMeter = 0.0f;

    isFrostbitten = false;

    snowFootstepTimer = 0.0f;

    stoneskinTimer = 0.0f;
    glacialImmunityTimer = 0.0f;
    berserkerTimer = 0.0f;
    grayHealth = 0.0f;
    grayHealthTimer = 0.0f;



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
    if (currentTotem == TotemType::CONQUEROR_TOTEM) {
        RelocateTowerCompanions();
    }

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

    frostbiteMeter = 0.0f;

    isFrostbitten = false;

    snowFootstepTimer = 0.0f;

    stoneskinTimer = 0.0f;
    glacialImmunityTimer = 0.0f;
    berserkerTimer = 0.0f;
    grayHealth = 0.0f;
    grayHealthTimer = 0.0f;

    pendingArmorSkillAoE = false;
    pendingAoEChill = 0.0f;
    pendingAoEStunNonBossOnly = false;
    armorSkillActiveTimer = 0.0f;
    armorSkillShieldHp = 0.0f;
    frostNovaVisualTimer = 0.0f;

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
    if (currentTotem == TotemType::CONQUEROR_TOTEM) {
        RelocateTowerCompanions();
    }

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

    frostbiteMeter = 0.0f;

    isFrostbitten = false;

    snowFootstepTimer = 0.0f;

    stoneskinTimer = 0.0f;
    glacialImmunityTimer = 0.0f;
    berserkerTimer = 0.0f;
    grayHealth = 0.0f;
    grayHealthTimer = 0.0f;

    pendingArmorSkillAoE = false;
    pendingAoEChill = 0.0f;
    pendingAoEStunNonBossOnly = false;
    armorSkillActiveTimer = 0.0f;
    armorSkillShieldHp = 0.0f;
    frostNovaVisualTimer = 0.0f;

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
    isHeavyWindingUp = false;
    heavyWindupTimer = 0.0f;
    comboStep = 1;
    comboTimer = 0.0f;

    inventory.UnlockWeapon(newWeapon);

    int hotbarSlot = inventory.GetWeaponHotbarSlot(newWeapon);
    if (hotbarSlot >= 0) {
        inventory.SetActiveWeaponSlot(hotbarSlot);
    } else if (newWeapon != WeaponType::NONE) {
        inventory.SetHotbarWeapon(inventory.GetActiveWeaponSlot(), newWeapon);
    }

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
            pendingAoEChill = 0.0f;
            pendingAoEStunNonBossOnly = false;
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
            pendingAoEChill = 0.0f;
            pendingAoEStunNonBossOnly = false;
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
            pendingAoEChill = 0.0f;
            pendingAoEStunNonBossOnly = false;

            SoundFX::PlayParrySuccess();
            particles.AddScreenShake(9.0f, 0.3f);
            particles.SpawnFloatingText(position, "BASALT SPIRE WALL!", Color{ 255, 160, 50, 255 }, 26.0f);
            particles.SpawnSparks(centerPillar, Color{ 245, 140, 40, 255 }, 40, 220.0f);
            break;
        }
        case ArmorSet::FROSTWARDEN: {
            pendingArmorSkillAoE = true;
            pendingAoEDamage = 75.0f;
            pendingAoERadius = 190.0f;
            pendingAoEStun = 1.2f;
            pendingAoEArmorShred = 0.20f;
            pendingAoEChill = 4.0f;
            pendingAoEStunNonBossOnly = true;

            armorSkillActiveTimer = 6.0f;
            armorSkillShieldHp = 45.0f;
            frostNovaVisualTimer = 0.65f;

            SoundFX::PlayParrySuccess();
            SoundFX::PlayCrusaderClash();
            particles.AddScreenShake(8.5f, 0.28f);
            particles.SpawnFloatingText(position, "FROST NOVA!", Color{ 160, 235, 255, 255 }, 26.0f);
            particles.SpawnSparks(position, Color{ 180, 245, 255, 255 }, 42, 250.0f);
            break;
        }
        default:
            break;
    }
}

int Player::GetActiveGolemSubMinionCount() const {
    int count = 0;
    for (int i = 0; i < 3; ++i) {
        if (golemSubMinions[i].active && golemSubMinions[i].health > 0.0f) {
            count++;
        }
    }
    return count;
}

void Player::InitGolemSubMinions() {
    for (int i = 0; i < 3; ++i) {
        golemSubMinions[i].active = true;
        golemSubMinions[i].maxHealth = 280.0f;
        golemSubMinions[i].health = 280.0f;
        golemSubMinions[i].defense = 40.0f;
        golemSubMinions[i].damage = 18.0f;
        golemSubMinions[i].radius = 14.0f;
        golemSubMinions[i].attackCooldown = 0.0f;
        golemSubMinions[i].attackVisualTimer = 0.0f;
        golemSubMinions[i].invulnTimer = 0.0f;
        golemSubMinions[i].formationAngle = (float)i * (2.0f * 3.14159265f / 3.0f);
    }
    RelocateGolemSubMinions();
}

void Player::RelocateGolemSubMinions() {
    for (int i = 0; i < 3; ++i) {
        float ang = golemSubMinions[i].formationAngle;
        Vector2 offset = { cosf(ang) * 48.0f, sinf(ang) * 48.0f };
        golemSubMinions[i].position = Vector2Add(companion.position, offset);
    }
}

void Player::RelocateCompanionToPlayer() {
    float sideAngle = (facingAngle - 90.0f) * DEG2RAD;
    Vector2 offset = { cosf(sideAngle) * 36.0f, sinf(sideAngle) * 36.0f };
    companion.position = Vector2Add(position, offset);
    companion.attackCooldown = 0.0f;
    companion.invulnTimer = 0.0f;
    if (companion.isCommandingTitan) {
        RelocateGolemSubMinions();
    }
    if (currentTotem == TotemType::TOTEM_MIST) {
        mistFairy.active = true;
        mistFairy.position = { position.x - 32.0f, position.y - 20.0f };
    }
}

int Player::GetActiveTowerCompanionCount() const {
    int count = 0;
    for (const auto& c : towerCompanions) {
        if (c.active && c.health > 0.0f) {
            count++;
        }
    }
    return count;
}

void Player::InitTowerSquad() {
    towerCompanions.clear();
    towerCompanions.resize(7);

    // 1. Tower Slasher (EnemyType::SLASHER)
    towerCompanions[0].unitType = EnemyType::SLASHER;
    towerCompanions[0].name = "Tower Slasher";
    towerCompanions[0].maxHealth = 80.0f;
    towerCompanions[0].health = 80.0f;
    towerCompanions[0].defense = 8.0f;
    towerCompanions[0].damage = 12.0f;
    towerCompanions[0].radius = 20.0f;
    towerCompanions[0].formationAngle = 0.0f * (2.0f * PI / 7.0f);

    // 2. Tower Brute (EnemyType::BRUTE)
    towerCompanions[1].unitType = EnemyType::BRUTE;
    towerCompanions[1].name = "Tower Brute";
    towerCompanions[1].maxHealth = 190.0f;
    towerCompanions[1].health = 190.0f;
    towerCompanions[1].defense = 32.0f;
    towerCompanions[1].damage = 16.0f;
    towerCompanions[1].radius = 28.0f;
    towerCompanions[1].formationAngle = 1.0f * (2.0f * PI / 7.0f);

    // 3. Tower Charger (EnemyType::CHARGER)
    towerCompanions[2].unitType = EnemyType::CHARGER;
    towerCompanions[2].name = "Tower Charger";
    towerCompanions[2].maxHealth = 65.0f;
    towerCompanions[2].health = 65.0f;
    towerCompanions[2].defense = 12.0f;
    towerCompanions[2].damage = 13.0f;
    towerCompanions[2].radius = 18.0f;
    towerCompanions[2].formationAngle = 2.0f * (2.0f * PI / 7.0f);

    // 4. Tower Archer (EnemyType::ARCHER)
    towerCompanions[3].unitType = EnemyType::ARCHER;
    towerCompanions[3].name = "Tower Archer";
    towerCompanions[3].maxHealth = 60.0f;
    towerCompanions[3].health = 60.0f;
    towerCompanions[3].defense = 6.0f;
    towerCompanions[3].damage = 11.0f;
    towerCompanions[3].radius = 18.0f;
    towerCompanions[3].formationAngle = 3.0f * (2.0f * PI / 7.0f);

    // 5. Tower Pyromancer (EnemyType::PYROMANCER)
    towerCompanions[4].unitType = EnemyType::PYROMANCER;
    towerCompanions[4].name = "Tower Pyromancer";
    towerCompanions[4].maxHealth = 75.0f;
    towerCompanions[4].health = 75.0f;
    towerCompanions[4].defense = 10.0f;
    towerCompanions[4].damage = 12.0f;
    towerCompanions[4].radius = 20.0f;
    towerCompanions[4].formationAngle = 4.0f * (2.0f * PI / 7.0f);

    // 6. Tower Stone Golem (EnemyType::ORE_GOLEM)
    towerCompanions[5].unitType = EnemyType::ORE_GOLEM;
    towerCompanions[5].name = "Tower Stone Golem";
    towerCompanions[5].maxHealth = 220.0f;
    towerCompanions[5].health = 220.0f;
    towerCompanions[5].defense = 45.0f;
    towerCompanions[5].damage = 14.0f;
    towerCompanions[5].radius = 26.0f;
    towerCompanions[5].formationAngle = 5.0f * (2.0f * PI / 7.0f);

    // 7. Tower Frost Troll (EnemyType::FROST_TROLL)
    towerCompanions[6].unitType = EnemyType::FROST_TROLL;
    towerCompanions[6].name = "Tower Frost Troll";
    towerCompanions[6].maxHealth = 280.0f;
    towerCompanions[6].health = 280.0f;
    towerCompanions[6].defense = 24.0f;
    towerCompanions[6].damage = 15.0f;
    towerCompanions[6].radius = 24.0f;
    towerCompanions[6].formationAngle = 6.0f * (2.0f * PI / 7.0f);

    bool shouldActivate = (currentTotem == TotemType::CONQUEROR_TOTEM);
    for (auto& c : towerCompanions) {
        c.active = shouldActivate;
        c.attackCooldown = 0.0f;
        c.attackVisualTimer = 0.0f;
        c.invulnTimer = 0.0f;
    }
    towerSquadRespawnTimer = 0.0f;
    RelocateTowerCompanions();
}

void Player::RelocateTowerCompanions() {
    if (towerCompanions.empty()) {
        InitTowerSquad();
    }
    float baseAngle = facingAngle * DEG2RAD;
    for (size_t i = 0; i < towerCompanions.size(); ++i) {
        float angle = baseAngle + (float)i * (2.0f * PI / (float)towerCompanions.size());
        Vector2 offset = { cosf(angle) * 72.0f, sinf(angle) * 72.0f };
        towerCompanions[i].position = Vector2Add(position, offset);
        towerCompanions[i].attackCooldown = 0.0f;
        towerCompanions[i].invulnTimer = 0.0f;
        towerCompanions[i].attackVisualTimer = 0.0f;
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
        int tier = inventory.GetTotemUpgrade(TotemType::ELEMENTAL_CORE_TOTEM);
        if (tier == 0) {
            companion.maxHealth = 460.0f;
            companion.defense = 55.0f;
            companion.damage = 32.0f;
            companion.radius = 16.0f;
            companion.isCommandingTitan = false;
        } else if (tier == 1) {
            companion.maxHealth = 600.0f;
            companion.defense = 65.0f;
            companion.damage = 40.0f;
            companion.radius = 18.0f;
            companion.isCommandingTitan = false;
        } else {
            companion.maxHealth = 920.0f;
            companion.defense = 75.0f;
            companion.damage = 48.0f;
            companion.radius = 28.0f;
            companion.isCommandingTitan = true;
            InitGolemSubMinions();
        }
        if (!companion.active && companion.respawnTimer <= 0.0f) {
            companion.active = true;
            companion.health = companion.maxHealth;
            RelocateCompanionToPlayer();
        }
    } else {
        companion.active = false;
        for (int i = 0; i < 3; ++i) golemSubMinions[i].active = false;
    }
    if (newTotem == TotemType::COMMANDER_TOTEM) {
        if (towerCompanions.empty()) {
            InitTowerSquad();
        } else {
            int living = GetActiveTowerCompanionCount();
            if (living == 0 && towerSquadRespawnTimer <= 0.0f) {
                for (auto& c : towerCompanions) {
                    c.active = true;
                    c.health = c.maxHealth;
                    c.invulnTimer = 0.5f;
                }
                towerSquadRespawnTimer = 0.0f;
            } else {
                for (auto& c : towerCompanions) {
                    if (c.health > 0.0f) c.active = true;
                }
            }
            RelocateTowerCompanions();
        }
    } else {
        for (auto& c : towerCompanions) {
            c.active = false;
        }
    }
    defense = GetDefense();
    will = GetWill();
    SoundFX::PlayEquipWeapon();
}

float Player::GetTotemDamageMultiplier() const {
    float mult = 1.0f;
    int tier = inventory.GetTotemUpgrade(currentTotem);
    if (currentTotem == TotemType::TOTEM_INSANITY) {
        float maxHp = GetMaxHealth();
        if (maxHp > 0.0f && (health / maxHp) <= 0.40f) {
            mult = (tier == 2) ? 3.00f : ((tier == 1) ? 2.60f : 2.35f);
        } else {
            mult = (tier == 2) ? 2.00f : ((tier == 1) ? 1.80f : 1.65f);
        }
    } else {
        mult = GetTotemStats(currentTotem, tier).damageMultiplier;
    }
    if (berserkerTimer > 0.0f) {
        mult *= 1.30f; // +30% Damage while Berserker Draught active
    }
    return mult;
}

float Player::GetMaxHealth() const {
    float bonus = GetHelmetStats(equippedHelmet).maxHpBonus
                + GetChestplateStats(equippedChestplate).maxHpBonus
                + GetLeggingsStats(equippedLeggings).maxHpBonus
                + GetTotemStats(currentTotem, inventory.GetTotemUpgrade(currentTotem)).maxHpBonus;
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
    float total = raw * GetTotemStats(currentTotem, inventory.GetTotemUpgrade(currentTotem)).defenseMultiplier;
    if (stoneskinTimer > 0.0f) {
        total *= 1.40f; // +40% Physical Defense while Stoneskin Tonic active
    }
    return total;
}

float Player::GetWill() const {
    float totalWill = 20.0f
                    + GetHelmetStats(equippedHelmet).willBonus
                    + GetChestplateStats(equippedChestplate).willBonus
                    + GetLeggingsStats(equippedLeggings).willBonus
                    + GetTotemStats(currentTotem, inventory.GetTotemUpgrade(currentTotem)).willBonus;
    if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE)) {
        totalWill += 25.0f;
    }
    return totalWill;
}

float Player::GetSpeed() const {
    float speedMult = GetHelmetStats(equippedHelmet).speedModifier
                    * GetChestplateStats(equippedChestplate).speedModifier
                    * GetLeggingsStats(equippedLeggings).speedModifier
                    * GetTotemStats(currentTotem, inventory.GetTotemUpgrade(currentTotem)).speedMultiplier;
    if (HasFullArmorSet(ArmorSet::SHADOWSTRIDER)) {
        speedMult *= 1.10f; // +10% Set Move Speed (~+21% total with light pieces)
    } else if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
        speedMult *= 0.96f;
    }
    float base = Config::PLAYER_SPEED * speedMult;
    if (speedBuffTimer > 0.0f) {
        base *= (1.0f + speedBuffAmount);
    }
    if (isFrostbitten) {
        base *= 0.80f; // -20% Move Speed when Frostbitten
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
    if (isFrostbitten) {
        rate *= 0.75f; // -25% Stamina Regen when Frostbitten
    }
    if (HasGaleHunterT2()) {
        rate *= 1.15f; // +15% Stamina Regen from Gale Hunter Tier 2
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

void Player::AddFrostbite(float amount, ParticleSystem* particles) {
    if (HasFullArmorSet(ArmorSet::FROSTWARDEN) || glacialImmunityTimer > 0.0f) {
        frostbiteMeter = 0.0f;
        isFrostbitten = false;
        return;
    }
    if (HasTrollSlayerT2()) {
        amount *= 0.75f; // 25% Frostbite resistance from Troll Slayer perk
    }
    bool wasFrostbitten = isFrostbitten;
    frostbiteMeter = fminf(100.0f, frostbiteMeter + amount);
    if (frostbiteMeter >= 100.0f) {
        isFrostbitten = true;
        if (!wasFrostbitten && particles != nullptr) {
            particles->SpawnFloatingText(position, "FROSTBITTEN! (-20% SPD, -25% STAM)", Color{ 140, 220, 255, 255 }, 22.0f);
            particles->SpawnSparks(position, Color{ 160, 230, 255, 255 }, 20, 130.0f);
        }
    }
}

void Player::ThawFrostbite(float amount) {
    frostbiteMeter = fmaxf(0.0f, frostbiteMeter - amount);
    if (frostbiteMeter < 100.0f) {
        isFrostbitten = false;
    }
}

void Player::ResetFrostbite() {
    frostbiteMeter = 0.0f;
    isFrostbitten = false;
}

void Player::UpdateSnowFootsteps(float dt, ParticleSystem& particles) {
    float moveSpeed = Vector2Length(velocity);
    if (moveSpeed > 25.0f) {
        snowFootstepTimer -= dt;
        if (snowFootstepTimer <= 0.0f) {
            SoundFX::PlaySnowFootstep(0.35f);
            snowFootstepTimer = 0.34f;
            Vector2 puffPos = { position.x + (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 6.0f, position.y + radius * 0.7f };
            particles.SpawnSparks(puffPos, Color{ 230, 245, 255, 180 }, 4, 35.0f);
        }
    } else {
        snowFootstepTimer = 0.05f;
    }
}

bool Player::EquipHotbarSlot(int slotIdx, ParticleSystem& particles) {
    if (slotIdx < 0 || slotIdx >= 3) return false;

    WeaponType targetWeapon = inventory.GetHotbarWeapon(slotIdx);
    inventory.SetActiveWeaponSlot(slotIdx);

    if (targetWeapon == WeaponType::NONE || !inventory.HasWeapon(targetWeapon)) {
        particles.SpawnFloatingText(position, "SLOT " + std::to_string(slotIdx + 1) + " EMPTY", Color{ 165, 175, 195, 220 }, 18.0f);
        return false;
    }

    if (targetWeapon == currentWeapon) {
        return true;
    }

    SetWeapon(targetWeapon);
    isHeavyWindingUp = false;
    heavyWindupTimer = 0.0f;
    heavyStage = 0;
    SoundFX::PlayEquipWeapon();
    WeaponStats stats = GetEquippedWeaponStats();
    particles.SpawnFloatingText(position, "[" + std::to_string(slotIdx + 1) + "] " + stats.name, Color{ 255, 225, 100, 255 }, 20.0f);
    particles.SpawnSparks(position, Color{ 255, 215, 80, 255 }, 10, 70.0f);
    return true;
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
    if (stoneskinTimer > 0.0f) stoneskinTimer -= dt;
    if (glacialImmunityTimer > 0.0f) {
        glacialImmunityTimer -= dt;
        ResetFrostbite();
    }
    if (berserkerTimer > 0.0f) berserkerTimer -= dt;
    if (grayHealthTimer > 0.0f) {
        grayHealthTimer -= dt;
        if (grayHealthTimer <= 0.0f) {
            grayHealth = 0.0f;
        }
    }



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
        int gTier = inventory.GetTotemUpgrade(TotemType::ELEMENTAL_CORE_TOTEM);
        companion.isCommandingTitan = (gTier >= 2);
        if (!companion.active) {
            companion.respawnTimer -= dt;
            if (companion.respawnTimer <= 0.0f) {
                companion.active = true;
                companion.health = companion.maxHealth;
                companion.respawnTimer = 0.0f;
                companion.invulnTimer = 0.5f;
                RelocateCompanionToPlayer();
                if (companion.isCommandingTitan) {
                    InitGolemSubMinions();
                    particles.SpawnFloatingText(position, "COMMANDING TITAN RECONSTRUCTED!", Color{ 255, 180, 50, 255 }, 26.0f);
                } else {
                    particles.SpawnFloatingText(position, "MINI GOLEM RECONSTRUCTED!", Color{ 120, 245, 180, 255 }, 24.0f);
                }
                particles.SpawnSparks(companion.position, Color{ 255, 170, 50, 255 }, 25, 180.0f);
                SoundFX::PlayParrySuccess();
            }
        } else {
            if (companion.invulnTimer > 0.0f) {
                companion.invulnTimer -= dt;
            }
            float regenRate = (gTier == 2) ? 8.0f : ((gTier == 1) ? 6.0f : 4.0f);
            if (companion.health < companion.maxHealth) {
                companion.health = fminf(companion.maxHealth, companion.health + regenRate * dt);
            }

            // Tier 3 Commanding Titan Sub-Minions logic
            if (companion.isCommandingTitan) {
                if (GetActiveGolemSubMinionCount() < 3) {
                    companion.subMinionSummonCooldown -= dt;
                    if (companion.subMinionSummonCooldown <= 0.0f) {
                        companion.subMinionSummonCooldown = 18.0f;
                        for (int i = 0; i < 3; ++i) {
                            if (!golemSubMinions[i].active || golemSubMinions[i].health <= 0.0f) {
                                golemSubMinions[i].active = true;
                                golemSubMinions[i].health = golemSubMinions[i].maxHealth;
                                golemSubMinions[i].invulnTimer = 0.5f;
                                float ang = (float)i * (2.0f * 3.14159265f / 3.0f);
                                golemSubMinions[i].position = Vector2Add(companion.position, { cosf(ang) * 45.0f, sinf(ang) * 45.0f });
                                particles.SpawnFloatingText(golemSubMinions[i].position, "MINI GOLEM SUMMONED!", Color{ 255, 180, 50, 255 }, 20.0f);
                                particles.SpawnSparks(golemSubMinions[i].position, Color{ 255, 160, 50, 255 }, 16, 120.0f);
                                break;
                            }
                        }
                    }
                }
                for (int i = 0; i < 3; ++i) {
                    if (golemSubMinions[i].active) {
                        if (golemSubMinions[i].invulnTimer > 0.0f) golemSubMinions[i].invulnTimer -= dt;
                        if (golemSubMinions[i].attackCooldown > 0.0f) golemSubMinions[i].attackCooldown -= dt;
                        if (golemSubMinions[i].attackVisualTimer > 0.0f) golemSubMinions[i].attackVisualTimer -= dt;
                    }
                }
            }
        }
    } else {
        companion.active = false;
        for (int i = 0; i < 3; ++i) golemSubMinions[i].active = false;
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

    // Conqueror's Totem & Tower Gladiators Squad Update
    if (currentTotem == TotemType::CONQUEROR_TOTEM) {
        if (towerCompanions.empty()) {
            InitTowerSquad();
        }
        int activeCount = GetActiveTowerCompanionCount();
        if (activeCount == 0) {
            // All 7 companions are defeated
            if (towerSquadRespawnTimer <= 0.0f) {
                towerSquadRespawnTimer = 120.0f; // 2 minutes respawn countdown begins
                particles.SpawnFloatingText(position, "CONQUEROR SQUAD FALLEN! (2m Respawn)", Color{ 255, 90, 90, 255 }, 24.0f);
            } else {
                towerSquadRespawnTimer -= dt;
                if (towerSquadRespawnTimer <= 0.0f) {
                    towerSquadRespawnTimer = 0.0f;
                    for (auto& c : towerCompanions) {
                        c.active = true;
                        c.health = c.maxHealth;
                        c.invulnTimer = 1.0f;
                        c.attackCooldown = 0.0f;
                        c.attackVisualTimer = 0.0f;
                    }
                    RelocateTowerCompanions();
                    particles.SpawnFloatingText(position, "CONQUEROR SQUAD RESURRECTED!", Color{ 215, 120, 255, 255 }, 26.0f);
                    particles.SpawnSparks(position, Color{ 200, 80, 255, 255 }, 40, 220.0f);
                    SoundFX::PlayParrySuccess();
                }
            }
        } else {
            // At least one companion is still alive: squad cannot respawn yet
            towerSquadRespawnTimer = 0.0f;
            for (auto& c : towerCompanions) {
                if (!c.active) continue;
                if (c.invulnTimer > 0.0f) c.invulnTimer -= dt;
                if (c.attackCooldown > 0.0f) c.attackCooldown -= dt;
                if (c.attackVisualTimer > 0.0f) c.attackVisualTimer -= dt;
                if (c.health < c.maxHealth) {
                    c.health = fminf(c.maxHealth, c.health + 2.5f * dt);
                }
            }
        }
    } else {
        for (auto& c : towerCompanions) {
            c.active = false;
        }
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



    // 3-Slot Weapon Hotbar switching via [1], [2], [3]
    if (IsKeyPressed(KEY_ONE)) {
        EquipHotbarSlot(0, particles);
    } else if (IsKeyPressed(KEY_TWO)) {
        EquipHotbarSlot(1, particles);
    } else if (IsKeyPressed(KEY_THREE)) {
        EquipHotbarSlot(2, particles);
    }

    // Quick Belt Potions & Elixirs via [H], [4], [5], [6]
    if (IsKeyPressed(KEY_H)) {
        UseHealthPotion(particles);
    }
    if (IsKeyPressed(KEY_FOUR)) {
        UseStoneskinTonic(particles);
    }
    if (IsKeyPressed(KEY_FIVE)) {
        UseGlacialDraught(particles);
    }
    if (IsKeyPressed(KEY_SIX)) {
        UseBerserkerDraught(particles);
    }



    // Armor Skill activation via [V]
    if (IsKeyPressed(KEY_V)) {
        TriggerArmorSkill(particles);
    }

    if (armorSkillCooldown > 0.0f) {
        armorSkillCooldown -= dt;
        if (armorSkillCooldown < 0.0f) armorSkillCooldown = 0.0f;
    }

    if (frostNovaVisualTimer > 0.0f) {
        frostNovaVisualTimer -= dt;
        if (frostNovaVisualTimer < 0.0f) frostNovaVisualTimer = 0.0f;
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
                pendingAoEChill = 0.0f;
                pendingAoEStunNonBossOnly = false;
                particles.SpawnFloatingText(position, "DAWNFIRE DETONATION!", Color{ 255, 220, 80, 255 }, 26.0f);
                particles.SpawnSparks(position, Color{ 255, 230, 90, 255 }, 30, 240.0f);
                armorSkillShieldHp = 0.0f;
            } else if (HasFullArmorSet(ArmorSet::FROSTWARDEN)) {
                armorSkillShieldHp = 0.0f;
                particles.SpawnFloatingText(position, "ICE WARD FADED", Color{ 130, 210, 245, 200 }, 16.0f);
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
            pendingAoEArmorShred = 0.0f;
            pendingAoEChill = 0.0f;
            pendingAoEStunNonBossOnly = false;
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
    if (currentTotem == TotemType::CHAMPION_TOTEM && health < GetMaxHealth()) {
        health = fminf(GetMaxHealth(), health + 2.0f * dt); // Steady +2 HP/s continuous combat regeneration
    }
    if (timeSinceCombat >= 4.0f && health < GetMaxHealth()) {
        float regenRate = (currentTotem == TotemType::CHAMPION_TOTEM) ? 14.0f : 8.0f;
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

            float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
            currentAttack.knockback = 280.0f * dStats.poiseMultiplier * poiseSetMult;

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
                float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;

                currentAttack.isFrontalCircle = true;

                currentAttack.circleRadius = 85.0f;

                currentAttack.circleCenter = { position.x + forwardDir.x * 70.0f, position.y + forwardDir.y * 70.0f };

                currentAttack.range = 155.0f;

                currentAttack.arcAngle = 180.0f;

                float baseDmg = (stats.upgradeLevel >= 2) ? 58.0f : ((stats.upgradeLevel >= 1) ? 50.0f : 42.0f);

                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();

                currentAttack.knockback = 380.0f * stats.poiseMultiplier * poiseSetMult;

                currentAttack.duration = 0.16f;

                currentAttack.timer = 0.16f;



                SoundFX::PlayHeavyAttack();

                particles.AddScreenShake(7.0f, 0.20f);

                particles.SpawnSparks(currentAttack.circleCenter, Color{ 255, 160, 50, 255 }, 24, 180.0f);

            } else if (currentWeapon == WeaponType::SWORD) {

                // Broadsword Heavy Stage 1: Wide Slash
                float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;

                currentAttack.range = 125.0f;

                currentAttack.arcAngle = 160.0f;

                float baseDmg = (stats.upgradeLevel >= 2) ? 40.0f : ((stats.upgradeLevel >= 1) ? 35.0f : 30.0f);

                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();

                currentAttack.knockback = 240.0f * stats.poiseMultiplier * poiseSetMult;

                currentAttack.duration = 0.16f;

                currentAttack.timer = 0.16f;



                SoundFX::PlayHeavyAttack();

                particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 190, 60, 255 });

                velocity.x += forwardDir.x * 180.0f;

                velocity.y += forwardDir.y * 180.0f;

            } else if (currentWeapon == WeaponType::HALBERD) {
                // Royal Halberd Heavy Stage 1: Long Lunge Thrust with Invulnerability Frames (i-frames)
                float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                heavyStage = 1;
                halberdStabCount = 0;
                iFrameTimer = 0.22f; // Invulnerability window during forward lunge!

                currentAttack.isLineBox = true;
                currentAttack.range = 175.0f;
                currentAttack.width = 52.0f;
                currentAttack.arcAngle = 0.0f;

                float baseDmg = (stats.upgradeLevel >= 2) ? 36.0f : ((stats.upgradeLevel >= 1) ? 32.0f : 28.0f);
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                currentAttack.knockback = 340.0f * stats.poiseMultiplier * poiseSetMult;
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
                float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;

                currentAttack.range = 135.0f;

                currentAttack.arcAngle = 360.0f;

                float baseDmg = (stats.upgradeLevel >= 1) ? 42.0f : 34.0f;

                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();

                currentAttack.knockback = 360.0f * stats.poiseMultiplier * poiseSetMult;

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

                // Sun-Forged Claymore Heavy Stage 1: Sunfire Crescent (180 deg sweep, inflicts 4s Solar Burn)
                float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                currentAttack.range = 165.0f;
                currentAttack.arcAngle = 180.0f;
                float baseDmg = (stats.upgradeLevel >= 2) ? 76.0f : ((stats.upgradeLevel >= 1) ? 65.0f : 55.0f);
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                currentAttack.knockback = 360.0f * stats.poiseMultiplier * poiseSetMult;
                currentAttack.defensePierce = stats.defensePierceBonus;
                currentAttack.duration = 0.22f;
                currentAttack.timer = 0.22f;

                SoundFX::PlayHeavyAttack();
                particles.AddScreenShake(6.5f, 0.20f);
                particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 175, 40, 255 });
                particles.SpawnSparks(position, Color{ 255, 215, 80, 255 }, 28, 200.0f);
                velocity.x += forwardDir.x * 140.0f;
                velocity.y += forwardDir.y * 140.0f;

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

                // Storm Glaive Heavy Stage 1: Zephyr Surge Step (0.18s i-frames, 190px piercing gale corridor)
                float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                iFrameTimer = 0.18f;
                currentAttack.isLineBox = true;
                currentAttack.isFrontalCircle = false;
                currentAttack.range = 190.0f;
                currentAttack.width = 52.0f;
                currentAttack.arcAngle = 0.0f;
                float baseDmg = (stats.upgradeLevel >= 2) ? 68.0f : ((stats.upgradeLevel >= 1) ? 58.0f : 48.0f);
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                currentAttack.knockback = 280.0f * stats.poiseMultiplier * poiseSetMult;
                currentAttack.defensePierce = stats.defensePierceBonus;
                currentAttack.duration = 0.18f;
                currentAttack.timer = 0.18f;

                SoundFX::PlayDash();
                SoundFX::PlayHeavyAttack();
                particles.AddScreenShake(5.0f, 0.16f);
                particles.SpawnSlashTrail(position, forwardDir, Color{ 100, 245, 230, 255 });
                particles.SpawnSparks(position, Color{ 140, 255, 240, 255 }, 22, 180.0f);
                velocity.x = forwardDir.x * 360.0f;
                velocity.y = forwardDir.y * 360.0f;

            } else if (currentWeapon == WeaponType::GLACIAL_CLEAVER) {

                // Twin Glacial Cleavers Heavy Stage 1: Fury of Cleaves (fast advancing frost barrage)
                float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                currentAttack.range = 140.0f;
                currentAttack.arcAngle = 165.0f;
                float baseDmg = (stats.upgradeLevel >= 2) ? 34.0f : ((stats.upgradeLevel >= 1) ? 28.0f : 24.0f);
                currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                currentAttack.knockback = 210.0f * stats.poiseMultiplier * poiseSetMult;
                currentAttack.defensePierce = stats.defensePierceBonus;
                currentAttack.duration = 0.11f;
                currentAttack.timer = 0.11f;

                SoundFX::PlayLightAttack();
                particles.AddScreenShake(3.5f, 0.12f);
                particles.SpawnSlashTrail(position, forwardDir, Color{ 140, 225, 255, 255 });
                particles.SpawnSparks(position, Color{ 180, 242, 255, 255 }, 14, 150.0f);
                velocity.x += forwardDir.x * 135.0f;
                velocity.y += forwardDir.y * 135.0f;

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
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = false;
                    currentAttack.range = 150.0f;
                    currentAttack.arcAngle = 360.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 50.0f : ((stats.upgradeLevel >= 1) ? 42.0f : 36.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 460.0f * stats.poiseMultiplier * poiseSetMult;
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
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.range = 150.0f;
                    currentAttack.width = 48.0f;
                    currentAttack.isLineBox = true;
                    currentAttack.arcAngle = 0.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 32.0f : ((stats.upgradeLevel >= 1) ? 28.0f : 24.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 300.0f * stats.poiseMultiplier * poiseSetMult;
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
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    heavyStage = 2;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isLineBox = false;
                    currentAttack.range = 155.0f;
                    currentAttack.arcAngle = 220.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 38.0f : ((stats.upgradeLevel >= 1) ? 33.0f : 28.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 320.0f * stats.poiseMultiplier * poiseSetMult;
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
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
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
                    currentAttack.knockback = 380.0f * stats.poiseMultiplier * poiseSetMult;
                    currentAttack.defensePierce = 1.0f; // 100% DEFENSE PIERCE!
                    currentAttack.duration = 0.22f;
                    currentAttack.timer = 0.22f;
                    currentAttack.hasHit = false;

                    CleanseDebuffs(particles); // Cleanses player debuffs!

                    SoundFX::PlayTranceActivate();
                    particles.AddScreenShake(8.0f, 0.26f);
                    particles.SpawnSparks(currentAttack.circleCenter, Color{ 255, 240, 100, 255 }, 35, 220.0f);
                } else if (currentWeapon == WeaponType::SUN_CLAYMORE) {
                    // Sun-Forged Claymore Heavy Stage 2: Solar Cataclysm Slam (Frontal pillar circle +25% Pierce)
                    heavyStage = 2;
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = true;
                    currentAttack.circleRadius = 115.0f;
                    currentAttack.circleCenter = { position.x + forwardDir.x * 85.0f, position.y + forwardDir.y * 85.0f };
                    currentAttack.range = 200.0f;
                    currentAttack.arcAngle = 0.0f;
                    currentAttack.isLineBox = false;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 86.0f : ((stats.upgradeLevel >= 1) ? 74.0f : 62.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 460.0f * stats.poiseMultiplier * poiseSetMult;
                    currentAttack.defensePierce = fminf(1.0f, 0.25f + stats.defensePierceBonus);
                    currentAttack.duration = 0.26f;
                    currentAttack.timer = 0.26f;
                    currentAttack.hasHit = false;
                    currentAttack.hasShockwave = false;

                    shockwaveActive = true;
                    shockwaveTimer = 0.38f;
                    shockwavePos = currentAttack.circleCenter;
                    shockwaveRadius = 115.0f;

                    if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
                        currentAttack.hasShockwave = true;
                        currentAttack.shockwavePos = currentAttack.circleCenter;
                        currentAttack.shockwaveRadius = 155.0f;
                        currentAttack.damage += 30.0f * GetTotemDamageMultiplier();
                        shockwaveRadius = 155.0f;
                        particles.SpawnFloatingText(position, "EARTH RUPTURE +30", Color{ 255, 180, 60, 255 }, 22.0f);
                    }

                    SoundFX::PlayTranceActivate();
                    SoundFX::PlayCrusaderClash();
                    particles.AddScreenShake(9.5f, 0.28f);
                    particles.SpawnSparks(currentAttack.circleCenter, Color{ 255, 225, 90, 255 }, 42, 240.0f);
                } else if (currentWeapon == WeaponType::STORM_GLAIVE) {
                    // Storm Glaive Heavy Stage 2: Tempest Cyclone (360 vortex + piercing Crescent Gale Wave)
                    heavyStage = 2;
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = false;
                    currentAttack.isLineBox = false;
                    currentAttack.range = 180.0f;
                    currentAttack.arcAngle = 360.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 74.0f : ((stats.upgradeLevel >= 1) ? 62.0f : 52.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 320.0f * stats.poiseMultiplier * poiseSetMult;
                    currentAttack.defensePierce = stats.defensePierceBonus;
                    currentAttack.duration = 0.24f;
                    currentAttack.timer = 0.24f;
                    currentAttack.hasHit = false;
                    currentAttack.hasShockwave = false;

                    shockwaveActive = true;
                    shockwaveTimer = 0.38f;
                    shockwavePos = position;
                    shockwaveRadius = 180.0f;

                    float waveBase = (stats.upgradeLevel >= 2) ? 62.0f : ((stats.upgradeLevel >= 1) ? 54.0f : 45.0f);
                    float waveDmg = (counterBuffTimer > 0.0f ? (waveBase * 1.5f) : waveBase) * GetTotemDamageMultiplier();
                    Vector2 waveSpawn = { position.x + forwardDir.x * (radius + 16.0f), position.y + forwardDir.y * (radius + 16.0f) };
                    Projectile galeWave(waveSpawn, { forwardDir.x * 680.0f, forwardDir.y * 680.0f }, 18.0f, waveDmg, ProjectileType::GALE_ARROW, true, 0.35f);
                    galeWave.SetPiercing(true);
                    spawnedProjectiles.push_back(galeWave);

                    SoundFX::PlayCrusaderClash();
                    particles.AddScreenShake(6.5f, 0.22f);
                    particles.SpawnSparks(position, Color{ 120, 255, 240, 255 }, 32, 220.0f);
                } else if (currentWeapon == WeaponType::GLACIAL_CLEAVER) {
                    // Twin Glacial Cleavers Heavy Stage 2: Fury of Cleaves (Left-hand rapid mirror sweep)
                    heavyStage = 2;
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = false;
                    currentAttack.isLineBox = false;
                    currentAttack.range = 140.0f;
                    currentAttack.arcAngle = 165.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 34.0f : ((stats.upgradeLevel >= 1) ? 28.0f : 24.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 210.0f * stats.poiseMultiplier * poiseSetMult;
                    currentAttack.defensePierce = stats.defensePierceBonus;
                    currentAttack.duration = 0.11f;
                    currentAttack.timer = 0.11f;
                    currentAttack.hasHit = false;
                    currentAttack.hasShockwave = false;

                    velocity.x += forwardDir.x * 135.0f;
                    velocity.y += forwardDir.y * 135.0f;

                    SoundFX::PlayLightAttack();
                    particles.AddScreenShake(3.5f, 0.12f);
                    particles.SpawnSlashTrail(position, forwardDir, Color{ 160, 235, 255, 255 });
                    particles.SpawnSparks(position, Color{ 180, 245, 255, 255 }, 16, 160.0f);
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
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (kickBase * 1.5f) : kickBase) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 200.0f * stats.poiseMultiplier * poiseSetMult;
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
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (execBase * 1.5f) : execBase) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 360.0f * stats.poiseMultiplier * poiseSetMult;
                    currentAttack.duration = 0.22f;
                    currentAttack.timer = 0.22f;
                    currentAttack.hasHit = false;

                    if (HasFullArmorSet(ArmorSet::BASALT_GOLEM)) {
                    currentAttack.hasShockwave = true;
                    currentAttack.shockwavePos = position;
                    currentAttack.shockwaveRadius = 150.0f;
                    currentAttack.damage += 30.0f;
                    shockwaveActive = true;
                    shockwaveTimer = 0.38f;
                    shockwavePos = position;
                    shockwaveRadius = 150.0f;
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
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 180.0f * stats.poiseMultiplier * poiseSetMult;
                    currentAttack.defensePierce = 0.20f + stats.defensePierceBonus;
                    currentAttack.duration = 0.09f;
                    currentAttack.timer = 0.09f;
                    currentAttack.hasHit = false;

                    velocity.x += forwardDir.x * 110.0f;
                    velocity.y += forwardDir.y * 110.0f;

                    SoundFX::PlayLightAttack();
                    particles.SpawnSparks(Vector2{ position.x + forwardDir.x * 90.0f, position.y + forwardDir.y * 90.0f }, Color{ 220, 245, 255, 255 }, 10, 130.0f);
                } else if (currentWeapon == WeaponType::GLACIAL_CLEAVER) {
                    // Twin Glacial Cleavers Heavy Stage 3: Fury of Cleaves (Cross-cleave flurry)
                    heavyStage = 3;
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    currentAttack.type = AttackType::HEAVY;
                    currentAttack.isFrontalCircle = false;
                    currentAttack.isLineBox = false;
                    currentAttack.range = 145.0f;
                    currentAttack.arcAngle = 175.0f;
                    float baseDmg = (stats.upgradeLevel >= 2) ? 34.0f : ((stats.upgradeLevel >= 1) ? 28.0f : 24.0f);
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 220.0f * stats.poiseMultiplier * poiseSetMult;
                    currentAttack.defensePierce = stats.defensePierceBonus;
                    currentAttack.duration = 0.11f;
                    currentAttack.timer = 0.11f;
                    currentAttack.hasHit = false;
                    currentAttack.hasShockwave = false;

                    velocity.x += forwardDir.x * 135.0f;
                    velocity.y += forwardDir.y * 135.0f;

                    SoundFX::PlayHeavyAttack();
                    particles.AddScreenShake(4.5f, 0.14f);
                    particles.SpawnSlashTrail(position, forwardDir, Color{ 160, 235, 255, 255 });
                    particles.SpawnSparks(position, Color{ 200, 248, 255, 255 }, 20, 180.0f);
                } else {
                    currentAttack.type = AttackType::NONE;
                    heavyStage = 1;
                }
            } else if (currentAttack.type == AttackType::HEAVY && heavyStage == 3 && currentWeapon == WeaponType::GLACIAL_CLEAVER) {
                // Twin Glacial Cleavers Heavy Stage 4 Finisher: Glacial Cross-Rupture
                heavyStage = 4;
                Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
                WeaponStats stats = GetEquippedWeaponStats();
                float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;

                currentAttack.type = AttackType::HEAVY;
                currentAttack.isFrontalCircle = false;
                currentAttack.isLineBox = false;
                currentAttack.range = 155.0f;
                currentAttack.arcAngle = 220.0f;
                float baseDmg = stats.heavyDamage;
                float rawDmg = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg);
                currentAttack.knockback = 420.0f * stats.poiseMultiplier * poiseSetMult;
                currentAttack.defensePierce = stats.defensePierceBonus;
                currentAttack.duration = 0.24f;
                currentAttack.timer = 0.24f;
                currentAttack.hasHit = false;
                currentAttack.hasShockwave = true;
                currentAttack.shockwavePos = { position.x + forwardDir.x * 70.0f, position.y + forwardDir.y * 70.0f };
                currentAttack.shockwaveRadius = 125.0f;

                if (HasFullArmorSet(ArmorSet::FROSTWARDEN)) {
                    currentAttack.shockwaveRadius = 175.0f;
                    rawDmg += 35.0f;
                    particles.SpawnFloatingText(position, "FROST TREMOR +35", Color{ 140, 230, 255, 255 }, 22.0f);
                }
                currentAttack.damage = rawDmg * GetTotemDamageMultiplier();

                shockwaveActive = true;
                shockwaveTimer = 0.38f;
                shockwavePos = currentAttack.shockwavePos;
                shockwaveRadius = currentAttack.shockwaveRadius;

                velocity.x += forwardDir.x * 150.0f;
                velocity.y += forwardDir.y * 150.0f;

                SoundFX::PlayCrusaderClash();
                SoundFX::PlayHeavyAttack();
                particles.AddScreenShake(9.0f, 0.26f);
                particles.SpawnSlashTrail(position, forwardDir, Color{ 140, 230, 255, 255 });
                particles.SpawnSparks(currentAttack.shockwavePos, Color{ 180, 245, 255, 255 }, 38, 240.0f);
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
                    float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
                    currentAttack.damage = (counterBuffTimer > 0.0f ? (baseDmg * 1.5f) : baseDmg) * GetTotemDamageMultiplier();
                    currentAttack.knockback = 180.0f * stats.poiseMultiplier * poiseSetMult;
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

        if (isHeavyWindingUp && currentWeapon != WeaponType::GLACIAL_CLEAVER) currentSpeed *= 0.25f;

        if (currentAttack.type == AttackType::LIGHT) currentSpeed *= 0.65f;



        if (moveDir.x != 0.0f || moveDir.y != 0.0f) {
            moveDir = Vector2Normalize(moveDir);

            if (map.GetEnvironment() == MapEnvironment::FROST_CAVERNS) {
                // Icy floor inertia: smooth gradual acceleration with momentum
                velocity.x = velocity.x * 0.88f + (moveDir.x * currentSpeed) * 0.12f;
                velocity.y = velocity.y * 0.88f + (moveDir.y * currentSpeed) * 0.12f;
            } else {
                velocity.x = moveDir.x * currentSpeed;
                velocity.y = moveDir.y * currentSpeed;
            }

            walkCycle += dt * 10.0f;
        } else {
            if (map.GetEnvironment() == MapEnvironment::FROST_CAVERNS) {
                // Icy floor inertia: low friction glide
                velocity.x *= 0.96f;
                velocity.y *= 0.96f;
                if (fabsf(velocity.x) > 25.0f || fabsf(velocity.y) > 25.0f) {
                    particles.SpawnSparks(position, Color{ 140, 220, 255, 120 }, 1, 15.0f);
                }
            } else {
                velocity.x *= 0.80f;
                velocity.y *= 0.80f;
            }
        }

    }



    // Collision

    Vector2 nextPos = { position.x + velocity.x * dt, position.y + velocity.y * dt };

    position = map.ResolveCollision(position, nextPos, radius);

}

void Player::UpdateBackgroundTimers(float dt, ParticleSystem& particles) {
    if (!IsAlive()) return;

    if (lightCooldown > 0.0f) lightCooldown -= dt;
    if (heavyCooldown > 0.0f) heavyCooldown -= dt;
    if (parryCooldown > 0.0f) parryCooldown -= dt;
    if (dashCooldown > 0.0f) dashCooldown -= dt;
    if (counterBuffTimer > 0.0f) counterBuffTimer -= dt;
    if (potionCooldown > 0.0f) potionCooldown -= dt;
    if (armorSkillCooldown > 0.0f) armorSkillCooldown -= dt;
    if (armorSkillActiveTimer > 0.0f) armorSkillActiveTimer -= dt;
    if (elementalProjectileCooldown > 0.0f) elementalProjectileCooldown -= dt;
    if (willOfAtlasCooldown > 0.0f) willOfAtlasCooldown -= dt;
    if (willOfAtlasTimer > 0.0f) willOfAtlasTimer -= dt;
    if (stoneskinTimer > 0.0f) stoneskinTimer -= dt;
    if (glacialImmunityTimer > 0.0f) glacialImmunityTimer -= dt;
    if (berserkerTimer > 0.0f) berserkerTimer -= dt;
    if (grayHealthTimer > 0.0f) grayHealthTimer -= dt;
    if (tranceTimer > 0.0f) tranceTimer -= dt;
    timeSinceCombat += dt;

    if (currentTotem == TotemType::CHAMPION_TOTEM && health < GetMaxHealth()) {
        health = fminf(GetMaxHealth(), health + 2.0f * dt);
    }
    if (timeSinceCombat >= 4.0f && health < GetMaxHealth()) {
        float regenRate = (currentTotem == TotemType::CHAMPION_TOTEM) ? 14.0f : 8.0f;
        health = fminf(GetMaxHealth(), health + regenRate * dt);
    }
    if (stamina < maxStamina) {
        stamina = fminf(maxStamina, stamina + GetStaminaRegenRate() * dt);
    }

    if (currentTotem == TotemType::ELEMENTAL_CORE_TOTEM) {
        if (!companion.active) {
            companion.respawnTimer -= dt;
            if (companion.respawnTimer <= 0.0f) {
                companion.active = true;
                companion.health = companion.maxHealth;
                companion.respawnTimer = 0.0f;
                companion.invulnTimer = 0.5f;
                RelocateCompanionToPlayer();
                particles.SpawnFloatingText(position, "MINI GOLEM RECONSTRUCTED!", Color{ 120, 245, 180, 255 }, 24.0f);
            }
        } else {
            if (companion.invulnTimer > 0.0f) companion.invulnTimer -= dt;
            if (companion.health < companion.maxHealth) {
                companion.health = fminf(companion.maxHealth, companion.health + 4.0f * dt);
            }
        }
    }

    if (currentTotem == TotemType::CONQUEROR_TOTEM) {
        int activeCount = GetActiveTowerCompanionCount();
        if (activeCount == 0) {
            if (towerSquadRespawnTimer <= 0.0f) {
                towerSquadRespawnTimer = 120.0f;
            } else {
                towerSquadRespawnTimer -= dt;
                if (towerSquadRespawnTimer <= 0.0f) {
                    towerSquadRespawnTimer = 0.0f;
                    for (auto& c : towerCompanions) {
                        c.active = true;
                        c.health = c.maxHealth;
                        c.invulnTimer = 1.0f;
                    }
                    RelocateTowerCompanions();
                    particles.SpawnFloatingText(position, "CONQUEROR SQUAD RESURRECTED!", Color{ 215, 120, 255, 255 }, 26.0f);
                }
            }
        } else {
            towerSquadRespawnTimer = 0.0f;
            for (auto& c : towerCompanions) {
                if (!c.active) continue;
                if (c.invulnTimer > 0.0f) c.invulnTimer -= dt;
                if (c.attackCooldown > 0.0f) c.attackCooldown -= dt;
                if (c.health < c.maxHealth) {
                    c.health = fminf(c.maxHealth, c.health + 2.5f * dt);
                }
            }
        }
    }

    if (currentTotem == TotemType::TOTEM_MIST) {
        mistFairy.flutterTime += dt;
        mistFairy.healOrbTimer -= dt;
        if (mistFairy.healOrbTimer <= 0.0f) {
            mistFairy.healOrbTimer = 14.0f;
            MistHealingOrb orb;
            orb.position = mistFairy.position;
            orb.lifetime = 20.0f;
            orb.healAmount = 30.0f;
            orb.active = true;
            mistHealingOrbs.push_back(orb);
        }
    }
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

    if (!isInPhlegmaticTrance) {
        if (stamina < stats.lightStamina) {
            ModifyComposure(-15.0f); // Out of stamina attack penalty
            particles.SpawnFloatingText(position, "EXHAUSTED", Color{ 240, 100, 100, 200 }, 16.0f);
            return;
        }
        stamina -= stats.lightStamina;
    }

    lightCooldown = stats.lightCooldown * GetTotemStats(currentTotem).cooldownMultiplier;



    // Truth Seeker & Twin Glacial Cleavers 2-hit alternating combo tracking

    if (currentWeapon == WeaponType::TRUTH_SEEKER || currentWeapon == WeaponType::GLACIAL_CLEAVER) {

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

    } else if (currentWeapon == WeaponType::GLACIAL_CLEAVER) {

        currentAttack.range = stats.lightRange;
        currentAttack.arcAngle = stats.lightArc;
        float baseLightDmg = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;

        float critChance = 0.05f;
        float critMult = stats.critMultiplier;
        if (currentTotem == TotemType::TOTEM_MIST) {
            critChance += 0.20f;
            critMult += 0.35f;
        }
        if ((rand() % 100) < (int)(critChance * 100.0f)) {
            baseLightDmg *= critMult;
            particles.SpawnFloatingText(position, "CRITICAL HIT!", Color{ 255, 230, 80, 255 }, 22.0f);
            particles.SpawnSparks(position, Color{ 255, 220, 80, 255 }, 14, 130.0f);
        }

        if (HasFullArmorSet(ArmorSet::FROSTWARDEN)) {
            baseLightDmg *= 1.20f;
            currentAttack.range += 15.0f;
        }

        currentAttack.damage = baseLightDmg * GetTotemDamageMultiplier();
        currentAttack.knockback = 280.0f;
        currentAttack.defensePierce = stats.defensePierceBonus;
        SoundFX::PlayLightAttack();
        particles.SpawnSlashTrail(position, forwardDir, Color{ 140, 220, 255, 255 });
        particles.SpawnSparks(Vector2{ position.x + forwardDir.x * 55.0f, position.y + forwardDir.y * 55.0f }, Color{ 180, 235, 255, 255 }, 12, 130.0f);

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

        float poiseSetMult = HasFullArmorSet(ArmorSet::FROSTWARDEN) ? 1.20f : 1.0f;
        currentAttack.knockback = 180.0f * stats.poiseMultiplier * poiseSetMult;

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
        currentWeapon == WeaponType::GLACIAL_CLEAVER ||
        HasFullArmorSet(ArmorSet::VANGUARD_BASTION) ||
        HasFullArmorSet(ArmorSet::FROSTWARDEN)
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
        if (currentTotem == TotemType::CHAMPION_TOTEM) {
            particles.SpawnFloatingText(position, "+14 HP (Champion -70%)", Color{ 255, 175, 120, 255 }, 22.0f);
        } else {
            particles.SpawnFloatingText(position, "+45 HP", Color{ 80, 255, 140, 255 }, 24.0f);
        }
        return true;
    }
    return false;
}

bool Player::UseStoneskinTonic(ParticleSystem& particles) {
    if (inventory.GetStoneskinTonicCount() <= 0) {
        particles.SpawnFloatingText(position, "NO STONESKIN TONICS", Color{ 240, 100, 100, 220 }, 16.0f);
        return false;
    }
    if (inventory.ConsumeStoneskinTonic()) {
        stoneskinTimer = 15.0f;
        SoundFX::PlayParrySuccess();
        particles.SpawnFloatingText(position, "+STONESKIN (+40% DEF, POISE)", Color{ 255, 215, 80, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 245, 195, 50, 255 }, 22, 140.0f);
        return true;
    }
    return false;
}

bool Player::UseGlacialDraught(ParticleSystem& particles) {
    if (inventory.GetGlacialDraughtCount() <= 0) {
        particles.SpawnFloatingText(position, "NO GLACIAL DRAUGHTS", Color{ 240, 100, 100, 220 }, 16.0f);
        return false;
    }
    if (inventory.ConsumeGlacialDraught()) {
        glacialImmunityTimer = 90.0f;
        ResetFrostbite();
        SoundFX::PlayCrusaderClash();
        particles.SpawnFloatingText(position, "+GLACIAL THAW & WARMTH (90s)", Color{ 140, 230, 255, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 120, 220, 255, 255 }, 22, 140.0f);
        return true;
    }
    return false;
}

bool Player::UseBerserkerDraught(ParticleSystem& particles) {
    if (inventory.GetBerserkerDraughtCount() <= 0) {
        particles.SpawnFloatingText(position, "NO BERSERKER BREW", Color{ 240, 100, 100, 220 }, 16.0f);
        return false;
    }
    if (inventory.ConsumeBerserkerDraught()) {
        berserkerTimer = 12.0f;
        SoundFX::PlayHeavyAttack();
        particles.SpawnFloatingText(position, "+BERSERKER FRENZY (+30% DMG)", Color{ 255, 60, 60, 255 }, 22.0f);
        particles.SpawnSparks(position, Color{ 255, 50, 50, 255 }, 26, 160.0f);
        return true;
    }
    return false;
}

void Player::RecoverGrayHealth(float amount, ParticleSystem& particles) {
    if (grayHealth <= 0.0f) return;
    float recovered = fminf(grayHealth, amount);
    grayHealth -= recovered;
    health = fminf(GetMaxHealth(), health + recovered);
    particles.SpawnFloatingText(position, "+" + std::to_string((int)recovered) + " GRAY HP", Color{ 140, 230, 255, 255 }, 18.0f);
    particles.SpawnSparks(position, Color{ 120, 215, 255, 200 }, 8, 50.0f);
}



bool Player::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles, float defensePierce, bool isCold) {

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

    if (berserkerTimer > 0.0f) {
        finalAmount *= 1.20f; // +20% incoming damage while Berserk
    }

    if (isCold && HasFullArmorSet(ArmorSet::FROSTWARDEN)) {
        finalAmount *= 0.70f; // 30% Cold Resistance
    }

    // Totem of the Phlegmatic Core: 80% knockback reduction + 25% damage converted to Gray Health
    if (currentTotem == TotemType::TOTEM_PHLEGMATIC_CORE) {
        knockbackDir.x *= 0.20f;
        knockbackDir.y *= 0.20f;
        float grayAdd = finalAmount * 0.25f;
        grayHealth = fminf(GetMaxHealth() - (health - finalAmount), grayHealth + grayAdd);
        grayHealthTimer = 6.0f;
        particles.SpawnSparks(position, Color{ 140, 220, 255, 200 }, 6, 40.0f);
    }

    // Armor Skill Shield Absorption (Frostwarden Ice Ward vs Crusader Dawnfire Aegis)
    if (armorSkillShieldHp > 0.0f) {
        if (HasFullArmorSet(ArmorSet::FROSTWARDEN)) {
            float absorbed = fminf(armorSkillShieldHp, finalAmount);
            armorSkillShieldHp -= absorbed;
            finalAmount -= absorbed;
            particles.SpawnFloatingText(position, "ICE WARD -" + std::to_string((int)absorbed), Color{ 140, 230, 255, 255 }, 20.0f);
            particles.SpawnSparks(position, Color{ 160, 240, 255, 255 }, 16, 150.0f);
            if (armorSkillShieldHp <= 0.0f) {
                armorSkillShieldHp = 0.0f;
                particles.SpawnFloatingText(position, "ICE WARD SHATTERED!", Color{ 150, 235, 255, 255 }, 22.0f);
                particles.SpawnSparks(position, Color{ 180, 248, 255, 255 }, 28, 210.0f);
            }
            if (finalAmount <= 0.0f) return false;
        } else {
            float absorbed = fminf(armorSkillShieldHp, finalAmount);
            armorSkillShieldHp -= absorbed;
            finalAmount -= absorbed;
            particles.SpawnFloatingText(position, "AEGIS SHIELD -" + std::to_string((int)absorbed), Color{ 255, 230, 90, 255 }, 22.0f);
            if (armorSkillShieldHp <= 0.0f) {
                armorSkillShieldHp = 0.0f;
                armorSkillActiveTimer = 0.0f;
                if (HasFullArmorSet(ArmorSet::CRUSADER_PLATE)) {
                    pendingArmorSkillAoE = true;
                    pendingAoEDamage = 45.0f;
                    pendingAoERadius = 180.0f;
                    pendingAoEStun = 1.5f;
                    pendingAoEChill = 0.0f;
                    pendingAoEStunNonBossOnly = false;
                    particles.SpawnFloatingText(position, "DAWNFIRE DETONATION!", Color{ 255, 220, 80, 255 }, 26.0f);
                    particles.SpawnSparks(position, Color{ 255, 230, 90, 255 }, 30, 240.0f);
                }
            }
            if (finalAmount <= 0.0f) return false;
        }
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
        currentWeapon == WeaponType::GLACIAL_CLEAVER ||
        HasFullArmorSet(ArmorSet::VANGUARD_BASTION) ||
        HasFullArmorSet(ArmorSet::FROSTWARDEN)
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
    if (currentTotem == TotemType::CHAMPION_TOTEM) {
        amount *= 0.30f; // Withered Vitality: -70% incoming healing from external sources
    }
    health = fminf(GetMaxHealth(), health + amount);
}



void Player::Draw() const {
    PlayerRenderer::Draw(*this);
}

bool Player::HasTrollSlayerT2() const {
    return (inventory.GetEnemyKills(9) + inventory.GetEnemyKills(10) + inventory.GetEnemyKills(11)) >= 5;
}

bool Player::HasTrollSlayerT3() const {
    return (inventory.GetEnemyKills(9) + inventory.GetEnemyKills(10) + inventory.GetEnemyKills(11)) >= 15;
}

bool Player::HasGolemBreakerT2() const {
    return (inventory.GetEnemyKills(5) + inventory.GetEnemyKills(6)) >= 5;
}

bool Player::HasGolemBreakerT3() const {
    return (inventory.GetEnemyKills(5) + inventory.GetEnemyKills(6)) >= 12;
}

bool Player::HasGaleHunterT2() const {
    return (inventory.GetEnemyKills(7) + inventory.GetEnemyKills(8)) >= 5;
}

bool Player::HasGaleHunterT3() const {
    return (inventory.GetEnemyKills(7) + inventory.GetEnemyKills(8)) >= 15;
}

bool Player::HasCultistPurifierT2() const {
    int total = inventory.GetEnemyKills(0) + inventory.GetEnemyKills(1) + inventory.GetEnemyKills(2) + inventory.GetEnemyKills(3);
    return total >= 10;
}

bool Player::HasCultistPurifierT3() const {
    int total = inventory.GetEnemyKills(0) + inventory.GetEnemyKills(1) + inventory.GetEnemyKills(2) + inventory.GetEnemyKills(3);
    return total >= 25;
}

float Player::GetBestiaryDamageMultiplier(EnemyType type) const {
    float mult = 1.0f;
    if (type == EnemyType::FROST_TROLL || type == EnemyType::BERSERKER_TROLL || type == EnemyType::FROST_REVENANT) {
        if (HasTrollSlayerT3()) mult *= 1.25f;
        else if (HasTrollSlayerT2()) mult *= 1.15f;
    } else if (type == EnemyType::ORE_GOLEM || type == EnemyType::ELEMENTAL_GOLEM || type == EnemyType::MINI_ELEMENTAL_GOLEM) {
        if (HasGolemBreakerT3()) mult *= 1.20f;
        else if (HasGolemBreakerT2()) mult *= 1.10f;
    } else if (type == EnemyType::GALE_BANDIT || type == EnemyType::GALE_ARCHER_BOSS) {
        if (HasGaleHunterT3()) mult *= 1.20f;
        else if (HasGaleHunterT2()) mult *= 1.10f;
    } else if (type == EnemyType::SLASHER || type == EnemyType::ARCHER || type == EnemyType::BRUTE || type == EnemyType::PYROMANCER) {
        if (HasCultistPurifierT3()) mult *= 1.15f;
        else if (HasCultistPurifierT2()) mult *= 1.10f;
    }
    return mult;
}

float Player::GetBestiaryDefensePierce(EnemyType type) const {
    if ((type == EnemyType::ORE_GOLEM || type == EnemyType::ELEMENTAL_GOLEM || type == EnemyType::MINI_ELEMENTAL_GOLEM) && HasGolemBreakerT3()) {
        return 0.25f;
    }
    return 0.0f;
}






