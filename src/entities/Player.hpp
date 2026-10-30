#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include "../combat/Weapon.hpp"
#include "../combat/Inventory.hpp"
#include "../combat/Projectile.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include <vector>
#include <cmath>

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

    // Weapon & Equipment
    void SetWeapon(WeaponType newWeapon);
    WeaponType GetWeapon() const { return currentWeapon; }

    void SetArmor(ArmorType newArmor);
    ArmorType GetArmor() const { return currentArmor; }

    void SetTotem(TotemType newTotem);
    TotemType GetTotem() const { return currentTotem; }

    // Position
    Vector2 GetPosition() const { return position; }
    void SetPosition(Vector2 pos) { position = pos; }
    float GetRadius() const { return radius; }
    float GetFacingAngle() const { return facingAngle; }
    float GetHealth() const { return health; }
    void SetHealth(float h) { health = fminf(h, GetMaxHealth()); }
    float GetMaxHealth() const;
    float GetStamina() const { return stamina; }
    float GetMaxStamina() const { return maxStamina; }
    bool IsParrying() const { return isParrying; }
    bool IsDashing() const { return isDashing; }
    bool IsAlive() const { return health > 0.0f; }
    bool HasCounterBuff() const { return counterBuffTimer > 0.0f; }

    WeaponStats GetEquippedWeaponStats() const { return GetWeaponStats(currentWeapon, inventory.GetWeaponUpgrade(currentWeapon)); }
    float GetDashCooldown() const { return dashCooldown; }
    float GetMaxDashCooldown() const { return GetEquippedWeaponStats().dashCooldown; }
    float GetParryCooldown() const { return parryCooldown; }
    float GetMaxParryCooldown() const;
    float GetHeavyCooldown() const { return heavyCooldown; }
    float GetMaxHeavyCooldown() const { return GetEquippedWeaponStats().heavyCooldown; }
    float GetElementalProjectileCooldown() const { return elementalProjectileCooldown; }

    const AttackInfo& GetCurrentAttack() const { return currentAttack; }
    bool IsAttacking() const { return currentAttack.type != AttackType::NONE; }

    void SetAttackHit() { currentAttack.hasHit = true; }
    void OnParrySuccessful(ParticleSystem& particles);

    float GetDefense() const;
    float GetWill() const;
    float GetSpeed() const;
    float GetStaminaRegenRate() const;

    // Stagger & Posture
    bool IsStaggered() const { return staggerTimer > 0.0f; }
    bool HasStaggerImmunity() const { return staggerImmunityTimer > 0.0f; }
    void ApplyStagger(float baseStunDuration, ParticleSystem& particles);
    float GetStaggerTimer() const { return staggerTimer; }

    // Invulnerability / I-Frames
    bool HasInvulnerability() const { return iFrameTimer > 0.0f; }
    float GetIFrameTimer() const { return iFrameTimer; }

    // Consumables: Potion
    bool UseHealthPotion(ParticleSystem& particles);
    float GetPotionCooldown() const { return potionCooldown; }
    float GetMaxPotionCooldown() const { return 7.0f; }

    // Cooldown accessors for UI
    float GetLightCooldown() const { return lightCooldown; }
    float GetMaxLightCooldown() const { return GetEquippedWeaponStats().lightCooldown; }

    int GetComboStep() const { return comboStep; }
    int GetHeavyStage() const { return heavyStage; }
    int GetHalberdStabCount() const { return halberdStabCount; }
    bool IsDaggerLunging() const { return isDaggerLunging; }
    bool IsShockwaveActive() const { return shockwaveActive; }
    Vector2 GetShockwavePos() const { return shockwavePos; }
    float GetShockwaveRadius() const { return shockwaveRadius; }
    Vector2 GetHeavyTargetPos() const { return heavyTargetPos; }
    bool IsHeavyWindingUp() const { return isHeavyWindingUp; }

    // Projectile management
    std::vector<Projectile>& GetSpawnedProjectiles() { return spawnedProjectiles; }
    const std::vector<Projectile>& GetSpawnedProjectiles() const { return spawnedProjectiles; }

    // Phlegmatic Composure System
    float GetComposure() const { return composure; }
    void SetComposure(float c) { composure = fmaxf(0.0f, fminf(100.0f, c)); }
    void ModifyComposure(float delta);
    float GetMaxComposure() const { return 100.0f; }

    bool IsInPhlegmaticTrance() const { return isInPhlegmaticTrance; }
    float GetTranceTimer() const { return tranceTimer; }
    bool TriggerPhlegmaticTrance(ParticleSystem& particles);

    // Passive Tiers
    bool IsDiscomposed() const { return !isInPhlegmaticTrance && composure < 25.0f; }
    bool IsComposed() const { return !isInPhlegmaticTrance && composure >= 25.0f && composure < 80.0f; }
    bool HasStoicPoise() const { return isInPhlegmaticTrance || (composure >= 80.0f); }
    bool IsTranceReady() const { return !isInPhlegmaticTrance && composure >= 100.0f; }

    // Humoral Alignment & Bleed Status
    HumorType GetHumor() const { return HumorType::PHLEGMATIC; }
    bool IsBleeding() const { return bleedTimer > 0.0f; }
    float GetBleedTimer() const { return bleedTimer; }
    void ApplyBleed(float duration, ParticleSystem& particles);

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
    ArmorType currentArmor;
    TotemType currentTotem;
    Inventory inventory;

    // Phlegmatic Composure & Trance State
    float composure;
    bool isInPhlegmaticTrance;
    float tranceTimer;

    // Stats
    float health;
    float maxHealth;
    float stamina;
    float maxStamina;
    float defense;
    float timeSinceCombat;
    float will;

    // Action Timers & States
    float lightCooldown;
    float heavyCooldown;
    float parryCooldown;
    float parryTimer;
    bool isParrying;
    float counterBuffTimer;
    float potionCooldown;
    float elementalProjectileCooldown;

    // Stagger & Poise State
    float staggerTimer;
    float staggerImmunityTimer;
    float bleedTimer;
    float bleedTickTimer;

    // Dash & I-Frames state (Q Key)
    float dashCooldown;
    float dashTimer;
    bool isDashing;
    Vector2 dashDir;
    float iFrameTimer;

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
    int halberdStabCount;

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
