#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include <string>
#include <unordered_map>

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager();

    void Init();
    void Unload();

    bool HasTexture(const std::string& key) const;
    Texture2D GetTexture(const std::string& key) const;
    void LoadCustomTexture(const std::string& key, const std::string& filepath);

    // Procedural Renderers
    static void DrawEntityShadow(Vector2 position, float radius);
    static void DrawPlayerSilhouette(Vector2 position, float rotationAngle, Color bodyColor, bool isParrying, bool isDashing);
    static void DrawEnemySilhouette(Vector2 position, float rotationAngle, Color bodyColor, float radius, bool isStunned);

    // Weapons
    static void DrawSword(Vector2 origin, float angle, float length, Color bladeColor, Color glowColor);
    static void DrawDaggers(Vector2 originL, Vector2 originR, float angleL, float angleR, Color bladeColor, Color glowColor);
    static void DrawMace(Vector2 origin, float angle, float length, Color headColor, Color glowColor);

    // Pedestals & Environment
    static void DrawWeaponPedestal(Vector2 position, WeaponType weapon, bool isSelected, bool isHovered, float time);

private:
    std::unordered_map<std::string, Texture2D> textures;
    bool initialized = false;
};
