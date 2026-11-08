#pragma once
#include "raylib.h"
#include "raymath.h"
#include "GameState.hpp"
#include "../core/Config.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include "../combat/Projectile.hpp"
#include "../combat/LootDrop.hpp"
#include "../world/Campfire.hpp"
#include <vector>
#include <string>
#include <functional>

struct NorthMobSpawner {
    Vector2 spawnPos;
    float respawnTimer;
    bool isAlive;
    EnemyType type;
};

struct NorthLoreStele {
    std::string fragmentId;
    std::string title;
    std::string prompt;
    Vector2 position;
    float radius;
    bool collected;
};

struct FrostCrystalNode {
    Vector2 position;
    float health = 35.0f;
    float maxHealth = 35.0f;
    bool broken = false;
    float respawnTimer = 0.0f;
    float shakeTimer = 0.0f;
    float radius = 24.0f;
};

struct BlizzardSnowflake {
    Vector2 position;
    Vector2 velocity;
    float size;
    float alpha;
};

class NorthZyphdenHandler {
public:
    NorthZyphdenHandler();

    void Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera);

    void Update(
        float dt,
        Player& player,
        GameMap& map,
        ParticleSystem& particles,
        std::vector<Enemy>& enemies,
        std::vector<Projectile>& projectiles,
        std::vector<LootDrop>& lootDrops,
        Camera2D& camera,
        GameState& currentState,
        const std::function<void()>& onReturnToZyphden,
        const std::function<void()>& onEnterLobby
    );

    void Render(
        Camera2D& camera,
        const GameMap& map,
        const std::vector<Enemy>& enemies,
        const std::vector<Projectile>& projectiles,
        const std::vector<LootDrop>& lootDrops,
        const Player& player,
        const ParticleSystem& particles,
        int screenWidth,
        int screenHeight,
        bool showControlsModal,
        bool showInventoryModal,
        int inventoryActiveTab,
        int inventorySelectedFragment
    );

    bool IsBossSummoned() const { return berserkerTrollSummoned; }
    bool IsBossDefeated() const { return berserkerTrollDefeated; }
    void SetBossDefeated(bool defeated) { berserkerTrollDefeated = defeated; }
    void InitSpawners();
    void InitFrostCrystals();
    void InitBlizzardParticles();
    void Reset() {
        berserkerTrollSummoned = false;
        berserkerTrollDefeated = false;
        bossRematchCooldown = 0.0f;
    }

private:
    Campfire highlandCampfire;
    std::vector<NorthMobSpawner> spawners;
    std::vector<NorthLoreStele> loreSteles;
    std::vector<FrostCrystalNode> frostCrystals;
    std::vector<BlizzardSnowflake> blizzardSnowflakes;

    bool berserkerTrollSummoned;
    bool berserkerTrollDefeated;
    float bossRematchCooldown;
    float entryBannerTimer;
    float exitCooldown;
};
