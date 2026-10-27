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
    } else if (env == MapEnvironment::CROSSROADS) {
        bounds = { 400.0f, 150.0f, 800.0f, 1600.0f };

        // Roadside pillars
        pillars.push_back({ { 530.0f, 400.0f }, 32.0f });
        pillars.push_back({ { 1070.0f, 400.0f }, 32.0f });
        pillars.push_back({ { 530.0f, 750.0f }, 32.0f });
        pillars.push_back({ { 1070.0f, 750.0f }, 32.0f });
        pillars.push_back({ { 530.0f, 1150.0f }, 32.0f });
        pillars.push_back({ { 1070.0f, 1150.0f }, 32.0f });
        pillars.push_back({ { 530.0f, 1500.0f }, 32.0f });
        pillars.push_back({ { 1070.0f, 1500.0f }, 32.0f });

        // Broken roadside masonry
        obstacles.push_back({ { 440.0f, 600.0f, 100.0f, 26.0f } });
        obstacles.push_back({ { 1060.0f, 600.0f, 100.0f, 26.0f } });
        obstacles.push_back({ { 440.0f, 1250.0f, 100.0f, 26.0f } });
        obstacles.push_back({ { 1060.0f, 1250.0f, 100.0f, 26.0f } });

        // Torches along roadside
        torches.push_back({ 530.0f, 400.0f });
        torches.push_back({ 1070.0f, 400.0f });
        torches.push_back({ 530.0f, 1150.0f });
        torches.push_back({ 1070.0f, 1150.0f });
        torches.push_back({ 800.0f, 200.0f });
    } else if (env == MapEnvironment::ZYPHDEN_REALM) {
        bounds = { 200.0f, 100.0f, 1600.0f, 1400.0f };

        // Town Square & Castle Pillars
        pillars.push_back({ { 750.0f, 400.0f }, 34.0f });
        pillars.push_back({ { 1250.0f, 400.0f }, 34.0f });
        pillars.push_back({ { 850.0f, 220.0f }, 30.0f });
        pillars.push_back({ { 1150.0f, 220.0f }, 30.0f });

        // Town Square Street Lantern Pillars
        pillars.push_back({ { 700.0f, 750.0f }, 24.0f });
        pillars.push_back({ { 1100.0f, 750.0f }, 24.0f });
        pillars.push_back({ { 700.0f, 1150.0f }, 24.0f });
        pillars.push_back({ { 1100.0f, 1150.0f }, 24.0f });

        // Courtyard Sparring Arena Pillars
        pillars.push_back({ { 1350.0f, 550.0f }, 26.0f });
        pillars.push_back({ { 1650.0f, 550.0f }, 26.0f });
        pillars.push_back({ { 1350.0f, 850.0f }, 26.0f });
        pillars.push_back({ { 1650.0f, 850.0f }, 26.0f });

        // Building Wall Obstacles
        // Eldon's Hearthside Tavern (Southwest) - Perimeter walls with open East & South doorways
        obstacles.push_back({ { 320.0f, 800.0f, 280.0f, 24.0f } }); // North wall
        obstacles.push_back({ { 320.0f, 800.0f, 24.0f, 220.0f } }); // West wall
        obstacles.push_back({ { 580.0f, 800.0f, 20.0f, 50.0f } });  // East wall top (y: 800-850)
        obstacles.push_back({ { 580.0f, 920.0f, 20.0f, 100.0f } }); // East wall bottom (y: 920-1020)
        obstacles.push_back({ { 320.0f, 996.0f, 75.0f, 24.0f } });  // South wall west (x: 320-395)
        obstacles.push_back({ { 485.0f, 996.0f, 115.0f, 24.0f } }); // South wall east (x: 485-600)
        // Carpenter's Masonry Workshop (Central West)
        obstacles.push_back({ { 320.0f, 480.0f, 220.0f, 180.0f } });
        // Royal Throne Keep (North Castle)
        obstacles.push_back({ { 800.0f, 120.0f, 400.0f, 200.0f } });
        // Courtyard Low Enclosure Fence/Walls
        obstacles.push_back({ { 1300.0f, 480.0f, 380.0f, 20.0f } }); // North courtyard rail
        obstacles.push_back({ { 1300.0f, 920.0f, 380.0f, 20.0f } }); // South courtyard rail
        obstacles.push_back({ { 1680.0f, 500.0f, 20.0f, 420.0f } }); // East courtyard rail

        // Torches & Braziers
        torches.push_back({ 750.0f, 400.0f });
        torches.push_back({ 1250.0f, 400.0f });
        torches.push_back({ 850.0f, 220.0f });
        torches.push_back({ 1150.0f, 220.0f });
        torches.push_back({ 1000.0f, 160.0f }); // Royal throne brazier
        torches.push_back({ 1500.0f, 550.0f }); // Courtyard brazier
        torches.push_back({ 1500.0f, 850.0f });
        torches.push_back({ 620.0f, 860.0f });  // Tavern entrance lantern
    } else if (env == MapEnvironment::TAVERN_INTERIOR) {
        // Hearthside Tavern Interior - Cozy rustic tavern
        bounds = { 600.0f, 600.0f, 400.0f, 320.0f };

        // Bar Counter Obstacle lowered to y = 700 so Eldon behind at y = 665 has clear name tag visibility
        obstacles.push_back({ { 650.0f, 700.0f, 150.0f, 32.0f } });
        // Stone Hearth Fireplace
        obstacles.push_back({ { 830.0f, 600.0f, 80.0f, 30.0f } });
        // Dining Table 1
        obstacles.push_back({ { 860.0f, 740.0f, 70.0f, 42.0f } });
        // Dining Table 2
        obstacles.push_back({ { 860.0f, 830.0f, 70.0f, 42.0f } });

        // Lanterns & Hearth glow
        torches.push_back({ 870.0f, 620.0f }); // Fireplace flame
        torches.push_back({ 680.0f, 760.0f }); // Wall lantern
        torches.push_back({ 920.0f, 760.0f }); // Wall lantern
    } else if (env == MapEnvironment::FORGE_INTERIOR) {
        // Blacksmith Varek's Forge Workshop - Atmospheric blacksmith shop
        bounds = { 500.0f, 400.0f, 600.0f, 520.0f };

        // 1. Blacksmith Blast Furnace & Brick Hood at {740, 400, 120, 65}
        obstacles.push_back({ { 740.0f, 400.0f, 120.0f, 65.0f } });
        // 2. Heavy Master Anvil at {784, 516, 48, 28}
        obstacles.push_back({ { 784.0f, 516.0f, 48.0f, 28.0f } });
        // 3. Quenching Water Trough at {580, 480, 32, 64}
        obstacles.push_back({ { 580.0f, 480.0f, 32.0f, 64.0f } });
        // 4. Armory Workbenches & Coal Bin
        obstacles.push_back({ { 960.0f, 460.0f, 90.0f, 36.0f } });
        obstacles.push_back({ { 960.0f, 620.0f, 90.0f, 36.0f } });

        // Roaring Furnace embers and lanterns
        torches.push_back({ 800.0f, 430.0f }); // Furnace fire
        torches.push_back({ 620.0f, 560.0f }); // Wall lantern
        torches.push_back({ 980.0f, 560.0f }); // Wall lantern
    } else if (env == MapEnvironment::ROYAL_KEEP) {
        // Zyphden Royal Keep - Grand Sovereign Throne Hall (Expanded 1200x1100)
        bounds = { 400.0f, 160.0f, 1200.0f, 1100.0f };

        // Throne Chair Obstacle (King Kreptu's elevated seat)
        obstacles.push_back({ { 950.0f, 160.0f, 100.0f, 65.0f } });

        // Western Wing (Archives) Library Bookcases & Desks
        obstacles.push_back({ { 480.0f, 320.0f, 200.0f, 28.0f } });
        obstacles.push_back({ { 480.0f, 440.0f, 200.0f, 28.0f } });
        obstacles.push_back({ { 480.0f, 580.0f, 120.0f, 40.0f } }); // Reading desk

        // Eastern Wing (Armory) Weapon Racks & Benches
        obstacles.push_back({ { 1320.0f, 320.0f, 200.0f, 28.0f } });
        obstacles.push_back({ { 1320.0f, 440.0f, 200.0f, 28.0f } });
        obstacles.push_back({ { 1400.0f, 580.0f, 120.0f, 40.0f } }); // Armorer bench

        // 8 Grand Colonnade Pillars flanking the royal runner
        // Western Row (x = 800)
        pillars.push_back({ { 800.0f, 460.0f }, 28.0f });
        pillars.push_back({ { 800.0f, 660.0f }, 28.0f });
        pillars.push_back({ { 800.0f, 860.0f }, 28.0f });
        pillars.push_back({ { 800.0f, 1060.0f }, 28.0f });

        // Eastern Row (x = 1200)
        pillars.push_back({ { 1200.0f, 460.0f }, 28.0f });
        pillars.push_back({ { 1200.0f, 660.0f }, 28.0f });
        pillars.push_back({ { 1200.0f, 860.0f }, 28.0f });
        pillars.push_back({ { 1200.0f, 1060.0f }, 28.0f });

        // Royal Braziers & Torches
        torches.push_back({ 890.0f, 230.0f });  // Dais Left
        torches.push_back({ 1110.0f, 230.0f }); // Dais Right
        torches.push_back({ 800.0f, 460.0f });
        torches.push_back({ 1200.0f, 460.0f });
        torches.push_back({ 800.0f, 660.0f });
        torches.push_back({ 1200.0f, 660.0f });
        torches.push_back({ 800.0f, 860.0f });
        torches.push_back({ 1200.0f, 860.0f });
        torches.push_back({ 800.0f, 1060.0f });
        torches.push_back({ 1200.0f, 1060.0f });

        // Wing lanterns
        torches.push_back({ 460.0f, 200.0f });
        torches.push_back({ 1540.0f, 200.0f });
    }
}

void GameMap::DrawFloor(Vector2 playerPos) const {
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
    } else if (currentEnv == MapEnvironment::CROSSROADS) {
        DrawCrossroadsFloor(playerPos);
    } else if (currentEnv == MapEnvironment::ZYPHDEN_REALM) {
        DrawZyphdenFloor();
    } else if (currentEnv == MapEnvironment::TAVERN_INTERIOR) {
        DrawTavernFloor();
    } else if (currentEnv == MapEnvironment::FORGE_INTERIOR) {
        DrawForgeFloor();
    } else if (currentEnv == MapEnvironment::ROYAL_KEEP) {
        DrawRoyalKeepFloor();
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

void GameMap::DrawWallsAndPillars(Vector2 playerPos) const {
    const float wallThickness = 24.0f;

    // Outer Perimeter Walls
    DrawRectangle((int)(bounds.x - wallThickness), (int)(bounds.y - wallThickness), (int)(bounds.width + wallThickness * 2), (int)wallThickness, Color{ 34, 40, 56, 255 });
    DrawRectangle((int)(bounds.x - wallThickness), (int)(bounds.y + bounds.height), (int)(bounds.width + wallThickness * 2), (int)wallThickness, Color{ 34, 40, 56, 255 });
    DrawRectangle((int)(bounds.x - wallThickness), (int)bounds.y, (int)wallThickness, (int)bounds.height, Color{ 34, 40, 56, 255 });
    DrawRectangle((int)(bounds.x + bounds.width), (int)bounds.y, (int)wallThickness, (int)bounds.height, Color{ 34, 40, 56, 255 });
    DrawRectangleLinesEx(Rectangle{ bounds.x - 2, bounds.y - 2, bounds.width + 4, bounds.height + 4 }, 2.0f, Color{ 70, 85, 115, 255 });

    if (currentEnv == MapEnvironment::RUINED_CASTLE) {
        DrawRuinedCastleWalls(playerPos);
    } else if (currentEnv == MapEnvironment::CROSSROADS) {
        DrawCrossroadsWalls(playerPos);
    } else if (currentEnv == MapEnvironment::ZYPHDEN_REALM) {
        DrawZyphdenWalls();
    } else if (currentEnv == MapEnvironment::TAVERN_INTERIOR) {
        DrawTavernWalls(playerPos);
    } else if (currentEnv == MapEnvironment::FORGE_INTERIOR) {
        DrawForgeWalls();
    } else if (currentEnv == MapEnvironment::ROYAL_KEEP) {
        DrawRoyalKeepWalls();
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

void GameMap::DrawRuinedCastleWalls(Vector2 playerPos) const {
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

        bool inRange = (playerPos.x > -9000.0f) && (Vector2Distance(playerPos, { gateX + gateW * 0.5f, bounds.y }) <= 160.0f);
        if (inRange) {
            DrawText("GATEWAY TO ZYPHDEN", (int)gateX + 18, (int)bounds.y - 52, 13, Color{ 255, 230, 120, 255 });
            DrawText("Step through or Press [E] to advance", (int)gateX + 2, (int)bounds.y - 32, 11, Color{ 160, 220, 255, 240 });
        }
    }
}

bool GameMap::CheckCircleCollision(Vector2 pos, float radius) const {
    // Ruined Castle & Crossroads Northern Gateway Corridor check (allows stepping north through open gateway)
    bool inGateCorridor = ((currentEnv == MapEnvironment::RUINED_CASTLE || currentEnv == MapEnvironment::CROSSROADS) && gateOpen &&
                           pos.x >= 680.0f && pos.x <= 920.0f &&
                           pos.y >= bounds.y - 120.0f);

    if (!inGateCorridor && (pos.y - radius < bounds.y)) {
        return true;
    }

    // Corridor side walls and northern dead-end barrier when north of bounds.y
    if ((currentEnv == MapEnvironment::RUINED_CASTLE || currentEnv == MapEnvironment::CROSSROADS) && gateOpen && pos.y < bounds.y) {
        if (pos.x - radius < 685.0f || pos.x + radius > 915.0f || pos.y - radius < bounds.y - 80.0f) {
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

void GameMap::DrawCrossroadsFloor(Vector2 playerPos) const {
    float time = (float)GetTime();

    // 1. Weathered earthy terrain
    DrawRectangleRec(bounds, Color{ 18, 22, 28, 255 });

    // 2. Central Stone Paved Royal Highway (X: 620 to 980)
    Rectangle road = { 620.0f, bounds.y, 360.0f, bounds.height };
    DrawRectangleRec(road, Color{ 28, 34, 46, 255 });
    DrawRectangleLinesEx(road, 2.0f, Color{ 55, 68, 92, 200 });

    // Paving lines
    for (int y = (int)bounds.y; y <= (int)(bounds.y + bounds.height); y += 48) {
        DrawLine(620, y, 980, y, Color{ 40, 48, 64, 140 });
    }
    DrawLine(800, (int)bounds.y, 800, (int)(bounds.y + bounds.height), Color{ 45, 55, 75, 120 });

    // 3. Roadside moss & wild grass patches
    for (int i = 0; i < 22; i++) {
        float gx = bounds.x + fmodf((float)(i * 123), bounds.width - 60.0f) + 30.0f;
        float gy = bounds.y + fmodf((float)(i * 179), bounds.height - 60.0f) + 30.0f;
        if (gx > 620.0f && gx < 980.0f) continue;
        DrawCircle((int)gx, (int)gy, 22.0f, Color{ 24, 42, 32, 90 });
    }

    // 4. Western Ruined Shrine Alcove for Hidden Totem Chest
    Vector2 shrinePos = { 490.0f, 560.0f };
    DrawCircleLines((int)shrinePos.x, (int)shrinePos.y, 45.0f, Color{ 60, 80, 110, 180 });
    DrawCircleLines((int)shrinePos.x, (int)shrinePos.y, 25.0f, Color{ 80, 120, 160, 200 });
    DrawRectangle(450, 540, 80, 40, Color{ 36, 44, 60, 255 });
    DrawRectangleLines(450, 540, 80, 40, Color{ 75, 95, 130, 220 });

    // 5. Northern Gateway Approach to Zyphden
    DrawRectangle(620, (int)bounds.y, 360, 80, Color{ 36, 48, 68, 255 });
    DrawRectangleLines(620, (int)bounds.y, 360, 80, Color{ 180, 150, 70, 220 });
    bool inGateRange = (playerPos.x > -9000.0f) && (Vector2Distance(playerPos, { 800.0f, bounds.y + 40.0f }) <= 160.0f);
    if (inGateRange) {
        DrawText("GATES OF ZYPHDEN", 720, (int)bounds.y + 30, 15, Color{ 255, 215, 80, 240 });
    }
}

void GameMap::DrawCrossroadsWalls(Vector2 playerPos) const {
    float time = (float)GetTime();

    // Roadside Pillars & Braziers
    for (const auto& pillar : pillars) {
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y + pillar.radius * 0.4f), pillar.radius * 1.15f, Color{ 10, 12, 16, 120 });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius, Color{ 34, 42, 58, 255 });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius - 5.0f, Color{ 50, 62, 85, 255 });
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), pillar.radius * 0.45f, Color{ 72, 88, 118, 255 });

        // Flame
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), 7.0f, Color{ 255, 160, 40, 240 });
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), 3.5f, Color{ 255, 240, 140, 255 });
    }

    // Roadside broken masonry
    for (const auto& obs : obstacles) {
        DrawRectangleRec(obs.rect, Color{ 32, 40, 54, 255 });
        DrawRectangleLinesEx(obs.rect, 2.0f, Color{ 65, 80, 108, 240 });
    }

    // Northern Archway Gateway
    float gateX = 660.0f;
    float gateY = bounds.y - 4.0f;
    float gateW = 280.0f;
    float gateH = 32.0f;

    DrawRectangle((int)gateX - 8, (int)gateY - 8, (int)gateW + 16, (int)gateH + 16, Color{ 26, 32, 46, 255 });
    DrawRectangleLines((int)gateX - 8, (int)gateY - 8, (int)gateW + 16, (int)gateH + 16, Color{ 200, 160, 70, 255 });

    float pulse = 0.5f + 0.5f * sinf(time * 3.0f);
    DrawCircleGradient(Vector2{ 800.0f, gateY + 16.0f }, 110.0f, Color{ 255, 215, 80, (unsigned char)(140 * pulse) }, Color{ 0, 0, 0, 0 });
    bool inGateRange = (playerPos.x > -9000.0f) && (Vector2Distance(playerPos, { 800.0f, bounds.y }) <= 160.0f);
    if (inGateRange) {
        DrawText("ENTER ZYPHDEN KINGDOM", 705, (int)bounds.y - 38, 13, Color{ 255, 225, 100, 255 });
    }
}

void GameMap::DrawZyphdenFloor() const {
    float time = (float)GetTime();

    // 1. Base terrain (outer town dirt and gravel)
    DrawRectangleRec(bounds, Color{ 18, 22, 28, 255 });

    // 2. Central Town Square Cobblestone Paving
    Rectangle townSquare = { 650.0f, 450.0f, 550.0f, 900.0f };
    DrawRectangleRec(townSquare, Color{ 26, 32, 44, 255 });
    DrawRectangleLinesEx(townSquare, 2.0f, Color{ 48, 60, 82, 180 });

    const int tileSize = 44;
    for (int x = (int)townSquare.x; x <= (int)(townSquare.x + townSquare.width); x += tileSize) {
        DrawLine(x, (int)townSquare.y, x, (int)(townSquare.y + townSquare.height), Color{ 36, 44, 60, 120 });
    }
    for (int y = (int)townSquare.y; y <= (int)(townSquare.y + townSquare.height); y += tileSize) {
        DrawLine((int)townSquare.x, y, (int)(townSquare.x + townSquare.width), y, Color{ 36, 44, 60, 120 });
    }

    // Town Square Central Fountain
    Vector2 fountainPos = { 920.0f, 880.0f };
    DrawCircle((int)fountainPos.x, (int)fountainPos.y, 42.0f, Color{ 32, 42, 58, 255 });
    DrawCircleLines((int)fountainPos.x, (int)fountainPos.y, 42.0f, Color{ 70, 95, 130, 255 });
    DrawCircle((int)fountainPos.x, (int)fountainPos.y, 32.0f, Color{ 22, 54, 82, 240 });
    float waterPulse = sinf(time * 2.5f) * 3.0f;
    DrawCircle((int)fountainPos.x, (int)fountainPos.y, 14.0f + waterPulse, Color{ 60, 140, 200, 200 });

    // 3. Castle Keep Royal Hall (North)
    Rectangle castleHall = { 750.0f, 100.0f, 500.0f, 320.0f };
    DrawRectangleRec(castleHall, Color{ 28, 36, 52, 255 });
    DrawRectangleLinesEx(castleHall, 2.0f, Color{ 75, 95, 135, 220 });

    // Royal Crimson and Gold Carpet
    Rectangle royalCarpet = { 960.0f, 140.0f, 80.0f, 280.0f };
    DrawRectangleRec(royalCarpet, Color{ 110, 25, 35, 255 });
    DrawRectangleLinesEx(royalCarpet, 2.0f, Color{ 215, 175, 60, 240 });

    // 4. Eldon's Hearthside Tavern Floor (Southwest)
    Rectangle tavernFloor = { 320.0f, 800.0f, 280.0f, 220.0f };
    DrawRectangleRec(tavernFloor, Color{ 36, 26, 20, 255 });
    DrawRectangleLinesEx(tavernFloor, 2.0f, Color{ 75, 52, 38, 240 });
    for (int y = (int)tavernFloor.y; y <= (int)(tavernFloor.y + tavernFloor.height); y += 22) {
        DrawLine((int)tavernFloor.x, y, (int)(tavernFloor.x + tavernFloor.width), y, Color{ 48, 34, 26, 180 });
    }

    // 5. Carpenter's Workshop Floor (Central West)
    Rectangle workshopFloor = { 320.0f, 480.0f, 220.0f, 180.0f };
    DrawRectangleRec(workshopFloor, Color{ 30, 28, 26, 255 });
    DrawRectangleLinesEx(workshopFloor, 2.0f, Color{ 65, 58, 52, 220 });

    // 6. Courtyard Sparring Ring (East)
    Rectangle courtyardRec = { 1300.0f, 480.0f, 380.0f, 440.0f };
    DrawRectangleRec(courtyardRec, Color{ 44, 38, 28, 255 });
    DrawRectangleLinesEx(courtyardRec, 2.0f, Color{ 90, 78, 55, 220 });

    // Sparring Chalk Ring
    Vector2 sparRingCenter = { 1490.0f, 700.0f };
    DrawCircleLines((int)sparRingCenter.x, (int)sparRingCenter.y, 130.0f, Color{ 180, 160, 120, 190 });
    DrawCircleLines((int)sparRingCenter.x, (int)sparRingCenter.y, 65.0f, Color{ 150, 130, 95, 140 });
}

void GameMap::DrawZyphdenWalls() const {
    float time = (float)GetTime();

    // 1. Building Wall Obstacles
    // Hearthside Tavern (Southwest)
    Rectangle tavern = { 320.0f, 800.0f, 280.0f, 220.0f };
    DrawRectangleRec(tavern, Color{ 38, 28, 22, 255 });
    DrawRectangleLinesEx(tavern, 3.0f, Color{ 85, 60, 42, 255 });
    DrawRectangle(350, 820, 220, 40, Color{ 52, 38, 28, 255 });

    // Warm tavern glowing windows
    DrawRectangle(360, 890, 40, 30, Color{ 255, 170, 50, 200 });
    DrawRectangle(500, 890, 40, 30, Color{ 255, 170, 50, 200 });

    // Blacksmith Varek's Forge Workshop (Northwest)
    Rectangle workshop = { 320.0f, 480.0f, 220.0f, 180.0f };
    DrawRectangleRec(workshop, Color{ 36, 32, 28, 255 });
    DrawRectangleLinesEx(workshop, 3.0f, Color{ 75, 68, 58, 255 });
    // Anvil & Timber stacks
    DrawRectangle(350, 560, 40, 24, Color{ 55, 60, 68, 255 });
    DrawRectangle(440, 560, 60, 30, Color{ 70, 50, 35, 255 });
    // Forge doorway archway
    Rectangle forgeDoor = { 400.0f, 650.0f, 60.0f, 14.0f };
    DrawRectangleRec(forgeDoor, Color{ 20, 16, 14, 255 });
    DrawRectangleLinesEx(forgeDoor, 2.0f, Color{ 220, 130, 50, 240 });
    DrawCircleGradient(Vector2{ 430.0f, 656.0f }, 35.0f, Color{ 255, 110, 30, 80 }, Color{ 0, 0, 0, 0 });

    // Royal Castle Keep (North)
    Rectangle castleKeep = { 800.0f, 120.0f, 400.0f, 200.0f };
    DrawRectangleRec(castleKeep, Color{ 35, 42, 58, 255 });
    DrawRectangleLinesEx(castleKeep, 3.0f, Color{ 85, 105, 145, 255 });

    // Grand Entrance Archway into Royal Keep (y: 308-320)
    Rectangle keepDoor = { 950.0f, 306.0f, 100.0f, 16.0f };
    DrawRectangleRec(keepDoor, Color{ 18, 22, 34, 255 });
    DrawRectangleLinesEx(keepDoor, 2.0f, Color{ 235, 195, 65, 255 });
    DrawCircleGradient(Vector2{ 1000.0f, 314.0f }, 45.0f, Color{ 120, 190, 255, 95 }, Color{ 0, 0, 0, 0 });
    DrawCircle(942, 314, 5.0f, Color{ 255, 200, 75, 240 });
    DrawCircle(1058, 314, 5.0f, Color{ 255, 200, 75, 240 });

    // Courtyard Enclosure Rails
    for (const auto& obs : obstacles) {
        if (obs.rect.y >= 480.0f && obs.rect.x >= 1300.0f) {
            DrawRectangleRec(obs.rect, Color{ 65, 54, 40, 255 });
            DrawRectangleLinesEx(obs.rect, 2.0f, Color{ 110, 92, 68, 240 });
        }
    }

    // Courtyard Weapon Racks
    DrawRectangle(1330, 510, 60, 18, Color{ 50, 42, 32, 255 });
    DrawRectangleLines(1330, 510, 60, 18, Color{ 130, 110, 80, 240 });
    DrawLineEx({ 1340, 504 }, { 1340, 532 }, 2.0f, Color{ 180, 190, 205, 240 });
    DrawLineEx({ 1360, 504 }, { 1360, 532 }, 2.0f, Color{ 180, 190, 205, 240 });
    DrawLineEx({ 1380, 504 }, { 1380, 532 }, 2.0f, Color{ 180, 190, 205, 240 });

    // 2. Pillars & Braziers
    for (const auto& pillar : pillars) {
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y + pillar.radius * 0.4f), pillar.radius * 1.15f, Color{ 10, 12, 16, 120 });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius, Color{ 36, 44, 60, 255 });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius - 5.0f, Color{ 52, 64, 88, 255 });
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), pillar.radius * 0.45f, Color{ 75, 92, 122, 255 });

        // Flame
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), 7.0f, Color{ 255, 160, 40, 240 });
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), 3.5f, Color{ 255, 240, 140, 255 });
    }

    // 3. Royal Banners of Zyphden (Blue & Gold)
    for (float bx : { 820.0f, 1180.0f }) {
        Rectangle bannerRec = { bx - 14.0f, 320.0f, 28.0f, 70.0f };
        DrawRectangleRec(bannerRec, Color{ 25, 45, 85, 255 });
        DrawRectangleLinesEx(bannerRec, 2.0f, Color{ 225, 185, 60, 255 });
        DrawCircle((int)bx, 355, 6.0f, Color{ 255, 215, 80, 255 });
    }

    // Tavern East Entrance Doorway (Main Street Entrance at {590, 885})
    Rectangle eastDoor = { 584.0f, 855.0f, 16.0f, 60.0f };
    DrawRectangleRec(eastDoor, Color{ 22, 16, 12, 255 });
    DrawRectangleLinesEx(eastDoor, 2.0f, Color{ 225, 180, 75, 240 });
    DrawCircleGradient(Vector2{ 600.0f, 885.0f }, 45.0f, Color{ 255, 180, 60, 95 }, Color{ 0, 0, 0, 0 });
    DrawCircle(594, 846, 5.0f, Color{ 255, 195, 75, 240 });

    // Tavern South Entrance Doorway (Town Square Entrance at {440, 1010})
    Rectangle southDoor = { 398.0f, 996.0f, 84.0f, 16.0f };
    DrawRectangleRec(southDoor, Color{ 22, 16, 12, 255 });
    DrawRectangleLinesEx(southDoor, 2.0f, Color{ 225, 180, 75, 240 });
    DrawCircleGradient(Vector2{ 440.0f, 1010.0f }, 55.0f, Color{ 255, 180, 60, 95 }, Color{ 0, 0, 0, 0 });
    DrawCircle(396, 992, 5.0f, Color{ 255, 195, 75, 240 });
    DrawCircle(484, 992, 5.0f, Color{ 255, 195, 75, 240 });
}

void GameMap::DrawTavernFloor() const {
    // Warm dark amber wood floor planks
    DrawRectangleRec(bounds, Color{ 38, 26, 18, 255 });

    // Wood plank seams
    for (int y = (int)bounds.y; y <= (int)(bounds.y + bounds.height); y += 20) {
        DrawLine((int)bounds.x, y, (int)(bounds.x + bounds.width), y, Color{ 26, 17, 12, 190 });
    }
    for (int x = (int)bounds.x; x <= (int)(bounds.x + bounds.width); x += 60) {
        for (int y = (int)bounds.y; y <= (int)(bounds.y + bounds.height); y += 40) {
            DrawLine(x, y, x, y + 20, Color{ 26, 17, 12, 140 });
        }
    }

    // Cozy rug in front of hearth
    Rectangle rug = { 750.0f, 640.0f, 100.0f, 50.0f };
    DrawRectangleRec(rug, Color{ 95, 30, 35, 230 });
    DrawRectangleLinesEx(rug, 2.0f, Color{ 190, 150, 60, 240 });

    // Doorway mat at southern entrance (doorway at {800, 915})
    Rectangle mat = { 760.0f, 885.0f, 80.0f, 25.0f };
    DrawRectangleRec(mat, Color{ 55, 42, 30, 240 });
    DrawRectangleLines(760, 885, 80, 25, Color{ 110, 85, 55, 200 });
}

void GameMap::DrawTavernWalls(Vector2 playerPos) const {
    float time = (float)GetTime();

    // 1. Bar Counter & Shelves (Lowered to y = 700 so Eldon behind at y = 665 has clear name tag visibility)
    Rectangle bar = { 650.0f, 700.0f, 150.0f, 32.0f };
    DrawRectangleRec(bar, Color{ 70, 48, 30, 255 });
    DrawRectangleLinesEx(bar, 2.0f, Color{ 120, 85, 55, 255 });
    bool inBarRange = (playerPos.x > -9000.0f) && (Vector2Distance(playerPos, { 725.0f, 716.0f }) <= 150.0f);
    if (inBarRange) {
        DrawText("BAR", 705, 708, 12, Color{ 255, 215, 120, 240 });
    }

    // Bar stools in front of counter at y = 746
    for (float sx = 665.0f; sx <= 785.0f; sx += 35.0f) {
        DrawCircle((int)sx, 746, 10.0f, Color{ 52, 36, 24, 255 });
        DrawCircleLines((int)sx, 746, 10.0f, Color{ 110, 80, 50, 255 });
    }

    // Shelves behind Eldon with bottles/mugs
    DrawRectangle(650, 620, 150, 16, Color{ 48, 34, 22, 255 });
    DrawRectangleLines(650, 620, 150, 16, Color{ 90, 65, 42, 255 });
    for (int b = 0; b < 6; b++) {
        DrawRectangle(660 + b * 22, 614, 10, 14, (b % 2 == 0) ? Color{ 80, 160, 120, 220 } : Color{ 180, 110, 60, 220 });
    }

    // 2. Fireplace Hearth
    Rectangle hearth = { 830.0f, 600.0f, 80.0f, 30.0f };
    DrawRectangleRec(hearth, Color{ 45, 42, 44, 255 });
    DrawRectangleLinesEx(hearth, 2.0f, Color{ 95, 90, 92, 255 });
    // Crackling fire
    float flicker = sinf(time * 12.0f) * 3.0f;
    DrawCircle(870, 616, 12.0f + flicker, Color{ 255, 120, 30, 240 });
    DrawCircle(870, 616, 6.0f, Color{ 255, 230, 100, 255 });

    // 3. Dining Tables & Chairs
    for (float ty : { 740.0f, 830.0f }) {
        Rectangle table = { 860.0f, ty, 70.0f, 42.0f };
        DrawRectangleRec(table, Color{ 62, 44, 28, 255 });
        DrawRectangleLinesEx(table, 2.0f, Color{ 110, 80, 52, 255 });
        // Stools around table
        DrawCircle(846, (int)ty + 21, 8.0f, Color{ 48, 34, 22, 255 });
        DrawCircle(944, (int)ty + 21, 8.0f, Color{ 48, 34, 22, 255 });
    }

    // 4. Southern Exit Doorway
    DrawRectangle(770, 915, 60, 6, Color{ 20, 15, 12, 255 });
    DrawRectangleLines(770, 915, 60, 6, Color{ 180, 140, 80, 255 });
}

void GameMap::DrawForgeFloor() const {
    // 1. Dark scorched flagstone masonry
    DrawRectangleRec(bounds, Color{ 22, 20, 24, 255 });

    const int tileSize = 48;
    int startX = (int)bounds.x;
    int endX = (int)(bounds.x + bounds.width);
    int startY = (int)bounds.y;
    int endY = (int)(bounds.y + bounds.height);

    for (int x = startX; x <= endX; x += tileSize) {
        DrawLine(x, startY, x, endY, Color{ 36, 32, 38, 160 });
    }
    for (int y = startY; y <= endY; y += tileSize) {
        DrawLine(startX, y, endX, y, Color{ 36, 32, 38, 160 });
    }

    // Scorched coal ash circle around the central blast furnace and anvil
    DrawCircleGradient(Vector2{ 800.0f, 500.0f }, 140.0f, Color{ 14, 12, 16, 230 }, Color{ 0, 0, 0, 0 });
    DrawCircleGradient(Vector2{ 800.0f, 440.0f }, 90.0f, Color{ 255, 90, 20, 45 }, Color{ 0, 0, 0, 0 });
}

void GameMap::DrawForgeWalls() const {
    float time = (float)GetTime();

    // 1. Northern Blacksmith Blast Furnace & Brick Hood
    Rectangle furnace = { 740.0f, 400.0f, 120.0f, 65.0f };
    DrawRectangleRec(furnace, Color{ 48, 36, 32, 255 });
    DrawRectangleLinesEx(furnace, 3.0f, Color{ 85, 60, 50, 255 });

    // Glowing Furnace Arch & Coal Bed
    Rectangle grate = { 765.0f, 420.0f, 70.0f, 40.0f };
    DrawRectangleRec(grate, Color{ 28, 16, 12, 255 });
    DrawRectangleLinesEx(grate, 2.0f, Color{ 120, 75, 45, 255 });
    // Roaring glowing fire
    float flicker = sinf(time * 14.0f) * 4.0f;
    DrawCircleGradient(Vector2{ 800.0f, 436.0f }, 32.0f + flicker, Color{ 255, 120, 25, 240 }, Color{ 255, 60, 10, 0 });
    DrawCircle(800, 436, 14.0f + flicker * 0.5f, Color{ 255, 200, 60, 255 });

    // Smoke Chimney Hood
    DrawTriangle(Vector2{ 740.0f, 400.0f }, Vector2{ 860.0f, 400.0f }, Vector2{ 800.0f, 380.0f }, Color{ 60, 45, 40, 255 });

    // 2. Heavy Master Blacksmith Anvil at {784, 516, 48, 28}
    Rectangle anvilBase = { 792.0f, 522.0f, 32.0f, 22.0f };
    DrawRectangleRec(anvilBase, Color{ 45, 50, 60, 255 });
    DrawRectangleLinesEx(anvilBase, 2.0f, Color{ 90, 105, 125, 255 });
    // Anvil Horn and Face
    DrawRectangle(784, 516, 48, 10, Color{ 75, 85, 100, 255 });
    DrawRectangleLines(784, 516, 48, 10, Color{ 130, 145, 170, 255 });
    // Hammer resting on anvil
    DrawLineEx(Vector2{ 795.0f, 518.0f }, Vector2{ 815.0f, 518.0f }, 2.5f, Color{ 140, 100, 65, 255 });
    DrawRectangle(812, 514, 8, 8, Color{ 180, 190, 205, 255 });

    // 3. Quenching Water Trough at {580, 480, 32, 64}
    Rectangle trough = { 580.0f, 480.0f, 32.0f, 64.0f };
    DrawRectangleRec(trough, Color{ 55, 42, 32, 255 });
    DrawRectangleLinesEx(trough, 2.0f, Color{ 100, 75, 55, 255 });
    // Water surface with faint blue ripple
    Rectangle water = { 584.0f, 484.0f, 24.0f, 56.0f };
    DrawRectangleRec(water, Color{ 35, 95, 140, 220 });
    // Rising steam wisps
    float steamPulse = sinf(time * 3.0f);
    DrawCircleGradient(Vector2{ 596.0f, 474.0f }, 12.0f, Color{ 200, 225, 255, (unsigned char)(40 + 20 * steamPulse) }, Color{ 0, 0, 0, 0 });

    // 4. Armory Workbenches & Coal Bin
    Rectangle bench1 = { 960.0f, 460.0f, 90.0f, 36.0f };
    DrawRectangleRec(bench1, Color{ 58, 44, 34, 255 });
    DrawRectangleLinesEx(bench1, 2.0f, Color{ 110, 85, 65, 255 });
    // Steel ingots on bench
    for (int i = 0; i < 3; i++) {
        DrawRectangle(975 + i * 22, 468, 16, 12, Color{ 120, 130, 145, 255 });
        DrawRectangleLines(975 + i * 22, 468, 16, 12, Color{ 180, 195, 215, 255 });
    }

    Rectangle bench2 = { 960.0f, 620.0f, 90.0f, 36.0f };
    DrawRectangleRec(bench2, Color{ 58, 44, 34, 255 });
    DrawRectangleLinesEx(bench2, 2.0f, Color{ 110, 85, 65, 255 });

    // 5. Southern Exit Doorway
    DrawRectangle(770, 915, 60, 6, Color{ 20, 15, 12, 255 });
    DrawRectangleLines(770, 915, 60, 6, Color{ 220, 140, 60, 255 });
}

void GameMap::DrawRoyalKeepFloor() const {
    // 1. Polished Royal Slate & Marble Flagstones across 1200x1100 hall
    DrawRectangleRec(bounds, Color{ 18, 22, 32, 255 });

    const int tileSize = 50;
    int startX = (int)bounds.x;
    int endX = (int)(bounds.x + bounds.width);
    int startY = (int)bounds.y;
    int endY = (int)(bounds.y + bounds.height);

    for (int x = startX; x <= endX; x += tileSize) {
        DrawLine(x, startY, x, endY, Color{ 28, 36, 52, 140 });
    }
    for (int y = startY; y <= endY; y += tileSize) {
        DrawLine(startX, y, endX, y, Color{ 28, 36, 52, 140 });
    }

    // 2. Western Wing (Archives) Floor Accent
    Rectangle westWingFloor = { 420.0f, 180.0f, 340.0f, 1060.0f };
    DrawRectangleRec(westWingFloor, Color{ 22, 26, 38, 255 });
    DrawRectangleLinesEx(westWingFloor, 2.0f, Color{ 48, 62, 85, 180 });

    // 3. Eastern Wing (Armory) Floor Accent
    Rectangle eastWingFloor = { 1240.0f, 180.0f, 340.0f, 1060.0f };
    DrawRectangleRec(eastWingFloor, Color{ 26, 24, 34, 255 });
    DrawRectangleLinesEx(eastWingFloor, 2.0f, Color{ 68, 52, 65, 180 });

    // 4. Grand Royal Crimson Carpet from South Entrance (y = 1240) to Throne Dais (y = 300)
    Rectangle carpet = { 930.0f, 300.0f, 140.0f, 940.0f };
    DrawRectangleRec(carpet, Color{ 110, 22, 34, 255 });
    DrawRectangleLinesEx(carpet, 3.5f, Color{ 235, 195, 65, 255 });

    // Carpet gold embroidery patterns
    for (int y = 330; y <= 1220; y += 45) {
        DrawLine(945, y, 1055, y, Color{ 160, 40, 50, 200 });
        DrawCircleLines(1000, y, 7.0f, Color{ 220, 180, 60, 200 });
    }

    // 5. Sovereign Seal Medallion in front of Dais (at y = 410)
    Vector2 sealCenter = { 1000.0f, 410.0f };
    DrawCircleLines((int)sealCenter.x, (int)sealCenter.y, 52.0f, Color{ 225, 190, 65, 180 });
    DrawCircleLines((int)sealCenter.x, (int)sealCenter.y, 30.0f, Color{ 225, 190, 65, 200 });
    DrawCircleLines((int)sealCenter.x, (int)sealCenter.y, 10.0f, Color{ 255, 220, 95, 240 });

    // 6. Stepped Throne Dais Platform
    Rectangle daisLower = { 880.0f, 210.0f, 240.0f, 95.0f };
    DrawRectangleRec(daisLower, Color{ 30, 36, 50, 255 });
    DrawRectangleLinesEx(daisLower, 2.0f, Color{ 180, 150, 60, 220 });

    Rectangle daisUpper = { 910.0f, 225.0f, 180.0f, 65.0f };
    DrawRectangleRec(daisUpper, Color{ 42, 50, 70, 255 });
    DrawRectangleLinesEx(daisUpper, 2.5f, Color{ 245, 205, 75, 255 });
}

void GameMap::DrawRoyalKeepWalls() const {
    float time = (float)GetTime();

    // 1. High-backed Gilded Sovereign Throne behind King Kreptu (at {1000, 250})
    Rectangle throneBack = { 972.0f, 220.0f, 56.0f, 48.0f };
    DrawRectangleRec(throneBack, Color{ 130, 20, 32, 255 });
    DrawRectangleLinesEx(throneBack, 3.0f, Color{ 255, 215, 65, 255 });

    // Golden Crown Finials on Throne
    DrawCircle(974, 218, 5.0f, Color{ 255, 220, 80, 255 });
    DrawCircle(1000, 214, 8.0f, Color{ 255, 230, 95, 255 });
    DrawCircle(1026, 218, 5.0f, Color{ 255, 220, 80, 255 });

    // Throne Armrests
    DrawRectangle(964, 254, 12, 20, Color{ 215, 175, 55, 255 });
    DrawRectangle(1024, 254, 12, 20, Color{ 215, 175, 55, 255 });

    // 2. Colonnade Pillars & Blazing Braziers
    for (const auto& pillar : pillars) {
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y + pillar.radius * 0.4f), pillar.radius * 1.15f, Color{ 10, 12, 16, 130 });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius, Color{ 42, 50, 70, 255 });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius - 5.0f, Color{ 62, 74, 102, 255 });
        DrawCircleLines((int)pillar.position.x, (int)pillar.position.y, pillar.radius - 2.0f, Color{ 225, 185, 65, 200 });

        // Blazing brazier flame
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), 8.0f, Color{ 255, 170, 45, 240 });
        DrawCircle((int)pillar.position.x, (int)(pillar.position.y - 4.0f), 4.0f, Color{ 255, 240, 140, 255 });
    }

    // 3. Western Wing (Archives) Fixtures: Bookshelves & Reading desks
    DrawRectangle(480, 320, 200, 28, Color{ 58, 42, 30, 255 });
    DrawRectangleLines(480, 320, 200, 28, Color{ 110, 80, 50, 255 });

    DrawRectangle(480, 440, 200, 28, Color{ 58, 42, 30, 255 });
    DrawRectangleLines(480, 440, 200, 28, Color{ 110, 80, 50, 255 });

    DrawRectangle(480, 580, 120, 40, Color{ 68, 48, 34, 255 });
    DrawRectangleLines(480, 580, 120, 40, Color{ 125, 95, 60, 255 });

    // 4. Eastern Wing (Armory) Fixtures: Weapon Racks & Armor Anvils
    DrawRectangle(1320, 320, 200, 28, Color{ 48, 52, 60, 255 });
    DrawRectangleLines(1320, 320, 200, 28, Color{ 90, 105, 125, 255 });

    DrawRectangle(1320, 440, 200, 28, Color{ 48, 52, 60, 255 });
    DrawRectangleLines(1320, 440, 200, 28, Color{ 90, 105, 125, 255 });

    DrawRectangle(1400, 580, 120, 40, Color{ 52, 56, 66, 255 });
    DrawRectangleLines(1400, 580, 120, 40, Color{ 110, 120, 140, 255 });

    // 5. Royal Banners along walls
    for (float by : { 360.0f, 600.0f, 840.0f, 1040.0f }) {
        // West Wall Banner
        Rectangle wBanner = { bounds.x + 8.0f, by, 24.0f, 75.0f };
        DrawRectangleRec(wBanner, Color{ 28, 48, 92, 255 });
        DrawRectangleLinesEx(wBanner, 2.0f, Color{ 235, 195, 65, 255 });
        DrawCircle((int)(wBanner.x + 12), (int)(by + 36), 6.0f, Color{ 255, 220, 80, 255 });

        // East Wall Banner
        Rectangle eBanner = { bounds.x + bounds.width - 32.0f, by, 24.0f, 75.0f };
        DrawRectangleRec(eBanner, Color{ 28, 48, 92, 255 });
        DrawRectangleLinesEx(eBanner, 2.0f, Color{ 235, 195, 65, 255 });
        DrawCircle((int)(eBanner.x + 12), (int)(by + 36), 6.0f, Color{ 255, 220, 80, 255 });
    }

    // 6. Southern Exit Threshold Doorway at y = 1250
    Rectangle exitDoor = { 950.0f, 1250.0f, 100.0f, 10.0f };
    DrawRectangleRec(exitDoor, Color{ 25, 30, 45, 255 });
    DrawRectangleLinesEx(exitDoor, 2.0f, Color{ 255, 215, 75, 255 });
    DrawCircleGradient(Vector2{ 1000.0f, 1245.0f }, 65.0f, Color{ 255, 210, 80, 85 }, Color{ 0, 0, 0, 0 });
}


