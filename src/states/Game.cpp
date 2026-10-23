#include "Game.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "raymath.h"
#include <cmath>
#include <string>
#include <algorithm>

Game::Game() : currentState(GameState::TITLE_SCREEN), showControlsModal(false), showInventoryModal(false), inventoryActiveTab(0), inventorySelectedFragment(0) {
}

Game::~Game() {
    SoundFX::Close();
    assetManager.Unload();
    CloseWindow();
}

void Game::Init() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE);
    SetTargetFPS(Config::TARGET_FPS);

    SoundFX::Init();
    assetManager.Init();

    pedestals.clear();
    pedestals.push_back({ { 600.0f, 650.0f }, WeaponType::SWORD });
    pedestals.push_back({ { 730.0f, 590.0f }, WeaponType::DAGGERS });
    pedestals.push_back({ { 870.0f, 590.0f }, WeaponType::MACE });
    pedestals.push_back({ { 1000.0f, 650.0f }, WeaponType::TRUTH_SEEKER });

    dummy.position = { 800.0f, 810.0f };
    portal.position = { 800.0f, 430.0f };

    camera.target = { 800.0f, 600.0f };
    camera.offset = { (float)Config::WINDOW_WIDTH * 0.5f, (float)Config::WINDOW_HEIGHT * 0.5f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    currentState = GameState::TITLE_SCREEN;
    showControlsModal = false;
    ruinedCastleHandler.Reset();
    testArenaUnlocked = LoadArenaUnlock();
    bandageSpawnTimer = 14.0f;
}

void Game::SaveArenaUnlock() {
    FILE* f = fopen("arena_unlock.dat", "w");
    if (f) {
        fprintf(f, "1\n");
        fclose(f);
    }
}

bool Game::LoadArenaUnlock() {
    FILE* f = fopen("arena_unlock.dat", "r");
    if (f) {
        int v = 0;
        if (fscanf(f, "%d", &v) == 1 && v == 1) {
            fclose(f);
            return true;
        }
        fclose(f);
    }
    return false;
}

void Game::EnterRuinedCastle() {
    map.Init(MapEnvironment::RUINED_CASTLE);
    player.Init({ 800.0f, 1180.0f });
    player.SetWeapon(WeaponType::NONE); // Start unarmed in tutorial!
    player.GetInventory().ClearWeapons(); // No starting weapons in tutorial!
    camera.target = player.GetPosition();
    currentState = GameState::RUINED_CASTLE;
    showControlsModal = false;
    showInventoryModal = false;
    ruinedCastleHandler.Reset();
    enemies.clear();
    projectiles.clear();
    particles.Clear();
    tutorial.Init();
}

void Game::EnterLobby() {
    map.Init(MapEnvironment::LOBBY);
    WeaponType keepWeapon = player.GetWeapon();
    player.Init({ 800.0f, 740.0f });
    if (keepWeapon != WeaponType::NONE) {
        player.SetWeapon(keepWeapon);
    } else if (player.GetInventory().HasWeapon(WeaponType::TRUTH_SEEKER)) {
        player.SetWeapon(WeaponType::TRUTH_SEEKER);
    } else if (player.GetInventory().HasWeapon(WeaponType::SWORD)) {
        player.SetWeapon(WeaponType::SWORD);
    }
    camera.target = player.GetPosition();
    currentState = GameState::LOBBY;
    showControlsModal = false;
    projectiles.clear();
    particles.Clear();
    particles.SpawnFloatingText(player.GetPosition(), "LOBBY SANCTUARY", Color{ 255, 220, 100, 255 }, 24.0f);
}

void Game::EnterArena() {
    map.Init(MapEnvironment::ARENA);
    WeaponType keepWeapon = player.GetWeapon();
    player.Init({ 1200.0f, 900.0f });
    if (keepWeapon != WeaponType::NONE) {
        player.SetWeapon(keepWeapon);
    } else if (player.GetInventory().HasWeapon(WeaponType::TRUTH_SEEKER)) {
        player.SetWeapon(WeaponType::TRUTH_SEEKER);
    } else if (player.GetInventory().HasWeapon(WeaponType::SWORD)) {
        player.SetWeapon(WeaponType::SWORD);
    }
    camera.target = player.GetPosition();
    projectiles.clear();
    particles.Clear();
    arenaBandages.clear();
    bandageSpawnTimer = 14.0f;
    AdhesiveBandage initBandage;
    initBandage.position = { 1200.0f, 1050.0f };
    arenaBandages.push_back(initBandage);

    waveManager.SpawnWave(1, enemies, projectiles, particles);
    currentState = GameState::ARENA;
    showControlsModal = false;
    SoundFX::PlayEquipWeapon();
    particles.SpawnSparks(player.GetPosition(), Color{ 100, 220, 255, 255 }, 12, 180.0f);
}

void Game::Update(float dt) {
    Vector2 mouseScreen = GetMousePosition();

    // Global Shortcut: '~' (KEY_GRAVE) returns immediately to Title Screen from any map/state
    if (IsKeyPressed(KEY_GRAVE)) {
        currentState = GameState::TITLE_SCREEN;
        showControlsModal = false;
        showInventoryModal = false;
        SoundFX::PlayButtonClick();
        return;
    }

    // If Controls Modal is active
    if (showControlsModal) {
        if (IsKeyPressed(KEY_C) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_TAB)) {
            showControlsModal = false;
            SoundFX::PlayButtonClick();
            return;
        }
        bool close = GameUI::DrawControlsModal(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        if (close) {
            showControlsModal = false;
        }
        return;
    }

    // If Inventory & Lore Journal Modal is active
    if (showInventoryModal) {
        if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ESCAPE)) {
            showInventoryModal = false;
            SoundFX::PlayButtonClick();
            return;
        }
        bool close = GameUI::DrawInventoryModal(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), player, inventoryActiveTab, inventorySelectedFragment);
        if (close) {
            showInventoryModal = false;
        }
        return;
    }

    // Toggle Controls Menu modal when closed
    if (IsKeyPressed(KEY_C)) {
        showControlsModal = true;
        showInventoryModal = false;
        SoundFX::PlayButtonClick();
        return;
    }

    // Toggle Inventory & Lore Journal modal when closed
    if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TAB)) {
        showInventoryModal = true;
        showControlsModal = false;
        inventoryActiveTab = 1; // Direct directly to Lore Journal tab
        SoundFX::PlayButtonClick();
        return;
    }

    // Check HUD top right button clicks in Ruined Castle, Lobby & Arena
    if (!showControlsModal && !showInventoryModal) {
        if (currentState == GameState::RUINED_CASTLE || currentState == GameState::LOBBY || currentState == GameState::ARENA) {
            Rectangle journalBtnRec = { (float)(Config::WINDOW_WIDTH - 275), 16.0f, 120.0f, 32.0f };
            Rectangle controlsBtnRec = { (float)(Config::WINDOW_WIDTH - 140), 16.0f, 116.0f, 32.0f };
            if (CheckCollisionPointRec(mouseScreen, journalBtnRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                showInventoryModal = true;
                showControlsModal = false;
                inventoryActiveTab = 1;
                SoundFX::PlayButtonClick();
                return;
            } else if (CheckCollisionPointRec(mouseScreen, controlsBtnRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                showControlsModal = true;
                showInventoryModal = false;
                SoundFX::PlayButtonClick();
                return;
            }
        }
    }

    // 1. TITLE SCREEN STATE
    if (currentState == GameState::TITLE_SCREEN) {
        int action = GameUI::DrawTitleScreen(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), testArenaUnlocked);
        if (action == 1) {
            cutscene.Reset();
            currentState = GameState::PROLOGUE_CUTSCENE;
        } else if (action == 2) {
            showControlsModal = true;
        } else if (action == 3) {
            if (testArenaUnlocked) {
                EnterArena();
            }
        }
        return;
    }

    // 1.5 PROLOGUE CUTSCENE STATE
    if (currentState == GameState::PROLOGUE_CUTSCENE) {
        if (cutscene.Update(dt)) {
            EnterRuinedCastle();
        }
        return;
    }

    // 1.8 RUINED CASTLE CHAPTER 1 STATE
    if (currentState == GameState::RUINED_CASTLE) {
        ruinedCastleHandler.Update(
            dt, player, map, particles, enemies, projectiles, tutorial, camera, currentState, testArenaUnlocked,
            [this]() { SaveArenaUnlock(); },
            [this]() { EnterLobby(); },
            [this]() { EnterRuinedCastle(); }
        );
        return;
    }

    // 1.9 BORDERLANDS GATEWAY EPILOGUE STATE
    if (currentState == GameState::BORDERLANDS_GATE) {
        int choice = GameUI::DrawBorderlandsGateway(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), player);
        if (choice == 1 || IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) { // Arena Test
            if (testArenaUnlocked) {
                EnterArena();
                return;
            } else {
                particles.SpawnFloatingText(player.GetPosition(), "ARENA LOCKED — COMPLETE CHAPTER 1 FIRST!", Color{ 255, 90, 90, 255 }, 20.0f);
                SoundFX::PlayHitImpact();
            }
        } else if (choice == 2 || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_H) || IsKeyPressed(KEY_L) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) { // Back to Hub
            EnterLobby();
            return;
        }
        return;
    }

    // 2. LOBBY STATE
    if (currentState == GameState::LOBBY) {
        Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

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

        player.Update(dt, map, mouseWorld, particles);
        dummy.Update(dt);

        if (player.IsAttacking() && !player.GetCurrentAttack().hasHit) {
            if (dummy.CheckHit(player.GetCurrentAttack(), particles)) {
                player.SetAttackHit();
                particles.AddScreenShake(2.5f, 0.1f);
            }
        }

        for (const auto& ped : pedestals) {
            bool near = ped.CheckInteraction(player.GetPosition());
            bool clicked = CheckCollisionPointCircle(mouseWorld, ped.position, ped.radius) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if ((near && IsKeyPressed(KEY_E)) || clicked) {
                if (ped.weapon == WeaponType::TRUTH_SEEKER && !player.GetInventory().HasWeapon(WeaponType::TRUTH_SEEKER)) {
                    particles.SpawnFloatingText(player.GetPosition(), "LOCKED — DEFEAT ZIL IN CHAPTER 1!", Color{ 255, 90, 90, 255 }, 20.0f);
                    SoundFX::PlayHitImpact();
                } else if (player.GetWeapon() != ped.weapon) {
                    player.SetWeapon(ped.weapon);
                    WeaponStats st = GetWeaponStats(ped.weapon);
                    particles.SpawnFloatingText(player.GetPosition(), "EQUIPPED: " + st.name, Color{ 255, 220, 80, 255 }, 22.0f);
                    particles.SpawnSparks(ped.position, Color{ 100, 220, 255, 255 }, 16, 120.0f);
                }
            }
        }

        if (portal.CheckPlayerInside(player.GetPosition()) || (Vector2Distance(player.GetPosition(), portal.position) < 80.0f && IsKeyPressed(KEY_E))) {
            if (!testArenaUnlocked) {
                particles.SpawnFloatingText(player.GetPosition(), "ARENA LOCKED — COMPLETE CHAPTER 1 FIRST!", Color{ 255, 90, 90, 255 }, 20.0f);
                SoundFX::PlayHitImpact();
            } else {
                EnterArena();
                return;
            }
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            currentState = GameState::TITLE_SCREEN;
            return;
        }

        particles.Update(dt);

        Vector2 shake = particles.GetScreenShakeOffset();
        Vector2 desiredTarget = { player.GetPosition().x + shake.x, player.GetPosition().y + shake.y };
        camera.target = Vector2Lerp(camera.target, desiredTarget, 8.0f * dt);
        return;
    }

    // 3. GAME OVER & VICTORY STATES
    if (currentState == GameState::GAME_OVER || currentState == GameState::VICTORY) {
        if (IsKeyPressed(KEY_R)) {
            EnterArena();
        }
        if (IsKeyPressed(KEY_L) || IsKeyPressed(KEY_ESCAPE)) {
            EnterLobby();
        }
        return;
    }

    // 4. ARENA STATE
    if (IsKeyPressed(KEY_R)) {
        EnterArena();
    }
    if (IsKeyPressed(KEY_L) || IsKeyPressed(KEY_ESCAPE)) {
        EnterLobby();
        return;
    }

    if (!player.IsAlive()) {
        currentState = GameState::GAME_OVER;
        return;
    }

    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

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

    player.Update(dt, map, mouseWorld, particles);

    // Flush player spawned projectiles (e.g. Truth Seeker Holy Beam)
    for (const auto& p : player.GetSpawnedProjectiles()) {
        projectiles.push_back(p);
    }
    player.GetSpawnedProjectiles().clear();

    int aliveCount = 0;
    for (auto& enemy : enemies) {
        if (enemy.IsAlive()) {
            enemy.Update(dt, player.GetPosition(), map, particles, projectiles);
            aliveCount++;
        }
    }

    for (auto& proj : projectiles) {
        if (proj.IsActive()) {
            proj.Update(dt, map, particles);
        }
    }
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) {
        return !p.IsActive();
    }), projectiles.end());

    bool victory = false;
    if (waveManager.Update(dt, aliveCount, enemies, projectiles, particles, { 1200.0f, 900.0f }, victory)) {
        if (victory) {
            currentState = GameState::VICTORY;
            particles.SpawnFloatingText(player.GetPosition(), "ARENA CLEARED!", Color{ 255, 230, 80, 255 }, 30.0f);
        }
    }

    std::vector<bool> enemyAliveBefore(enemies.size());
    for (size_t i = 0; i < enemies.size(); i++) {
        enemyAliveBefore[i] = enemies[i].IsAlive();
    }

    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera);

    // Chance to drop adhesive bandage on enemy defeat
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemyAliveBefore[i] && !enemies[i].IsAlive()) {
            if (arenaBandages.size() < 4 && (rand() % 100) < 35) {
                AdhesiveBandage b;
                b.position = enemies[i].GetPosition();
                arenaBandages.push_back(b);
                particles.SpawnSparks(b.position, Color{ 80, 255, 140, 255 }, 14, 110.0f);
            }
        }
    }

    // Periodic Arena Adhesive Bandage Spawning
    bandageSpawnTimer -= dt;
    if (bandageSpawnTimer <= 0.0f) {
        bandageSpawnTimer = 18.0f;
        if (arenaBandages.size() < 4) {
            float rx = 900.0f + (float)(rand() % 600);
            float ry = 650.0f + (float)(rand() % 500);
            AdhesiveBandage b;
            b.position = { rx, ry };
            arenaBandages.push_back(b);
            particles.SpawnSparks(b.position, Color{ 100, 255, 160, 255 }, 16, 120.0f);
            particles.SpawnFloatingText(b.position, "+ BANDAGE SPAWNED +", Color{ 120, 255, 170, 240 }, 18.0f);
        }
    }

    // Update & Pickup Adhesive Bandages
    for (auto& b : arenaBandages) {
        if (!b.active) continue;
        b.Update(dt);
        if (b.CheckPickup(player.GetPosition())) {
            player.Heal(b.healAmount);
            particles.SpawnFloatingText(player.GetPosition(), "+30 HP [BANDAGE]", Color{ 80, 255, 140, 255 }, 24.0f);
            particles.SpawnSparks(b.position, Color{ 100, 255, 160, 255 }, 20, 150.0f);
            SoundFX::PlayParrySuccess();
            b.active = false;
        }
    }
    arenaBandages.erase(std::remove_if(arenaBandages.begin(), arenaBandages.end(), [](const AdhesiveBandage& b) {
        return !b.active;
    }), arenaBandages.end());

    particles.Update(dt);

    Vector2 shake = particles.GetScreenShakeOffset();
    Vector2 desiredTarget = { player.GetPosition().x + shake.x, player.GetPosition().y + shake.y };
    camera.target = Vector2Lerp(camera.target, desiredTarget, 8.0f * dt);
}

void Game::Render() {
    BeginDrawing();

    int w = Config::WINDOW_WIDTH;
    int h = Config::WINDOW_HEIGHT;
    float time = (float)GetTime();

    if (currentState == GameState::TITLE_SCREEN) {
        Vector2 mouseScreen = GetMousePosition();
        GameUI::DrawTitleScreen(w, h, mouseScreen, false, testArenaUnlocked);

        if (showControlsModal) {
            GameUI::DrawControlsModal(w, h, mouseScreen, false);
        }
        EndDrawing();
        return;
    }

    if (currentState == GameState::PROLOGUE_CUTSCENE) {
        cutscene.Draw(w, h);
        EndDrawing();
        return;
    }

    if (currentState == GameState::BORDERLANDS_GATE) {
        ruinedCastleHandler.Render(
            camera, map, enemies, projectiles, player, particles, tutorial,
            w, h, false, false, 0, 0
        );
        Vector2 mousePos = GetMousePosition();
        GameUI::DrawBorderlandsGateway(w, h, mousePos, false, player);
        EndDrawing();
        return;
    }

    if (currentState == GameState::RUINED_CASTLE) {
        ruinedCastleHandler.Render(
            camera, map, enemies, projectiles, player, particles, tutorial,
            w, h, showControlsModal, showInventoryModal, inventoryActiveTab, inventorySelectedFragment
        );
        EndDrawing();
        return;
    }

    if (currentState == GameState::LOBBY) {
        BeginMode2D(camera);
        map.DrawFloor();

        portal.Draw(time, testArenaUnlocked);

        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        for (const auto& ped : pedestals) {
            bool isSel = (player.GetWeapon() == ped.weapon);
            bool isHov = CheckCollisionPointCircle(mouseWorld, ped.position, ped.radius) || ped.CheckInteraction(player.GetPosition());
            AssetManager::DrawWeaponPedestal(ped.position, ped.weapon, isSel, isHov, time);
        }

        dummy.Draw();
        map.DrawWallsAndPillars();
        player.Draw();
        particles.Draw();
        EndMode2D();

        Vector2 mousePos = GetMousePosition();
        DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 255, 255, 255, 180 });
        DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 0, 229, 255, 240 });

        bool nearAny = false;
        std::string nearName = "";
        for (const auto& ped : pedestals) {
            if (ped.CheckInteraction(player.GetPosition())) {
                nearAny = true;
                nearName = GetWeaponStats(ped.weapon).name;
                break;
            }
        }

        bool hasUnread = player.GetInventory().HasUnreadFragments();
        GameUI::DrawLobbyHUD(w, h, player.GetHealth(), player.GetMaxHealth(), player.GetStamina(), player.GetMaxStamina(), player.GetWeapon(), player.GetDashCooldown(), player.GetParryCooldown(), player.GetHeavyCooldown(), nearAny, nearName, hasUnread);

        if (showControlsModal) {
            GameUI::DrawControlsModal(w, h, mousePos, false);
        } else if (showInventoryModal) {
            GameUI::DrawInventoryModal(w, h, mousePos, false, player, inventoryActiveTab, inventorySelectedFragment);
        }

        EndDrawing();
        return;
    }

    // ARENA RENDERING
    BeginMode2D(camera);
    map.DrawFloor();

    for (const auto& enemy : enemies) {
        enemy.DrawTelegraph();
    }

    map.DrawWallsAndPillars();

    for (const auto& b : arenaBandages) {
        b.Draw();
    }

    for (const auto& proj : projectiles) {
        proj.Draw();
    }

    for (const auto& enemy : enemies) {
        enemy.Draw();
    }

    if (player.IsAlive()) {
        player.Draw();
    }

    particles.Draw();
    EndMode2D();

    Vector2 mousePos = GetMousePosition();
    DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 255, 255, 255, 180 });
    DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 0, 229, 255, 240 });

    bool hasUnread = player.GetInventory().HasUnreadFragments();
    GameUI::DrawCombatHUD(w, h, player.GetHealth(), player.GetMaxHealth(), player.GetStamina(), player.GetMaxStamina(), player.GetWeapon(), player.GetDashCooldown(), player.GetParryCooldown(), player.GetHeavyCooldown(), player.IsParrying(), player.HasCounterBuff(), waveManager.GetCurrentWave(), waveManager.GetTotalWaves(), hasUnread);

    // Boss Health Bar during Boss Wave
    for (const auto& enemy : enemies) {
        if (enemy.IsBoss() && enemy.IsAlive()) {
            GameUI::DrawBossHealthBar(w, enemy.GetName(), enemy.GetHealth(), enemy.GetMaxHealth());
            break;
        }
    }

    if (currentState == GameState::GAME_OVER) {
        GameUI::DrawGameOver(w, h);
    } else if (currentState == GameState::VICTORY) {
        GameUI::DrawVictory(w, h);
    }

    if (showControlsModal) {
        GameUI::DrawControlsModal(w, h, mousePos, false);
    } else if (showInventoryModal) {
        GameUI::DrawInventoryModal(w, h, mousePos, false, player, inventoryActiveTab, inventorySelectedFragment);
    }

    EndDrawing();
}

void Game::Run() {
    Init();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;

        Update(dt);
        Render();
    }
}
