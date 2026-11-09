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

struct FrostStalagmite {
    Vector2 position;
    float health = 30.0f;
    float maxHealth = 30.0f;
    bool broken = false;
    float respawnTimer = 0.0f;
    float shakeTimer = 0.0f;
    float radius = 22.0f;
};

struct CryptMobSpawner {
    Vector2 spawnPos;
    float respawnTimer;
    bool isAlive;
    EnemyType type;
};

class FrostCavernsHandler {
public:
    FrostCavernsHandler();

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
        const std::function<void()>& onReturnToHighlands,
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

    bool IsBossSummoned() const { return frostRevenantSummoned; }
    bool IsBossDefeated() const { return frostRevenantDefeated; }
    void SetBossDefeated(bool defeated) { frostRevenantDefeated = defeated; }
    void Reset() {
        frostRevenantSummoned = false;
        frostRevenantDefeated = false;
        stalagmites.clear();
        spawners.clear();
        entryBannerTimer = 0.0f;
        exitCooldown = 0.0f;
    }

private:
    Campfire campfire;
    std::vector<FrostStalagmite> stalagmites;
    std::vector<CryptMobSpawner> spawners;
    Vector2 sarcophagusPos;
    float sarcophagusRadius;
    Vector2 loreStelePos;
    float loreSteleRadius;
    bool steleCollected = false;
    bool frostRevenantSummoned;
    bool frostRevenantDefeated;
    float entryBannerTimer;
    float exitCooldown;

    void InitStalagmites();
    void InitSpawners();
};

