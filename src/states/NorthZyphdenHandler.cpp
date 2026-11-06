#include "NorthZyphdenHandler.hpp"
#include "UI.hpp"
#include "../ui/HUD.hpp"
#include "../ui/ModalUI.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/CollisionSystem.hpp"
#include <algorithm>

NorthZyphdenHandler::NorthZyphdenHandler() {
    berserkerTrollSummoned = false;
    berserkerTrollDefeated = false;
    bossRematchCooldown = 0.0f;
    entryBannerTimer = 0.0f;
    exitCooldown = 0.0f;
    highlandCampfire = { 6, "Highland Crags Bonfire", { 1200.0f, 1300.0f } };
    InitSpawners();
}

void NorthZyphdenHandler::InitSpawners() {
    spawners.clear();
    // Frost Troll highland packs
    spawners.push_back({ { 650.0f, 600.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 1550.0f, 600.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 900.0f, 850.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 1500.0f, 850.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 500.0f, 1050.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 1850.0f, 1050.0f }, 0.0f, false, EnemyType::FROST_TROLL });

    // Heavy support brutes & scouts
    spawners.push_back({ { 700.0f, 450.0f }, 0.0f, false, EnemyType::BRUTE });
    spawners.push_back({ { 1700.0f, 450.0f }, 0.0f, false, EnemyType::BRUTE });
    spawners.push_back({ { 1080.0f, 1050.0f }, 0.0f, false, EnemyType::SLASHER });
    spawners.push_back({ { 1320.0f, 1050.0f }, 0.0f, false, EnemyType::SLASHER });

    // 4 Lore Steles across Northern Crags
    loreSteles.clear();
    loreSteles.push_back({
        "frag_11",
        "Fragment XI: The Frigid Highland War",
        "[E] Read Ancient Outpost Monolith",
        Vector2{ 450.0f, 400.0f },
        36.0f,
        false
    });
    loreSteles.push_back({
        "frag_12",
        "Fragment XII: Curse of the Berserker",
        "[E] Inspect Troll Burial Mound",
        Vector2{ 1850.0f, 420.0f },
        36.0f,
        false
    });
    loreSteles.push_back({
        "frag_13",
        "Fragment XIII: The Shattered Totem of Blood",
        "[E] Decipher Blood Altar Inscription",
        Vector2{ 1200.0f, 240.0f },
        42.0f,
        false
    });
    loreSteles.push_back({
        "frag_14",
        "Fragment XIV: Watch of the Northern Vanguard",
        "[E] Inspect Vanguard Lookout Cairn",
        Vector2{ 1750.0f, 1050.0f },
        36.0f,
        false
    });
}

void NorthZyphdenHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    map.Init(MapEnvironment::NORTH_ZYPHDEN);
    player.SetPosition({ 1200.0f, 1380.0f }); // Enters from southern pass
    camera.target = player.GetPosition();
    projectiles.clear();
    enemies.clear();

    InitSpawners();

    // Spawn initial roaming mobs
    for (auto& spawner : spawners) {
        enemies.emplace_back(spawner.type, spawner.spawnPos);
        spawner.isAlive = true;
        spawner.respawnTimer = 0.0f;
    }

    berserkerTrollSummoned = false;
    entryBannerTimer = 4.0f;
    exitCooldown = 1.0f;
}

void NorthZyphdenHandler::Update(
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
    if (exitCooldown > 0.0f) exitCooldown -= dt;
    if (bossRematchCooldown > 0.0f) bossRematchCooldown -= dt;

    // Player Death & Respawn
    if (!player.IsAlive()) {
        if (IsKeyPressed(KEY_R)) {
            player.Revive({ 1200.0f, 1380.0f });
            camera.target = player.GetPosition();
            enemies.clear();
            InitSpawners();
            for (auto& spawner : spawners) {
                enemies.emplace_back(spawner.type, spawner.spawnPos);
                spawner.isAlive = true;
                spawner.respawnTimer = 0.0f;
            }
            berserkerTrollSummoned = false;
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

    // Shortcuts
    if (IsKeyPressed(KEY_L)) {
        if (onEnterLobby) onEnterLobby();
        return;
    }

    Vector2 playerPos = player.GetPosition();

    // Highland Campfire Waypoint Rest
    highlandCampfire.Update(player, particles);

    // Southern Return Gate Interaction
    Vector2 returnGatePos = { 1200.0f, 1450.0f };
    float distToReturnGate = Vector2Distance(playerPos, returnGatePos);
    if (distToReturnGate <= 95.0f && exitCooldown <= 0.0f) {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_T)) {
            exitCooldown = 1.0f;
            if (onReturnToZyphden) onReturnToZyphden();
            return;
        }
    }

    // Lore Steles Interaction
    for (auto& stele : loreSteles) {
        const auto* frag = player.GetInventory().GetFragment(stele.fragmentId);
        stele.collected = (frag != nullptr && frag->discovered);

        if (!stele.collected && Vector2Distance(playerPos, stele.position) <= stele.radius) {
            if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                stele.collected = true;
                player.GetInventory().UnlockFragment(stele.fragmentId);
                SoundFX::PlayParrySuccess();
                particles.SpawnFloatingText(stele.position, "DISCOVERED: " + stele.title + "!", Color{ 255, 215, 80, 255 }, 24.0f);
                particles.SpawnSparks(stele.position, Color{ 255, 220, 90, 255 }, 25, 140.0f);
                particles.AddScreenShake(4.0f, 0.2f);
            }
        }
    }

    // Berserker Troll Arena Trigger & Altar Rematch
    Vector2 arenaCenter = { 1200.0f, 440.0f };
    float distToArena = Vector2Distance(playerPos, arenaCenter);

    bool bossAlive = false;
    for (const auto& en : enemies) {
        if (en.GetType() == EnemyType::BERSERKER_TROLL && en.IsAlive()) {
            bossAlive = true;
            break;
        }
    }

    // Spawn Berserker Troll when entering northern ritual dais
    if (!berserkerTrollSummoned && !berserkerTrollDefeated && !bossAlive && distToArena <= 260.0f) {
        berserkerTrollSummoned = true;
        enemies.emplace_back(EnemyType::BERSERKER_TROLL, Vector2{ 1200.0f, 400.0f });
        particles.SpawnFloatingText(Vector2{ 1200.0f, 400.0f }, "BERSERKER TROLL AWAKENS!", Color{ 255, 45, 60, 255 }, 26.0f);
        particles.AddScreenShake(6.0f, 0.35f);
        SoundFX::PlayHitImpact();
    }

    // Rematch at Altar
    Vector2 altarPos = { 1200.0f, 240.0f };
    float distToAltar = Vector2Distance(playerPos, altarPos);
    if (distToAltar <= 85.0f && !bossAlive && bossRematchCooldown <= 0.0f && berserkerTrollDefeated) {
        if (IsKeyPressed(KEY_E)) {
            enemies.emplace_back(EnemyType::BERSERKER_TROLL, Vector2{ 1200.0f, 400.0f });
            berserkerTrollSummoned = true;
            particles.SpawnFloatingText(Vector2{ 1200.0f, 400.0f }, "BERSERKER TROLL REMATCH COMMENCED!", Color{ 255, 45, 60, 255 }, 26.0f);
            particles.AddScreenShake(6.0f, 0.35f);
            SoundFX::PlayParrySuccess();
        }
    }

    // Entity updates
    player.Update(dt, map, mouseWorld, particles);

    for (const auto& p : player.GetSpawnedProjectiles()) {
        projectiles.push_back(p);
    }
    player.GetSpawnedProjectiles().clear();

    for (auto& enemy : enemies) {
        enemy.Update(dt, player.GetTargetPosition(), map, particles, projectiles);
    }

    for (auto& proj : projectiles) {
        if (proj.IsActive()) proj.Update(dt, map, particles);
    }
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) { return !p.IsActive(); }), projectiles.end());

    // Mob Spawner Respawn Timers
    for (auto& spawner : spawners) {
        if (!spawner.isAlive) {
            spawner.respawnTimer -= dt;
            if (spawner.respawnTimer <= 0.0f) {
                enemies.emplace_back(spawner.type, spawner.spawnPos);
                spawner.isAlive = true;
            }
        }
    }

    // Combat Collisions
    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera, &lootDrops);

    // Enemy Death & Boss Loot Checks
    for (auto it = enemies.begin(); it != enemies.end();) {
        if (!it->IsAlive()) {
            if (it->GetType() == EnemyType::BERSERKER_TROLL) {
                berserkerTrollDefeated = true;
                berserkerTrollSummoned = false;
                bossRematchCooldown = 30.0f;

                // Guaranteed Totem of Insanity drop unlock
                if (!player.GetInventory().HasTotem(TotemType::TOTEM_INSANITY)) {
                    player.GetInventory().UnlockTotem(TotemType::TOTEM_INSANITY);
                    particles.SpawnFloatingText(it->GetPosition(), "CLAIMED: TOTEM OF INSANITY!", Color{ 255, 45, 60, 255 }, 26.0f);
                } else {
                    particles.SpawnFloatingText(it->GetPosition(), "BERSERKER TROLL VANQUISHED!", Color{ 255, 90, 90, 255 }, 24.0f);
                }
                SoundFX::PlayParrySuccess();
                particles.AddScreenShake(7.0f, 0.4f);
            }

            // Sync with spawner tracking
            for (auto& spawner : spawners) {
                if (spawner.isAlive && Vector2Distance(it->GetPosition(), spawner.spawnPos) <= 120.0f) {
                    spawner.isAlive = false;
                    spawner.respawnTimer = 25.0f;
                    break;
                }
            }

            LootDrop::SpawnFromEnemy(*it, lootDrops);
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    // Loot drops update
    for (auto& loot : lootDrops) {
        loot.Update(dt, playerPos, player, particles);
    }
    lootDrops.erase(std::remove_if(lootDrops.begin(), lootDrops.end(), [](const LootDrop& l) { return l.IsCollected(); }), lootDrops.end());

    particles.Update(dt);

    // Camera smoothing
    Vector2 shake = particles.GetScreenShakeOffset();
    Vector2 desiredTarget = { playerPos.x + shake.x, playerPos.y + shake.y };
    camera.target = Vector2Lerp(camera.target, desiredTarget, 10.0f * dt);
}

void NorthZyphdenHandler::Render(
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

    BeginMode2D(camera);

    map.DrawFloor(player.GetPosition());

    // Draw Lore Steles in world
    float time = (float)GetTime();
    Vector2 playerPos = player.GetPosition();

    // Highland Campfire Waypoint Rest
    highlandCampfire.Draw(time, player);
    for (const auto& stele : loreSteles) {
        // Drop shadow
        DrawEllipse((int)stele.position.x, (int)(stele.position.y + 12), stele.radius * 1.15f, stele.radius * 0.55f, Color{ 10, 14, 20, 140 });

        // Carved Monolith Stele
        DrawRectangle((int)stele.position.x - 18, (int)stele.position.y - 28, 36, 56, Color{ 42, 50, 65, 255 });
        DrawRectangleLines((int)stele.position.x - 18, (int)stele.position.y - 28, 36, 56, Color{ 75, 95, 125, 255 });

        float pulse = 0.5f + 0.5f * sinf(time * 3.5f + stele.position.x);
        Color runeGlow = stele.collected ? Color{ 120, 180, 240, 140 } : Color{ 255, 215, 80, (unsigned char)(160 + 80 * pulse) };
        DrawCircleGradient(stele.position, 28.0f * pulse, runeGlow, Color{ 0, 0, 0, 0 });
        DrawCircle((int)stele.position.x, (int)stele.position.y, 4.0f, runeGlow);

        if (Vector2Distance(playerPos, stele.position) <= stele.radius) {
            std::string prompt = stele.collected ? "[E] Re-read " + stele.title : stele.prompt;
            int pw = FontManager::MeasureText(prompt, 10);
            FontManager::DrawGameTextShadow(prompt, (int)stele.position.x - pw / 2, (int)stele.position.y - 42, 10, Color{ 255, 230, 120, 240 });
        }
    }

    // Rematch Altar Inscription prompt
    Vector2 altarPos = { 1200.0f, 240.0f };
    float distToAltar = Vector2Distance(playerPos, altarPos);
    if (distToAltar <= 120.0f && berserkerTrollDefeated) {
        bool bossAlive = false;
        for (const auto& en : enemies) {
            if (en.GetType() == EnemyType::BERSERKER_TROLL && en.IsAlive()) { bossAlive = true; break; }
        }
        if (!bossAlive) {
            if (bossRematchCooldown <= 0.0f) {
                FontManager::DrawGameTextShadow("[E] CHALLENGE BERSERKER TROLL REMATCH", (int)altarPos.x - 130, (int)altarPos.y - 46, 11, Color{ 255, 80, 95, 255 });
            } else {
                std::string cdMsg = "CRAG COOLDOWN: " + std::to_string((int)ceilf(bossRematchCooldown)) + "s";
                FontManager::DrawGameTextShadow(cdMsg, (int)altarPos.x - 65, (int)altarPos.y - 46, 11, Color{ 180, 190, 205, 200 });
            }
        }
    }

    // Southern Return Gate prompt
    Vector2 returnGatePos = { 1200.0f, 1450.0f };
    if (Vector2Distance(playerPos, returnGatePos) <= 120.0f) {
        FontManager::DrawGameTextShadow("[E] RETURN TO ZYPHDEN OVERWORLD", (int)returnGatePos.x - 100, (int)returnGatePos.y - 40, 11, Color{ 160, 220, 255, 240 });
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

    // Crosshair
    Vector2 mousePos = GetMousePosition();
    DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 255, 120, 120, 180 });
    DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 255, 80, 80, 220 });

    // Campaign HUD
    bool hasUnread = player.GetInventory().HasUnreadFragments();
    GameUI::DrawCampaignHUD(screenWidth, screenHeight, player, "NORTHERN CRAGS OF ZYPHDEN", hasUnread);

    // Boss Health Bar for Berserker Troll
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::BERSERKER_TROLL && enemy.IsAlive()) {
            float barW = 640.0f;
            float barH = 18.0f;
            float barX = ((float)screenWidth - barW) * 0.5f;
            float barY = 56.0f;

            DrawRectangleRounded(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 18, 12, 14, 240 });
            DrawRectangleRoundedLines(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 255, 55, 70, 240 });

            FontManager::DrawTextTitleCentered("THE BERSERKER TROLL (CRAG WARLORD)", screenWidth / 2, (int)barY - 14, 15, Color{ 255, 85, 95, 255 });

            char defBuf[64];
            snprintf(defBuf, sizeof(defBuf), "ARMOR: %d  |  BLOOD FRENZY: ACTIVE", (int)enemy.GetDefense());
            int dw = FontManager::MeasureText(defBuf, 10);
            FontManager::DrawGameTextShadow(defBuf, (screenWidth - dw) / 2, (int)barY + (int)barH + 4, 10, Color{ 240, 180, 180, 220 });

            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 28, 14, 16, 255 });
            float pct = fmaxf(0.0f, enemy.GetHealth() / enemy.GetMaxHealth());
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 235, 45, 60, 255 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 255, 120, 130, 220 });
            break;
        }
    }
}
