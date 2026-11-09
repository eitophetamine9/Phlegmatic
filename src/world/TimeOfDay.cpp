#include "TimeOfDay.hpp"
#include <cmath>
#include <cstdio>
#include <algorithm>

static Color LerpColor(Color c1, Color c2, float t) {
    t = fmaxf(0.0f, fminf(1.0f, t));
    return Color{
        (unsigned char)(c1.r + (c2.r - c1.r) * t),
        (unsigned char)(c1.g + (c2.g - c1.g) * t),
        (unsigned char)(c1.b + (c2.b - c1.b) * t),
        (unsigned char)(c1.a + (c2.a - c1.a) * t)
    };
}

TimeOfDaySystem::TimeOfDaySystem()
    : worldHours(8.0f), timeSpeed(24.0f / 360.0f) { // 360 seconds for full 24h cycle
}

void TimeOfDaySystem::Update(float dt) {
    worldHours += timeSpeed * dt;
    if (worldHours >= 24.0f) {
        worldHours = fmodf(worldHours, 24.0f);
    }
}

float TimeOfDaySystem::GetHour() const {
    return floorf(worldHours);
}

float TimeOfDaySystem::GetMinute() const {
    float mins = (worldHours - floorf(worldHours)) * 60.0f;
    return floorf(mins);
}

std::string TimeOfDaySystem::GetFormattedTime() const {
    int h = (int)GetHour();
    int m = (int)GetMinute();
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", h, m);
    return std::string(buf);
}

TimePhase TimeOfDaySystem::GetPhase() const {
    if (worldHours >= 5.0f && worldHours < 8.5f) return TimePhase::DAWN;
    if (worldHours >= 8.5f && worldHours < 18.0f) return TimePhase::DAY;
    if (worldHours >= 18.0f && worldHours < 21.0f) return TimePhase::DUSK;
    return TimePhase::NIGHT;
}

const char* TimeOfDaySystem::GetPhaseName() const {
    switch (GetPhase()) {
        case TimePhase::DAWN: return "DAWN";
        case TimePhase::DAY: return "DAY";
        case TimePhase::DUSK: return "DUSK";
        case TimePhase::NIGHT: return "NIGHT";
    }
    return "UNKNOWN";
}

Color TimeOfDaySystem::GetAmbientColor() const {
    // 08:30 - 18:00 : Clear daylight
    if (worldHours >= 8.5f && worldHours < 18.0f) {
        return Color{ 0, 0, 0, 0 };
    }
    // 18:00 - 20:00 : Sunset twilight onset
    if (worldHours >= 18.0f && worldHours < 20.0f) {
        float t = (worldHours - 18.0f) / 2.0f;
        return LerpColor(Color{ 0, 0, 0, 0 }, Color{ 145, 65, 105, 75 }, t);
    }
    // 20:00 - 21.5 : Dusk into deep night
    if (worldHours >= 20.0f && worldHours < 21.5f) {
        float t = (worldHours - 20.0f) / 1.5f;
        return LerpColor(Color{ 145, 65, 105, 75 }, Color{ 8, 12, 28, 160 }, t);
    }
    // 21:30 - 05:00 : Deep night
    if (worldHours >= 21.5f || worldHours < 5.0f) {
        return Color{ 8, 12, 28, 160 };
    }
    // 05:00 - 07:00 : Night to dawn sunrise
    if (worldHours >= 5.0f && worldHours < 7.0f) {
        float t = (worldHours - 5.0f) / 2.0f;
        return LerpColor(Color{ 8, 12, 28, 160 }, Color{ 235, 150, 75, 65 }, t);
    }
    // 07:00 - 08:30 : Dawn into clear day
    float t = (worldHours - 7.0f) / 1.5f;
    return LerpColor(Color{ 235, 150, 75, 65 }, Color{ 0, 0, 0, 0 }, t);
}

float TimeOfDaySystem::GetDarknessLevel() const {
    Color amb = GetAmbientColor();
    return (float)amb.a / 255.0f;
}

void TimeOfDaySystem::ClearLights() {
    activeLights.clear();
}

void TimeOfDaySystem::RegisterLight(Vector2 pos, float radius, Color color, float intensity) {
    activeLights.push_back({ pos, radius, color, intensity });
}

void TimeOfDaySystem::DrawLighting(Camera2D camera, Vector2 playerPos, const Rectangle& mapBounds, const std::vector<Vector2>& torches, bool isInterior) {
    (void)camera;

    if (isInterior) {
        // Soft cozy tavern/forge warm hearth glow
        DrawRectangleRec(mapBounds, Color{ 25, 15, 10, 35 });
        return;
    }

    Color ambColor = GetAmbientColor();
    float darkness = GetDarknessLevel();

    if (ambColor.a == 0 && darkness < 0.01f) {
        return;
    }

    // 1. World Ambient Darkness Tint
    DrawRectangleRec(mapBounds, ambColor);

    // 2. Point Lights (Flickering Torches, Campfires, and Player Lantern)
    // Player hip lantern
    if (playerPos.x > -9000.0f) {
        float pFlicker = 1.0f + 0.03f * sinf((float)GetTime() * 8.0f);
        DrawCircleGradient(playerPos, 190.0f * pFlicker, Color{ 255, 225, 150, (unsigned char)(70 * darkness) }, Color{ 0, 0, 0, 0 });
        DrawCircleGradient(playerPos, 90.0f * pFlicker, Color{ 255, 245, 190, (unsigned char)(110 * darkness) }, Color{ 0, 0, 0, 0 });
    }

    // Registered Torches from Map
    for (const auto& torch : torches) {
        float tFlicker = 1.0f + 0.05f * sinf((float)GetTime() * 9.0f + torch.x);
        DrawCircleGradient(torch, 150.0f * tFlicker, Color{ 255, 155, 45, (unsigned char)(75 * darkness) }, Color{ 0, 0, 0, 0 });
        DrawCircleGradient(torch, 70.0f * tFlicker, Color{ 255, 210, 95, (unsigned char)(125 * darkness) }, Color{ 0, 0, 0, 0 });
    }

    // Registered dynamic point lights (campfires, altars, spells)
    for (const auto& light : activeLights) {
        float lFlicker = 1.0f + 0.04f * sinf((float)GetTime() * 7.5f + light.position.y);
        DrawCircleGradient(light.position, light.radius * lFlicker, ColorAlpha(light.color, 0.40f * light.intensity * darkness), Color{ 0, 0, 0, 0 });
        DrawCircleGradient(light.position, light.radius * 0.45f * lFlicker, ColorAlpha(light.color, 0.65f * light.intensity * darkness), Color{ 0, 0, 0, 0 });
    }

    // 3. Starlight Twinkles at Deep Night
    if (GetPhase() == TimePhase::NIGHT) {
        float t = (float)GetTime();
        for (int i = 0; i < 40; ++i) {
            float sx = mapBounds.x + fmodf((float)(i * 137 + 41), mapBounds.width);
            float sy = mapBounds.y + fmodf((float)(i * 93 + 17), mapBounds.height * 0.45f);
            float twinkle = 0.5f + 0.5f * sinf(t * 3.0f + (float)i * 1.7f);
            DrawCircle((int)sx, (int)sy, 1.5f, Color{ 210, 230, 255, (unsigned char)(80 * twinkle) });
        }
    }
}

