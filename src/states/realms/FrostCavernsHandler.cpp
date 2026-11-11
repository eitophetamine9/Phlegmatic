#include "FrostCavernsHandler.hpp"
#include "../ui/UI.hpp"
#include "../combat/CollisionSystem.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/FontManager.hpp"
#include <cmath>
#include <cstdio>
#include <algorithm>

FrostCavernsHandler::FrostCavernsHandler() {
    campfire = { 7, "Permafrost Crypts Bonfire", { 300.0f, 1200.0f } };
    sarcophagusPos = { 900.0f, 350.0f };
    sarcophagusRadius = 65.0f;
    loreStelePos = { 900.0f, 450.0f };
    loreSteleRadius = 45.0f;
    frostRevenantSummoned = false;
    frostRevenantDefeated = false;
    entryBannerTimer = 4.0f;
    exitCooldown = 0.5f;
    steleCollected = false;
}

void FrostCavernsHandler::InitStalagmites() {
    stalagmites.clear();
    stalagmites.push_back({ { 500.0f, 400.0f } });
    stalagmites.push_back({ { 1300.0f, 400.0f } });
    stalagmites.push_back({ { 700.0f, 750.0f } });
    stalagmites.push_back({ { 1100.0f, 750.0f } });
    stalagmites.push_back({ { 400.0f, 950.0f } });
    stalagmites.push_back({ { 1400.0f, 950.0f } });
    stalagmites.push_back({ { 900.0f, 1050.0f } });
}

void FrostCavernsHandler::InitSpawners() {
    spawners.clear();
    spawners.push_back({ { 550.0f, 650.0f }, 0.0f, true, EnemyType::FROST_TROLL });
    spawners.push_back({ { 1250.0f, 650.0f }, 0.0f, true, EnemyType::FROST_TROLL });
    spawners.push_back({ { 700.0f, 900.0f }, 0.0f, true, EnemyType::ARCHER });
    spawners.push_back({ { 1100.0f, 900.0f }, 0.0f, true, EnemyType::PYROMANCER });
    spawners.push_back({ { 900.0f, 800.0f }, 0.0f, true, EnemyType::BRUTE });
}

void FrostCavernsHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    map.Init(MapEnvironment::FROST_CAVERNS);

    player.Init({ 280.0f, 1230.0f });
    enemies.clear();
    projectiles.clear();

    InitStalagmites();
    InitSpawners();

    for (auto& spawner : spawners) {
        enemies.emplace_back(spawner.type, spawner.spawnPos);
        spawner.isAlive = true;
        spawner.respawnTimer = 0.0f;
    }

    frostRevenantSummoned = false;
    frostRevenantDefeated = false;
    entryBannerTimer = 4.0f;
    exitCooldown = 0.5f;

    camera.target = player.GetPosition();
}

void FrostCavernsHandler::Update(
    float dt,
    Player& player,
    GameMap& map,
    ParticleSystem& particles,
    std::vector<Enemy>& enemies,
    std::vector<Projectile>& projectiles,
    std::vector<LootDrop>& lootDrops,
    Camera2D& camera,
    GameState& currentState,
    const std::function<void()>& onReturnToHighlands,
    const std::function<void()>& onEnterLobby
) {
    (void)currentState;
    (void)onEnterLobby;

    if (entryBannerTimer > 0.0f) entryBannerTimer -= dt;
    if (exitCooldown > 0.0f) exitCooldown -= dt;

    if (!player.IsAlive()) {
        if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            player.Revive(campfire.position, 1.0f);
            enemies.clear();
            InitSpawners();
            for (auto& spawner : spawners) {
                enemies.emplace_back(spawner.type, spawner.spawnPos);
                spawner.isAlive = true;
                spawner.respawnTimer = 0.0f;
            }
            frostRevenantSummoned = false;
        }
        if (IsKeyPressed(KEY_L) || IsKeyPressed(KEY_T)) {
            if (onReturnToHighlands) onReturnToHighlands();
        }
        return;
    }

    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    // Subtle subterranean icy mist particle drift
    if ((rand() % 100) < 30) {
        float px = 200.0f + (float)(rand() % 1400);
        float py = 200.0f + (float)(rand() % 1000);
        particles.SpawnSparks({ px, py }, Color{ 140, 215, 255, 90 }, 1, 10.0f);
    }

    // Campfire update
    campfire.Update(player, particles);

    // Stalagmites Update & Player Attack Collision
    AttackInfo pAtk = player.GetCurrentAttack();
    for (auto& sm : stalagmites) {
        if (sm.broken) {
            sm.respawnTimer -= dt;
            if (sm.respawnTimer <= 0.0f) {
                sm.broken = false;
                sm.health = sm.maxHealth;
                particles.SpawnSparks(sm.position, Color{ 130, 215, 255, 220 }, 14, 120.0f);
            }
            continue;
        }

        if (sm.shakeTimer > 0.0f) sm.shakeTimer -= dt;

        if (pAtk.type != AttackType::NONE) {
            bool hit = false;
            if (pAtk.isFrontalCircle) {
                float dist = Vector2Distance(sm.position, pAtk.circleCenter);
                if (dist <= (sm.radius + pAtk.circleRadius)) hit = true;
            } else {
                float dist = Vector2Distance(sm.position, player.GetPosition());
                if (dist <= (sm.radius + pAtk.range)) {
                    Vector2 toNode = Vector2Normalize({ sm.position.x - player.GetPosition().x, sm.position.y - player.GetPosition().y });
                    float angleToNode = atan2f(toNode.y, toNode.x) * RAD2DEG;
                    float angleDiff = fabsf(angleToNode - player.GetFacingAngle());
                    while (angleDiff > 180.0f) angleDiff = fabsf(angleDiff - 360.0f);
                    if (angleDiff <= (pAtk.arcAngle * 0.5f)) hit = true;
                }
            }

            if (hit) {
                float dmg = (pAtk.type == AttackType::HEAVY) ? 35.0f : 16.0f;
                sm.health -= dmg;
                sm.shakeTimer = 0.14f;
                SoundFX::PlayHitImpact();
                particles.SpawnSparks(sm.position, Color{ 160, 230, 255, 255 }, 12, 110.0f);
                particles.SpawnFloatingText(sm.position, "-" + std::to_string((int)dmg), Color{ 180, 230, 255, 240 }, 18.0f);

                if (sm.health <= 0.0f) {
                    sm.broken = true;
                    sm.respawnTimer = 45.0f;
                    SoundFX::PlayCrusaderClash();
                    particles.SpawnSparks(sm.position, Color{ 180, 240, 255, 255 }, 24, 180.0f);
                    particles.SpawnFloatingText({ sm.position.x, sm.position.y - 12.0f }, "SHATTERED!", Color{ 140, 220, 255, 255 }, 20.0f);
                    lootDrops.emplace_back(LootType::SHINGLES, sm.position, 6 + (rand() % 6));
                    if ((rand() % 100) < 40) {
                        lootDrops.emplace_back(LootType::SHADOW_ESSENCE, Vector2{ sm.position.x + 8.0f, sm.position.y }, 1);
                    }
                }
            }
        }
    }

    // Spawners respawn tick
    for (auto& spawner : spawners) {
        if (!spawner.isAlive) {
            spawner.respawnTimer += dt;
            if (spawner.respawnTimer >= 35.0f) {
                enemies.emplace_back(spawner.type, spawner.spawnPos);
                spawner.isAlive = true;
                spawner.respawnTimer = 0.0f;
                particles.SpawnSparks(spawner.spawnPos, Color{ 120, 200, 255, 240 }, 18, 140.0f);
            }
        }
    }

    // Lore Stele Interaction
    const auto* frag = player.GetInventory().GetFragment("frag_15");
    steleCollected = (frag != nullptr && frag->discovered);
    if (!steleCollected && Vector2Distance(player.GetPosition(), loreStelePos) <= loreSteleRadius) {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            steleCollected = true;
            player.GetInventory().UnlockFragment("frag_15");
            particles.SpawnSparks(loreStelePos, Color{ 100, 220, 255, 255 }, 28, 180.0f);
            particles.SpawnFloatingText(loreStelePos, "LORE FRAGMENT XV UNLOCKED!", Color{ 130, 230, 255, 255 }, 22.0f);
            SoundFX::PlayParrySuccess();
        }
    }

    // Boss Summoning Sarcophagus Interaction
    bool bossAlive = false;
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::FROST_REVENANT && enemy.IsAlive()) {
            bossAlive = true;
            break;
        }
    }

    if (!bossAlive && frostRevenantSummoned) {
        frostRevenantDefeated = true;
    }

    if (!bossAlive && !frostRevenantDefeated && Vector2Distance(player.GetPosition(), sarcophagusPos) <= sarcophagusRadius) {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            frostRevenantSummoned = true;
            enemies.emplace_back(EnemyType::FROST_REVENANT, Vector2{ 900.0f, 290.0f });
            particles.AddScreenShake(8.0f, 0.35f);
            particles.SpawnSparks(sarcophagusPos, Color{ 80, 210, 255, 255 }, 40, 240.0f);
            particles.SpawnFloatingText(sarcophagusPos, "FROST REVENANT AWAKENED!", Color{ 120, 230, 255, 255 }, 26.0f);
            SoundFX::PlayHeavyAttack();
        }
    }

    // Exit Archway Interaction
    Vector2 returnArchPos = { 240.0f, 1250.0f };
    if (Vector2Distance(player.GetPosition(), returnArchPos) <= 75.0f && exitCooldown <= 0.0f) {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_T)) {
            exitCooldown = 1.0f;
            if (onReturnToHighlands) onReturnToHighlands();
            return;
        }
    }

    // Standard Combat & Enemy Collisions
    player.Update(dt, map, mouseWorld, particles);

    for (size_t i = 0; i < enemies.size(); ) {
        if (!enemies[i].IsAlive()) {
            for (auto& spawner : spawners) {
                if (Vector2Distance(spawner.spawnPos, enemies[i].GetPosition()) < 24.0f) {
                    spawner.isAlive = false;
                    spawner.respawnTimer = 0.0f;
                }
            }
            LootDrop::SpawnFromEnemy(enemies[i], lootDrops);
            enemies.erase(enemies.begin() + i);
        } else {
            enemies[i].Update(dt, player.GetPosition(), map, particles, projectiles);
            ++i;
        }
    }

    for (size_t i = 0; i < projectiles.size(); ) {
        projectiles[i].Update(dt, map, particles);
        if (!projectiles[i].IsActive()) {
            projectiles.erase(projectiles.begin() + i);
        } else {
            ++i;
        }
    }

    for (auto& loot : lootDrops) {
        loot.Update(dt, player.GetPosition(), player, particles);
    }
    lootDrops.erase(std::remove_if(lootDrops.begin(), lootDrops.end(), [](const LootDrop& l) { return l.IsCollected(); }), lootDrops.end());

    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera, &lootDrops);

    // Camera smoothing
    camera.target.x += (player.GetPosition().x - camera.target.x) * 0.12f;
    camera.target.y += (player.GetPosition().y - camera.target.y) * 0.12f;
}

void FrostCavernsHandler::Render(
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
    (void)showControlsModal;
    (void)showInventoryModal;
    (void)inventoryActiveTab;
    (void)inventorySelectedFragment;

    float time = (float)GetTime();
    Vector2 playerPos = player.GetPosition();

    BeginMode2D(camera);

    map.DrawFloor(playerPos);

    // Draw Stalagmites
    for (const auto& sm : stalagmites) {
        if (sm.broken) {
            DrawCircle((int)sm.position.x, (int)sm.position.y, 8.0f, Color{ 35, 60, 90, 160 });
            DrawCircleLines((int)sm.position.x, (int)sm.position.y, 8.0f, Color{ 70, 130, 190, 140 });
        } else {
            float shakeX = (sm.shakeTimer > 0.0f) ? sinf(time * 60.0f) * 3.0f : 0.0f;
            Vector2 pos = { sm.position.x + shakeX, sm.position.y };
            DrawCircle((int)pos.x, (int)(pos.y + 6.0f), sm.radius * 1.1f, Color{ 8, 14, 22, 140 });
            DrawPoly(pos, 6, sm.radius, 30.0f, Color{ 35, 65, 105, 255 });
            DrawPolyLines(pos, 6, sm.radius, 30.0f, Color{ 100, 200, 255, 230 });

            // Glint
            float gPulse = 0.5f + 0.5f * sinf(time * 3.0f + sm.position.x);
            DrawCircleGradient(pos, sm.radius * 0.6f, Color{ 120, 220, 255, (unsigned char)(80 + 60 * gPulse) }, Color{ 0, 0, 0, 0 });
            DrawCircle((int)pos.x, (int)pos.y, 3.0f, Color{ 200, 245, 255, 240 });
        }
    }

    // Draw Campfire
    campfire.Draw(time, player);

    // Draw Lore Stele Pedestal
    DrawCircle((int)loreStelePos.x, (int)loreStelePos.y, 22.0f, Color{ 24, 40, 60, 255 });
    DrawCircleLines((int)loreStelePos.x, (int)loreStelePos.y, 22.0f, Color{ 90, 195, 255, 230 });
    if (!steleCollected) {
        float sPulse = 0.5f + 0.5f * sinf(time * 3.5f);
        DrawCircleGradient(loreStelePos, 34.0f, Color{ 100, 220, 255, (unsigned char)(80 + 70 * sPulse) }, Color{ 0, 0, 0, 0 });
        DrawText("LORE STELE XV", (int)loreStelePos.x - 38, (int)loreStelePos.y - 32, 10, Color{ 150, 225, 255, 240 });
    }

    // World interaction prompts
    if (!steleCollected && Vector2Distance(playerPos, loreStelePos) <= loreSteleRadius) {
        FontManager::DrawGameTextShadow("[E] DECIPHER GLACIAL RUNE INSCRIPTION", (int)loreStelePos.x - 120, (int)loreStelePos.y - 42, 11, Color{ 130, 225, 255, 255 });
    }

    bool bossAlive = false;
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::FROST_REVENANT && enemy.IsAlive()) {
            bossAlive = true;
            break;
        }
    }

    if (!bossAlive && !frostRevenantDefeated && Vector2Distance(playerPos, sarcophagusPos) <= sarcophagusRadius) {
        FontManager::DrawGameTextShadow("[E] AWAKEN FROST REVENANT", (int)sarcophagusPos.x - 90, (int)sarcophagusPos.y - 44, 11, Color{ 130, 230, 255, 255 });
    }

    Vector2 returnArchPos = { 240.0f, 1250.0f };
    if (Vector2Distance(playerPos, returnArchPos) <= 85.0f) {
        FontManager::DrawGameTextShadow("[E] ASCEND TO NORTHERN HIGHLANDS", (int)returnArchPos.x - 110, (int)returnArchPos.y - 36, 11, Color{ 160, 225, 255, 240 });
    }

    for (const auto& drop : lootDrops) {
        drop.Draw(time);
    }

    for (const auto& enemy : enemies) {
        enemy.DrawTelegraph();
        enemy.Draw();
    }

    player.Draw();

    for (const auto& proj : projectiles) {
        proj.Draw();
    }

    map.DrawWallsAndPillars(player.GetPosition());

    particles.Draw();

    EndMode2D();

    map.ClearDynamicLights();
    map.AddDynamicLight(campfire.position, 310.0f, Color{ 255, 165, 65, 255 }, 1.00f);
    map.AddDynamicLight(loreStelePos, 175.0f, Color{ 110, 220, 255, 255 }, 0.78f);
    map.AddDynamicLight(sarcophagusPos, 245.0f, Color{ 110, 200, 255, 255 }, 0.84f);
    map.AddDynamicLight(returnArchPos, 185.0f, Color{ 150, 220, 255, 255 }, 0.72f);
    for (const auto& sm : stalagmites) {
        if (!sm.broken) {
            map.AddDynamicLight(sm.position, 115.0f, Color{ 115, 215, 255, 255 }, 0.62f);
        }
    }
    Projectile::RegisterDynamicLights(projectiles, map);

    map.DrawLighting(camera, player.GetPosition());

    // Frostbite Vignette
    float frostMeter = player.GetFrostbiteMeter();
    if (frostMeter > 10.0f) {
        float maxAlpha = (frostMeter / 100.0f) * 110.0f;
        if (player.IsFrostbitten()) {
            float pulse = 0.7f + 0.3f * sinf(time * 4.0f);
            maxAlpha = 140.0f * pulse;
        }
        DrawRectangleGradientV(0, 0, screenWidth, 54, Color{ 130, 210, 255, (unsigned char)maxAlpha }, Color{ 130, 210, 255, 0 });
        DrawRectangleGradientV(0, screenHeight - 54, screenWidth, 54, Color{ 130, 210, 255, 0 }, Color{ 130, 210, 255, (unsigned char)maxAlpha });
        DrawRectangleGradientH(0, 0, 54, screenHeight, Color{ 130, 210, 255, (unsigned char)maxAlpha }, Color{ 130, 210, 255, 0 });
        DrawRectangleGradientH(screenWidth - 54, 0, 54, screenHeight, Color{ 130, 210, 255, 0 }, Color{ 130, 210, 255, (unsigned char)maxAlpha });
    }

    // Crosshair
    Vector2 mousePos = GetMousePosition();
    DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 140, 220, 255, 180 });
    DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 100, 200, 255, 220 });

    // Campaign HUD
    bool hasUnread = player.GetInventory().HasUnreadFragments();
    GameUI::DrawCampaignHUD(screenWidth, screenHeight, player, "THE PERMAFROST CRYPTS", hasUnread, &map.GetTimeOfDay());

    // Tactical Radar
    GameUI::DrawTacticalRadar(screenWidth, screenHeight, player, enemies, { campfire.position }, {}, { sarcophagusPos, loreStelePos, returnArchPos });

    // Boss Health Bar
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::FROST_REVENANT && enemy.IsAlive()) {
            float barW = 640.0f;
            float barH = 18.0f;
            float barX = ((float)screenWidth - barW) * 0.5f;
            float barY = 56.0f;

            DrawRectangleRounded(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 12, 18, 28, 240 });
            DrawRectangleRoundedLines(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 80, 205, 255, 240 });

            FontManager::DrawTextTitleCentered("THE FROST REVENANT (CHASM PHANTOM)", screenWidth / 2, (int)barY - 14, 15, Color{ 130, 225, 255, 255 });

            char defBuf[64];
            snprintf(defBuf, sizeof(defBuf), "ARMOR: %d  |  CHILL NOVA: ACTIVE", (int)enemy.GetDefense());
            int dw = FontManager::MeasureText(defBuf, 10);
            FontManager::DrawGameTextShadow(defBuf, (screenWidth - dw) / 2, (int)barY + (int)barH + 4, 10, Color{ 160, 220, 255, 220 });

            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 18, 26, 38, 255 });
            float pct = fmaxf(0.0f, enemy.GetHealth() / enemy.GetMaxHealth());
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 60, 180, 245, 255 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 120, 225, 255, 220 });
            break;
        }
    }

    // Entry Banner
    if (entryBannerTimer > 0.0f) {
        float alpha = fminf(1.0f, entryBannerTimer > 3.0f ? (4.0f - entryBannerTimer) : entryBannerTimer);
        Color cBg = { 10, 18, 28, (unsigned char)(210 * alpha) };
        Color cBorder = { 90, 200, 255, (unsigned char)(230 * alpha) };
        Color cTitle = { 150, 230, 255, (unsigned char)(255 * alpha) };
        Color cSub = { 180, 225, 255, (unsigned char)(220 * alpha) };

        float bw = 520.0f;
        float bh = 80.0f;
        float bx = ((float)screenWidth - bw) * 0.5f;
        float by = 130.0f;
        DrawRectangleRounded(Rectangle{ bx, by, bw, bh }, 0.2f, 4, cBg);
        DrawRectangleRoundedLines(Rectangle{ bx, by, bw, bh }, 0.2f, 4, cBorder);
        FontManager::DrawTextTitleCentered("THE PERMAFROST CRYPTS", screenWidth / 2, (int)by + 14, 18, cTitle);
        FontManager::DrawTextTitleCentered("Subterranean Chasm of the Ancient Wardens", screenWidth / 2, (int)by + 44, 12, cSub);
    }
}

