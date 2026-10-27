#pragma once
#include "raylib.h"
#include "raymath.h"
#include "GameState.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include "../combat/Projectile.hpp"
#include "../world/Interactable.hpp"
#include <vector>
#include <string>
#include <functional>

struct HiddenTotemChest {
    Vector2 position = { 490.0f, 560.0f };
    float radius = 32.0f;
    bool opened = false;

    bool CheckInteraction(Vector2 playerPos) const {
        return !opened && Vector2Distance(playerPos, position) <= (radius + 36.0f);
    }

    void Draw(float time, Vector2 playerPos = { -9999.0f, -9999.0f }) const;
};

struct RoadsideJournal {
    Vector2 position = { 1120.0f, 980.0f };
    float radius = 24.0f;
    bool collected = false;

    bool CheckInteraction(Vector2 playerPos) const {
        return !collected && Vector2Distance(playerPos, position) <= (radius + 32.0f);
    }

    void Draw(float time, Vector2 playerPos = { -9999.0f, -9999.0f }) const;
};

class LootDrop;

class CrossroadsHandler {
public:
    CrossroadsHandler();

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

    bool HasClaimedTotemOfSanity() const { return chest.opened; }

private:
    HiddenTotemChest chest;
    RoadsideJournal journal;
    InscribedPillar crossroadsPillar;
    RelicChest crossroadsChest;
    mutable bool showObjectives = true;
    bool enemiesSpawned = false;
    float entryBannerTimer = 4.0f;
};
