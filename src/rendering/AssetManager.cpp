#include "AssetManager.hpp"
#include "raymath.h"
#include <cmath>

AssetManager::~AssetManager() {
    Unload();
}

void AssetManager::Init() {
    if (initialized) return;
    initialized = true;

    if (FileExists("assets/player.png")) {
        LoadCustomTexture("player", "assets/player.png");
    }
    if (FileExists("assets/enemy_melee.png")) {
        LoadCustomTexture("enemy_melee", "assets/enemy_melee.png");
    }
    if (FileExists("assets/enemy_brute.png")) {
        LoadCustomTexture("enemy_brute", "assets/enemy_brute.png");
    }
}

void AssetManager::Unload() {
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
    textures.clear();
    initialized = false;
}

bool AssetManager::HasTexture(const std::string& key) const {
    return textures.find(key) != textures.end();
}

Texture2D AssetManager::GetTexture(const std::string& key) const {
    auto it = textures.find(key);
    if (it != textures.end()) {
        return it->second;
    }
    return Texture2D{0};
}

void AssetManager::LoadCustomTexture(const std::string& key, const std::string& filepath) {
    if (FileExists(filepath.c_str())) {
        Texture2D tex = LoadTexture(filepath.c_str());
        textures[key] = tex;
    }
}

void AssetManager::DrawEntityShadow(Vector2 position, float radius) {
    Vector2 shadowPos = { position.x, position.y + radius * 0.45f };
    DrawEllipse((int)shadowPos.x, (int)shadowPos.y, radius * 1.15f, radius * 0.55f, Color{ 10, 12, 18, 90 });
}

void AssetManager::DrawPlayerSilhouette(Vector2 position, float rotationAngle, Color bodyColor, bool isParrying, bool isAttacking) {
    DrawEntityShadow(position, 22.0f);

    float rad = rotationAngle * DEG2RAD;
    Vector2 dir = { cosf(rad), sinf(rad) };
    Vector2 normal = { -dir.y, dir.x };

    if (isParrying) {
        DrawCircleV(position, 34.0f, Color{ 64, 224, 208, 60 });
        DrawCircleLines((int)position.x, (int)position.y, 34.0f, Color{ 100, 255, 240, 220 });
        DrawCircleLines((int)position.x, (int)position.y, 36.0f, Color{ 255, 255, 255, 180 });
    }

    DrawCircleV(position, 22.0f, Color{ 25, 30, 42, 255 });
    DrawCircleV(position, 19.0f, bodyColor);

    Vector2 leftShoulder = { position.x - normal.x * 16.0f - dir.x * 4.0f, position.y - normal.y * 16.0f - dir.y * 4.0f };
    Vector2 rightShoulder = { position.x + normal.x * 16.0f - dir.x * 4.0f, position.y + normal.y * 16.0f - dir.y * 4.0f };
    DrawCircleV(leftShoulder, 8.0f, Color{ 38, 48, 68, 255 });
    DrawCircleV(rightShoulder, 8.0f, Color{ 38, 48, 68, 255 });
    DrawCircleV(leftShoulder, 6.0f, Color{ 80, 95, 130, 255 });
    DrawCircleV(rightShoulder, 6.0f, Color{ 80, 95, 130, 255 });

    DrawCircleV(position, 12.0f, Color{ 18, 22, 32, 255 });
    DrawCircleV(position, 10.0f, Color{ 48, 58, 80, 255 });

    Vector2 visorCenter = { position.x + dir.x * 7.0f, position.y + dir.y * 7.0f };
    DrawCircleV(visorCenter, 4.0f, isParrying ? Color{ 100, 255, 240, 255 } : Color{ 0, 229, 255, 255 });

    Vector2 handPos = { position.x + dir.x * 14.0f + normal.x * 12.0f, position.y + dir.y * 14.0f + normal.y * 12.0f };
    DrawCircleV(handPos, 5.0f, Color{ 30, 36, 50, 255 });
}

void AssetManager::DrawEnemySilhouette(Vector2 position, float rotationAngle, Color bodyColor, float radius, bool isStunned) {
    DrawEntityShadow(position, radius);

    float rad = rotationAngle * DEG2RAD;
    Vector2 dir = { cosf(rad), sinf(rad) };
    Vector2 normal = { -dir.y, dir.x };

    if (isStunned) {
        float time = (float)GetTime() * 6.0f;
        for (int i = 0; i < 3; i++) {
            float starAngle = time + (float)i * 2.094f;
            Vector2 starPos = {
                position.x + cosf(starAngle) * (radius + 12.0f),
                position.y + sinf(starAngle) * (radius * 0.5f + 6.0f) - 10.0f
            };
            DrawCircleV(starPos, 3.5f, Color{ 255, 230, 80, 255 });
        }
    }

    DrawCircleV(position, radius, Color{ 28, 12, 14, 255 });
    DrawCircleV(position, radius - 3.0f, isStunned ? Color{ 110, 80, 90, 255 } : bodyColor);

    Vector2 leftShoulder = { position.x - normal.x * (radius * 0.75f), position.y - normal.y * (radius * 0.75f) };
    Vector2 rightShoulder = { position.x + normal.x * (radius * 0.75f), position.y + normal.y * (radius * 0.75f) };
    DrawCircleV(leftShoulder, radius * 0.38f, Color{ 40, 16, 20, 255 });
    DrawCircleV(rightShoulder, radius * 0.38f, Color{ 40, 16, 20, 255 });

    DrawCircleV(position, radius * 0.55f, Color{ 20, 10, 12, 255 });

    Vector2 eyeLeft = { position.x + dir.x * (radius * 0.35f) - normal.x * 4.0f, position.y + dir.y * (radius * 0.35f) - normal.y * 4.0f };
    Vector2 eyeRight = { position.x + dir.x * (radius * 0.35f) + normal.x * 4.0f, position.y + dir.y * (radius * 0.35f) + normal.y * 4.0f };
    Color eyeColor = isStunned ? Color{ 255, 220, 0, 200 } : Color{ 255, 60, 60, 255 };
    DrawCircleV(eyeLeft, 2.5f, eyeColor);
    DrawCircleV(eyeRight, 2.5f, eyeColor);
}

void AssetManager::DrawSword(Vector2 origin, float angle, float length, Color bladeColor, Color glowColor) {
    float rad = angle * DEG2RAD;
    Vector2 dir = { cosf(rad), sinf(rad) };
    Vector2 normal = { -dir.y, dir.x };
    Vector2 tip = { origin.x + dir.x * length, origin.y + dir.y * length };

    DrawLineEx(origin, tip, 4.0f, glowColor);
    DrawLineEx(origin, tip, 2.0f, bladeColor);

    Vector2 guardL = { origin.x + dir.x * 10.0f - normal.x * 7.0f, origin.y + dir.y * 10.0f - normal.y * 7.0f };
    Vector2 guardR = { origin.x + dir.x * 10.0f + normal.x * 7.0f, origin.y + dir.y * 10.0f + normal.y * 7.0f };
    DrawLineEx(guardL, guardR, 3.0f, Color{ 200, 180, 120, 255 });
    DrawCircleV(origin, 3.0f, Color{ 220, 200, 130, 255 });
}
