#include "RuinedCastleHandler.hpp"
#include "UI.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/CollisionSystem.hpp"
#include "raymath.h"
#include <algorithm>

RuinedCastleHandler::RuinedCastleHandler() {
    Reset();
}

void RuinedCastleHandler::Reset() {
    zilEncounterActive = false;
    zilDefeated = false;
    zilSparedPlayer = false;
    hasUsedZilRetry = false;
    showZilDefeatModal = false;
    zilDialogueTimer = 0.0f;
    zilDialogueSpeaker = "";
    zilDialogueText = "";
    zilDialogueColor = WHITE;
}

void RuinedCastleHandler::Update(
    float dt,
    Player& player,
    GameMap& map,
    ParticleSystem& particles,
    std::vector<Enemy>& enemies,
    std::vector<Projectile>& projectiles,
    TutorialManager& tutorial,
    Camera2D& camera,
    GameState& currentState,
    bool& testArenaUnlocked,
    const std::function<void()>& onSaveArenaUnlock,
    const std::function<void()>& onEnterLobby,
    const std::function<void()>& onEnterRuinedCastle
) {
    Vector2 mouseScreen = GetMousePosition();

    // Interactive Tutorial Pop-up Modal (pauses combat and gameplay while visible)
    if (tutorial.HasActivePopup()) {
        tutorial.DrawPopup(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        return;
    }

    // Handle Zil Defeat Modal Interaction
    if (showZilDefeatModal) {
        int choice = GameUI::DrawZilDefeatModal(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), !hasUsedZilRetry);
        if (choice == 1) { // Retry Duel (1 attempt only)
            hasUsedZilRetry = true;
            showZilDefeatModal = false;
            player.Revive({ 800.0f, 520.0f }, 1.0f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();
            enemies.emplace_back(EnemyType::ZIL_BOSS, Vector2{ 800.0f, 290.0f });
            zilEncounterActive = true;
            zilDialogueSpeaker = "ZIL THE CORRUPTED CRUSADER";
            zilDialogueText = "“Rise then, and face Truth Seeker once more!”";
            zilDialogueColor = Color{ 255, 170, 40, 255 };
            zilDialogueTimer = 4.0f;
            particles.SpawnFloatingText({ 800.0f, 290.0f }, "DUEL RESTARTED!", Color{ 255, 200, 60, 255 }, 24.0f);
            return;
        } else if (choice == 2) { // Accept Spare & Proceed
            testArenaUnlocked = true;
            if (onSaveArenaUnlock) onSaveArenaUnlock();
            showZilDefeatModal = false;
            zilEncounterActive = false;
            zilSparedPlayer = true;
            player.Revive({ 800.0f, 420.0f }, 0.6f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();
            map.SetGateOpen(true);
            zilDialogueSpeaker = "ZIL THE CORRUPTED CRUSADER";
            zilDialogueText = "“Go then, wanderer. Carry your blade north into Zyphden... uncover the truth of Charlz.”";
            zilDialogueColor = Color{ 140, 210, 255, 255 };
            zilDialogueTimer = 6.0f;
            particles.SpawnFloatingText(player.GetPosition(), "SPARED BY ZIL — NORTH GATE OPEN!", Color{ 140, 220, 255, 255 }, 24.0f);
            return;
        }
        return;
    }

    if (!player.IsAlive()) {
        if (zilEncounterActive) {
            showZilDefeatModal = true;
            return;
        }
        if (IsKeyPressed(KEY_R)) {
            if (onEnterRuinedCastle) onEnterRuinedCastle();
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
            if (tutorial.HasClaimedWeapon()) {
                player.TriggerLightAttack(particles);
            } else {
                particles.SpawnFloatingText(player.GetPosition(), "CLAIM BLADE FIRST [E]!", Color{ 255, 180, 80, 240 }, 16.0f);
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            if (tutorial.HasClaimedWeapon()) {
                player.TriggerHeavyAttack(particles, mouseWorld);
            } else {
                particles.SpawnFloatingText(player.GetPosition(), "CLAIM BLADE FIRST [E]!", Color{ 255, 180, 80, 240 }, 16.0f);
            }
        }
        if (IsKeyPressed(KEY_Q)) {
            player.TriggerDash(particles);
        }
        if (IsKeyPressed(KEY_F)) {
            player.TriggerParry(particles);
        }
    }

    // Shortcuts
    if (IsKeyPressed(KEY_L)) {
        if (onEnterLobby) onEnterLobby();
        return;
    }
    if (IsKeyPressed(KEY_R)) {
        if (onEnterRuinedCastle) onEnterRuinedCastle();
        return;
    }

    player.Update(dt, map, mouseWorld, particles);

    // Flush player spawned projectiles (e.g. Truth Seeker Holy Beam)
    for (const auto& p : player.GetSpawnedProjectiles()) {
        projectiles.push_back(p);
    }
    player.GetSpawnedProjectiles().clear();

    // Update enemies
    for (auto& enemy : enemies) {
        enemy.Update(dt, player.GetPosition(), map, particles, projectiles);
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

    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera);

    // Tutorial progression
    tutorial.Update(dt, player, map, particles, enemies, projectiles);

    // Zil Boss Encounter Trigger
    if (tutorial.GetStep() == TutorialStep::COMPLETED && !zilEncounterActive && !zilDefeated && !zilSparedPlayer) {
        if (player.GetPosition().y <= 420.0f) {
            zilEncounterActive = true;
            enemies.clear();
            projectiles.clear();
            enemies.emplace_back(EnemyType::ZIL_BOSS, Vector2{ 800.0f, 290.0f });
            zilDialogueSpeaker = "ZIL THE CORRUPTED CRUSADER";
            zilDialogueText = "“Halt, intruder! None shall pass to Zyphden without testing their blade against Truth Seeker!”";
            zilDialogueColor = Color{ 255, 170, 40, 255 };
            zilDialogueTimer = 5.0f;
            particles.AddScreenShake(6.0f, 0.25f);
            SoundFX::PlayHeavyAttack();
        }
    }

    // Zil Boss Defeat Check
    if (zilEncounterActive) {
        bool zilAlive = false;
        for (auto& enemy : enemies) {
            if (enemy.GetType() == EnemyType::ZIL_BOSS && enemy.IsAlive()) {
                zilAlive = true;
                break;
            }
        }

        if (!zilAlive && !zilDefeated) {
            zilDefeated = true;
            testArenaUnlocked = true;
            if (onSaveArenaUnlock) onSaveArenaUnlock();
            zilEncounterActive = false;
            enemies.clear();
            projectiles.clear();

            // Award Truth Seeker!
            player.GetInventory().UnlockWeapon(WeaponType::TRUTH_SEEKER);
            player.SetWeapon(WeaponType::TRUTH_SEEKER);

            // Unlock Lore Fragment III: The King's Madness
            player.GetInventory().UnlockFragment("frag_03");

            map.SetGateOpen(true);
            zilDialogueSpeaker = "ZIL THE CORRUPTED CRUSADER";
            zilDialogueText = "“You possess the resolve... Take Truth Seeker and pierce the illusions of Charlz...”";
            zilDialogueColor = Color{ 255, 215, 60, 255 };
            zilDialogueTimer = 6.0f;
            particles.SpawnFloatingText(player.GetPosition(), "VICTORY! CLAIMED 'TRUTH SEEKER'!", Color{ 255, 215, 60, 255 }, 26.0f);
            particles.AddScreenShake(7.0f, 0.3f);
        }
    }

    // Dialogue timer
    if (zilDialogueTimer > 0.0f) {
        zilDialogueTimer -= dt;
    }

    // Check if player walks through northern open gateway into Zyphden
    bool atNorthernGate = map.IsGateOpen() &&
                          (player.GetPosition().y <= map.GetBounds().y + 60.0f &&
                           player.GetPosition().x >= 670.0f && player.GetPosition().x <= 930.0f);
    bool interactGate = map.IsGateOpen() &&
                        (Vector2Distance(player.GetPosition(), Vector2{ 800.0f, map.GetBounds().y + 10.0f }) < 140.0f) &&
                        (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER));

    if (atNorthernGate || interactGate) {
        currentState = GameState::BORDERLANDS_GATE;
        SoundFX::PlayGateOpen();
        particles.SpawnSparks(player.GetPosition(), Color{ 255, 215, 80, 255 }, 40, 220.0f);
        return;
    }

    // Camera follow
    camera.target.x += (player.GetPosition().x - camera.target.x) * 0.08f;
    camera.target.y += (player.GetPosition().y - camera.target.y) * 0.08f;

    // Particle and floating text decay
    particles.Update(dt);
}

void RuinedCastleHandler::Render(
    Camera2D& camera,
    const GameMap& map,
    const std::vector<Enemy>& enemies,
    const std::vector<Projectile>& projectiles,
    const Player& player,
    const ParticleSystem& particles,
    TutorialManager& tutorial,
    int screenWidth,
    int screenHeight,
    bool showControlsModal,
    bool showInventoryModal,
    int inventoryActiveTab,
    int inventorySelectedFragment
) {
    float time = (float)GetTime();

    BeginMode2D(camera);
    map.DrawFloor();

    tutorial.DrawWorldElements(time);

    for (const auto& enemy : enemies) {
        enemy.DrawTelegraph();
    }

    map.DrawWallsAndPillars();

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

    // Tutorial Guidance Banner at top center
    tutorial.DrawHUD(screenWidth, screenHeight);

    bool hasUnread = player.GetInventory().HasUnreadFragments();
    GameUI::DrawCombatHUD(
        screenWidth, screenHeight,
        player.GetHealth(), player.GetMaxHealth(),
        player.GetStamina(), player.GetMaxStamina(),
        player.GetWeapon(),
        player.GetDashCooldown(), player.GetParryCooldown(), player.GetHeavyCooldown(),
        player.IsParrying(), player.HasCounterBuff(),
        1, 1,
        hasUnread
    );

    // Zil Boss Health Bar
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::ZIL_BOSS && enemy.IsAlive()) {
            GameUI::DrawBossHealthBar(screenWidth, enemy.GetName(), enemy.GetHealth(), enemy.GetMaxHealth());
            break;
        }
    }

    // Boss Dialogue Banner
    if (zilDialogueTimer > 0.0f) {
        GameUI::DrawBossDialogueBanner(screenWidth, screenHeight, zilDialogueSpeaker, zilDialogueText, zilDialogueColor);
    }

    if (showControlsModal) {
        GameUI::DrawControlsModal(screenWidth, screenHeight, mousePos, false);
    } else if (showInventoryModal) {
        GameUI::DrawInventoryModal(screenWidth, screenHeight, mousePos, false, const_cast<Player&>(player), inventoryActiveTab, inventorySelectedFragment);
    } else if (showZilDefeatModal) {
        GameUI::DrawZilDefeatModal(screenWidth, screenHeight, mousePos, false, !hasUsedZilRetry);
    } else if (tutorial.HasActivePopup()) {
        tutorial.DrawPopup(screenWidth, screenHeight, mousePos, false);
    } else if (!player.IsAlive() && !zilEncounterActive) {
        GameUI::DrawGameOver(screenWidth, screenHeight);
    }
}
