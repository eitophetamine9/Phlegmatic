#pragma once
#include "raylib.h"
#include "Combat.hpp"
#include "Map.hpp"
#include "Particles.hpp"

class Player {
public:
    Player();

    void Init(Vector2 startPos);
    void Update(float dt, const GameMap& map, Vector2 mouseWorldPos, ParticleSystem& particles);
    void Draw() const;

    // Combat triggers
    void TriggerLightAttack(ParticleSystem& particles);
    void TriggerHeavyAttack(ParticleSystem& particles);
    void TriggerParry(ParticleSystem& particles);

    // Damage handling
    bool TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles);
    void Heal(float amount);

    // Getters
    Vector2 GetPosition() const { return position; }
    float GetRadius() const { return radius; }
    float GetFacingAngle() const { return facingAngle; }
    float GetHealth() const { return health; }
    float GetMaxHealth() const { return maxHealth; }
    float GetStamina() const { return stamina; }
    float GetMaxStamina() const { return maxStamina; }
    bool IsParrying() const { return isParrying; }
    bool IsAlive() const { return health > 0.0f; }
    bool HasCounterBuff() const { return counterBuffTimer > 0.0f; }

    const AttackInfo& GetCurrentAttack() const { return currentAttack; }
    bool IsAttacking() const { return currentAttack.type != AttackType::NONE; }

    void SetAttackHit() { currentAttack.hasHit = true; }
    void OnParrySuccessful(ParticleSystem& particles);

private:
    Vector2 position;
    Vector2 velocity;
    float radius;
    float facingAngle; // in degrees

    // Stats
    float health;
    float maxHealth;
    float stamina;
    float maxStamina;

    // Action Timers
    float lightCooldown;
    float heavyCooldown;
    float parryCooldown;
    float parryTimer;
    bool isParrying;
    float counterBuffTimer; // Buff after successful parry

    // Attack state
    AttackInfo currentAttack;
    float heavyWindupTimer;
    bool isHeavyWindingUp;

    // Movement animation
    float walkCycle;
};
