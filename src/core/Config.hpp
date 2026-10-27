#pragma once
#include "raylib.h"
#include "../rendering/FontManager.hpp"
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

    // Movement: Dash settings (Q Key)
    inline constexpr float DASH_SPEED = 740.0f;
    inline constexpr float DASH_DURATION = 0.18f;
    inline constexpr float DASH_COOLDOWN = 0.55f;
    inline constexpr float DASH_STAMINA_COST = 18.0f;

    // Defense: Parry settings (F Key)
    inline constexpr float PARRY_WINDOW = 0.28f;       // Active parry duration (F pressed at right time)
    inline constexpr float PARRY_COOLDOWN = 0.60f;     // Cooldown after parry
    inline constexpr float PARRY_STAMINA_COST = 16.0f; // Small stamina cost
    inline constexpr float PARRY_STUN_DURATION = 2.0f; // Enemy stun duration upon being parried
    inline constexpr float COUNTER_BUFF_DURATION = 2.5f;// Extra damage window for player after parrying

    // Weapon: Sword settings (Balanced)
    inline constexpr float SWORD_LIGHT_DAMAGE = 18.0f;
    inline constexpr float SWORD_LIGHT_RANGE = 75.0f;
    inline constexpr float SWORD_LIGHT_ARC = 110.0f;
    inline constexpr float SWORD_LIGHT_DURATION = 0.22f;
    inline constexpr float SWORD_LIGHT_COOLDOWN = 0.35f;
    inline constexpr float SWORD_LIGHT_STAMINA = 15.0f;

    inline constexpr float SWORD_HEAVY_DAMAGE = 42.0f;
    inline constexpr float SWORD_HEAVY_RANGE = 115.0f;
    inline constexpr float SWORD_HEAVY_ARC = 140.0f;
    inline constexpr float SWORD_HEAVY_WINDUP = 0.18f;
    inline constexpr float SWORD_HEAVY_DURATION = 0.30f;
    inline constexpr float SWORD_HEAVY_COOLDOWN = 0.70f;
    inline constexpr float SWORD_HEAVY_STAMINA = 32.0f;

    // Weapon: Daggers settings (Fast, dual hits, whirlwind heavy)
    inline constexpr float DAGGERS_LIGHT_DAMAGE = 11.0f;
    inline constexpr float DAGGERS_LIGHT_RANGE = 62.0f;
    inline constexpr float DAGGERS_LIGHT_ARC = 95.0f;
    inline constexpr float DAGGERS_LIGHT_DURATION = 0.14f;
    inline constexpr float DAGGERS_LIGHT_COOLDOWN = 0.20f;
    inline constexpr float DAGGERS_LIGHT_STAMINA = 9.0f;

    inline constexpr float DAGGERS_HEAVY_DAMAGE = 38.0f;
    inline constexpr float DAGGERS_HEAVY_RANGE = 75.0f;
    inline constexpr float DAGGERS_HEAVY_ARC = 360.0f; // Whirlwind spin
    inline constexpr float DAGGERS_HEAVY_WINDUP = 0.12f;
    inline constexpr float DAGGERS_HEAVY_DURATION = 0.24f;
    inline constexpr float DAGGERS_HEAVY_COOLDOWN = 0.50f;
    inline constexpr float DAGGERS_HEAVY_STAMINA = 22.0f;

    // Weapon: Mace settings (Heavy crushing, high stagger, wide impact)
    inline constexpr float MACE_LIGHT_DAMAGE = 28.0f;
    inline constexpr float MACE_LIGHT_RANGE = 82.0f;
    inline constexpr float MACE_LIGHT_ARC = 85.0f;
    inline constexpr float MACE_LIGHT_DURATION = 0.28f;
    inline constexpr float MACE_LIGHT_COOLDOWN = 0.45f;
    inline constexpr float MACE_LIGHT_STAMINA = 22.0f;

    inline constexpr float MACE_HEAVY_DAMAGE = 52.0f;
    inline constexpr float MACE_HEAVY_RANGE = 135.0f;
    inline constexpr float MACE_HEAVY_ARC = 150.0f;
    inline constexpr float MACE_HEAVY_WINDUP = 0.26f;
    inline constexpr float MACE_HEAVY_DURATION = 0.38f;
    inline constexpr float MACE_HEAVY_COOLDOWN = 0.85f;
    inline constexpr float MACE_HEAVY_STAMINA = 40.0f;
}
