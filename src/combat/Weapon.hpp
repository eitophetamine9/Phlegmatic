#pragma once
#include "CombatTypes.hpp"
#include <string>

struct WeaponStats {
    std::string name;
    std::string description;
    float lightDamage;
    float lightRange;
    float lightArc;
    float lightDuration;
    float lightCooldown;
    float lightStamina;

    float heavyDamage;
    float heavyRange;
    float heavyArc;
    float heavyWindup;
    float heavyDuration;
    float heavyCooldown;
    float heavyStamina;

    float dashStaminaCost;
    float dashCooldown;
    int upgradeLevel = 0;
    float poiseMultiplier = 1.0f;
    float defensePierceBonus = 0.0f;
    float critMultiplier = 1.0f;
    float holyBeamDamage = 22.0f;
};

inline WeaponStats GetWeaponStats(WeaponType type, int upgradeLevel = 0) {
    WeaponStats stats;
    switch (type) {
        case WeaponType::NONE:
            return {
                "Unarmed",
                "Empty hands. Search the castle courtyard or ancient altar to claim a weapon.",
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                15.0f, 0.75f,
                0, 1.0f, 0.0f, 1.0f, 0.0f
            };
        case WeaponType::DAGGERS:
            stats = {
                "Dual Daggers",
                "Swift twin-blade flurry with 0 stamina light attacks. Heavy attack unleashes a 3-stage combo: Shadow Puncture (invulnerable lunge applying Bleed), Backstab Cross-Slash (+40% bonus against bleeding/rear targets), and Vault Kick (staggers target and propels player backward).",
                12.0f, 65.0f, 95.0f, 0.14f, 0.18f, 0.0f,
                58.0f, 95.0f, 360.0f, 0.18f, 0.12f, 1.80f, 18.0f,
                14.0f, 0.50f,
                0, 1.0f, 0.0f, 1.0f, 0.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "Dual Daggers +1";
                stats.lightDamage = 15.0f;
                stats.heavyDamage = 68.0f;
            }
            if (upgradeLevel >= 2) {
                stats.upgradeLevel = 2;
                stats.name = "Dual Daggers +2";
                stats.lightDamage = 18.0f;
                stats.heavyDamage = 80.0f;
                stats.critMultiplier = 1.10f;
            }
            return stats;

        case WeaponType::MACE:
            stats = {
                "War Mace",
                "Crushing heavy warhammer. Heavy attack unleashes a 2-stage devastating combo: Overhead Skull Crusher (hyper-armor, inflicts -20% Armor Fracture debuff for 4s), and Seismic Tremor (360-deg shockwave piercing 25% armor and knocking foes flat).",
                24.0f, 80.0f, 85.0f, 0.28f, 0.52f, 22.0f,
                78.0f, 150.0f, 360.0f, 0.45f, 0.35f, 3.20f, 36.0f,
                26.0f, 1.25f,
                0, 1.0f, 0.0f, 1.0f, 0.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "War Mace +1";
                stats.lightDamage = 28.0f;
                stats.heavyDamage = 92.0f;
            }
            if (upgradeLevel >= 2) {
                stats.upgradeLevel = 2;
                stats.name = "War Mace +2";
                stats.lightDamage = 34.0f;
                stats.heavyDamage = 108.0f;
                stats.defensePierceBonus = 0.10f;
            }
            return stats;

        case WeaponType::TRUTH_SEEKER:
            stats = {
                "Truth Seeker (Partial)",
                "Legendary holy broadsword fragment containing 1% of primordial blade power. Heavy attack unleashes Dawnfire Ring (34 dmg 360-deg sweep applying Solar Burn & firing holy beam) into Sacred Judgment Pillar (38 dmg beam with 100% defense pierce that cleanses player debuffs).",
                22.0f, 98.0f, 130.0f, 0.22f, 0.26f, 14.0f,
                72.0f, 145.0f, 360.0f, 0.35f, 0.40f, 3.20f, 32.0f,
                16.0f, 0.65f,
                0, 1.0f, 0.0f, 1.0f, 22.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "Truth Seeker [Consecrated]";
                stats.lightDamage = 28.0f;
                stats.heavyDamage = 88.0f;
                stats.holyBeamDamage = 28.0f;
            }
            return stats;

        case WeaponType::HALBERD:
            stats = {
                "Royal Halberd",
                "Vanguard polearm with extended reach. Heavy attack performs a 3-stage combo: a long lunge with invulnerability frames, a sweeping 220-deg cleave, and a rapid flurry of 3 consecutive stabs dealing 30% weapon damage.",
                16.0f, 140.0f, 45.0f, 0.20f, 0.35f, 14.0f,
                28.0f, 155.0f, 220.0f, 0.26f, 0.35f, 1.80f, 24.0f,
                18.0f, 0.80f,
                0, 1.25f, 0.10f, 1.0f, 0.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "Royal Halberd +1";
                stats.lightDamage = 20.0f;
                stats.heavyDamage = 34.0f;
            }
            if (upgradeLevel >= 2) {
                stats.upgradeLevel = 2;
                stats.name = "Royal Halberd +2";
                stats.lightDamage = 24.0f;
                stats.heavyDamage = 40.0f;
                stats.defensePierceBonus = 0.20f;
                stats.poiseMultiplier = 1.35f;
            }
            return stats;

        case WeaponType::SWORD:
        default:
            stats = {
                "Crusader Blade",
                "Consecrated broadsword of the Order. Heavy attack executes a 3-stage martial combo: Crescent Cleave (restores +12 Composure), Shock Thrust (piercing line interrupting enemy windups), and Radiant Execution (downward holy cleave dealing +50% bonus damage to staggered foes).",
                18.0f, 75.0f, 110.0f, 0.22f, 0.35f, 15.0f,
                88.0f, 125.0f, 160.0f, 0.22f, 0.38f, 2.20f, 30.0f,
                20.0f, 0.85f,
                0, 1.0f, 0.0f, 1.0f, 0.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "Crusader Blade +1";
                stats.lightDamage = 22.0f;
                stats.heavyDamage = 102.0f;
            }
            if (upgradeLevel >= 2) {
                stats.upgradeLevel = 2;
                stats.name = "Crusader Blade +2";
                stats.lightDamage = 26.0f;
                stats.heavyDamage = 118.0f;
                stats.poiseMultiplier = 1.05f;
            }
            return stats;
    }
}
