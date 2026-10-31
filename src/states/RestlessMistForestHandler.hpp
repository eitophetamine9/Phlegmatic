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
#include <vector>
#include <string>
#include <functional>

struct MistForestSpawner {
    Vector2 spawnPos;
    float respawnTimer;
    bool isAlive;
};

class RestlessMistForestHandler {
public:
    RestlessMistForestHandler();

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

    bool IsBossSummoned() const { return galeArcherSummoned; }
    bool IsBossDefeated() const { return galeArcherDefeated; }
    void SetBossDefeated(bool defeated) { galeArcherDefeated = defeated; }

private:
    std::vector<MistForestSpawner> spawners;
    bool galeArcherSummoned = false;
    bool galeArcherDefeated = false;
    float ambientMistTimer = 0.0f;
    float entryBannerTimer = 0.0f;
    float gateCooldown = 0.0f;
    Vector2 altarPosition = { 1130.0f, 650.0f };
};

