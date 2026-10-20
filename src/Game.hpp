#pragma once
#include "raylib.h"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Map.hpp"
#include "Particles.hpp"
#include "AssetManager.hpp"
#include "Storyboard.hpp"
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
    void RenderUI();

    void SpawnWave(int waveNumber);
    void CheckCombatCollisions();
    void ResetGame();

    Camera2D camera;
    Player player;
    GameMap map;
    ParticleSystem particles;
    AssetManager assetManager;
    StoryboardSystem storyboard;
    std::vector<Enemy> enemies;

    int currentWave;
    float waveTimer;
    bool isGameOver;
    bool isVictory;
};
