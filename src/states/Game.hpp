#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "UI.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
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

    void SpawnWave(int waveNumber);
    void CheckCombatCollisions();
    void ResetGame();

    GameState currentState;
    Camera2D camera;
    Player player;
    GameMap map;
    ParticleSystem particles;
    AssetManager assetManager;
    std::vector<Enemy> enemies;

    int currentWave;
    float waveTimer;
};
