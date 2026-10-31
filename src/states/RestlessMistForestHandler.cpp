#include "RestlessMistForestHandler.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/FontManager.hpp"
#include <cmath>

RestlessMistForestHandler::RestlessMistForestHandler() {
    // 5 Gale Bandit Spawners spread across the woodland glades
    spawners = {
        { { 500.0f, 450.0f }, 0.0f, false },
        { { 850.0f, 380.0f }, 0.0f, false },
        { { 580.0f, 850.0f }, 0.0f, false },
        { { 950.0f, 920.0f }, 0.0f, false },
        { { 1450.0f, 650.0f }, 0.0f, false }
    };
}

void RestlessMistForestHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    map.Init(MapEnvironment::RESTLESS_MIST_FOREST);
    player.SetPosition({ 160.0f, 700.0f }); // Enters from eastern gate (facing west)
    camera.target = player.GetPosition();
    projectiles.clear();
    enemies.clear();

    // Spawn regular Gale Bandits at clearings
    for (auto& spawner : spawners) {
        enemies.emplace_back(EnemyType::GALE_BANDIT, spawner.spawnPos);
        spawner.isAlive = true;
        spawner.respawnTimer = 0.0f;
    }

    entryBannerTimer = 4.0f;
    gateCooldown = 1.0f;
    ambientMistTimer = 0.0f;
}

void RestlessMistForestHandler::Update(
    float dt,
    Player& player,
    GameMap& map,
    ParticleSystem& particles,
    std::vector<Enemy>& enemies,
    std::vector<Projectile>& projectiles,
    std::vector<LootDrop>& lootDrops,
    Camera2D& camera,
    GameState& currentState,
    const std::function<void()>& onReturnToZyphden,
    const std::function<void()>& onEnterLobby
) {
    if (entryBannerTimer > 0.0f) entryBannerTimer -= dt;
    if (gateCooldown > 0.0f) gateCooldown -= dt;

    // Ambient floating mist particles
    ambientMistTimer += dt;
    if (ambientMistTimer >= 0.18f) {
        ambientMistTimer = 0.0f;
        Vector2 mistPos = {
            player.GetPosition().x + (float)GetRandomValue(-600, 600),
            player.GetPosition().y + (float)GetRandomValue(-400, 400)
        };
        particles.SpawnSparks(mistPos, Color{ 100, 240, 215, 60 }, 3, 35.0f);
    }

    // Respawn regular Gale Bandits when killed (22-second respawn timer)
    for (size_t i = 0; i < spawners.size(); ++i) {
        bool alive = false;
        for (const auto& enemy : enemies) {
            if (enemy.GetType() == EnemyType::GALE_BANDIT && enemy.IsAlive()) {
                if (Vector2Distance(enemy.GetPosition(), spawners[i].spawnPos) < 260.0f) {
                    alive = true;
                    break;
                }
            }
        }
        spawners[i].isAlive = alive;

        if (!alive) {
            spawners[i].respawnTimer += dt;
            if (spawners[i].respawnTimer >= 22.0f) {
                spawners[i].respawnTimer = 0.0f;
                enemies.emplace_back(EnemyType::GALE_BANDIT, spawners[i].spawnPos);
                spawners[i].isAlive = true;
                particles.SpawnSparks(spawners[i].spawnPos, Color{ 90, 235, 215, 200 }, 15, 120.0f);
            }
        }
    }

    // Check Gale Archer Boss Defeat & 15% Totem of Mist drop
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::GALE_ARCHER_BOSS && !enemy.IsAlive() && galeArcherSummoned && !galeArcherDefeated) {
            galeArcherDefeated = true;
            galeArcherSummoned = false;
            int roll = GetRandomValue(1, 100);
            if (roll <= 15 && !player.GetInventory().HasTotem(TotemType::TOTEM_MIST)) {
                player.GetInventory().UnlockTotem(TotemType::TOTEM_MIST);
                particles.SpawnFloatingText(enemy.GetPosition(), "TOTEM OF MIST DISCOVERED! (15% DROP)", Color{ 90, 240, 220, 255 }, 24.0f);
                SoundFX::PlayParrySuccess();
            }
        }
    }

    // Altar of Whispers Interaction
    float distToAltar = Vector2Distance(player.GetPosition(), altarPosition);
    if (distToAltar <= 85.0f && !galeArcherSummoned) {
        if (IsKeyPressed(KEY_E)) {
            if (player.GetInventory().GetEssenceOfMist() >= 2) {
                player.GetInventory().SpendEssenceOfMist(2);
                galeArcherSummoned = true;
                galeArcherDefeated = false;

                // Spawn the Gale Archer Boss
                Vector2 bossPos = { altarPosition.x, altarPosition.y - 130.0f };
                enemies.emplace_back(EnemyType::GALE_ARCHER_BOSS, bossPos);

                particles.AddScreenShake(9.0f, 0.45f);
                particles.SpawnSparks(altarPosition, Color{ 110, 255, 230, 255 }, 45, 260.0f);
                particles.SpawnFloatingText(altarPosition, "GALE ARCHER HAS AWAKENED!", Color{ 110, 255, 230, 255 }, 26.0f);
                SoundFX::PlayCrusaderClash();
            } else {
                particles.SpawnFloatingText(player.GetPosition(), "Requires 2x Essence of Mist from Gale Bandits!", Color{ 255, 110, 110, 255 }, 18.0f);
                SoundFX::PlayGateOpen();
            }
        }
    }

    // Eastern Gateway Return to Zyphden
    Vector2 eastGatePos = { 100.0f, 700.0f };
    float distToEastGate = Vector2Distance(player.GetPosition(), eastGatePos);
    if (distToEastGate <= 75.0f && gateCooldown <= 0.0f) {
        if (IsKeyPressed(KEY_E)) {
            if (onReturnToZyphden) onReturnToZyphden();
            return;
        }
    }

    // Hotkey to return to Zyphden
    if (IsKeyPressed(KEY_T)) {
        if (onReturnToZyphden) onReturnToZyphden();
        return;
    }
}

void RestlessMistForestHandler::Render(
    Camera2D& camera,
    const GameMap& map,
    const std::vector<Enemy>& enemies,
    const std::vector<Projectile>& projectiles,
    const std::vector<LootDrop>& lootDrops,
    const Player& player,
    const ParticleSystem& particles,
    int screenWidth,
    int screenHeight,
    bool showControlsModal,
    bool showInventoryModal,
    int inventoryActiveTab,
    int inventorySelectedFragment
) {
    // Floating prompt above Altar of Whispers
    float distToAltar = Vector2Distance(player.GetPosition(), altarPosition);
    if (distToAltar <= 120.0f && !galeArcherSummoned) {
        int mistCount = player.GetInventory().GetEssenceOfMist();
        std::string altarMsg = "[E] ALTAR OF WHISPERS: Offer 2x Essence of Mist (You have: " + std::to_string(mistCount) + ")";
        FontManager::DrawGameTextShadow(altarMsg, (int)altarPosition.x - 170, (int)altarPosition.y - 75, 11,
            (mistCount >= 2) ? Color{ 110, 255, 230, 240 } : Color{ 255, 130, 130, 240 });
    }

    // Floating prompt at Eastern Gateway
    Vector2 eastGatePos = { 100.0f, 700.0f };
    float distToEastGate = Vector2Distance(player.GetPosition(), eastGatePos);
    if (distToEastGate <= 110.0f) {
        FontManager::DrawGameTextShadow("[E] RETURN TO ZYPHDEN REALM (EAST)", (int)eastGatePos.x - 40, (int)eastGatePos.y - 45, 11, Color{ 255, 225, 120, 240 });
    }

    // Realm Entry Banner
    if (entryBannerTimer > 0.0f) {
        float alpha = (entryBannerTimer > 3.0f) ? (4.0f - entryBannerTimer) : (entryBannerTimer / 3.0f);
        if (alpha > 1.0f) alpha = 1.0f;
        Color textCol = Color{ 110, 245, 225, (unsigned char)(255 * alpha) };
        Color subCol = Color{ 175, 215, 235, (unsigned char)(220 * alpha) };
        FontManager::DrawTextTitleCenteredShadow("RESTLESS MIST FOREST", screenWidth / 2, 70, 24, textCol);
        FontManager::DrawTextCenteredShadow("Haunt of the Gale Bandits & The Whispering Altar", screenWidth / 2, 102, 12, subCol);
    }
}

