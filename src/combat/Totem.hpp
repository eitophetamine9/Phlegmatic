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
    float maxHpBonus;             // Flat bonus or penalty to Max Health

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
                1.15f, 1.0f, 1.0f, 1.0f, 1.0f, 15.0f, 0.0f,
                Color{ 100, 235, 180, 255 }
            };
        case TotemType::TOTEM_INSANITY:
            return {
                "Totem of Insanity",
                "A feral crimson relic carved from the skull horn of the Berserker Troll. Awakens unstoppable berserker bloodlust, surging overall damage and speed, but leaving the bearer terribly vulnerable and reckless to all hostile strikes. When health falls below 40%, blood frenzy awakens, surging overall damage from +65% to +135%.",
                "+65% DMG (+135% DMG below 40% HP), +35% Move Speed, +25% Attack Speed",
                "-50% Armor Defense, Takes x1.5 Extra Damage from all enemy attacks",
                1.65f, 0.50f, 1.35f, 1.30f, 0.75f, 0.0f, 0.0f,
                Color{ 255, 45, 60, 255 }
            };
        case TotemType::TOTEM_MIST:
            return {
                "Totem of Mist",
                "A phantom talisman swirling with woodland vapor. Awakens a loyal Mist Fairy that guides strikes into vital points (+20% Crit Chance, +35% Crit Dmg) and periodically conjures restorative mist healing orbs (+30 HP).",
                "Summons Mist Fairy (+20% Crit, +35% Crit Dmg, Healing Orbs), +20% Stamina Regen",
                "-15% Defense, -25 Max Health",
                1.0f, 0.85f, 1.0f, 1.20f, 0.95f, 10.0f, -25.0f,
                Color{ 110, 240, 225, 255 }
            };
        case TotemType::CRUSADER_TOTEM:
            return {
                "Crusader's Totem",
                "An engraved stone talisman blessed by Zyphden high marshals. Hardens armor plate against devastating blows, boosts fortitude and attack, but slows battle tempo. Under 20% Health, channels Will of Atlas.",
                "+25% Defense, +35 Max HP, +18% Attack | Will of Atlas: 4s Invulnerability at <20% HP (60s CD)",
                "-12% Movement Speed, -20% Attack Speed",
                1.18f, 1.25f, 0.88f, 1.0f, 1.25f, 15.0f, 35.0f,
                Color{ 255, 215, 80, 255 }
            };
        case TotemType::ELEMENTAL_CORE_TOTEM:
            return {
                "Elemental Core Totem",
                "A core of primordial magma, frost, and basalt. Awakens an empowered Mini Elemental Golem (380 HP, 42 DEF) wielding AoE slams, bash knockbacks, and ranged spike throws, plus piercing elemental bolts.",
                "Empowered Mini Golem Companion (Slam AoE, Bash Knockback, Spike Throw), Piercing Projectile (54 DMG)",
                "Companion Dies: 60s Respawn | -10% Movement Speed",
                1.0f, 1.25f, 0.90f, 1.0f, 1.0f, 25.0f, 0.0f,
                Color{ 255, 140, 45, 255 }
            };
        case TotemType::NONE:
        default:
            return {
                "None Equipped",
                "No totem talisman active. Equip a discovered totem relic to channel combat traits.",
                "None",
                "None",
                1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                Color{ 120, 140, 160, 200 }
            };
    }
}
