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
                "+65% DMG (+135% DMG below 40% HP), +18% Move Speed, +25% Attack Speed",
                "-50% Armor Defense, Takes x1.5 Extra Damage from all enemy attacks",
                1.65f, 0.50f, 1.18f, 1.30f, 0.75f, 0.0f, 0.0f,
                Color{ 255, 45, 60, 255 }
            };
        case TotemType::TOTEM_MIST:
            return {
                "Totem of Mist",
                "A phantom talisman swirling with woodland vapor. Awakens a loyal Mist Fairy that guides strikes into vital points (+20% Crit Chance, +35% Crit Dmg) and periodically conjures restorative mist healing orbs (+30 HP).",
                "Summons Mist Fairy (+20% Crit, +35% Crit Dmg, Healing Orbs), +10% Stamina Regen, +10 Will",
                "-5% Movement Speed, -15% Defense, -25 Max Health",
                1.0f, 0.85f, 0.95f, 1.10f, 1.0f, 10.0f, -25.0f,
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
                "A core of primordial magma, frost, and basalt. Awakens an empowered Mini Elemental Golem (460 HP, 55 DEF, +4 HP/s Regen) wielding Heavy AoE Slams (36 DMG), Forward Bash Fractures (32 DMG), and Piercing Spike Throws (34 DMG), plus 54 DMG bearer bolts.",
                "Empowered Mini Golem (460 HP, 55 DEF, Slam/Bash/Spike), Bearer Bolt (54 DMG, 3s CD)",
                "Companion Dies: 35s Respawn | -10% Movement Speed",
                1.0f, 1.25f, 0.90f, 1.0f, 1.0f, 25.0f, 0.0f,
                Color{ 255, 140, 45, 255 }
            };
        case TotemType::TOTEM_PHLEGMATIC_CORE:
            return {
                "Totem of the Phlegmatic Core",
                "The sacred humoral core of Tydeshire's stoic protectors. Imbues the bearer with unyielding cold resolve: -80% knockback received, +30 Max Health, +25 Will, and Phlegmatic Apathy: 25% of all sustained damage becomes recoverable Gray Health by landing counter-attacks. Striking foes chills and slows them.",
                "+30 Max HP, +25 Will, -80% Knockback, Gray Health Recovery on Melee, Chill Frost Counter",
                "-8% Move Speed, -10% Stamina Regen",
                1.0f, 1.20f, 0.92f, 0.90f, 1.0f, 25.0f, 30.0f,
                Color{ 140, 220, 255, 255 }
            };
        case TotemType::CHAMPION_TOTEM:
            return {
                "Champion's Totem",
                "A colossal laurels-and-sun-crested vanguard effigy forged in the royal gladiatorial pits of Zyphden. Bestowed upon victors who conquer the Royal Combat Arena. Imbues peerless martial prowess, swiftness, and unyielding armor with surged passive regeneration, but demands merciless endurance: external potions and restorative aids are severely suppressed.",
                "+35% DMG, +30% DEF, +15% Speed, +35% Stamina Regen, -20% CDs, +40 Will, +50 Max HP, +14 HP/s Passive Regen (+2 in combat)",
                "Withered Vitality: All incoming healing from potions, drops, and external sources reduced by 70%",
                1.35f, 1.30f, 1.15f, 1.35f, 0.80f, 40.0f, 50.0f,
                Color{ 245, 200, 60, 255 }
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
