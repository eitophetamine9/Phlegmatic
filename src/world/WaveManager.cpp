#include "WaveManager.hpp"
#include "../entities/Enemy.hpp"
#include "../combat/Projectile.hpp"
#include "../core/Particles.hpp"

WaveManager::WaveManager() : currentWave(1), waveTimer(0.0f), isBossGauntletMode(false), gauntletStage(0) {
}

void WaveManager::Reset() {
    currentWave = 1;
    waveTimer = 0.0f;
    isBossGauntletMode = false;
    gauntletStage = 0;
}

void WaveManager::SpawnWave(int waveNumber, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center) {
    enemies.clear();
    projectiles.clear();
    currentWave = waveNumber;
    waveTimer = 0.0f;

    switch (waveNumber) {
        // -------------------------------------------------------------
        // TIER 1: VANGUARD SKIRMISH & THE FIRST BOSS (WAVES 1 - 5)
        // -------------------------------------------------------------
        case 1:
            // Wave 1: Vanguard Skirmish (3 Slashers & 2 Archers)
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 260.0f, center.y - 100.0f });
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 260.0f, center.y - 100.0f });
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x, center.y - 180.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 380.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 380.0f, center.y + 160.0f });
            particles.SpawnFloatingText(center, "WAVE 1: VANGUARD SKIRMISH", Color{ 255, 220, 80, 255 }, 24.0f);
            break;

        case 2:
            // Wave 2: Flankers & Occult Pyromancers (2 Chargers & 3 Pyromancers)
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 360.0f, center.y });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 360.0f, center.y });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 220.0f, center.y - 200.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 220.0f, center.y - 200.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x, center.y + 220.0f });
            particles.SpawnFloatingText(center, "WAVE 2: FLANKERS & OCCULT PYROMANCERS", Color{ 100, 240, 255, 255 }, 24.0f);
            break;

        case 3:
            // Wave 3: The Armored Phalanx (2 Iron Brutes & 2 Archers)
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x - 220.0f, center.y - 140.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x + 220.0f, center.y - 140.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 350.0f, center.y + 180.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 350.0f, center.y + 180.0f });
            particles.SpawnFloatingText(center, "WAVE 3: THE ARMORED PHALANX", Color{ 255, 140, 50, 255 }, 24.0f);
            break;

        case 4:
            // Wave 4: Chaos Legion Elite (1 Brute, 2 Chargers, 2 Pyromancers, 2 Slashers)
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x, center.y - 240.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 340.0f, center.y - 50.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 340.0f, center.y - 50.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 250.0f, center.y + 180.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 250.0f, center.y + 180.0f });
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 120.0f, center.y + 200.0f });
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 120.0f, center.y + 200.0f });
            particles.SpawnFloatingText(center, "WAVE 4: CHAOS LEGION ELITE", Color{ 240, 80, 160, 255 }, 26.0f);
            break;

        case 5:
            // Wave 5: BOSS ROUND 1 - The Grand Inquisitor (Boss + 1 Iron Brute + 2 Pyromancers)
            enemies.emplace_back(EnemyType::INQUISITOR_BOSS, Vector2{ center.x, center.y - 250.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x, center.y + 160.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 300.0f, center.y });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 300.0f, center.y });
            particles.SpawnFloatingText(center, "BOSS ROUND 1: THE GRAND INQUISITOR", Color{ 255, 60, 60, 255 }, 28.0f);
            break;

        // -------------------------------------------------------------
        // TIER 2: GALE SKIRMISHERS & CORRUPTED CHAMPION (WAVES 6 - 10)
        // -------------------------------------------------------------
        case 6:
            // Wave 6: Mist Skirmishers (3 Gale Bandits & 2 Archers)
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 220.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 220.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x, center.y + 180.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 340.0f, center.y + 120.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 340.0f, center.y + 120.0f });
            particles.SpawnFloatingText(center, "WAVE 6: MIST SKIRMISHERS", Color{ 110, 245, 225, 255 }, 24.0f);
            break;

        case 7:
            // Wave 7: Heavy Iron & Swift Blades (2 Iron Brutes & 3 Gale Bandits)
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x - 240.0f, center.y });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x + 240.0f, center.y });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 180.0f, center.y - 180.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 180.0f, center.y - 180.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x, center.y + 200.0f });
            particles.SpawnFloatingText(center, "WAVE 7: HEAVY IRON & SWIFT BLADES", Color{ 245, 180, 70, 255 }, 24.0f);
            break;

        case 8:
            // Wave 8: Void Pyromancy & High Mobility (3 Pyromancers & 3 Gale Bandits)
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 260.0f, center.y - 140.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 260.0f, center.y - 140.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x, center.y + 220.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 200.0f, center.y + 100.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 200.0f, center.y + 100.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x, center.y - 200.0f });
            particles.SpawnFloatingText(center, "WAVE 8: VOID TEMPEST PHALANX", Color{ 180, 100, 255, 255 }, 24.0f);
            break;

        case 9:
            // Wave 9: The Charging Vanguard (3 Chargers, 2 Iron Brutes, 2 Gale Bandits)
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 320.0f, center.y - 80.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 320.0f, center.y - 80.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x, center.y - 240.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x - 180.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x + 180.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 300.0f, center.y + 180.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 300.0f, center.y + 180.0f });
            particles.SpawnFloatingText(center, "WAVE 9: CHARGING VANGUARD", Color{ 255, 140, 50, 255 }, 26.0f);
            break;

        case 10:
            // Wave 10: BOSS ROUND 2 - Zil's Remnant (Truth Seeker Duel + 2 Chargers + 2 Archers)
            enemies.emplace_back(EnemyType::ZIL_BOSS, Vector2{ center.x, center.y - 220.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 300.0f, center.y + 80.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 300.0f, center.y + 80.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 360.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 360.0f, center.y - 120.0f });
            particles.SpawnFloatingText(center, "BOSS ROUND 2: ZIL'S REMNANT", Color{ 255, 215, 80, 255 }, 28.0f);
            break;

        // -------------------------------------------------------------
        // TIER 3: PRIMAL EARTH & THE GALE ARCHER (WAVES 11 - 15)
        // -------------------------------------------------------------
        case 11:
            // Wave 11: Quarry Basalt Awakening (2 Ore Golems & 3 Gale Bandits)
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x - 220.0f, center.y - 100.0f });
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x + 220.0f, center.y - 100.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 280.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 280.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x, center.y + 220.0f });
            particles.SpawnFloatingText(center, "WAVE 11: QUARRY BASALT AWAKENING", Color{ 230, 160, 60, 255 }, 24.0f);
            break;

        case 12:
            // Wave 12: Heavy Artillery & Stone (2 Ore Golems, 2 Pyromancers, 2 Archers)
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x - 240.0f, center.y });
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x + 240.0f, center.y });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 320.0f, center.y - 180.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 320.0f, center.y - 180.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 280.0f, center.y + 180.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 280.0f, center.y + 180.0f });
            particles.SpawnFloatingText(center, "WAVE 12: OCCULT BASALT ARTILLERY", Color{ 200, 100, 240, 255 }, 24.0f);
            break;

        case 13:
            // Wave 13: The Monolith Bastion (3 Ore Golems & 2 Chargers)
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x - 250.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x + 250.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x, center.y + 200.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 340.0f, center.y });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 340.0f, center.y });
            particles.SpawnFloatingText(center, "WAVE 13: THE MONOLITH BASTION", Color{ 255, 175, 45, 255 }, 26.0f);
            break;

        case 14:
            // Wave 14: Wind & Stone Synergy (2 Ore Golems, 3 Gale Bandits, 2 Pyromancers)
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x - 200.0f, center.y - 150.0f });
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x + 200.0f, center.y - 150.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 300.0f, center.y });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 300.0f, center.y });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x, center.y - 250.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 220.0f, center.y + 180.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 220.0f, center.y + 180.0f });
            particles.SpawnFloatingText(center, "WAVE 14: WIND & STONE CATACLYSM", Color{ 120, 240, 210, 255 }, 26.0f);
            break;

        case 15:
            // Wave 15: BOSS ROUND 3 - The Gale Archer Boss (Gale Archer + 2 Gale Bandits + 2 Chargers)
            enemies.emplace_back(EnemyType::GALE_ARCHER_BOSS, Vector2{ center.x, center.y - 250.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 240.0f, center.y });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 240.0f, center.y });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 320.0f, center.y + 150.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 320.0f, center.y + 150.0f });
            particles.SpawnFloatingText(center, "BOSS ROUND 3: THE GALE ARCHER", Color{ 90, 245, 220, 255 }, 28.0f);
            break;

        // -------------------------------------------------------------
        // TIER 4: PRIMORDIAL CATACLYSM & THE FINAL TITAN (WAVES 16 - 20)
        // -------------------------------------------------------------
        case 16:
            // Wave 16: Living Shards (2 Mini Elemental Golems & 2 Iron Brutes)
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x - 200.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x + 200.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x - 260.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x + 260.0f, center.y + 160.0f });
            particles.SpawnFloatingText(center, "WAVE 16: LIVING VOLCANIC SHARDS", Color{ 255, 130, 40, 255 }, 24.0f);
            break;

        case 17:
            // Wave 17: Shard Snipers & Gale Blades (3 Mini Elemental Golems & 3 Gale Bandits)
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x - 260.0f, center.y - 140.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x + 260.0f, center.y - 140.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x, center.y + 220.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 200.0f, center.y + 80.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 200.0f, center.y + 80.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x, center.y - 220.0f });
            particles.SpawnFloatingText(center, "WAVE 17: SHARD SNIPERS & GALE STRIKES", Color{ 100, 245, 200, 255 }, 26.0f);
            break;

        case 18:
            // Wave 18: Primal Mayhem (2 Mini Golems, 2 Pyromancers, 2 Chargers, 2 Archers)
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x - 220.0f, center.y });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x + 220.0f, center.y });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 300.0f, center.y - 180.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 300.0f, center.y - 180.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 280.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 280.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x, center.y - 250.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x, center.y + 240.0f });
            particles.SpawnFloatingText(center, "WAVE 18: PRIMAL CHAOS MAYHEM", Color{ 255, 90, 160, 255 }, 26.0f);
            break;

        case 19:
            // Wave 19: Vanguard Cataclysm (3 Mini Golems, 2 Iron Brutes, 2 Gale Bandits)
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x - 240.0f, center.y - 100.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x + 240.0f, center.y - 100.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x, center.y - 220.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x - 200.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x + 200.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x - 340.0f, center.y });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x + 340.0f, center.y });
            particles.SpawnFloatingText(center, "WAVE 19: VANGUARD CATACLYSM", Color{ 255, 60, 60, 255 }, 28.0f);
            break;

        case 20:
            // Wave 20: BOSS ROUND 4 - The Primordial Elemental Golem Titan (Titan + 2 Mini Golems + 2 Pyromancers)
            enemies.emplace_back(EnemyType::ELEMENTAL_GOLEM, Vector2{ center.x, center.y - 220.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x - 240.0f, center.y + 140.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x + 240.0f, center.y + 140.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 320.0f, center.y });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 320.0f, center.y });
            particles.SpawnFloatingText(center, "BOSS ROUND 4: PRIMORDIAL TITAN", Color{ 255, 180, 40, 255 }, 30.0f);
            break;

        // -------------------------------------------------------------
        // TIER 5: FROSTBOUND HIGHLAND TRIALS (WAVES 21 - 25)
        // -------------------------------------------------------------
        case 21:
            // Wave 21: Highland Glacial Recon (2 Frost Trolls, 2 Slashers, 2 Gale Bandits)
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x - 240.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x + 240.0f, center.y - 120.0f });
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 320.0f, center.y + 150.0f });
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 320.0f, center.y + 150.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x, center.y + 220.0f });
            enemies.emplace_back(EnemyType::GALE_BANDIT, Vector2{ center.x, center.y - 240.0f });
            particles.SpawnFloatingText(center, "WAVE 21: HIGHLAND GLACIAL RECON", Color{ 140, 215, 255, 255 }, 26.0f);
            break;

        case 22:
            // Wave 22: Basalt & Crags Crush (2 Frost Trolls, 2 Unleashed Ore Golems, 2 Deathseeker Archers)
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x - 220.0f, center.y });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x + 220.0f, center.y });
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x - 260.0f, center.y - 160.0f });
            enemies.emplace_back(EnemyType::ORE_GOLEM, Vector2{ center.x + 260.0f, center.y - 160.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 300.0f, center.y + 200.0f });
            enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 300.0f, center.y + 200.0f });
            particles.SpawnFloatingText(center, "WAVE 22: CRAGS & BASALT CRUSH", Color{ 160, 220, 245, 255 }, 26.0f);
            break;

        case 23:
            // Wave 23: Frostfire Mayhem (3 Frost Trolls, 2 Void Pyromancers, 2 Chargers)
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x - 240.0f, center.y - 100.0f });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x + 240.0f, center.y - 100.0f });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x, center.y + 200.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 320.0f, center.y + 140.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 320.0f, center.y + 140.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 280.0f, center.y - 200.0f });
            enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 280.0f, center.y - 200.0f });
            particles.SpawnFloatingText(center, "WAVE 23: FROSTFIRE MAYHEM", Color{ 255, 110, 80, 255 }, 28.0f);
            break;

        case 24:
            // Wave 24: Crag Warlord Vanguard (3 Frost Trolls, 2 Mini Elemental Golems, 2 Iron Brutes)
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x - 260.0f, center.y });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x + 260.0f, center.y });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x, center.y - 220.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x - 200.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x + 200.0f, center.y + 160.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x - 340.0f, center.y - 140.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x + 340.0f, center.y - 140.0f });
            particles.SpawnFloatingText(center, "WAVE 24: WRATH OF THE HIGHLANDS", Color{ 220, 140, 255, 255 }, 28.0f);
            break;

        case 25:
        default:
            // Wave 25: APEX FINAL BOSS ROUND 5 - The Berserker Troll Warlord (+ 2 Frost Trolls + 2 Pyromancers)
            enemies.emplace_back(EnemyType::BERSERKER_TROLL, Vector2{ center.x, center.y - 220.0f });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x - 240.0f, center.y + 140.0f });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x + 240.0f, center.y + 140.0f });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 320.0f, center.y });
            enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 320.0f, center.y });
            particles.SpawnFloatingText(center, "APEX FINAL WAVE 25: CRAG BERSERKER WARLORD", Color{ 255, 45, 60, 255 }, 30.0f);
            break;
    }
}

void WaveManager::StartBossGauntlet(std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center) {
    isBossGauntletMode = true;
    gauntletStage = 1;
    waveTimer = 0.0f;
    SpawnGauntletStage(1, enemies, projectiles, particles, center);
}

void WaveManager::SpawnGauntletStage(int stage, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center) {
    enemies.clear();
    projectiles.clear();
    gauntletStage = stage;
    waveTimer = 0.0f;

    switch (stage) {
        case 1:
            // Stage 1: Remnant Zil (Holy Champion) + 2 Iron Brutes
            enemies.emplace_back(EnemyType::ZIL_BOSS, Vector2{ center.x, center.y - 180.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x - 240.0f, center.y + 120.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x + 240.0f, center.y + 120.0f });
            particles.SpawnFloatingText(center, "APEX STAGE I: REMNANT ZIL (CHAMPION)", Color{ 255, 220, 80, 255 }, 28.0f);
            particles.AddScreenShake(6.0f, 0.3f);
            break;

        case 2:
            // Stage 2: Primordial Elemental Golem Titan + 2 Mini Elemental Golems
            enemies.emplace_back(EnemyType::ELEMENTAL_GOLEM, Vector2{ center.x, center.y - 180.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x - 220.0f, center.y + 120.0f });
            enemies.emplace_back(EnemyType::MINI_ELEMENTAL_GOLEM, Vector2{ center.x + 220.0f, center.y + 120.0f });
            particles.SpawnFloatingText(center, "APEX STAGE II: PRIMORDIAL GOLEM TITAN", Color{ 255, 175, 50, 255 }, 28.0f);
            particles.AddScreenShake(7.0f, 0.35f);
            break;

        case 3:
        default:
            // Stage 3: The Berserker Troll Warlord + 2 Frost Trolls
            enemies.emplace_back(EnemyType::BERSERKER_TROLL, Vector2{ center.x, center.y - 180.0f });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x - 240.0f, center.y + 130.0f });
            enemies.emplace_back(EnemyType::FROST_TROLL, Vector2{ center.x + 240.0f, center.y + 130.0f });
            particles.SpawnFloatingText(center, "APEX FINAL STAGE: CRAG BERSERKER WARLORD", Color{ 255, 45, 60, 255 }, 30.0f);
            particles.AddScreenShake(8.0f, 0.4f);
            break;
    }
}

bool WaveManager::Update(float dt, int aliveCount, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Vector2 center, bool& outVictory) {
    outVictory = false;

    if (isBossGauntletMode) {
        if (aliveCount == 0) {
            waveTimer += dt;
            if (waveTimer >= 2.0f) {
                waveTimer = 0.0f;
                gauntletStage++;
                if (gauntletStage <= totalGauntletStages) {
                    SpawnGauntletStage(gauntletStage, enemies, projectiles, particles, center);
                    return true;
                } else {
                    outVictory = true;
                    return true;
                }
            }
        }
        return false;
    }

    if (aliveCount == 0) {
        waveTimer += dt;
        if (waveTimer >= 1.6f) {
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
