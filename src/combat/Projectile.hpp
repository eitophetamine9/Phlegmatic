#pragma once
#include "raylib.h"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"

enum class ProjectileType {
    BOLT,           // Archer's piercing bolt
    VOID_ORB,       // Pyromancer's occult orb
    NOVA_STAR,      // Boss radial star
    BEAM_SHARD,     // Boss focused beam / shard
    HOLY_BEAM,      // Truth Seeker piercing holy beam
    ELEMENTAL_BOLT, // Elemental Golem & Elemental Core Totem projectile
    GALE_ARROW,     // Gale Archer high-speed wind piercing projectile
    THROWING_DAGGER // Gale Bandit throwing knife
};

class Projectile {
public:
    Projectile(Vector2 pos, Vector2 vel, float radius, float damage, ProjectileType type, bool isFromPlayer = false, float defensePierce = 0.0f);

    void Update(float dt, const GameMap& map, ParticleSystem& particles);
    void Draw() const;

    bool IsActive() const { return active; }
    void Destroy() { active = false; }

    void Reflect(Vector2 newVel);

    Vector2 GetPosition() const { return position; }
    Vector2 GetVelocity() const { return velocity; }
    float GetRadius() const { return radius; }
    float GetDamage() const { return damage; }
    bool IsFromPlayer() const { return isFromPlayer; }
    ProjectileType GetType() const { return type; }
    float GetDefensePierce() const { return defensePierce; }
    void SetDefensePierce(float p) { defensePierce = p; }

    bool IsPiercing() const { return isPiercing; }
    void SetPiercing(bool p) { isPiercing = p; }
    bool HasHitEnemy(int id) const {
        for (int h : hitEnemyIds) if (h == id) return true;
        return false;
    }
    void RecordHitEnemy(int id) { hitEnemyIds.push_back(id); }

private:
    Vector2 position;
    Vector2 velocity;
    float radius;
    float damage;
    ProjectileType type;
    bool isFromPlayer;
    float defensePierce;
    bool active;
    float lifetime;
    float maxLifetime;
    Color color;
    bool isPiercing = false;
    std::vector<int> hitEnemyIds;
};
