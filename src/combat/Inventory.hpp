#pragma once
#include "raylib.h"
#include "CombatTypes.hpp"
#include "Armor.hpp"
#include "Totem.hpp"
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

class Inventory {
public:
    Inventory();

    void InitDefault();

    // Weapon Arsenal
    bool HasWeapon(WeaponType type) const;
    void UnlockWeapon(WeaponType type);
    void ClearWeapons() { unlockedWeapons.clear(); }
    const std::vector<WeaponType>& GetUnlockedWeapons() const { return unlockedWeapons; }

    // Armor Wardrobe
    bool HasArmor(ArmorType type) const;
    void UnlockArmor(ArmorType type);
    void ClearArmors() {
        unlockedArmors.clear();
        unlockedArmors.push_back(ArmorType::NONE);
        equippedArmor = ArmorType::NONE;
    }
    const std::vector<ArmorType>& GetUnlockedArmors() const { return unlockedArmors; }
    ArmorType GetEquippedArmor() const { return equippedArmor; }
    void SetEquippedArmor(ArmorType type) { equippedArmor = type; }

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
        if (idx >= 0 && idx < 6) return weaponUpgradeLevel[idx];
        return 0;
    }
    void SetWeaponUpgrade(WeaponType type, int level) {
        int idx = (int)type;
        if (idx >= 0 && idx < 6) weaponUpgradeLevel[idx] = level;
    }
    void UpgradeWeapon(WeaponType type) {
        int idx = (int)type;
        if (idx >= 0 && idx < 6) weaponUpgradeLevel[idx]++;
    }

private:
    std::vector<WeaponType> unlockedWeapons;
    std::vector<ArmorType> unlockedArmors;
    ArmorType equippedArmor = ArmorType::NONE;

    std::vector<TotemType> unlockedTotems;
    TotemType equippedTotem = TotemType::NONE;

    std::vector<LoreFragment> fragments;
    std::vector<QuestItem> questItems;

    int healthPotions = 1; // Default starting health potions
    int shingles = 0;      // Currency: Shingles
    int oreScraps = 0;     // Material: Ore Scraps (from Quarry Ore Golems)
    int weaponUpgradeLevel[6] = { 0 };
};
