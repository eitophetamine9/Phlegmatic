#pragma once
#include "raylib.h"
#include <string>

namespace Config {
    // Window settings
    inline constexpr int WINDOW_WIDTH = 1280;
    inline constexpr int WINDOW_HEIGHT = 720;
    inline const char* WINDOW_TITLE = "Phlegmatic - Top-Down Action RPG";
    inline constexpr int TARGET_FPS = 60;

    // World settings
    inline constexpr int MAP_WIDTH = 2400;
    inline constexpr int MAP_HEIGHT = 1800;
    inline constexpr int TILE_SIZE = 64;

    // Player settings
    inline constexpr float PLAYER_SPEED = 280.0f;
    inline constexpr float PLAYER_RADIUS = 22.0f;
    inline constexpr float PLAYER_MAX_HEALTH = 100.0f;
    inline constexpr float PLAYER_MAX_STAMINA = 100.0f;
    inline constexpr float STAMINA_REGEN_RATE = 28.0f;

    // Combat settings
    inline constexpr float LIGHT_ATTACK_STAMINA = 15.0f;
    inline constexpr float LIGHT_ATTACK_DAMAGE = 25.0f;
    inline constexpr float LIGHT_ATTACK_RANGE = 75.0f;
    inline constexpr float LIGHT_ATTACK_ARC = 110.0f; // degrees
    inline constexpr float LIGHT_ATTACK_DURATION = 0.22f;
    inline constexpr float LIGHT_ATTACK_COOLDOWN = 0.35f;

    inline constexpr float HEAVY_ATTACK_STAMINA = 32.0f;
    inline constexpr float HEAVY_ATTACK_DAMAGE = 65.0f;
    inline constexpr float HEAVY_ATTACK_RANGE = 115.0f;
    inline constexpr float HEAVY_ATTACK_ARC = 140.0f;
    inline constexpr float HEAVY_ATTACK_WINDUP = 0.18f;
    inline constexpr float HEAVY_ATTACK_DURATION = 0.32f;
    inline constexpr float HEAVY_ATTACK_COOLDOWN = 0.70f;

    // Parry settings
    inline constexpr float PARRY_WINDOW = 0.28f;       // Active parry duration (Q pressed at the right time)
    inline constexpr float PARRY_COOLDOWN = 0.65f;     // Cooldown if parry fails or completes
    inline constexpr float PARRY_STAMINA_COST = 20.0f;  // Small stamina cost
    inline constexpr float PARRY_STUN_DURATION = 2.0f; // Enemy stun duration upon being parried
    inline constexpr float COUNTER_BUFF_DURATION = 2.5f;// Extra damage window for player after parrying
}
