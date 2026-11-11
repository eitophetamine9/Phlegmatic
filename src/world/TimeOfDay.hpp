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
    ~TimeOfDaySystem();

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

    void InitResources(int width, int height);
    void UnloadResources();

    void DrawLighting(
        Camera2D camera,
        Vector2 playerPos,
        const Rectangle& mapBounds,
        const std::vector<Vector2>& torches,
        bool isInterior = false,
        bool isSubterranean = false,
        bool isFrostSubterranean = false
    );

private:
    float worldHours;
    float timeSpeed;
    std::vector<PointLight> activeLights;

    RenderTexture2D lightMap = { 0 };
    bool lightMapInitialized = false;
    Shader pointLightShader = { 0 };
    Shader compositeShader = { 0 };
    bool shadersInitialized = false;
    int compositeDarknessLoc = -1;
    Texture2D whiteQuadTex = { 0 };
    Texture2D radialFalloffTex = { 0 };
    bool texturesInitialized = false;
};
