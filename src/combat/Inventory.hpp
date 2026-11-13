#pragma once
#include "raylib.h"
#include "CombatTypes.hpp"
#include "Armor.hpp"
#include "Totem.hpp"
#include "../core/GameConstants.hpp"
#include <string>
#include <vector>

struct LoreFragment {
    std::string id;
    std::string title;
    std::string realm;
    std::string preview;
    std::string fullText;
    std::string clue;
    bool discovered = false;
    bool isRead = false;
};

struct QuestItem {
    std::string id;
    std::string name;
    std::string description;
    Color rarityColor;
};

struct EquipmentPreset {
    WeaponType weapon = WeaponType::SWORD;
    HelmetType helmet = HelmetType::NONE;
    ChestplateType chestplate = ChestplateType::NONE;
    LeggingsType leggings = LeggingsType::NONE;
    TotemType totem = TotemType::NONE;
    bool isConfigured = false;
};

struct PlayerStash {
    int shingles = 0;
    int oreScraps = 0;
    int frostShards = 0;
    int essenceOfMist = 0;
    int shadowEssence = 0;
    int healthPotions = 0;
    int stoneskinTonics = 0;
    int glacialDraughts = 0;
    int berserkerDraughts = 0;
};

class Inventory {
public:
    Inventory();

    void InitDefault();

    // Weapon Arsenal
    bool HasWeapon(WeaponType type) const;
    void UnlockWeapon(WeaponType type);
    void ClearWeapons() {
        unlockedWeapons.clear();
        weaponHotbar[0] = WeaponType::NONE;
        weaponHotbar[1] = WeaponType::NONE;
        weaponHotbar[2] = WeaponType::NONE;
        activeWeaponSlot = 0;
    }
    const std::vector<WeaponType>& GetUnlockedWeapons() const { return unlockedWeapons; }

    // 3-Slot Weapon Hotbar ([1], [2], [3])
    WeaponType GetHotbarWeapon(int slotIdx) const;
    void SetHotbarWeapon(int slotIdx, WeaponType type);
    int GetActiveWeaponSlot() const { return activeWeaponSlot; }
    void SetActiveWeaponSlot(int slotIdx);
    int GetWeaponHotbarSlot(WeaponType type) const;

    // Armor Wardrobe (Modular Pieces)
    bool HasArmor(ArmorType type) const;
    void UnlockArmor(ArmorType type);
    void ClearArmors() {
        unlockedArmors.clear();
        unlockedArmors.push_back(ArmorType::NONE);
        equippedArmor = ArmorType::NONE;
        unlockedHelmets.clear();
        unlockedHelmets.push_back(HelmetType::NONE);
        equippedHelmet = HelmetType::NONE;
        unlockedChestplates.clear();
        unlockedChestplates.push_back(ChestplateType::NONE);
        equippedChestplate = ChestplateType::NONE;
        unlockedLeggings.clear();
        unlockedLeggings.push_back(LeggingsType::NONE);
        equippedLeggings = LeggingsType::NONE;
    }
    const std::vector<ArmorType>& GetUnlockedArmors() const { return unlockedArmors; }
    ArmorType GetEquippedArmor() const { return equippedArmor; }
    void SetEquippedArmor(ArmorType type);

    // Modular Piece Queries & Setters
    bool HasHelmet(HelmetType type) const;
    void UnlockHelmet(HelmetType type);
    const std::vector<HelmetType>& GetUnlockedHelmets() const { return unlockedHelmets; }
    HelmetType GetEquippedHelmet() const { return equippedHelmet; }
    void SetEquippedHelmet(HelmetType type) { equippedHelmet = type; }

    bool HasChestplate(ChestplateType type) const;
    void UnlockChestplate(ChestplateType type);
    const std::vector<ChestplateType>& GetUnlockedChestplates() const { return unlockedChestplates; }
    ChestplateType GetEquippedChestplate() const { return equippedChestplate; }
    void SetEquippedChestplate(ChestplateType type) { equippedChestplate = type; }

    bool HasLeggings(LeggingsType type) const;
    void UnlockLeggings(LeggingsType type);
    const std::vector<LeggingsType>& GetUnlockedLeggings() const { return unlockedLeggings; }
    LeggingsType GetEquippedLeggings() const { return equippedLeggings; }
    void SetEquippedLeggings(LeggingsType type) { equippedLeggings = type; }

    // Totem Relics
    bool HasTotem(TotemType type) const;
    void UnlockTotem(TotemType type);
    void ClearTotems() {
        unlockedTotems.clear();
        unlockedTotems.push_back(TotemType::NONE);
        equippedTotem = TotemType::NONE;
    }
    const std::vector<TotemType>& GetUnlockedTotems() const { return unlockedTotems; }
    TotemType GetEquippedTotem() const { return equippedTotem; }
    void SetEquippedTotem(TotemType type) { equippedTotem = type; }

    // Lore Fragments
    bool UnlockFragment(const std::string& id);
    bool HasFragment(const std::string& id) const;
    void MarkFragmentRead(const std::string& id);
    const std::vector<LoreFragment>& GetFragments() const { return fragments; }
    const LoreFragment* GetFragment(const std::string& id) const;
    int GetDiscoveredFragmentCount() const;
    int GetTotalFragmentCount() const { return (int)fragments.size(); }
    bool HasUnreadFragments() const;

    // Quest / Key Items
    void AddQuestItem(const QuestItem& item);
    bool HasQuestItem(const std::string& id) const;
    const std::vector<QuestItem>& GetQuestItems() const { return questItems; }

    // Consumables: Health Potions
    int GetHealthPotionCount() const { return healthPotions; }
    void SetHealthPotionCount(int count) { healthPotions = count >= 0 ? count : 0; }
    void AddHealthPotions(int count) { healthPotions += count; }
    bool ConsumeHealthPotion() {
        if (healthPotions > 0) {
            healthPotions--;
            return true;
        }
        return false;
    }

    // Currency: Shingles
    int GetShingles() const { return shingles; }
    void SetShingles(int count) { shingles = count >= 0 ? count : 0; }
    void AddShingles(int count) { shingles += count; }
    bool SpendShingles(int count) {
        if (shingles >= count) {
            shingles -= count;
            return true;
        }
        return false;
    }

    // Material: Ore Scraps
    int GetOreScraps() const { return oreScraps; }
    void SetOreScraps(int count) { oreScraps = count >= 0 ? count : 0; }
    void AddOreScraps(int count) { oreScraps += count; }

    // Material: Frost Shards
    int GetFrostShards() const { return frostShards; }
    void SetFrostShards(int count) { frostShards = count >= 0 ? count : 0; }
    void AddFrostShards(int count) { frostShards += count; }
    bool SpendFrostShards(int count) {
        if (frostShards >= count) {
            frostShards -= count;
            return true;
        }
        return false;
    }

    // Elixirs
    int GetStoneskinTonicCount() const { return stoneskinTonics; }
    void SetStoneskinTonicCount(int c) { stoneskinTonics = c >= 0 ? c : 0; }
    void AddStoneskinTonics(int c) { stoneskinTonics += c; }
    bool ConsumeStoneskinTonic() {
        if (stoneskinTonics > 0) {
            stoneskinTonics--;
            return true;
        }
        return false;
    }

    int GetGlacialDraughtCount() const { return glacialDraughts; }
    void SetGlacialDraughtCount(int c) { glacialDraughts = c >= 0 ? c : 0; }
    void AddGlacialDraughts(int c) { glacialDraughts += c; }
    bool ConsumeGlacialDraught() {
        if (glacialDraughts > 0) {
            glacialDraughts--;
            return true;
        }
        return false;
    }

    int GetBerserkerDraughtCount() const { return berserkerDraughts; }
    void SetBerserkerDraughtCount(int c) { berserkerDraughts = c >= 0 ? c : 0; }
    void AddBerserkerDraughts(int c) { berserkerDraughts += c; }
    bool ConsumeBerserkerDraught() {
        if (berserkerDraughts > 0) {
            berserkerDraughts--;
            return true;
        }
        return false;
    }

    // Storage Stash Vault
    PlayerStash& GetStash() { return stash; }
    const PlayerStash& GetStash() const { return stash; }
    void DepositItem(int itemId, int count = 1);
    void WithdrawItem(int itemId, int count = 1);

    int GetEssenceOfMist() const { return essenceOfMist; }
    void SetEssenceOfMist(int count) { essenceOfMist = count >= 0 ? count : 0; }
    void AddEssenceOfMist(int count) { essenceOfMist += count; }
    bool SpendEssenceOfMist(int count) {
        if (essenceOfMist >= count) {
            essenceOfMist -= count;
            return true;
        }
        return false;
    }

    int GetShadowEssence() const { return shadowEssence; }
    void SetShadowEssence(int count) { shadowEssence = count >= 0 ? count : 0; }
    void AddShadowEssence(int count) { shadowEssence += count; }
    bool SpendShadowEssence(int count) {
        if (shadowEssence >= count) {
            shadowEssence -= count;
            return true;
        }
        return false;
    }
    bool SpendOreScraps(int count) {
        if (oreScraps >= count) {
            oreScraps -= count;
            return true;
        }
        return false;
    }

    // Weapon Upgrades (+0, +1, +2)
    int GetWeaponUpgrade(WeaponType type) const {
        int idx = (int)type;
        if (idx >= 0 && idx < GameLimits::WEAPON_COUNT) return weaponUpgradeLevel[idx];
        return 0;
    }
    void SetWeaponUpgrade(WeaponType type, int level) {
        int idx = (int)type;
        if (idx >= 0 && idx < GameLimits::WEAPON_COUNT) weaponUpgradeLevel[idx] = level;
    }
    void UpgradeWeapon(WeaponType type) {
        int idx = (int)type;
        if (idx >= 0 && idx < GameLimits::WEAPON_COUNT) weaponUpgradeLevel[idx]++;
    }

    // Campfire Waypoints (1: Castle, 2: Crossroads, 3: Zyphden, 4: Quarry, 5: Forest, 6: North Zyphden, 7: Permafrost Crypts)
    bool unlockedCampfires[GameLimits::CAMPFIRE_COUNT] = { true, false, false, false, false, false, false };
    bool IsCampfireUnlocked(int id) const {
        if (id >= 1 && id <= GameLimits::CAMPFIRE_COUNT) return unlockedCampfires[id - 1];
        return false;
    }
    void UnlockCampfire(int id) {
        if (id >= 1 && id <= GameLimits::CAMPFIRE_COUNT) unlockedCampfires[id - 1] = true;
    }
    const bool* GetUnlockedCampfires() const { return unlockedCampfires; }
    void SetCampfireUnlocked(int id, bool val) {
        if (id >= 1 && id <= GameLimits::CAMPFIRE_COUNT) unlockedCampfires[id - 1] = val;
    }

    // Bestiary Kill Tracking
    // 0: Slasher, 1: Archer, 2: Pyromancer, 3: Brute, 4: Inquisitor Boss,
    // 5: Ore/Mini Golem, 6: Elemental Golem Boss, 7: Gale Bandit, 8: Gale Archer Boss
    int enemyKillCounts[GameLimits::BESTIARY_COUNT] = { 0 };
    void RecordEnemyKill(int typeIdx) {
        if (typeIdx >= 0 && typeIdx < GameLimits::BESTIARY_COUNT) enemyKillCounts[typeIdx]++;
    }
    int GetEnemyKills(int idx) const {
        if (idx >= 0 && idx < GameLimits::BESTIARY_COUNT) return enemyKillCounts[idx];
        return 0;
    }
    void SetEnemyKills(int idx, int val) {
        if (idx >= 0 && idx < GameLimits::BESTIARY_COUNT) enemyKillCounts[idx] = val;
    }

    // Equipment Loadout Presets [ I ] [ II ] [ III ]
    int GetActiveLoadoutIndex() const { return activeLoadoutIdx; }
    void SetActiveLoadoutIndex(int idx) { if (idx >= 0 && idx < 3) activeLoadoutIdx = idx; }
    const EquipmentPreset& GetLoadout(int idx) const { return loadouts[(idx >= 0 && idx < 3) ? idx : 0]; }
    EquipmentPreset& GetLoadout(int idx) { return loadouts[(idx >= 0 && idx < 3) ? idx : 0]; }
    void SetLoadout(int idx, const EquipmentPreset& preset) {
        if (idx >= 0 && idx < 3) loadouts[idx] = preset;
    }
    void SaveCurrentToLoadout(int idx, WeaponType wep, HelmetType helm, ChestplateType chest, LeggingsType leg, TotemType tot) {
        if (idx < 0 || idx >= 3) return;
        loadouts[idx].weapon = wep;
        loadouts[idx].helmet = helm;
        loadouts[idx].chestplate = chest;
        loadouts[idx].leggings = leg;
        loadouts[idx].totem = tot;
        loadouts[idx].isConfigured = true;
    }

private:
    EquipmentPreset loadouts[3];
    int activeLoadoutIdx = 0;
    WeaponType weaponHotbar[3] = { WeaponType::NONE, WeaponType::NONE, WeaponType::NONE };
    int activeWeaponSlot = 0;
    std::vector<WeaponType> unlockedWeapons;
    std::vector<ArmorType> unlockedArmors;
    ArmorType equippedArmor = ArmorType::NONE;

    std::vector<HelmetType> unlockedHelmets;
    HelmetType equippedHelmet = HelmetType::NONE;

    std::vector<ChestplateType> unlockedChestplates;
    ChestplateType equippedChestplate = ChestplateType::NONE;

    std::vector<LeggingsType> unlockedLeggings;
    LeggingsType equippedLeggings = LeggingsType::NONE;

    std::vector<TotemType> unlockedTotems;
    TotemType equippedTotem = TotemType::NONE;

    std::vector<LoreFragment> fragments;
    std::vector<QuestItem> questItems;

    int healthPotions = 1; // Default starting health potions
    int shingles = 0;      // Currency: Shingles
    int oreScraps = 0;     // Material: Ore Scraps (from Quarry Ore Golems)
    int frostShards = 0;   // Material: Frost Shards (from Highland Frost Crystals)
    int essenceOfMist = 0; // Material: Essence of Mist (from Gale Bandits)
    int shadowEssence = 0; // Material: Shadow Essence (from Gale Archer Boss)
    int stoneskinTonics = 0;
    int glacialDraughts = 0;
    int berserkerDraughts = 0;
    PlayerStash stash;
    int weaponUpgradeLevel[GameLimits::WEAPON_COUNT] = { 0 };
};
