#pragma once
#include "raylib.h"
#include "raymath.h"
#include "GameState.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include "../combat/Projectile.hpp"
#include "../combat/LootDrop.hpp"
#include <vector>
#include <string>
#include <functional>

struct QuarryGolemSpawner {
    Vector2 spawnPos;
    float respawnTimer;
    bool isAlive;
};

class QuarryHandler {
public:
    QuarryHandler();

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

    bool IsBossSummoned() const { return elementalBossSummoned; }
    bool IsBossDefeated() const { return elementalBossDefeated; }
    void SetBossDefeated(bool defeated) { elementalBossDefeated = defeated; }

private:
    std::vector<QuarryGolemSpawner> golemSpawners;
    Vector2 altarPosition;
    float altarRadius;
    bool elementalBossSummoned;
    bool elementalBossDefeated;
    float entryBannerTimer;
    float doorCooldown;
};
