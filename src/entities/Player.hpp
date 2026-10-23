#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include "../combat/Weapon.hpp"
#include "../combat/Inventory.hpp"
#include "../combat/Projectile.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include <vector>

class Player {
public:
    Player();

    void Init(Vector2 startPos);
    void Update(float dt, const GameMap& map, Vector2 mouseWorldPos, ParticleSystem& particles);
    void Draw() const;

    // Movement & Combat triggers
    void TriggerDash(ParticleSystem& particles);
    void TriggerLightAttack(ParticleSystem& particles);
    void TriggerHeavyAttack(ParticleSystem& particles, Vector2 targetPos = { 0.0f, 0.0f });
    void TriggerParry(ParticleSystem& particles);

    // Damage handling
    bool TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles, float defensePierce = 0.0f);
    void Heal(float amount);
    void Revive(Vector2 startPos, float hpPercent = 1.0f);

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
    float GetMaxDashCooldown() const { return GetWeaponStats(currentWeapon).dashCooldown; }
    float GetParryCooldown() const { return parryCooldown; }
    float GetHeavyCooldown() const { return heavyCooldown; }
    float GetMaxHeavyCooldown() const { return GetWeaponStats(currentWeapon).heavyCooldown; }

    const AttackInfo& GetCurrentAttack() const { return currentAttack; }
    bool IsAttacking() const { return currentAttack.type != AttackType::NONE; }

    void SetAttackHit() { currentAttack.hasHit = true; }
    void OnParrySuccessful(ParticleSystem& particles);

    float GetDefense() const { return defense; }

    int GetComboStep() const { return comboStep; }
    int GetHeavyStage() const { return heavyStage; }
    bool IsDaggerLunging() const { return isDaggerLunging; }
    bool IsShockwaveActive() const { return shockwaveActive; }
    Vector2 GetShockwavePos() const { return shockwavePos; }
    float GetShockwaveRadius() const { return shockwaveRadius; }
    Vector2 GetHeavyTargetPos() const { return heavyTargetPos; }
    bool IsHeavyWindingUp() const { return isHeavyWindingUp; }

    // Projectile management
    std::vector<Projectile>& GetSpawnedProjectiles() { return spawnedProjectiles; }
    const std::vector<Projectile>& GetSpawnedProjectiles() const { return spawnedProjectiles; }

    // Inventory & Lore
    Inventory& GetInventory() { return inventory; }
    const Inventory& GetInventory() const { return inventory; }

    friend class PlayerRenderer;

private:
    Vector2 position;
    Vector2 velocity;
    float radius;
    float facingAngle;

    WeaponType currentWeapon;
    Inventory inventory;

    // Stats
    float health;
    float maxHealth;
    float stamina;
    float maxStamina;
    float defense;

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

    // Daggers Lunge
    bool isDaggerLunging;
    float daggerLungeTimer;
    Vector2 daggerLungeDir;

    // Attack state
    AttackInfo currentAttack;
    float heavyWindupTimer;
    bool isHeavyWindingUp;
    Vector2 heavyTargetPos;
    int heavyStage;
    float heavyStageTimer;

    int comboStep;
    float comboTimer;

    // Shockwave AoE state
    bool shockwaveActive;
    float shockwaveTimer;
    float shockwaveRadius;
    Vector2 shockwavePos;

    // Spawned projectiles queue (for ranged weapon effects)
    std::vector<Projectile> spawnedProjectiles;

    float walkCycle;
};
