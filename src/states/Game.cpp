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

    camera.target = player.GetPosition();
    camera.offset = { (float)Config::WINDOW_WIDTH * 0.5f, (float)Config::WINDOW_HEIGHT * 0.5f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    ResetGame();
    currentState = GameState::TITLE_SCREEN;
}

void Game::ResetGame() {
    player.Init({ 1200.0f, 900.0f });
    currentWave = 1;
    waveTimer = 0.0f;
    SpawnWave(currentWave);
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

    // TITLE SCREEN STATE
    if (currentState == GameState::TITLE_SCREEN) {
        bool clickedPlay = GameUI::DrawTitleScreen(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        if (clickedPlay) {
            ResetGame();
            currentState = GameState::ARENA;
        }
        return;
    }

    // GAME OVER & VICTORY STATES
    if (currentState == GameState::GAME_OVER || currentState == GameState::VICTORY) {
        if (IsKeyPressed(KEY_R)) {
            ResetGame();
            currentState = GameState::ARENA;
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            currentState = GameState::TITLE_SCREEN;
        }
        return;
    }

    // ARENA STATE
    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GameState::TITLE_SCREEN;
        return;
    }

    if (!player.IsAlive()) {
        currentState = GameState::GAME_OVER;
        return;
    }

    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    // Player inputs:
    // Left Click: Light Attack
    // Right Click: Heavy Attack
    // Q: Dash
    // F: Parry
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

    if (currentState == GameState::TITLE_SCREEN) {
        Vector2 mouseScreen = GetMousePosition();
        GameUI::DrawTitleScreen(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, false);
        EndDrawing();
        return;
    }

    // World Rendering
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

    // HUD & Screen UI
    int w = Config::WINDOW_WIDTH;
    int h = Config::WINDOW_HEIGHT;

    // Crosshair
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
