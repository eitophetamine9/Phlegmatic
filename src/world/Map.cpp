#include "Map.hpp"
#include "../core/Config.hpp"
#include "raymath.h"
#include <cmath>

GameMap::GameMap() : currentEnv(MapEnvironment::LOBBY), gateOpen(false) {
    Init(MapEnvironment::LOBBY);
}

void GameMap::Init(MapEnvironment env) {
    currentEnv = env;
    pillars.clear();
    torches.clear();
    obstacles.clear();
    gateOpen = false;

    if (env == MapEnvironment::LOBBY) {
        // Compact cozy sanctuary
        bounds = { 460.0f, 320.0f, 680.0f, 620.0f };

        // Corner decorative pillars
        pillars.push_back({ { 530.0f, 390.0f }, 30.0f });
        pillars.push_back({ { 1070.0f, 390.0f }, 30.0f });
        pillars.push_back({ { 530.0f, 870.0f }, 30.0f });
        pillars.push_back({ { 1070.0f, 870.0f }, 30.0f });

        // Wall sconce torches
        torches.push_back({ 530.0f, 390.0f });
        torches.push_back({ 1070.0f, 390.0f });
        torches.push_back({ 530.0f, 870.0f });
        torches.push_back({ 1070.0f, 870.0f });
        torches.push_back({ 800.0f, 340.0f });
    } else if (env == MapEnvironment::ARENA) {
        // Grand Combat Arena
        bounds = { 100.0f, 100.0f, Config::MAP_WIDTH - 200.0f, Config::MAP_HEIGHT - 200.0f };

        pillars.push_back({ { 700.0f, 600.0f }, 45.0f });
        pillars.push_back({ { 700.0f, 1200.0f }, 45.0f });
        pillars.push_back({ { 1700.0f, 600.0f }, 45.0f });
        pillars.push_back({ { 1700.0f, 1200.0f }, 45.0f });
        pillars.push_back({ { 1200.0f, 400.0f }, 35.0f });
        pillars.push_back({ { 1200.0f, 1400.0f }, 35.0f });

        torches.push_back({ 700.0f, 600.0f });
        torches.push_back({ 700.0f, 1200.0f });
        torches.push_back({ 1700.0f, 600.0f });
        torches.push_back({ 1700.0f, 1200.0f });
        torches.push_back({ 1200.0f, 400.0f });
        torches.push_back({ 1200.0f, 1400.0f });
    } else if (env == MapEnvironment::RUINED_CASTLE) {
        // Ruined Castle of Zyphden
        bounds = { 200.0f, 150.0f, 1200.0f, 1150.0f };

        // Sanctuary Upper Pillars
        pillars.push_back({ { 540.0f, 320.0f }, 34.0f });
        pillars.push_back({ { 1060.0f, 320.0f }, 34.0f });
        pillars.push_back({ { 680.0f, 250.0f }, 28.0f });
        pillars.push_back({ { 920.0f, 250.0f }, 28.0f });

        // Middle Courtyard Broken Pillars
        pillars.push_back({ { 480.0f, 640.0f }, 36.0f });
        pillars.push_back({ { 480.0f, 880.0f }, 36.0f });
        pillars.push_back({ { 1020.0f, 640.0f }, 36.0f });

        // Wall obstacles (ruined stone walls & statue bases)
        obstacles.push_back({ { 320.0f, 540.0f, 180.0f, 24.0f } });
        obstacles.push_back({ { 1060.0f, 540.0f, 180.0f, 24.0f } });
        obstacles.push_back({ { 500.0f, 740.0f, 44.0f, 44.0f } });
        obstacles.push_back({ { 1060.0f, 740.0f, 44.0f, 44.0f } });

        // Torches & Braziers
        torches.push_back({ 540.0f, 320.0f });
        torches.push_back({ 1060.0f, 320.0f });
        torches.push_back({ 720.0f, 170.0f });
        torches.push_back({ 880.0f, 170.0f });
        torches.push_back({ 480.0f, 640.0f });
        torches.push_back({ 1020.0f, 640.0f });
        torches.push_back({ 1100.0f, 840.0f });
    }
}

void GameMap::DrawFloor() const {
    ClearBackground(Color{ 8, 10, 15, 255 });

    if (currentEnv == MapEnvironment::LOBBY) {
        // Warm sanctuary floor
        DrawRectangleRec(bounds, Color{ 24, 28, 40, 255 });

        const int tileSize = 48;
        int startX = (int)bounds.x;
        int endX = (int)(bounds.x + bounds.width);
        int startY = (int)bounds.y;
        int endY = (int)(bounds.y + bounds.height);

        for (int x = startX; x <= endX; x += tileSize) {
            DrawLine(x, startY, x, endY, Color{ 34, 40, 58, 140 });
        }
        for (int y = startY; y <= endY; y += tileSize) {
            DrawLine(startX, y, endX, y, Color{ 34, 40, 58, 140 });
        }

        // Center ceremonial seal
        Vector2 center = { bounds.x + bounds.width * 0.5f, bounds.y + bounds.height * 0.5f };
        DrawCircleLines((int)center.x, (int)center.y, 180.0f, Color{ 48, 62, 88, 140 });
        DrawCircleLines((int)center.x, (int)center.y, 80.0f, Color{ 65, 82, 115, 160 });
    } else if (currentEnv == MapEnvironment::ARENA) {
        // Arena Floor
        DrawRectangleRec(bounds, Color{ 22, 26, 36, 255 });

        const int tileSize = Config::TILE_SIZE;
        int startX = (int)bounds.x;
        int endX = (int)(bounds.x + bounds.width);
        int startY = (int)bounds.y;
        int endY = (int)(bounds.y + bounds.height);

        for (int x = startX; x <= endX; x += tileSize) {
            DrawLine(x, startY, x, endY, Color{ 28, 34, 48, 160 });
        }
        for (int y = startY; y <= endY; y += tileSize) {
            DrawLine(startX, y, endX, y, Color{ 28, 34, 48, 160 });
        }

        Vector2 center = { bounds.x + bounds.width * 0.5f, bounds.y + bounds.height * 0.5f };
        DrawCircleLines((int)center.x, (int)center.y, 450.0f, Color{ 45, 55, 78, 140 });
        DrawCircleLines((int)center.x, (int)center.y, 200.0f, Color{ 60, 75, 105, 160 });
        DrawCircleLines((int)center.x, (int)center.y, 50.0f, Color{ 80, 100, 140, 180 });
    } else if (currentEnv == MapEnvironment::RUINED_CASTLE) {
        DrawRuinedCastleFloor();
    }

    float time = (float)GetTime();
    for (const auto& torch : torches) {
        float flicker = 1.0f + 0.08f * sinf(time * 8.0f + torch.x);
        DrawCircleGradient(torch, 140.0f * flicker, Color{ 255, 160, 50, 45 }, Color{ 0, 0, 0, 0 });
    }
}

void GameMap::DrawRuinedCastleFloor() const {
    float time = (float)GetTime();

    // 1. Weathered Stone Flagstone base
    DrawRectangleRec(bounds, Color{ 22, 26, 34, 255 });

    const int tileSize = 52;
    int startX = (int)bounds.x;
    int endX = (int)(bounds.x + bounds.width);
    int startY = (int)bounds.y;
    int endY = (int)(bounds.y + bounds.height);

    for (int x = startX; x <= endX; x += tileSize) {
        DrawLine(x, startY, x, endY, Color{ 30, 36, 48, 120 });
    }
    for (int y = startY; y <= endY; y += tileSize) {
        DrawLine(startX, y, endX, y, Color{ 30, 36, 48, 120 });
    }

    // 2. Overgrown Moss Patches
    for (int i = 0; i < 28; i++) {
        float mx = bounds.x + fmodf((float)(i * 137), bounds.width - 80.0f) + 40.0f;
        float my = bounds.y + fmodf((float)(i * 191), bounds.height - 80.0f) + 40.0f;
        float mr = 18.0f + (float)(i % 5) * 8.0f;
        DrawCircle((int)mx, (int)my, mr, Color{ 28, 48, 36, 95 });
        DrawCircle((int)(mx + 6.0f), (int)(my - 4.0f), mr * 0.6f, Color{ 38, 65, 46, 80 });
    }

    // 3. Eastern Flowing River Stream
    float streamX = bounds.x + bounds.width - 150.0f;
    float streamW = 85.0f;
    DrawRectangle((int)streamX, (int)bounds.y, (int)streamW, (int)bounds.height, Color{ 16, 40, 58, 230 });
    DrawRectangle((int)(streamX + 10), (int)bounds.y, (int)(streamW - 20), (int)bounds.height, Color{ 22, 58, 82, 240 });

    // Water ripple reflections
    for (int r = 0; r < 14; r++) {
        float ry = bounds.y + fmodf(time * 30.0f + (float)(r * 85), bounds.height);
        float rw = 30.0f + sinf(time * 2.0f + (float)r) * 15.0f;
        DrawLineEx({ streamX + 20.0f, ry }, { streamX + 20.0f + rw, ry }, 2.0f, Color{ 80, 180, 230, 110 });
    }

    // Stone Bridge crossing the stream
    float bridgeY = 820.0f;
    DrawRectangle((int)streamX - 16, (int)bridgeY, (int)streamW + 32, 54, Color{ 38, 44, 58, 255 });
    DrawRectangleLines((int)streamX - 16, (int)bridgeY, (int)streamW + 32, 54, Color{ 70, 85, 110, 255 });
    DrawLine((int)streamX - 16, (int)bridgeY + 18, (int)(streamX + streamW + 16), (int)bridgeY + 18, Color{ 55, 65, 85, 255 });
    DrawLine((int)streamX - 16, (int)bridgeY + 36, (int)(streamX + streamW + 16), (int)bridgeY + 36, Color{ 55, 65, 85, 255 });

    // 4. Elevated Dais and Carved Steps
    // Three stone steps
    DrawRectangle(580, 430, 440, 14, Color{ 32, 38, 52, 255 });
    DrawRectangle(590, 416, 420, 14, Color{ 40, 48, 64, 255 });
    DrawRectangle(600, 402, 400, 14, Color{ 48, 58, 76, 255 });

    // Upper Altar Platform
    DrawRectangle(460, (int)bounds.y, 680, 400, Color{ 28, 34, 46, 255 });
    DrawRectangleLines(460, (int)bounds.y, 680, 400, Color{ 55, 68, 92, 255 });

    // Grand Altar Seal of Zyphden
    Vector2 altarCenter = { 800.0f, 290.0f };
    DrawCircleLines((int)altarCenter.x, (int)altarCenter.y, 110.0f, Color{ 60, 78, 105, 180 });
    DrawCircleLines((int)altarCenter.x, (int)altarCenter.y, 50.0f, Color{ 85, 110, 145, 200 });
    DrawCircleLines((int)altarCenter.x, (int)altarCenter.y, 18.0f, Color{ 200, 170, 70, 180 });

    // Lower Courtyard Awakening Circle
    Vector2 awakenCenter = { 800.0f, 1180.0f };
    DrawCircleLines((int)awakenCenter.x, (int)awakenCenter.y, 45.0f, Color{ 45, 60, 85, 160 });
}

void GameMap::DrawWallsAndPillars() const {
    const float wallThickness = 24.0f;

    // Outer Perimeter Walls
    DrawRectangle((int)(bounds.x - wallThickness), (int)(bounds.y - wallThickness), (int)(bounds.width + wallThickness * 2), (int)wallThickness, Color{ 34, 40, 56, 255 });
    DrawRectangle((int)(bounds.x - wallThickness), (int)(bounds.y + bounds.height), (int)(bounds.width + wallThickness * 2), (int)wallThickness, Color{ 34, 40, 56, 255 });
    DrawRectangle((int)(bounds.x - wallThickness), (int)bounds.y, (int)wallThickness, (int)bounds.height, Color{ 34, 40, 56, 255 });
    DrawRectangle((int)(bounds.x + bounds.width), (int)bounds.y, (int)wallThickness, (int)bounds.height, Color{ 34, 40, 56, 255 });
    DrawRectangleLinesEx(Rectangle{ bounds.x - 2, bounds.y - 2, bounds.width + 4, bounds.height + 4 }, 2.0f, Color{ 70, 85, 115, 255 });

    if (currentEnv == MapEnvironment::RUINED_CASTLE) {
        DrawRuinedCastleWalls();
    } else {
        // Standard Pillars
        for (const auto& pillar : pillars) {
            DrawCircle((int)pillar.position.x, (int)(pillar.position.y + pillar.radius * 0.4f), pillar.radius * 1.15f, Color{ 10, 12, 16, 120 });
            DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius, Color{ 32, 38, 52, 255 });
            DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius - 5.0f, Color{ 48, 58, 78, 255 });
            DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), pillar.radius * 0.45f, Color{ 68, 80, 108, 255 });
            DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 5.0f), 6.0f, Color{ 255, 150, 40, 240 });
            DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 5.0f), 3.0f, Color{ 255, 240, 120, 255 });
        }
    }
}

void GameMap::DrawRuinedCastleWalls() const {
    float time = (float)GetTime();

    // 1. Ruined Interior Wall Segments
    for (const auto& obs : obstacles) {
        DrawRectangleRec(obs.rect, Color{ 38, 44, 58, 255 });
        DrawRectangleLinesEx(obs.rect, 2.0f, Color{ 68, 82, 108, 255 });

        // Ivy vines hanging
        for (int v = 0; v < 3; v++) {
            float vx = obs.rect.x + 20.0f + (float)v * 45.0f;
            DrawLineEx({ vx, obs.rect.y }, { vx + 4.0f, obs.rect.y + obs.rect.height + 8.0f }, 2.5f, Color{ 40, 80, 50, 200 });
        }
    }

    // 2. Ancient Guardian Statues at (500, 740) and (1060, 740)
    Vector2 statue1 = { 522.0f, 762.0f };
    Vector2 statue2 = { 1082.0f, 762.0f };
    for (Vector2 st : { statue1, statue2 }) {
        // Pedestal base
        DrawRectangle((int)st.x - 20, (int)st.y - 20, 40, 40, Color{ 34, 40, 54, 255 });
        DrawRectangleLines((int)st.x - 20, (int)st.y - 20, 40, 40, Color{ 75, 90, 118, 255 });

        // Statue torso & knight helm
        DrawCircleV(st, 16.0f, Color{ 60, 72, 95, 255 });
        DrawCircleV({ st.x, st.y - 4.0f }, 10.0f, Color{ 85, 102, 132, 255 });
        DrawLineEx({ st.x - 8, st.y }, { st.x + 8, st.y }, 2.0f, Color{ 20, 25, 35, 255 });

        // Shield & Spear
        DrawLineEx({ st.x + 14, st.y - 24 }, { st.x + 14, st.y + 24 }, 2.5f, Color{ 140, 155, 180, 240 });
        DrawCircle((int)(st.x - 12), (int)st.y, 8.0f, Color{ 100, 118, 148, 255 });
    }

    // 3. Fluted Pillars & Broken Columns
    for (const auto& pillar : pillars) {
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y + pillar.radius * 0.35f), pillar.radius * 1.1f, Color{ 10, 12, 16, 130 });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius, Color{ 35, 42, 56, 255 });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius - 5.0f, Color{ 52, 62, 82, 255 });
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 3.0f), pillar.radius * 0.45f, Color{ 75, 90, 118, 255 });

        // Brazier flame
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), 7.0f, Color{ 255, 160, 40, 240 });
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), 3.5f, Color{ 255, 240, 140, 255 });

        // Moss on base
        DrawCircle((int)(pillar.position.x - 8), (int)(pillar.position.y + 8), 6.0f, Color{ 40, 75, 50, 180 });
    }

    // 4. Grand Northern Iron Portcullis
    float gateX = 710.0f;
    float gateY = bounds.y - 4.0f;
    float gateW = 180.0f;
    float gateH = 28.0f;

    DrawRectangle((int)gateX - 8, (int)gateY - 8, (int)gateW + 16, (int)gateH + 16, Color{ 26, 30, 42, 255 });
    DrawRectangleLines((int)gateX - 8, (int)gateY - 8, (int)gateW + 16, (int)gateH + 16, Color{ 70, 85, 115, 255 });

    if (!gateOpen) {
        // Iron bars across gate
        for (int b = 0; b < 10; b++) {
            float bx = gateX + 8.0f + (float)b * 18.0f;
            DrawLineEx({ bx, gateY }, { bx, gateY + gateH }, 3.5f, Color{ 90, 105, 130, 255 });
        }
        // Glowing seal lock
        DrawCircle((int)(gateX + gateW * 0.5f), (int)(gateY + gateH * 0.5f), 12.0f, Color{ 200, 60, 60, 220 });
        DrawCircleLines((int)(gateX + gateW * 0.5f), (int)(gateY + gateH * 0.5f), 14.0f, Color{ 255, 100, 80, 255 });
    } else {
        // Open gate with golden dawn glow & overland path to Zyphden
        float pulse = 0.5f + 0.5f * sinf(time * 3.0f);

        // Overland road extending north into Zyphden
        Rectangle roadRec = { gateX, bounds.y - 70.0f, gateW, 70.0f };
        DrawRectangleRec(roadRec, Color{ 30, 42, 58, 240 });
        DrawRectangleLinesEx(roadRec, 2.0f, Color{ 70, 95, 130, 240 });

        // Overgrown moss and stone pavers
        for (int p = 0; p < 4; p++) {
            float py = bounds.y - 15.0f - (float)p * 16.0f;
            DrawLineEx({ gateX + 10.0f, py }, { gateX + gateW - 10.0f, py }, 2.0f, Color{ 50, 70, 95, 200 });
        }

        // Portcullis threshold
        DrawRectangle((int)gateX, (int)gateY, (int)gateW, (int)gateH, Color{ 40, 60, 95, 220 });
        DrawCircleGradient(Vector2{ gateX + gateW * 0.5f, gateY + gateH * 0.5f }, 90.0f, Color{ 255, 215, 80, (unsigned char)(140 * pulse) }, Color{ 0, 0, 0, 0 });

        // Pathway archway pillars & lanterns
        DrawCircle((int)gateX - 8, (int)bounds.y - 40, 16.0f, Color{ 55, 68, 92, 255 });
        DrawCircle((int)(gateX + gateW + 8), (int)bounds.y - 40, 16.0f, Color{ 55, 68, 92, 255 });
        DrawCircle((int)gateX - 8, (int)bounds.y - 42, 5.0f, Color{ 255, 180, 40, 240 });
        DrawCircle((int)(gateX + gateW + 8), (int)bounds.y - 42, 5.0f, Color{ 255, 180, 40, 240 });

        DrawText("GATEWAY TO ZYPHDEN", (int)gateX + 18, (int)bounds.y - 52, 13, Color{ 255, 230, 120, 255 });
        DrawText("Step through or Press [E] to advance", (int)gateX + 2, (int)bounds.y - 32, 11, Color{ 160, 220, 255, 240 });
    }
}

bool GameMap::CheckCircleCollision(Vector2 pos, float radius) const {
    // Ruined Castle Northern Gateway Corridor check (allows stepping out into Zyphden)
    bool inGateCorridor = (currentEnv == MapEnvironment::RUINED_CASTLE && gateOpen &&
                           pos.x >= 700.0f && pos.x <= 900.0f &&
                           pos.y >= bounds.y - 120.0f);

    if (!inGateCorridor && (pos.y - radius < bounds.y)) {
        return true;
    }

    // Corridor side walls and northern dead-end barrier when north of bounds.y
    if (currentEnv == MapEnvironment::RUINED_CASTLE && gateOpen && pos.y < bounds.y) {
        if (pos.x - radius < 705.0f || pos.x + radius > 895.0f || pos.y - radius < bounds.y - 80.0f) {
            return true;
        }
    }

    // Outer boundaries
    if (pos.x - radius < bounds.x || pos.x + radius > bounds.x + bounds.width ||
        pos.y + radius > bounds.y + bounds.height) {
        return true;
    }

    // Pillars
    for (const auto& pillar : pillars) {
        float distSq = (pos.x - pillar.position.x) * (pos.x - pillar.position.x) +
                       (pos.y - pillar.position.y) * (pos.y - pillar.position.y);
        float minSafeDist = radius + pillar.radius;
        if (distSq < minSafeDist * minSafeDist) {
            return true;
        }
    }

    // Wall Obstacles
    for (const auto& obs : obstacles) {
        if (CheckCollisionCircleRec(pos, radius, obs.rect)) {
            return true;
        }
    }

    // Northern Gate barrier if closed
    if (currentEnv == MapEnvironment::RUINED_CASTLE && !gateOpen) {
        Rectangle gateRec = { 710.0f, bounds.y - 10.0f, 180.0f, 36.0f };
        if (CheckCollisionCircleRec(pos, radius, gateRec)) {
            return true;
        }
    }

    return false;
}

Vector2 GameMap::ResolveCollision(Vector2 oldPos, Vector2 newPos, float radius) const {
    Vector2 testX = { newPos.x, oldPos.y };
    if (CheckCircleCollision(testX, radius)) {
        testX.x = oldPos.x;
    }

    Vector2 testY = { testX.x, newPos.y };
    if (CheckCircleCollision(testY, radius)) {
        testY.y = oldPos.y;
    }

    return testY;
}
