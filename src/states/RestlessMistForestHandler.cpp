#include "RestlessMistForestHandler.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/FontManager.hpp"
#include "../combat/CollisionSystem.hpp"
#include "UI.hpp"
#include <cmath>
#include <algorithm>

void RestlessMistForestHandler::InitSpawners() {
    campfire = { 5, "Restless Glade Bonfire", { 540.0f, 640.0f } };
    spawners = {
        { { 500.0f, 450.0f }, 0.0f, true },
        { { 850.0f, 380.0f }, 0.0f, true },
        { { 580.0f, 850.0f }, 0.0f, true },
        { { 950.0f, 920.0f }, 0.0f, true },
        { { 1450.0f, 650.0f }, 0.0f, true }
    };
}

RestlessMistForestHandler::RestlessMistForestHandler() {
    InitSpawners();
}

void RestlessMistForestHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    map.Init(MapEnvironment::RESTLESS_MIST_FOREST);
    player.SetPosition({ 180.0f, 700.0f }); // Enters from eastern gate (facing west into the forest)
    camera.target = player.GetPosition();
    projectiles.clear();
    enemies.clear();

    InitSpawners();

    // Spawn regular Gale Bandits at clearings
    for (auto& spawner : spawners) {
        enemies.emplace_back(EnemyType::GALE_BANDIT, spawner.spawnPos);
        spawner.isAlive = true;
        spawner.respawnTimer = 0.0f;
    }

    galeArcherSummoned = false;
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

    // Player Death & Respawn
    if (!player.IsAlive()) {
        if (IsKeyPressed(KEY_R)) {
            player.Revive({ 180.0f, 700.0f });
            camera.target = player.GetPosition();
            enemies.clear();
            InitSpawners();
            for (auto& spawner : spawners) {
                enemies.emplace_back(EnemyType::GALE_BANDIT, spawner.spawnPos);
                spawner.isAlive = true;
                spawner.respawnTimer = 0.0f;
            }
            galeArcherSummoned = false;
        }
        if (IsKeyPressed(KEY_L) || IsKeyPressed(KEY_T)) {
            if (onReturnToZyphden) onReturnToZyphden();
        }
        return;
    }

    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    // Player Combat Inputs
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
    if (IsKeyPressed(KEY_H) || IsKeyPressed(KEY_ONE)) {
        player.UseHealthPotion(particles);
    }
    if (IsKeyPressed(KEY_V)) {
        player.TriggerArmorSkill(particles);
    }

    // Update Player Physics and Animation
    player.Update(dt, map, mouseWorld, particles);
    campfire.Update(player, particles);

    // Flush any projectiles spawned by the player
    for (const auto& p : player.GetSpawnedProjectiles()) {
        projectiles.push_back(p);
    }
    player.GetSpawnedProjectiles().clear();

    // Update active enemies
    for (auto& enemy : enemies) {
        if (enemy.IsAlive()) {
            enemy.Update(dt, player.GetTargetPosition(), map, particles, projectiles);
        }
    }

    // Update active projectiles
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

    // Ambient floating mist particles
    ambientMistTimer += dt;
    if (ambientMistTimer >= 0.16f) {
        ambientMistTimer = 0.0f;
        Vector2 mistPos = {
            player.GetPosition().x + (float)GetRandomValue(-600, 600),
            player.GetPosition().y + (float)GetRandomValue(-400, 400)
        };
        particles.SpawnSparks(mistPos, Color{ 100, 240, 215, 60 }, 3, 35.0f);
    }

    // Check dead enemies, boss defeat, and spawner cooldowns
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!it->IsAlive()) {
            int kIdx = -1;
            if (it->GetType() == EnemyType::GALE_BANDIT) kIdx = 7;
            else if (it->GetType() == EnemyType::GALE_ARCHER_BOSS) kIdx = 8;
            if (kIdx >= 0) player.GetInventory().RecordEnemyKill(kIdx);
            if (it->GetType() == EnemyType::GALE_ARCHER_BOSS) {
                if (galeArcherSummoned && !galeArcherDefeated) {
                    galeArcherDefeated = true;
                    galeArcherSummoned = false;
                    int roll = GetRandomValue(1, 100);
                    if (roll <= 15 && !player.GetInventory().HasTotem(TotemType::TOTEM_MIST)) {
                        player.GetInventory().UnlockTotem(TotemType::TOTEM_MIST);
                        particles.SpawnFloatingText(it->GetPosition(), "TOTEM OF MIST DISCOVERED! (15% DROP)", Color{ 90, 240, 220, 255 }, 24.0f);
                        SoundFX::PlayParrySuccess();
                    } else {
                        particles.SpawnFloatingText(player.GetPosition(), "GALE ARCHER VANQUISHED!", Color{ 110, 245, 225, 255 }, 24.0f);
                    }
                }
            } else if (it->GetType() == EnemyType::GALE_BANDIT) {
                float bestDist = 999999.0f;
                int bestIdx = -1;
                for (size_t i = 0; i < spawners.size(); ++i) {
                    if (spawners[i].isAlive) {
                        float d = Vector2Distance(it->GetPosition(), spawners[i].spawnPos);
                        if (d < bestDist) {
                            bestDist = d;
                            bestIdx = (int)i;
                        }
                    }
                }
                if (bestIdx != -1) {
                    spawners[bestIdx].isAlive = false;
                    spawners[bestIdx].respawnTimer = 22.0f;
                }
            }
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    // Respawn regular Gale Bandits when killed (22-second respawn timer)
    for (auto& spawner : spawners) {
        if (!spawner.isAlive) {
            spawner.respawnTimer -= dt;
            if (spawner.respawnTimer <= 0.0f) {
                spawner.respawnTimer = 0.0f;
                spawner.isAlive = true;
                enemies.emplace_back(EnemyType::GALE_BANDIT, spawner.spawnPos);
                particles.SpawnSparks(spawner.spawnPos, Color{ 90, 235, 215, 200 }, 15, 120.0f);
            }
        }
    }

    // Check if Gale Archer is currently alive in the arena
    bool galeArcherActive = false;
    for (const auto& en : enemies) {
        if (en.GetType() == EnemyType::GALE_ARCHER_BOSS && en.IsAlive()) {
            galeArcherActive = true;
            break;
        }
    }
    if (!galeArcherActive) {
        galeArcherSummoned = false;
    }

    // Altar of Whispers Interaction
    // The altar obstacle is a 160x140 plinth centered at { 1130.0f, 650.0f }.
    // We check an expanded bounding box and radius (165.0f) so interaction
    // works reliably from any direction (North, South, East, West, and all corners).
    Rectangle altarPlinth = { 1050.0f, 580.0f, 160.0f, 140.0f };
    Rectangle altarInteractRec = { altarPlinth.x - 55.0f, altarPlinth.y - 55.0f, altarPlinth.width + 110.0f, altarPlinth.height + 110.0f };
    float distToAltar = Vector2Distance(player.GetPosition(), altarPosition);
    bool nearAltar = CheckCollisionPointRec(player.GetPosition(), altarInteractRec) || (distToAltar <= 165.0f);
    bool interactKey = IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);

    if (nearAltar && interactKey) {
        if (galeArcherActive) {
            particles.SpawnFloatingText(altarPosition, "Gale Archer is already hunting you!", Color{ 255, 120, 120, 255 }, 20.0f);
            SoundFX::PlayHitImpact();
        } else {
            int mistCount = player.GetInventory().GetEssenceOfMist();
            if (mistCount >= 2) {
                player.GetInventory().SpendEssenceOfMist(2);
                galeArcherSummoned = true;
                galeArcherDefeated = false;

                // Spawn the Gale Archer Boss 140px north of the altar in the clearing
                Vector2 bossPos = { altarPosition.x, altarPosition.y - 140.0f };
                enemies.emplace_back(EnemyType::GALE_ARCHER_BOSS, bossPos);

                particles.AddScreenShake(9.0f, 0.45f);
                particles.SpawnSparks(altarPosition, Color{ 110, 255, 230, 255 }, 45, 260.0f);
                particles.SpawnFloatingText(altarPosition, "GALE ARCHER HAS AWAKENED!", Color{ 110, 255, 230, 255 }, 26.0f);
                SoundFX::PlayCrusaderClash();
            } else {
                std::string warn = "Requires 2x Essence of Mist from Gale Bandits! (" + std::to_string(mistCount) + " / 2)";
                particles.SpawnFloatingText(player.GetPosition(), warn, Color{ 255, 110, 110, 255 }, 18.0f);
                SoundFX::PlayHitImpact();
            }
        }
    }

    // Eastern Gateway Return to Zyphden
    Vector2 eastGatePos = { 100.0f, 700.0f };
    float distToEastGate = Vector2Distance(player.GetPosition(), eastGatePos);
    if ((distToEastGate <= 85.0f || player.GetPosition().x <= 110.0f) && gateCooldown <= 0.0f) {
        if (IsKeyPressed(KEY_E) || player.GetPosition().x <= 95.0f) {
            if (onReturnToZyphden) onReturnToZyphden();
            return;
        }
    }

    // Hotkey to return to Zyphden
    if (IsKeyPressed(KEY_T)) {
        if (onReturnToZyphden) onReturnToZyphden();
        return;
    }

    // Camera follow player with trauma screen shake
    Vector2 shake = particles.GetScreenShakeOffset();
    Vector2 desiredTarget = { player.GetPosition().x + shake.x, player.GetPosition().y + shake.y };
    camera.target = Vector2Lerp(camera.target, desiredTarget, 8.0f * dt);
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
    campfire.Draw(time, player);

    // Floating prompt and runic beacon above Altar of Whispers
    Rectangle altarPlinth = { 1050.0f, 580.0f, 160.0f, 140.0f };
    Rectangle altarInteractRec = { altarPlinth.x - 55.0f, altarPlinth.y - 55.0f, altarPlinth.width + 110.0f, altarPlinth.height + 110.0f };
    float distToAltar = Vector2Distance(player.GetPosition(), altarPosition);
    bool nearAltar = CheckCollisionPointRec(player.GetPosition(), altarInteractRec) || (distToAltar <= 165.0f);

    bool galeArcherActive = false;
    for (const auto& en : enemies) {
        if (en.GetType() == EnemyType::GALE_ARCHER_BOSS && en.IsAlive()) {
            galeArcherActive = true;
            break;
        }
    }

    if (nearAltar) {
        float pulse = (sinf(time * 4.0f) + 1.0f) * 0.5f;
        Color runeColor = galeArcherActive ? Color{ 255, 110, 90, (unsigned char)(140 + 80 * pulse) }
                        : ((player.GetInventory().GetEssenceOfMist() >= 2) ? Color{ 110, 255, 230, (unsigned char)(160 + 80 * pulse) }
                                                                           : Color{ 255, 140, 140, (unsigned char)(120 + 60 * pulse) });
        DrawCircleLines((int)altarPosition.x, (int)altarPosition.y, 45.0f + 6.0f * pulse, runeColor);
        DrawPolyLines(altarPosition, 4, 30.0f, time * 35.0f, runeColor);

        if (galeArcherActive) {
            FontManager::DrawGameTextShadow("[E] ALTAR OF WHISPERS (GALE ARCHER ACTIVE)", (int)altarPosition.x - 145, (int)altarPosition.y - 85, 12, Color{ 255, 130, 110, 240 });
        } else {
            int mistCount = player.GetInventory().GetEssenceOfMist();
            if (mistCount >= 2) {
                std::string altarMsg = "[E] ALTAR OF WHISPERS: Offer 2x Essence of Mist (" + std::to_string(mistCount) + " Available)";
                FontManager::DrawGameTextShadow(altarMsg, (int)altarPosition.x - 175, (int)altarPosition.y - 85, 12, Color{ 110, 255, 230, 255 });
            } else {
                std::string altarMsg = "[E] ALTAR OF WHISPERS: Requires 2x Essence of Mist (" + std::to_string(mistCount) + "/2)";
                FontManager::DrawGameTextShadow(altarMsg, (int)altarPosition.x - 165, (int)altarPosition.y - 85, 12, Color{ 255, 140, 140, 240 });
            }
        }
    }

    // Floating prompt at Eastern Gateway
    Vector2 eastGatePos = { 100.0f, 700.0f };
    float distToEastGate = Vector2Distance(player.GetPosition(), eastGatePos);
    if (distToEastGate <= 130.0f) {
        FontManager::DrawGameTextShadow("[E] / [T] RETURN TO ZYPHDEN REALM (EAST)", (int)eastGatePos.x - 30, (int)eastGatePos.y - 45, 11, Color{ 255, 225, 120, 240 });
    }

    EndMode2D();

    // Screen-space reticle
    Vector2 mousePos = GetMousePosition();
    DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 110, 245, 220, 180 });
    DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 110, 245, 220, 220 });

    // Campaign HUD
    bool hasUnread = player.GetInventory().HasUnreadFragments();
    GameUI::DrawCampaignHUD(screenWidth, screenHeight, player, "RESTLESS MIST FOREST", hasUnread);

    // Boss Health Bar if Gale Archer Boss is active
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::GALE_ARCHER_BOSS && enemy.IsAlive()) {
            float barW = 620.0f;
            float barH = 18.0f;
            float barX = ((float)screenWidth - barW) * 0.5f;
            float barY = 56.0f;

            DrawRectangleRounded(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 10, 18, 16, 240 });
            DrawRectangleRoundedLines(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 90, 240, 215, 240 });

            FontManager::DrawTextTitleCentered("THE GALE ARCHER (MIST PHANTOM)", screenWidth / 2, (int)barY - 14, 15, Color{ 110, 255, 230, 255 });

            char defBuf[64];
            snprintf(defBuf, sizeof(defBuf), "ARMOR: %d  |  GALE WINDS: ACTIVE", (int)enemy.GetDefense());
            int dw = FontManager::MeasureText(defBuf, 10);
            FontManager::DrawGameTextShadow(defBuf, (screenWidth - dw) / 2, (int)barY + (int)barH + 4, 10, Color{ 160, 240, 225, 220 });

            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 18, 26, 24, 255 });
            float pct = fmaxf(0.0f, enemy.GetHealth() / enemy.GetMaxHealth());
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 40, 210, 180, 255 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 90, 240, 215, 220 });
            break;
        }
    }

    // Top Location Entry Banner
    if (entryBannerTimer > 0.0f) {
        float alpha = fminf(1.0f, entryBannerTimer);
        int bw = MeasureText("RESTLESS MIST FOREST", 22);
        int bx = (screenWidth - bw) / 2;
        int by = 68;
        DrawRectangle(bx - 24, by - 8, bw + 48, 40, Color{ 10, 18, 16, (unsigned char)(210 * alpha) });
        DrawRectangleLines(bx - 24, by - 8, bw + 48, 40, Color{ 90, 240, 215, (unsigned char)(240 * alpha) });
        DrawText("RESTLESS MIST FOREST", bx, by, 22, Color{ 120, 255, 230, (unsigned char)(255 * alpha) });
    }

    // Player Fallen Screen
    if (!player.IsAlive()) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{ 16, 6, 8, 220 });
        FontManager::DrawTextTitleCentered("LOST IN THE MIST", screenWidth / 2, screenHeight / 2 - 40, 38, Color{ 255, 60, 60, 255 });
        FontManager::DrawTextCentered("Press [R] to Respawn at East Entrance  |  [T] Return to Zyphden", screenWidth / 2, screenHeight / 2 + 18, 15, Color{ 220, 225, 240, 230 });
    }
}
