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

    // Apex Boss Gauntlet
    void StartBossGauntlet(std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center = { 1200.0f, 900.0f });
    bool IsBossGauntlet() const { return isBossGauntletMode; }
    int GetGauntletStage() const { return gauntletStage; }
    int GetTotalGauntletStages() const { return totalGauntletStages; }
    void SpawnGauntletStage(int stage, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center = { 1200.0f, 900.0f });

private:
    int currentWave;
    float waveTimer;
    const int totalWaves = 25;

    bool isBossGauntletMode;
    int gauntletStage;
    const int totalGauntletStages = 3;
};
