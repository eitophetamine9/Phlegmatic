#include "SaveManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdio>

int SaveManager::activeSlotIndex = 1;

std::string SaveManager::GetSlotFilePath(int slotIndex) {
    if (slotIndex < 1) slotIndex = 1;
    if (slotIndex > 3) slotIndex = 3;
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

static std::string ExtractStringValue(const std::string& line) {
    size_t colon = line.find(':');
    if (colon == std::string::npos) return "";
    size_t firstQuote = line.find('"', colon);
    if (firstQuote == std::string::npos) return "";
    size_t secondQuote = line.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos) return "";
    return line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

static int ExtractIntValue(const std::string& line) {
    size_t colon = line.find(':');
    if (colon == std::string::npos) return 0;
    std::string sub = line.substr(colon + 1);
    int val = 0;
    if (sscanf(sub.c_str(), "%d", &val) == 1) {
        return val;
    }
    return 0;
}

static float ExtractFloatValue(const std::string& line) {
    size_t colon = line.find(':');
    if (colon == std::string::npos) return 0.0f;
    std::string sub = line.substr(colon + 1);
    float val = 0.0f;
    if (sscanf(sub.c_str(), "%f", &val) == 1) {
        return val;
    }
    return 0.0f;
}

static bool ExtractBoolValue(const std::string& line) {
    size_t colon = line.find(':');
    if (colon == std::string::npos) return false;
    if (line.find("true", colon) != std::string::npos || line.find("1", colon) != std::string::npos) {
        return true;
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
    outSummary.totalFragments = 10;
    outSummary.discoveredFragments = 0;

    std::string line;
    bool inFragArray = false;
    int fragCount = 0;

    while (std::getline(file, line)) {
        if (line.find("\"isValid\"") != std::string::npos) {
            outSummary.isValid = ExtractBoolValue(line);
        } else if (line.find("\"realmName\"") != std::string::npos) {
            outSummary.realmName = ExtractStringValue(line);
        } else if (line.find("\"playerHealth\"") != std::string::npos) {
            outSummary.playerHealth = ExtractFloatValue(line);
        } else if (line.find("\"playerMaxHealth\"") != std::string::npos) {
            outSummary.playerMaxHealth = ExtractFloatValue(line);
        } else if (line.find("\"shingles\"") != std::string::npos) {
            outSummary.shingles = ExtractIntValue(line);
        } else if (line.find("\"oreScraps\"") != std::string::npos) {
            outSummary.oreScraps = ExtractIntValue(line);
        } else if (line.find("\"potionCount\"") != std::string::npos) {
            outSummary.potionCount = ExtractIntValue(line);
        } else if (line.find("\"equippedWeapon\"") != std::string::npos) {
            outSummary.equippedWeapon = ExtractIntValue(line);
        } else if (line.find("\"equippedArmor\"") != std::string::npos) {
            outSummary.equippedArmor = ExtractIntValue(line);
        } else if (line.find("\"equippedTotem\"") != std::string::npos) {
            outSummary.equippedTotem = ExtractIntValue(line);
        } else if (line.find("\"playtimeSeconds\"") != std::string::npos) {
            outSummary.playtimeSeconds = (uint32_t)ExtractIntValue(line);
        } else if (line.find("\"dateString\"") != std::string::npos) {
            outSummary.dateString = ExtractStringValue(line);
        } else if (line.find("\"unlockedFragments\"") != std::string::npos) {
            // Count trues in the fragment list
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
            outData.isValid = ExtractBoolValue(line);
        } else if (line.find("\"gameState\"") != std::string::npos) {
            outData.gameState = ExtractIntValue(line);
        } else if (line.find("\"realmName\"") != std::string::npos) {
            outData.realmName = ExtractStringValue(line);
        } else if (line.find("\"playerX\"") != std::string::npos) {
            outData.playerX = ExtractFloatValue(line);
        } else if (line.find("\"playerY\"") != std::string::npos) {
            outData.playerY = ExtractFloatValue(line);
        } else if (line.find("\"playerHealth\"") != std::string::npos) {
            outData.playerHealth = ExtractFloatValue(line);
        } else if (line.find("\"playerMaxHealth\"") != std::string::npos) {
            outData.playerMaxHealth = ExtractFloatValue(line);
        } else if (line.find("\"playerComposure\"") != std::string::npos) {
            outData.playerComposure = ExtractFloatValue(line);
        } else if (line.find("\"potionCount\"") != std::string::npos) {
            outData.potionCount = ExtractIntValue(line);
        } else if (line.find("\"shingles\"") != std::string::npos) {
            outData.shingles = ExtractIntValue(line);
        } else if (line.find("\"oreScraps\"") != std::string::npos) {
            outData.oreScraps = ExtractIntValue(line);
        } else if (line.find("\"equippedWeapon\"") != std::string::npos) {
            outData.equippedWeapon = ExtractIntValue(line);
        } else if (line.find("\"equippedArmor\"") != std::string::npos) {
            outData.equippedArmor = ExtractIntValue(line);
        } else if (line.find("\"equippedTotem\"") != std::string::npos) {
            outData.equippedTotem = ExtractIntValue(line);
        } else if (line.find("\"bossZilDefeated\"") != std::string::npos) {
            outData.bossZilDefeated = ExtractBoolValue(line);
        } else if (line.find("\"knightLenonSparCompleted\"") != std::string::npos) {
            outData.knightLenonSparCompleted = ExtractBoolValue(line);
        } else if (line.find("\"arenaHighestWave\"") != std::string::npos) {
            outData.arenaHighestWave = ExtractIntValue(line);
        } else if (line.find("\"playtimeSeconds\"") != std::string::npos) {
            outData.playtimeSeconds = (uint32_t)ExtractIntValue(line);
        } else if (line.find("\"timestamp\"") != std::string::npos) {
            outData.timestamp = (uint64_t)ExtractIntValue(line);
        } else if (line.find("\"dateString\"") != std::string::npos) {
            outData.dateString = ExtractStringValue(line);
        } else if (line.find("\"weaponUpgrades\"") != std::string::npos) {
            // e.g. [0, 1, 0, 0, 0]
            size_t openBracket = line.find('[');
            size_t closeBracket = line.find(']');
            if (openBracket != std::string::npos && closeBracket != std::string::npos) {
                std::string inner = line.substr(openBracket + 1, closeBracket - openBracket - 1);
                std::stringstream ss(inner);
                std::string item;
                int idx = 0;
                while (std::getline(ss, item, ',') && idx < 5) {
                    outData.weaponUpgrades[idx++] = std::stoi(item);
                }
            }
        } else if (line.find("\"unlockedWeapons\"") != std::string::npos) {
            size_t openBracket = line.find('[');
            size_t closeBracket = line.find(']');
            if (openBracket != std::string::npos && closeBracket != std::string::npos) {
                std::string inner = line.substr(openBracket + 1, closeBracket - openBracket - 1);
                std::stringstream ss(inner);
                std::string item;
                int idx = 0;
                while (std::getline(ss, item, ',') && idx < 5) {
                    outData.unlockedWeapons[idx++] = (item.find("true") != std::string::npos || item.find("1") != std::string::npos);
                }
            }
        } else if (line.find("\"unlockedFragments\"") != std::string::npos) {
            size_t openBracket = line.find('[');
            size_t closeBracket = line.find(']');
            if (openBracket != std::string::npos && closeBracket != std::string::npos) {
                std::string inner = line.substr(openBracket + 1, closeBracket - openBracket - 1);
                std::stringstream ss(inner);
                std::string item;
                int idx = 0;
                while (std::getline(ss, item, ',') && idx < 10) {
                    outData.unlockedFragments[idx++] = (item.find("true") != std::string::npos || item.find("1") != std::string::npos);
                }
            }
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
    file << "  \"equippedWeapon\": " << data.equippedWeapon << ",\n";
    file << "  \"equippedArmor\": " << data.equippedArmor << ",\n";
    file << "  \"equippedTotem\": " << data.equippedTotem << ",\n";

    // Weapon upgrades
    file << "  \"weaponUpgrades\": [";
    for (int i = 0; i < 5; ++i) {
        file << data.weaponUpgrades[i] << (i < 4 ? ", " : "");
    }
    file << "],\n";

    // Unlocked weapons
    file << "  \"unlockedWeapons\": [";
    for (int i = 0; i < 5; ++i) {
        file << (data.unlockedWeapons[i] ? "true" : "false") << (i < 4 ? ", " : "");
    }
    file << "],\n";

    // Unlocked fragments
    file << "  \"unlockedFragments\": [";
    for (int i = 0; i < 10; ++i) {
        file << (data.unlockedFragments[i] ? "true" : "false") << (i < 9 ? ", " : "");
    }
    file << "],\n";

    file << "  \"bossZilDefeated\": " << (data.bossZilDefeated ? "true" : "false") << ",\n";
    file << "  \"knightLenonSparCompleted\": " << (data.knightLenonSparCompleted ? "true" : "false") << ",\n";
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
    if (slotIndex >= 1 && slotIndex <= 3) {
        activeSlotIndex = slotIndex;
    }
}

int SaveManager::GetActiveSlot() {
    return activeSlotIndex;
}

std::string SaveManager::GetRealmNameFromState(int state) {
    switch (state) {
        case 1: return "The Ruined Castle";
        case 3: return "The Crossroads of the Damned";
        case 4: return "Fortified Realm of Zyphden";
        case 6: return "The Armory Sanctuary";
        case 7: return "The Royal Combat Arena";
        case 9: return "Zyphden Threshold";
        default: return "Land of Tydeshire";
    }
}
