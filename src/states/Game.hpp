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

    void EnterLobby();
    void EnterArena();
    void SpawnWave(int waveNumber);
    void CheckCombatCollisions();

    GameState currentState;
    bool showControlsModal;

    Camera2D camera;
    Player player;
    GameMap map;
    ParticleSystem particles;
    AssetManager assetManager;
    std::vector<Enemy> enemies;

    // Lobby Elements
    std::vector<WeaponPedestal> pedestals;
    TrainingDummy dummy;
    ArenaPortal portal;

    int currentWave;
    float waveTimer;
};
