#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager();

    void Init();
    void Unload();

    // Check if custom asset was provided
    bool HasTexture(const std::string& key) const;
    Texture2D GetTexture(const std::string& key) const;
    void LoadCustomTexture(const std::string& key, const std::string& filepath);

    // Procedural Fallback Renderers (Sleek silhouettes ready for texture replacement)
    static void DrawEntityShadow(Vector2 position, float radius);
    static void DrawPlayerSilhouette(Vector2 position, float rotationAngle, Color bodyColor, bool isParrying, bool isAttacking);
    static void DrawEnemySilhouette(Vector2 position, float rotationAngle, Color bodyColor, float radius, bool isStunned);
    static void DrawSword(Vector2 origin, float angle, float length, Color bladeColor, Color glowColor);

private:
    std::unordered_map<std::string, Texture2D> textures;
    bool initialized = false;
};
