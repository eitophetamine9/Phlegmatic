#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include "../combat/Weapon.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"

class Player {
public:
    Player();

    void Init(Vector2 startPos);
    void Update(float dt, const GameMap& map, Vector2 mouseWorldPos, ParticleSystem& particles);
    void Draw() const;

    // Movement & Combat triggers
    void TriggerDash(ParticleSystem& particles);
    void TriggerLightAttack(ParticleSystem& particles);
    void TriggerHeavyAttack(ParticleSystem& particles);
    void TriggerParry(ParticleSystem& particles);

    // Damage handling
    bool TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles);
    void Heal(float amount);

    // Weapon equipment
    void SetWeapon(WeaponType newWeapon);
    WeaponType GetWeapon() const { return currentWeapon; }

    // Getters
    Vector2 GetPosition() const { return position; }
    float GetRadius() const { return radius; }
    float GetFacingAngle() const { return facingAngle; }
    float GetHealth() const { return health; }
    float GetMaxHealth() const { return maxHealth; }
    float GetStamina() const { return stamina; }
    float GetMaxStamina() const { return maxStamina; }
    bool IsParrying() const { return isParrying; }
    bool IsDashing() const { return isDashing; }
    bool IsAlive() const { return health > 0.0f; }
    bool HasCounterBuff() const { return counterBuffTimer > 0.0f; }

    float GetDashCooldown() const { return dashCooldown; }
    float GetParryCooldown() const { return parryCooldown; }

    const AttackInfo& GetCurrentAttack() const { return currentAttack; }
    bool IsAttacking() const { return currentAttack.type != AttackType::NONE; }

    void SetAttackHit() { currentAttack.hasHit = true; }
    void OnParrySuccessful(ParticleSystem& particles);

private:
    Vector2 position;
    Vector2 velocity;
    float radius;
    float facingAngle;

    WeaponType currentWeapon;

    // Stats
    float health;
    float maxHealth;
    float stamina;
    float maxStamina;

    // Action Timers & States
    float lightCooldown;
    float heavyCooldown;
    float parryCooldown;
    float parryTimer;
    bool isParrying;
    float counterBuffTimer;

    // Dash state (Q Key)
    float dashCooldown;
    float dashTimer;
    bool isDashing;
    Vector2 dashDir;

    // Attack state
    AttackInfo currentAttack;
    float heavyWindupTimer;
    bool isHeavyWindingUp;

    float walkCycle;
};
