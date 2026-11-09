#pragma once
#include "raylib.h"
#include <cmath>
#include <string>
#include <vector>

enum class TimePhase {
    DAWN,
    DAY,
    DUSK,
    NIGHT
};

struct PointLight {
    Vector2 position;
    float radius;
    Color color;
    float intensity = 1.0f;
};

class TimeOfDaySystem {
public:
    TimeOfDaySystem();

    void Update(float dt);

    float GetHour() const;
    float GetMinute() const;
    std::string GetFormattedTime() const;
    TimePhase GetPhase() const;
    const char* GetPhaseName() const;
    Color GetAmbientColor() const;
    float GetDarknessLevel() const;

    bool IsNight() const { return GetPhase() == TimePhase::NIGHT; }
    bool IsDuskOrNight() const { return GetPhase() == TimePhase::DUSK || GetPhase() == TimePhase::NIGHT; }

    void SetHour(float hour) { worldHours = fmodf(hour + 24.0f, 24.0f); }
    float GetRawHours() const { return worldHours; }

    void ClearLights();
    void RegisterLight(Vector2 pos, float radius, Color color, float intensity = 1.0f);

    void DrawLighting(Camera2D camera, Vector2 playerPos, const Rectangle& mapBounds, const std::vector<Vector2>& torches, bool isInterior = false);

private:
    float worldHours;
    float timeSpeed;
    std::vector<PointLight> activeLights;
};
