#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include "../combat/Projectile.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include <vector>
#include <string>

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
        stateTimer = duration;
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
    void ApplyChill(float duration) { if (duration > chillTimer) chillTimer = duration; }
    bool IsChilled() const { return chillTimer > 0.0f; }

    bool IsBoss() const { return type == EnemyType::INQUISITOR_BOSS || type == EnemyType::ZIL_BOSS || type == EnemyType::KNIGHT_LENON_BOSS || type == EnemyType::ELEMENTAL_GOLEM || type == EnemyType::BERSERKER_TROLL || type == EnemyType::GALE_ARCHER_BOSS || type == EnemyType::FROST_REVENANT; }
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

    void SetLeashed(bool leashed) { isLeashed = leashed; }
    bool IsLeashed() const { return isLeashed; }

    bool IsNightfallElite() const { return isNightfallElite; }
    void SetNightfallElite(bool elite);

    bool IsApexBoss() const { return isApexBoss; }
    void SetApexBoss(bool apex);

    friend class EnemyRenderer;

private:
    EnemyType type;
    HumorType humor;
    EnemyState state;
    bool isPerformingMeleeSmite;
    bool isNightfallElite = false;
    bool isApexBoss = false;
    float quenchedTimer;
    float shatteredTimer;

    // Status effect debuff timers
    float bleedTimer = 0.0f;
    float bleedTickTimer = 0.0f;
    float armorFractureTimer = 0.0f;
    float solarBurnTimer = 0.0f;
    float solarBurnTickTimer = 0.0f;
    float chillTimer = 0.0f;

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
    bool isLeashed = false;

    // Boss & Ranged mechanics
    int bossAttackPhase;
    int burstShotsRemaining;
    float burstIntervalTimer;
    float summonCooldown = 2.0f;
    float miniGolemRangedTimer = 7.0f;
    float galeDaggerTimer = 4.0f;
    float galeVaultTimer = 5.0f;
    bool pendingMinionSummon = false;

    TelegraphIndicator telegraph;
};
