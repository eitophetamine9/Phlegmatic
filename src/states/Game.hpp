#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "UI.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../world/Interactable.hpp"
#include "../core/Particles.hpp"
#include "../rendering/AssetManager.hpp"
#include "../combat/Projectile.hpp"
#include "Cutscene.hpp"
#include "Tutorial.hpp"
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    void Init();
    void Update(float dt);
    void Render();

    void EnterRuinedCastle();
    void EnterLobby();
    void EnterArena();
    void SpawnWave(int waveNumber);
    void CheckCombatCollisions();

    GameState currentState;
    bool showControlsModal;
    bool showInventoryModal;
    int inventoryActiveTab;
    int inventorySelectedFragment;

    Camera2D camera;
    Player player;
    GameMap map;
    ParticleSystem particles;
    AssetManager assetManager;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    // Lobby Elements
    std::vector<WeaponPedestal> pedestals;
    TrainingDummy dummy;
    ArenaPortal portal;

    // Narrative & Cutscenes
    Cutscene cutscene;
    TutorialManager tutorial;

    int currentWave;
    float waveTimer;

    // Zil Boss Encounter & Branching Logic
    bool zilEncounterActive;
    bool zilDefeated;
    bool zilSparedPlayer;
    bool hasUsedZilRetry;
    bool showZilDefeatModal;
    float zilDialogueTimer;
    std::string zilDialogueSpeaker;
    std::string zilDialogueText;
    Color zilDialogueColor;
};
