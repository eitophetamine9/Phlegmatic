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
                "Swift twin-blade flurry with 0 stamina light attacks. Heavy attack executes a high-speed piercing lunge into an instant 360 whirlwind spin.",
                11.0f, 65.0f, 95.0f, 0.14f, 0.18f, 0.0f,
                60.0f, 95.0f, 360.0f, 0.20f, 0.12f, 1.80f, 20.0f,
                14.0f, 0.50f,
                0, 1.0f, 0.0f, 1.0f, 0.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "Dual Daggers +1";
                stats.lightDamage = 14.0f;
                stats.heavyDamage = 72.0f;
            }
            if (upgradeLevel >= 2) {
                stats.upgradeLevel = 2;
                stats.name = "Dual Daggers +2";
                stats.lightDamage = 17.0f;
                stats.heavyDamage = 84.0f;
                stats.critMultiplier = 1.10f;
            }
            return stats;
        case WeaponType::MACE:
            stats = {
                "War Mace",
                "Crushing heavy warhammer. Basic attack smacks in a frontal circle. Heavy attack charges an amplified smack into a 360 shockwave ignoring 20% defense.",
                28.0f, 80.0f, 85.0f, 0.28f, 0.52f, 22.0f,
                96.0f, 150.0f, 360.0f, 0.45f, 0.35f, 3.60f, 40.0f,
                26.0f, 1.25f,
                0, 1.0f, 0.0f, 1.0f, 0.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "War Mace +1";
                stats.lightDamage = 34.0f;
                stats.heavyDamage = 112.0f;
            }
            if (upgradeLevel >= 2) {
                stats.upgradeLevel = 2;
                stats.name = "War Mace +2";
                stats.lightDamage = 40.0f;
                stats.heavyDamage = 128.0f;
                stats.defensePierceBonus = 0.10f;
            }
            return stats;
        case WeaponType::TRUTH_SEEKER:
            stats = {
                "Truth Seeker (Partial)",
                "Legendary holy broadsword fragment containing 1% of the primordial blade power. Features a fluid 2-hit combo: horizontal sweep into upward radiant slice, and dual 360 dawnfire heavy swings firing a 10% defense piercing beam.",
                26.0f, 98.0f, 130.0f, 0.22f, 0.26f, 14.0f,
                78.0f, 145.0f, 360.0f, 0.35f, 0.40f, 3.80f, 36.0f,
                16.0f, 0.65f,
                0, 1.0f, 0.0f, 1.0f, 22.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "Truth Seeker [Consecrated]";
                stats.lightDamage = 32.0f;
                stats.heavyDamage = 96.0f;
                stats.holyBeamDamage = 28.0f;
            }
            return stats;
        case WeaponType::HALBERD:
            stats = {
                "Royal Halberd",
                "Vanguard polearm with extended reach. Light attack executes a linear piercing thrust impaling foes. Heavy attack spins a wide 220-deg cleave followed by a forward vault thrust.",
                24.0f, 140.0f, 45.0f, 0.22f, 0.38f, 16.0f,
                88.0f, 155.0f, 220.0f, 0.30f, 0.35f, 2.60f, 32.0f,
                18.0f, 0.80f,
                0, 1.25f, 0.10f, 1.0f, 0.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "Royal Halberd +1";
                stats.lightDamage = 30.0f;
                stats.heavyDamage = 104.0f;
            }
            if (upgradeLevel >= 2) {
                stats.upgradeLevel = 2;
                stats.name = "Royal Halberd +2";
                stats.lightDamage = 36.0f;
                stats.heavyDamage = 120.0f;
                stats.defensePierceBonus = 0.20f;
                stats.poiseMultiplier = 1.35f;
            }
            return stats;
        case WeaponType::SWORD:
        default:
            stats = {
                "Broadsword",
                "Balanced martial blade. Heavy attack executes a wide cleave followed by a brutal upward shock slash.",
                18.0f, 75.0f, 110.0f, 0.22f, 0.35f, 15.0f,
                70.0f, 125.0f, 160.0f, 0.22f, 0.38f, 2.10f, 32.0f,
                20.0f, 0.85f,
                0, 1.0f, 0.0f, 1.0f, 0.0f
            };
            if (upgradeLevel >= 1) {
                stats.upgradeLevel = 1;
                stats.name = "Broadsword +1";
                stats.lightDamage = 22.0f;
                stats.heavyDamage = 80.0f;
            }
            if (upgradeLevel >= 2) {
                stats.upgradeLevel = 2;
                stats.name = "Broadsword +2";
                stats.lightDamage = 26.0f;
                stats.heavyDamage = 90.0f;
                stats.poiseMultiplier = 1.05f;
            }
            return stats;
    }
}
