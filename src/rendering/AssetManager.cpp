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

void AssetManager::DrawPlayerSilhouette(Vector2 position, float rotationAngle, Color bodyColor, bool isParrying, bool isDashing, Color trimColor, Color helmColor, Color visorColor) {
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
    DrawCircleV(leftShoulder, 6.0f, trimColor);
    DrawCircleV(rightShoulder, 6.0f, trimColor);

    // Head / Helm
    DrawCircleV(position, 12.0f, Color{ 18, 22, 32, 255 });
    DrawCircleV(position, 10.0f, helmColor);

    // Visor
    Vector2 visorCenter = { position.x + dir.x * 7.0f, position.y + dir.y * 7.0f };
    DrawCircleV(visorCenter, 4.0f, isParrying ? Color{ 100, 255, 240, 255 } : visorColor);

    // Hands
    Vector2 handPosL = { position.x + dir.x * 14.0f - normal.x * 12.0f, position.y + dir.y * 14.0f - normal.y * 12.0f };
    Vector2 handPosR = { position.x + dir.x * 14.0f + normal.x * 12.0f, position.y + dir.y * 14.0f + normal.y * 12.0f };
    DrawCircleV(handPosL, 5.0f, Color{ 30, 36, 50, 255 });
    DrawCircleV(handPosR, 5.0f, Color{ 30, 36, 50, 255 });
}

void AssetManager::DrawCharacterDoll(Vector2 center, float scale, WeaponType weapon, ArmorType armor, TotemType totem, float time) {
    ArmorStats aStats = GetArmorStats(armor);
    TotemStats tStats = GetTotemStats(totem);

    // Stone dais / runic base below feet
    Vector2 daisPos = { center.x, center.y + 75.0f * scale };
    DrawEllipse((int)daisPos.x, (int)daisPos.y, 48.0f * scale, 16.0f * scale, Color{ 12, 16, 24, 200 });
    DrawEllipse((int)daisPos.x, (int)daisPos.y, 44.0f * scale, 14.0f * scale, Color{ 25, 32, 48, 255 });
    DrawEllipseLines((int)daisPos.x, (int)daisPos.y, 44.0f * scale, 14.0f * scale, Color{ 60, 80, 115, 255 });

    // Totem glowing ring / runes on the dais
    if (totem != TotemType::NONE) {
        float pulse = 0.85f + 0.15f * sinf(time * 3.0f);
        Color runeCol = tStats.glowColor;
        runeCol.a = (unsigned char)(140 * pulse);
        DrawEllipseLines((int)daisPos.x, (int)daisPos.y, 40.0f * scale * pulse, 12.0f * scale * pulse, runeCol);
    }

    // Floating idle breathing animation
    float bob = sinf(time * 2.2f) * 3.5f * scale;
    Vector2 p = { center.x, center.y + bob };

    // Shadow directly under character
    DrawEllipse((int)p.x, (int)(daisPos.y - 4.0f * scale), 26.0f * scale, 8.0f * scale, Color{ 8, 10, 16, 180 });

    // Totem aura particles/glow behind character
    if (totem != TotemType::NONE) {
        float auraPulse = 1.0f + 0.1f * sinf(time * 2.8f);
        Color auraBg = tStats.glowColor;
        auraBg.a = 28;
        DrawCircleV({ p.x, p.y + 10.0f * scale }, 65.0f * scale * auraPulse, auraBg);
        auraBg.a = 55;
        DrawCircleV({ p.x, p.y + 10.0f * scale }, 48.0f * scale * auraPulse, auraBg);

        // Orbiting totem floating talisman near shoulder
        float relicAngle = time * 1.5f;
        Vector2 relicPos = {
            p.x - 38.0f * scale + cosf(relicAngle) * 6.0f * scale,
            p.y - 35.0f * scale + sinf(relicAngle * 1.4f) * 6.0f * scale
        };
        DrawCircleV(relicPos, 10.0f * scale, Color{ 14, 18, 28, 220 });
        DrawCircleV(relicPos, 7.5f * scale, tStats.glowColor);
        DrawCircleLines((int)relicPos.x, (int)relicPos.y, 7.5f * scale, Color{ 255, 255, 255, 220 });
        DrawCircleV(relicPos, 3.5f * scale, Color{ 255, 255, 255, 255 });

        // Tiny floating motes
        for (int i = 0; i < 3; i++) {
            float moteAngle = relicAngle + (float)i * 2.094f;
            Vector2 motePos = {
                relicPos.x + cosf(moteAngle) * (14.0f * scale),
                relicPos.y + sinf(moteAngle) * (10.0f * scale)
            };
            DrawCircleV(motePos, 2.0f * scale, tStats.glowColor);
        }
    }

    // Flowing Cape (behind torso & legs)
    float capeSway = sinf(time * 1.8f) * 4.0f * scale;
    Vector2 capeTopL = { p.x - 18.0f * scale, p.y - 5.0f * scale };
    Vector2 capeTopR = { p.x + 18.0f * scale, p.y - 5.0f * scale };
    Vector2 capeBotL = { p.x - 22.0f * scale + capeSway, p.y + 60.0f * scale };
    Vector2 capeBotR = { p.x + 22.0f * scale + capeSway, p.y + 60.0f * scale };
    DrawTriangle(capeTopL, capeBotL, capeTopR, Color{ 16, 20, 30, 240 });
    DrawTriangle(capeTopR, capeBotL, capeBotR, Color{ 16, 20, 30, 240 });
    DrawLineEx(capeBotL, capeBotR, 2.0f * scale, aStats.trimColor);

    // Legs / Greaves
    // Left Leg
    Rectangle legL = { p.x - 14.0f * scale, p.y + 28.0f * scale, 10.0f * scale, 38.0f * scale };
    DrawRectangleRounded(legL, 0.25f, 4, Color{ 24, 28, 40, 255 });
    DrawRectangleRoundedLines(legL, 0.25f, 4, aStats.primaryColor);
    // Left Boot / Sabaton
    Rectangle bootL = { p.x - 16.0f * scale, p.y + 60.0f * scale, 13.0f * scale, 12.0f * scale };
    DrawRectangleRounded(bootL, 0.3f, 4, aStats.helmColor);
    DrawRectangleRoundedLines(bootL, 0.3f, 4, aStats.trimColor);

    // Right Leg
    Rectangle legR = { p.x + 4.0f * scale, p.y + 28.0f * scale, 10.0f * scale, 38.0f * scale };
    DrawRectangleRounded(legR, 0.25f, 4, Color{ 24, 28, 40, 255 });
    DrawRectangleRoundedLines(legR, 0.25f, 4, aStats.primaryColor);
    // Right Boot / Sabaton
    Rectangle bootR = { p.x + 3.0f * scale, p.y + 60.0f * scale, 13.0f * scale, 12.0f * scale };
    DrawRectangleRounded(bootR, 0.3f, 4, aStats.helmColor);
    DrawRectangleRoundedLines(bootR, 0.3f, 4, aStats.trimColor);

    // Torso & Cuirass
    Rectangle torsoRec = { p.x - 19.0f * scale, p.y - 10.0f * scale, 38.0f * scale, 40.0f * scale };
    DrawRectangleRounded(torsoRec, 0.2f, 4, aStats.primaryColor);
    DrawRectangleRoundedLines(torsoRec, 0.2f, 4, aStats.trimColor);

    // Breastplate decorative center ridge & filigree
    DrawLineEx({ p.x, p.y - 8.0f * scale }, { p.x, p.y + 22.0f * scale }, 2.5f * scale, aStats.trimColor);
    DrawLineEx({ p.x - 12.0f * scale, p.y + 4.0f * scale }, { p.x + 12.0f * scale, p.y + 4.0f * scale }, 2.0f * scale, aStats.trimColor);

    // Belt
    Rectangle beltRec = { p.x - 19.0f * scale, p.y + 24.0f * scale, 38.0f * scale, 7.0f * scale };
    DrawRectangleRec(beltRec, Color{ 28, 20, 16, 255 });
    // Belt Buckle
    Rectangle buckleRec = { p.x - 5.0f * scale, p.y + 23.0f * scale, 10.0f * scale, 9.0f * scale };
    DrawRectangleRec(buckleRec, Color{ 220, 180, 80, 255 });
    DrawRectangleLinesEx(buckleRec, 1.0f * scale, Color{ 140, 100, 30, 255 });

    // Pauldrons (Left & Right Shoulders)
    Vector2 leftShoulder = { p.x - 22.0f * scale, p.y - 8.0f * scale };
    Vector2 rightShoulder = { p.x + 22.0f * scale, p.y - 8.0f * scale };

    DrawCircleV(leftShoulder, 13.0f * scale, aStats.helmColor);
    DrawCircleLines((int)leftShoulder.x, (int)leftShoulder.y, 13.0f * scale, aStats.trimColor);
    DrawCircleV(leftShoulder, 9.0f * scale, aStats.primaryColor);

    DrawCircleV(rightShoulder, 13.0f * scale, aStats.helmColor);
    DrawCircleLines((int)rightShoulder.x, (int)rightShoulder.y, 13.0f * scale, aStats.trimColor);
    DrawCircleV(rightShoulder, 9.0f * scale, aStats.primaryColor);

    // Arms & Vambraces
    // Left Arm (relaxed at side)
    Rectangle armL = { p.x - 25.0f * scale, p.y + 3.0f * scale, 8.0f * scale, 24.0f * scale };
    DrawRectangleRounded(armL, 0.3f, 4, Color{ 24, 28, 40, 255 });
    DrawRectangleRoundedLines(armL, 0.3f, 4, aStats.trimColor);
    // Left Gauntlet
    DrawCircleV({ p.x - 21.0f * scale, p.y + 28.0f * scale }, 6.0f * scale, aStats.helmColor);

    // Right Arm (angled forward holding weapon grip)
    Rectangle armR = { p.x + 18.0f * scale, p.y + 2.0f * scale, 9.0f * scale, 22.0f * scale };
    DrawRectangleRounded(armR, 0.3f, 4, Color{ 24, 28, 40, 255 });
    DrawRectangleRoundedLines(armR, 0.3f, 4, aStats.trimColor);
    // Right Gauntlet
    Vector2 handR = { p.x + 22.0f * scale, p.y + 25.0f * scale };
    DrawCircleV(handR, 6.5f * scale, aStats.helmColor);
    DrawCircleLines((int)handR.x, (int)handR.y, 6.5f * scale, aStats.trimColor);

    // Helmet & Neck Gorget
    // Neck gorget
    DrawRectangle((int)(p.x - 8.0f * scale), (int)(p.y - 16.0f * scale), (int)(16.0f * scale), (int)(8.0f * scale), aStats.helmColor);

    // Helmet Head
    Rectangle helmRec = { p.x - 14.0f * scale, p.y - 44.0f * scale, 28.0f * scale, 30.0f * scale };
    DrawRectangleRounded(helmRec, 0.35f, 6, aStats.helmColor);
    DrawRectangleRoundedLines(helmRec, 0.35f, 6, aStats.trimColor);

    // Visor Slit
    Rectangle visorRec = { p.x - 10.0f * scale, p.y - 32.0f * scale, 20.0f * scale, 5.0f * scale };
    DrawRectangleRounded(visorRec, 0.3f, 4, Color{ 14, 18, 26, 255 });
    DrawRectangleRounded(Rectangle{ p.x - 8.0f * scale, p.y - 31.0f * scale, 16.0f * scale, 3.0f * scale }, 0.2f, 4, aStats.visorColor);

    // Helmet Plume / Crest
    Vector2 plumeBase = { p.x, p.y - 44.0f * scale };
    Vector2 plumeTip = { p.x - 4.0f * scale + sinf(time * 2.5f) * 2.0f * scale, p.y - 58.0f * scale };
    DrawLineEx(plumeBase, plumeTip, 3.5f * scale, aStats.trimColor);
    DrawCircleV(plumeTip, 3.0f * scale, aStats.visorColor);

    // Equipped Weapon in Hand
    if (weapon == WeaponType::SWORD) {
        Vector2 hilt = { handR.x + 3.0f * scale, handR.y - 2.0f * scale };
        Vector2 tip = { hilt.x + 8.0f * scale, hilt.y - 62.0f * scale };
        DrawCircleV({ hilt.x - 1.0f * scale, hilt.y + 10.0f * scale }, 3.5f * scale, Color{ 220, 190, 80, 255 });
        DrawLineEx({ hilt.x, hilt.y + 8.0f * scale }, hilt, 3.0f * scale, Color{ 60, 45, 30, 255 });
        DrawLineEx({ hilt.x - 12.0f * scale, hilt.y - 1.0f * scale }, { hilt.x + 12.0f * scale, hilt.y + 1.0f * scale }, 4.0f * scale, Color{ 220, 190, 80, 255 });
        DrawLineEx(hilt, tip, 5.0f * scale, Color{ 160, 210, 255, 120 });
        DrawLineEx(hilt, tip, 3.0f * scale, Color{ 235, 245, 255, 255 });
        float shimmerT = fmodf(time * 1.5f, 1.0f);
        Vector2 gleamPos = { hilt.x + (tip.x - hilt.x) * shimmerT, hilt.y + (tip.y - hilt.y) * shimmerT };
        DrawCircleV(gleamPos, 3.0f * scale, Color{ 255, 255, 255, 200 });
    }
    else if (weapon == WeaponType::DAGGERS) {
        Vector2 hiltR = { handR.x + 2.0f * scale, handR.y };
        Vector2 tipR = { hiltR.x + 10.0f * scale, hiltR.y + 32.0f * scale };
        DrawLineEx(hiltR, tipR, 4.0f * scale, Color{ 60, 240, 180, 120 });
        DrawLineEx(hiltR, tipR, 2.5f * scale, Color{ 200, 255, 230, 255 });
        DrawCircleV(hiltR, 3.0f * scale, Color{ 220, 200, 120, 255 });

        Vector2 hiltL = { p.x - 21.0f * scale, p.y + 28.0f * scale };
        Vector2 tipL = { hiltL.x - 10.0f * scale, hiltL.y + 30.0f * scale };
        DrawLineEx(hiltL, tipL, 4.0f * scale, Color{ 60, 240, 180, 120 });
        DrawLineEx(hiltL, tipL, 2.5f * scale, Color{ 200, 255, 230, 255 });
        DrawCircleV(hiltL, 3.0f * scale, Color{ 220, 200, 120, 255 });
    }
    else if (weapon == WeaponType::MACE) {
        Vector2 hilt = { handR.x + 2.0f * scale, handR.y + 6.0f * scale };
        Vector2 head = { hilt.x + 6.0f * scale, hilt.y - 54.0f * scale };
        DrawLineEx(hilt, head, 4.5f * scale, Color{ 80, 60, 45, 255 });
        DrawLineEx(hilt, head, 2.5f * scale, Color{ 140, 110, 80, 255 });
        DrawCircleV(head, 13.0f * scale, Color{ 255, 140, 40, 160 });
        DrawCircleV(head, 9.5f * scale, Color{ 220, 180, 120, 255 });
        DrawCircleLines((int)head.x, (int)head.y, 9.5f * scale, Color{ 40, 30, 35, 255 });
        DrawLineEx({ head.x - 12.0f * scale, head.y }, { head.x + 12.0f * scale, head.y }, 3.5f * scale, Color{ 240, 220, 190, 255 });
        DrawLineEx({ head.x, head.y - 12.0f * scale }, { head.x, head.y + 12.0f * scale }, 3.5f * scale, Color{ 240, 220, 190, 255 });
    }
    else if (weapon == WeaponType::TRUTH_SEEKER) {
        Vector2 hilt = { handR.x + 3.0f * scale, handR.y - 2.0f * scale };
        Vector2 tip = { hilt.x + 8.0f * scale, hilt.y - 70.0f * scale };
        DrawLineEx(hilt, tip, 8.0f * scale, Color{ 255, 215, 60, 100 });
        DrawLineEx(hilt, tip, 4.0f * scale, Color{ 255, 245, 200, 255 });
        DrawLineEx({ hilt.x - 14.0f * scale, hilt.y }, { hilt.x + 14.0f * scale, hilt.y }, 4.5f * scale, Color{ 255, 215, 80, 255 });
        DrawCircleV(hilt, 4.0f * scale, Color{ 255, 240, 140, 255 });
        for (int i = 1; i <= 3; i++) {
            float frac = (float)i * 0.25f;
            Vector2 rune = { hilt.x + (tip.x - hilt.x) * frac, hilt.y + (tip.y - hilt.y) * frac };
            DrawCircleV(rune, 2.5f * scale, Color{ 255, 255, 255, 230 });
        }
    }
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
    } else if (weapon == WeaponType::TRUTH_SEEKER) {
        DrawSword({ weaponAnchor.x - 16.0f, weaponAnchor.y + 16.0f }, -45.0f, 44.0f, Color{ 255, 235, 140, 255 }, Color{ 255, 200, 50, 240 });
        const char* name = "TRUTH SEEKER";
        int w = MeasureText(name, 11);
        DrawText(name, (int)(position.x - w * 0.5f), (int)(position.y + 40), 11, Color{ 255, 215, 70, 255 });
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
