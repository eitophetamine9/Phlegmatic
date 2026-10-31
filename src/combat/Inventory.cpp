#include "Inventory.hpp"
#include <algorithm>

Inventory::Inventory() {
    InitDefault();
}

void Inventory::InitDefault() {
    unlockedWeapons.clear();
    unlockedArmors.clear();
    unlockedTotems.clear();
    fragments.clear();
    questItems.clear();

    // Default starting weapons
    unlockedWeapons.push_back(WeaponType::NONE);

    // Default armors & pieces: ONLY NONE!
    equippedArmor = ArmorType::NONE;
    unlockedArmors.push_back(ArmorType::NONE);
    equippedHelmet = HelmetType::NONE;
    unlockedHelmets.push_back(HelmetType::NONE);
    equippedChestplate = ChestplateType::NONE;
    unlockedChestplates.push_back(ChestplateType::NONE);
    equippedLeggings = LeggingsType::NONE;
    unlockedLeggings.push_back(LeggingsType::NONE);

    // Default totems: ONLY NONE!
    equippedTotem = TotemType::NONE;
    unlockedTotems.push_back(TotemType::NONE);

    // Default health potions & currency
    healthPotions = 1;
    shingles = 0;
    oreScraps = 0;
    for (int i = 0; i < 6; ++i) {
        weaponUpgradeLevel[i] = 0;
    }

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
        "In the 500th year of unending strife, King Charlz crushed the warring factions beneath his warhammer. By solemn decree at the High Summit, five sovereign kings knelt before the throne of Charlz the Lord Liege.\n\nThe five realms of Azshden, Fleurshden, Zyphden, Noirden, and Solsden were chartered in blood and silver. For three golden decades, heralds sang of an eternal peace, and borders stood without blood.",
        "Ruined Castle — Inscribed upon the weathered Chronicle Stele west of the awakening flagstones.",
        false, // Discovered in tutorial by inspecting the Chronicle Stele
        false
    });

    fragments.push_back({
        "frag_02",
        "Fragment II: The Shadow over Zyphden",
        "Kingdom of Zyphden",
        "Whispers spread from the northern watchtowers. The Lord Liege no longer grants audience...",
        "Whispers spread like wildfire from the northern watchtowers of Zyphden. The Lord Liege no longer grants audience to any mortal counsel.\n\nRoyal envoys dispatched to the Crown Domain returned in chains or not at all. The holy paladins who once guarded the realm have turned their blades inward, murmuring incantations in forgotten tongues. The Land of Peace is no more.",
        "Ruined Castle — Sealed within an ancient iron relic chest across the eastern stone stream bridge.",
        false,
        false
    });

    fragments.push_back({
        "frag_03",
        "Fragment III: Zil's Holy Oath",
        "Ruined Castle of the Vanguard",
        "Zil, once first champion of the Sun Vanguard, swore to purge the corruption...",
        "Zil, once first champion of the Sun Vanguard, swore an unbroken oath to purge the dark madness that seized King Charlz.\n\nWithin these desecrated castle halls, a remnant fragment possessing 1% of his true power guards the holy blade 'Truth Seeker (Partial)'. Believing he alone guards the realm from absolute ruin, he slays all who draw near.",
        "Ruined Castle — Bestowed atop the high altar dais upon overcoming or proving resolve to Zil's Remnant.",
        false,
        false
    });

    fragments.push_back({
        "frag_04",
        "Fragment IV: The Five Broken Seals",
        "Crown Domain of King Charlz",
        "Azshden, Fleurshden, Zyphden, Noirden, and Solsden each bore an ancient seal...",
        "Azshden of the Iron Cliffs, Fleurshden of the Sunlit Valleys, Zyphden of the Gale Winds, Noirden of the Obsidian Mists, and Solsden of the Radiant Altars.\n\nEach kingdom was entrusted with an ancient primordial seal bound directly to the Lord Liege's Crown. If the five seals are shattered by the Lord Liege's madness, the veil separating Tydeshire from the outer abyss will collapse forever.",
        "Ruined Castle — Carved into a battle-worn Sun-Crusader shield resting in the southeast overgrown ramparts.",
        false,
        false
    });

    fragments.push_back({
        "frag_05",
        "Fragment V: The Crossroads Watch",
        "Crossroads of Tydeshire",
        "The highway north was once flooded with silk merchants. Now only beasts remain...",
        "The highway north was once flooded with merchants bearing silk and spice from Fleurshden.\n\nNow, only prowling slashers and iron brutes wander the shattered cobblestone. I watched the royal vanguard ride south three moons ago. None returned. The King in the north has barricaded the gates, waiting for the sky to fall.",
        "Crossroads — Resting in a leather travel satchel upon a stone roadside plinth southeast of the main highway junction.",
        false,
        false
    });

    fragments.push_back({
        "frag_06",
        "Fragment VI: Rite of the Gale Winds",
        "Shrine of the Gale Winds",
        "Long before the Unification, the Gale Monks communed with the northern tempests...",
        "Long before the Unification, the Gale Monks of Zyphden communed with the howling northern tempests.\n\nThey foresaw that when the sovereign crown fell to corruption, the Totems of old would awaken. The Totem of Sanity was hidden beneath the roadside ruins to guide the one who would dare face the Lord Liege.",
        "The Realm of Zyphden — Inscribed upon an ancient celestial Wind Lectern in the northern town courtyard.",
        false,
        false
    });

    fragments.push_back({
        "frag_07",
        "Fragment VII: Lenon's Field Log",
        "Knight's Garrison of Zyphden",
        "My sworn brother Zil rode south to investigate the Ruined Castle...",
        "My sworn brother Zil rode south to investigate the Ruined Castle. He carried the shattered remnant of Truth Seeker, believing he could sever the madness at its root.\n\nI pray he still breathes. If a stranger ever enters Zyphden bearing his blade or token, I will know my brother's fate.",
        "The Realm of Zyphden — Discovered in a Vanguard Field Log resting outside the western garrison armory.",
        false,
        false
    });

    fragments.push_back({
        "frag_08",
        "Fragment VIII: The Sovereign's Burden",
        "Royal Keep of King Kreptu",
        "To all subjects of Zyphden: The High Summit is broken. We shall not bend our knee...",
        "To all subjects of Zyphden: The High Summit is broken. King Charlz answers no raven.\n\nWe have fortified the southern gates and summoned all remaining crusaders to the courtyard. We shall not bend our knee to madness. Zyphden shall stand as long as a single crusader draws breath.",
        "The Realm of Zyphden — Preserved within a gilded archive pedestal beside the courtyard sparring training grounds.",
        false,
        false
    });

    fragments.push_back({
        "frag_09",
        "Fragment IX: Inscription of the Ancient Pillars",
        "High Sanctuaries of Tydeshire",
        "The monolithic stone pillars bear ancient runic glyphs carved by the First Wardens...",
        "The monolithic stone pillars standing across the valleys of Tydeshire were not hewn for mere decoration.\n\nCarved by the First Wardens before the Unification Wars, each pillar channels primordial leylines that steady mortal willpower. When the mind falters against madness, gaze upon the runes to rekindle inner poise.",
        "Royal Keep — Carved into the monolithic runic pillar inside King Kreptu's Great Throne Hall.",
        false,
        false
    });

    fragments.push_back({
        "frag_10",
        "Fragment X: The Royal Armory Archives",
        "Keep Armory of Zyphden",
        "Records of royal weaponsmithing and the tempered armaments of the High Marshals...",
        "Official records recovered from the keep archives: The high marshals of Zyphden quenched their blades in lunar spring waters to counterbalance the dark abyssal energies creeping from the south.\n\nNo sword alone can vanquish the lord liege; one must hone posture, temper their will against devastating blows, and seize the window of an enemy's stagger.",
        "Royal Keep — Locked inside the gilded Royal Armory relic chest in the eastern vault of the Throne Hall.",
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

void Inventory::SetEquippedArmor(ArmorType type) {
    equippedArmor = type;
    switch (type) {
        case ArmorType::COMMONERS_SCRAP:
            equippedHelmet = HelmetType::COMMONERS_HOOD;
            equippedChestplate = ChestplateType::COMMONERS_TUNIC;
            equippedLeggings = LeggingsType::COMMONERS_BREECHES;
            break;
        case ArmorType::CRUSADER_PLATE:
            equippedHelmet = HelmetType::CRUSADER_HELM;
            equippedChestplate = ChestplateType::CRUSADER_CUIRASS;
            equippedLeggings = LeggingsType::CRUSADER_GREAVES;
            break;
        case ArmorType::VANGUARD_MAIL:
            equippedHelmet = HelmetType::VANGUARD_GREATHELM;
            equippedChestplate = ChestplateType::VANGUARD_CHEST;
            equippedLeggings = LeggingsType::VANGUARD_GREAVES;
            break;
        case ArmorType::SHADOW_LEATHER:
            equippedHelmet = HelmetType::SHADOW_COWL;
            equippedChestplate = ChestplateType::SHADOW_GARB;
            equippedLeggings = LeggingsType::SHADOW_LEGGINGS;
            break;
        case ArmorType::NONE:
        default:
            equippedHelmet = HelmetType::NONE;
            equippedChestplate = ChestplateType::NONE;
            equippedLeggings = LeggingsType::NONE;
            break;
    }
}

bool Inventory::HasArmor(ArmorType type) const {
    for (ArmorType a : unlockedArmors) {
        if (a == type) return true;
    }
    return false;
}

void Inventory::UnlockArmor(ArmorType type) {
    if (!HasArmor(type)) {
        unlockedArmors.push_back(type);
    }
    switch (type) {
        case ArmorType::COMMONERS_SCRAP:
            UnlockHelmet(HelmetType::COMMONERS_HOOD);
            UnlockChestplate(ChestplateType::COMMONERS_TUNIC);
            UnlockLeggings(LeggingsType::COMMONERS_BREECHES);
            break;
        case ArmorType::CRUSADER_PLATE:
            UnlockHelmet(HelmetType::CRUSADER_HELM);
            UnlockChestplate(ChestplateType::CRUSADER_CUIRASS);
            UnlockLeggings(LeggingsType::CRUSADER_GREAVES);
            break;
        case ArmorType::VANGUARD_MAIL:
            UnlockHelmet(HelmetType::VANGUARD_GREATHELM);
            UnlockChestplate(ChestplateType::VANGUARD_CHEST);
            UnlockLeggings(LeggingsType::VANGUARD_GREAVES);
            break;
        case ArmorType::SHADOW_LEATHER:
            UnlockHelmet(HelmetType::SHADOW_COWL);
            UnlockChestplate(ChestplateType::SHADOW_GARB);
            UnlockLeggings(LeggingsType::SHADOW_LEGGINGS);
            break;
        case ArmorType::NONE:
        default:
            break;
    }
}

bool Inventory::HasHelmet(HelmetType type) const {
    for (HelmetType h : unlockedHelmets) {
        if (h == type) return true;
    }
    return false;
}

void Inventory::UnlockHelmet(HelmetType type) {
    if (!HasHelmet(type)) {
        unlockedHelmets.push_back(type);
    }
}

bool Inventory::HasChestplate(ChestplateType type) const {
    for (ChestplateType c : unlockedChestplates) {
        if (c == type) return true;
    }
    return false;
}

void Inventory::UnlockChestplate(ChestplateType type) {
    if (!HasChestplate(type)) {
        unlockedChestplates.push_back(type);
    }
}

bool Inventory::HasLeggings(LeggingsType type) const {
    for (LeggingsType l : unlockedLeggings) {
        if (l == type) return true;
    }
    return false;
}

void Inventory::UnlockLeggings(LeggingsType type) {
    if (!HasLeggings(type)) {
        unlockedLeggings.push_back(type);
    }
}

bool Inventory::HasTotem(TotemType type) const {
    for (TotemType t : unlockedTotems) {
        if (t == type) return true;
    }
    return false;
}

void Inventory::UnlockTotem(TotemType type) {
    if (!HasTotem(type)) {
        unlockedTotems.push_back(type);
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

bool Inventory::HasFragment(const std::string& id) const {
    const LoreFragment* frag = GetFragment(id);
    return frag && frag->discovered;
}

void Inventory::AddQuestItem(const QuestItem& item) {
    questItems.push_back(item);
}

bool Inventory::HasQuestItem(const std::string& id) const {
    for (const auto& item : questItems) {
        if (item.id == id) return true;
    }
    return false;
}
