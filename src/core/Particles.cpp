#include "Particles.hpp"
#include "raymath.h"
#include <cstdlib>
#include <cmath>

void ParticleSystem::Update(float dt) {
    if (shakeTimer > 0.0f) {
        shakeTimer -= dt;
        if (shakeTimer <= 0.0f) {
            shakeIntensity = 0.0f;
        }
    }

    for (size_t i = 0; i < particles.size();) {
        particles[i].position.x += particles[i].velocity.x * dt;
        particles[i].position.y += particles[i].velocity.y * dt;
        particles[i].velocity.x *= 0.92f;
        particles[i].velocity.y *= 0.92f;
        particles[i].life -= dt;

        if (particles[i].life <= 0.0f) {
            particles[i] = particles.back();
            particles.pop_back();
        } else {
            i++;
        }
    }

    for (size_t i = 0; i < floatingTexts.size();) {
        floatingTexts[i].position.y -= 40.0f * dt;
        floatingTexts[i].life -= dt;

        if (floatingTexts[i].life <= 0.0f) {
            floatingTexts[i] = floatingTexts.back();
            floatingTexts.pop_back();
        } else {
            i++;
        }
    }
}

void ParticleSystem::Draw() const {
    for (const auto& p : particles) {
        float alphaFactor = (p.life / p.maxLife);
        Color c = p.color;
        c.a = (unsigned char)(c.a * alphaFactor);
        DrawCircleV(p.position, p.size * (0.5f + 0.5f * alphaFactor), c);
    }

    for (const auto& ft : floatingTexts) {
        float alphaFactor = (ft.life / ft.maxLife);
        Color c = ft.color;
        c.a = (unsigned char)(255 * alphaFactor);
        int textWidth = MeasureText(ft.text.c_str(), (int)ft.fontSize);
        DrawText(ft.text.c_str(), (int)(ft.position.x - textWidth / 2 + 1), (int)(ft.position.y + 1), (int)ft.fontSize, Color{ 0, 0, 0, (unsigned char)(180 * alphaFactor) });
        DrawText(ft.text.c_str(), (int)(ft.position.x - textWidth / 2), (int)ft.position.y, (int)ft.fontSize, c);
    }
}

void ParticleSystem::SpawnSparks(Vector2 pos, Color color, int count, float speed) {
    for (int i = 0; i < count; i++) {
        float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * PI;
        float spd = speed * (0.4f + 0.6f * ((float)rand() / (float)RAND_MAX));
        float life = 0.25f + 0.25f * ((float)rand() / (float)RAND_MAX);
        particles.push_back({
            pos,
            { cosf(angle) * spd, sinf(angle) * spd },
            color,
            3.5f + ((float)rand() / (float)RAND_MAX) * 2.5f,
            life,
            life
        });
    }
}

void ParticleSystem::SpawnParryBurst(Vector2 pos) {
    SpawnSparks(pos, Color{ 255, 230, 80, 255 }, 24, 260.0f);
    SpawnSparks(pos, Color{ 80, 240, 255, 255 }, 18, 200.0f);
    AddScreenShake(7.0f, 0.22f);
}

void ParticleSystem::SpawnDashTrail(Vector2 pos, Vector2 dir, Color color) {
    for (int i = 0; i < 4; i++) {
        float offsetAngle = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.6f;
        float baseAngle = atan2f(-dir.y, -dir.x) + offsetAngle;
        float spd = 40.0f + 40.0f * ((float)rand() / (float)RAND_MAX);
        float life = 0.16f + 0.14f * ((float)rand() / (float)RAND_MAX);
        particles.push_back({
            pos,
            { cosf(baseAngle) * spd, sinf(baseAngle) * spd },
            color,
            5.0f,
            life,
            life
        });
    }
}

void ParticleSystem::SpawnSlashTrail(Vector2 pos, Vector2 dir, Color color) {
    for (int i = 0; i < 6; i++) {
        float offsetAngle = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.8f;
        float baseAngle = atan2f(dir.y, dir.x) + offsetAngle;
        float spd = 60.0f + 60.0f * ((float)rand() / (float)RAND_MAX);
        float life = 0.15f + 0.15f * ((float)rand() / (float)RAND_MAX);
        particles.push_back({
            pos,
            { cosf(baseAngle) * spd, sinf(baseAngle) * spd },
            color,
            4.0f,
            life,
            life
        });
    }
}

void ParticleSystem::SpawnFloatingText(Vector2 pos, const std::string& text, Color color, float size) {
    floatingTexts.push_back({
        pos,
        text,
        color,
        size,
        0.85f,
        0.85f
    });
}

void ParticleSystem::AddScreenShake(float intensity, float duration) {
    if (intensity > shakeIntensity || shakeTimer <= 0.0f) {
        shakeIntensity = intensity;
        shakeDuration = duration;
        shakeTimer = duration;
    }
}

Vector2 ParticleSystem::GetScreenShakeOffset() const {
    if (shakeTimer <= 0.0f) return { 0.0f, 0.0f };
    float factor = shakeTimer / shakeDuration;
    float currentIntensity = shakeIntensity * factor;
    float offsetX = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * currentIntensity;
    float offsetY = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * currentIntensity;
    return { offsetX, offsetY };
}
