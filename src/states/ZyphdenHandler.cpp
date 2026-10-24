#include "ZyphdenHandler.hpp"
#include "UI.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/CollisionSystem.hpp"
#include "../rendering/AssetManager.hpp"
#include "raymath.h"
#include <algorithm>
#include <cmath>

void ZyphdenNPC::Draw(float time) const {
    DrawEllipse((int)position.x, (int)(position.y + 10), radius * 1.1f, radius * 0.5f, Color{ 10, 12, 18, 140 });

    if (id == "eldon") {
        // Tavernkeep Eldon: Rustic brown vest, cream shirt, leather belt
        DrawCircle((int)position.x, (int)position.y, radius, Color{ 75, 48, 32, 255 });
        DrawCircle((int)position.x, (int)(position.y - 10), radius * 0.65f, Color{ 220, 185, 150, 255 });
        // Friendly cap
        DrawCircle((int)position.x, (int)(position.y - 14), radius * 0.55f, Color{ 55, 36, 24, 255 });
        // Mug in hand
        DrawRectangle((int)(position.x + 12), (int)(position.y - 4), 8, 12, Color{ 180, 160, 120, 255 });
    } else if (id == "noticeboard") {
        // Royal Decree Noticeboard
        DrawRectangle((int)(position.x - 18), (int)(position.y - 24), 36, 44, Color{ 68, 52, 38, 255 });
        DrawRectangleLines((int)(position.x - 18), (int)(position.y - 24), 36, 44, Color{ 110, 85, 55, 255 });
        // Parchment paper
        DrawRectangle((int)(position.x - 14), (int)(position.y - 20), 28, 36, Color{ 235, 225, 195, 255 });
        DrawRectangleLines((int)(position.x - 14), (int)(position.y - 20), 28, 36, Color{ 175, 155, 120, 255 });
        // Wax seal
        DrawCircle((int)position.x, (int)(position.y - 12), 4.0f, Color{ 190, 45, 45, 255 });
        // Post support
        DrawRectangle((int)(position.x - 3), (int)(position.y + 18), 6, 14, Color{ 50, 38, 28, 255 });
    } else if (id == "king") {
        // King Kreptu: Seated on royal throne dais with golden crown & crimson cape
        DrawRectangle((int)(position.x - 22), (int)(position.y - 18), 44, 38, Color{ 110, 24, 34, 255 });
        DrawRectangleLines((int)(position.x - 22), (int)(position.y - 18), 44, 38, Color{ 235, 195, 65, 255 });
        DrawCircle((int)position.x, (int)position.y, radius * 0.85f, Color{ 48, 56, 85, 255 });
        DrawCircle((int)position.x, (int)(position.y - 8), radius * 0.6f, Color{ 225, 195, 165, 255 });
        // Royal Crown
        DrawRectangle((int)(position.x - 9), (int)(position.y - 18), 18, 6, Color{ 255, 215, 65, 255 });
        DrawCircle((int)(position.x - 6), (int)(position.y - 20), 2.5f, Color{ 255, 230, 95, 255 });
        DrawCircle((int)position.x, (int)(position.y - 22), 3.0f, Color{ 255, 230, 95, 255 });
        DrawCircle((int)(position.x + 6), (int)(position.y - 20), 2.5f, Color{ 255, 230, 95, 255 });
    } else if (id == "lenon") {
        // Knight Lenon: Golden Crusader armor, royal navy cape, gleaming greatsword
        DrawCircle((int)position.x, (int)position.y, radius, Color{ 32, 50, 85, 255 });
        // Golden pauldrons
        DrawCircle((int)(position.x - 14), (int)position.y, 7.0f, Color{ 225, 185, 65, 255 });
        DrawCircle((int)(position.x + 14), (int)position.y, 7.0f, Color{ 225, 185, 65, 255 });
        // Polished steel helm with gold visor
        DrawCircle((int)position.x, (int)(position.y - 6), 11.0f, Color{ 150, 170, 195, 255 });
        DrawRectangle((int)(position.x - 6), (int)(position.y - 8), 12, 4, Color{ 255, 215, 65, 255 });
        // Greatsword sheathed at side
        DrawLineEx({ position.x + 18.0f, position.y - 20.0f }, { position.x + 18.0f, position.y + 24.0f }, 3.0f, Color{ 210, 225, 245, 255 });
        DrawRectangle((int)(position.x + 14), (int)(position.y - 12), 9, 3, Color{ 225, 185, 65, 255 });
    }

    // Interaction Prompt Icon & Label
    float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
    DrawCircleGradient(position, radius + 14.0f + 4.0f * pulse, Color{ accentColor.r, accentColor.g, accentColor.b, (unsigned char)(50 * pulse) }, Color{ 0, 0, 0, 0 });

    std::string prompt = "[E] " + name;
    int pw = MeasureText(prompt.c_str(), 11);
    DrawText(prompt.c_str(), (int)(position.x - pw * 0.5f), (int)(position.y - radius - 18.0f), 11, accentColor);
}

ZyphdenHandler::ZyphdenHandler() {
    eldon = { "eldon", "Tavernkeep Eldon", { 480.0f, 890.0f }, 24.0f, Color{ 255, 190, 80, 255 } };
    noticeboard = { "noticeboard", "Noticeboard", { 780.0f, 720.0f }, 20.0f, Color{ 130, 220, 255, 255 } };
    kingKreptu = { "king", "King Kreptu", { 1000.0f, 220.0f }, 28.0f, Color{ 255, 215, 70, 255 } };
    knightLenonNPC = { "lenon", "Knight Lenon", { 1490.0f, 700.0f }, 26.0f, Color{ 225, 185, 65, 255 } };

    hasTalkedToKing = false;
    sparActive = false;
    lenonDefeated = false;
    showObjectives = true;
    entryBannerTimer = 4.0f;
    dialogueTimer = 0.0f;
}

void ZyphdenHandler::SetDialogue(const std::string& speaker, const std::string& text, Color color, float duration) {
    dialogueSpeaker = speaker;
    dialogueText = text;
    dialogueColor = color;
    dialogueTimer = duration;
    SoundFX::PlayButtonClick();
}

void ZyphdenHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    map.Init(MapEnvironment::ZYPHDEN_REALM);
    map.SetGateOpen(true);

    player.SetPosition({ 920.0f, 1380.0f });
    camera.target = player.GetPosition();

    enemies.clear();
    projectiles.clear();

    eldon = { "eldon", "Tavernkeep Eldon", { 480.0f, 890.0f }, 24.0f, Color{ 255, 190, 80, 255 } };
    noticeboard = { "noticeboard", "Noticeboard", { 780.0f, 720.0f }, 20.0f, Color{ 130, 220, 255, 255 } };
    kingKreptu = { "king", "King Kreptu", { 1000.0f, 220.0f }, 28.0f, Color{ 255, 215, 70, 255 } };
    knightLenonNPC = { "lenon", "Knight Lenon", { 1490.0f, 700.0f }, 26.0f, Color{ 225, 185, 65, 255 } };

    sparActive = false;
    entryBannerTimer = 4.0f;
    dialogueTimer = 0.0f;
}

void ZyphdenHandler::Update(
    float dt,
    Player& player,
    GameMap& map,
    ParticleSystem& particles,
    std::vector<Enemy>& enemies,
    std::vector<Projectile>& projectiles,
    Camera2D& camera,
    GameState& currentState,
    bool& testArenaUnlocked,
    const std::function<void()>& onSaveArenaUnlock,
    const std::function<void()>& onEnterLobby
) {
    Vector2 mouseScreen = GetMousePosition();

    if (entryBannerTimer > 0.0f) {
        entryBannerTimer -= dt;
    }

    if (dialogueTimer > 0.0f) {
        dialogueTimer -= dt;
    }

    // Toggle Middle-Left Objectives
    if (IsKeyPressed(KEY_O)) {
        showObjectives = !showObjectives;
        SoundFX::PlayButtonClick();
    }

    // Player Death & Respawn
    if (!player.IsAlive()) {
        if (IsKeyPressed(KEY_R)) {
            player.Revive({ 1490.0f, 780.0f }, 1.0f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();
            sparActive = false;
            SetDialogue("KNIGHT LENON", "Take your breath, warrior. Rest and challenge me again when ready!", Color{ 225, 185, 65, 255 }, 4.0f);
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

    // Check Courtyard Spar Completion
    if (sparActive) {
        bool lenonStillAlive = false;
        for (const auto& enemy : enemies) {
            if (enemy.GetType() == EnemyType::KNIGHT_LENON_BOSS && enemy.IsAlive()) {
                lenonStillAlive = true;
                break;
            }
        }

        if (!lenonStillAlive && !lenonDefeated) {
            // Player successfully defeated Knight Lenon in spar!
            sparActive = false;
            lenonDefeated = true;
            enemies.clear();

            // Reward Crusader Plate
            player.GetInventory().UnlockArmor(ArmorType::CRUSADER_PLATE);
            player.SetArmor(ArmorType::CRUSADER_PLATE);
            player.Heal(player.GetMaxHealth());

            // Unlock Arena in Title Screen
            testArenaUnlocked = true;
            if (onSaveArenaUnlock) onSaveArenaUnlock();

            SoundFX::PlayParrySuccess();
            SoundFX::PlayGateOpen();
            particles.SpawnFloatingText(player.GetPosition(), "VICTORY! CLAIMED CRUSADER PLATE (+24 DEF, +30 HP)!", Color{ 255, 215, 80, 255 }, 24.0f);
            particles.SpawnSparks(player.GetPosition(), Color{ 255, 215, 80, 255 }, 45, 220.0f);
            particles.AddScreenShake(6.0f, 0.25f);

            SetDialogue("KNIGHT LENON - VANGUARD CAPTAIN", "Superb technique, warrior! Wear this Crusader Plate with pride! You have earned Zyphden's highest honor!", Color{ 225, 185, 65, 255 }, 7.0f);
        }
    }

    // NPC Interactivity (Press [E], [SPACE], or [ENTER])
    bool interactKey = IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
    Vector2 playerPos = player.GetPosition();

    if (interactKey) {
        // 1. Tavernkeep Eldon
        if (eldon.CheckInteraction(playerPos)) {
            SetDialogue("TAVERNKEEP ELDON", "Welcome to Zyphden! Speak with King Kreptu in the Northern Keep, or test your blade against Knight Lenon!", Color{ 255, 190, 80, 255 }, 5.5f);
            particles.SpawnFloatingText(eldon.position, "Tavern Rumors Shared", Color{ 255, 200, 100, 255 }, 18.0f);
        }
        // 2. Town Noticeboard
        else if (noticeboard.CheckInteraction(playerPos)) {
            player.GetInventory().UnlockFragment("frag_02");
            player.GetInventory().UnlockFragment("frag_04");
            SetDialogue("ROYAL NOTICEBOARD", "The Five Monarchs keep vigil over Tydeshire. Vanguard Captain Lenon oversees all martial trials in the Eastern Courtyard.", Color{ 130, 220, 255, 255 }, 6.0f);
            particles.SpawnFloatingText(noticeboard.position, "Lore Fragment Unlocked!", Color{ 130, 220, 255, 255 }, 18.0f);
        }
        // 3. King Kreptu
        else if (kingKreptu.CheckInteraction(playerPos)) {
            hasTalkedToKing = true;
            if (lenonDefeated) {
                SetDialogue("KING KREPTU - MONARCH OF ZYPHDEN", "You bested Captain Lenon and wear our kingdom's Crusader Plate! Zyphden proudly salutes your courage, Champion!", Color{ 255, 215, 70, 255 }, 6.0f);
            } else if (player.GetInventory().HasWeapon(WeaponType::TRUTH_SEEKER)) {
                SetDialogue("KING KREPTU - MONARCH OF ZYPHDEN", "You carry the Truth Seeker remnant from Zil! Go to the Eastern Courtyard and spar with Captain Lenon to claim our Crusader Plate!", Color{ 255, 215, 70, 255 }, 6.5f);
            } else {
                SetDialogue("KING KREPTU - MONARCH OF ZYPHDEN", "Welcome, wanderer. Prove your strength against Vanguard Captain Knight Lenon in our Eastern Courtyard to earn our Crusader Plate!", Color{ 255, 215, 70, 255 }, 6.0f);
            }
            particles.SpawnFloatingText(kingKreptu.position, "Royal Audience Granted", Color{ 255, 220, 100, 255 }, 18.0f);
        }
        // 4. Knight Lenon
        else if (!sparActive && knightLenonNPC.CheckInteraction(playerPos)) {
            if (lenonDefeated) {
                SetDialogue("KNIGHT LENON", "A well-earned victory, friend! That Crusader Plate suits you magnificently.", Color{ 225, 185, 65, 255 }, 5.0f);
            } else {
                // Begin courtyard spar duel!
                sparActive = true;
                enemies.clear();
                projectiles.clear();
                enemies.emplace_back(EnemyType::KNIGHT_LENON_BOSS, Vector2{ 1490.0f, 660.0f });

                SoundFX::PlayCrusaderClash();
                particles.SpawnSparks(knightLenonNPC.position, Color{ 255, 215, 80, 255 }, 30, 180.0f);
                particles.AddScreenShake(5.0f, 0.2f);
                SetDialogue("KNIGHT LENON - VANGUARD CAPTAIN", "Draw your steel, wanderer! Let us see if your technique matches your reputation!", Color{ 225, 185, 65, 255 }, 5.5f);
            }
        }
    }

    particles.Update(dt);

    // Camera smoothing
    camera.target.x += (player.GetPosition().x - camera.target.x) * 0.08f;
    camera.target.y += (player.GetPosition().y - camera.target.y) * 0.08f;
}

void ZyphdenHandler::Render(
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
    float time = (float)GetTime();

    BeginMode2D(camera);
    map.DrawFloor();

    // Draw Peaceful NPCs when not actively sparring as boss
    eldon.Draw(time);
    noticeboard.Draw(time);
    kingKreptu.Draw(time);

    if (!sparActive) {
        knightLenonNPC.Draw(time);
    }

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
    GameUI::DrawCampaignHUD(
        screenWidth, screenHeight,
        player.GetHealth(), player.GetMaxHealth(),
        player.GetStamina(), player.GetMaxStamina(),
        player.GetWeapon(),
        player.GetDashCooldown(), player.GetParryCooldown(), player.GetHeavyCooldown(),
        player.IsParrying(), player.HasCounterBuff(),
        "ZYPHDEN",
        hasUnread
    );

    // Boss Health Bar during Courtyard Spar
    if (sparActive) {
        for (const auto& enemy : enemies) {
            if (enemy.GetType() == EnemyType::KNIGHT_LENON_BOSS && enemy.IsAlive()) {
                GameUI::DrawBossHealthBar(screenWidth, enemy.GetName(), enemy.GetHealth(), enemy.GetMaxHealth());
                break;
            }
        }
    }

    // Middle-Left Collapsible Objectives Tracker
    float objY = (float)screenHeight * 0.38f;
    float objX = 18.0f;
    Rectangle toggleTabRec = { objX, objY - 26.0f, 130.0f, 22.0f };
    bool hoverTab = CheckCollisionPointRec(mousePos, toggleTabRec);
    if (hoverTab && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        showObjectives = !showObjectives;
        SoundFX::PlayButtonClick();
    }

    DrawRectangleRounded(toggleTabRec, 0.3f, 4, hoverTab ? Color{ 42, 54, 75, 240 } : Color{ 22, 28, 40, 210 });
    DrawRectangleRoundedLines(toggleTabRec, 0.3f, 4, Color{ 85, 105, 140, 220 });
    DrawText(showObjectives ? "[-] OBJECTIVES [O]" : "[+] OBJECTIVES [O]", (int)objX + 8, (int)objY - 22, 11, hoverTab ? Color{ 255, 230, 120, 255 } : Color{ 200, 215, 240, 230 });

    if (showObjectives) {
        float cardW = 270.0f;
        float cardH = 120.0f;
        Rectangle cardRec = { objX, objY, cardW, cardH };
        DrawRectangleRounded(cardRec, 0.15f, 4, Color{ 14, 18, 26, 235 });
        DrawRectangleRoundedLines(cardRec, 0.15f, 4, Color{ 55, 72, 100, 220 });

        DrawText("CHAPTER 1: ZYPHDEN", (int)objX + 14, (int)objY + 12, 11, Color{ 255, 215, 80, 255 });

        // Objective 1: Speak with King Kreptu
        Color col1 = hasTalkedToKing ? Color{ 100, 235, 160, 255 } : Color{ 220, 225, 240, 230 };
        const char* mark1 = hasTalkedToKing ? "[x] 1. King Kreptu in Northern Keep" : "[ ] 1. King Kreptu in Northern Keep";
        DrawText(mark1, (int)objX + 14, (int)objY + 36, 11, col1);

        // Objective 2: Challenge Knight Lenon
        Color col2 = (sparActive || lenonDefeated) ? Color{ 100, 235, 160, 255 } : (hasTalkedToKing ? Color{ 220, 225, 240, 230 } : Color{ 140, 150, 165, 200 });
        const char* mark2 = lenonDefeated ? "[x] 2. Spar with Knight Lenon" : (sparActive ? "[>] 2. Sparring with Knight Lenon" : "[ ] 2. Spar with Knight Lenon");
        DrawText(mark2, (int)objX + 14, (int)objY + 58, 11, col2);

        // Objective 3: Claim Crusader Plate
        Color col3 = lenonDefeated ? Color{ 100, 235, 160, 255 } : Color{ 140, 150, 165, 200 };
        const char* mark3 = lenonDefeated ? "[x] 3. Claim Crusader Plate (+24 Def)" : "[ ] 3. Claim Crusader Plate (+24 Def)";
        DrawText(mark3, (int)objX + 14, (int)objY + 80, 11, col3);
    }

    // Dialogue Banner
    if (dialogueTimer > 0.0f) {
        GameUI::DrawBossDialogueBanner(screenWidth, screenHeight, dialogueSpeaker, dialogueText, dialogueColor);
    }

    // Top Location Entry Banner
    if (entryBannerTimer > 0.0f) {
        float alpha = fminf(1.0f, entryBannerTimer);
        int bw = MeasureText("CHAPTER 1: THE REALM OF ZYPHDEN", 22);
        int bx = (screenWidth - bw) / 2;
        int by = 68;
        DrawRectangle(bx - 24, by - 8, bw + 48, 40, Color{ 12, 16, 24, (unsigned char)(210 * alpha) });
        DrawRectangleLines(bx - 24, by - 8, bw + 48, 40, Color{ 225, 185, 65, (unsigned char)(240 * alpha) });
        DrawText("CHAPTER 1: THE REALM OF ZYPHDEN", bx, by, 22, Color{ 255, 225, 100, (unsigned char)(255 * alpha) });
    }

    if (showControlsModal) {
        GameUI::DrawControlsModal(screenWidth, screenHeight, mousePos, false);
    } else if (showInventoryModal) {
        GameUI::DrawInventoryModal(screenWidth, screenHeight, mousePos, false, const_cast<Player&>(player), inventoryActiveTab, inventorySelectedFragment);
    } else if (!player.IsAlive()) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{ 20, 5, 8, 210 });
        int titleW = MeasureText("FALLEN IN BATTLE", 38);
        DrawText("FALLEN IN BATTLE", (screenWidth - titleW) / 2, screenHeight / 2 - 40, 38, Color{ 255, 50, 60, 255 });
        int subW = MeasureText("Press [R] to Recover at Courtyard Bench  |  [L] Return to Lobby", 16);
        DrawText("Press [R] to Recover at Courtyard Bench  |  [L] Return to Lobby", (screenWidth - subW) / 2, screenHeight / 2 + 18, 16, Color{ 220, 225, 240, 230 });
    }
}
