#include "WaveManager.hpp"
#include "../entities/Enemy.hpp"
#include "../combat/Projectile.hpp"
#include "../core/Particles.hpp"

WaveManager::WaveManager() : currentWave(1), waveTimer(0.0f) {
}

void WaveManager::Reset() {
    currentWave = 1;
    waveTimer = 0.0f;
}

void WaveManager::SpawnWave(int waveNumber, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center) {
    enemies.clear();
    projectiles.clear();
    currentWave = waveNumber;
    waveTimer = 0.0f;

    if (waveNumber == 1) {
        // Wave 1: Vanguard Skirmish (3 Slashers & 2 Archers)
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 260.0f, center.y - 100.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 260.0f, center.y - 100.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x, center.y - 180.0f });
        enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 380.0f, center.y + 160.0f });
        enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 380.0f, center.y + 160.0f });
        particles.SpawnFloatingText(center, "WAVE 1: VANGUARD SKIRMISH", Color{ 255, 220, 80, 255 }, 24.0f);
    } else if (waveNumber == 2) {
        // Wave 2: Ranged Artillery & Flankers (2 Chargers & 3 Pyromancers)
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 360.0f, center.y });
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 360.0f, center.y });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 220.0f, center.y - 200.0f });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 220.0f, center.y - 200.0f });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x, center.y + 220.0f });
        particles.SpawnFloatingText(center, "WAVE 2: FLANKERS & OCCULT PYROMANCERS", Color{ 100, 240, 255, 255 }, 24.0f);
    } else if (waveNumber == 3) {
        // Wave 3: The Armored Phalanx (2 Iron Brutes & 2 Archers)
        enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x - 220.0f, center.y - 140.0f });
        enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x + 220.0f, center.y - 140.0f });
        enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 350.0f, center.y + 180.0f });
        enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 350.0f, center.y + 180.0f });
        particles.SpawnFloatingText(center, "WAVE 3: THE ARMORED PHALANX", Color{ 255, 140, 50, 255 }, 24.0f);
    } else if (waveNumber == 4) {
        // Wave 4: Chaos Legion Elite (1 Brute, 2 Chargers, 2 Pyromancers, 2 Slashers)
        enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x, center.y - 240.0f });
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 340.0f, center.y - 50.0f });
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 340.0f, center.y - 50.0f });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 250.0f, center.y + 180.0f });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 250.0f, center.y + 180.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 120.0f, center.y + 200.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 120.0f, center.y + 200.0f });
        particles.SpawnFloatingText(center, "WAVE 4: CHAOS LEGION ELITE", Color{ 240, 80, 160, 255 }, 26.0f);
    } else if (waveNumber >= 5) {
        // Wave 5: The Grand Inquisitor Boss Trial (Boss + 1 Iron Brute Guard + 2 Pyromancers)
        enemies.emplace_back(EnemyType::INQUISITOR_BOSS, Vector2{ center.x, center.y - 250.0f });
        enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x, center.y + 160.0f });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 300.0f, center.y });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 300.0f, center.y });
        particles.SpawnFloatingText(center, "WAVE 5: THE GRAND INQUISITOR", Color{ 255, 60, 60, 255 }, 28.0f);
    }
}

bool WaveManager::Update(float dt, int aliveCount, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center, bool& outVictory) {
    outVictory = false;

    if (aliveCount == 0) {
        waveTimer += dt;
        if (waveTimer >= 1.5f) {
            waveTimer = 0.0f;
            currentWave++;
            if (currentWave <= totalWaves) {
                SpawnWave(currentWave, enemies, projectiles, particles, center);
                return true;
            } else {
                outVictory = true;
                return true;
            }
        }
    }
    return false;
}
