#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include "../combat/Projectile.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include <vector>
#include <string>

enum class EnemyType {
    SLASHER,         // Cone sector telegraph (Melee)
    BRUTE,           // Circle AoE ground slam telegraph (Heavy Melee)
    CHARGER,         // Linear dash thrust telegraph (Dash Melee)
    ARCHER,          // Laser aim line telegraph, piercing bolt (Ranged)
    PYROMANCER,      // Fan spread telegraph, 3 void orbs (Caster)
    INQUISITOR_BOSS, // Grand Boss: Nova, Beam, Teleport Smite
    ZIL_BOSS,        // Zil's Fragment (1% Power): Truth Seeker upward slice, 360 dawnfire, holy beam
    KNIGHT_LENON_BOSS,// Knight Lenon: Vanguard Captain, Royal Greatsword Lunge, Golden Whirlwind
    ORE_GOLEM,       // Quarry Ore Golem: Heavy stone chassis, ground seismic slam, drops Ore Scraps
    ELEMENTAL_GOLEM, // Secret Boss: Primordial Titan, quad-elemental shocks, fires elemental bolts
    MINI_ELEMENTAL_GOLEM // Compact Elemental Golem minion summoned by Titan or player totem
};

enum class EnemyState {
    IDLE_CHASE,
    TELEGRAPH_WINDUP,
    STRIKE,
    RECOVERY,
    STUNNED,
    DEAD
};

class Enemy {
public:
    Enemy(EnemyType type, Vector2 startPos);

    void Update(float dt, Vector2 playerPos, const GameMap& map, ParticleSystem& particles, std::vector<Projectile>& outProjectiles);
    void Draw() const;
    void DrawTelegraph() const;

    bool IsInStrike() const { return state == EnemyState::STRIKE && !hasDamagedPlayer; }
    bool IsTelegraphing() const { return state == EnemyState::TELEGRAPH_WINDUP; }
    void MarkDamagedPlayer() { hasDamagedPlayer = true; }
    void OnParried(ParticleSystem& particles);

    bool TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles, float defensePierce = 0.0f);
    bool CheckHit(const AttackInfo& attack) const;

    Vector2 GetPosition() const { return position; }
    void SetPosition(Vector2 p) { position = p; }
    void ApplyStagger(float duration) {
        state = EnemyState::STUNNED;
        stunDuration = duration;
    }
    float GetRadius() const { return radius; }
    float GetFacingAngle() const { return facingAngle; }
    EnemyType GetType() const { return type; }
    EnemyState GetState() const { return state; }
    bool IsAlive() const { return state != EnemyState::DEAD; }
    float GetHealth() const { return health; }
    float GetMaxHealth() const { return maxHealth; }
    float GetDefense() const { return defense; }
    float GetWill() const { return will; }
    TelegraphIndicator GetTelegraph() const { return telegraph; }
    bool NeedsMinionSpawn() const { return pendingMinionSummon; }
    void ClearMinionSpawn() { pendingMinionSummon = false; }

    // Weapon Heavy Attack debuffs
    void ApplyBleed(float duration) { bleedTimer = duration; }
    bool IsBleeding() const { return bleedTimer > 0.0f; }
    void ApplyArmorFracture(float duration) { armorFractureTimer = duration; }
    bool HasArmorFracture() const { return armorFractureTimer > 0.0f; }
    void ApplySolarBurn(float duration) { solarBurnTimer = duration; }
    bool HasSolarBurn() const { return solarBurnTimer > 0.0f; }

    bool IsBoss() const { return type == EnemyType::INQUISITOR_BOSS || type == EnemyType::ZIL_BOSS || type == EnemyType::KNIGHT_LENON_BOSS || type == EnemyType::ELEMENTAL_GOLEM; }
    bool HasMeleeHitbox() const {
        if (type == EnemyType::ARCHER || type == EnemyType::PYROMANCER) return false;
        if (type == EnemyType::INQUISITOR_BOSS || type == EnemyType::ZIL_BOSS || type == EnemyType::KNIGHT_LENON_BOSS || type == EnemyType::ELEMENTAL_GOLEM) {
            return isPerformingMeleeSmite;
        }
        return true; // SLASHER, BRUTE, CHARGER, ORE_GOLEM
    }
    std::string GetName() const;

    // Humoral Alignment
    HumorType GetHumor() const { return humor; }
    std::string GetHumorName() const;
    Color GetHumorColor() const;
    bool IsQuenched() const { return quenchedTimer > 0.0f; }
    bool IsShattered() const { return shatteredTimer > 0.0f; }
    void Quench(ParticleSystem& particles);
    void ShatterShell(ParticleSystem& particles);

    friend class EnemyRenderer;

private:
    EnemyType type;
    HumorType humor;
    EnemyState state;
    bool isPerformingMeleeSmite;
    float quenchedTimer;
    float shatteredTimer;

    // Status effect debuff timers
    float bleedTimer = 0.0f;
    float bleedTickTimer = 0.0f;
    float armorFractureTimer = 0.0f;
    float solarBurnTimer = 0.0f;
    float solarBurnTickTimer = 0.0f;

    Vector2 spawnPoint;
    Vector2 position;
    Vector2 velocity;
    float radius;
    float facingAngle;

    float health;
    float maxHealth;
    float defense;
    float will;
    float moveSpeed;

    float attackRange;
    float kitingRange;
    float windupDuration;
    float strikeDuration;
    float recoveryDuration;
    float stunDuration;

    float stateTimer;
    bool hasDamagedPlayer;
    bool warnedSound;

    // Boss & Ranged mechanics
    int bossAttackPhase;
    int burstShotsRemaining;
    float burstIntervalTimer;
    float summonCooldown = 2.0f;
    float miniGolemRangedTimer = 7.0f;
    bool pendingMinionSummon = false;

    TelegraphIndicator telegraph;
};
