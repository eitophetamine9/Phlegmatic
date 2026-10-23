#pragma once
#include "raylib.h"
#include "CombatTypes.hpp"
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
    const std::vector<WeaponType>& GetUnlockedWeapons() const { return unlockedWeapons; }

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
    std::vector<LoreFragment> fragments;
    std::vector<QuestItem> questItems;
};
