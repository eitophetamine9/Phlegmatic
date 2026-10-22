#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include "../combat/Weapon.hpp"
#include "../core/Particles.hpp"
#include "../core/SoundFX.hpp"
#include "raymath.h"
#include <cmath>

struct WeaponPedestal {
    Vector2 position;
    WeaponType weapon;
    float radius = 40.0f;

    bool CheckInteraction(Vector2 playerPos) const {
        return Vector2Distance(playerPos, position) <= (radius + 28.0f);
    }
};

struct TrainingDummy {
    Vector2 position = { 800.0f, 820.0f };
    float radius = 24.0f;
    float shakeTimer = 0.0f;
    float totalDamageTaken = 0.0f;

    void Update(float dt) {
        if (shakeTimer > 0.0f) {
            shakeTimer -= dt;
        }
    }

    bool CheckHit(const AttackInfo& attack, ParticleSystem& particles) {
        if (attack.type == AttackType::NONE) return false;

        float dist = Vector2Distance(position, attack.origin);
        if (dist > attack.range + radius) return false;

        float dx = position.x - attack.origin.x;
        float dy = position.y - attack.origin.y;
        float hitAngle = atan2f(dy, dx) * RAD2DEG;
        float diff = fabsf(hitAngle - attack.angle);
        while (diff > 180.0f) diff = 360.0f - diff;

        if (diff <= (attack.arcAngle * 0.5f + 15.0f)) {
            shakeTimer = 0.15f;
            totalDamageTaken += attack.damage;
            SoundFX::PlayHitImpact();
            particles.SpawnSparks(position, Color{ 255, 200, 80, 255 }, 14, 140.0f);
            particles.SpawnFloatingText(position, "-" + std::to_string((int)attack.damage), Color{ 255, 220, 80, 255 }, 22.0f);
            return true;
        }
        return false;
    }

    void Draw() const {
        Vector2 drawPos = position;
        if (shakeTimer > 0.0f) {
            drawPos.x += (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 4.0f;
            drawPos.y += (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 4.0f;
        }

        // Shadow
        DrawEllipse((int)drawPos.x, (int)(drawPos.y + 12), radius * 1.1f, radius * 0.5f, Color{ 10, 12, 18, 90 });

        // Wooden base stand
        DrawRectangle((int)(drawPos.x - 18), (int)(drawPos.y + 6), 36, 14, Color{ 70, 48, 30, 255 });
        DrawRectangleLines((int)(drawPos.x - 18), (int)(drawPos.y + 6), 36, 14, Color{ 110, 80, 50, 255 });

        // Straw Torso
        DrawCircleV(drawPos, radius, Color{ 180, 140, 75, 255 });
        DrawCircleLines((int)drawPos.x, (int)drawPos.y, radius, Color{ 120, 90, 45, 255 });

        // Cross ropes
        DrawLineEx({ drawPos.x - 14, drawPos.y - 14 }, { drawPos.x + 14, drawPos.y + 14 }, 3.0f, Color{ 100, 75, 40, 255 });
        DrawLineEx({ drawPos.x + 14, drawPos.y - 14 }, { drawPos.x - 14, drawPos.y + 14 }, 3.0f, Color{ 100, 75, 40, 255 });

        // Target bullseye mark
        DrawCircleV(drawPos, 8.0f, Color{ 220, 50, 50, 230 });
        DrawCircleV(drawPos, 4.0f, Color{ 255, 255, 255, 255 });

        // Label above
        DrawText("TRAINING DUMMY", (int)(drawPos.x - 46), (int)(drawPos.y - radius - 18), 10, Color{ 200, 215, 235, 200 });
    }
};

struct ArenaPortal {
    Vector2 position = { 800.0f, 430.0f };
    float radius = 52.0f;

    bool CheckPlayerInside(Vector2 playerPos) const {
        return Vector2Distance(playerPos, position) <= radius;
    }

    void Draw(float time) const {
        // Glowing portal rings
        float pulse = 1.0f + 0.06f * sinf(time * 3.5f);
        DrawCircleGradient(position, radius * 1.5f * pulse, Color{ 80, 160, 255, 55 }, Color{ 0, 0, 0, 0 });
        DrawCircleLines((int)position.x, (int)position.y, radius * pulse, Color{ 100, 220, 255, 240 });
        DrawCircleLines((int)position.x, (int)position.y, radius * 0.7f, Color{ 70, 140, 255, 180 });
        DrawCircleV(position, radius * 0.45f, Color{ 30, 60, 110, 180 });

        // Rotating magical runes
        for (int i = 0; i < 4; i++) {
            float angle = time * 1.8f + (float)i * (PI * 0.5f);
            Vector2 runePos = { position.x + cosf(angle) * (radius * 0.82f), position.y + sinf(angle) * (radius * 0.82f) };
            DrawCircleV(runePos, 4.5f, Color{ 140, 240, 255, 240 });
        }

        // Text prompt
        const char* label = "ENTER ARENA";
        int w = MeasureText(label, 12);
        DrawText(label, (int)(position.x - w * 0.5f), (int)(position.y - 6), 12, Color{ 255, 255, 255, 255 });
    }
};
