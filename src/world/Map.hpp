#pragma once
#include "raylib.h"
#include <vector>

struct Pillar {
    Vector2 position;
    float radius;
};

class GameMap {
public:
    GameMap();

    void Init();
    void DrawFloor() const;
    void DrawWallsAndPillars() const;

    bool CheckCircleCollision(Vector2 pos, float radius) const;
    Vector2 ResolveCollision(Vector2 oldPos, Vector2 newPos, float radius) const;

    Rectangle GetBounds() const { return bounds; }
    const std::vector<Pillar>& GetPillars() const { return pillars; }

private:
    Rectangle bounds;
    std::vector<Pillar> pillars;
    std::vector<Vector2> torches;
};
