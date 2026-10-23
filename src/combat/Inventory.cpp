#include "Inventory.hpp"
#include <algorithm>

Inventory::Inventory() {
    InitDefault();
}

void Inventory::InitDefault() {
    unlockedWeapons.clear();
    fragments.clear();
    questItems.clear();

    // Default starting weapons available in arsenal
    unlockedWeapons.push_back(WeaponType::SWORD);
    unlockedWeapons.push_back(WeaponType::DAGGERS);
    unlockedWeapons.push_back(WeaponType::MACE);

    // Initial quest item: Knight's Signet
    questItems.push_back({
        "item_signet",
        "Knight's Crest of Zyphden",
        "An engraved silver sigil bearing the twin gale-hawks of the Kingdom of Zyphden. Proof of noble lineage.",
        Color{ 130, 200, 255, 255 }
    });

    // Register Tydeshire Lore Fragments
    fragments.push_back({
        "frag_01",
        "Fragment I: The Great Unification",
        "The Land of Tydeshire",
        "In the 500th year of unending strife, King Charlz unified the realms...",
        "In the 500th year of unending strife, King Charlz crushed the warring factions beneath his warhammer. By solemn decree at the High Summit, four sovereign kings knelt before the throne of Charlz the Lord Liege.\n\nThe realms of Azshden, Fleurshden, Zyphden, and Noirden were chartered in blood and silver. For three golden decades, heralds sang of an eternal peace, and borders stood without blood.",
        false, // Discovered in tutorial by inspecting the Chronicle Stele
        false
    });

    fragments.push_back({
        "frag_02",
        "Fragment II: The Shadow over Zyphden",
        "Kingdom of Zyphden",
        "Whispers spread from the northern watchtowers. The Lord Liege no longer grants audience...",
        "Whispers spread like wildfire from the northern watchtowers of Zyphden. The Lord Liege no longer grants audience to any mortal counsel.\n\nRoyal envoys dispatched to the Crown Domain returned in chains or not at all. The holy paladins who once guarded the realm have turned their blades inward, murmuring incantations in forgotten tongues. The Land of Peace is no more.",
        false,
        false
    });

    fragments.push_back({
        "frag_03",
        "Fragment III: Zil's Holy Oath",
        "Ruined Castle of the Vanguard",
        "Zil, once first champion of the Sun Vanguard, swore to purge the corruption...",
        "Zil, once first champion of the Sun Vanguard, swore an unbroken oath to purge the dark madness that seized King Charlz.\n\nYet within these desecrated castle halls, the miasma took root in his righteous heart. Now clutching the sacred royal broadsword 'Truth Seeker', he slays all who draw near, believing he alone guards the realm from absolute ruin.",
        false,
        false
    });

    fragments.push_back({
        "frag_04",
        "Fragment IV: The Five Broken Seals",
        "Crown Domain of King Charlz",
        "Azshden, Fleurshden, Zyphden, and Noirden each bore an ancient seal...",
        "Azshden of the Iron Cliffs, Fleurshden of the Sunlit Valleys, Zyphden of the Gale Winds, and Noirden of the Obsidian Mists.\n\nEach kingdom was entrusted with an ancient primordial seal bound directly to the Lord Liege's Crown. If the five seals are shattered by the Lord Liege's madness, the veil separating Tydeshire from the outer abyss will collapse forever.",
        false,
        false
    });
}

bool Inventory::HasWeapon(WeaponType type) const {
    for (WeaponType w : unlockedWeapons) {
        if (w == type) return true;
    }
    return false;
}

void Inventory::UnlockWeapon(WeaponType type) {
    if (!HasWeapon(type)) {
        unlockedWeapons.push_back(type);
    }
}

bool Inventory::UnlockFragment(const std::string& id) {
    for (auto& frag : fragments) {
        if (frag.id == id) {
            if (!frag.discovered) {
                frag.discovered = true;
                frag.isRead = false;
                return true;
            }
            return false;
        }
    }
    return false;
}

void Inventory::MarkFragmentRead(const std::string& id) {
    for (auto& frag : fragments) {
        if (frag.id == id) {
            frag.isRead = true;
            break;
        }
    }
}

const LoreFragment* Inventory::GetFragment(const std::string& id) const {
    for (const auto& frag : fragments) {
        if (frag.id == id) return &frag;
    }
    return nullptr;
}

int Inventory::GetDiscoveredFragmentCount() const {
    int count = 0;
    for (const auto& frag : fragments) {
        if (frag.discovered) count++;
    }
    return count;
}

bool Inventory::HasUnreadFragments() const {
    for (const auto& frag : fragments) {
        if (frag.discovered && !frag.isRead) return true;
    }
    return false;
}

void Inventory::AddQuestItem(const QuestItem& item) {
    questItems.push_back(item);
}
