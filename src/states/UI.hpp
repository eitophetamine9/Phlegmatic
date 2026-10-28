#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "../combat/CombatTypes.hpp"
#include <string>
#include <vector>

class Player;

class GameUI {
public:
    // Returns 0 = None, 1 = Controls, 2 = Test Arena, 11..13 = Load Slot 1..3, 21..23 = New Game Slot 1..3
    static int DrawTitleScreen(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool isArenaUnlocked, bool& showSlotModal, int& deletingSlotIndex);

    // Controls Menu Modal (Accessible from Title, Lobby, and Arena)
    static bool DrawControlsModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed);

    // Inventory & Lore Journal Modal ([I] Key)
    static bool DrawInventoryModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex);

    // Interactive Realm Map Modal ([M] Key)
    static bool DrawMapModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player, GameState currentState);

    static void DrawLobbyHUD(int windowWidth, int windowHeight, const Player& player, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore = false);
    static void DrawCombatHUD(int windowWidth, int windowHeight, const Player& player, int currentWave, int totalWaves, bool hasUnreadLore = false);
    static void DrawCampaignHUD(int windowWidth, int windowHeight, const Player& player, const std::string& locationName, bool hasUnreadLore = false);

    static void DrawLobbyHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore = false);
    static void DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves, bool hasUnreadLore = false);
    static void DrawCampaignHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, const std::string& locationName, bool hasUnreadLore = false);
    static void DrawBossHealthBar(int windowWidth, const std::string& bossName, float hp, float maxHp);
    static void DrawGameOver(int windowWidth, int windowHeight);
    static void DrawVictory(int windowWidth, int windowHeight);

    // Zil Defeat Branching Modal
    // Returns 0 = None, 1 = Retry Duel, 2 = Accept Spare & Proceed
    static int DrawZilDefeatModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool canRetry);

    // Cinematic Boss Dialogue Banner
    static void DrawBossDialogueBanner(int windowWidth, int windowHeight, const std::string& speaker, const std::string& text, Color accentColor);

    // Interactive Paginated NPC Dialogue Box with ESC dismiss
    static bool DrawNPCDialogueBox(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const std::string& speaker, const std::vector<std::string>& pages, int& currentPage, Color accentColor);

    // Open World Borderlands Gateway Epilogue Screen
    // Returns: 0 = None, 1 = Re-enter Castle, 2 = Return to Lobby
    static int DrawBorderlandsGateway(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player);

    // Realm Transition Screen (Zyphden Entry Placeholder Screen)
    // Returns true when player confirms entry into Zyphden
    static bool DrawZyphdenTransition(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, float dt);

    // Quarry Depths Transition Screen (Eastern Mines Entry Screen)
    // Returns true when player confirms descent into Quarry Mines
    static bool DrawQuarryTransition(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, float dt);
};
