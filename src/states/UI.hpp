#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "../combat/CombatTypes.hpp"
#include <string>

class Player;

class GameUI {
public:
    // Returns 0 = None, 1 = Play, 2 = Controls
    static int DrawTitleScreen(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed);

    // Controls Menu Modal (Accessible from Title, Lobby, and Arena)
    static bool DrawControlsModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed);

    // Inventory & Lore Journal Modal ([I] Key)
    static bool DrawInventoryModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex);

    static void DrawLobbyHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore = false);
    static void DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves, bool hasUnreadLore = false);
    static void DrawBossHealthBar(int windowWidth, const std::string& bossName, float hp, float maxHp);
    static void DrawGameOver(int windowWidth, int windowHeight);
    static void DrawVictory(int windowWidth, int windowHeight);

    // Zil Defeat Branching Modal
    // Returns 0 = None, 1 = Retry Duel, 2 = Accept Spare & Proceed
    static int DrawZilDefeatModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool canRetry);

    // Cinematic Boss Dialogue Banner
    static void DrawBossDialogueBanner(int windowWidth, int windowHeight, const std::string& speaker, const std::string& text, Color accentColor);

    // Open World Borderlands Gateway Epilogue Screen
    // Returns: 0 = None, 1 = Re-enter Castle, 2 = Return to Lobby
    static int DrawBorderlandsGateway(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player);
};
