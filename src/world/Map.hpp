#pragma once
#include "raylib.h"
#include <vector>

enum class MapEnvironment {
    LOBBY,
    ARENA,
    RUINED_CASTLE
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
};
