#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "../combat/CombatTypes.hpp"
#include <string>

class GameUI {
public:
    // Returns 0 = None, 1 = Play, 2 = Controls
    static int DrawTitleScreen(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed);

    // Controls Menu Modal (Accessible from Title, Lobby, and Arena)
    static bool DrawControlsModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed);

    static void DrawLobbyHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, bool isNearPedestal, const std::string& nearWeaponName);
    static void DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves);
    static void DrawBossHealthBar(int windowWidth, const std::string& bossName, float hp, float maxHp);
    static void DrawGameOver(int windowWidth, int windowHeight);
    static void DrawVictory(int windowWidth, int windowHeight);
};
