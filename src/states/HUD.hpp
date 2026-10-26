#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include <string>
#include <vector>

class Player;

struct ActionSlotInfo {
    Rectangle bounds;
    const char* actionName;
    const char* keybind;
    float currentCooldown;
    float maxCooldown;
    int stockCount = -1; // -1 = infinite / normal ability, >= 0 = consumable stock
    Color themeColor;
    bool isActive = false;
};

class HUD {
public:
    // Modular Visual Cooldown Action Bar
    static void DrawActionSlot(const ActionSlotInfo& slot);
    static void DrawActionBar(int windowWidth, int windowHeight, const Player& player);

    // Primary Player-based Draw Methods
    static void DrawLobby(int windowWidth, int windowHeight, const Player& player, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore = false);
    static void DrawCombat(int windowWidth, int windowHeight, const Player& player, int currentWave, int totalWaves, bool hasUnreadLore = false);
    static void DrawCampaignHUD(int windowWidth, int windowHeight, const Player& player, const std::string& locationName, bool hasUnreadLore = false);

    // Backward-compatible float-based Draw Methods
    static void DrawLobby(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore = false);
    static void DrawCombat(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves, bool hasUnreadLore = false);
    static void DrawCampaignHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, const std::string& locationName, bool hasUnreadLore = false);

    // Headers, Banners & Overlays
    static void DrawLocationHeader(int windowWidth, const std::string& locationName);
    static void DrawBossHealthBar(int windowWidth, const std::string& bossName, float hp, float maxHp);
    static void DrawBossDialogueBanner(int windowWidth, int windowHeight, const std::string& speaker, const std::string& text, Color accentColor);
    static bool DrawNPCDialogueBox(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const std::string& speaker, const std::vector<std::string>& pages, int& currentPage, Color accentColor);
    static void DrawGameOver(int windowWidth, int windowHeight);
    static void DrawVictory(int windowWidth, int windowHeight);
};
