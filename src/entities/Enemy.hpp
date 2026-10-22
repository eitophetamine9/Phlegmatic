#pragma once
#include "raylib.h"
#include "../combat/CombatTypes.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"

enum class EnemyType {
    SLASHER,  // Cone sector telegraph
    BRUTE,    // Circle AoE ground slam telegraph
    CHARGER   // Linear dash thrust telegraph
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

    void Update(float dt, Vector2 playerPos, const GameMap& map, ParticleSystem& particles);
    void Draw() const;
    void DrawTelegraph() const;

    bool IsInStrike() const { return state == EnemyState::STRIKE && !hasDamagedPlayer; }
    void MarkDamagedPlayer() { hasDamagedPlayer = true; }
    void OnParried(ParticleSystem& particles);

    bool TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles);
    bool CheckHit(const AttackInfo& attack) const;

    Vector2 GetPosition() const { return position; }
    float GetRadius() const { return radius; }
    EnemyType GetType() const { return type; }
    EnemyState GetState() const { return state; }
    bool IsAlive() const { return state != EnemyState::DEAD; }
    float GetHealth() const { return health; }
    float GetMaxHealth() const { return maxHealth; }
    TelegraphIndicator GetTelegraph() const { return telegraph; }

private:
    EnemyType type;
    EnemyState state;

    Vector2 position;
    Vector2 velocity;
    float radius;
    float facingAngle;

    float health;
    float maxHealth;
    float moveSpeed;

    float attackRange;
    float windupDuration;
    float strikeDuration;
    float recoveryDuration;
    float stunDuration;

    float stateTimer;
    bool hasDamagedPlayer;
    bool warnedSound;

    TelegraphIndicator telegraph;
};
