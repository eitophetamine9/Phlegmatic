#include "RuinedCastleHandler.hpp"
#include "UI.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/CollisionSystem.hpp"
#include "../combat/LootDrop.hpp"
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
    isZilDialogueOpen = false;
    zilDialoguePages.clear();
    zilDialoguePage = 0;
    overworldEnemiesSpawned = false;
    castlePillar = { "frag_09", "Inscribed Castle Monolith", { 680.0f, 960.0f }, 26.0f, false };
    castleChest = { "", 2, "Vanguard Relic Chest", { 1350.0f, 620.0f }, 28.0f, false };
}

void RuinedCastleHandler::Update(
    float dt,
    Player& player,
    GameMap& map,
    ParticleSystem& particles,
    std::vector<Enemy>& enemies,
    std::vector<Projectile>& projectiles,
    std::vector<LootDrop>& lootDrops,
    TutorialManager& tutorial,
    Camera2D& camera,
    GameState& currentState,
    bool& testArenaUnlocked,
    const std::function<void()>& onSaveArenaUnlock,
    const std::function<void()>& onEnterLobby,
    const std::function<void()>& onEnterRuinedCastle
) {
    Vector2 mouseScreen = GetMousePosition();

    // Shortcut: '~' returns to Title Screen
    if (IsKeyPressed(KEY_GRAVE)) {
        currentState = GameState::TITLE_SCREEN;
        SoundFX::PlayButtonClick();
        return;
    }

    // Interactive Tutorial Pop-up Modal (pauses combat and gameplay while visible)
    if (tutorial.HasActivePopup()) {
        tutorial.DrawPopup(GetScreenWidth(), GetScreenHeight(), mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        return;
    }

    // Handle Zil Defeat Modal Interaction
    if (showZilDefeatModal) {
        int choice = GameUI::DrawZilDefeatModal(GetScreenWidth(), GetScreenHeight(), mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), !hasUsedZilRetry);
        if (choice == 1) { // Retry Duel (1 attempt only)
            hasUsedZilRetry = true;
            showZilDefeatModal = false;
            player.Revive({ 800.0f, 520.0f }, 1.0f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();
            enemies.emplace_back(EnemyType::ZIL_BOSS, Vector2{ 800.0f, 290.0f });
            zilEncounterActive = true;
            zilDialogueSpeaker = "ZIL'S FRAGMENT - 1% TRUE POWER";
            zilDialogueText = "Rise then, and face Truth Seeker once more!";
            zilDialogueColor = Color{ 255, 170, 40, 255 };
            zilDialogueTimer = 4.0f;
            particles.SpawnFloatingText({ 800.0f, 290.0f }, "DUEL RESTARTED!", Color{ 255, 200, 60, 255 }, 24.0f);
            return;
        } else if (choice == 2) { // Accept Spare & Claim Zil's Token
            showZilDefeatModal = false;
            zilEncounterActive = false;
            zilSparedPlayer = true;
            overworldEnemiesSpawned = true;
            map.SetGateOpen(true);
            player.Revive({ 800.0f, 480.0f }, 1.0f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();

            // Award Zil's Token (quest item)
            player.GetInventory().AddQuestItem(QuestItem{ "zils_token", "Zil's Token", "A scorched iron crusade badge bestowed by Zil. Proves you survived the trial of the Ruined Castle.", Color{ 255, 180, 60, 255 } });

            zilDialogueSpeaker = "ZIL'S REMNANT - SHADOW OF THE CRUSADER";
            zilDialogueText = "You have heart, though your footing betrayed you. I have seen too much blood spilt for hollow pride. Take my crusade seal to Knight Lenon in Zyphden—tell him Zil's watch endures to the end.";
            zilDialogueColor = Color{ 255, 180, 60, 255 };
            zilDialogueTimer = 7.0f;
            particles.SpawnFloatingText(player.GetPosition(), "ACQUIRED: Zil's Token! Gate to Crossroads Unlocked!", Color{ 255, 180, 60, 255 }, 24.0f);
            particles.AddScreenShake(4.0f, 0.2f);
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
            player.Revive({ 800.0f, 1180.0f }, 1.0f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();
            lootDrops.clear();
            if (tutorial.HasDefeatedTutorialEnemy() && !zilDefeated && !zilSparedPlayer) {
                enemies.emplace_back(EnemyType::SLASHER, Vector2{ 620.0f, 750.0f });
                enemies.emplace_back(EnemyType::SLASHER, Vector2{ 980.0f, 750.0f });
                enemies.emplace_back(EnemyType::BRUTE, Vector2{ 800.0f, 920.0f });
            }
            return;
        }
        if (IsKeyPressed(KEY_L)) {
            if (onEnterLobby) onEnterLobby();
            return;
        }
    }

    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    if (isZilDialogueOpen) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            isZilDialogueOpen = false;
            SoundFX::PlayButtonClick();
        }
    }

    if (player.IsAlive() && !isZilDialogueOpen) {
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

    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera, &lootDrops);

    // Update active loot drops
    for (auto& drop : lootDrops) {
        drop.Update(dt, player.GetPosition(), player, particles);
    }
    lootDrops.erase(std::remove_if(lootDrops.begin(), lootDrops.end(), [](const LootDrop& d) {
        return d.IsCollected();
    }), lootDrops.end());

    // Tutorial progression
    tutorial.Update(dt, player, map, particles, enemies, projectiles);

    // Overworld enemy respawn in courtyard after tutorial Sentinel defeated (Only before encountering or defeating Zil)
    if (tutorial.HasDefeatedTutorialEnemy() && !zilDefeated && !zilSparedPlayer && !zilEncounterActive && !overworldEnemiesSpawned && enemies.empty()) {
        overworldEnemiesSpawned = true;
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ 620.0f, 750.0f });
        enemies.emplace_back(EnemyType::SLASHER, Vector2{ 980.0f, 750.0f });
        enemies.emplace_back(EnemyType::BRUTE, Vector2{ 800.0f, 920.0f });
    }

    // Ensure no enemies ever exist or spawn in Ruined Castle after Zil has been defeated or spared
    if ((zilDefeated || zilSparedPlayer) && !enemies.empty()) {
        enemies.clear();
    }

    // High Altar Steps Barrier (Cannot enter until first tutorial enemy is officially beaten)
    if (!tutorial.HasDefeatedTutorialEnemy() && player.GetPosition().y <= 430.0f) {
        player.SetPosition({ player.GetPosition().x, 435.0f });
        static float barrierWarningTimer = 0.0f;
        barrierWarningTimer -= dt;
        if (barrierWarningTimer <= 0.0f) {
            particles.SpawnFloatingText(player.GetPosition(), "DAIS SEALED — DEFEAT THE SANCTUARY SENTINEL FIRST!", Color{ 255, 90, 90, 255 }, 20.0f);
            SoundFX::PlayHitImpact();
            barrierWarningTimer = 1.5f;
        }
    }

    // Zil Boss Encounter Trigger (Strictly requires first tutorial enemy officially defeated)
    if (tutorial.HasDefeatedTutorialEnemy() && !zilEncounterActive && !zilDefeated && !zilSparedPlayer) {
        if (player.GetPosition().y <= 420.0f) {
            zilEncounterActive = true;
            enemies.clear();
            projectiles.clear();
            enemies.emplace_back(EnemyType::ZIL_BOSS, Vector2{ 800.0f, 290.0f });

            isZilDialogueOpen = true;
            zilDialogueSpeaker = "ZIL'S REMNANT - SHADOW OF THE CRUSADER";
            zilDialogueColor = Color{ 255, 170, 40, 255 };
            zilDialoguePages = {
                "Another soul seeking absolution in these ruined halls... Even shattered to a mere shadow of my former self, Truth Seeker will not yield to an unsteady hand.",
                "Draw your steel, wanderer. Let us see if your spirit burns with genuine conviction or merely hollow pride!"
            };
            zilDialoguePage = 0;
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
            overworldEnemiesSpawned = true;
            enemies.clear();
            projectiles.clear();

            // Award Truth Seeker (Partial)!
            player.GetInventory().UnlockWeapon(WeaponType::TRUTH_SEEKER);
            player.SetWeapon(WeaponType::TRUTH_SEEKER);

            // Unlock Lore Fragment III: Zil's Holy Oath
            player.GetInventory().UnlockFragment("frag_03");

            map.SetGateOpen(true);

            isZilDialogueOpen = true;
            zilDialogueSpeaker = "ZIL'S REMNANT - SHADOW OF THE CRUSADER";
            zilDialogueColor = Color{ 255, 215, 60, 255 };
            zilDialoguePages = {
                "So... the blade recognizes you. Even a fractured remnant can tell true conviction from hollow bravado.",
                "Take Truth Seeker. Carry it north across the high pass into Zyphden, where King Charlz's corruption falls darkest.",
                "Seek King Kreptu and Captain Lenon in the mountain city. Tell them Zil fulfilled his holy oath until the very end."
            };
            zilDialoguePage = 0;
            particles.SpawnFloatingText(player.GetPosition(), "VICTORY! CLAIMED TRUTH SEEKER (PARTIAL)!", Color{ 255, 215, 60, 255 }, 24.0f);
            particles.AddScreenShake(7.0f, 0.3f);
            SoundFX::PlayParrySuccess();
        }
    }

    // Dialogue timer
    if (zilDialogueTimer > 0.0f) {
        zilDialogueTimer -= dt;
    }

    // Inscribed Castle Monolith Interaction
    Vector2 playerPos = player.GetPosition();
    if (!castlePillar.read && castlePillar.CheckInteraction(playerPos) && (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))) {
        castlePillar.read = true;
        player.GetInventory().UnlockFragment("frag_09");
        SoundFX::PlayParrySuccess();
        particles.SpawnFloatingText(castlePillar.position, "DISCOVERED: Fragment IX (Ancient Pillars)!", Color{ 100, 220, 255, 255 }, 22.0f);
        particles.SpawnSparks(castlePillar.position, Color{ 100, 220, 255, 255 }, 20, 120.0f);
    }

    // Vanguard Relic Chest Interaction
    if (!castleChest.opened && castleChest.CheckInteraction(playerPos) && (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))) {
        castleChest.opened = true;
        player.GetInventory().AddHealthPotions(2);
        SoundFX::PlayHeal();
        particles.SpawnFloatingText(castleChest.position, "FOUND: +2 Health Potions!", Color{ 120, 255, 170, 255 }, 22.0f);
        particles.SpawnSparks(castleChest.position, Color{ 120, 255, 170, 255 }, 24, 140.0f);
    }

    // Check if player walks through northern open gateway into Crossroads (Requires Zil defeated OR Zil spared)
    bool gateUnlocked = (zilDefeated || zilSparedPlayer) && map.IsGateOpen();
    bool atNorthernGate = gateUnlocked &&
                          (player.GetPosition().y <= map.GetBounds().y + 60.0f &&
                           player.GetPosition().x >= 670.0f && player.GetPosition().x <= 930.0f);
    bool interactGate = gateUnlocked &&
                        (Vector2Distance(player.GetPosition(), Vector2{ 800.0f, map.GetBounds().y + 10.0f }) < 140.0f) &&
                        (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER));

    if (atNorthernGate || interactGate) {
        currentState = GameState::CROSSROADS;
        SoundFX::PlayGateOpen();
        particles.Clear();
        return;
    }

    // If player attempts to interact at the gate before facing Zil
    if (!gateUnlocked && (Vector2Distance(player.GetPosition(), Vector2{ 800.0f, map.GetBounds().y + 10.0f }) < 140.0f) &&
        (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))) {
        particles.SpawnFloatingText(player.GetPosition(), "GATE SEALED — OVERCOME ZIL'S TRIAL FIRST!", Color{ 255, 80, 80, 255 }, 20.0f);
        SoundFX::PlayHitImpact();
    }

    // Camera follow with screen shake
    Vector2 shake = particles.GetScreenShakeOffset();
    camera.target.x += (player.GetPosition().x + shake.x - camera.target.x) * 0.12f;
    camera.target.y += (player.GetPosition().y + shake.y - camera.target.y) * 0.12f;
}

void RuinedCastleHandler::Render(
    Camera2D& camera,
    const GameMap& map,
    const std::vector<Enemy>& enemies,
    const std::vector<Projectile>& projectiles,
    const std::vector<LootDrop>& lootDrops,
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
    map.DrawFloor(player.GetPosition());

    // Runic Ward across High Altar steps if sentinel not yet defeated
    if (!tutorial.HasDefeatedTutorialEnemy()) {
        float pulse = 0.5f + 0.5f * sinf(time * 4.0f);
        DrawLineEx({ 580.0f, 430.0f }, { 1020.0f, 430.0f }, 3.5f, Color{ 220, 60, 60, (unsigned char)(160 + 80 * pulse) });
        DrawCircleGradient(Vector2{ 800.0f, 430.0f }, 70.0f, Color{ 200, 40, 40, (unsigned char)(50 * pulse) }, Color{ 0, 0, 0, 0 });
        if (Vector2Distance(player.GetPosition(), { 800.0f, 430.0f }) <= 110.0f) {
            const char* wardText = "[SANCTUARY WARD — DEFEAT SENTINEL TO ADVANCE]";
            int wtw = MeasureText(wardText, 10);
            DrawText(wardText, 800 - wtw / 2, 436, 10, Color{ 255, 120, 120, (unsigned char)(190 + 60 * pulse) });
        }
    }

    tutorial.DrawWorldElements(time, player.GetPosition());
    castlePillar.Draw(time, player.GetPosition());
    castleChest.Draw(time, player.GetPosition());

    for (const auto& enemy : enemies) {
        enemy.DrawTelegraph();
    }

    map.DrawWallsAndPillars(player.GetPosition());

    for (const auto& proj : projectiles) {
        proj.Draw();
    }

    for (const auto& enemy : enemies) {
        enemy.Draw();
    }

    for (const auto& drop : lootDrops) {
        drop.Draw(time);
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
    GameUI::DrawCampaignHUD(screenWidth, screenHeight, player, "RUINED CASTLE", hasUnread);

    // Zil Boss Health Bar
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::ZIL_BOSS && enemy.IsAlive()) {
            GameUI::DrawBossHealthBar(screenWidth, enemy.GetName(), enemy.GetHealth(), enemy.GetMaxHealth());
            break;
        }
    }

    // Zil Boss Paginated Dialogue Box (matching NPC dialogue system)
    if (isZilDialogueOpen) {
        bool stillOpen = GameUI::DrawNPCDialogueBox(
            screenWidth, screenHeight, mousePos,
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
            zilDialogueSpeaker, zilDialoguePages, zilDialoguePage, zilDialogueColor
        );
        if (!stillOpen) {
            isZilDialogueOpen = false;
            SoundFX::PlayButtonClick();
        }
    }

    if (showZilDefeatModal) {
        GameUI::DrawZilDefeatModal(screenWidth, screenHeight, mousePos, false, !hasUsedZilRetry);
    } else if (tutorial.HasActivePopup()) {
        tutorial.DrawPopup(screenWidth, screenHeight, mousePos, false);
    } else if (!player.IsAlive() && !zilEncounterActive) {
        GameUI::DrawGameOver(screenWidth, screenHeight);
    }
}
