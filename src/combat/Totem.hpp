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
    int tier = 0;                 // 0 = Tier I, 1 = Tier II, 2 = Tier III (Max)
};

inline TotemStats GetTotemStats(TotemType type, int tier = 0) {
    if (tier < 0) tier = 0;
    if (tier > 2) tier = 2;

    switch (type) {
        case TotemType::TOTEM_SANITY: {
            if (tier == 1) {
                return {
                    "Totem of Sanity [Tier II]",
                    "An illuminated relic of Zyphden recovered from the hidden ruins. Heightens resolute mental clarity and restores stamina.",
                    "+22% Overall Damage, +25 Will, +5% Stamina Regen",
                    "None (Pure Relic)",
                    1.22f, 1.0f, 1.0f, 1.05f, 1.0f, 25.0f, 0.0f,
                    Color{ 100, 235, 180, 255 }, 1
                };
            } else if (tier == 2) {
                return {
                    "Totem of Sanity [Tier III (Mastered)]",
                    "A pristine master relic of unclouded clarity. Elevates strike potency and fortifies maximum vitality.",
                    "+30% Overall Damage, +40 Will, +10% Stamina Regen, +20 Max Health",
                    "None (Pure Relic)",
                    1.30f, 1.0f, 1.0f, 1.10f, 1.0f, 40.0f, 20.0f,
                    Color{ 130, 255, 200, 255 }, 2
                };
            }
            return {
                "Totem of Sanity",
                "An uncorrupted relic of Zyphden recovered from the hidden ruins along the Crossroads path. Channels resolute mental clarity.",
                "+15% Overall Damage, +15 Will",
                "None (Pure Relic)",
                1.15f, 1.0f, 1.0f, 1.0f, 1.0f, 15.0f, 0.0f,
                Color{ 100, 235, 180, 255 }, 0
            };
        }
        case TotemType::TOTEM_INSANITY: {
            if (tier == 1) {
                return {
                    "Totem of Insanity [Tier II]",
                    "A feral crimson skull relic pulsing with heightened berserker fury. Surges offensive potency and evasion while slightly reducing incoming damage vulnerability.",
                    "+80% DMG (+160% DMG below 40% HP), +22% Move Speed, +30% Attack Speed",
                    "-45% Armor Defense, Takes x1.4 Extra Damage from hostile strikes",
                    1.80f, 0.55f, 1.22f, 1.35f, 0.70f, 0.0f, 0.0f,
                    Color{ 255, 55, 70, 255 }, 1
                };
            } else if (tier == 2) {
                return {
                    "Totem of Insanity [Tier III (Mastered)]",
                    "The apocalyptic apex of berserker bloodlust. Massive damage surges to overwhelming proportions.",
                    "+100% DMG (+200% DMG below 40% HP), +26% Move Speed, +35% Attack Speed",
                    "-40% Armor Defense, Takes x1.3 Extra Damage from hostile strikes",
                    2.00f, 0.60f, 1.26f, 1.40f, 0.65f, 0.0f, 0.0f,
                    Color{ 255, 30, 45, 255 }, 2
                };
            }
            return {
                "Totem of Insanity",
                "A feral crimson relic carved from the skull horn of the Berserker Troll. Awakens unstoppable berserker bloodlust, surging overall damage and speed, but leaving the bearer terribly vulnerable and reckless to all hostile strikes. When health falls below 40%, blood frenzy awakens, surging overall damage from +65% to +135%.",
                "+65% DMG (+135% DMG below 40% HP), +18% Move Speed, +25% Attack Speed",
                "-50% Armor Defense, Takes x1.5 Extra Damage from all enemy attacks",
                1.65f, 0.50f, 1.18f, 1.30f, 0.75f, 0.0f, 0.0f,
                Color{ 255, 45, 60, 255 }, 0
            };
        }
        case TotemType::TOTEM_MIST: {
            if (tier == 1) {
                return {
                    "Totem of Mist [Tier II]",
                    "An empowered woodland talisman. The Mist Fairy guides critical strikes with greater lethality and manifests healing orbs every 12 seconds (+45 HP).",
                    "Empowered Mist Fairy (+28% Crit, +50% Crit Dmg, 12s Orbs +45 HP), +15% Stamina Regen, +15 Will",
                    "-5% Movement Speed, -10% Defense, -15 Max Health",
                    1.0f, 0.90f, 0.95f, 1.15f, 1.0f, 15.0f, -15.0f,
                    Color{ 120, 245, 230, 255 }, 1
                };
            } else if (tier == 2) {
                return {
                    "Totem of Mist [Tier III (Mastered)]",
                    "The celestial Arch-Mist Talisman. The Arch-Fairy rapidly creates potent restoration orbs every 8 seconds (+60 HP) with zero defense or health penalty!",
                    "Arch-Mist Fairy (+38% Crit, +70% Crit Dmg, 8s Orbs +60 HP), +20% Stamina Regen, +25 Will",
                    "None (Drawback Cleansed at Tier III)",
                    1.0f, 1.0f, 1.0f, 1.20f, 1.0f, 25.0f, 0.0f,
                    Color{ 140, 255, 240, 255 }, 2
                };
            }
            return {
                "Totem of Mist",
                "A phantom talisman swirling with woodland vapor. Awakens a loyal Mist Fairy that guides strikes into vital points (+20% Crit Chance, +35% Crit Dmg) and periodically conjures restorative mist healing orbs (+30 HP).",
                "Summons Mist Fairy (+20% Crit, +35% Crit Dmg, Healing Orbs), +10% Stamina Regen, +10 Will",
                "-5% Movement Speed, -15% Defense, -25 Max Health",
                1.0f, 0.85f, 0.95f, 1.10f, 1.0f, 10.0f, -25.0f,
                Color{ 110, 240, 225, 255 }, 0
            };
        }
        case TotemType::CRUSADER_TOTEM: {
            if (tier == 1) {
                return {
                    "Crusader's Totem [Tier II]",
                    "A consecrated fortress standard. Bolsters armor defense and reduces Will of Atlas cooldown to 45 seconds.",
                    "+32% Defense, +50 Max HP, +22% Attack | Will of Atlas: 5s Invulnerability at <20% HP (45s CD)",
                    "-8% Movement Speed, -15% Attack Speed",
                    1.22f, 1.32f, 0.92f, 1.0f, 1.18f, 20.0f, 50.0f,
                    Color{ 255, 225, 95, 255 }, 1
                };
            } else if (tier == 2) {
                return {
                    "Crusader's Totem [Tier III (Mastered)]",
                    "The indomitable Aegis of Atlas. Supreme armor fortification and rapid 30s Will of Atlas awakening.",
                    "+40% Defense, +70 Max HP, +28% Attack | Will of Atlas: 6s Invulnerability at <20% HP (30s CD)",
                    "-5% Movement Speed, -10% Attack Speed",
                    1.28f, 1.40f, 0.95f, 1.0f, 1.12f, 30.0f, 70.0f,
                    Color{ 255, 240, 120, 255 }, 2
                };
            }
            return {
                "Crusader's Totem",
                "An engraved stone talisman blessed by Zyphden high marshals. Hardens armor plate against devastating blows, boosts fortitude and attack, but slows battle tempo. Under 20% Health, channels Will of Atlas.",
                "+25% Defense, +35 Max HP, +18% Attack | Will of Atlas: 4s Invulnerability at <20% HP (60s CD)",
                "-12% Movement Speed, -20% Attack Speed",
                1.18f, 1.25f, 0.88f, 1.0f, 1.25f, 15.0f, 35.0f,
                Color{ 255, 215, 80, 255 }, 0
            };
        }
        case TotemType::ELEMENTAL_CORE_TOTEM: {
            if (tier == 1) {
                return {
                    "Elemental Core Totem [Tier II]",
                    "An empowered volcanic heartstone. The Mini Golem gains +25% Health (600 HP, 65 DEF), +6 HP/s passive regeneration, and faster 25s resurrection.",
                    "Empowered Mini Golem (600 HP, 65 DEF, +6/s Regen), +32% Defense, Bearer Bolt (64 DMG, 3s CD)",
                    "Companion Defeated: 25s Respawn | -8% Movement Speed",
                    1.0f, 1.32f, 0.92f, 1.0f, 1.0f, 30.0f, 0.0f,
                    Color{ 255, 155, 60, 255 }, 1
                };
            } else if (tier == 2) {
                return {
                    "Elemental Core Totem [Tier III (EVOLVED)]",
                    "The primordial titan heart awakens! Evolves your minion into the colossal COMMANDING ELEMENTAL GOLEM (920 HP, 75 DEF) which commands and summons 3 MINI GOLEMS (280 HP, 40 DEF, 18 DMG) to fight in formation! Fallen sub-minions reconstitute every 18 seconds.",
                    "COMMANDING TITAN (920 HP, 75 DEF) + SUMMONS 3 MINI GOLEMS, +40% Defense, Bearer Bolt (76 DMG, 2.5s CD)",
                    "Titan Defeated: 20s Respawn | -5% Movement Speed",
                    1.0f, 1.40f, 0.95f, 1.0f, 1.0f, 40.0f, 0.0f,
                    Color{ 255, 175, 40, 255 }, 2
                };
            }
            return {
                "Elemental Core Totem",
                "A core of primordial magma, frost, and basalt. Awakens an empowered Mini Elemental Golem (460 HP, 55 DEF, +4 HP/s Regen) wielding Heavy AoE Slams (36 DMG), Forward Bash Fractures (32 DMG), and Piercing Spike Throws (34 DMG), plus 54 DMG bearer bolts.",
                "Empowered Mini Golem (460 HP, 55 DEF, Slam/Bash/Spike), Bearer Bolt (54 DMG, 3s CD)",
                "Companion Dies: 35s Respawn | -10% Movement Speed",
                1.0f, 1.25f, 0.90f, 1.0f, 1.0f, 25.0f, 0.0f,
                Color{ 255, 140, 45, 255 }, 0
            };
        }
        case TotemType::TOTEM_PHLEGMATIC_CORE: {
            if (tier == 1) {
                return {
                    "Totem of the Phlegmatic Core [Tier II]",
                    "A crystal-sheathed heart of absolute cold poise. Increases Max Health and enhances Gray Health conversion to 35%.",
                    "+50 Max HP, +35 Will, -90% Knockback, 35% Gray Health Recovery, Chills Foes on Hit",
                    "-6% Move Speed, -8% Stamina Regen",
                    1.0f, 1.28f, 0.94f, 0.92f, 1.0f, 35.0f, 50.0f,
                    Color{ 150, 230, 255, 255 }, 1
                };
            } else if (tier == 2) {
                return {
                    "Totem of the Phlegmatic Core [Tier III (Mastered)]",
                    "The glacial bedrock of Stoic apathy. Absolute knockback immunity (-100%), 50% Gray Health recovery, and permanent frost chill aura.",
                    "+75 Max HP, +50 Will, KNOCKBACK IMMUNITY, 50% Gray Health Recovery, Frost Ward Aura",
                    "-5% Move Speed, -5% Stamina Regen",
                    1.0f, 1.36f, 0.95f, 0.95f, 1.0f, 50.0f, 75.0f,
                    Color{ 175, 245, 255, 255 }, 2
                };
            }
            return {
                "Totem of the Phlegmatic Core",
                "The sacred humoral core of Tydeshire's stoic protectors. Imbues the bearer with unyielding cold resolve: -80% knockback received, +30 Max Health, +25 Will, and Phlegmatic Apathy: 25% of all sustained damage becomes recoverable Gray Health by landing counter-attacks. Striking foes chills and slows them.",
                "+30 Max HP, +25 Will, -80% Knockback, Gray Health Recovery on Melee, Chill Frost Counter",
                "-8% Move Speed, -10% Stamina Regen",
                1.0f, 1.20f, 0.92f, 0.90f, 1.0f, 25.0f, 30.0f,
                Color{ 140, 220, 255, 255 }, 0
            };
        }
        case TotemType::CHAMPION_TOTEM: {
            if (tier == 1) {
                return {
                    "Champion's Totem [Tier II]",
                    "An empowered laurel-crested sun effigy. Amplifies martial offense, armor defense, and elevates passive regeneration to +18 HP/s.",
                    "+45% DMG, +38% DEF, +18% Speed, +45% Stamina Regen, -25% CDs, +50 Will, +70 Max HP, +18 HP/s Passive Regen (+4 in combat)",
                    "Withered Vitality: All incoming healing from external sources reduced by 60%",
                    1.45f, 1.38f, 1.18f, 1.45f, 0.75f, 50.0f, 70.0f,
                    Color{ 255, 215, 75, 255 }, 1
                };
            } else if (tier == 2) {
                return {
                    "Champion's Totem [Tier III (Mastered)]",
                    "The glorious Apex Sovereign Standard. Unrivaled warrior majesty with +22 HP/s passive regeneration and softened potion penalty.",
                    "+55% DMG, +45% DEF, +22% Speed, +55% Stamina Regen, -30% CDs, +65 Will, +95 Max HP, +22 HP/s Passive Regen (+6 in combat)",
                    "Withered Vitality: All incoming healing from external sources reduced by 45%",
                    1.55f, 1.45f, 1.22f, 1.55f, 0.70f, 65.0f, 95.0f,
                    Color{ 255, 235, 100, 255 }, 2
                };
            }
            return {
                "Champion's Totem",
                "A colossal laurels-and-sun-crested vanguard effigy forged in the royal gladiatorial pits of Zyphden. Bestowed upon victors who conquer the Royal Combat Arena. Imbues peerless martial prowess, swiftness, and unyielding armor with surged passive regeneration, but demands merciless endurance: external potions and restorative aids are severely suppressed.",
                "+35% DMG, +30% DEF, +15% Speed, +35% Stamina Regen, -20% CDs, +40 Will, +50 Max HP, +14 HP/s Passive Regen (+2 in combat)",
                "Withered Vitality: All incoming healing from potions, drops, and external sources reduced by 70%",
                1.35f, 1.30f, 1.15f, 1.35f, 0.80f, 40.0f, 50.0f,
                Color{ 245, 200, 60, 255 }, 0
            };
        }
        case TotemType::COMMANDER_TOTEM: {
            if (tier == 1) {
                return {
                    "Commander's Totem [Tier II]",
                    "An empowered obsidian standard. Strengthens your 7 gladiators (+15% HP/DEF, 55% strike power) and lowers squad recovery time to 90 seconds.",
                    "Summons 7 Tower Gladiators (115% HP/DEF, 55% DMG), +28% DEF, +60 Max HP, +35 Will",
                    "-40% Player Attack Damage | All 7 summons must perish before 90s resurrection begins",
                    0.60f, 1.28f, 1.0f, 1.0f, 1.0f, 35.0f, 60.0f,
                    Color{ 205, 65, 145, 255 }, 1
                };
            } else if (tier == 2) {
                return {
                    "Commander's Totem [Tier III (Mastered)]",
                    "The Royal Imperial Vanguard Standard. Commands elite gladiators with supreme resilience (135% HP/DEF, 70% strike power, +15% move speed) and a rapid 60s squad revival!",
                    "ROYAL IMPERIAL VANGUARD: 7 Gladiators (135% HP/DEF, 70% DMG, +15% Speed), +36% DEF, +85 Max HP, +50 Will",
                    "-40% Player Attack Damage | All 7 summons must perish before 60s resurrection begins",
                    0.60f, 1.36f, 1.0f, 1.0f, 1.0f, 50.0f, 85.0f,
                    Color{ 225, 80, 170, 255 }, 2
                };
            }
            return {
                "Commander's Totem",
                "A monolithic obsidian standard forged in the crucible of the 25-Wave Royal Combat Arena. Summons an undying vanguard of 7 Tower gladiators who stick close and fight by your side. Companions retain 100% of their base HP and defense with lowered offensive strike power (~40%). Bearer's direct attack damage is reduced by 40% as the legion wages war on your behalf. If all 7 fall in battle, the squad reconstitutes after a 2-minute recovery period.",
                "Summons 7 Tower Gladiators (100% HP/DEF, 40% DMG), +20% DEF, +40 Max HP, +25 Will",
                "-40% Player Attack Damage | All 7 summons must perish before 2-min resurrection begins",
                0.60f, 1.20f, 1.0f, 1.0f, 1.0f, 25.0f, 40.0f,
                Color{ 185, 45, 120, 255 }, 0
            };
        }
        case TotemType::NONE:
        default:
            return {
                "None Equipped",
                "No totem talisman active. Equip a discovered totem relic to channel combat traits.",
                "None",
                "None",
                1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                Color{ 120, 140, 160, 200 }, 0
            };
    }
}
