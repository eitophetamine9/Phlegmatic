#include "CrossroadsHandler.hpp"
#include "UI.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/CollisionSystem.hpp"
#include "../rendering/AssetManager.hpp"
#include "raymath.h"
#include <algorithm>
#include <cmath>

void HiddenTotemChest::Draw(float time) const {
    DrawEllipse((int)position.x, (int)(position.y + 12), radius * 1.15f, radius * 0.55f, Color{ 10, 12, 18, 140 });

    if (!opened) {
        // Closed Relic Chest with pulsating Totem aura
        DrawRectangle((int)(position.x - 22), (int)(position.y - 14), 44, 28, Color{ 48, 56, 72, 255 });
        DrawRectangleLines((int)(position.x - 22), (int)(position.y - 14), 44, 28, Color{ 25, 32, 44, 255 });

        // Gilded runes
        DrawRectangle((int)(position.x - 16), (int)(position.y - 14), 6, 28, Color{ 100, 235, 180, 220 });
        DrawRectangle((int)(position.x + 10), (int)(position.y - 14), 6, 28, Color{ 100, 235, 180, 220 });

        float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
        DrawCircle((int)position.x, (int)position.y, 6.0f, Color{ 120, 255, 200, 255 });
        DrawCircleGradient(position, 32.0f * pulse, Color{ 100, 235, 180, (unsigned char)(75 * pulse) }, Color{ 0, 0, 0, 0 });

        const char* label = "[E] Hidden Relic Chest";
        int lw = MeasureText(label, 11);
        DrawText(label, (int)(position.x - lw * 0.5f), (int)(position.y - radius - 16), 11, Color{ 120, 255, 200, 255 });
    } else {
        // Open chest with residual soft aura
        DrawRectangle((int)(position.x - 22), (int)(position.y - 8), 44, 22, Color{ 36, 42, 54, 255 });
        DrawRectangleLines((int)(position.x - 22), (int)(position.y - 8), 44, 22, Color{ 24, 28, 38, 255 });
        DrawRectangle((int)(position.x - 24), (int)(position.y - 20), 48, 12, Color{ 55, 65, 85, 255 });
        DrawCircleGradient(position, 20.0f, Color{ 100, 235, 180, 30 }, Color{ 0, 0, 0, 0 });
    }
}

CrossroadsHandler::CrossroadsHandler() {
    chest.position = { 510.0f, 950.0f };
    chest.opened = false;
    showObjectives = true;
    enemiesSpawned = false;
    entryBannerTimer = 4.0f;
}

void CrossroadsHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    map.Init(MapEnvironment::CROSSROADS);
    map.SetGateOpen(true);

    player.SetPosition({ 800.0f, 1680.0f });
    camera.target = player.GetPosition();

    enemies.clear();
    projectiles.clear();

    // Guarding corrupted hostiles along the Crossroads highway
    enemies.emplace_back(EnemyType::SLASHER, Vector2{ 740.0f, 1300.0f });
    enemies.emplace_back(EnemyType::SLASHER, Vector2{ 860.0f, 1000.0f });
    enemies.emplace_back(EnemyType::BRUTE, Vector2{ 800.0f, 650.0f });

    chest.position = { 510.0f, 950.0f };
    chest.opened = player.GetInventory().HasTotem(TotemType::TOTEM_SANITY);
    enemiesSpawned = true;
    entryBannerTimer = 4.0f;
}

void CrossroadsHandler::Update(
    float dt,
    Player& player,
    GameMap& map,
    ParticleSystem& particles,
    std::vector<Enemy>& enemies,
    std::vector<Projectile>& projectiles,
    Camera2D& camera,
    GameState& currentState,
    const std::function<void()>& onEnterLobby
) {
    Vector2 mouseScreen = GetMousePosition();

    if (entryBannerTimer > 0.0f) {
        entryBannerTimer -= dt;
    }

    if (!player.IsAlive()) {
        if (IsKeyPressed(KEY_R)) {
            player.Revive({ 800.0f, 1680.0f }, 1.0f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ 740.0f, 1300.0f });
            enemies.emplace_back(EnemyType::SLASHER, Vector2{ 860.0f, 1000.0f });
            enemies.emplace_back(EnemyType::BRUTE, Vector2{ 800.0f, 650.0f });
            return;
        }
        if (IsKeyPressed(KEY_L)) {
            if (onEnterLobby) onEnterLobby();
            return;
        }
    }

    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    if (player.IsAlive()) {
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
    }

    if (IsKeyPressed(KEY_L)) {
        if (onEnterLobby) onEnterLobby();
        return;
    }

    player.Update(dt, map, mouseWorld, particles);

    // Flush projectiles
    for (const auto& p : player.GetSpawnedProjectiles()) {
        projectiles.push_back(p);
    }
    player.GetSpawnedProjectiles().clear();

    for (auto& enemy : enemies) {
        enemy.Update(dt, player.GetPosition(), map, particles, projectiles);
    }

    for (auto& proj : projectiles) {
        if (proj.IsActive()) {
            proj.Update(dt, map, particles);
        }
    }
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) {
        return !p.IsActive();
    }), projectiles.end());

    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera);

    // Hidden Chest Interaction
    Vector2 playerPos = player.GetPosition();
    if (!chest.opened && chest.CheckInteraction(playerPos) && (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))) {
        chest.opened = true;
        player.GetInventory().UnlockTotem(TotemType::TOTEM_SANITY);
        player.SetTotem(TotemType::TOTEM_SANITY);
        SoundFX::PlayParrySuccess();
        particles.SpawnFloatingText(chest.position, "ACQUIRED: Totem of Sanity (+15% Damage)!", Color{ 100, 235, 180, 255 }, 24.0f);
        particles.AddScreenShake(5.0f, 0.2f);
    }

    // Northern Gateway into Zyphden (Y <= bounds.y + 80.0f)
    bool atNorthernGate = (player.GetPosition().y <= map.GetBounds().y + 80.0f &&
                           player.GetPosition().x >= 680.0f && player.GetPosition().x <= 920.0f);
    bool interactGate = (Vector2Distance(player.GetPosition(), Vector2{ 800.0f, map.GetBounds().y + 30.0f }) < 120.0f) &&
                        (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER));

    if (atNorthernGate || interactGate) {
        currentState = GameState::BORDERLANDS_GATE;
        SoundFX::PlayGateOpen();
        particles.SpawnSparks(player.GetPosition(), Color{ 255, 215, 80, 255 }, 40, 220.0f);
        return;
    }

    camera.target.x += (player.GetPosition().x - camera.target.x) * 0.08f;
    camera.target.y += (player.GetPosition().y - camera.target.y) * 0.08f;
}

void CrossroadsHandler::Render(
    Camera2D& camera,
    const GameMap& map,
    const std::vector<Enemy>& enemies,
    const std::vector<Projectile>& projectiles,
    const Player& player,
    const ParticleSystem& particles,
    int screenWidth,
    int screenHeight,
    bool showControlsModal,
    bool showInventoryModal,
    int inventoryActiveTab,
    int inventorySelectedFragment
) {
    BeginMode2D(camera);
    {
        map.DrawFloor();

        // Draw Hidden Totem of Sanity Chest
        chest.Draw((float)GetTime());

        for (const auto& enemy : enemies) {
            enemy.Draw();
        }

        player.Draw();

        for (const auto& proj : projectiles) {
            proj.Draw();
        }

        particles.Draw();

        map.DrawWallsAndPillars();
    }
    EndMode2D();

    // Location Header: "CROSSROADS"
    GameUI::DrawCampaignHUD(
        screenWidth,
        screenHeight,
        player.GetHealth(),
        player.GetMaxHealth(),
        player.GetStamina(),
        player.GetMaxStamina(),
        player.GetWeapon(),
        player.GetDashCooldown(),
        player.GetParryCooldown(),
        player.GetHeavyCooldown(),
        player.IsParrying(),
        player.HasCounterBuff(),
        "CROSSROADS",
        player.GetInventory().HasUnreadFragments()
    );

    // Objectives Tracker on Middle-Left (toggleable with [O])
    if (IsKeyPressed(KEY_O)) {
        showObjectives = !showObjectives;
    }

    float bannerX = 24.0f;
    float bannerY = (float)screenHeight * 0.38f;
    Vector2 mousePos = GetMousePosition();
    bool isPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (!showObjectives) {
        const float tabW = 160.0f;
        const float tabH = 34.0f;
        Rectangle tabRec = { bannerX, bannerY, tabW, tabH };
        bool isHovered = CheckCollisionPointRec(mousePos, tabRec);

        if (isHovered && isPressed) {
            showObjectives = true;
            SoundFX::PlayButtonClick();
        }

        DrawRectangleRounded(tabRec, 0.25f, 4, isHovered ? Color{ 36, 48, 72, 230 } : Color{ 18, 24, 36, 210 });
        DrawRectangleRoundedLines(tabRec, 0.25f, 4, isHovered ? Color{ 255, 215, 80, 240 } : Color{ 60, 80, 115, 200 });
        DrawText("OBJECTIVES [O]", (int)bannerX + 16, (int)bannerY + 10, 11, isHovered ? Color{ 255, 225, 100, 255 } : Color{ 200, 215, 235, 220 });
    } else {
        const float bannerW = 340.0f;
        const float bannerH = 100.0f;
        Rectangle bannerRec = { bannerX, bannerY, bannerW, bannerH };

        DrawRectangleRounded(bannerRec, 0.12f, 4, Color{ 16, 20, 30, 230 });
        DrawRectangleRoundedLines(bannerRec, 0.12f, 4, Color{ 60, 80, 115, 240 });

        DrawText("CURRENT OBJECTIVE", (int)bannerX + 16, (int)bannerY + 12, 11, Color{ 255, 215, 80, 255 });

        Rectangle minRec = { bannerX + bannerW - 78.0f, bannerY + 8.0f, 66.0f, 20.0f };
        bool isMinHov = CheckCollisionPointRec(mousePos, minRec);
        if (isMinHov && isPressed) {
            showObjectives = false;
            SoundFX::PlayButtonClick();
        }
        DrawRectangleRounded(minRec, 0.2f, 4, isMinHov ? Color{ 45, 60, 90, 220 } : Color{ 25, 32, 48, 200 });
        DrawText("HIDE [O]", (int)minRec.x + 8, (int)minRec.y + 5, 9, isMinHov ? Color{ 255, 255, 255, 255 } : Color{ 170, 190, 215, 220 });

        DrawText("Follow the ancient highway north to reach Zyphden.", (int)bannerX + 16, (int)bannerY + 34, 11, Color{ 240, 245, 255, 240 });
        DrawText("Search roadside ruins west for the Totem of Sanity.", (int)bannerX + 16, (int)bannerY + 52, 11, Color{ 140, 210, 255, 230 });
        DrawText("Walk north [Y <= 240] to enter Zyphden kingdom.", (int)bannerX + 16, (int)bannerY + 74, 10, Color{ 255, 205, 80, 220 });
    }

    if (!player.IsAlive()) {
        GameUI::DrawGameOver(screenWidth, screenHeight);
    }

    if (showControlsModal) {
        GameUI::DrawControlsModal(screenWidth, screenHeight, mousePos, isPressed);
    }
    if (showInventoryModal) {
        GameUI::DrawInventoryModal(screenWidth, screenHeight, mousePos, isPressed, const_cast<Player&>(player), inventoryActiveTab, inventorySelectedFragment);
    }
}
