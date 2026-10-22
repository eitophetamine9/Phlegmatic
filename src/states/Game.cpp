#include "Game.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "raymath.h"
#include <cmath>
#include <string>

Game::Game() : currentState(GameState::TITLE_SCREEN), currentWave(1), waveTimer(0.0f) {
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

    // Setup Lobby Pedestals
    pedestals.clear();
    pedestals.push_back({ { 650.0f, 650.0f }, WeaponType::SWORD });
    pedestals.push_back({ { 800.0f, 590.0f }, WeaponType::DAGGERS });
    pedestals.push_back({ { 950.0f, 650.0f }, WeaponType::MACE });

    dummy.position = { 800.0f, 810.0f };
    portal.position = { 800.0f, 430.0f };

    camera.target = { 800.0f, 600.0f };
    camera.offset = { (float)Config::WINDOW_WIDTH * 0.5f, (float)Config::WINDOW_HEIGHT * 0.5f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    currentState = GameState::TITLE_SCREEN;
}

void Game::EnterLobby() {
    map.Init(MapEnvironment::LOBBY);
    player.Init({ 800.0f, 740.0f });
    camera.target = player.GetPosition();
    currentState = GameState::LOBBY;
    particles.SpawnFloatingText(player.GetPosition(), "LOBBY SANCTUARY", Color{ 255, 220, 100, 255 }, 24.0f);
}

void Game::EnterArena() {
    map.Init(MapEnvironment::ARENA);
    player.Init({ 1200.0f, 900.0f });
    camera.target = player.GetPosition();
    currentWave = 1;
    waveTimer = 0.0f;
    SpawnWave(currentWave);
    currentState = GameState::ARENA;
    SoundFX::PlayEquipWeapon();
    particles.SpawnSparks(player.GetPosition(), Color{ 100, 220, 255, 255 }, 25, 220.0f);
}

void Game::SpawnWave(int waveNumber) {
    enemies.clear();
    Vector2 center = { 1200.0f, 900.0f };

    if (waveNumber == 1) {
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 300.0f, center.y - 150.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 300.0f, center.y - 150.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x, center.y - 320.0f });
        particles.SpawnFloatingText(center, "WAVE 1", Color{ 255, 220, 80, 255 }, 24.0f);
    } else if (waveNumber == 2) {
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 360.0f, center.y });
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 360.0f, center.y });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 180.0f, center.y + 260.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 180.0f, center.y + 260.0f });
        particles.SpawnFloatingText(center, "WAVE 2", Color{ 100, 240, 255, 255 }, 24.0f);
    } else if (waveNumber >= 3) {
        enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x, center.y - 280.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 280.0f, center.y + 200.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 280.0f, center.y + 200.0f });
        particles.SpawnFloatingText(center, "WAVE 3", Color{ 255, 80, 80, 255 }, 26.0f);
    }
}

void Game::CheckCombatCollisions() {
    if (player.IsAttacking() && !player.GetCurrentAttack().hasHit) {
        const auto& attack = player.GetCurrentAttack();
        bool hitAny = false;

        for (auto& enemy : enemies) {
            if (enemy.IsAlive() && enemy.CheckHit(attack)) {
                Vector2 knockbackDir = Vector2Normalize({
                    enemy.GetPosition().x - player.GetPosition().x,
                    enemy.GetPosition().y - player.GetPosition().y
                });
                enemy.TakeDamage(attack.damage, knockbackDir, particles);
                hitAny = true;
            }
        }

        if (hitAny) {
            player.SetAttackHit();
            particles.AddScreenShake(attack.type == AttackType::HEAVY ? 5.0f : 2.5f, 0.1f);
        }
    }

    Vector2 playerPos = player.GetPosition();
    float playerRadius = player.GetRadius();

    for (auto& enemy : enemies) {
        if (!enemy.IsAlive() || !enemy.IsInStrike()) continue;

        TelegraphIndicator tel = enemy.GetTelegraph();
        bool playerHit = false;

        if (tel.shape == TelegraphShape::CONE) {
            float dx = playerPos.x - enemy.GetPosition().x;
            float dy = playerPos.y - enemy.GetPosition().y;
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist <= tel.radius + playerRadius) {
                float hitAngle = atan2f(dy, dx) * RAD2DEG;
                float diff = fabsf(hitAngle - tel.angle);
                while (diff > 180.0f) diff = 360.0f - diff;
                if (diff <= (tel.arcAngle * 0.5f + 12.0f)) {
                    playerHit = true;
                }
            }
        } else if (tel.shape == TelegraphShape::CIRCLE) {
            float dx = playerPos.x - enemy.GetPosition().x;
            float dy = playerPos.y - enemy.GetPosition().y;
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist <= tel.radius + playerRadius) {
                playerHit = true;
            }
        } else if (tel.shape == TelegraphShape::LINE) {
            float rad = tel.angle * DEG2RAD;
            Vector2 dir = { cosf(rad), sinf(rad) };
            Vector2 normal = { -dir.y, dir.x };
            Vector2 toPlayer = { playerPos.x - enemy.GetPosition().x, playerPos.y - enemy.GetPosition().y };
            float projDist = toPlayer.x * dir.x + toPlayer.y * dir.y;
            float perpDist = fabsf(toPlayer.x * normal.x + toPlayer.y * normal.y);
            if (projDist >= -playerRadius && projDist <= tel.radius + playerRadius && perpDist <= (tel.width * 0.5f + playerRadius)) {
                playerHit = true;
            }
        }

        if (playerHit) {
            enemy.MarkDamagedPlayer();
            Vector2 knockbackDir = Vector2Normalize({
                playerPos.x - enemy.GetPosition().x,
                playerPos.y - enemy.GetPosition().y
            });

            float baseDamage = (enemy.GetType() == EnemyType::BRUTE) ? 38.0f : ((enemy.GetType() == EnemyType::CHARGER) ? 22.0f : 18.0f);

            bool tookDamage = player.TakeDamage(baseDamage, knockbackDir, particles);
            if (!tookDamage && player.IsParrying()) {
                enemy.OnParried(particles);
            }
        }
    }
}

void Game::Update(float dt) {
    Vector2 mouseScreen = GetMousePosition();

    // 1. TITLE SCREEN STATE
    if (currentState == GameState::TITLE_SCREEN) {
        bool clickedPlay = GameUI::DrawTitleScreen(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        if (clickedPlay) {
            EnterLobby();
        }
        return;
    }

    // 2. LOBBY STATE
    if (currentState == GameState::LOBBY) {
        Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

        // Player Actions
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            player.TriggerLightAttack(particles);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            player.TriggerHeavyAttack(particles);
        }
        if (IsKeyPressed(KEY_Q)) {
            player.TriggerDash(particles);
        }
        if (IsKeyPressed(KEY_F)) {
            player.TriggerParry(particles);
        }

        player.Update(dt, map, mouseWorld, particles);
        dummy.Update(dt);

        // Test attacks on Training Dummy
        if (player.IsAttacking() && !player.GetCurrentAttack().hasHit) {
            if (dummy.CheckHit(player.GetCurrentAttack(), particles)) {
                player.SetAttackHit();
                particles.AddScreenShake(2.5f, 0.1f);
            }
        }

        // Weapon Pedestal Interactions
        for (const auto& ped : pedestals) {
            bool near = ped.CheckInteraction(player.GetPosition());
            bool clicked = CheckCollisionPointCircle(mouseWorld, ped.position, ped.radius) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if ((near && IsKeyPressed(KEY_E)) || clicked) {
                if (player.GetWeapon() != ped.weapon) {
                    player.SetWeapon(ped.weapon);
                    WeaponStats st = GetWeaponStats(ped.weapon);
                    particles.SpawnFloatingText(player.GetPosition(), "EQUIPPED: " + st.name, Color{ 255, 220, 80, 255 }, 22.0f);
                    particles.SpawnSparks(ped.position, Color{ 100, 220, 255, 255 }, 16, 120.0f);
                }
            }
        }

        // Arena Portal Interaction
        if (portal.CheckPlayerInside(player.GetPosition()) || (Vector2Distance(player.GetPosition(), portal.position) < 80.0f && IsKeyPressed(KEY_E))) {
            EnterArena();
            return;
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
        player.TriggerHeavyAttack(particles);
    }
    if (IsKeyPressed(KEY_Q)) {
        player.TriggerDash(particles);
    }
    if (IsKeyPressed(KEY_F)) {
        player.TriggerParry(particles);
    }

    player.Update(dt, map, mouseWorld, particles);

    int aliveCount = 0;
    for (auto& enemy : enemies) {
        if (enemy.IsAlive()) {
            enemy.Update(dt, player.GetPosition(), map, particles);
            aliveCount++;
        }
    }

    if (aliveCount == 0 && currentState == GameState::ARENA) {
        waveTimer += dt;
        if (waveTimer >= 1.5f) {
            waveTimer = 0.0f;
            currentWave++;
            if (currentWave <= 3) {
                SpawnWave(currentWave);
            } else {
                currentState = GameState::VICTORY;
                particles.SpawnFloatingText(player.GetPosition(), "ARENA CLEARED!", Color{ 255, 230, 80, 255 }, 30.0f);
            }
        }
    }

    CheckCombatCollisions();
    particles.Update(dt);

    Vector2 shake = particles.GetScreenShakeOffset();
    Vector2 desiredTarget = { player.GetPosition().x + shake.x, player.GetPosition().y + shake.y };
    camera.target = Vector2Lerp(camera.target, desiredTarget, 8.0f * dt);
}

void Game::Render() {
    BeginDrawing();

    // TITLE SCREEN
    if (currentState == GameState::TITLE_SCREEN) {
        Vector2 mouseScreen = GetMousePosition();
        GameUI::DrawTitleScreen(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, false);
        EndDrawing();
        return;
    }

    float time = (float)GetTime();
    int w = Config::WINDOW_WIDTH;
    int h = Config::WINDOW_HEIGHT;

    // LOBBY RENDERING
    if (currentState == GameState::LOBBY) {
        BeginMode2D(camera);
        map.DrawFloor();

        // Draw Arena Portal
        portal.Draw(time);

        // Draw Pedestals
        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        for (const auto& ped : pedestals) {
            bool isSel = (player.GetWeapon() == ped.weapon);
            bool isHov = CheckCollisionPointCircle(mouseWorld, ped.position, ped.radius) || ped.CheckInteraction(player.GetPosition());
            AssetManager::DrawWeaponPedestal(ped.position, ped.weapon, isSel, isHov, time);
        }

        // Draw Training Dummy
        dummy.Draw();

        map.DrawWallsAndPillars();

        // Draw Player
        player.Draw();

        particles.Draw();
        EndMode2D();

        // Lobby HUD
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

        GameUI::DrawLobbyHUD(w, h, player.GetHealth(), player.GetMaxHealth(), player.GetStamina(), player.GetMaxStamina(), player.GetWeapon(), player.GetDashCooldown(), player.GetParryCooldown(), nearAny, nearName);

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

    for (const auto& enemy : enemies) {
        enemy.Draw();
    }

    if (player.IsAlive()) {
        player.Draw();
    }

    particles.Draw();
    EndMode2D();

    // Arena Screen UI
    Vector2 mousePos = GetMousePosition();
    DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 255, 255, 255, 180 });
    DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 0, 229, 255, 240 });

    GameUI::DrawCombatHUD(w, h, player.GetHealth(), player.GetMaxHealth(), player.GetStamina(), player.GetMaxStamina(), player.GetWeapon(), player.GetDashCooldown(), player.GetParryCooldown(), player.IsParrying(), player.HasCounterBuff(), currentWave, 3);

    if (currentState == GameState::GAME_OVER) {
        GameUI::DrawGameOver(w, h);
    } else if (currentState == GameState::VICTORY) {
        GameUI::DrawVictory(w, h);
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
