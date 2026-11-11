#include "SaveManager.hpp"
#include "SaveHelpers.hpp"
#include "GameConstants.hpp"
#include "../states/GameState.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdio>

int SaveManager::activeSlotIndex = 1;

std::string SaveManager::GetSlotFilePath(int slotIndex) {
    if (slotIndex < 1) slotIndex = 1;
    if (slotIndex > GameLimits::MAX_SAVE_SLOTS) slotIndex = GameLimits::MAX_SAVE_SLOTS;
    return "save_slot_" + std::to_string(slotIndex) + ".json";
}

bool SaveManager::HasSave(int slotIndex) {
    std::string path = GetSlotFilePath(slotIndex);
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("\"isValid\": true") != std::string::npos ||
            line.find("\"isValid\": 1") != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool SaveManager::LoadSummary(int slotIndex, SaveSlotSummary& outSummary) {
    std::string path = GetSlotFilePath(slotIndex);
    std::ifstream file(path);
    if (!file.is_open()) {
        outSummary.slotIndex = slotIndex;
        outSummary.isValid = false;
        return false;
    }

    outSummary.slotIndex = slotIndex;
    outSummary.isValid = false;
    outSummary.totalFragments = GameLimits::LORE_FRAGMENT_COUNT;
    outSummary.discoveredFragments = 0;

    std::string line;
    int fragCount = 0;

    while (std::getline(file, line)) {
        if (line.find("\"isValid\"") != std::string::npos) {
            outSummary.isValid = SaveHelpers::ExtractBoolValue(line);
        } else if (line.find("\"realmName\"") != std::string::npos) {
            outSummary.realmName = SaveHelpers::ExtractStringValue(line);
        } else if (line.find("\"playerHealth\"") != std::string::npos) {
            outSummary.playerHealth = SaveHelpers::ExtractFloatValue(line, 100.0f);
        } else if (line.find("\"playerMaxHealth\"") != std::string::npos) {
            outSummary.playerMaxHealth = SaveHelpers::ExtractFloatValue(line, 100.0f);
        } else if (line.find("\"shingles\"") != std::string::npos) {
            outSummary.shingles = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"oreScraps\"") != std::string::npos) {
            outSummary.oreScraps = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"essenceOfMist\"") != std::string::npos) {
            outSummary.essenceOfMist = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"shadowEssence\"") != std::string::npos) {
            outSummary.shadowEssence = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"galeArcherDefeated\"") != std::string::npos) {
            outSummary.galeArcherDefeated = SaveHelpers::ExtractBoolValue(line, false);
        } else if (line.find("\"elementalGolemDefeated\"") != std::string::npos) {
            outSummary.elementalGolemDefeated = SaveHelpers::ExtractBoolValue(line, false);
        } else if (line.find("\"arenaHighestWave\"") != std::string::npos) {
            outSummary.arenaHighestWave = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"potionCount\"") != std::string::npos) {
            outSummary.potionCount = SaveHelpers::ExtractIntValue(line, 1);
        } else if (line.find("\"equippedWeapon\"") != std::string::npos) {
            outSummary.equippedWeapon = SaveHelpers::ExtractIntValue(line, 1);
        } else if (line.find("\"equippedArmor\"") != std::string::npos) {
            outSummary.equippedArmor = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedHelmet\"") != std::string::npos) {
            outSummary.equippedHelmet = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedChestplate\"") != std::string::npos) {
            outSummary.equippedChestplate = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedLeggings\"") != std::string::npos) {
            outSummary.equippedLeggings = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedTotem\"") != std::string::npos) {
            outSummary.equippedTotem = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"playtimeSeconds\"") != std::string::npos) {
            outSummary.playtimeSeconds = (uint32_t)SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"dateString\"") != std::string::npos) {
            outSummary.dateString = SaveHelpers::ExtractStringValue(line);
        } else if (line.find("\"unlockedFragments\"") != std::string::npos) {
            size_t pos = 0;
            while ((pos = line.find("true", pos)) != std::string::npos) {
                fragCount++;
                pos += 4;
            }
        }
    }

    outSummary.discoveredFragments = fragCount;
    return outSummary.isValid;
}

bool SaveManager::LoadGame(int slotIndex, GameSaveData& outData) {
    std::string path = GetSlotFilePath(slotIndex);
    std::ifstream file(path);
    if (!file.is_open()) {
        outData.slotIndex = slotIndex;
        outData.isValid = false;
        return false;
    }

    outData.slotIndex = slotIndex;
    outData.isValid = false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("\"isValid\"") != std::string::npos) {
            outData.isValid = SaveHelpers::ExtractBoolValue(line);
        } else if (line.find("\"gameState\"") != std::string::npos) {
            outData.gameState = SaveHelpers::ExtractIntValue(line, 1);
        } else if (line.find("\"realmName\"") != std::string::npos) {
            outData.realmName = SaveHelpers::ExtractStringValue(line);
        } else if (line.find("\"playerX\"") != std::string::npos) {
            outData.playerX = SaveHelpers::ExtractFloatValue(line, 800.0f);
        } else if (line.find("\"playerY\"") != std::string::npos) {
            outData.playerY = SaveHelpers::ExtractFloatValue(line, 800.0f);
        } else if (line.find("\"playerHealth\"") != std::string::npos) {
            outData.playerHealth = SaveHelpers::ExtractFloatValue(line, 100.0f);
        } else if (line.find("\"playerMaxHealth\"") != std::string::npos) {
            outData.playerMaxHealth = SaveHelpers::ExtractFloatValue(line, 100.0f);
        } else if (line.find("\"playerComposure\"") != std::string::npos) {
            outData.playerComposure = SaveHelpers::ExtractFloatValue(line, 50.0f);
        } else if (line.find("\"potionCount\"") != std::string::npos) {
            outData.potionCount = SaveHelpers::ExtractIntValue(line, 1);
        } else if (line.find("\"shingles\"") != std::string::npos) {
            outData.shingles = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"oreScraps\"") != std::string::npos) {
            outData.oreScraps = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"frostShards\"") != std::string::npos) {
            outData.frostShards = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"essenceOfMist\"") != std::string::npos) {
            outData.essenceOfMist = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"shadowEssence\"") != std::string::npos) {
            outData.shadowEssence = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stoneskinTonics\"") != std::string::npos) {
            outData.stoneskinTonics = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"glacialDraughts\"") != std::string::npos) {
            outData.glacialDraughts = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"berserkerDraughts\"") != std::string::npos) {
            outData.berserkerDraughts = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashShingles\"") != std::string::npos) {
            outData.stashShingles = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashOreScraps\"") != std::string::npos) {
            outData.stashOreScraps = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashFrostShards\"") != std::string::npos) {
            outData.stashFrostShards = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashEssenceOfMist\"") != std::string::npos) {
            outData.stashEssenceOfMist = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashShadowEssence\"") != std::string::npos) {
            outData.stashShadowEssence = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashPotions\"") != std::string::npos) {
            outData.stashPotions = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashStoneskinTonics\"") != std::string::npos) {
            outData.stashStoneskinTonics = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashGlacialDraughts\"") != std::string::npos) {
            outData.stashGlacialDraughts = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"stashBerserkerDraughts\"") != std::string::npos) {
            outData.stashBerserkerDraughts = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"galeArcherDefeated\"") != std::string::npos) {
            outData.galeArcherDefeated = SaveHelpers::ExtractBoolValue(line, false);
        } else if (line.find("\"elementalGolemDefeated\"") != std::string::npos) {
            outData.elementalGolemDefeated = SaveHelpers::ExtractBoolValue(line, false);
        } else if (line.find("\"arenaHighestWave\"") != std::string::npos) {
            outData.arenaHighestWave = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedWeapon\"") != std::string::npos) {
            outData.equippedWeapon = SaveHelpers::ExtractIntValue(line, 1);
        } else if (line.find("\"activeWeaponSlot\"") != std::string::npos) {
            outData.activeWeaponSlot = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"weaponHotbar\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.weaponHotbar, [](const std::string& token) {
                return SaveHelpers::SafeParseInt(token, 0);
            });
        } else if (line.find("\"equippedArmor\"") != std::string::npos) {
            outData.equippedArmor = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedHelmet\"") != std::string::npos) {
            outData.equippedHelmet = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedChestplate\"") != std::string::npos) {
            outData.equippedChestplate = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedLeggings\"") != std::string::npos) {
            outData.equippedLeggings = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"equippedTotem\"") != std::string::npos) {
            outData.equippedTotem = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"bossZilDefeated\"") != std::string::npos) {
            outData.bossZilDefeated = SaveHelpers::ExtractBoolValue(line, false);
        } else if (line.find("\"knightLenonSparCompleted\"") != std::string::npos) {
            outData.knightLenonSparCompleted = SaveHelpers::ExtractBoolValue(line, false);
        } else if (line.find("\"zyphdenQuestPhase\"") != std::string::npos) {
            outData.zyphdenQuestPhase = SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"playtimeSeconds\"") != std::string::npos) {
            outData.playtimeSeconds = (uint32_t)SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"timestamp\"") != std::string::npos) {
            outData.timestamp = (uint64_t)SaveHelpers::ExtractIntValue(line, 0);
        } else if (line.find("\"dateString\"") != std::string::npos) {
            outData.dateString = SaveHelpers::ExtractStringValue(line);
        } else if (line.find("\"weaponUpgrades\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.weaponUpgrades, [](const std::string& token) {
                return SaveHelpers::SafeParseInt(token, 0);
            });
        } else if (line.find("\"unlockedWeapons\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.unlockedWeapons, [](const std::string& token) {
                return SaveHelpers::SafeParseBool(token, false);
            });
        } else if (line.find("\"unlockedHelmets\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.unlockedHelmets, [](const std::string& token) {
                return SaveHelpers::SafeParseBool(token, false);
            });
        } else if (line.find("\"unlockedChestplates\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.unlockedChestplates, [](const std::string& token) {
                return SaveHelpers::SafeParseBool(token, false);
            });
        } else if (line.find("\"unlockedLeggings\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.unlockedLeggings, [](const std::string& token) {
                return SaveHelpers::SafeParseBool(token, false);
            });
        } else if (line.find("\"unlockedTotems\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.unlockedTotems, [](const std::string& token) {
                return SaveHelpers::SafeParseBool(token, false);
            });
        } else if (line.find("\"unlockedCampfires\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.unlockedCampfires, [](const std::string& token) {
                return SaveHelpers::SafeParseBool(token, false);
            });
        } else if (line.find("\"bestiaryKills\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.bestiaryKills, [](const std::string& token) {
                return SaveHelpers::SafeParseInt(token, 0);
            });
        } else if (line.find("\"unlockedFragments\"") != std::string::npos) {
            SaveHelpers::ParseJsonArray(line, outData.unlockedFragments, [](const std::string& token) {
                return SaveHelpers::SafeParseBool(token, false);
            });
        }
    }

    return outData.isValid;
}

bool SaveManager::SaveGame(int slotIndex, const GameSaveData& data) {
    std::string path = GetSlotFilePath(slotIndex);
    std::ofstream file(path);
    if (!file.is_open()) return false;

    // Generate date string if empty
    std::string dateStr = data.dateString;
    if (dateStr.empty()) {
        std::time_t now = std::time(nullptr);
        char buf[64];
        if (std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", std::localtime(&now))) {
            dateStr = buf;
        } else {
            dateStr = "Unknown Date";
        }
    }

    file << "{\n";
    file << "  \"slotIndex\": " << slotIndex << ",\n";
    file << "  \"isValid\": true,\n";
    file << "  \"gameState\": " << data.gameState << ",\n";
    file << "  \"realmName\": \"" << data.realmName << "\",\n";
    file << "  \"playerX\": " << data.playerX << ",\n";
    file << "  \"playerY\": " << data.playerY << ",\n";
    file << "  \"playerHealth\": " << data.playerHealth << ",\n";
    file << "  \"playerMaxHealth\": " << data.playerMaxHealth << ",\n";
    file << "  \"playerComposure\": " << data.playerComposure << ",\n";
    file << "  \"potionCount\": " << data.potionCount << ",\n";
    file << "  \"shingles\": " << data.shingles << ",\n";
    file << "  \"oreScraps\": " << data.oreScraps << ",\n";
    file << "  \"frostShards\": " << data.frostShards << ",\n";
    file << "  \"essenceOfMist\": " << data.essenceOfMist << ",\n";
    file << "  \"shadowEssence\": " << data.shadowEssence << ",\n";
    file << "  \"stoneskinTonics\": " << data.stoneskinTonics << ",\n";
    file << "  \"glacialDraughts\": " << data.glacialDraughts << ",\n";
    file << "  \"berserkerDraughts\": " << data.berserkerDraughts << ",\n";
    file << "  \"stashShingles\": " << data.stashShingles << ",\n";
    file << "  \"stashOreScraps\": " << data.stashOreScraps << ",\n";
    file << "  \"stashFrostShards\": " << data.stashFrostShards << ",\n";
    file << "  \"stashEssenceOfMist\": " << data.stashEssenceOfMist << ",\n";
    file << "  \"stashShadowEssence\": " << data.stashShadowEssence << ",\n";
    file << "  \"stashPotions\": " << data.stashPotions << ",\n";
    file << "  \"stashStoneskinTonics\": " << data.stashStoneskinTonics << ",\n";
    file << "  \"stashGlacialDraughts\": " << data.stashGlacialDraughts << ",\n";
    file << "  \"stashBerserkerDraughts\": " << data.stashBerserkerDraughts << ",\n";
    file << "  \"galeArcherDefeated\": " << (data.galeArcherDefeated ? "true" : "false") << ",\n";
    file << "  \"elementalGolemDefeated\": " << (data.elementalGolemDefeated ? "true" : "false") << ",\n";
    file << "  \"arenaHighestWave\": " << data.arenaHighestWave << ",\n";
    file << "  \"equippedWeapon\": " << data.equippedWeapon << ",\n";
    file << "  \"activeWeaponSlot\": " << data.activeWeaponSlot << ",\n";
    file << "  \"weaponHotbar\": [" << data.weaponHotbar[0] << ", " << data.weaponHotbar[1] << ", " << data.weaponHotbar[2] << "],\n";
    file << "  \"equippedArmor\": " << data.equippedArmor << ",\n";
    file << "  \"equippedHelmet\": " << data.equippedHelmet << ",\n";
    file << "  \"equippedChestplate\": " << data.equippedChestplate << ",\n";
    file << "  \"equippedLeggings\": " << data.equippedLeggings << ",\n";
    file << "  \"equippedTotem\": " << data.equippedTotem << ",\n";

    // Weapon upgrades
    file << "  \"weaponUpgrades\": [";
    for (int i = 0; i < GameLimits::WEAPON_COUNT; ++i) {
        file << data.weaponUpgrades[i] << (i < GameLimits::WEAPON_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    // Unlocked weapons
    file << "  \"unlockedWeapons\": [";
    for (int i = 0; i < GameLimits::WEAPON_COUNT; ++i) {
        file << (data.unlockedWeapons[i] ? "true" : "false") << (i < GameLimits::WEAPON_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    // Unlocked modular helmets
    file << "  \"unlockedHelmets\": [";
    for (int i = 0; i < GameLimits::HELMET_COUNT; ++i) {
        file << (data.unlockedHelmets[i] ? "true" : "false") << (i < GameLimits::HELMET_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    // Unlocked modular chestplates
    file << "  \"unlockedChestplates\": [";
    for (int i = 0; i < GameLimits::CHESTPLATE_COUNT; ++i) {
        file << (data.unlockedChestplates[i] ? "true" : "false") << (i < GameLimits::CHESTPLATE_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    // Unlocked modular leggings
    file << "  \"unlockedLeggings\": [";
    for (int i = 0; i < GameLimits::LEGGINGS_COUNT; ++i) {
        file << (data.unlockedLeggings[i] ? "true" : "false") << (i < GameLimits::LEGGINGS_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    // Unlocked totems
    file << "  \"unlockedTotems\": [";
    for (int i = 0; i < GameLimits::TOTEM_COUNT; ++i) {
        file << (data.unlockedTotems[i] ? "true" : "false") << (i < GameLimits::TOTEM_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    // Unlocked campfires
    file << "  \"unlockedCampfires\": [";
    for (int i = 0; i < GameLimits::CAMPFIRE_COUNT; ++i) {
        file << (data.unlockedCampfires[i] ? "true" : "false") << (i < GameLimits::CAMPFIRE_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    // Bestiary kills
    file << "  \"bestiaryKills\": [";
    for (int i = 0; i < GameLimits::BESTIARY_COUNT; ++i) {
        file << data.bestiaryKills[i] << (i < GameLimits::BESTIARY_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    // Unlocked fragments
    file << "  \"unlockedFragments\": [";
    for (int i = 0; i < GameLimits::LORE_FRAGMENT_COUNT; ++i) {
        file << (data.unlockedFragments[i] ? "true" : "false") << (i < GameLimits::LORE_FRAGMENT_COUNT - 1 ? ", " : "");
    }
    file << "],\n";

    file << "  \"bossZilDefeated\": " << (data.bossZilDefeated ? "true" : "false") << ",\n";
    file << "  \"knightLenonSparCompleted\": " << (data.knightLenonSparCompleted ? "true" : "false") << ",\n";
    file << "  \"zyphdenQuestPhase\": " << data.zyphdenQuestPhase << ",\n";
    file << "  \"arenaHighestWave\": " << data.arenaHighestWave << ",\n";
    file << "  \"playtimeSeconds\": " << data.playtimeSeconds << ",\n";
    file << "  \"timestamp\": " << (data.timestamp > 0 ? data.timestamp : (uint64_t)std::time(nullptr)) << ",\n";
    file << "  \"dateString\": \"" << dateStr << "\"\n";
    file << "}\n";

    file.close();
    return true;
}

bool SaveManager::DeleteSave(int slotIndex) {
    std::string path = GetSlotFilePath(slotIndex);
    return (std::remove(path.c_str()) == 0);
}

void SaveManager::SetActiveSlot(int slotIndex) {
    if (slotIndex >= 1 && slotIndex <= GameLimits::MAX_SAVE_SLOTS) {
        activeSlotIndex = slotIndex;
    }
}

int SaveManager::GetActiveSlot() {
    return activeSlotIndex;
}

std::string SaveManager::GetRealmNameFromState(int state) {
    switch ((GameState)state) {
        case GameState::TITLE_SCREEN: return "Title Screen";
        case GameState::PROLOGUE_CUTSCENE: return "The Awakening";
        case GameState::RUINED_CASTLE: return "The Ruined Castle";
        case GameState::CROSSROADS: return "The Crossroads of the Damned";
        case GameState::ZYPHDEN_TRANSITION: return "Zyphden Threshold";
        case GameState::ZYPHDEN_REALM: return "Fortified Realm of Zyphden";
        case GameState::QUARRY_TRANSITION: return "Basalt Passageway";
        case GameState::QUARRY_DEPTHS: return "Eastern Quarry Depths";
        case GameState::RESTLESS_MIST_FOREST: return "Restless Mist Forest";
        case GameState::LOBBY: return "The Armory Sanctuary";
        case GameState::ARENA: return "The Royal Combat Arena";
        case GameState::BORDERLANDS_GATE: return "Borderlands Gateway";
        default: return "Land of Tydeshire";
    }
}
