#include "Game.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "raymath.h"
#include <cmath>
#include <string>
#include <algorithm>

Game::Game() : currentState(GameState::TITLE_SCREEN), showControlsModal(false), showInventoryModal(false), inventoryActiveTab(0), inventorySelectedFragment(0), currentWave(1), waveTimer(0.0f) {
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
    showControlsModal = false;
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

void Game::EnterRuinedCastle() {
    map.Init(MapEnvironment::RUINED_CASTLE);
    player.Init({ 800.0f, 1180.0f });
    camera.target = player.GetPosition();
    currentState = GameState::RUINED_CASTLE;
    showControlsModal = false;
    showInventoryModal = false;
    zilEncounterActive = false;
    zilDefeated = false;
    zilSparedPlayer = false;
    hasUsedZilRetry = false;
    showZilDefeatModal = false;
    zilDialogueTimer = 0.0f;
    zilDialogueSpeaker = "";
    zilDialogueText = "";
    zilDialogueColor = WHITE;
    enemies.clear();
    projectiles.clear();
    particles.Clear();
    tutorial.Init();
}

void Game::EnterLobby() {
    map.Init(MapEnvironment::LOBBY);
    player.Init({ 800.0f, 740.0f });
    camera.target = player.GetPosition();
    currentState = GameState::LOBBY;
    showControlsModal = false;
    projectiles.clear();
    particles.Clear();
    particles.SpawnFloatingText(player.GetPosition(), "LOBBY SANCTUARY", Color{ 255, 220, 100, 255 }, 24.0f);
}

void Game::EnterArena() {
    map.Init(MapEnvironment::ARENA);
    player.Init({ 1200.0f, 900.0f });
    camera.target = player.GetPosition();
    currentWave = 1;
    waveTimer = 0.0f;
    projectiles.clear();
    particles.Clear();
    SpawnWave(currentWave);
    currentState = GameState::ARENA;
    showControlsModal = false;
    SoundFX::PlayEquipWeapon();
    particles.SpawnSparks(player.GetPosition(), Color{ 100, 220, 255, 255 }, 12, 180.0f);
}

void Game::SpawnWave(int waveNumber) {
    enemies.clear();
    projectiles.clear();
    Vector2 center = { 1200.0f, 900.0f };

    if (waveNumber == 1) {
        // Wave 1: 2 Slashers & 2 Archers
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x - 280.0f, center.y - 120.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ center.x + 280.0f, center.y - 120.0f });
        enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x - 360.0f, center.y + 160.0f });
        enemies.emplace_back(EnemyType::ARCHER, Vector2{ center.x + 360.0f, center.y + 160.0f });
        particles.SpawnFloatingText(center, "WAVE 1: MELEE & ARCHERS", Color{ 255, 220, 80, 255 }, 24.0f);
    } else if (waveNumber == 2) {
        // Wave 2: 1 Brute, 2 Chargers, 2 Pyromancers
        enemies.emplace_back(EnemyType::BRUTE, Vector2{ center.x, center.y - 280.0f });
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x - 380.0f, center.y });
        enemies.emplace_back(EnemyType::CHARGER, Vector2{ center.x + 380.0f, center.y });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 240.0f, center.y + 240.0f });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 240.0f, center.y + 240.0f });
        particles.SpawnFloatingText(center, "WAVE 2: CHARGERS & PYROMANCERS", Color{ 100, 240, 255, 255 }, 24.0f);
    } else if (waveNumber >= 3) {
        // Wave 3: The Grand Inquisitor Boss & 2 Pyromancer acolytes
        enemies.emplace_back(EnemyType::INQUISITOR_BOSS, Vector2{ center.x, center.y - 240.0f });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x - 320.0f, center.y + 180.0f });
        enemies.emplace_back(EnemyType::PYROMANCER, Vector2{ center.x + 320.0f, center.y + 180.0f });
        particles.SpawnFloatingText(center, "WAVE 3: THE GRAND INQUISITOR", Color{ 255, 60, 60, 255 }, 28.0f);
    }
}

void Game::CheckCombatCollisions() {
    // 1. Player melee attack vs enemies
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

    // 2. Enemy melee strikes vs player (ONLY pure melee strikes, NOT ranged attacks!)
    for (auto& enemy : enemies) {
        if (!enemy.IsAlive() || !enemy.IsInStrike()) continue;
        if (!enemy.HasMeleeHitbox()) continue; // Skip ranged enemies; their projectiles deal damage!

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

            float baseDamage = 18.0f;
            if (enemy.GetType() == EnemyType::BRUTE) baseDamage = 38.0f;
            else if (enemy.GetType() == EnemyType::CHARGER) baseDamage = 22.0f;
            else if (enemy.GetType() == EnemyType::INQUISITOR_BOSS) baseDamage = 45.0f;
            else if (enemy.GetType() == EnemyType::ZIL_BOSS) baseDamage = 32.0f;

            bool tookDamage = player.TakeDamage(baseDamage, knockbackDir, particles);
            if (!tookDamage && player.IsParrying()) {
                enemy.OnParried(particles);
            }
        }
    }

    // 3. Projectile Collisions
    for (auto& proj : projectiles) {
        if (!proj.IsActive()) continue;

        if (!proj.IsFromPlayer()) {
            // Enemy projectile vs Player
            float dx = playerPos.x - proj.GetPosition().x;
            float dy = playerPos.y - proj.GetPosition().y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist <= playerRadius + proj.GetRadius()) {
                if (player.IsDashing()) {
                    // Q Dash invulnerability frames: Evaded!
                    particles.SpawnFloatingText(playerPos, "EVADED", Color{ 120, 240, 180, 255 }, 20.0f);
                    proj.Destroy();
                } else if (player.IsParrying()) {
                    // F Parry reflection!
                    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
                    Vector2 aimDir = Vector2Normalize({ mouseWorld.x - playerPos.x, mouseWorld.y - playerPos.y });
                    if (Vector2Length(aimDir) < 0.1f) {
                        aimDir = Vector2Normalize({ -proj.GetVelocity().x, -proj.GetVelocity().y });
                    }
                    Vector2 reflectedVel = { aimDir.x * 680.0f, aimDir.y * 680.0f };
                    proj.Reflect(reflectedVel);
                    SoundFX::PlayProjectileReflect();
                    particles.SpawnSparks(proj.GetPosition(), Color{ 80, 240, 255, 255 }, 18, 160.0f);
                    particles.SpawnFloatingText(playerPos, "PROJECTILE REFLECTED!", Color{ 80, 240, 255, 255 }, 22.0f);
                    particles.AddScreenShake(3.5f, 0.12f);
                } else {
                    // Hit player
                    Vector2 knockDir = Vector2Normalize(proj.GetVelocity());
                    player.TakeDamage(proj.GetDamage(), knockDir, particles);
                    proj.Destroy();
                }
            }
        } else {
            // Reflected Player projectile vs Enemies
            for (auto& enemy : enemies) {
                if (!enemy.IsAlive()) continue;

                float dx = enemy.GetPosition().x - proj.GetPosition().x;
                float dy = enemy.GetPosition().y - proj.GetPosition().y;
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist <= enemy.GetRadius() + proj.GetRadius()) {
                    Vector2 knockDir = Vector2Normalize(proj.GetVelocity());
                    enemy.TakeDamage(proj.GetDamage(), knockDir, particles);
                    particles.SpawnSparks(proj.GetPosition(), Color{ 100, 230, 255, 255 }, 14, 130.0f);
                    particles.SpawnFloatingText(enemy.GetPosition(), "COUNTER HIT!", Color{ 255, 220, 40, 255 }, 20.0f);
                    proj.Destroy();
                    break;
                }
            }
        }
    }
}

void Game::Update(float dt) {
    Vector2 mouseScreen = GetMousePosition();

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
        int action = GameUI::DrawTitleScreen(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        if (action == 1) {
            cutscene.Reset();
            currentState = GameState::PROLOGUE_CUTSCENE;
        } else if (action == 2) {
            showControlsModal = true;
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
                EnterRuinedCastle();
                return;
            }
            if (IsKeyPressed(KEY_L)) {
                EnterLobby();
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
                    player.TriggerHeavyAttack(particles);
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
            EnterLobby();
            return;
        }
        if (IsKeyPressed(KEY_R)) {
            EnterRuinedCastle();
            return;
        }

        player.Update(dt, map, mouseWorld, particles);

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

        CheckCombatCollisions();

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
        if (map.IsGateOpen() && player.GetPosition().y <= map.GetBounds().y - 20.0f) {
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
        return;
    }

    // 1.9 BORDERLANDS GATEWAY EPILOGUE STATE
    if (currentState == GameState::BORDERLANDS_GATE) {
        int choice = GameUI::DrawBorderlandsGateway(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), player);
        if (choice == 1) { // Re-enter Ruined Castle to explore
            currentState = GameState::RUINED_CASTLE;
            player.Revive({ 800.0f, 320.0f }, 1.0f);
            camera.target = player.GetPosition();
            return;
        } else if (choice == 2) { // Return to Lobby
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
                if (player.GetWeapon() != ped.weapon) {
                    player.SetWeapon(ped.weapon);
                    WeaponStats st = GetWeaponStats(ped.weapon);
                    particles.SpawnFloatingText(player.GetPosition(), "EQUIPPED: " + st.name, Color{ 255, 220, 80, 255 }, 22.0f);
                    particles.SpawnSparks(ped.position, Color{ 100, 220, 255, 255 }, 16, 120.0f);
                }
            }
        }

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

    int w = Config::WINDOW_WIDTH;
    int h = Config::WINDOW_HEIGHT;
    float time = (float)GetTime();

    if (currentState == GameState::TITLE_SCREEN) {
        Vector2 mouseScreen = GetMousePosition();
        GameUI::DrawTitleScreen(w, h, mouseScreen, false);

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
        Vector2 mousePos = GetMousePosition();
        GameUI::DrawBorderlandsGateway(w, h, mousePos, false, player);
        EndDrawing();
        return;
    }

    if (currentState == GameState::RUINED_CASTLE) {
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
        tutorial.DrawHUD(w, h);

        bool hasUnread = player.GetInventory().HasUnreadFragments();
        GameUI::DrawCombatHUD(w, h, player.GetHealth(), player.GetMaxHealth(), player.GetStamina(), player.GetMaxStamina(), player.GetWeapon(), player.GetDashCooldown(), player.GetParryCooldown(), player.GetHeavyCooldown(), player.IsParrying(), player.HasCounterBuff(), 1, 1, hasUnread);

        // Zil Boss Health Bar
        for (const auto& enemy : enemies) {
            if (enemy.GetType() == EnemyType::ZIL_BOSS && enemy.IsAlive()) {
                GameUI::DrawBossHealthBar(w, enemy.GetName(), enemy.GetHealth(), enemy.GetMaxHealth());
                break;
            }
        }

        // Boss Dialogue Banner
        if (zilDialogueTimer > 0.0f) {
            GameUI::DrawBossDialogueBanner(w, h, zilDialogueSpeaker, zilDialogueText, zilDialogueColor);
        }

        if (showControlsModal) {
            GameUI::DrawControlsModal(w, h, mousePos, false);
        } else if (showInventoryModal) {
            GameUI::DrawInventoryModal(w, h, mousePos, false, player, inventoryActiveTab, inventorySelectedFragment);
        } else if (showZilDefeatModal) {
            GameUI::DrawZilDefeatModal(w, h, mousePos, false, !hasUsedZilRetry);
        } else if (tutorial.HasActivePopup()) {
            tutorial.DrawPopup(w, h, mousePos, false);
        } else if (!player.IsAlive() && !zilEncounterActive) {
            GameUI::DrawGameOver(w, h);
        }

        EndDrawing();
        return;
    }

    if (currentState == GameState::LOBBY) {
        BeginMode2D(camera);
        map.DrawFloor();

        portal.Draw(time);

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
    GameUI::DrawCombatHUD(w, h, player.GetHealth(), player.GetMaxHealth(), player.GetStamina(), player.GetMaxStamina(), player.GetWeapon(), player.GetDashCooldown(), player.GetParryCooldown(), player.GetHeavyCooldown(), player.IsParrying(), player.HasCounterBuff(), currentWave, 3, hasUnread);

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
