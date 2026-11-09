#pragma once
#include "raylib.h"
#include "../rendering/FontManager.hpp"
#include "TimeOfDay.hpp"
#include <vector>

enum class MapEnvironment {
    LOBBY,
    ARENA,
    RUINED_CASTLE,
    CROSSROADS,
    ZYPHDEN_REALM,
    TAVERN_INTERIOR,
    FORGE_INTERIOR,
    ROYAL_KEEP,
    QUARRY_DEPTHS,
    RESTLESS_MIST_FOREST,
    NORTH_ZYPHDEN
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
    void DrawFloor(Vector2 playerPos = { -9999.0f, -9999.0f }) const;
    void DrawWallsAndPillars(Vector2 playerPos = { -9999.0f, -9999.0f }) const;

    bool CheckCircleCollision(Vector2 pos, float radius) const;
    Vector2 ResolveCollision(Vector2 oldPos, Vector2 newPos, float radius) const;

    Rectangle GetBounds() const { return bounds; }
    MapEnvironment GetEnvironment() const { return currentEnv; }

    void SetGateOpen(bool open) { gateOpen = open; }
    bool IsGateOpen() const { return gateOpen; }

    void Update(float dt) { timeOfDay.Update(dt); }
    void DrawLighting(Camera2D camera, Vector2 playerPos) const;
    TimeOfDaySystem& GetTimeOfDay() { return timeOfDay; }
    const TimeOfDaySystem& GetTimeOfDay() const { return timeOfDay; }
    const std::vector<Vector2>& GetTorches() const { return torches; }
    bool IsInterior() const {
        return currentEnv == MapEnvironment::TAVERN_INTERIOR ||
               currentEnv == MapEnvironment::FORGE_INTERIOR ||
               currentEnv == MapEnvironment::ROYAL_KEEP;
    }

private:
    MapEnvironment currentEnv;
    Rectangle bounds;
    std::vector<Pillar> pillars;
    std::vector<Vector2> torches;
    std::vector<WallObstacle> obstacles;
    bool gateOpen = false;
    mutable TimeOfDaySystem timeOfDay;

    void DrawRuinedCastleFloor() const;
    void DrawRuinedCastleWalls(Vector2 playerPos) const;
    void DrawCrossroadsFloor(Vector2 playerPos) const;
    void DrawCrossroadsWalls(Vector2 playerPos) const;
    void DrawZyphdenFloor() const;
    void DrawZyphdenWalls() const;
    void DrawTavernFloor() const;
    void DrawTavernWalls(Vector2 playerPos) const;
    void DrawForgeFloor() const;
    void DrawForgeWalls() const;
    void DrawRoyalKeepFloor() const;
    void DrawRoyalKeepWalls() const;
    void DrawQuarryDepthsFloor() const;
    void DrawQuarryDepthsWalls(Vector2 playerPos) const;
    void DrawRestlessMistForestFloor() const;
    void DrawRestlessMistForestWalls(Vector2 playerPos) const;
    void DrawNorthZyphdenFloor(Vector2 playerPos) const;
    void DrawNorthZyphdenWalls(Vector2 playerPos) const;
};
