#pragma once
#include "raylib.h"
#include "raymath.h"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include <string>
#include <vector>

enum class TutorialStep {
    AWAKENING,        // Step 1: Move with [W, A, S, D]
    WEAPON_STAND,     // Step 2: Claim Broadsword & Commoner's Scrap [E]
    COMBAT_BASICS,    // Step 3: Test Light [L-Click] & Heavy [R-Click] attacks
    INVENTORY_CHECK,  // Step 4: Open Journal / Inventory [I] to inspect equipment and attributes
    LORE_DISCOVERY,   // Step 5: Cross eastern bridge to open Ancient Relic Chest [E]
    DEFENSE_SENTINEL, // Step 6: Evade with [Q] Dash & stagger with [F] Parry
    GATE_OPENED,      // Step 7: Sanctuary steps unlocked, proceed north to High Altar
    COMPLETED
};

enum class LoreType {
    STELE,      // Ancient stone stele / tablet
    CHEST,      // Wooden & iron relic chest
    SHIELD,     // Sun Crusader shield & sword relic
    RELIQUARY   // Royal gold & obsidian reliquary
};

struct LoreCollectible {
    std::string fragmentId;
    std::string title;
    std::string label;
    Vector2 position;
    float radius = 28.0f;
    bool collected = false;
    LoreType type = LoreType::CHEST;

    bool CheckInteraction(Vector2 playerPos) const {
        return !collected && Vector2Distance(playerPos, position) <= (radius + 36.0f);
    }

    void Draw(float time, bool isInteractable = true, Vector2 playerPos = { -9999.0f, -9999.0f }) const;
};

struct AncientChest {
    Vector2 position = { 1100.0f, 840.0f };
    float radius = 30.0f;
    bool opened = false;

    bool CheckInteraction(Vector2 playerPos) const {
        return !opened && Vector2Distance(playerPos, position) <= (radius + 32.0f);
    }

    void Draw(float time, Vector2 playerPos = { -9999.0f, -9999.0f }) const;
};

class TutorialManager {
public:
    TutorialManager();

    void Init();
    void Update(float dt, Player& player, GameMap& map, ParticleSystem& particles, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles);
    void DrawWorldElements(float time, Vector2 playerPos = { -9999.0f, -9999.0f }) const;
    void DrawHUD(int windowWidth, int windowHeight) const;
    bool DrawPopup(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed);

    bool HasActivePopup() const { return showPopup; }
    void DismissPopup() { showPopup = false; }

    TutorialStep GetStep() const { return currentStep; }
    bool IsCompleted() const { return currentStep == TutorialStep::COMPLETED; }
    bool HasClaimedWeapon() const { return hasClaimedWeapon; }
    bool HasClaimedArmor() const { return hasClaimedArmor; }
    bool HasDefeatedTutorialEnemy() const { return tutorialEnemyDefeated; }
    void SkipToBoss(GameMap& map);

    AncientChest& GetChest() { return chest; }
    const std::vector<LoreCollectible>& GetLoreItems() const { return loreItems; }
    std::vector<LoreCollectible>& GetLoreItems() { return loreItems; }
    Vector2 GetWeaponStandPos() const { return weaponStandPos; }
    Vector2 GetArmorStandPos() const { return armorStandPos; }

    void TriggerStepPopup(TutorialStep step);
    void NotifyInventoryOpened() { hasOpenedInventory = true; }
    bool IsObjectivesVisible() const { return showObjectives; }
    void ToggleObjectives() { showObjectives = !showObjectives; }

private:
    TutorialStep currentStep;
    Vector2 initialPlayerPos;
    float movementDistance;

    Vector2 weaponStandPos;
    bool hasClaimedWeapon;

    Vector2 armorStandPos;
    bool hasClaimedArmor;

    int lightAttacksLanded;
    int heavyAttacksLanded;
    bool hasOpenedInventory;

    AncientChest chest;
    bool hasOpenedChest;

    std::vector<LoreCollectible> loreItems;

    bool sentinelSpawned;
    bool tutorialEnemyDefeated;
    int parriesAchieved;

    float stepTransitionTimer;
    std::string currentObjectiveText;
    std::string currentControlHint;



    // Pop-up modal state
    bool showPopup;
    std::string popupTitle;
    std::string popupBody;
    std::string popupHint;

    mutable bool showObjectives = true;
};
