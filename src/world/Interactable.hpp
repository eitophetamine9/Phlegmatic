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

        if (attack.isLineBox) {
            float rad = attack.angle * DEG2RAD;
            Vector2 dir = { cosf(rad), sinf(rad) };
            Vector2 normal = { -dir.y, dir.x };
            Vector2 toDummy = { position.x - attack.origin.x, position.y - attack.origin.y };
            float forwardDist = toDummy.x * dir.x + toDummy.y * dir.y;
            float lateralDist = fabsf(toDummy.x * normal.x + toDummy.y * normal.y);
            if (forwardDist >= -radius && forwardDist <= (attack.range + radius) &&
                lateralDist <= (attack.width * 0.5f + radius)) {
                shakeTimer = 0.15f;
                totalDamageTaken += attack.damage;
                SoundFX::PlayHitImpact();
                particles.SpawnSparks(position, Color{ 255, 200, 80, 255 }, 14, 140.0f);
                particles.SpawnFloatingText(position, "-" + std::to_string((int)attack.damage), Color{ 255, 220, 80, 255 }, 22.0f);
                return true;
            }
            return false;
        }

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

    void Draw(float time, bool isUnlocked = true) const {
        float pulse = 1.0f + 0.06f * sinf(time * 3.5f);
        if (isUnlocked) {
            // Glowing portal rings
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
        } else {
            // Locked red runic barrier
            DrawCircleGradient(position, radius * 1.25f, Color{ 160, 30, 40, 45 }, Color{ 0, 0, 0, 0 });
            DrawCircleLines((int)position.x, (int)position.y, radius, Color{ 170, 50, 50, 200 });
            DrawCircleLines((int)position.x, (int)position.y, radius * 0.65f, Color{ 120, 35, 35, 160 });
            DrawCircleV(position, radius * 0.35f, Color{ 45, 15, 20, 220 });

            const char* label = "ARENA [LOCKED]";
            int w = MeasureText(label, 11);
            DrawText(label, (int)(position.x - w * 0.5f), (int)(position.y - 6), 11, Color{ 240, 100, 100, 220 });
        }
    }
};

struct AdhesiveBandage {
    Vector2 position;
    float radius = 18.0f;
    float healAmount = 30.0f;
    bool active = true;
    float bobTimer = 0.0f;

    void Update(float dt) {
        bobTimer += dt;
    }

    bool CheckPickup(Vector2 playerPos) const {
        if (!active) return false;
        return Vector2Distance(playerPos, position) <= (radius + 22.0f);
    }

    void Draw() const {
        if (!active) return;
        float bob = sinf(bobTimer * 4.0f) * 3.5f;
        Vector2 drawPos = { position.x, position.y + bob };

        // Soft ground shadow
        DrawEllipse((int)position.x, (int)(position.y + 10), radius * 0.9f, radius * 0.45f, Color{ 10, 12, 18, 90 });

        // Healing emerald aura glow
        float auraPulse = 1.0f + 0.15f * sinf(bobTimer * 5.0f);
        DrawCircleGradient(drawPos, (radius + 12.0f) * auraPulse, Color{ 60, 230, 120, 50 }, Color{ 0, 0, 0, 0 });
        DrawCircleLines((int)drawPos.x, (int)drawPos.y, radius + 2.0f, Color{ 100, 255, 160, 140 });

        // Bandage cloth backing
        Rectangle bandageRect = { drawPos.x, drawPos.y, 28.0f, 14.0f };
        Vector2 origin = { 14.0f, 7.0f };
        DrawRectanglePro(bandageRect, origin, -18.0f, Color{ 245, 230, 200, 255 }); // Pale cloth adhesive
        
        // Bandage borders & details
        DrawRectangle((int)(drawPos.x - 4.0f), (int)(drawPos.y - 7.0f), 8, 14, Color{ 225, 210, 180, 255 });

        // Emerald medical cross in center
        DrawRectangle((int)(drawPos.x - 2.0f), (int)(drawPos.y - 5.0f), 4, 10, Color{ 30, 200, 80, 255 });
        DrawRectangle((int)(drawPos.x - 5.0f), (int)(drawPos.y - 2.0f), 10, 4, Color{ 30, 200, 80, 255 });

        // Glint sparkle
        float glintX = drawPos.x + 8.0f;
        float glintY = drawPos.y - 6.0f;
        DrawCircle((int)glintX, (int)glintY, 2.0f, Color{ 255, 255, 255, (unsigned char)(180 + 75 * sinf(bobTimer * 6.0f)) });

        // Name tag
        const char* label = "ADHESIVE BANDAGE (+30 HP)";
        int w = MeasureText(label, 10);
        DrawText(label, (int)(drawPos.x - w * 0.5f), (int)(drawPos.y - radius - 16.0f), 10, Color{ 120, 255, 170, 220 });
    }
};
