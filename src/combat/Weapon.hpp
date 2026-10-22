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
        case WeaponType::DAGGERS:
            return {
                "Dual Daggers",
                "Fast twin-slash flurry. 360 whirlwind heavy. Swift dash with reduced stamina cost.",
                18.0f, 62.0f, 95.0f, 0.14f, 0.20f, 9.0f,
                52.0f, 75.0f, 360.0f, 0.12f, 0.24f, 1.20f, 22.0f,
                14.0f, 0.50f
            };
        case WeaponType::MACE:
            return {
                "War Mace",
                "Heavy blunt crusher. Massive stagger poise damage and earth-shattering slam.",
                40.0f, 82.0f, 85.0f, 0.28f, 0.45f, 22.0f,
                95.0f, 135.0f, 150.0f, 0.26f, 0.38f, 3.20f, 42.0f,
                26.0f, 1.25f
            };
        case WeaponType::SWORD:
        default:
            return {
                "Broadsword",
                "Balanced martial blade. Reliable reach, sweeping arcs, and steady recovery.",
                25.0f, 75.0f, 110.0f, 0.22f, 0.35f, 15.0f,
                65.0f, 115.0f, 140.0f, 0.18f, 0.30f, 1.90f, 32.0f,
                20.0f, 0.85f
            };
    }
}
