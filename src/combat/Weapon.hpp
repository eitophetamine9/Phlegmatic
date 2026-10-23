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
};

inline WeaponStats GetWeaponStats(WeaponType type) {
    switch (type) {
        case WeaponType::NONE:
            return {
                "Unarmed",
                "Empty hands. Search the castle courtyard or ancient altar to claim a weapon.",
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                15.0f, 0.75f
            };
        case WeaponType::DAGGERS:
            return {
                "Dual Daggers",
                "Swift twin-blade flurry with 0 stamina light attacks. Heavy attack executes a high-speed piercing lunge into an instant 360 whirlwind spin.",
                22.0f, 65.0f, 95.0f, 0.14f, 0.18f, 0.0f,
                110.0f, 95.0f, 360.0f, 0.20f, 0.12f, 1.80f, 20.0f,
                14.0f, 0.50f
            };
        case WeaponType::MACE:
            return {
                "War Mace",
                "Crushing heavy warhammer. Basic attack smacks in a frontal circle. Heavy attack charges an amplified smack into a 360 shockwave ignoring 25% defense.",
                52.0f, 80.0f, 85.0f, 0.28f, 0.52f, 22.0f,
                155.0f, 150.0f, 360.0f, 0.45f, 0.35f, 3.60f, 40.0f,
                26.0f, 1.25f
            };
        case WeaponType::TRUTH_SEEKER:
            return {
                "Truth Seeker",
                "Legendary holy broadsword. Features a fluid 2-hit combo (horizontal sweep into upward radiant slice) and dual 360 dawnfire heavy swings firing a 10% defense piercing beam.",
                46.0f, 98.0f, 130.0f, 0.22f, 0.26f, 14.0f,
                120.0f, 145.0f, 360.0f, 0.35f, 0.40f, 3.80f, 36.0f,
                16.0f, 0.65f
            };
        case WeaponType::SWORD:
        default:
            return {
                "Broadsword",
                "Balanced martial blade. Heavy attack executes a wide cleave followed by a brutal upward shock slash.",
                25.0f, 75.0f, 110.0f, 0.22f, 0.35f, 15.0f,
                85.0f, 125.0f, 160.0f, 0.22f, 0.38f, 2.10f, 32.0f,
                20.0f, 0.85f
            };
    }
}
