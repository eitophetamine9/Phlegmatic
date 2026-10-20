#include "Map.hpp"
#include "Config.hpp"
#include "raymath.h"
#include <cmath>

GameMap::GameMap() {
    bounds = { 100.0f, 100.0f, Config::MAP_WIDTH - 200.0f, Config::MAP_HEIGHT - 200.0f };
    Init();
}

void GameMap::Init() {
    pillars.clear();
    // Symmetric grand hall pillars
    pillars.push_back({ { 700.0f, 600.0f }, 45.0f });
    pillars.push_back({ { 700.0f, 1200.0f }, 45.0f });
    pillars.push_back({ { 1700.0f, 600.0f }, 45.0f });
    pillars.push_back({ { 1700.0f, 1200.0f }, 45.0f });

    // Center decorative markers
    pillars.push_back({ { 1200.0f, 400.0f }, 35.0f });
    pillars.push_back({ { 1200.0f, 1400.0f }, 35.0f });

    torches.clear();
    torches.push_back({ 700.0f, 600.0f });
    torches.push_back({ 700.0f, 1200.0f });
    torches.push_back({ 1700.0f, 600.0f });
    torches.push_back({ 1700.0f, 1200.0f });
    torches.push_back({ 1200.0f, 400.0f });
    torches.push_back({ 1200.0f, 1400.0f });
}

void GameMap::DrawFloor() const {
    // Background deep void
    ClearBackground(Color{ 14, 16, 22, 255 });

    // Arena Floor
    DrawRectangleRec(bounds, Color{ 22, 26, 36, 255 });

    // Tile grid lines & patterns
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

    // Grand Central Arena Combat Ring
    Vector2 center = { bounds.x + bounds.width * 0.5f, bounds.y + bounds.height * 0.5f };
    DrawCircleLines((int)center.x, (int)center.y, 450.0f, Color{ 45, 55, 78, 140 });
    DrawCircleLines((int)center.x, (int)center.y, 200.0f, Color{ 60, 75, 105, 160 });
    DrawCircleLines((int)center.x, (int)center.y, 50.0f, Color{ 80, 100, 140, 180 });

    // Torch light glows (ambient warm lighting)
    float time = (float)GetTime();
    for (const auto& torch : torches) {
        float flicker = 1.0f + 0.08f * sinf(time * 8.0f + torch.x);
        DrawCircleGradient(torch, 160.0f * flicker, Color{ 255, 160, 50, 45 }, Color{ 0, 0, 0, 0 });
    }
}

void GameMap::DrawWallsAndPillars() const {
    // Outer Border Walls (3D beveled appearance)
    const float wallThickness = 28.0f;

    // Top wall
    DrawRectangle((int)(bounds.x - wallThickness), (int)(bounds.y - wallThickness), (int)(bounds.width + wallThickness * 2), (int)wallThickness, Color{ 36, 42, 58, 255 });
    // Bottom wall
    DrawRectangle((int)(bounds.x - wallThickness), (int)(bounds.y + bounds.height), (int)(bounds.width + wallThickness * 2), (int)wallThickness, Color{ 36, 42, 58, 255 });
    // Left wall
    DrawRectangle((int)(bounds.x - wallThickness), (int)bounds.y, (int)wallThickness, (int)bounds.height, Color{ 36, 42, 58, 255 });
    // Right wall
    DrawRectangle((int)(bounds.x + bounds.width), (int)bounds.y, (int)wallThickness, (int)bounds.height, Color{ 36, 42, 58, 255 });

    // Wall Highlights
    DrawRectangleLinesEx(Rectangle{ bounds.x - 2, bounds.y - 2, bounds.width + 4, bounds.height + 4 }, 2.0f, Color{ 70, 85, 115, 255 });

    // Pillars
    for (const auto& pillar : pillars) {
        // Shadow
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y + pillar.radius * 0.4f), pillar.radius * 1.15f, Color{ 10, 12, 16, 120 });
        // Base ring
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius, Color{ 32, 38, 52, 255 });
        // Inner stone cap
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius - 6.0f, Color{ 48, 58, 78, 255 });
        // Top highlight
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 5.0f), pillar.radius * 0.45f, Color{ 68, 80, 108, 255 });
        // Torch braziers atop pillars
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 6.0f), 7.0f, Color{ 255, 150, 40, 240 });
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 6.0f), 3.0f, Color{ 255, 240, 120, 255 });
    }
}

bool GameMap::CheckCircleCollision(Vector2 pos, float radius) const {
    // Check boundary walls
    if (pos.x - radius < bounds.x || pos.x + radius > bounds.x + bounds.width ||
        pos.y - radius < bounds.y || pos.y + radius > bounds.y + bounds.height) {
        return true;
    }

    // Check pillars
    for (const auto& pillar : pillars) {
        float distSq = (pos.x - pillar.position.x) * (pos.x - pillar.position.x) +
                       (pos.y - pillar.position.y) * (pos.y - pillar.position.y);
        float minSafeDist = radius + pillar.radius;
        if (distSq < minSafeDist * minSafeDist) {
            return true;
        }
    }

    return false;
}

Vector2 GameMap::ResolveCollision(Vector2 oldPos, Vector2 newPos, float radius) const {
    // Try X movement first
    Vector2 testX = { newPos.x, oldPos.y };
    if (CheckCircleCollision(testX, radius)) {
        testX.x = oldPos.x;
    }

    // Try Y movement
    Vector2 testY = { testX.x, newPos.y };
    if (CheckCircleCollision(testY, radius)) {
        testY.y = oldPos.y;
    }

    return testY;
}
