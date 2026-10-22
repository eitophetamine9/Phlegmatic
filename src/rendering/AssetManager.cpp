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

void AssetManager::DrawPlayerSilhouette(Vector2 position, float rotationAngle, Color bodyColor, bool isParrying, bool isDashing) {
    DrawEntityShadow(position, 22.0f);

    float rad = rotationAngle * DEG2RAD;
    Vector2 dir = { cosf(rad), sinf(rad) };
    Vector2 normal = { -dir.y, dir.x };

    // Parry stance glow / energy ring
    if (isParrying) {
        DrawCircleV(position, 34.0f, Color{ 64, 224, 208, 60 });
        DrawCircleLines((int)position.x, (int)position.y, 34.0f, Color{ 100, 255, 240, 220 });
        DrawCircleLines((int)position.x, (int)position.y, 36.0f, Color{ 255, 255, 255, 180 });
    }

    // Outer armor rim
    DrawCircleV(position, 22.0f, Color{ 25, 30, 42, 255 });

    // Inner cloak / body
    DrawCircleV(position, 19.0f, bodyColor);

    // Pauldrons
    Vector2 leftShoulder = { position.x - normal.x * 16.0f - dir.x * 4.0f, position.y - normal.y * 16.0f - dir.y * 4.0f };
    Vector2 rightShoulder = { position.x + normal.x * 16.0f - dir.x * 4.0f, position.y + normal.y * 16.0f - dir.y * 4.0f };
    DrawCircleV(leftShoulder, 8.0f, Color{ 38, 48, 68, 255 });
    DrawCircleV(rightShoulder, 8.0f, Color{ 38, 48, 68, 255 });
    DrawCircleV(leftShoulder, 6.0f, Color{ 80, 95, 130, 255 });
    DrawCircleV(rightShoulder, 6.0f, Color{ 80, 95, 130, 255 });

    // Head / Helm
    DrawCircleV(position, 12.0f, Color{ 18, 22, 32, 255 });
    DrawCircleV(position, 10.0f, Color{ 48, 58, 80, 255 });

    // Visor
    Vector2 visorCenter = { position.x + dir.x * 7.0f, position.y + dir.y * 7.0f };
    DrawCircleV(visorCenter, 4.0f, isParrying ? Color{ 100, 255, 240, 255 } : Color{ 0, 229, 255, 255 });

    // Hands
    Vector2 handPosL = { position.x + dir.x * 14.0f - normal.x * 12.0f, position.y + dir.y * 14.0f - normal.y * 12.0f };
    Vector2 handPosR = { position.x + dir.x * 14.0f + normal.x * 12.0f, position.y + dir.y * 14.0f + normal.y * 12.0f };
    DrawCircleV(handPosL, 5.0f, Color{ 30, 36, 50, 255 });
    DrawCircleV(handPosR, 5.0f, Color{ 30, 36, 50, 255 });
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

void AssetManager::DrawDaggers(Vector2 originL, Vector2 originR, float angleL, float angleR, Color bladeColor, Color glowColor) {
    // Left Dagger
    float radL = angleL * DEG2RAD;
    Vector2 dirL = { cosf(radL), sinf(radL) };
    Vector2 tipL = { originL.x + dirL.x * 24.0f, originL.y + dirL.y * 24.0f };
    DrawLineEx(originL, tipL, 3.5f, glowColor);
    DrawLineEx(originL, tipL, 2.0f, bladeColor);
    DrawCircleV(originL, 2.5f, Color{ 180, 220, 200, 255 });

    // Right Dagger
    float radR = angleR * DEG2RAD;
    Vector2 dirR = { cosf(radR), sinf(radR) };
    Vector2 tipR = { originR.x + dirR.x * 24.0f, originR.y + dirR.y * 24.0f };
    DrawLineEx(originR, tipR, 3.5f, glowColor);
    DrawLineEx(originR, tipR, 2.0f, bladeColor);
    DrawCircleV(originR, 2.5f, Color{ 180, 220, 200, 255 });
}

void AssetManager::DrawMace(Vector2 origin, float angle, float length, Color headColor, Color glowColor) {
    float rad = angle * DEG2RAD;
    Vector2 dir = { cosf(rad), sinf(rad) };
    Vector2 normal = { -dir.y, dir.x };
    Vector2 tip = { origin.x + dir.x * length, origin.y + dir.y * length };

    // Shaft
    DrawLineEx(origin, tip, 4.0f, Color{ 80, 60, 45, 255 });
    DrawLineEx(origin, tip, 2.0f, Color{ 130, 100, 75, 255 });

    // Heavy Spiked Flanged Mace Head
    Vector2 headPos = { origin.x + dir.x * (length - 4.0f), origin.y + dir.y * (length - 4.0f) };
    DrawCircleV(headPos, 11.0f, glowColor);
    DrawCircleV(headPos, 8.5f, headColor);
    DrawCircleLines((int)headPos.x, (int)headPos.y, 8.5f, Color{ 40, 30, 35, 255 });

    // Flange spikes
    DrawLineEx({ headPos.x - normal.x * 12.0f, headPos.y - normal.y * 12.0f },
               { headPos.x + normal.x * 12.0f, headPos.y + normal.y * 12.0f }, 3.5f, Color{ 220, 200, 180, 255 });
    DrawLineEx({ headPos.x - dir.x * 10.0f, headPos.y - dir.y * 10.0f },
               { headPos.x + dir.x * 10.0f, headPos.y + dir.y * 10.0f }, 3.5f, Color{ 220, 200, 180, 255 });

    DrawCircleV(origin, 3.5f, Color{ 160, 140, 110, 255 });
}

void AssetManager::DrawWeaponPedestal(Vector2 position, WeaponType weapon, bool isSelected, bool isHovered, float time) {
    // Pedestal Stone Base
    DrawCircle((int)position.x, (int)(position.y + 16), 36.0f, Color{ 12, 16, 24, 140 });
    DrawCircle((int)position.x, (int)position.y, 34.0f, Color{ 32, 40, 56, 255 });
    DrawCircle((int)position.x, (int)position.y, 30.0f, Color{ 48, 58, 80, 255 });

    // Glow Ring
    Color ringColor = isSelected ? Color{ 255, 215, 80, 240 } : (isHovered ? Color{ 100, 220, 255, 220 } : Color{ 60, 80, 110, 180 });
    DrawCircleLines((int)position.x, (int)position.y, 34.0f, ringColor);
    if (isSelected || isHovered) {
        float pulse = 1.0f + 0.08f * sinf(time * 4.0f);
        DrawCircleLines((int)position.x, (int)position.y, 38.0f * pulse, ringColor);
    }

    // Floating weapon icon preview above pedestal
    float floatOffset = sinf(time * 3.0f + position.x) * 5.0f - 14.0f;
    Vector2 weaponAnchor = { position.x, position.y + floatOffset };

    if (weapon == WeaponType::SWORD) {
        DrawSword({ weaponAnchor.x - 14.0f, weaponAnchor.y + 14.0f }, -45.0f, 38.0f, Color{ 240, 245, 255, 255 }, Color{ 0, 220, 255, 200 });
        const char* name = "BROADSWORD";
        int w = MeasureText(name, 11);
        DrawText(name, (int)(position.x - w * 0.5f), (int)(position.y + 40), 11, isSelected ? Color{ 255, 220, 90, 255 } : Color{ 200, 215, 235, 220 });
    } else if (weapon == WeaponType::DAGGERS) {
        DrawDaggers({ weaponAnchor.x - 8.0f, weaponAnchor.y }, { weaponAnchor.x + 8.0f, weaponAnchor.y }, -60.0f, -120.0f, Color{ 180, 255, 220, 255 }, Color{ 60, 240, 180, 200 });
        const char* name = "DUAL DAGGERS";
        int w = MeasureText(name, 11);
        DrawText(name, (int)(position.x - w * 0.5f), (int)(position.y + 40), 11, isSelected ? Color{ 255, 220, 90, 255 } : Color{ 200, 215, 235, 220 });
    } else if (weapon == WeaponType::MACE) {
        DrawMace({ weaponAnchor.x - 12.0f, weaponAnchor.y + 14.0f }, -45.0f, 36.0f, Color{ 230, 180, 120, 255 }, Color{ 255, 140, 40, 200 });
        const char* name = "WAR MACE";
        int w = MeasureText(name, 11);
        DrawText(name, (int)(position.x - w * 0.5f), (int)(position.y + 40), 11, isSelected ? Color{ 255, 220, 90, 255 } : Color{ 200, 215, 235, 220 });
    }

    if (isSelected) {
        const char* eq = "[EQUIPPED]";
        int ew = MeasureText(eq, 10);
        DrawText(eq, (int)(position.x - ew * 0.5f), (int)(position.y + 54), 10, Color{ 255, 215, 80, 255 });
    } else if (isHovered) {
        const char* pr = "[E] Equip";
        int pw = MeasureText(pr, 10);
        DrawText(pr, (int)(position.x - pw * 0.5f), (int)(position.y + 54), 10, Color{ 100, 220, 255, 255 });
    }
}
