#pragma once
#include "raylib.h"
#include <vector>
#include <string>

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float size;
    float life;
    float maxLife;
};

struct FloatingText {
    Vector2 position;
    std::string text;
    Color color;
    float fontSize;
    float life;
    float maxLife;
};

class ParticleSystem {
public:
    ParticleSystem() = default;

    void Update(float dt);
    void Draw() const;

    void SpawnSparks(Vector2 pos, Color color, int count = 12, float speed = 140.0f);
    void SpawnParryBurst(Vector2 pos);
    void SpawnDashTrail(Vector2 pos, Vector2 dir, Color color);
    void SpawnSlashTrail(Vector2 pos, Vector2 dir, Color color);
    void SpawnFloatingText(Vector2 pos, const std::string& text, Color color, float size = 20.0f);
    void Clear();

    // Screen shake
    void AddScreenShake(float intensity, float duration);
    Vector2 GetScreenShakeOffset() const;

private:
    std::vector<Particle> particles;
    std::vector<FloatingText> floatingTexts;

    float shakeIntensity = 0.0f;
    float shakeDuration = 0.0f;
    float shakeTimer = 0.0f;
};
