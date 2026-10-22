#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "../combat/CombatTypes.hpp"
#include <string>

class GameUI {
public:
    static bool DrawTitleScreen(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed);
    static void DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves);
    static void DrawGameOver(int windowWidth, int windowHeight);
    static void DrawVictory(int windowWidth, int windowHeight);
};
