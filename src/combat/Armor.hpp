#pragma once
#include "raylib.h"
#include "CombatTypes.hpp"
#include <string>

struct ArmorStats {
    std::string name;
    std::string description;
    float defense;
    float maxHpBonus;
    float speedModifier;
    float staminaRegenModifier;
    float willBonus;

    // Visual Palette for Character Silhouette Rendering
    Color primaryColor;
    Color trimColor;
    Color helmColor;
    Color visorColor;
};

inline ArmorStats GetArmorStats(ArmorType type) {
    switch (type) {
        case ArmorType::COMMONERS_SCRAP:
            return {
                "Commoner's Scrap",
                "Padded linen tunic reinforced with salvage iron studs found in the castle ruins. Light and unrestrictive.",
                6.0f, 0.0f, 1.0f, 1.0f, 15.0f,
                Color{ 130, 90, 60, 255 },     // Rustic weathered tunic
                Color{ 120, 130, 145, 255 },   // Salvaged iron rivets
                Color{ 85, 65, 48, 255 },      // Cloth leather cowl
                Color{ 255, 195, 80, 255 }     // Amber visor
            };
        case ArmorType::CRUSADER_PLATE:
            return {
                "Crusader Plate",
                "Tempered royal steel plate bearing Zyphden's golden heraldry. Awarded after besting Knight Lenon in the royal courtyard.",
                24.0f, 30.0f, 0.95f, 0.95f, 45.0f,
                Color{ 32, 50, 85, 255 },      // Royal knight navy
                Color{ 225, 185, 65, 255 },    // Gilded golden pauldrons
                Color{ 150, 170, 195, 255 },   // Polished steel helm
                Color{ 255, 230, 90, 255 }     // Radiant gold visor
            };
        case ArmorType::VANGUARD_MAIL:
            return {
                "Vanguard Bastion Mail",
                "Heavy interlocking iron rings and massive pauldron plates designed to shrug off siege-engine shocks.",
                32.0f, 50.0f, 0.90f, 0.85f, 40.0f,
                Color{ 38, 42, 54, 255 },      // Heavy dark mail
                Color{ 75, 85, 105, 255 },     // Fortified iron pauldrons
                Color{ 30, 35, 46, 255 },      // Full-face brute helm
                Color{ 245, 75, 75, 255 }      // Crimson visor slit
            };
        case ArmorType::SHADOW_LEATHER:
            return {
                "Shadowstrider Garb",
                "Boiled midnight-leather tailored for evasive footwork and silent assassination across crumbling ruins.",
                8.0f, 0.0f, 1.12f, 1.25f, 10.0f,
                Color{ 24, 28, 40, 255 },      // Dark shadow-weave
                Color{ 95, 70, 135, 255 },     // Violet rune trim
                Color{ 18, 22, 32, 255 },      // Shadow cowl
                Color{ 60, 240, 175, 255 }     // Spectral emerald visor
            };
        case ArmorType::NONE:
        default:
            return {
                "Adventurer's Tunic",
                "Standard traveler garments offering minimal physical defense against blade strikes.",
                0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                Color{ 40, 110, 210, 255 },    // Classic blue tunic
                Color{ 80, 95, 130, 255 },     // Light pauldrons
                Color{ 48, 58, 80, 255 },      // Iron helm
                Color{ 0, 229, 255, 255 }      // Cyan visor
            };
    }
}
