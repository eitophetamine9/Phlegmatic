#pragma once
#include "raylib.h"
#include "../rendering/FontManager.hpp"
#include <vector>

enum class MapEnvironment {
    LOBBY,
    ARENA,
    RUINED_CASTLE,
    CROSSROADS,
    ZYPHDEN_REALM,
    TAVERN_INTERIOR,
    FORGE_INTERIOR,
    ROYAL_KEEP
};

struct Pillar {
    Vector2 position;
    float radius;
};

struct WallObstacle {
    Rectangle rect;
};

class GameMap {
public:
    GameMap();

    void Init(MapEnvironment env);
    void DrawFloor() const;
    void DrawWallsAndPillars() const;

    bool CheckCircleCollision(Vector2 pos, float radius) const;
    Vector2 ResolveCollision(Vector2 oldPos, Vector2 newPos, float radius) const;

    Rectangle GetBounds() const { return bounds; }
    MapEnvironment GetEnvironment() const { return currentEnv; }

    void SetGateOpen(bool open) { gateOpen = open; }
    bool IsGateOpen() const { return gateOpen; }

private:
    MapEnvironment currentEnv;
    Rectangle bounds;
    std::vector<Pillar> pillars;
    std::vector<Vector2> torches;
    std::vector<WallObstacle> obstacles;
    bool gateOpen = false;

    void DrawRuinedCastleFloor() const;
    void DrawRuinedCastleWalls() const;
    void DrawCrossroadsFloor() const;
    void DrawCrossroadsWalls() const;
    void DrawZyphdenFloor() const;
    void DrawZyphdenWalls() const;
    void DrawTavernFloor() const;
    void DrawTavernWalls() const;
    void DrawForgeFloor() const;
    void DrawForgeWalls() const;
    void DrawRoyalKeepFloor() const;
    void DrawRoyalKeepWalls() const;
};
