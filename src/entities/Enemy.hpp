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
    KNIGHT_LENON_BOSS// Knight Lenon: Vanguard Captain, Royal Greatsword Lunge, Golden Whirlwind
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
    float GetRadius() const { return radius; }
    EnemyType GetType() const { return type; }
    EnemyState GetState() const { return state; }
    bool IsAlive() const { return state != EnemyState::DEAD; }
    float GetHealth() const { return health; }
    float GetMaxHealth() const { return maxHealth; }
    float GetDefense() const { return defense; }
    float GetWill() const { return will; }
    TelegraphIndicator GetTelegraph() const { return telegraph; }

    bool IsBoss() const { return type == EnemyType::INQUISITOR_BOSS || type == EnemyType::ZIL_BOSS || type == EnemyType::KNIGHT_LENON_BOSS; }
    bool HasMeleeHitbox() const {
        if (type == EnemyType::ARCHER || type == EnemyType::PYROMANCER) return false;
        if (type == EnemyType::INQUISITOR_BOSS || type == EnemyType::ZIL_BOSS || type == EnemyType::KNIGHT_LENON_BOSS) {
            return isPerformingMeleeSmite;
        }
        return true; // SLASHER, BRUTE, CHARGER
    }
    std::string GetName() const;

    friend class EnemyRenderer;

private:
    EnemyType type;
    EnemyState state;
    bool isPerformingMeleeSmite;

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

    TelegraphIndicator telegraph;
};
