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

struct ZyphdenNPC {
    std::string id;
    std::string name;
    Vector2 position;
    float radius;
    Color accentColor;

    bool CheckInteraction(Vector2 playerPos) const {
        return Vector2Distance(playerPos, position) <= (radius + 36.0f);
    }

    void Draw(float time) const;
};

class ZyphdenHandler {
public:
    ZyphdenHandler();

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
        bool& testArenaUnlocked,
        const std::function<void()>& onSaveArenaUnlock,
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

    bool HasDefeatedLenon() const { return lenonDefeated; }

private:
    // NPCs
    ZyphdenNPC eldon;
    ZyphdenNPC noticeboard;
    ZyphdenNPC kingKreptu;
    ZyphdenNPC knightLenonNPC;

    // Quest Progression State
    bool hasTalkedToKing = false;
    bool sparActive = false;
    bool lenonDefeated = false;

    // HUD & Banners
    bool showObjectives = true;
    float entryBannerTimer = 0.0f;

    // Dialogue Banner State
    float dialogueTimer = 0.0f;
    std::string dialogueSpeaker = "";
    std::string dialogueText = "";
    Color dialogueColor = WHITE;

    void SetDialogue(const std::string& speaker, const std::string& text, Color color, float duration = 5.0f);
};
