#include "QuarryHandler.hpp"
#include "../combat/CollisionSystem.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/FontManager.hpp"
#include "UI.hpp"
#include "HUD.hpp"
#include <algorithm>
#include <cstdio>

QuarryHandler::QuarryHandler()
    : altarPosition{ 1000.0f, 320.0f },
      altarRadius(95.0f),
      elementalBossSummoned(false),
      elementalBossDefeated(false),
      entryBannerTimer(4.0f),
      doorCooldown(0.0f) {
}

void QuarryHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    map.Init(MapEnvironment::QUARRY_DEPTHS);
    player.SetPosition({ 280.0f, 750.0f });
    camera.target = player.GetPosition();

    enemies.clear();
    projectiles.clear();
    altarPosition = { 1000.0f, 320.0f };
    altarRadius = 95.0f;
    entryBannerTimer = 4.0f;
    doorCooldown = 0.5f;

    // 4 Ore Golem Spawners across the Quarry plateau
    golemSpawners.clear();
    golemSpawners.push_back({ { 600.0f, 550.0f }, 0.0f, true });
    golemSpawners.push_back({ { 1350.0f, 550.0f }, 0.0f, true });
    golemSpawners.push_back({ { 650.0f, 1000.0f }, 0.0f, true });
    golemSpawners.push_back({ { 1300.0f, 1000.0f }, 0.0f, true });

    for (const auto& spawner : golemSpawners) {
        enemies.emplace_back(EnemyType::ORE_GOLEM, spawner.spawnPos);
    }
}

void QuarryHandler::Update(
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
    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    if (entryBannerTimer > 0.0f) {
        entryBannerTimer -= dt;
    }

    if (doorCooldown > 0.0f) {
        doorCooldown -= dt;
    }

    // Player Death & Respawn
    if (!player.IsAlive()) {
        if (IsKeyPressed(KEY_R)) {
            player.Revive(Vector2{ 280.0f, 750.0f }, 1.0f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();
            elementalBossSummoned = false;

            // Re-spawn Ore Golems
            for (auto& spawner : golemSpawners) {
                spawner.isAlive = true;
                spawner.respawnTimer = 0.0f;
                enemies.emplace_back(EnemyType::ORE_GOLEM, spawner.spawnPos);
            }
            return;
        }
        if (IsKeyPressed(KEY_L)) {
            if (onEnterLobby) onEnterLobby();
            return;
        }
        return;
    }

    // Combat Controls
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        player.TriggerLightAttack(particles);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        player.TriggerHeavyAttack(particles, mouseWorld);
    }
    if (IsKeyPressed(KEY_Q)) {
        player.TriggerDash(particles);
    }
    if (IsKeyPressed(KEY_F)) {
        player.TriggerParry(particles);
    }
    if (IsKeyPressed(KEY_H)) {
        player.UseHealthPotion(particles);
    }

    player.Update(dt, map, mouseWorld, particles);

    // Flush player spawned projectiles (e.g. Elemental Bolt or Holy Beam)
    for (const auto& p : player.GetSpawnedProjectiles()) {
        projectiles.push_back(p);
    }
    player.GetSpawnedProjectiles().clear();

    // Update active enemies
    for (auto& enemy : enemies) {
        if (enemy.IsAlive()) {
            enemy.Update(dt, player.GetPosition(), map, particles, projectiles);
        }
    }

    // Update projectiles
    for (auto& proj : projectiles) {
        if (proj.IsActive()) {
            proj.Update(dt, map, particles);
        }
    }
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) {
        return !p.IsActive();
    }), projectiles.end());

    // Combat Collisions & Loot Drops
    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera, &lootDrops);

    // Update active loot drops
    for (auto& drop : lootDrops) {
        drop.Update(dt, player.GetPosition(), player, particles);
    }
    lootDrops.erase(std::remove_if(lootDrops.begin(), lootDrops.end(), [](const LootDrop& d) {
        return d.IsCollected();
    }), lootDrops.end());

    // Check dead enemies & update Ore Golem respawn timers
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!it->IsAlive()) {
            if (it->GetType() == EnemyType::ORE_GOLEM) {
                // Start 7-second respawn timer on closest spawner
                float bestDist = 999999.0f;
                int bestIdx = -1;
                for (size_t i = 0; i < golemSpawners.size(); ++i) {
                    if (golemSpawners[i].isAlive) {
                        float d = Vector2Distance(it->GetPosition(), golemSpawners[i].spawnPos);
                        if (d < bestDist) {
                            bestDist = d;
                            bestIdx = (int)i;
                        }
                    }
                }
                if (bestIdx != -1) {
                    golemSpawners[bestIdx].isAlive = false;
                    golemSpawners[bestIdx].respawnTimer = 7.0f;
                }
            } else if (it->GetType() == EnemyType::ELEMENTAL_GOLEM) {
                elementalBossDefeated = true;
                player.GetInventory().UnlockTotem(TotemType::ELEMENTAL_CORE_TOTEM);
                player.SetTotem(TotemType::ELEMENTAL_CORE_TOTEM);

                SoundFX::PlayGateOpen();
                SoundFX::PlayCoinPickup();
                particles.AddScreenShake(10.0f, 0.35f);
                particles.SpawnSparks(it->GetPosition(), Color{ 255, 215, 80, 255 }, 40, 240.0f);
                particles.SpawnFloatingText(player.GetPosition(), "ELEMENTAL CORE TOTEM UNLOCKED & ATTUNED!", Color{ 255, 220, 90, 255 }, 24.0f);
            }
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    // Countdown respawn timers and resurrect Ore Golems after exactly 7.0 seconds
    for (auto& spawner : golemSpawners) {
        if (!spawner.isAlive) {
            spawner.respawnTimer -= dt;
            if (spawner.respawnTimer <= 0.0f) {
                spawner.respawnTimer = 0.0f;
                spawner.isAlive = true;
                enemies.emplace_back(EnemyType::ORE_GOLEM, spawner.spawnPos);

                SoundFX::PlayHitImpact();
                particles.SpawnSparks(spawner.spawnPos, Color{ 255, 190, 50, 255 }, 25, 160.0f);
                particles.AddScreenShake(3.5f, 0.15f);
                particles.SpawnFloatingText(Vector2{ spawner.spawnPos.x, spawner.spawnPos.y - 25.0f }, "ORE GOLEM RISES", Color{ 255, 200, 70, 240 }, 18.0f);
            }
        }
    }

    // West Exit Archway to Zyphden at { 220.0f, 750.0f }
    bool interactKey = IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
    Vector2 playerPos = player.GetPosition();
    bool nearWestArch = Vector2Distance(playerPos, { 220.0f, 750.0f }) <= 85.0f || playerPos.x <= 225.0f;

    if (nearWestArch && (interactKey || playerPos.x <= 215.0f) && doorCooldown <= 0.0f) {
        SoundFX::PlayGateOpen();
        if (onReturnToZyphden) {
            onReturnToZyphden();
        }
        return;
    }

    // Primordial Elemental Altar Interaction at { 1000.0f, 320.0f }
    bool nearAltar = Vector2Distance(playerPos, altarPosition) <= altarRadius;
    if (nearAltar && interactKey) {
        if (elementalBossDefeated) {
            particles.SpawnFloatingText(playerPos, "The Primordial Altar is pacified. The elemental core hums quietly.", Color{ 140, 220, 255, 255 }, 18.0f);
            SoundFX::PlayParrySuccess();
        } else if (elementalBossSummoned) {
            particles.SpawnFloatingText(playerPos, "The Elemental Titan is already summoned! Vanquish the behemoth!", Color{ 255, 120, 120, 255 }, 20.0f);
            SoundFX::PlayHitImpact();
        } else {
            // Count unlocked lore fragments
            int unlockedCount = player.GetInventory().GetDiscoveredFragmentCount();

            if (unlockedCount < 10) {
                char buf[128];
                snprintf(buf, sizeof(buf), "The Primordial Altar demands all 10 Lore Fragments! (%d / 10 Collected)", unlockedCount);
                particles.SpawnFloatingText(playerPos, buf, Color{ 255, 120, 120, 255 }, 20.0f);
                particles.AddScreenShake(3.5f, 0.15f);
                SoundFX::PlayHitImpact();
            } else {
                // Awakening the Secret Boss: ELEMENTAL GOLEM!
                elementalBossSummoned = true;
                SoundFX::PlayGateOpen();
                SoundFX::PlayHeavyAttack();
                particles.AddScreenShake(12.0f, 0.45f);

                Vector2 bossSpawn = { 1000.0f, 430.0f };
                particles.SpawnSparks(bossSpawn, Color{ 255, 180, 50, 255 }, 40, 250.0f);
                particles.SpawnSparks(bossSpawn, Color{ 100, 220, 255, 255 }, 40, 250.0f);
                particles.SpawnSparks(bossSpawn, Color{ 200, 110, 255, 255 }, 40, 250.0f);
                particles.SpawnFloatingText(bossSpawn, "ELEMENTAL GOLEM AWAKENS!", Color{ 255, 220, 80, 255 }, 26.0f);

                enemies.emplace_back(EnemyType::ELEMENTAL_GOLEM, bossSpawn);
            }
        }
    }

    // Camera follow
    Vector2 shake = particles.GetScreenShakeOffset();
    Vector2 desiredTarget = { player.GetPosition().x + shake.x, player.GetPosition().y + shake.y };
    camera.target = Vector2Lerp(camera.target, desiredTarget, 8.0f * dt);
}

void QuarryHandler::Render(
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
    float time = (float)GetTime();

    BeginMode2D(camera);
    map.DrawFloor(player.GetPosition());

    // Enemy Telegraphs
    for (const auto& enemy : enemies) {
        enemy.DrawTelegraph();
    }

    map.DrawWallsAndPillars(player.GetPosition());

    // Projectiles
    for (const auto& proj : projectiles) {
        proj.Draw();
    }

    // Enemies
    for (const auto& enemy : enemies) {
        enemy.Draw();
    }

    // Loot drops
    for (const auto& drop : lootDrops) {
        drop.Draw(time);
    }

    if (player.IsAlive()) {
        player.Draw();
    }

    particles.Draw();

    // World Space Interactive Prompts
    // 1. West Exit Archway Prompt
    if (Vector2Distance(player.GetPosition(), { 240.0f, 750.0f }) <= 130.0f) {
        FontManager::DrawGameTextShadow("[E] RETURN TO ZYPHDEN COURTYARD", 140, 690, 12, Color{ 255, 220, 110, 250 });
    }

    // 2. Primordial Altar Prompt
    if (Vector2Distance(player.GetPosition(), altarPosition) <= 140.0f) {
        if (elementalBossDefeated) {
            FontManager::DrawGameTextShadow("[E] PRIMORDIAL ALTAR (PACIFIED)", 890, 205, 12, Color{ 140, 220, 255, 240 });
        } else if (elementalBossSummoned) {
            FontManager::DrawGameTextShadow("[E] PRIMORDIAL ALTAR (TITAN ENGAGED)", 880, 205, 12, Color{ 255, 110, 110, 240 });
        } else {
            int unlockedCount = player.GetInventory().GetDiscoveredFragmentCount();
            char buf[64];
            snprintf(buf, sizeof(buf), "[E] PRIMORDIAL ALTAR (%d/10 FRAGMENTS)", unlockedCount);
            FontManager::DrawGameTextShadow(buf, 860, 205, 12, (unlockedCount >= 10) ? Color{ 255, 215, 80, 255 } : Color{ 240, 140, 140, 240 });
        }
    }

    EndMode2D();

    // Mouse Reticle
    Vector2 mousePos = GetMousePosition();
    DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 255, 255, 255, 180 });
    DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 255, 195, 60, 240 });

    // Campaign HUD
    bool hasUnread = player.GetInventory().HasUnreadFragments();
    GameUI::DrawCampaignHUD(screenWidth, screenHeight, player, "QUARRY DEPTHS", hasUnread);

    // Boss Health Bar if Elemental Golem is summoned & alive
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::ELEMENTAL_GOLEM && enemy.IsAlive()) {
            float barW = 620.0f;
            float barH = 18.0f;
            float barX = ((float)screenWidth - barW) * 0.5f;
            float barY = 56.0f;

            // Backing card
            DrawRectangleRounded(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 12, 14, 20, 240 });
            DrawRectangleRoundedLines(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 225, 175, 55, 240 });

            // Title
            FontManager::DrawTextTitleCentered("ELEMENTAL GOLEM (PRIMORDIAL TITAN)", screenWidth / 2, (int)barY - 14, 15, Color{ 255, 220, 90, 255 });

            // Defense / Poise subtext
            char defBuf[64];
            snprintf(defBuf, sizeof(defBuf), "ARMOR: %d  |  PRIMORDIAL SHIELD: ACTIVE", (int)enemy.GetDefense());
            int dw = FontManager::MeasureText(defBuf, 10);
            FontManager::DrawGameTextShadow(defBuf, (screenWidth - dw) / 2, (int)barY + (int)barH + 4, 10, Color{ 180, 215, 255, 220 });

            // Health Bar
            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 22, 24, 34, 255 });
            float pct = fmaxf(0.0f, enemy.GetHealth() / enemy.GetMaxHealth());
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 225, 80, 45, 255 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 255, 215, 80, 220 });
            break;
        }
    }

    // Top Location Entry Banner
    if (entryBannerTimer > 0.0f) {
        float alpha = fminf(1.0f, entryBannerTimer);
        int bw = MeasureText("THE EASTERN QUARRY: BASALT DEPTHS", 22);
        int bx = (screenWidth - bw) / 2;
        int by = 68;
        DrawRectangle(bx - 24, by - 8, bw + 48, 40, Color{ 12, 16, 24, (unsigned char)(210 * alpha) });
        DrawRectangleLines(bx - 24, by - 8, bw + 48, 40, Color{ 225, 175, 65, (unsigned char)(240 * alpha) });
        DrawText("THE EASTERN QUARRY: BASALT DEPTHS", bx, by, 22, Color{ 255, 215, 80, (unsigned char)(255 * alpha) });
    }

    // Player Death Screen
    if (!player.IsAlive()) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{ 16, 6, 8, 220 });
        FontManager::DrawTextTitleCentered("FALLEN IN THE DEPTHS", screenWidth / 2, screenHeight / 2 - 40, 38, Color{ 255, 60, 60, 255 });
        FontManager::DrawTextCentered("Press [R] to Respawn at West Entrance  |  [L] Return to Lobby", screenWidth / 2, screenHeight / 2 + 18, 15, Color{ 220, 225, 240, 230 });
    }
}
