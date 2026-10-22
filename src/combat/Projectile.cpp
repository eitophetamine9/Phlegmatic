#include "Projectile.hpp"
#include "../core/SoundFX.hpp"
#include "raymath.h"
#include <cmath>

Projectile::Projectile(Vector2 pos, Vector2 vel, float radius, float damage, ProjectileType type, bool isFromPlayer)
    : position(pos), velocity(vel), radius(radius), damage(damage), type(type), isFromPlayer(isFromPlayer), active(true), lifetime(0.0f) {
    
    switch (type) {
        case ProjectileType::BOLT:
            maxLifetime = 2.2f;
            color = isFromPlayer ? Color{ 100, 230, 255, 255 } : Color{ 255, 140, 50, 255 };
            break;
        case ProjectileType::VOID_ORB:
            maxLifetime = 3.5f;
            color = isFromPlayer ? Color{ 80, 240, 255, 255 } : Color{ 180, 70, 255, 255 };
            break;
        case ProjectileType::NOVA_STAR:
            maxLifetime = 3.0f;
            color = isFromPlayer ? Color{ 120, 255, 200, 255 } : Color{ 255, 190, 40, 255 };
            break;
        case ProjectileType::BEAM_SHARD:
            maxLifetime = 1.8f;
            color = isFromPlayer ? Color{ 100, 240, 255, 255 } : Color{ 255, 60, 90, 255 };
            break;
    }
}

void Projectile::Update(float dt, const GameMap& map, ParticleSystem& particles) {
    if (!active) return;

    lifetime += dt;
    if (lifetime >= maxLifetime) {
        active = false;
        return;
    }

    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    // Subtle trail sparks
    if (type == ProjectileType::VOID_ORB && GetRandomValue(0, 2) == 0) {
        particles.SpawnSparks(position, color, 1, 25.0f);
    } else if (type == ProjectileType::NOVA_STAR && GetRandomValue(0, 3) == 0) {
        particles.SpawnSparks(position, color, 1, 30.0f);
    }

    // Check collision against map walls and pillars
    if (map.CheckCircleCollision(position, radius)) {
        active = false;
        particles.SpawnSparks(position, color, 8, 80.0f);
    }
}

void Projectile::Reflect(Vector2 newVel) {
    isFromPlayer = true;
    velocity = newVel;
    damage *= 1.5f; // +50% counter damage
    lifetime = 0.0f; // Reset lifetime so it reaches target
    color = Color{ 80, 230, 255, 255 }; // Cyan player barrier energy
}

void Projectile::Draw() const {
    if (!active) return;

    float angle = atan2f(velocity.y, velocity.x) * RAD2DEG;
    float rad = angle * DEG2RAD;
    Vector2 dir = { cosf(rad), sinf(rad) };
    Vector2 normal = { -dir.y, dir.x };

    switch (type) {
        case ProjectileType::BOLT: {
            // High-speed piercing bolt: arrowhead + trailing line
            Vector2 tip = { position.x + dir.x * (radius * 2.2f), position.y + dir.y * (radius * 2.2f) };
            Vector2 tail = { position.x - dir.x * (radius * 2.5f), position.y - dir.y * (radius * 2.5f) };
            Vector2 barbL = { position.x - dir.x * 2.0f - normal.x * (radius * 1.2f), position.y - dir.y * 2.0f - normal.y * (radius * 1.2f) };
            Vector2 barbR = { position.x - dir.x * 2.0f + normal.x * (radius * 1.2f), position.y - dir.y * 2.0f + normal.y * (radius * 1.2f) };

            DrawLineEx(tail, tip, 3.0f, color);
            DrawTriangle(tip, barbL, barbR, color);
            DrawCircleV(tip, 2.5f, Color{ 255, 255, 255, 240 });
            break;
        }

        case ProjectileType::VOID_ORB: {
            // Pulsing occult sphere with outer corona
            float time = (float)GetTime() * 8.0f;
            float pulse = 1.0f + 0.15f * sinf(time);

            Color outerAura = color;
            outerAura.a = 70;
            DrawCircleV(position, radius * 1.8f * pulse, outerAura);

            Color innerColor = color;
            innerColor.a = 210;
            DrawCircleV(position, radius * pulse, innerColor);
            DrawCircleV(position, radius * 0.45f, Color{ 255, 255, 255, 240 });
            break;
        }

        case ProjectileType::NOVA_STAR: {
            // 4-pointed radiant stellar projectile
            float time = (float)GetTime() * 10.0f;
            float pulseRadius = radius * (1.0f + 0.2f * sinf(time));

            Color aura = color;
            aura.a = 80;
            DrawCircleV(position, pulseRadius * 1.6f, aura);
            DrawCircleV(position, pulseRadius, color);

            // Cross spikes
            float spikeLen = pulseRadius * 2.0f;
            DrawLineEx({ position.x - spikeLen, position.y }, { position.x + spikeLen, position.y }, 2.0f, Color{ 255, 255, 255, 220 });
            DrawLineEx({ position.x, position.y - spikeLen }, { position.x, position.y + spikeLen }, 2.0f, Color{ 255, 255, 255, 220 });
            DrawCircleV(position, pulseRadius * 0.4f, Color{ 255, 255, 255, 255 });
            break;
        }

        case ProjectileType::BEAM_SHARD: {
            // Elongated diamond shard with intense core
            Vector2 tip = { position.x + dir.x * (radius * 2.8f), position.y + dir.y * (radius * 2.8f) };
            Vector2 tail = { position.x - dir.x * (radius * 2.8f), position.y - dir.y * (radius * 2.8f) };
            Vector2 sideL = { position.x - normal.x * (radius * 0.9f), position.y - normal.y * (radius * 0.9f) };
            Vector2 sideR = { position.x + normal.x * (radius * 0.9f), position.y + normal.y * (radius * 0.9f) };

            DrawTriangle(tip, sideL, sideR, color);
            DrawTriangle(tail, sideR, sideL, color);
            DrawLineEx(tail, tip, 2.0f, Color{ 255, 255, 255, 240 });
            break;
        }
    }
}
