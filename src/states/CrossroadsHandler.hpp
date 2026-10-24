#pragma once
#include "raylib.h"
#include "raymath.h"
#include "GameState.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include "../combat/Projectile.hpp"
#include <vector>
#include <string>
#include <functional>

struct HiddenTotemChest {
    Vector2 position = { 510.0f, 950.0f };
    float radius = 32.0f;
    bool opened = false;

    bool CheckInteraction(Vector2 playerPos) const {
        return !opened && Vector2Distance(playerPos, position) <= (radius + 36.0f);
    }

    void Draw(float time) const;
};

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
        Camera2D& camera,
        GameState& currentState,
        const std::function<void()>& onEnterLobby
    );

    void Render(
        Camera2D& camera,
        const GameMap& map,
        const std::vector<Enemy>& enemies,
        const std::vector<Projectile>& projectiles,
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
    mutable bool showObjectives = true;
    bool enemiesSpawned = false;
    float entryBannerTimer = 4.0f;
};
