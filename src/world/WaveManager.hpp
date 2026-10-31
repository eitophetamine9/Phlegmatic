#pragma once
#include "raylib.h"
#include <vector>

class Enemy;
class Projectile;
class ParticleSystem;

class WaveManager {
public:
    WaveManager();

    void Reset();
    void SpawnWave(int waveNumber, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center = { 1200.0f, 900.0f });
    
    // Returns true if wave changed or victory achieved
    bool Update(float dt, int aliveCount, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center, bool& outVictory);

    int GetCurrentWave() const { return currentWave; }
    int GetTotalWaves() const { return totalWaves; }

private:
    int currentWave;
    float waveTimer;
    const int totalWaves = 20;
};
