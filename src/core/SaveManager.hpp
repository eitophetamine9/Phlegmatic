#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct SaveSlotSummary {
    int slotIndex = 1;                     // 1, 2, or 3
    bool isValid = false;
    std::string realmName = "Ruined Castle";
    float playerHealth = 100.0f;
    float playerMaxHealth = 100.0f;
    int shingles = 0;
    int oreScraps = 0;
    int essenceOfMist = 0;
    int shadowEssence = 0;
    bool galeArcherDefeated = false;
    int arenaHighestWave = 0;
    int potionCount = 1;
    int equippedWeapon = 1;                // 0: Unarmed, 1: Sword, 2: Daggers, 3: Mace, 4: Truth Seeker, 5: Halberd
    int equippedArmor = 0;
    int equippedHelmet = 0;
    int equippedChestplate = 0;
    int equippedLeggings = 0;
    int equippedTotem = 0;
    int discoveredFragments = 0;
    int totalFragments = 10;
    uint32_t playtimeSeconds = 0;
    std::string dateString = "";
};

struct GameSaveData {
    int slotIndex = 1;
    bool isValid = false;
    int gameState = 1;                     // GameState enum value
    std::string realmName = "Ruined Castle";
    float playerX = 800.0f;
    float playerY = 800.0f;
    float playerHealth = 100.0f;
    float playerMaxHealth = 100.0f;
    float playerComposure = 50.0f;
    int potionCount = 1;
    int shingles = 0;
    int oreScraps = 0;
    int essenceOfMist = 0;
    int shadowEssence = 0;
    bool galeArcherDefeated = false;
    int equippedWeapon = 1;
    int equippedArmor = 0;
    int equippedHelmet = 0;
    int equippedChestplate = 0;
    int equippedLeggings = 0;
    int equippedTotem = 0;
    int weaponUpgrades[6] = { 0 };
    bool unlockedWeapons[6] = { true, true, false, false, false, false };
    bool unlockedHelmets[7] = { true, false, false, false, false, false, false };
    bool unlockedChestplates[7] = { true, false, false, false, false, false, false };
    bool unlockedLeggings[7] = { true, false, false, false, false, false, false };
    bool unlockedTotems[6] = { true, false, false, false, false, false };
    bool unlockedFragments[10] = { false };
    bool bossZilDefeated = false;
    bool knightLenonSparCompleted = false;
    int zyphdenQuestPhase = 0;
    int arenaHighestWave = 0;
    uint32_t playtimeSeconds = 0;
    uint64_t timestamp = 0;
    std::string dateString = "";
};

class SaveManager {
public:
    static std::string GetSlotFilePath(int slotIndex);
    static bool HasSave(int slotIndex);
    static bool LoadSummary(int slotIndex, SaveSlotSummary& outSummary);
    static bool LoadGame(int slotIndex, GameSaveData& outData);
    static bool SaveGame(int slotIndex, const GameSaveData& data);
    static bool DeleteSave(int slotIndex);

    // Active slot tracking for current play session
    static void SetActiveSlot(int slotIndex);
    static int GetActiveSlot();

    static std::string GetRealmNameFromState(int state);

private:
    static int activeSlotIndex;
};
