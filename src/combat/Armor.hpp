#pragma once
#include "raylib.h"
#include "CombatTypes.hpp"
#include <string>

struct HelmetStats {
    std::string name;
    std::string description;
    float defense;
    float maxHpBonus;
    float speedModifier;
    float staminaRegenModifier;
    float willBonus;
    Color helmColor;
    Color visorColor;
    ArmorSet setAffinity;
};

struct ChestplateStats {
    std::string name;
    std::string description;
    float defense;
    float maxHpBonus;
    float speedModifier;
    float staminaRegenModifier;
    float willBonus;
    Color primaryColor;
    Color trimColor;
    ArmorSet setAffinity;
};

struct LeggingsStats {
    std::string name;
    std::string description;
    float defense;
    float maxHpBonus;
    float speedModifier;
    float staminaRegenModifier;
    float willBonus;
    Color legColor;
    Color bootColor;
    ArmorSet setAffinity;
};

struct ArmorSetInfo {
    ArmorSet set;
    std::string name;
    std::string passiveName;
    std::string passiveDescription;
    std::string skillName;
    std::string skillDescription;
    float skillCooldown;
};

// Legacy ArmorStats struct for backward compatibility
struct ArmorStats {
    std::string name;
    std::string description;
    float defense;
    float maxHpBonus;
    float speedModifier;
    float staminaRegenModifier;
    float willBonus;

    Color primaryColor;
    Color trimColor;
    Color helmColor;
    Color visorColor;
};

inline HelmetStats GetHelmetStats(HelmetType type) {
    switch (type) {
        case HelmetType::COMMONERS_HOOD:
            return {
                "Commoner's Cowl",
                "Weathered cloth and salvage leather cowl that muffles incoming blade blows.",
                2.0f, 0.0f, 1.0f, 1.0f, 8.0f,
                Color{ 85, 65, 48, 255 }, Color{ 255, 195, 80, 255 }, ArmorSet::COMMONERS_SCRAP
            };
        case HelmetType::CRUSADER_HELM:
            return {
                "Crusader Sallet",
                "Tempered royal steel sallet adorned with gilded filigree, shielding against concussions.",
                6.0f, 5.0f, 0.98f, 0.98f, 25.0f,
                Color{ 150, 170, 195, 255 }, Color{ 255, 230, 90, 255 }, ArmorSet::CRUSADER_PLATE
            };
        case HelmetType::VANGUARD_GREATHELM:
            return {
                "Vanguard Heavy Greathelm",
                "Impenetrable full-face iron visor built to withstand battering rams and dragonfire.",
                8.0f, 10.0f, 0.97f, 0.95f, 20.0f,
                Color{ 30, 35, 46, 255 }, Color{ 245, 75, 75, 255 }, ArmorSet::VANGUARD_BASTION
            };
        case HelmetType::SHADOW_COWL:
            return {
                "Shadowstrider Cowl",
                "Dark-weave assassin's hood that blends into the damp dungeon fog.",
                2.0f, 0.0f, 1.03f, 1.15f, 6.0f,
                Color{ 18, 22, 32, 255 }, Color{ 60, 240, 175, 255 }, ArmorSet::SHADOWSTRIDER
            };
        case HelmetType::SCRAP_SALLET:
            return {
                "Scrap-Iron Sallet",
                "Crude forged iron cap beaten together at Varek's anvil from quarry scrap.",
                6.0f, 0.0f, 1.0f, 1.0f, 15.0f,
                Color{ 110, 115, 125, 255 }, Color{ 240, 180, 70, 255 }, ArmorSet::FORGED_SCRAP_IRON
            };
        case HelmetType::BASALT_GREATHELM:
            return {
                "Basalt Golem Greathelm",
                "Carved volcanic stone crest imbued with primordial earth resonance.",
                10.0f, 15.0f, 0.97f, 0.95f, 25.0f,
                Color{ 45, 50, 58, 255 }, Color{ 255, 150, 40, 255 }, ArmorSet::BASALT_GOLEM
            };
        case HelmetType::FROSTWARDEN_HOOD:
            return {
                "Frostwarden Fur Cowl",
                "Polar bear and troll fur-trimmed iron cowl providing supreme warmth and concussion resistance.",
                7.0f, 15.0f, 1.02f, 1.04f, 20.0f,
                Color{ 38, 56, 72, 255 }, Color{ 140, 220, 245, 255 }, ArmorSet::FROSTWARDEN
            };
        case HelmetType::NONE:
        default:
            return {
                "Bare Head",
                "No headwear equipped. Vulnerable to direct blunt strikes.",
                0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                Color{ 48, 58, 80, 255 }, Color{ 0, 229, 255, 255 }, ArmorSet::NONE
            };
    }
}

inline ChestplateStats GetChestplateStats(ChestplateType type) {
    switch (type) {
        case ChestplateType::COMMONERS_TUNIC:
            return {
                "Commoner's Tunic",
                "Padded linen gambeson reinforced with salvage iron studs found in castle ruins.",
                3.0f, 0.0f, 1.0f, 1.0f, 4.0f,
                Color{ 130, 90, 60, 255 }, Color{ 120, 130, 145, 255 }, ArmorSet::COMMONERS_SCRAP
            };
        case ChestplateType::CRUSADER_CUIRASS:
            return {
                "Crusader Cuirass",
                "Tempered royal steel breastplate bearing Zyphden's sun crest.",
                12.0f, 20.0f, 0.98f, 0.98f, 12.0f,
                Color{ 32, 50, 85, 255 }, Color{ 225, 185, 65, 255 }, ArmorSet::CRUSADER_PLATE
            };
        case ChestplateType::VANGUARD_CHEST:
            return {
                "Vanguard Bastion Mail",
                "Interlocking iron hauberk and heavy pauldron plates built for front-line sieges.",
                16.0f, 30.0f, 0.95f, 0.92f, 12.0f,
                Color{ 38, 42, 54, 255 }, Color{ 75, 85, 105, 255 }, ArmorSet::VANGUARD_BASTION
            };
        case ChestplateType::SHADOW_GARB:
            return {
                "Shadowstrider Doublet",
                "Supple midnight-leather vest tailored for silent footwork and agile blade dances.",
                4.0f, 0.0f, 1.04f, 1.15f, 2.0f,
                Color{ 24, 28, 40, 255 }, Color{ 95, 70, 135, 255 }, ArmorSet::SHADOWSTRIDER
            };
        case ChestplateType::SCRAP_CUIRASS:
            return {
                "Forged Scrap Cuirass",
                "Riveted iron slag plates hammered together, providing stout physical resilience.",
                10.0f, 15.0f, 1.0f, 1.0f, 8.0f,
                Color{ 85, 90, 100, 255 }, Color{ 160, 165, 175, 255 }, ArmorSet::FORGED_SCRAP_IRON
            };
        case ChestplateType::BASALT_CUIRASS:
            return {
                "Basalt Plated Bastion",
                "Dense slabs of volcanic basalt bound with tempered iron bands.",
                18.0f, 30.0f, 0.95f, 0.92f, 15.0f,
                Color{ 40, 44, 52, 255 }, Color{ 210, 110, 30, 255 }, ArmorSet::BASALT_GOLEM
            };
        case ChestplateType::FROSTWARDEN_HAUBERK:
            return {
                "Frostwarden Pelt Hauberk",
                "Heavy riveted iron breastplate draped with thick white beast fur and jagged bone studs.",
                15.0f, 25.0f, 0.98f, 1.02f, 18.0f,
                Color{ 32, 48, 64, 255 }, Color{ 210, 235, 250, 255 }, ArmorSet::FROSTWARDEN
            };
        case ChestplateType::NONE:
        default:
            return {
                "Adventurer's Tunic",
                "Standard traveler garments offering minimal physical defense.",
                0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                Color{ 40, 110, 210, 255 }, Color{ 80, 95, 130, 255 }, ArmorSet::NONE
            };
    }
}

inline LeggingsStats GetLeggingsStats(LeggingsType type) {
    switch (type) {
        case LeggingsType::COMMONERS_BREECHES:
            return {
                "Commoner's Breeches",
                "Rough wool breeches bound with sturdy leather straps.",
                1.0f, 0.0f, 1.0f, 1.0f, 3.0f,
                Color{ 95, 70, 50, 255 }, Color{ 60, 45, 35, 255 }, ArmorSet::COMMONERS_SCRAP
            };
        case LeggingsType::CRUSADER_GREAVES:
            return {
                "Crusader Greaves",
                "Gilded steel shin-guards and sollerets allowing steady knightly footing.",
                6.0f, 5.0f, 0.99f, 0.99f, 8.0f,
                Color{ 45, 60, 95, 255 }, Color{ 200, 165, 55, 255 }, ArmorSet::CRUSADER_PLATE
            };
        case LeggingsType::VANGUARD_GREAVES:
            return {
                "Vanguard Heavy Legguards",
                "Massive banded iron greaves that anchor the bearer against crushing knockback.",
                8.0f, 10.0f, 0.98f, 0.96f, 8.0f,
                Color{ 48, 52, 65, 255 }, Color{ 65, 75, 95, 255 }, ArmorSet::VANGUARD_BASTION
            };
        case LeggingsType::SHADOW_LEGGINGS:
            return {
                "Shadowstrider Tights",
                "Ultra-light treated leather breeches permitting silent and swift dashing.",
                2.0f, 0.0f, 1.03f, 1.15f, 2.0f,
                Color{ 20, 24, 35, 255 }, Color{ 80, 60, 115, 255 }, ArmorSet::SHADOWSTRIDER
            };
        case LeggingsType::SCRAP_GREAVES:
            return {
                "Reinforced Scrap Greaves",
                "Hardened iron shins crafted from bent quarry plates.",
                5.0f, 5.0f, 1.02f, 1.0f, 5.0f,
                Color{ 75, 80, 90, 255 }, Color{ 130, 135, 145, 255 }, ArmorSet::FORGED_SCRAP_IRON
            };
        case LeggingsType::BASALT_GREAVES:
            return {
                "Basalt Heavy Greaves",
                "Monolithic volcanic stone greaves that crush stones with every step.",
                8.0f, 10.0f, 0.97f, 0.95f, 10.0f,
                Color{ 35, 38, 45, 255 }, Color{ 180, 95, 25, 255 }, ArmorSet::BASALT_GOLEM
            };
        case LeggingsType::FROSTWARDEN_GREAVES:
            return {
                "Frostwarden Spiked Greaves",
                "Spiked steel-plated boots lined with fur, granting sure footing on sheer icy scree.",
                7.0f, 10.0f, 1.0f, 1.0f, 12.0f,
                Color{ 42, 54, 70, 255 }, Color{ 160, 205, 230, 255 }, ArmorSet::FROSTWARDEN
            };
        case LeggingsType::NONE:
        default:
            return {
                "Traveler's Trousers",
                "Simple cloth trousers with basic leather boots.",
                0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                Color{ 50, 60, 75, 255 }, Color{ 35, 42, 55, 255 }, ArmorSet::NONE
            };
    }
}

inline ArmorSet GetMatchingArmorSet(HelmetType h, ChestplateType c, LeggingsType l) {
    if (h == HelmetType::COMMONERS_HOOD && c == ChestplateType::COMMONERS_TUNIC && l == LeggingsType::COMMONERS_BREECHES)
        return ArmorSet::COMMONERS_SCRAP;
    if (h == HelmetType::CRUSADER_HELM && c == ChestplateType::CRUSADER_CUIRASS && l == LeggingsType::CRUSADER_GREAVES)
        return ArmorSet::CRUSADER_PLATE;
    if (h == HelmetType::VANGUARD_GREATHELM && c == ChestplateType::VANGUARD_CHEST && l == LeggingsType::VANGUARD_GREAVES)
        return ArmorSet::VANGUARD_BASTION;
    if (h == HelmetType::SHADOW_COWL && c == ChestplateType::SHADOW_GARB && l == LeggingsType::SHADOW_LEGGINGS)
        return ArmorSet::SHADOWSTRIDER;
    if (h == HelmetType::SCRAP_SALLET && c == ChestplateType::SCRAP_CUIRASS && l == LeggingsType::SCRAP_GREAVES)
        return ArmorSet::FORGED_SCRAP_IRON;
    if (h == HelmetType::BASALT_GREATHELM && c == ChestplateType::BASALT_CUIRASS && l == LeggingsType::BASALT_GREAVES)
        return ArmorSet::BASALT_GOLEM;
    if (h == HelmetType::FROSTWARDEN_HOOD && c == ChestplateType::FROSTWARDEN_HAUBERK && l == LeggingsType::FROSTWARDEN_GREAVES)
        return ArmorSet::FROSTWARDEN;
    return ArmorSet::NONE;
}

inline ArmorSetInfo GetArmorSetInfo(ArmorSet set) {
    switch (set) {
        case ArmorSet::COMMONERS_SCRAP:
            return {
                ArmorSet::COMMONERS_SCRAP,
                "Commoner's Scrap",
                "Scrapper's Versatility",
                "+20% Stamina Regen & +15% Loot Find. [Sword Synergy]: +15% attack speed and -25% stamina cost.",
                "Desperate Dust Burst",
                "Slams pocket dust into the ground: stuns adjacent enemies for 1.5s & grants +35% move speed for 4s.",
                30.0f
            };
        case ArmorSet::CRUSADER_PLATE:
            return {
                ArmorSet::CRUSADER_PLATE,
                "Crusader Plate",
                "Aegis of the Dawnfire",
                "+20% Physical Defense & +25 Will. [Truth Seeker & Sword Synergy]: Attacks deal +25% holy damage, and holy beams pierce +20% defense.",
                "Dawnfire Aegis",
                "Envelops player in a radiant golden bubble absorbing 60 damage for 6s. Detonates in a holy stun flash!",
                35.0f
            };
        case ArmorSet::VANGUARD_BASTION:
            return {
                ArmorSet::VANGUARD_BASTION,
                "Vanguard Bastion",
                "Unstoppable Phalanx",
                "+50 Max HP & unbreakable hyper-armor during attacks. [Halberd & Glaive Synergy]: Reach extended +35px, +25% armor penetration, and cleaves unleash concussive shockwaves.",
                "Phalanx Vanguard Charge",
                "Unstoppable shield & polearm rush (180px) dealing 65 physical damage, crushing enemy armor (-30% for 5s), and granting 4s Fortification shield (+30% defense).",
                22.0f
            };
        case ArmorSet::SHADOWSTRIDER:
            return {
                ArmorSet::SHADOWSTRIDER,
                "Shadowstrider Garb",
                "Veil of the Mist",
                "+10% Set Move Speed (~21% total w/ pieces), -30% Dash Stamina Cost, +0.10s Dodge i-frames. [Daggers & Bow Synergy]: +15% Crit Chance. Perfect evades have a 15% chance to trigger an AoE Smoke Mist that increases evasion and poisons enemies inside.",
                "Shadow Decoy & Vanish",
                "Vaporizes into mist with 0.5s invulnerability. Leaves an illusion decoy drawing aggro before exploding for 40 dmg.",
                25.0f
            };
        case ArmorSet::FORGED_SCRAP_IRON:
            return {
                ArmorSet::FORGED_SCRAP_IRON,
                "Forged Scrap Iron",
                "Tempered Ironclad",
                "+18 Physical Defense & +20% Poise Damage. [War Mace Synergy]: Heavy attacks inflict -25% Armor Fracture, and light hits generate +50% extra Composure.",
                "Ironclad Spike Wall",
                "Jagged metal spikes erupt from armor for 6s, reflecting 40% melee damage & granting hyper-armor.",
                25.0f
            };
        case ArmorSet::BASALT_GOLEM:
            return {
                ArmorSet::BASALT_GOLEM,
                "Basalt Golem Carapace",
                "Primordial Titan Carapace",
                "+35 Physical Defense, 70% knockback resist, absorbs 1 heavy strike every 20s. [Sun Claymore Synergy]: Heavy attacks unleash a secondary molten shockwave (+30 earth dmg) with unbreakable hyper-armor.",
                "Basalt Spire Wall",
                "Slams ground erecting 3 jagged basalt pillars blocking pathing and enemy projectiles while dealing 55 dmg.",
                30.0f
            };
        case ArmorSet::FROSTWARDEN:
            return {
                ArmorSet::FROSTWARDEN,
                "Frostwarden Attire",
                "Glacial Tenacity",
                "+30% Cold Resistance & immunity to Frostbite chill/slow. Heavy attacks deal +20% poise damage. [Glacial Cleaver Synergy]: Heavy finisher slams unleash Frost Tremor (+35 frost damage, 175px radius, 4s enemy chill).",
                "FROST NOVA",
                "Unleashes a 190px crystalline Frost Nova dealing 75 frost damage, stunning non-bosses for 1.2s, infusing 4s Chill and -20% Armor Fracture, and bestowing a 6s Ice Ward (45 HP shield that retaliates against melee hits for 18 frost dmg & 3s Chill).",
                28.0f
            };
        case ArmorSet::NONE:
        default:
            return {
                ArmorSet::NONE,
                "No Set Bonus",
                "Mismatched Attire",
                "Equip 3 matching set pieces to awaken passive set bonuses and special activated skills.",
                "None",
                "No armor skill available.",
                0.0f
            };
    }
}

// Composite visuals helper
inline ArmorStats GetCompositeArmorVisuals(HelmetType helm, ChestplateType chest, LeggingsType legs) {
    HelmetStats hs = GetHelmetStats(helm);
    ChestplateStats cs = GetChestplateStats(chest);
    LeggingsStats ls = GetLeggingsStats(legs);

    ArmorStats stats;
    stats.name = cs.name;
    stats.description = cs.description;
    stats.defense = hs.defense + cs.defense + ls.defense;
    stats.maxHpBonus = hs.maxHpBonus + cs.maxHpBonus + ls.maxHpBonus;
    stats.speedModifier = hs.speedModifier * cs.speedModifier * ls.speedModifier;
    stats.staminaRegenModifier = hs.staminaRegenModifier * cs.staminaRegenModifier * ls.staminaRegenModifier;
    stats.willBonus = hs.willBonus + cs.willBonus + ls.willBonus;

    stats.primaryColor = cs.primaryColor;
    stats.trimColor = cs.trimColor;
    stats.helmColor = hs.helmColor;
    stats.visorColor = hs.visorColor;
    return stats;
}

// Backward-compatible GetArmorStats
inline ArmorStats GetArmorStats(ArmorType type) {
    switch (type) {
        case ArmorType::COMMONERS_SCRAP:
            return GetCompositeArmorVisuals(HelmetType::COMMONERS_HOOD, ChestplateType::COMMONERS_TUNIC, LeggingsType::COMMONERS_BREECHES);
        case ArmorType::CRUSADER_PLATE:
            return GetCompositeArmorVisuals(HelmetType::CRUSADER_HELM, ChestplateType::CRUSADER_CUIRASS, LeggingsType::CRUSADER_GREAVES);
        case ArmorType::VANGUARD_MAIL:
            return GetCompositeArmorVisuals(HelmetType::VANGUARD_GREATHELM, ChestplateType::VANGUARD_CHEST, LeggingsType::VANGUARD_GREAVES);
        case ArmorType::SHADOW_LEATHER:
            return GetCompositeArmorVisuals(HelmetType::SHADOW_COWL, ChestplateType::SHADOW_GARB, LeggingsType::SHADOW_LEGGINGS);
        case ArmorType::NONE:
        default:
            return GetCompositeArmorVisuals(HelmetType::NONE, ChestplateType::NONE, LeggingsType::NONE);
    }
}
