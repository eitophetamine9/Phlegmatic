#pragma once
#include "raylib.h"
#include "CombatTypes.hpp"
#include <string>

struct TotemStats {
    std::string name;
    std::string description;
    std::string buffText;
    std::string nerfText;

    float damageMultiplier;       // Multiplier for light & heavy attack damage
    float defenseMultiplier;      // Multiplier for effective player armor defense
    float speedMultiplier;        // Multiplier for 8-dir movement speed
    float staminaRegenMultiplier; // Multiplier for neutral stamina recovery rate
    float cooldownMultiplier;     // Multiplier for attack/dash cooldowns (< 1.0 is faster)
    float willBonus;              // Bonus to Will stat for stagger mitigation

    Color glowColor;
};

inline TotemStats GetTotemStats(TotemType type) {
    switch (type) {
        case TotemType::TOTEM_SANITY:
            return {
                "Totem of Sanity",
                "An uncorrupted relic of Zyphden recovered from the hidden ruins along the Crossroads path. Channels resolute mental clarity.",
                "+15% Overall Damage, +15 Will",
                "None (Pure Relic)",
                1.15f, 1.0f, 1.0f, 1.0f, 1.0f, 15.0f,
                Color{ 100, 235, 180, 255 }
            };
        case TotemType::TOTEM_INSANITY:
            return {
                "Totem of Insanity",
                "A pulsating crimson effigy whispering mad battle hymns. Amplifies strike ferocity at the cost of defensive guard.",
                "+30% Overall Damage",
                "-10% Armor Defense",
                1.30f, 0.90f, 1.0f, 1.0f, 1.0f, 0.0f,
                Color{ 255, 65, 80, 255 }
            };
        case TotemType::TOTEM_MIST:
            return {
                "Totem of Mist",
                "A phantom talisman swirling with vapor. Accelerates breath recovery and blade reflexes at slight sacrifice to physical impact.",
                "+20% Stamina Regen, +5% Cooldown Speed, +5 Will",
                "-5% Overall Damage",
                0.95f, 1.0f, 1.0f, 1.20f, 0.95f, 5.0f,
                Color{ 140, 210, 255, 255 }
            };
        case TotemType::CRUSADER_TOTEM:
            return {
                "Crusader's Totem",
                "An engraved stone talisman blessed by Zyphden high marshals. Hardens armor plate against devastating blows, but weighs upon the bearer.",
                "+30% Armor Defense, +20 Will",
                "-10% Movement Speed",
                1.0f, 1.30f, 0.90f, 1.0f, 1.0f, 20.0f,
                Color{ 255, 215, 80, 255 }
            };
        case TotemType::ELEMENTAL_CORE_TOTEM:
            return {
                "Elemental Core Totem",
                "A core of primordial magma, frost, and basalt. Awakens a loyal Mini Elemental Golem companion (105 HP, 14 DMG) and unleashes infinitely piercing elemental bolts (54 DMG, 3s CD).",
                "Friendly Mini Golem Companion, Infinitely Piercing Projectile (54 DMG, 3s CD)",
                "Companion Dies: 60s Respawn | -10% Movement Speed",
                1.0f, 1.25f, 0.90f, 1.0f, 1.0f, 25.0f,
                Color{ 255, 140, 45, 255 }
            };
        case TotemType::NONE:
        default:
            return {
                "None Equipped",
                "No totem talisman active. Equip a discovered totem relic to channel combat traits.",
                "None",
                "None",
                1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                Color{ 120, 140, 160, 200 }
            };
    }
}
