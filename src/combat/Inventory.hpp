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
    void ClearArmors() { unlockedArmors.clear(); }
    const std::vector<ArmorType>& GetUnlockedArmors() const { return unlockedArmors; }
    ArmorType GetEquippedArmor() const { return equippedArmor; }
    void SetEquippedArmor(ArmorType type) { equippedArmor = type; }

    // Totem Relics
    bool HasTotem(TotemType type) const;
    void UnlockTotem(TotemType type);
    void ClearTotems() { unlockedTotems.clear(); }
    const std::vector<TotemType>& GetUnlockedTotems() const { return unlockedTotems; }
    TotemType GetEquippedTotem() const { return equippedTotem; }
    void SetEquippedTotem(TotemType type) { equippedTotem = type; }

    // Lore Fragments
    bool UnlockFragment(const std::string& id);
    void MarkFragmentRead(const std::string& id);
    const std::vector<LoreFragment>& GetFragments() const { return fragments; }
    const LoreFragment* GetFragment(const std::string& id) const;
    int GetDiscoveredFragmentCount() const;
    int GetTotalFragmentCount() const { return (int)fragments.size(); }
    bool HasUnreadFragments() const;

    // Quest / Key Items
    void AddQuestItem(const QuestItem& item);
    const std::vector<QuestItem>& GetQuestItems() const { return questItems; }

private:
    std::vector<WeaponType> unlockedWeapons;
    std::vector<ArmorType> unlockedArmors;
    ArmorType equippedArmor = ArmorType::NONE;

    std::vector<TotemType> unlockedTotems;
    TotemType equippedTotem = TotemType::NONE;

    std::vector<LoreFragment> fragments;
    std::vector<QuestItem> questItems;
};
