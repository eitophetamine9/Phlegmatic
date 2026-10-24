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
    } else if (id == "cedric") {
        // Scholar Cedric: Royal azure scholar robes, hood, holding an open chronicle scroll
        DrawCircle((int)position.x, (int)position.y, radius, Color{ 35, 65, 120, 255 });
        DrawCircle((int)position.x, (int)(position.y - 8), radius * 0.6f, Color{ 225, 200, 175, 255 });
        DrawCircle((int)position.x, (int)(position.y - 12), radius * 0.5f, Color{ 24, 45, 90, 255 });
        DrawRectangle((int)(position.x + 12), (int)(position.y - 6), 6, 14, Color{ 240, 230, 205, 255 });
    } else if (id == "varek") {
        // Blacksmith Varek: Soot leather apron, forge hammer, anvil
        DrawCircle((int)position.x, (int)position.y, radius, Color{ 70, 50, 40, 255 });
        DrawRectangle((int)(position.x - 8), (int)(position.y - 6), 16, 16, Color{ 40, 28, 22, 255 });
        DrawLineEx({ position.x + 12.0f, position.y - 10.0f }, { position.x + 12.0f, position.y + 12.0f }, 3.0f, Color{ 160, 165, 180, 255 });
        DrawRectangle((int)(position.x - 22), (int)(position.y + 4), 14, 10, Color{ 65, 70, 80, 255 });
    } else if (id == "maeve") {
        // Elder Maeve: Moss green wool shawl, wooden walking cane
        DrawCircle((int)position.x, (int)position.y, radius, Color{ 60, 85, 65, 255 });
        DrawCircle((int)position.x, (int)(position.y - 8), radius * 0.55f, Color{ 220, 195, 170, 255 });
        DrawLineEx({ position.x + 12.0f, position.y - 12.0f }, { position.x + 12.0f, position.y + 16.0f }, 2.5f, Color{ 110, 80, 50, 255 });
    }

    // Interaction Prompt Icon & Label
    float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
    DrawCircleGradient(position, radius + 14.0f + 4.0f * pulse, Color{ accentColor.r, accentColor.g, accentColor.b, (unsigned char)(50 * pulse) }, Color{ 0, 0, 0, 0 });

    std::string prompt = "[E] " + name;
    int pw = MeasureText(prompt.c_str(), 11);
    DrawText(prompt.c_str(), (int)(position.x - pw * 0.5f), (int)(position.y - radius - 18.0f), 11, accentColor);
}

void ZyphdenLoreItem::Draw(float time) const {
    if (collected) return;

    DrawEllipse((int)position.x, (int)(position.y + 10), radius * 1.1f, radius * 0.5f, Color{ 10, 12, 18, 120 });

    if (fragmentId == "frag_06") {
        // Wind Lectern: Stone plinth with celestial blue open tome & wind runes
        DrawRectangle((int)(position.x - 14), (int)(position.y - 10), 28, 20, Color{ 60, 75, 95, 255 });
        DrawRectangleLines((int)(position.x - 14), (int)(position.y - 10), 28, 20, Color{ 100, 140, 180, 255 });
        DrawRectangle((int)(position.x - 12), (int)(position.y - 8), 11, 16, Color{ 235, 240, 250, 255 });
        DrawRectangle((int)(position.x + 1), (int)(position.y - 8), 11, 16, Color{ 235, 240, 250, 255 });
        float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
        DrawCircleGradient(position, 24.0f * pulse, Color{ 120, 215, 255, (unsigned char)(60 * pulse) }, Color{ 0, 0, 0, 0 });
    } else if (fragmentId == "frag_07") {
        // Vanguard Field Log: Leather notebook with gold clasp
        DrawRectangle((int)(position.x - 12), (int)(position.y - 10), 24, 20, Color{ 90, 55, 35, 255 });
        DrawRectangleLines((int)(position.x - 12), (int)(position.y - 10), 24, 20, Color{ 60, 35, 22, 255 });
        DrawRectangle((int)(position.x - 3), (int)(position.y - 12), 6, 24, Color{ 225, 185, 65, 240 });
        float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
        DrawCircleGradient(position, 22.0f * pulse, Color{ 225, 185, 65, (unsigned char)(55 * pulse) }, Color{ 0, 0, 0, 0 });
    } else {
        // Royal Archives: Gilded archive pedestal with royal decree scroll
        DrawRectangle((int)(position.x - 14), (int)(position.y - 12), 28, 24, Color{ 48, 56, 85, 255 });
        DrawRectangleLines((int)(position.x - 14), (int)(position.y - 12), 28, 24, Color{ 235, 195, 65, 255 });
        DrawCircle((int)position.x, (int)position.y, 5.0f, Color{ 255, 215, 65, 255 });
        float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
        DrawCircleGradient(position, 24.0f * pulse, Color{ 255, 215, 65, (unsigned char)(65 * pulse) }, Color{ 0, 0, 0, 0 });
    }

    std::string prompt = "[E] " + name;
    int lw = MeasureText(prompt.c_str(), 11);
    DrawText(prompt.c_str(), (int)(position.x - lw * 0.5f), (int)(position.y - radius - 16), 11, Color{ 240, 240, 255, 240 });
}

ZyphdenHandler::ZyphdenHandler() {
    inTavern = false;
    eldon = { "eldon", "Tavernkeep Eldon", { 720.0f, 636.0f }, 24.0f, Color{ 255, 190, 80, 255 } };
    noticeboard = { "noticeboard", "Noticeboard", { 780.0f, 720.0f }, 20.0f, Color{ 130, 220, 255, 255 } };
    kingKreptu = { "king", "King Kreptu", { 1000.0f, 220.0f }, 28.0f, Color{ 255, 215, 70, 255 } };
    knightLenonNPC = { "lenon", "Knight Lenon", { 1490.0f, 700.0f }, 26.0f, Color{ 225, 185, 65, 255 } };

    cedric = { "cedric", "Chronicler Cedric", { 720.0f, 1020.0f }, 24.0f, Color{ 100, 200, 255, 255 } };
    varek = { "varek", "Blacksmith Varek", { 320.0f, 700.0f }, 24.0f, Color{ 255, 140, 50, 255 } };
    maeve = { "maeve", "Elder Maeve", { 920.0f, 880.0f }, 22.0f, Color{ 160, 230, 160, 255 } };

    windLectern = { "frag_06", "Ancient Wind Lectern", { 720.0f, 480.0f }, 24.0f, false };
    vanguardLog = { "frag_07", "Vanguard Field Log", { 420.0f, 820.0f }, 24.0f, false };
    royalArchives = { "frag_08", "Royal Archives", { 1140.0f, 220.0f }, 24.0f, false };

    questPhase = ZyphdenQuestPhase::SEEK_TAVERN;
    hasTalkedToKing = false;
    sparActive = false;
    lenonDefeated = false;
    lenonSpawned = false;
    lenonInTavern = false;
    lenonSpawnTimer = 0.0f;
    showObjectives = true;
    entryBannerTimer = 4.0f;
    isDialogueOpen = false;
    dialoguePages.clear();
    currentDialoguePage = 0;
}

void ZyphdenHandler::OpenDialogue(const std::string& npcId, const std::string& speaker, const std::vector<std::string>& pages, Color color) {
    currentDialogueNPCId = npcId;
    dialogueSpeaker = speaker;
    dialoguePages = pages;
    currentDialoguePage = 0;
    dialogueColor = color;
    isDialogueOpen = true;
    SoundFX::PlayButtonClick();
}

void ZyphdenHandler::CloseDialogue() {
    isDialogueOpen = false;
    SoundFX::PlayButtonClick();

    if (currentDialogueNPCId == "eldon_intro") {
        questPhase = ZyphdenQuestPhase::LENON_APPROACHING;
        lenonSpawned = true;
        lenonInTavern = true;
        knightLenonNPC.position = { 800.0f, 915.0f };
        SoundFX::PlayGateOpen();
    } else if (currentDialogueNPCId == "lenon_tavern_intro") {
        questPhase = ZyphdenQuestPhase::SEEK_KING;
        lenonInTavern = false;
        knightLenonNPC.position = { 1490.0f, 700.0f }; // Leaves tavern to Eastern Courtyard
    }
}

void ZyphdenHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    inTavern = false;
    inRoyalKeep = false;
    map.Init(MapEnvironment::ZYPHDEN_REALM);
    map.SetGateOpen(true);

    player.SetPosition({ 920.0f, 1380.0f });
    camera.target = player.GetPosition();

    enemies.clear();
    projectiles.clear();

    eldon = { "eldon", "Tavernkeep Eldon", { 720.0f, 665.0f }, 24.0f, Color{ 255, 190, 80, 255 } };
    noticeboard = { "noticeboard", "Noticeboard", { 780.0f, 720.0f }, 20.0f, Color{ 130, 220, 255, 255 } };
    kingKreptu = { "king", "King Kreptu", { 950.0f, 465.0f }, 28.0f, Color{ 255, 215, 70, 255 } };
    knightLenonNPC = { "lenon", "Knight Lenon", { 1490.0f, 700.0f }, 26.0f, Color{ 225, 185, 65, 255 } };

    cedric = { "cedric", "Chronicler Cedric", { 720.0f, 1020.0f }, 24.0f, Color{ 100, 200, 255, 255 } };
    varek = { "varek", "Blacksmith Varek", { 320.0f, 700.0f }, 24.0f, Color{ 255, 140, 50, 255 } };
    maeve = { "maeve", "Elder Maeve", { 920.0f, 880.0f }, 22.0f, Color{ 160, 230, 160, 255 } };

    windLectern = { "frag_06", "Ancient Wind Lectern", { 720.0f, 480.0f }, 24.0f, player.GetInventory().HasFragment("frag_06") };
    vanguardLog = { "frag_07", "Vanguard Field Log", { 420.0f, 820.0f }, 24.0f, player.GetInventory().HasFragment("frag_07") };
    royalArchives = { "frag_08", "Royal Archives", { 1140.0f, 220.0f }, 24.0f, player.GetInventory().HasFragment("frag_08") };

    lenonDefeated = player.GetInventory().HasArmor(ArmorType::CRUSADER_PLATE);
    if (lenonDefeated) {
        questPhase = ZyphdenQuestPhase::VICTORIOUS;
        lenonSpawned = true;
        lenonInTavern = false;
        knightLenonNPC.position = { 1490.0f, 700.0f };
    } else {
        questPhase = ZyphdenQuestPhase::SEEK_TAVERN;
        lenonSpawned = false;
        lenonInTavern = false;
    }

    sparActive = false;
    entryBannerTimer = 4.0f;
    doorCooldown = 0.0f;
    tavernExitSpawnPos = { 635.0f, 885.0f };
    isDialogueOpen = false;
    dialoguePages.clear();
    currentDialoguePage = 0;
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

    if (doorCooldown > 0.0f) {
        doorCooldown -= dt;
    }

    // Toggle Middle-Left Objectives
    if (IsKeyPressed(KEY_O)) {
        showObjectives = !showObjectives;
        SoundFX::PlayButtonClick();
    }

    // Sync arena unlock if storyline progress was already completed
    if (!testArenaUnlocked && player.GetInventory().HasArmor(ArmorType::CRUSADER_PLATE)) {
        testArenaUnlocked = true;
        if (onSaveArenaUnlock) {
            onSaveArenaUnlock();
        }
    }

    // Dialogue dismissal via ESC or UI handled here as well
    if (isDialogueOpen) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseDialogue();
            return;
        }
    }

    // Player Death & Respawn
    if (!player.IsAlive()) {
        if (IsKeyPressed(KEY_R)) {
            player.Revive(inTavern ? Vector2{ 750.0f, 800.0f } : Vector2{ 1490.0f, 780.0f }, 1.0f);
            camera.target = player.GetPosition();
            enemies.clear();
            projectiles.clear();
            sparActive = false;
            return;
        }
        if (IsKeyPressed(KEY_L)) {
            if (onEnterLobby) onEnterLobby();
            return;
        }
    }

    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    if (player.IsAlive() && !isDialogueOpen) {
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

    if (!isDialogueOpen) {
        player.Update(dt, map, mouseWorld, particles);
    }

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
            questPhase = ZyphdenQuestPhase::VICTORIOUS;
            enemies.clear();

            // Reward Crusader Plate
            player.GetInventory().UnlockArmor(ArmorType::CRUSADER_PLATE);
            player.SetArmor(ArmorType::CRUSADER_PLATE);
            player.Heal(player.GetMaxHealth());

            // Current progress of storyline completed! Unlock the Combat Arena!
            testArenaUnlocked = true;
            if (onSaveArenaUnlock) {
                onSaveArenaUnlock();
            }

            SoundFX::PlayParrySuccess();
            SoundFX::PlayGateOpen();
            particles.SpawnFloatingText(player.GetPosition(), "VICTORY! CLAIMED CRUSADER PLATE (+24 DEF, +30 HP)!", Color{ 255, 215, 80, 255 }, 24.0f);
            particles.SpawnSparks(player.GetPosition(), Color{ 255, 215, 80, 255 }, 45, 220.0f);
            particles.AddScreenShake(6.0f, 0.25f);

            std::vector<std::string> pages = {
                "Superb technique, warrior! Wear this Crusader Plate with pride! You have earned Zyphden's highest honor!",
                "With this armor, the tempest winds beyond our eastern borders will no longer tear you asunder. Go forth, Champion of the Gale Winds!",
                "Martial mastery recognized! The Royal Combat Arena is now unlocked. You can access it from the Title Screen or the Lobby portal!"
            };
            OpenDialogue("lenon_win", "KNIGHT LENON - VANGUARD CAPTAIN", pages, Color{ 225, 185, 65, 255 });
        }
    }

    // Lenon Walking Into Tavern Sequence
    if (questPhase == ZyphdenQuestPhase::LENON_APPROACHING && inTavern && lenonInTavern) {
        Vector2 target = { 720.0f, 765.0f };
        float dx = target.x - knightLenonNPC.position.x;
        float dy = target.y - knightLenonNPC.position.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist > 8.0f) {
            Vector2 walkDir = Vector2Normalize({ dx, dy });
            knightLenonNPC.position.x += walkDir.x * 130.0f * dt;
            knightLenonNPC.position.y += walkDir.y * 130.0f * dt;
        } else if (!isDialogueOpen && currentDialogueNPCId != "lenon_tavern_intro") {
            // Lenon arrived in front of the bar counter! Strike up conversation about Zil ONCE!
            SoundFX::PlayParrySuccess();
            particles.SpawnSparks(knightLenonNPC.position, Color{ 255, 215, 80, 255 }, 24, 120.0f);

            std::vector<std::string> lenonPages;
            if (player.GetInventory().HasWeapon(WeaponType::TRUTH_SEEKER)) {
                lenonPages.push_back("Hold, traveler! That blade at your side... it is Truth Seeker! My sworn brother Zil bore that sacred steel when he rode south to purge the madness.");
                lenonPages.push_back("You bested him in honorable combat? Then his long torment is finally ended. Zyphden owes you an eternal debt for granting him peace.");
                lenonPages.push_back("Before I test your mettle, ancient protocol demands you present yourself to King Kreptu in the Northern Keep outside. Seek His Majesty's blessing first!");
                lenonPages.push_back("Once the King has sanctioned our trial, meet me at the Sparring Ring in the Eastern Courtyard. I shall await you there!");
            } else if (player.GetInventory().HasQuestItem("zil_token")) {
                lenonPages.push_back("Hold, traveler! That silver token you bear... it belongs to my sworn brother Zil! He spared you and gave you his crest? Then honor still endures in the ruins.");
                lenonPages.push_back("Before I grant you a trial in our Eastern Courtyard, ancient protocol demands you present yourself to King Kreptu in the Northern Keep outside. Seek His Majesty's blessing first!");
                lenonPages.push_back("Once the King has sanctioned our trial, meet me at the Sparring Ring in the Eastern Courtyard. I shall await you there!");
            } else {
                lenonPages.push_back("Hold, traveler! Word reached our sentries that a capable warrior survived the southern ruins and the Crossroads.");
                lenonPages.push_back("Before I grant you a trial in our Eastern Courtyard, ancient protocol demands you present yourself to King Kreptu in the Northern Keep outside. Go speak with His Majesty first!");
                lenonPages.push_back("Once the King sanctions our duel, meet me at the Sparring Ring in the Eastern Courtyard!");
            }

            OpenDialogue("lenon_tavern_intro", "KNIGHT LENON - VANGUARD CAPTAIN", lenonPages, Color{ 225, 185, 65, 255 });
        }
    }

    // NPC & Lore & Door Interactivity
    bool interactKey = IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
    Vector2 playerPos = player.GetPosition();

    if (!isDialogueOpen) {
        if (inTavern) {
            // Inside Hearthside Tavern
            // 1. South Doorway to Zyphden exterior (doorway at {800, 915})
            if (doorCooldown <= 0.0f && (playerPos.y >= 910.0f || (Vector2Distance(playerPos, { 800.0f, 915.0f }) <= 45.0f && interactKey))) {
                inTavern = false;
                doorCooldown = 0.5f;
                map.Init(MapEnvironment::ZYPHDEN_REALM);
                player.SetPosition(tavernExitSpawnPos);
                camera.target = player.GetPosition();
                SoundFX::PlayGateOpen();
                return;
            }

            // 2. Tavernkeep Eldon behind the bar (Range increased to 110px across the counter)
            bool eldonInRange = Vector2Distance(playerPos, eldon.position) <= 110.0f;
            if (interactKey && eldonInRange) {
                if (questPhase == ZyphdenQuestPhase::SEEK_TAVERN) {
                    std::vector<std::string> pages = {
                        "Welcome to the Hearthside Tavern, traveler! Pull up a stool and warm yourself by the hearth. Few make it past the Crossroads beasts these days.",
                        "The kingdom has been on edge ever since the mad King Charlz fractured the crown. But wait... hear those heavy armored footsteps at the door?"
                    };
                    OpenDialogue("eldon_intro", "TAVERNKEEP ELDON", pages, Color{ 255, 190, 80, 255 });
                    particles.SpawnFloatingText(eldon.position, "Main Quest Started!", Color{ 255, 215, 80, 255 }, 22.0f);
                    SoundFX::PlayParrySuccess();
                } else {
                    std::vector<std::string> pages = {
                        "Warm fire and cold ale for any who brave the wilds! Speak with Captain Lenon or King Kreptu if you seek to aid our realm."
                    };
                    OpenDialogue("eldon_idle", "TAVERNKEEP ELDON", pages, Color{ 255, 190, 80, 255 });
                }
            }
            // 3. Knight Lenon if inside tavern
            else if (interactKey && lenonInTavern && knightLenonNPC.CheckInteraction(playerPos)) {
                std::vector<std::string> pages = {
                    "Ancient protocol demands you speak with King Kreptu in the Northern Keep outside first! Once His Majesty sanctions our duel, meet me at the Sparring Ring."
                };
                OpenDialogue("lenon_tavern_idle", "KNIGHT LENON", pages, Color{ 225, 185, 65, 255 });
            }
        } else if (inRoyalKeep) {
            // Inside Zyphden Royal Keep (Grand Sovereign Throne Hall)
            // 1. Southern Doorway back to Zyphden exterior (doorway at {950, 995})
            if (doorCooldown <= 0.0f && (playerPos.y >= 990.0f || (Vector2Distance(playerPos, { 950.0f, 990.0f }) <= 55.0f && interactKey))) {
                inRoyalKeep = false;
                doorCooldown = 0.5f;
                map.Init(MapEnvironment::ZYPHDEN_REALM);
                player.SetPosition({ 1000.0f, 355.0f });
                camera.target = player.GetPosition();
                SoundFX::PlayGateOpen();
                return;
            }

            // 2. Audience with King Kreptu on his Golden Throne Dais
            bool kingInRange = Vector2Distance(playerPos, kingKreptu.position) <= 90.0f;
            if (interactKey && kingInRange) {
                if (questPhase == ZyphdenQuestPhase::SEEK_KING) {
                    questPhase = ZyphdenQuestPhase::CHALLENGE_READY;
                    hasTalkedToKing = true;
                    std::vector<std::string> pages = {
                        "Captain Lenon told me of your arrival, wanderer. You brought word of Lord Zil and braved the corrupted highway.",
                        "The Lord Liege's madness consumes Tydeshire, and we need warriors of true resolve. I hereby sanction your martial trial!",
                        "Go to the Eastern Courtyard and spar with Captain Lenon. Defeat him, and the royal Crusader Plate is yours!"
                    };
                    OpenDialogue("king_sanction", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });
                    particles.SpawnFloatingText(kingKreptu.position, "Courtyard Trial Sanctioned!", Color{ 255, 220, 90, 255 }, 22.0f);
                    SoundFX::PlayParrySuccess();
                } else if (questPhase == ZyphdenQuestPhase::VICTORIOUS) {
                    std::vector<std::string> pages = {
                        "You bested Captain Lenon and wear our kingdom's sacred Crusader Plate! Zyphden proudly honors you, Champion of the Gale Winds!"
                    };
                    OpenDialogue("king_win", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });
                } else if (questPhase == ZyphdenQuestPhase::CHALLENGE_READY) {
                    std::vector<std::string> pages = {
                        "Captain Lenon awaits your blade in the Eastern Courtyard. Prove your strength in the trial!"
                    };
                    OpenDialogue("king_ready", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });
                } else {
                    std::vector<std::string> pages = {
                        "Welcome, wanderer. If you seek news of our realm, speak with Eldon inside the Hearthside Tavern first."
                    };
                    OpenDialogue("king_idle", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });
                }
            }
        } else {
            // Outside in Zyphden Realm
            // 1. Enter Hearthside Tavern (Both East and South doors supported, with generous range and walk-in threshold)
            bool nearEastDoor = (Vector2Distance(playerPos, { 600.0f, 885.0f }) <= 85.0f);
            bool nearSouthDoor = (Vector2Distance(playerPos, { 440.0f, 1010.0f }) <= 85.0f);
            bool inEastDoorThreshold = (playerPos.x <= 605.0f && playerPos.x >= 560.0f && playerPos.y >= 845.0f && playerPos.y <= 925.0f);
            bool inSouthDoorThreshold = (playerPos.y <= 1018.0f && playerPos.y >= 975.0f && playerPos.x >= 390.0f && playerPos.x <= 490.0f);

            if (doorCooldown <= 0.0f && ((nearEastDoor || nearSouthDoor) && (interactKey || inEastDoorThreshold || inSouthDoorThreshold))) {
                inTavern = true;
                doorCooldown = 0.5f;
                if (nearEastDoor || inEastDoorThreshold) {
                    tavernExitSpawnPos = { 635.0f, 885.0f };
                } else {
                    tavernExitSpawnPos = { 440.0f, 1040.0f };
                }
                map.Init(MapEnvironment::TAVERN_INTERIOR);
                player.SetPosition({ 800.0f, 840.0f }); // Safely inside tavern room
                camera.target = player.GetPosition();
                SoundFX::PlayGateOpen();
                return;
            }

            // 2. Enter Royal Keep (King Kreptu's Throne Hall at {1000, 320})
            bool nearKeepDoor = (Vector2Distance(playerPos, { 1000.0f, 325.0f }) <= 85.0f);
            bool inKeepDoorThreshold = (playerPos.x >= 940.0f && playerPos.x <= 1060.0f && playerPos.y <= 335.0f && playerPos.y >= 295.0f);

            if (doorCooldown <= 0.0f && ((nearKeepDoor && interactKey) || inKeepDoorThreshold)) {
                inRoyalKeep = true;
                doorCooldown = 0.5f;
                map.Init(MapEnvironment::ROYAL_KEEP);
                player.SetPosition({ 950.0f, 940.0f });
                camera.target = player.GetPosition();
                SoundFX::PlayGateOpen();
                return;
            }

            if (interactKey) {
                // 2. Scholar Cedric (Zyphden Lore)
                if (cedric.CheckInteraction(playerPos)) {
                    std::vector<std::string> pages = {
                        "Zyphden is the Realm of the Gale Tempests. For centuries our royal wind-spires stood as sentinels against the void.",
                        "When King Charlz's crown fell to madness, the ancient seals began to fracture. What you saw in the southern ruins is only the beginning...",
                        "Beyond our eastern borders lies the untamed wilderness. Only those clad in consecrated armor can withstand the tempest winds."
                    };
                    OpenDialogue("cedric", "CHRONICLER CEDRIC", pages, Color{ 100, 200, 255, 255 });
                    particles.SpawnFloatingText(cedric.position, "Ancient Lore Shared", Color{ 100, 200, 255, 255 }, 18.0f);
                }
                // 3. Blacksmith Varek (Armorcraft Lore)
                else if (varek.CheckInteraction(playerPos)) {
                    std::vector<std::string> pages = {
                        "Looking at that scrap armor of yours, wanderer... it won't stop a charging brute in the outer wildlands.",
                        "The Crusader Plate worn by Captain Lenon was forged from fallen stars and wind-tempered steel. Best him in the courtyard if you hope to survive what lies beyond Zyphden!"
                    };
                    OpenDialogue("varek", "BLACKSMITH VAREK", pages, Color{ 255, 140, 50, 255 });
                    particles.SpawnFloatingText(varek.position, "Master Armorcraft Lore", Color{ 255, 150, 60, 255 }, 18.0f);
                }
                // 4. Elder Maeve (Town Lore & Refugees)
                else if (maeve.CheckInteraction(playerPos)) {
                    std::vector<std::string> pages = {
                        "Bless you, child. When the beasts overran the highway, my family barely made it inside these gates.",
                        "King Kreptu gave us bread and sanctuary, but dark clouds gather to the east. Keep your blade sharp and your mind sane."
                    };
                    OpenDialogue("maeve", "ELDER MAEVE", pages, Color{ 160, 230, 160, 255 });
                    particles.SpawnFloatingText(maeve.position, "Town Lore Shared", Color{ 160, 230, 160, 255 }, 18.0f);
                }
                // 5. Town Noticeboard
                else if (noticeboard.CheckInteraction(playerPos)) {
                    player.GetInventory().UnlockFragment("frag_02");
                    player.GetInventory().UnlockFragment("frag_04");
                    std::vector<std::string> pages = {
                        "ROYAL DECREE: The Five Monarchs keep vigil over Tydeshire. Vanguard Captain Lenon oversees all martial trials in the Eastern Courtyard.",
                        "Travelers are warned: The eastern road beyond Zyphden is fraught with tempest anomalies. Proper plate armor required."
                    };
                    OpenDialogue("noticeboard", "ROYAL NOTICEBOARD", pages, Color{ 130, 220, 255, 255 });
                    particles.SpawnFloatingText(noticeboard.position, "Lore Fragment Unlocked!", Color{ 130, 220, 255, 255 }, 18.0f);
                }
                // 6. Lore Item: Wind Lectern (Fragment VI)
                else if (!windLectern.collected && windLectern.CheckInteraction(playerPos)) {
                    windLectern.collected = true;
                    player.GetInventory().UnlockFragment("frag_06");
                    SoundFX::PlayParrySuccess();
                    particles.SpawnFloatingText(windLectern.position, "DISCOVERED: Fragment VI (Gale Winds)!", Color{ 120, 220, 255, 255 }, 22.0f);
                    particles.SpawnSparks(windLectern.position, Color{ 120, 220, 255, 255 }, 24, 140.0f);
                }
                // 7. Lore Item: Vanguard Field Log (Fragment VII)
                else if (!vanguardLog.collected && vanguardLog.CheckInteraction(playerPos)) {
                    vanguardLog.collected = true;
                    player.GetInventory().UnlockFragment("frag_07");
                    SoundFX::PlayParrySuccess();
                    particles.SpawnFloatingText(vanguardLog.position, "DISCOVERED: Fragment VII (Lenon's Vigil)!", Color{ 255, 215, 80, 255 }, 22.0f);
                    particles.SpawnSparks(vanguardLog.position, Color{ 255, 215, 80, 255 }, 24, 140.0f);
                }
                // 8. Lore Item: Royal Archives (Fragment VIII)
                else if (!royalArchives.collected && royalArchives.CheckInteraction(playerPos)) {
                    royalArchives.collected = true;
                    player.GetInventory().UnlockFragment("frag_08");
                    SoundFX::PlayParrySuccess();
                    particles.SpawnFloatingText(royalArchives.position, "DISCOVERED: Fragment VIII (Sovereign's Burden)!", Color{ 255, 215, 80, 255 }, 22.0f);
                    particles.SpawnSparks(royalArchives.position, Color{ 255, 215, 80, 255 }, 24, 140.0f);
                }
                // 9. Knight Lenon in Eastern Courtyard
                else if (lenonSpawned && !sparActive && !lenonInTavern && knightLenonNPC.CheckInteraction(playerPos)) {
                    if (questPhase == ZyphdenQuestPhase::CHALLENGE_READY) {
                        // Begin courtyard spar duel!
                        sparActive = true;
                        questPhase = ZyphdenQuestPhase::SPARRING;
                        enemies.clear();
                        projectiles.clear();
                        enemies.emplace_back(EnemyType::KNIGHT_LENON_BOSS, Vector2{ 1490.0f, 660.0f });

                        SoundFX::PlayCrusaderClash();
                        particles.SpawnSparks(knightLenonNPC.position, Color{ 255, 215, 80, 255 }, 30, 180.0f);
                        particles.AddScreenShake(5.0f, 0.2f);
                        std::vector<std::string> pages = {
                            "His Majesty has sanctioned our duel! Draw your steel, wanderer! Let us see if your resolve can pierce my defense!"
                        };
                        OpenDialogue("lenon_spar_start", "KNIGHT LENON - VANGUARD CAPTAIN", pages, Color{ 225, 185, 65, 255 });
                    } else if (questPhase == ZyphdenQuestPhase::VICTORIOUS) {
                        std::vector<std::string> pages = {
                            "A well-earned victory, friend! That Crusader Plate suits you magnificently."
                        };
                        OpenDialogue("lenon_spar_win", "KNIGHT LENON", pages, Color{ 225, 185, 65, 255 });
                    } else if (questPhase == ZyphdenQuestPhase::SEEK_KING) {
                        std::vector<std::string> pages = {
                            "Speak with King Kreptu in the Northern Keep first! Once His Majesty sanctions our duel, meet me here in the Courtyard."
                        };
                        OpenDialogue("lenon_spar_wait", "KNIGHT LENON", pages, Color{ 225, 185, 65, 255 });
                    }
                }
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

    if (inTavern) {
        // Tavern Interior Entities
        eldon.Draw(time);
        if (lenonInTavern) {
            knightLenonNPC.Draw(time);
        }
        // Southern doorway prompt
        DrawText("[E] EXIT TAVERN", 745, 900, 11, Color{ 255, 220, 120, 240 });
    } else if (inRoyalKeep) {
        // Royal Keep Throne Hall Interior Entities
        kingKreptu.Draw(time);
        DrawText("[E] EXIT ROYAL KEEP", 885, 975, 11, Color{ 255, 230, 120, 240 });
    } else {
        // Zyphden Exterior Lore Collectible Items
        windLectern.Draw(time);
        vanguardLog.Draw(time);
        royalArchives.Draw(time);

        // Zyphden Exterior NPCs
        noticeboard.Draw(time);
        cedric.Draw(time);
        varek.Draw(time);
        maeve.Draw(time);

        if (lenonSpawned && !sparActive && !lenonInTavern) {
            knightLenonNPC.Draw(time);
        }

        // Tavern entrance prompts outside
        if (Vector2Distance(player.GetPosition(), { 600.0f, 885.0f }) <= 120.0f) {
            DrawText("[E] ENTER TAVERN", 612, 880, 11, Color{ 255, 220, 120, 240 });
        }
        if (Vector2Distance(player.GetPosition(), { 440.0f, 1010.0f }) <= 120.0f) {
            DrawText("[E] ENTER TAVERN", 400, 1024, 11, Color{ 255, 220, 120, 240 });
        }

        // Royal Keep entrance prompt outside
        if (Vector2Distance(player.GetPosition(), { 1000.0f, 325.0f }) <= 130.0f) {
            DrawText("[E] ENTER ROYAL KEEP (KING KREPTU)", 895, 335, 11, Color{ 255, 225, 110, 240 });
        }
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
    const char* locationTitle = inRoyalKeep ? "ZYPHDEN ROYAL KEEP" : (inTavern ? "HEARTHSIDE TAVERN" : "ZYPHDEN");
    GameUI::DrawCampaignHUD(
        screenWidth, screenHeight,
        player.GetHealth(), player.GetMaxHealth(),
        player.GetStamina(), player.GetMaxStamina(),
        player.GetWeapon(),
        player.GetDashCooldown(), player.GetParryCooldown(), player.GetHeavyCooldown(),
        player.IsParrying(), player.HasCounterBuff(),
        locationTitle,
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
        float cardW = 285.0f;
        float cardH = 135.0f;
        Rectangle cardRec = { objX, objY, cardW, cardH };
        DrawRectangleRounded(cardRec, 0.15f, 4, Color{ 14, 18, 26, 235 });
        DrawRectangleRoundedLines(cardRec, 0.15f, 4, Color{ 55, 72, 100, 220 });

        DrawText("CHAPTER 1: ZYPHDEN", (int)objX + 14, (int)objY + 12, 11, Color{ 255, 215, 80, 255 });

        // Objective 1: Visit Tavernkeep Eldon
        bool obj1Done = questPhase > ZyphdenQuestPhase::SEEK_TAVERN;
        Color col1 = obj1Done ? Color{ 100, 235, 160, 255 } : Color{ 255, 230, 120, 255 };
        const char* mark1 = obj1Done ? "[x] 1. Spoke with Tavernkeep Eldon" : "[>] 1. Visit Hearthside Tavern & speak with Eldon";
        DrawText(mark1, (int)objX + 14, (int)objY + 34, 10, col1);

        // Objective 2: Royal Audience with King Kreptu
        bool obj2Done = questPhase >= ZyphdenQuestPhase::CHALLENGE_READY;
        Color col2 = obj2Done ? Color{ 100, 235, 160, 255 } : (questPhase == ZyphdenQuestPhase::SEEK_KING ? Color{ 255, 230, 120, 255 } : Color{ 140, 150, 165, 200 });
        const char* mark2 = obj2Done ? "[x] 2. Royal Audience with King Kreptu" : (questPhase == ZyphdenQuestPhase::SEEK_KING ? "[>] 2. Audience with King Kreptu in Northern Keep" : "[ ] 2. Speak with Captain Lenon & King Kreptu");
        DrawText(mark2, (int)objX + 14, (int)objY + 56, 10, col2);

        // Objective 3: Courtyard Spar with Knight Lenon
        bool obj3Done = questPhase == ZyphdenQuestPhase::VICTORIOUS;
        Color col3 = obj3Done ? Color{ 100, 235, 160, 255 } : (questPhase == ZyphdenQuestPhase::CHALLENGE_READY || questPhase == ZyphdenQuestPhase::SPARRING ? Color{ 255, 230, 120, 255 } : Color{ 140, 150, 165, 200 });
        const char* mark3 = obj3Done ? "[x] 3. Bested Knight Lenon in Courtyard" : (questPhase == ZyphdenQuestPhase::SPARRING ? "[!] 3. Sparring with Knight Lenon!" : (questPhase == ZyphdenQuestPhase::CHALLENGE_READY ? "[>] 3. Challenge Knight Lenon in Courtyard" : "[ ] 3. Spar with Knight Lenon in Courtyard"));
        DrawText(mark3, (int)objX + 14, (int)objY + 78, 10, col3);

        // Objective 4: Claim Crusader Plate
        Color col4 = obj3Done ? Color{ 100, 235, 160, 255 } : Color{ 140, 150, 165, 200 };
        const char* mark4 = obj3Done ? "[x] 4. Claimed Crusader Plate (+24 Def)" : "[ ] 4. Claim Crusader Plate (+24 Def)";
        DrawText(mark4, (int)objX + 14, (int)objY + 100, 10, col4);
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

    // Interactive Paginated NPC Dialogue Box
    if (isDialogueOpen) {
        bool stillOpen = GameUI::DrawNPCDialogueBox(
            screenWidth, screenHeight,
            mousePos, IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
            dialogueSpeaker, dialoguePages, currentDialoguePage,
            dialogueColor
        );
        if (!stillOpen) {
            CloseDialogue();
        }
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
