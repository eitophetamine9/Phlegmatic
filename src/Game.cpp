#include "Game.hpp"
#include "Config.hpp"
#include "SoundFX.hpp"
#include "raymath.h"
#include <cmath>
#include <string>

Game::Game() : currentWave(1), waveTimer(0.0f), isGameOver(false), isVictory(false) {
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
}

void Game::ResetGame() {
    player.Init({ 1200.0f, 900.0f });
    currentWave = 1;
    isGameOver = false;
    isVictory = false;
    SpawnWave(currentWave);
}

void Game::SpawnWave(int waveNumber) {
    enemies.clear();
    Vector2 center = { 1200.0f, 900.0f };

    if (waveNumber == 1) {
        // Wave 1: 3 Slashers demonstrating Cone telegraphs
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 300.0f, center.y - 150.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 300.0f, center.y - 150.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x, center.y - 320.0f });
        particles.SpawnFloatingText(center, "TRIAL 1: CONE TELEGRAPHS", Color{ 255, 220, 80, 255 }, 24.0f);
    } else if (waveNumber == 2) {
        // Wave 2: 2 Chargers + 2 Slashers demonstrating fast line thrusts and parry timing
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 360.0f, center.y });
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 360.0f, center.y });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 180.0f, center.y + 260.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 180.0f, center.y + 260.0f });
        particles.SpawnFloatingText(center, "TRIAL 2: PARRY TIMING", Color{ 100, 240, 255, 255 }, 24.0f);
    } else if (waveNumber >= 3) {
        // Wave 3: The Brute Crusher Boss + 2 Slashers
        enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x, center.y - 280.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 280.0f, center.y + 200.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 280.0f, center.y + 200.0f });
        particles.SpawnFloatingText(center, "TRIAL 3: BRUTE CRUSHER", Color{ 255, 80, 80, 255 }, 26.0f);
    }
}

void Game::CheckCombatCollisions() {
    // 1. Player Attack vs Enemies
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

    // 2. Enemy Attacks vs Player
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
            if (!tookDamage) {
                // Was parried by player!
                enemy.OnParried(particles);
            }
        }
    }
}

void Game::Update(float dt) {
    storyboard.Update(dt);

    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }

    // Check game over
    if (!player.IsAlive()) {
        isGameOver = true;
        return;
    }

    // Mouse world coordinates
    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    // Player inputs
    if (!storyboard.IsOverlayOpen()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            player.TriggerLightAttack(particles);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            player.TriggerHeavyAttack(particles);
        }
        if (IsKeyPressed(KEY_Q)) {
            player.TriggerParry(particles);
        }
    }

    // Update Player
    player.Update(dt, map, mouseWorld, particles);

    // Update Enemies & count active
    int aliveCount = 0;
    for (auto& enemy : enemies) {
        if (enemy.IsAlive()) {
            enemy.Update(dt, player.GetPosition(), map, particles);
            aliveCount++;
        }
    }

    // Wave progression
    if (aliveCount == 0 && !isGameOver && !isVictory) {
        waveTimer += dt;
        if (waveTimer >= 1.5f) {
            waveTimer = 0.0f;
            currentWave++;
            if (currentWave <= 3) {
                SpawnWave(currentWave);
            } else {
                isVictory = true;
                particles.SpawnFloatingText(player.GetPosition(), "ARENA CLEARED!", Color{ 255, 230, 80, 255 }, 30.0f);
            }
        }
    }

    CheckCombatCollisions();
    particles.Update(dt);

    // Smooth camera tracking player with screen shake offset
    Vector2 shake = particles.GetScreenShakeOffset();
    Vector2 desiredTarget = { player.GetPosition().x + shake.x, player.GetPosition().y + shake.y };
    camera.target = Vector2Lerp(camera.target, desiredTarget, 8.0f * dt);
}

void Game::Render() {
    BeginDrawing();

    // 1. World Rendering (Camera space)
    BeginMode2D(camera);

    map.DrawFloor();

    // Draw enemy telegraph indicators on the ground (underneath characters)
    for (const auto& enemy : enemies) {
        enemy.DrawTelegraph();
    }

    map.DrawWallsAndPillars();

    // Draw enemies
    for (const auto& enemy : enemies) {
        enemy.Draw();
    }

    // Draw player
    if (player.IsAlive()) {
        player.Draw();
    }

    // Draw world particles & floating numbers
    particles.Draw();

    EndMode2D();

    // 2. Screen Space UI
    RenderUI();

    EndDrawing();
}

void Game::RenderUI() {
    int w = Config::WINDOW_WIDTH;
    int h = Config::WINDOW_HEIGHT;

    // Crosshair at mouse position
    Vector2 mousePos = GetMousePosition();
    DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 255, 255, 255, 180 });
    DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 0, 229, 255, 240 });

    // Top Storyboard Banner
    storyboard.DrawHUD(w, h);

    // Player HUD Panel (Bottom Left)
    const float hudX = 24.0f;
    const float hudY = (float)h - 100.0f;
    const float hudWidth = 260.0f;
    const float hudHeight = 78.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    // Health Bar
    float hpPercent = player.GetHealth() / player.GetMaxHealth();
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 68, (int)hudY + 12, 172, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 68, (int)hudY + 12, (int)(172 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 68, (int)hudY + 12, 172, 12, Color{ 80, 40, 50, 220 });

    // Stamina Bar
    float stamPercent = player.GetStamina() / player.GetMaxStamina();
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 68, (int)hudY + 32, 172, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 68, (int)hudY + 32, (int)(172 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 68, (int)hudY + 32, 172, 12, Color{ 90, 75, 30, 220 });

    // Counter Buff / Status text
    if (player.HasCounterBuff()) {
        DrawText("COUNTER BUFF: +50% DAMAGE!", (int)hudX + 14, (int)hudY + 54, 12, Color{ 255, 230, 80, 255 });
    } else if (player.IsParrying()) {
        DrawText("PARRY WINDOW ACTIVE!", (int)hudX + 14, (int)hudY + 54, 12, Color{ 80, 240, 255, 255 });
    } else {
        DrawText("[Q] Parry Ready", (int)hudX + 14, (int)hudY + 54, 11, Color{ 150, 170, 195, 220 });
    }

    // Bottom Right Controls Tooltip Panel
    const float ctrlW = 310.0f;
    const float ctrlH = 78.0f;
    float ctrlX = (float)w - ctrlW - 24.0f;
    float ctrlY = (float)h - ctrlH - 24.0f;

    DrawRectangleRounded(Rectangle{ ctrlX, ctrlY, ctrlW, ctrlH }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ ctrlX, ctrlY, ctrlW, ctrlH }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    DrawText("CONTROLS:", (int)ctrlX + 12, (int)ctrlY + 10, 11, Color{ 255, 210, 80, 255 });
    DrawText("WASD / Arrows : Move", (int)ctrlX + 12, (int)ctrlY + 28, 11, Color{ 200, 215, 235, 240 });
    DrawText("Left Click    : Light Attack", (int)ctrlX + 12, (int)ctrlY + 44, 11, Color{ 120, 220, 255, 240 });
    DrawText("Right Click   : Heavy Attack", (int)ctrlX + 160, (int)ctrlY + 28, 11, Color{ 255, 180, 70, 240 });
    DrawText("Q Key         : Parry (Timely)", (int)ctrlX + 160, (int)ctrlY + 44, 11, Color{ 255, 230, 90, 255 });
    DrawText("[R] Restart   |  [TAB] Storyboard", (int)ctrlX + 12, (int)ctrlY + 60, 10, Color{ 150, 170, 195, 220 });

    // Trial / Wave counter (Top Right)
    std::string waveStr = "Wave " + std::to_string(currentWave) + " / 3";
    int waveTextWidth = MeasureText(waveStr.c_str(), 16);
    DrawRectangleRounded(Rectangle{ (float)(w - waveTextWidth - 40), 16.0f, (float)(waveTextWidth + 24), 32.0f }, 0.3f, 4, Color{ 16, 20, 28, 210 });
    DrawText(waveStr.c_str(), w - waveTextWidth - 28, 24, 16, Color{ 255, 225, 100, 255 });

    // Game Over Overlay
    if (isGameOver) {
        DrawRectangle(0, 0, w, h, Color{ 20, 5, 8, 210 });
        int titleW = MeasureText("FALLEN IN BATTLE", 38);
        DrawText("FALLEN IN BATTLE", (w - titleW) / 2, h / 2 - 40, 38, Color{ 255, 50, 60, 255 });
        int subW = MeasureText("Press [R] to Respawn and Try Again", 18);
        DrawText("Press [R] to Respawn and Try Again", (w - subW) / 2, h / 2 + 18, 18, Color{ 220, 225, 240, 230 });
    }

    // Victory Overlay
    if (isVictory) {
        DrawRectangle(0, 0, w, h, Color{ 10, 24, 18, 210 });
        int titleW = MeasureText("TRIALS CONQUERED", 38);
        DrawText("TRIALS CONQUERED", (w - titleW) / 2, h / 2 - 40, 38, Color{ 255, 225, 80, 255 });
        int subW = MeasureText("Press [R] to Replay Trials  |  Press [TAB] for Storyboard Notes", 18);
        DrawText("Press [R] to Replay Trials  |  Press [TAB] for Storyboard Notes", (w - subW) / 2, h / 2 + 18, 18, Color{ 210, 240, 225, 230 });
    }

    // Storyboard Modal overlay if active
    storyboard.DrawOverlay(w, h);
}

void Game::Run() {
    Init();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f; // prevent spiral of death on lag

        Update(dt);
        Render();
    }
}
