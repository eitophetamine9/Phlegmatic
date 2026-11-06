#pragma once
#include <cstddef>

namespace GameLimits {
    // Save Persistence
    inline constexpr int MAX_SAVE_SLOTS = 3;

    // Arsenal & Armaments
    inline constexpr int WEAPON_COUNT = 10;         // NONE, SWORD, DAGGERS, MACE, TRUTH_SEEKER, HALBERD, ZEPHYR_DAGGERS, WINDRUNNER_RECURVE, SUN_CLAYMORE, STORM_GLAIVE
    inline constexpr int WEAPON_DRAWER_COUNT = 9;   // Playable weapons listed in UI (excluding NONE)

    // Modular Armor Pieces
    inline constexpr int HELMET_COUNT = 7;          // NONE, COMMONERS_HOOD, CRUSADER_HELM, VANGUARD_GREATHELM, SHADOW_COWL, SCRAP_SALLET, BASALT_GREATHELM
    inline constexpr int CHESTPLATE_COUNT = 7;      // NONE, COMMONERS_TUNIC, CRUSADER_CUIRASS, VANGUARD_CHEST, SHADOW_GARB, SCRAP_CUIRASS, BASALT_CUIRASS
    inline constexpr int LEGGINGS_COUNT = 7;        // NONE, COMMONERS_BREECHES, CRUSADER_GREAVES, VANGUARD_GREAVES, SHADOW_LEGGINGS, SCRAP_GREAVES, BASALT_GREAVES
    inline constexpr int ARMOR_SET_COUNT = 7;       // NONE, COMMONERS_SCRAP, CRUSADER_PLATE, VANGUARD_BASTION, SHADOWSTRIDER, FORGED_SCRAP_IRON, BASALT_GOLEM

    // Sacred Totems
    inline constexpr int TOTEM_COUNT = 6;           // NONE, TOTEM_SANITY, TOTEM_INSANITY, TOTEM_MIST, CRUSADER_TOTEM, ELEMENTAL_CORE_TOTEM
    inline constexpr int ATTUNABLE_TOTEM_COUNT = 5; // Equippable relics listed in UI (excluding NONE)

    // World & Exploration Progression
    inline constexpr int CAMPFIRE_COUNT = 6;        // 1: Castle, 2: Crossroads, 3: Zyphden, 4: Quarry, 5: Forest, 6: North Zyphden
    inline constexpr int BESTIARY_COUNT = 9;        // 0: Slasher, 1: Archer, 2: Pyromancer, 3: Brute, 4: Inquisitor, 5: Ore Golem, 6: Elemental Golem, 7: Gale Bandit, 8: Gale Archer
    inline constexpr int LORE_FRAGMENT_COUNT = 14;  // frag_01 through frag_14
    inline constexpr int LOADOUT_COUNT = 3;         // Loadouts: I, II, III
    inline constexpr int MATERIAL_SLOT_COUNT = 5;   // Shingles, Ore Scraps, Mist Essence, Shadow Essence, Potions
}

