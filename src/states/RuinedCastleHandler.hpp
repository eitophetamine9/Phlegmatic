#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include "../combat/Projectile.hpp"
#include "../world/Interactable.hpp"
#include "Tutorial.hpp"
#include <vector>
#include <string>
#include <functional>

class RuinedCastleHandler {
public:
    RuinedCastleHandler();

    void Reset();

    void Update(
        float dt,
        Player& player,
        GameMap& map,
        ParticleSystem& particles,
        std::vector<Enemy>& enemies,
        std::vector<Projectile>& projectiles,
        TutorialManager& tutorial,
        Camera2D& camera,
        GameState& currentState,
        bool& testArenaUnlocked,
        const std::function<void()>& onSaveArenaUnlock,
        const std::function<void()>& onEnterLobby,
        const std::function<void()>& onEnterRuinedCastle
    );

    void Render(
        Camera2D& camera,
        const GameMap& map,
        const std::vector<Enemy>& enemies,
        const std::vector<Projectile>& projectiles,
        const Player& player,
        const ParticleSystem& particles,
        TutorialManager& tutorial,
        int screenWidth,
        int screenHeight,
        bool showControlsModal,
        bool showInventoryModal,
        int inventoryActiveTab,
        int inventorySelectedFragment
    );

    bool IsZilEncounterActive() const { return zilEncounterActive; }
    bool IsZilDefeated() const { return zilDefeated; }
    bool HasUsedZilRetry() const { return hasUsedZilRetry; }
    bool ShowZilDefeatModal() const { return showZilDefeatModal; }

private:
    bool zilEncounterActive;
    bool zilDefeated;
    bool zilSparedPlayer;
    bool hasUsedZilRetry;
    bool showZilDefeatModal;
    float zilDialogueTimer;
    std::string zilDialogueSpeaker;
    std::string zilDialogueText;
    Color zilDialogueColor;
    InscribedPillar castlePillar;
    RelicChest castleChest;
};
