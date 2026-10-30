#include "ZyphdenHandler.hpp"

#include "UI.hpp"

#include "../core/Config.hpp"

#include "../core/SoundFX.hpp"

#include "../combat/CollisionSystem.hpp"

#include "../combat/LootDrop.hpp"

#include "../rendering/AssetManager.hpp"

#include "../rendering/FontManager.hpp"

#include "raymath.h"

#include <algorithm>

#include <cmath>



void ZyphdenNPC::Draw(float time, Vector2 playerPos) const {

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

    } else if (id == "malakor") {

        // Chancellor Malakor: Royal purple robes, golden torque, holding an imperial decree scroll

        DrawCircle((int)position.x, (int)position.y, radius, Color{ 80, 42, 115, 255 });

        DrawCircle((int)position.x, (int)(position.y - 8), radius * 0.6f, Color{ 220, 195, 170, 255 });

        DrawCircle((int)position.x, (int)(position.y - 12), radius * 0.5f, Color{ 60, 30, 90, 255 });

        DrawCircleLines((int)position.x, (int)position.y, 10.0f, Color{ 245, 205, 75, 255 });

        DrawRectangle((int)(position.x + 12), (int)(position.y - 8), 5, 18, Color{ 245, 230, 200, 255 });

    } else if (id == "donald") {

        // Sentinel Donald: Steel plate, royal blue tabard, long halberd

        DrawCircle((int)position.x, (int)position.y, radius, Color{ 45, 65, 105, 255 });

        DrawCircle((int)(position.x - 14), (int)position.y, 6.0f, Color{ 140, 160, 185, 255 });

        DrawCircle((int)(position.x + 14), (int)position.y, 6.0f, Color{ 140, 160, 185, 255 });

        DrawCircle((int)position.x, (int)(position.y - 6), 11.0f, Color{ 155, 175, 200, 255 });

        DrawLineEx({ position.x + 16.0f, position.y - 24.0f }, { position.x + 16.0f, position.y + 24.0f }, 3.0f, Color{ 110, 85, 60, 255 });

        DrawTriangle({ position.x + 16.0f, position.y - 28.0f }, { position.x + 24.0f, position.y - 18.0f }, { position.x + 16.0f, position.y - 14.0f }, Color{ 210, 225, 245, 255 });

    } else if (id == "garrick") {

        // Sentinel Garrick: Heavy plate, cobalt cloak, tower shield

        DrawCircle((int)position.x, (int)position.y, radius, Color{ 45, 65, 105, 255 });

        DrawCircle((int)position.x, (int)(position.y - 6), 11.0f, Color{ 155, 175, 200, 255 });

        DrawRectangle((int)(position.x - 20), (int)(position.y - 14), 14, 28, Color{ 65, 85, 130, 255 });

        DrawRectangleLines((int)(position.x - 20), (int)(position.y - 14), 14, 28, Color{ 235, 195, 65, 255 });

    } else if (id == "thaddeus") {

        // Archivist Thaddeus: Deep sapphire hooded robes, spectacles, open ancient codex

        DrawCircle((int)position.x, (int)position.y, radius, Color{ 28, 48, 88, 255 });

        DrawCircle((int)position.x, (int)(position.y - 8), radius * 0.58f, Color{ 225, 200, 175, 255 });

        DrawCircle((int)position.x, (int)(position.y - 12), radius * 0.5f, Color{ 18, 34, 68, 255 });

        DrawCircleLines((int)(position.x - 4), (int)(position.y - 8), 3.0f, Color{ 240, 210, 80, 255 });

        DrawCircleLines((int)(position.x + 4), (int)(position.y - 8), 3.0f, Color{ 240, 210, 80, 255 });

    } else if (id == "genevieve") {

        // Lady Genevieve: Elegant crimson silk dress, gold tiara, jeweled rapier

        DrawCircle((int)position.x, (int)position.y, radius, Color{ 135, 28, 45, 255 });

        DrawCircle((int)position.x, (int)(position.y - 8), radius * 0.55f, Color{ 235, 210, 190, 255 });

        DrawRectangle((int)(position.x - 7), (int)(position.y - 16), 14, 3, Color{ 255, 220, 80, 255 });

        DrawCircle((int)position.x, (int)(position.y - 18), 2.5f, Color{ 255, 230, 95, 255 });

        DrawLineEx({ position.x + 12.0f, position.y - 10.0f }, { position.x + 16.0f, position.y + 18.0f }, 2.0f, Color{ 220, 230, 245, 255 });

        DrawCircle((int)(position.x + 12), (int)(position.y - 8), 3.0f, Color{ 255, 215, 75, 255 });

    }



    // Interaction Prompt Icon & Label (Decluttered: only shown when player is within interaction distance)

    bool inRange = (playerPos.x > -9000.0f) ? CheckInteraction(playerPos) : true;

    if (inRange) {

        float pulse = 0.5f + 0.5f * sinf(time * 3.5f);

        DrawCircleGradient(position, radius + 14.0f + 4.0f * pulse, Color{ accentColor.r, accentColor.g, accentColor.b, (unsigned char)(50 * pulse) }, Color{ 0, 0, 0, 0 });



        // Eldon and Varek have dedicated contextual action prompts rendered by caller

        if (id != "eldon" && id != "varek") {

            std::string prompt = "[E] " + name;

            int pw = MeasureText(prompt.c_str(), 11);

            DrawText(prompt.c_str(), (int)(position.x - pw * 0.5f), (int)(position.y - radius - 18.0f), 11, accentColor);

        }

    }

}



void ZyphdenLoreItem::Draw(float time, Vector2 playerPos) const {

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



    bool inRange = (playerPos.x > -9000.0f) ? CheckInteraction(playerPos) : true;

    if (inRange) {

        std::string prompt = "[E] " + name;

        int lw = MeasureText(prompt.c_str(), 11);

        DrawText(prompt.c_str(), (int)(position.x - lw * 0.5f), (int)(position.y - radius - 16), 11, Color{ 240, 240, 255, 240 });

    }

}



ZyphdenHandler::ZyphdenHandler() {

    inTavern = false;

    inForge = false;

    inRoyalKeep = false;

    eldon = { "eldon", "Tavernkeep Eldon", { 720.0f, 636.0f }, 24.0f, Color{ 255, 190, 80, 255 } };

    noticeboard = { "noticeboard", "Noticeboard", { 780.0f, 720.0f }, 20.0f, Color{ 130, 220, 255, 255 } };

    kingKreptu = { "king", "King Kreptu", { 1000.0f, 215.0f }, 28.0f, Color{ 255, 215, 70, 255 } };

    knightLenonNPC = { "lenon", "Knight Lenon", { 1490.0f, 700.0f }, 26.0f, Color{ 225, 185, 65, 255 } };



    // 5 Throne Room NPCs in Expanded Royal Keep

    malakor = { "malakor", "Chancellor Malakor", { 920.0f, 360.0f }, 24.0f, Color{ 190, 130, 240, 255 } };

    donald = { "donald", "Sentinel Donald", { 890.0f, 240.0f }, 26.0f, Color{ 100, 180, 255, 255 } };

    garrick = { "garrick", "Sentinel Garrick", { 1110.0f, 240.0f }, 26.0f, Color{ 100, 180, 255, 255 } };

    thaddeus = { "thaddeus", "Archivist Thaddeus", { 540.0f, 480.0f }, 24.0f, Color{ 85, 175, 255, 255 } };

    genevieve = { "genevieve", "Lady Genevieve", { 1460.0f, 480.0f }, 22.0f, Color{ 245, 140, 160, 255 } };



    // Keep Lore & Supply Interactables

    keepPillar = { "frag_09", "Inscribed Keep Monolith", { 650.0f, 600.0f }, 26.0f, false };

    keepChest = { "frag_10", 2, "Royal Armory Relic Chest", { 1480.0f, 380.0f }, 28.0f, false };

    keepPantryChest = { "", 1, "Keep Provisions Chest", { 650.0f, 380.0f }, 28.0f, false };



    cedric = { "cedric", "Chronicler Cedric", { 720.0f, 1020.0f }, 24.0f, Color{ 100, 200, 255, 255 } };

    varek = { "varek", "Blacksmith Varek", { 750.0f, 530.0f }, 24.0f, Color{ 255, 140, 50, 255 } };

    maeve = { "maeve", "Elder Maeve", { 920.0f, 880.0f }, 22.0f, Color{ 160, 230, 160, 255 } };



    windLectern = { "frag_06", "Ancient Wind Lectern", { 720.0f, 480.0f }, 24.0f, false };

    vanguardLog = { "frag_07", "Vanguard Field Log", { 420.0f, 820.0f }, 24.0f, false };

    royalArchives = { "frag_08", "Royal Archives", { 1390.0f, 540.0f }, 24.0f, false };



    questPhase = ZyphdenQuestPhase::SEEK_TAVERN;

    hasTalkedToKing = false;

    sparActive = false;

    sparPendingStart = false;

    hasSparredOnce = false;

    lenonDefeated = false;

    lenonSpawned = false;

    lenonInTavern = false;

    lenonSpawnTimer = 0.0f;

    showObjectives = true;

    entryBannerTimer = 4.0f;

    isDialogueOpen = false;

    isForgeOpen = false;

    isProvisionsOpen = false;

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

    } else if (currentDialogueNPCId == "lenon_spar_start") {

        sparPendingStart = true;

    }

}



void ZyphdenHandler::Reset() {
    questPhase = ZyphdenQuestPhase::SEEK_TAVERN;
    hasTalkedToKing = false;
    sparActive = false;
    sparPendingStart = false;
    hasSparredOnce = false;
    lenonDefeated = false;
    lenonSpawned = false;
    lenonInTavern = false;
    lenonSpawnTimer = 0.0f;
    inTavern = false;
    inForge = false;
    forgeTab = 0;
    inRoyalKeep = false;
    entryBannerTimer = 4.0f;
    showObjectives = true;
    isDialogueOpen = false;
    currentDialogueNPCId = "";
    dialogueSpeaker = "";
    dialoguePages.clear();
    currentDialoguePage = 0;
    isForgeOpen = false;
    isProvisionsOpen = false;
}

void ZyphdenHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {

    inTavern = false;

    inForge = false;

    inRoyalKeep = false;

    sparPendingStart = false;

    map.Init(MapEnvironment::ZYPHDEN_REALM);

    map.SetGateOpen(true);



    player.SetPosition({ 920.0f, 1380.0f });

    camera.target = player.GetPosition();



    enemies.clear();

    projectiles.clear();



    eldon = { "eldon", "Tavernkeep Eldon", { 720.0f, 665.0f }, 24.0f, Color{ 255, 190, 80, 255 } };

    noticeboard = { "noticeboard", "Noticeboard", { 780.0f, 720.0f }, 20.0f, Color{ 130, 220, 255, 255 } };

    kingKreptu = { "king", "King Kreptu", { 1000.0f, 215.0f }, 28.0f, Color{ 255, 215, 70, 255 } };

    knightLenonNPC = { "lenon", "Knight Lenon", { 1490.0f, 700.0f }, 26.0f, Color{ 225, 185, 65, 255 } };



    cedric = { "cedric", "Chronicler Cedric", { 720.0f, 1020.0f }, 24.0f, Color{ 100, 200, 255, 255 } };

    varek = { "varek", "Blacksmith Varek", { 750.0f, 530.0f }, 24.0f, Color{ 255, 140, 50, 255 } };

    maeve = { "maeve", "Elder Maeve", { 920.0f, 880.0f }, 22.0f, Color{ 160, 230, 160, 255 } };



    windLectern = { "frag_06", "Ancient Wind Lectern", { 720.0f, 480.0f }, 24.0f, player.GetInventory().HasFragment("frag_06") };

    vanguardLog = { "frag_07", "Vanguard Field Log", { 420.0f, 820.0f }, 24.0f, player.GetInventory().HasFragment("frag_07") };

    royalArchives = { "frag_08", "Royal Archives", { 1390.0f, 540.0f }, 24.0f, player.GetInventory().HasFragment("frag_08") };



    // Only set to VICTORIOUS if actually defeated and past that stage
    if (lenonDefeated && questPhase >= ZyphdenQuestPhase::VICTORIOUS) {
        lenonDefeated = true;
        lenonSpawned = true;
        lenonInTavern = false;
        knightLenonNPC.position = { 1490.0f, 700.0f };
    } else if (questPhase >= ZyphdenQuestPhase::SEEK_KING) {

        lenonDefeated = false;

        lenonSpawned = true;

        lenonInTavern = false;

        knightLenonNPC.position = { 1490.0f, 700.0f };

    } else if (questPhase == ZyphdenQuestPhase::LENON_APPROACHING) {

        lenonDefeated = false;

        lenonSpawned = true;

        lenonInTavern = true;

        knightLenonNPC.position = { 720.0f, 765.0f };

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



    // Eastern Quarry Ore Golems moved to dedicated Quarry Depths map

    golemSpawners.clear();

}



void ZyphdenHandler::Update(

    float dt,

    Player& player,

    GameMap& map,

    ParticleSystem& particles,

    std::vector<Enemy>& enemies,

    std::vector<Projectile>& projectiles,

    std::vector<LootDrop>& lootDrops,

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

            player.Revive(inTavern ? Vector2{ 750.0f, 800.0f } : (inForge ? Vector2{ 800.0f, 850.0f } : Vector2{ 1380.0f, 750.0f }), 1.0f);

            camera.target = player.GetPosition();

            enemies.clear();

            projectiles.clear();

            sparActive = false;

            sparPendingStart = false;

            if (questPhase == ZyphdenQuestPhase::SPARRING) {

                questPhase = ZyphdenQuestPhase::CHALLENGE_READY;

            }

            lenonSpawned = true;

            lenonInTavern = false;

            knightLenonNPC.position = { 1490.0f, 700.0f };

            return;

        }

        if (IsKeyPressed(KEY_L)) {

            if (onEnterLobby) onEnterLobby();

            return;

        }

    }



    // Delayed spar boss duel spawn: starts only AFTER conversation text has disappeared!

    if (sparPendingStart && !isDialogueOpen) {

        sparPendingStart = false;

        sparActive = true;

        hasSparredOnce = true;

        questPhase = ZyphdenQuestPhase::SPARRING;

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) {

            return e.GetType() == EnemyType::KNIGHT_LENON_BOSS;

        }), enemies.end());

        projectiles.clear();

        enemies.emplace_back(EnemyType::KNIGHT_LENON_BOSS, Vector2{ 1490.0f, 660.0f });



        SoundFX::PlayCrusaderClash();

        particles.SpawnSparks(knightLenonNPC.position, Color{ 255, 215, 80, 255 }, 30, 180.0f);

        particles.AddScreenShake(5.0f, 0.2f);

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



    if (!inTavern && !inForge && !inRoyalKeep) {

        for (auto& enemy : enemies) {

            enemy.Update(dt, player.GetPosition(), map, particles, projectiles);

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



    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera, &lootDrops);



    // Update active loot drops

    for (auto& drop : lootDrops) {

        drop.Update(dt, player.GetPosition(), player, particles);

    }

    lootDrops.erase(std::remove_if(lootDrops.begin(), lootDrops.end(), [](const LootDrop& d) {

        return d.IsCollected();

    }), lootDrops.end());



    // Clean up dead enemies in Zyphden

    if (!inTavern && !inForge && !inRoyalKeep) {

        for (auto it = enemies.begin(); it != enemies.end(); ) {

            if (!it->IsAlive()) {

                it = enemies.erase(it);

            } else {

                ++it;

            }

        }

    }



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

            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) {

                return e.GetType() == EnemyType::KNIGHT_LENON_BOSS;

            }), enemies.end());



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

                "Magnificent bladecraft... A masterclass in balance and poise. You fought like one who has stared into the teeth of death and refused to flinch. The Crusader Plate is yours by every right of arms.",

                "That plate was tempered in tempest-bellowed coals; with it, the freezing winds of the eastern passes will no longer tear you asunder. Wear it with pride, Champion of the Gale Winds.",

                "Your martial mastery is recognized throughout our garrison. The Royal Combat Arena has been unsealed in your honor—you may enter its trials from the lobby portal whenever you seek to test your limits."

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

                lenonPages.push_back("Hold... Lower your hand. That blade at your side... I would recognize that radiant hue anywhere in this dying realm. It is Truth Seeker—the consecrated steel Zil carried when he rode south.");

                lenonPages.push_back("Tell me... did he fall with his honor intact? ...Then his long torment is finally ended. For granting my sworn brother peace, Zyphden owes you a debt that words can scarcely repay.");

                lenonPages.push_back("Before we cross blades in the yard, courtly protocol demands you present yourself to King Kreptu in the keep above. Seek His Majesty's blessing first. If you survive his scrutiny, I shall be waiting for you in the eastern ring.");

            } else if (player.GetInventory().HasQuestItem("zil_token")) {

                lenonPages.push_back("Hold your step, traveler. That scorched silver crest upon your tunic... that was forged for Zil's breastplate with my own hands. If he gave you that token instead of taking your life, then some ember of his old honor still flickers amidst the ruins.");

                lenonPages.push_back("A warrior who earns Zil's mercy is either extraordinarily fortunate or possessed of rare conviction. Before I test which one you are, courtly protocol demands you present yourself to King Kreptu in the keep above.");

                lenonPages.push_back("Seek His Majesty's blessing first. Once the crown sanctions our trial, meet me at the Sparring Ring in the Eastern Courtyard. We shall see what mettle you possess.");

            } else {

                lenonPages.push_back("Hold your step, wanderer. Whispers travel fast upon the wind—our scouts reported a lone warrior cutting through the corrupted mire of the Crossroads.");

                lenonPages.push_back("Before I grant you a trial in our courtyard, courtly protocol demands you present yourself to King Kreptu in the keep above. Seek His Majesty's blessing first.");

                lenonPages.push_back("Once the crown sanctions our bout, meet me at the Sparring Ring in the Eastern Courtyard. We shall see if the rumors do you justice.");

            }



            OpenDialogue("lenon_tavern_intro", "KNIGHT LENON - VANGUARD CAPTAIN", lenonPages, Color{ 225, 185, 65, 255 });

        }

    }



    // NPC & Lore & Door Interactivity

    bool interactKey = IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);

    bool talkKey = IsKeyPressed(KEY_T);

    Vector2 playerPos = player.GetPosition();



    if (isForgeOpen || isProvisionsOpen) {

        if (IsKeyPressed(KEY_ESCAPE)) {

            isForgeOpen = false;

            isProvisionsOpen = false;

            SoundFX::PlayButtonClick();

        }

    } else if (!isDialogueOpen) {

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

            if (eldonInRange) {

                if (questPhase == ZyphdenQuestPhase::SEEK_TAVERN) {

                    if (interactKey || talkKey) {

                        std::vector<std::string> pages = {

                            "Step close to the hearth and let the chill seep out of your bones, traveler. After what you’ve walked through on the Crossroads highway, you look like you haven't slept under a dry roof in weeks. The fire is warm, and the stew is fresh.",

                            "Zyphden used to be a city of open laughter and high banners, but ever since the northern roads went quiet, every stranger at my door brings fresh dread. Listen... those heavy footsteps outside the archway. That’s Captain Lenon’s stride. He rarely visits the tavern unless the wind carries trouble."

                        };

                        OpenDialogue("eldon_intro", "TAVERNKEEP ELDON", pages, Color{ 255, 190, 80, 255 });

                        particles.SpawnFloatingText(eldon.position, "Audience with Vanguard Captain", Color{ 255, 215, 80, 255 }, 22.0f);

                        SoundFX::PlayParrySuccess();

                    }

                } else {

                    if (talkKey) {

                        std::vector<std::string> pages = {

                            "Rest your feet as long as you have need, friend. The hearth fire never goes dark so long as I’m drawing breath. If you’re seeking purpose or a warm bed, the Captain and the King have their hands full of both.",

                            "The mountain brews I sell aren't fancy, but they will knit broken ribs together faster than any apothecary chant. Stock up before you venture into the tempest."

                        };

                        OpenDialogue("eldon_idle", "TAVERNKEEP ELDON", pages, Color{ 255, 190, 80, 255 });

                    } else if (interactKey) {

                        isProvisionsOpen = true;

                        isForgeOpen = false;

                        SoundFX::PlayButtonClick();

                    }

                }

            }

            // 3. Knight Lenon if inside tavern

            else if (interactKey && lenonInTavern && knightLenonNPC.CheckInteraction(playerPos)) {

                std::vector<std::string> pages = {

                    "Protocol is protocol, friend. King Kreptu awaits you in the Northern Keep. Once the monarch sanctions our trial, my blade will be waiting in the courtyard."

                };

                OpenDialogue("lenon_tavern_idle", "KNIGHT LENON", pages, Color{ 225, 185, 65, 255 });

            }

        } else if (inRoyalKeep) {

            // Inside Zyphden Royal Keep (Grand Sovereign Throne Hall - Expanded 1200x1100)

            // 1. Southern Doorway back to Zyphden exterior (doorway at {1000, 1250})

            if (doorCooldown <= 0.0f && (playerPos.y >= 1235.0f || (Vector2Distance(playerPos, { 1000.0f, 1240.0f }) <= 65.0f && interactKey))) {

                inRoyalKeep = false;

                doorCooldown = 0.5f;

                map.Init(MapEnvironment::ZYPHDEN_REALM);

                player.SetPosition({ 1000.0f, 355.0f });

                camera.target = player.GetPosition();

                SoundFX::PlayGateOpen();

                return;

            }



            // 2. Audience with King Kreptu on his Golden Throne Dais

            bool kingInRange = Vector2Distance(playerPos, kingKreptu.position) <= 95.0f;

            if (interactKey && kingInRange) {

                if (questPhase == ZyphdenQuestPhase::SEEK_KING) {

                    questPhase = ZyphdenQuestPhase::CHALLENGE_READY;

                    hasTalkedToKing = true;

                    std::vector<std::string> pages = {

                        "Step forward into the light, traveler. Lenon spoke of your arrival, and the wind carried word of your passage through the southern ruins.",

                        "In an age where King Charlz's madness rots the very foundation of Tydeshire, a kingdom cannot afford to lean on fragile men. Lenon wishes to test your blade in the courtyard, and I grant my royal sanction.",

                        "Go to the Eastern Courtyard. Face our captain, and prove that your will is as unyielding as the stone beneath our feet. Should you best him, the sacred Crusader Plate shall be yours."

                    };

                    OpenDialogue("king_sanction", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });

                    particles.SpawnFloatingText(kingKreptu.position, "Courtyard Trial Sanctioned!", Color{ 255, 220, 90, 255 }, 22.0f);

                    SoundFX::PlayParrySuccess();

                } else if (questPhase == ZyphdenQuestPhase::VICTORIOUS) {

                    questPhase = ZyphdenQuestPhase::EXPLORE_REALM;

                    std::vector<std::string> pages = {

                        "You bested Captain Lenon—a martial feat I have not witnessed in twenty winters of peace or war. The sacred Crusader Plate rests upon shoulders worthy of its weight.",

                        "Your true journey lies beyond our mountain ramparts. I hereby grant you the Crown's sanction to travel to the next kingdom through the northern frontier passes.",

                        "Hear me well, Champion: there is no haste. Take your time to explore Zyphden freely, sharpen your armaments at Varek's master forge, or test your mettle against the quarry beasts to the east. You may roam without worry."

                    };

                    OpenDialogue("king_win", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });

                    particles.SpawnFloatingText(kingKreptu.position, "The Frontier Unlocked!", Color{ 255, 220, 90, 255 }, 22.0f);

                    SoundFX::PlayParrySuccess();

                } else if (questPhase == ZyphdenQuestPhase::EXPLORE_REALM) {

                    std::vector<std::string> pages = {

                        "The northern border road is open whenever you are prepared to cross into the next realm.",

                        "Until then, you remain an honored champion of Zyphden. Roam our courtyards, workshops, and borders freely."

                    };

                    OpenDialogue("king_explore", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });

                } else if (questPhase == ZyphdenQuestPhase::CHALLENGE_READY) {

                    std::vector<std::string> pages = {

                        "Captain Lenon is not a man to be kept waiting in the yard. Go, traveler. Show our garrison what fire burns in your blood."

                    };

                    OpenDialogue("king_ready", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });

                } else {

                    std::vector<std::string> pages = {

                        "Heavy hangs the crown when old allies turn their blades toward one another. If you seek news of the town, speak with Eldon at the tavern first."

                    };

                    OpenDialogue("king_idle", "KING KREPTU - MONARCH OF ZYPHDEN", pages, Color{ 255, 215, 70, 255 });

                }

            }

            // 3. Chancellor Malakor

            else if (interactKey && malakor.CheckInteraction(playerPos)) {

                if (questPhase == ZyphdenQuestPhase::SEEK_KING) {

                    std::vector<std::string> pages = {

                        "Step lightly, traveler. If you seek royal sanction, approach His Majesty King Kreptu at the center of the throne dais ahead. The Privy Council defers all judgments of arms to the King alone."

                    };

                    OpenDialogue("malakor_seek", "CHANCELLOR MALAKOR", pages, Color{ 190, 130, 240, 255 });

                } else {

                    std::vector<std::string> pages = {

                        "Step lightly upon these flagstones, traveler. The Privy Council has little patience for mud-spattered wanderers, yet His Majesty insists on granting audience to any blade that survives the highway. I manage the kingdom's leylines and our dwindling grain stores while soldiers dream of glorious charges.",

                        "Lord Zil was our finest vanguard before the Liege of Kings vanished into the northern mist. When Charlz went mad, Zil believed he could anchor the southern reaches with the relic Truth Seeker. If his mind has fractured, then the corruption is no longer a rumor—it is at our throats.",

                        "Do not mistake our stone ramparts for absolute safety. When empires rot, they rot from the crown downward. Bolster your willpower, wanderer. Steel may turn aside an arrow, but only an iron mind can endure what is coming."

                    };

                    OpenDialogue("malakor", "CHANCELLOR MALAKOR", pages, Color{ 190, 130, 240, 255 });

                    particles.SpawnFloatingText(malakor.position, "Kingdom Politics & Lore Shared", Color{ 190, 130, 240, 255 }, 18.0f);

                }

            }

            // 4. Sentinel Donald

            else if (interactKey && donald.CheckInteraction(playerPos)) {

                if (questPhase == ZyphdenQuestPhase::SEEK_KING) {

                    std::vector<std::string> pages = {

                        "Keep two paces from the dais, wanderer. State your business to His Majesty King Kreptu upon the throne. Step forward with respect and do not make sudden moves."

                    };

                    OpenDialogue("donald_seek", "SENTINEL DONALD", pages, Color{ 100, 180, 255, 255 });

                } else {

                    std::vector<std::string> pages = {

                        "Keep two paces from the dais, wanderer. I care little what tales you brought from the southern highway—my orders are absolute. Let your blade rest easy in its scabbard, and we will have no quarrel.",

                        "My halberd was forged by Varek himself to crack dragonbone and pierce corrupted hides. It has cleaved fifty horrors along the border moors. Do not think to test our vigil."

                    };

                    OpenDialogue("donald", "SENTINEL DONALD", pages, Color{ 100, 180, 255, 255 });

                }

            }

            // 5. Sentinel Garrick

            else if (interactKey && garrick.CheckInteraction(playerPos)) {

                if (questPhase == ZyphdenQuestPhase::SEEK_KING) {

                    std::vector<std::string> pages = {

                        "Lower your hackles, Donald. Look at their eyes—they’ve got the quiet stillness of someone who has survived the ruins. Traveler, if you seek royal sanction for the trial, speak directly to King Kreptu upon the throne dais ahead. He awaits you."

                    };

                    OpenDialogue("garrick_seek", "SENTINEL GARRICK", pages, Color{ 100, 180, 255, 255 });

                } else {

                    std::vector<std::string> pages = {

                        "Lower your hackles, Donald. Look at their eyes—they’ve got the quiet stillness of someone who has dodged a fatal blow by a hair's breadth and countered before the blood hit the dirt.",

                        "A word of advice before you face Captain Lenon in the ring: the man fights like a raging gale. Don’t try to out-muscle his whirlwind. Watch his balance, wait for the split second his blade overextends, and parry with conviction. If you break his posture, you break his fight."

                    };

                    OpenDialogue("garrick", "SENTINEL GARRICK", pages, Color{ 100, 180, 255, 255 });

                }

            }

            // 6. Archivist Thaddeus

            else if (interactKey && thaddeus.CheckInteraction(playerPos)) {

                if (questPhase == ZyphdenQuestPhase::SEEK_KING) {

                    std::vector<std::string> pages = {

                        "Quiet, please... King Kreptu sits upon the Golden Throne ahead. Address His Majesty first before browsing our codices."

                    };

                    OpenDialogue("thaddeus_seek", "ARCHIVIST THADDEUS", pages, Color{ 85, 175, 255, 255 });

                } else {

                    std::vector<std::string> pages = {

                        "Quiet, please... even the dust here remembers what the world has forgotten. I am Thaddeus. For thirty winters I have tended these folios, chronicling how one man's madness could unravel five centuries of hard-won peace.",

                        "The stone monoliths you pass in the wilderness are not mere decorative monuments. They were raised by the First Wardens along convergence points of the earth. When a warrior meditates upon their carved runes, the soul remembers its own weight. It tempers your Will—allowing you to shrug off blows that would leave lesser fighters reeling in the dirt.",

                        "Seek out the monolith in our archive wing. Its inscriptions have survived fire, siege, and betrayal. Perhaps they will offer you the clarity they could not grant our fallen kings."

                    };

                    OpenDialogue("thaddeus", "ARCHIVIST THADDEUS", pages, Color{ 85, 175, 255, 255 });

                    particles.SpawnFloatingText(thaddeus.position, "Ancient Knowledge Shared", Color{ 85, 175, 255, 255 }, 18.0f);

                }

            }

            // 7. Lady Genevieve

            else if (interactKey && genevieve.CheckInteraction(playerPos)) {

                if (questPhase == ZyphdenQuestPhase::SEEK_KING) {

                    std::vector<std::string> pages = {

                        "Step forward to the throne dais and speak with King Kreptu, wanderer. Only His Majesty can grant you leave to duel Captain Lenon in the courtyard."

                    };

                    OpenDialogue("genevieve_seek", "LADY GENEVIEVE", pages, Color{ 245, 140, 160, 255 });

                } else {

                    std::vector<std::string> pages = {

                        "Ah, so you are the survivor everyone in the bailey is murmuring about. I am Lady Genevieve. My family funds the king's armories—which means I spend half my days lecturing hot-headed knights on the value of defensive craftsmanship.",

                        "Every weapon tells a philosophy, wanderer. The heavy mace crushes bone straight through plate armor by sheer blunt momentum, while twin daggers rely on breathless speed and slipping beneath a guard. Yet even the sharpest edge is useless if its bearer panics at the first wound.",

                        "Our quartermasters keep restorative draughts tucked beside the weapon racks. Take what you need from the chests. It does our treasury no good to leave healing draughts gathering dust while the realm bleeds."

                    };

                    OpenDialogue("genevieve", "LADY GENEVIEVE", pages, Color{ 245, 140, 160, 255 });

                    particles.SpawnFloatingText(genevieve.position, "Armory Secrets Shared", Color{ 245, 140, 160, 255 }, 18.0f);

                }

            }

            // 8. Inscribed Keep Monolith

            else if (interactKey && keepPillar.CheckInteraction(playerPos)) {

                keepPillar.read = true;

                player.GetInventory().UnlockFragment("frag_09");

                std::vector<std::string> pages = {

                    "RUNIC INSCRIPTION OF THE FIRST WARDENS: 'The stone pillar anchors what the storm seeks to tear away. Stand firm as unyielding bedrock.'",

                    "'For he who tempers his Will shall neither stumble before the hammer nor bend before the tyrant's decree. Poise is the soul's armor against madness.'"

                };

                OpenDialogue("keep_pillar", "INSCRIBED KEEP MONOLITH", pages, Color{ 100, 220, 255, 255 });

                particles.SpawnFloatingText(keepPillar.position, "Lore Fragment IX Unlocked!", Color{ 100, 220, 255, 255 }, 22.0f);

                SoundFX::PlayParrySuccess();

            }

            // 9. Royal Armory Relic Chest

            else if (interactKey && keepChest.CheckInteraction(playerPos)) {

                keepChest.opened = true;

                player.GetInventory().UnlockFragment("frag_10");

                player.GetInventory().AddHealthPotions(2);

                std::vector<std::string> pages = {

                    "You have unlatched the Royal Armory Relic Chest.",

                    "Obtained two flasks of Consecrated Health Elixir! Press [1] in battle to drink an elixir and restore 45 Health (7.0s cooldown).",

                    "Recovered Fragment X: 'The Royal Armory Archives' — recording the ancient arts of armor penetration, posture breaking, and counter ripostes."

                };

                OpenDialogue("keep_chest", "ROYAL ARMORY RELIC CHEST", pages, Color{ 255, 215, 80, 255 });

                particles.SpawnFloatingText(keepChest.position, "+2 Potions & Fragment X!", Color{ 120, 255, 170, 255 }, 22.0f);

                SoundFX::PlayHeal();

            }

            // 10. Keep Provisions Chest

            else if (interactKey && keepPantryChest.CheckInteraction(playerPos)) {

                keepPantryChest.opened = true;

                player.GetInventory().AddHealthPotions(1);

                std::vector<std::string> pages = {

                    "You opened the Keep Provisions Chest.",

                    "Obtained one flask of Consecrated Health Elixir! Restores 45 Health upon consumption."

                };

                OpenDialogue("keep_pantry", "KEEP PROVISIONS CHEST", pages, Color{ 120, 255, 170, 255 });

                particles.SpawnFloatingText(keepPantryChest.position, "+1 Health Potion!", Color{ 120, 255, 170, 255 }, 20.0f);

                SoundFX::PlayHeal();

            }

        } else if (inForge) {

            // Inside Blacksmith Varek's Forge Interior

            // Southern Doorway back to Zyphden exterior (doorway at {800, 915})

            if (doorCooldown <= 0.0f && (playerPos.y >= 910.0f || (Vector2Distance(playerPos, { 800.0f, 915.0f }) <= 65.0f && interactKey))) {

                inForge = false;

                doorCooldown = 0.5f;

                map.Init(MapEnvironment::ZYPHDEN_REALM);

                player.SetPosition({ 430.0f, 680.0f });

                camera.target = player.GetPosition();

                SoundFX::PlayGateOpen();

                return;

            }



            // Blacksmith Varek at anvil ({ 750.0f, 530.0f })

            if (varek.CheckInteraction(playerPos)) {

                if (talkKey) {

                    std::vector<std::string> pages = {

                        "That iron on your back looks like it was scavenged from a ditch, wanderer. Don't take offense—I'm a smith, not a courtier. In the wildlands beyond our gates, beast-hide and rusted mail will get you cleft in two before you can draw breath.",

                        "The Crusader Plate worn by Captain Lenon was folded a hundred times in wind-bellowed coals. If you want armor that will actually turn a demon's claw aside, you’ll have to earn it from him in the ring. Win or lose, respect the steel."

                    };

                    OpenDialogue("varek", "BLACKSMITH VAREK", pages, Color{ 255, 140, 50, 255 });

                    particles.SpawnFloatingText(varek.position, "Master Armorcraft Lore", Color{ 255, 150, 60, 255 }, 18.0f);

                } else if (interactKey) {

                    isForgeOpen = true;

                    isProvisionsOpen = false;

                    SoundFX::PlayButtonClick();

                }

            }

        } else {

            // Outside in Zyphden Realm

            // 0. Enter The Forge (Archway at { 430.0f, 656.0f })

            bool nearForgeDoor = (Vector2Distance(playerPos, { 430.0f, 656.0f }) <= 85.0f);

            bool inForgeDoorThreshold = (playerPos.x >= 390.0f && playerPos.x <= 470.0f && playerPos.y <= 665.0f && playerPos.y >= 635.0f);



            if (doorCooldown <= 0.0f && ((nearForgeDoor && interactKey) || inForgeDoorThreshold)) {

                inForge = true;

                doorCooldown = 0.5f;

                map.Init(MapEnvironment::FORGE_INTERIOR);

                player.SetPosition({ 800.0f, 870.0f });

                camera.target = player.GetPosition();

                SoundFX::PlayGateOpen();

                return;

            }



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

                player.SetPosition({ 1000.0f, 1210.0f });

                camera.target = player.GetPosition();

                SoundFX::PlayGateOpen();

                return;

            }



            // 3. Eastern Quarry Archway Gate (Courtyard Gateway at { 1680.0f, 700.0f })

            bool nearQuarryArchway = (Vector2Distance(playerPos, { 1680.0f, 700.0f }) <= 95.0f);

            if (nearQuarryArchway && interactKey && doorCooldown <= 0.0f) {

                if (questPhase < ZyphdenQuestPhase::VICTORIOUS) {

                    SoundFX::PlayHitImpact();

                    particles.SpawnFloatingText(player.GetPosition(), "Royal Decree: Spar with Captain Lenon in the courtyard before venturing east.", Color{ 255, 120, 120, 255 }, 20.0f);

                    particles.AddScreenShake(3.0f, 0.12f);

                } else {

                    SoundFX::PlayGateOpen();

                    currentState = GameState::QUARRY_TRANSITION;

                    return;

                }

            }



            if (interactKey) {

                // 2. Scholar Cedric (Zyphden Lore)

                if (cedric.CheckInteraction(playerPos)) {

                    std::vector<std::string> pages = {

                        "Zyphden was named for the gale tempests that sweep through these valley spires. Our founders believed the wind cleansed the land of rot. But now? Even the gales smell of old iron and sulfur.",

                        "When King Charlz vanished, the covenant between the five crowns dissolved into bitter suspicion. The seals guarding the borderlands are fracturing one by one. The shadows you witnessed in the southern ruins are merely the vanguard of a greater dark.",

                        "Beyond our eastern ramparts lie the untamed wildlands. Only those clad in consecrated plate can withstand the howling storms that guard the northern frontier."

                    };

                    OpenDialogue("cedric", "CHRONICLER CEDRIC", pages, Color{ 100, 200, 255, 255 });

                    particles.SpawnFloatingText(cedric.position, "Ancient Lore Shared", Color{ 100, 200, 255, 255 }, 18.0f);

                }

                // 3. Elder Maeve (Town Lore & Refugees)

                else if (maeve.CheckInteraction(playerPos)) {

                    std::vector<std::string> pages = {

                        "May the winds keep you safe, child. When the abominations crawled from the moors, half my village vanished into the mist. Those of us who made it within these stone walls owe our lives to King Kreptu’s bread and shelter.",

                        "Yet bread cannot cure terror. The nights grow colder, and the sentries look to the northern horizon with fear in their eyes. Keep your wits about you, traveler. In times like these, a clear head is worth more than a pouch of gold."

                    };

                    OpenDialogue("maeve", "ELDER MAEVE", pages, Color{ 160, 230, 160, 255 });

                    particles.SpawnFloatingText(maeve.position, "Town Lore Shared", Color{ 160, 230, 160, 255 }, 18.0f);

                }

                // 4. Town Noticeboard

                else if (noticeboard.CheckInteraction(playerPos)) {

                    player.GetInventory().UnlockFragment("frag_02");

                    player.GetInventory().UnlockFragment("frag_04");

                    std::vector<std::string> pages = {

                        "ROYAL PROCLAMATION: By order of King Kreptu, all able-bodied warriors entering Zyphden are subject to martial inspection. Captain Lenon oversees vanguard duels in the Eastern Courtyard.",

                        "CAUTIONARY WARNING: The eastern pass beyond the city walls is subject to violent atmospheric tempest anomalies. Entry without consecrated plate armor is strictly forbidden."

                    };

                    OpenDialogue("noticeboard", "ROYAL NOTICEBOARD", pages, Color{ 130, 220, 255, 255 });

                    particles.SpawnFloatingText(noticeboard.position, "Lore Fragment Unlocked!", Color{ 130, 220, 255, 255 }, 18.0f);

                }

                // 5. Lore Item: Wind Lectern (Fragment VI)

                else if (!windLectern.collected && windLectern.CheckInteraction(playerPos)) {

                    windLectern.collected = true;

                    player.GetInventory().UnlockFragment("frag_06");

                    SoundFX::PlayParrySuccess();

                    particles.SpawnFloatingText(windLectern.position, "DISCOVERED: Fragment VI (Gale Winds)!", Color{ 120, 220, 255, 255 }, 22.0f);

                    particles.SpawnSparks(windLectern.position, Color{ 120, 220, 255, 255 }, 24, 140.0f);

                }

                // 6. Lore Item: Vanguard Field Log (Fragment VII)

                else if (!vanguardLog.collected && vanguardLog.CheckInteraction(playerPos)) {

                    vanguardLog.collected = true;

                    player.GetInventory().UnlockFragment("frag_07");

                    SoundFX::PlayParrySuccess();

                    particles.SpawnFloatingText(vanguardLog.position, "DISCOVERED: Fragment VII (Lenon's Vigil)!", Color{ 255, 215, 80, 255 }, 22.0f);

                    particles.SpawnSparks(vanguardLog.position, Color{ 255, 215, 80, 255 }, 24, 140.0f);

                }

                // 7. Lore Item: Royal Archives (Fragment VIII)

                else if (!royalArchives.collected && royalArchives.CheckInteraction(playerPos)) {

                    royalArchives.collected = true;

                    player.GetInventory().UnlockFragment("frag_08");

                    SoundFX::PlayParrySuccess();

                    particles.SpawnFloatingText(royalArchives.position, "DISCOVERED: Fragment VIII (Sovereign's Burden)!", Color{ 255, 215, 80, 255 }, 22.0f);

                    particles.SpawnSparks(royalArchives.position, Color{ 255, 215, 80, 255 }, 24, 140.0f);

                }

                // 8. Knight Lenon in Eastern Courtyard

                else if (lenonSpawned && !sparActive && !lenonInTavern && knightLenonNPC.CheckInteraction(playerPos)) {

                    if (questPhase == ZyphdenQuestPhase::CHALLENGE_READY) {

                        std::vector<std::string> pages;

                        if (hasSparredOnce) {

                            pages = {

                                "Welcome back to the yard, friend. A true warrior learns more in defeat than in a hundred hollow victories.",

                                "Clear your mind, steady your poise, and remember the rhythm of the wind. Draw when you are ready!"

                            };

                        } else {

                            pages = {

                                "The King has given his blessing, and the courtyard belongs to us. No malice, no hollow pride—only steel testing steel. Draw, wanderer! Let us see if your resolve can withstand the tempests of Zyphden!"

                            };

                        }

                        OpenDialogue("lenon_spar_start", "KNIGHT LENON - VANGUARD CAPTAIN", pages, Color{ 225, 185, 65, 255 });

                    } else if (questPhase == ZyphdenQuestPhase::VICTORIOUS || questPhase == ZyphdenQuestPhase::EXPLORE_REALM) {

                        std::vector<std::string> pages = {

                            "A well-earned triumph, friend. You move like a storm given human form. May that armor serve you well in the dark lands ahead."

                        };

                        OpenDialogue("lenon_spar_win", "KNIGHT LENON", pages, Color{ 225, 185, 65, 255 });

                    } else if (questPhase == ZyphdenQuestPhase::SEEK_KING) {

                        std::vector<std::string> pages = {

                            "Protocol is protocol, friend. Speak with King Kreptu in the Northern Keep first! Once His Majesty sanctions our duel, meet me here in the Courtyard."

                        };

                        OpenDialogue("lenon_spar_wait", "KNIGHT LENON", pages, Color{ 225, 185, 65, 255 });

                    }

                }

            }

        }

    }



    // Camera smoothing with screen shake

    Vector2 shake = particles.GetScreenShakeOffset();

    camera.target.x += (player.GetPosition().x + shake.x - camera.target.x) * 0.12f;

    camera.target.y += (player.GetPosition().y + shake.y - camera.target.y) * 0.12f;

}



void ZyphdenHandler::Render(

    Camera2D& camera,

    const GameMap& map,

    const std::vector<Enemy>& enemies,

    const std::vector<Projectile>& projectiles,

    const std::vector<LootDrop>& lootDrops,

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

    map.DrawFloor(player.GetPosition());



    if (inTavern) {

        // Tavern Interior Entities

        eldon.Draw(time, player.GetPosition());

        if (eldon.CheckInteraction(player.GetPosition())) {

            if (questPhase == ZyphdenQuestPhase::SEEK_TAVERN) {

                DrawText("[E] Speak with Eldon", (int)eldon.position.x - 55, (int)eldon.position.y - 36, 11, Color{ 255, 215, 80, 245 });

            } else {

                DrawText("[E] Provisions  |  [T] Lore", (int)eldon.position.x - 65, (int)eldon.position.y - 36, 11, Color{ 255, 215, 80, 245 });

            }

        }

        if (lenonInTavern) {

            knightLenonNPC.Draw(time, player.GetPosition());

        }

        // Southern doorway prompt (only when near exit)

        if (Vector2Distance(player.GetPosition(), { 800.0f, 915.0f }) <= 120.0f) {

            DrawText("[E] EXIT TAVERN", 745, 900, 11, Color{ 255, 220, 120, 240 });

        }

    } else if (inForge) {

        // Forge Interior Entities

        varek.Draw(time, player.GetPosition());

        if (varek.CheckInteraction(player.GetPosition())) {

            DrawText("[E] Master Forge  |  [T] Lore", (int)varek.position.x - 70, (int)varek.position.y - 38, 11, Color{ 255, 215, 80, 245 });

        }

        // Southern doorway prompt (only when near exit)

        if (Vector2Distance(player.GetPosition(), { 800.0f, 915.0f }) <= 120.0f) {

            DrawText("[E] EXIT FORGE", 750, 900, 11, Color{ 255, 220, 120, 240 });

        }

    } else if (inRoyalKeep) {

        // Royal Keep Throne Hall Interior Entities (Expanded 1200x1100)

        keepPillar.Draw(time, player.GetPosition());

        keepChest.Draw(time, player.GetPosition());

        keepPantryChest.Draw(time, player.GetPosition());



        kingKreptu.Draw(time, player.GetPosition());

        malakor.Draw(time, player.GetPosition());

        donald.Draw(time, player.GetPosition());

        garrick.Draw(time, player.GetPosition());

        thaddeus.Draw(time, player.GetPosition());

        genevieve.Draw(time, player.GetPosition());



        // Southern doorway prompt (only when near exit)

        if (Vector2Distance(player.GetPosition(), { 1000.0f, 1240.0f }) <= 130.0f) {

            DrawText("[E] EXIT ROYAL KEEP", 930, 1226, 11, Color{ 255, 230, 120, 240 });

        }

    } else {

        // Zyphden Exterior Lore Collectible Items

        windLectern.Draw(time, player.GetPosition());

        vanguardLog.Draw(time, player.GetPosition());

        royalArchives.Draw(time, player.GetPosition());



        // Zyphden Exterior NPCs

        noticeboard.Draw(time, player.GetPosition());

        cedric.Draw(time, player.GetPosition());

        maeve.Draw(time, player.GetPosition());



        if (lenonSpawned && !sparActive && !lenonInTavern) {

            knightLenonNPC.Draw(time, player.GetPosition());

        }



        // Forge entrance prompt outside (at { 430.0f, 656.0f })

        if (Vector2Distance(player.GetPosition(), { 430.0f, 656.0f }) <= 110.0f) {

            DrawText("[E] ENTER THE FORGE (VAREK)", 340, 640, 11, Color{ 255, 200, 100, 240 });

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



        // Eastern Quarry Archway Gate prompt outside

        if (Vector2Distance(player.GetPosition(), { 1680.0f, 700.0f }) <= 140.0f) {

            if (questPhase < ZyphdenQuestPhase::VICTORIOUS) {

                FontManager::DrawGameTextShadow("[E] QUARRY ARCHWAY (LOCKED - SPAR WITH LENON FIRST)", 1470, 640, 11, Color{ 255, 110, 110, 240 });

            } else {

                FontManager::DrawGameTextShadow("[E] ENTER EASTERN QUARRY MINES", 1510, 640, 12, Color{ 255, 215, 80, 255 });

            }

        }

    }



    if (!inTavern && !inForge && !inRoyalKeep) {

        for (const auto& enemy : enemies) {

            enemy.DrawTelegraph();

        }

    }



    map.DrawWallsAndPillars(player.GetPosition());



    for (const auto& proj : projectiles) {

        proj.Draw();

    }



    if (!inTavern && !inForge && !inRoyalKeep) {

        for (const auto& enemy : enemies) {

            enemy.Draw();

        }

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



    bool hasUnread = player.GetInventory().HasUnreadFragments();

    const char* locationTitle = inRoyalKeep ? "ZYPHDEN ROYAL KEEP" : (inTavern ? "HEARTHSIDE TAVERN" : "ZYPHDEN");

    GameUI::DrawCampaignHUD(screenWidth, screenHeight, player, locationTitle, hasUnread);



    // Boss Health Bar during Courtyard Spar

    if (sparActive) {

        for (const auto& enemy : enemies) {

            if (enemy.GetType() == EnemyType::KNIGHT_LENON_BOSS && enemy.IsAlive()) {

                GameUI::DrawBossHealthBar(screenWidth, enemy.GetName(), enemy.GetHealth(), enemy.GetMaxHealth());

                break;

            }

        }

    }



    // Middle-Left Collapsible Objectives Tracker (Tutorial-Style Single-Objective Display)



    float bannerX = 24.0f;

    float bannerY = (float)screenHeight * 0.38f;



    if (!showObjectives) {

        // Collapsed Sleek Bookmark Tab

        const float tabW = 160.0f;

        const float tabH = 34.0f;

        Rectangle tabRec = { bannerX, bannerY, tabW, tabH };

        bool isHovered = CheckCollisionPointRec(mousePos, tabRec);



        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            showObjectives = true;

            SoundFX::PlayButtonClick();

        }



        DrawRectangleRounded(tabRec, 0.25f, 4, isHovered ? Color{ 28, 38, 56, 235 } : Color{ 16, 22, 34, 215 });

        DrawRectangleRoundedLines(tabRec, 0.25f, 4, isHovered ? Color{ 255, 215, 80, 240 } : Color{ 65, 85, 120, 200 });

        DrawCircle((int)bannerX + 16, (int)bannerY + 17, 4.0f, Color{ 255, 215, 80, 255 });

        FontManager::DrawGameTextShadow("[O] OBJECTIVES", (int)bannerX + 28, (int)bannerY + 11, 11, isHovered ? Color{ 255, 255, 255, 255 } : Color{ 200, 220, 245, 220 });

    } else {

        // Expanded Middle-Left Single-Objective Card (Matching Tutorial Display)

        const float bannerW = 620.0f;

        const float bannerH = 92.0f;

        Rectangle cardRec = { bannerX, bannerY, bannerW, bannerH };



        DrawRectangleRounded(cardRec, 0.15f, 6, Color{ 14, 18, 28, 235 });

        DrawRectangleRoundedLines(cardRec, 0.15f, 6, Color{ 205, 165, 75, 240 });

        DrawRectangleRoundedLines(Rectangle{ bannerX + 3.0f, bannerY + 3.0f, bannerW - 6.0f, bannerH - 6.0f }, 0.12f, 6, Color{ 45, 58, 85, 160 });



        int stepNum = 1;

        std::string objTitle = "";

        std::string objHint = "";



        switch (questPhase) {

            case ZyphdenQuestPhase::SEEK_TAVERN:

                stepNum = 1;

                objTitle = "TAVERN INQUIRY: Visit the Hearthside Tavern and speak with Tavernkeep Eldon [E].";

                objHint = "Enter the Tavern in southwest Zyphden to gather rumors of the realm";

                break;

            case ZyphdenQuestPhase::LENON_APPROACHING:

                stepNum = 2;

                objTitle = "VANGUARD CAPTAIN: Captain Lenon approaches outside! Speak with him regarding Zil.";

                objHint = "Speak with Knight Lenon outside the Tavern to hear King Kreptu's summons";

                break;

            case ZyphdenQuestPhase::SEEK_KING:

                stepNum = 3;

                objTitle = "ROYAL AUDIENCE: Seek King Kreptu on his throne dais in the Northern Royal Keep [E].";

                objHint = "Follow the central stone road north and enter the royal throne room";

                break;

            case ZyphdenQuestPhase::CHALLENGE_READY:

                stepNum = 4;

                objTitle = "COURTYARD CHALLENGE: Meet Knight Lenon in the courtyard to initiate the martial spar [E].";

                objHint = "Approach Knight Lenon near the central banners and speak with him to begin";

                break;

            case ZyphdenQuestPhase::SPARRING:

                stepNum = 5;

                objTitle = "MARTIAL DUEL: Spar with Knight Lenon! Master parries and break his poise.";

                objHint = "Timed Parry [F] deflects heavy strikes; Dash [Q] evades whirlwind slashes";

                break;

            case ZyphdenQuestPhase::VICTORIOUS:

                stepNum = 6;

                objTitle = "ROYAL DEBRIEF: Return to the Royal Keep and speak with King Kreptu [E].";

                objHint = "Enter the Royal Keep and report your victory over Lenon to the King";

                break;

            case ZyphdenQuestPhase::EXPLORE_REALM:

            default:

                stepNum = 7;

                objTitle = "THE OPEN REALM: Journey to the next kingdom across northern gate (open exploration).";

                objHint = "Explore Zyphden, craft blades at Varek's Forge, or enter the Eastern Quarry at your leisure";

                break;

        }



        std::string chapterHeader = "MAIN QUEST: ZYPHDEN  (" + std::to_string(stepNum) + "/7)";

        FontManager::DrawGameTextShadow(chapterHeader.c_str(), (int)bannerX + 16, (int)bannerY + 10, 11, Color{ 255, 215, 80, 255 });



        // Toggle button on top right of the card

        Rectangle toggleBtn = { bannerX + bannerW - 96.0f, bannerY + 8.0f, 84.0f, 22.0f };

        bool isTogHov = CheckCollisionPointRec(mousePos, toggleBtn);

        if (isTogHov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            showObjectives = false;

            SoundFX::PlayButtonClick();

        }

        DrawRectangleRounded(toggleBtn, 0.3f, 3, isTogHov ? Color{ 40, 52, 75, 220 } : Color{ 22, 28, 42, 180 });

        DrawRectangleRoundedLines(toggleBtn, 0.3f, 3, isTogHov ? Color{ 140, 210, 255, 240 } : Color{ 60, 80, 115, 200 });

        FontManager::DrawGameTextShadow("HIDE [O]", (int)toggleBtn.x + 16, (int)toggleBtn.y + 5, 10, isTogHov ? Color{ 255, 255, 255, 255 } : Color{ 180, 205, 235, 220 });



        FontManager::DrawGameTextShadow(objTitle.c_str(), (int)bannerX + 16, (int)bannerY + 34, 12, Color{ 255, 255, 255, 255 });

        FontManager::DrawGameTextShadow(objHint.c_str(), (int)bannerX + 16, (int)bannerY + 58, 11, Color{ 120, 220, 255, 220 });

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

    } else if (isForgeOpen) {

        DrawForgeModal(const_cast<Player&>(player), const_cast<ParticleSystem&>(particles), screenWidth, screenHeight, mousePos, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));

    } else if (isProvisionsOpen) {

        DrawProvisionsModal(const_cast<Player&>(player), const_cast<ParticleSystem&>(particles), screenWidth, screenHeight, mousePos, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));

    }



    if (!player.IsAlive()) {

        DrawRectangle(0, 0, screenWidth, screenHeight, Color{ 20, 5, 8, 210 });

        int titleW = MeasureText("FALLEN IN BATTLE", 38);

        DrawText("FALLEN IN BATTLE", (screenWidth - titleW) / 2, screenHeight / 2 - 40, 38, Color{ 255, 50, 60, 255 });

        int subW = MeasureText("Press [R] to Recover at Courtyard Bench  |  [L] Return to Lobby", 16);

        DrawText("Press [R] to Recover at Courtyard Bench  |  [L] Return to Lobby", (screenWidth - subW) / 2, screenHeight / 2 + 18, 16, Color{ 220, 225, 240, 230 });

    }

}



void ZyphdenHandler::DrawForgeModal(Player& player, ParticleSystem& particles, int screenWidth, int screenHeight, Vector2 mousePos, bool isMousePressed) {

    // Backdrop dimming

    DrawRectangle(0, 0, screenWidth, screenHeight, Color{ 6, 8, 14, 215 });



    const float modalW = 880.0f;

    const float modalH = 615.0f;

    const float modalX = ((float)screenWidth - modalW) * 0.5f;

    const float modalY = ((float)screenHeight - modalH) * 0.5f;



    Rectangle modalRec = { modalX, modalY, modalW, modalH };



    // Ornate double border panel

    DrawRectangleRounded(modalRec, 0.04f, 8, Color{ 15, 18, 26, 250 });

    DrawRectangleRoundedLines(modalRec, 0.04f, 8, Color{ 215, 175, 65, 255 });

    DrawRectangleLinesEx(Rectangle{ modalX + 4.0f, modalY + 4.0f, modalW - 8.0f, modalH - 8.0f }, 1.5f, Color{ 65, 85, 120, 180 });



    // Corner Filigree Accents

    float cfSize = 14.0f;

    DrawPolyLines(Vector2{ modalX + cfSize + 2, modalY + cfSize + 2 }, 4, cfSize, 45.0f, Color{ 255, 215, 80, 255 });

    DrawPolyLines(Vector2{ modalX + modalW - cfSize - 2, modalY + cfSize + 2 }, 4, cfSize, 45.0f, Color{ 255, 215, 80, 255 });

    DrawPolyLines(Vector2{ modalX + cfSize + 2, modalY + modalH - cfSize - 2 }, 4, cfSize, 45.0f, Color{ 255, 215, 80, 255 });

    DrawPolyLines(Vector2{ modalX + modalW - cfSize - 2, modalY + modalH - cfSize - 2 }, 4, cfSize, 45.0f, Color{ 255, 215, 80, 255 });



    // Header

    FontManager::DrawText("VAREK'S MASTER FORGE", (int)modalX + 32, (int)modalY + 20, 22, Color{ 255, 225, 95, 255 }, GameFont::TITLE);

    FontManager::DrawText("Craft armaments with Quarry Ore Scraps and temper steel with silver Shingles.", (int)modalX + 32, (int)modalY + 46, 12, Color{ 180, 205, 235, 210 }, GameFont::BODY);



    // Live Currency & Material Badges in Top-Right of Modal Header

    int playerShingles = player.GetInventory().GetShingles();

    int playerScraps = player.GetInventory().GetOreScraps();



    // 1. Silver Shingles Badge

    float shingleBadgeW = 140.0f;

    float shingleBadgeH = 32.0f;

    float shingleBadgeX = modalX + modalW - shingleBadgeW - 32.0f;

    float shingleBadgeY = modalY + 20.0f;

    DrawRectangleRounded(Rectangle{ shingleBadgeX, shingleBadgeY, shingleBadgeW, shingleBadgeH }, 0.25f, 4, Color{ 20, 26, 40, 240 });

    DrawRectangleRoundedLines(Rectangle{ shingleBadgeX, shingleBadgeY, shingleBadgeW, shingleBadgeH }, 0.25f, 4, Color{ 140, 180, 225, 220 });

    // Silver coin icon

    DrawRectanglePro(Rectangle{ shingleBadgeX + 18.0f, shingleBadgeY + 16.0f, 10.0f, 10.0f }, Vector2{ 5.0f, 5.0f }, 45.0f, Color{ 220, 235, 255, 255 });

    DrawPolyLines(Vector2{ shingleBadgeX + 18.0f, shingleBadgeY + 16.0f }, 4, 7.0f, 45.0f, Color{ 100, 135, 180, 255 });

    std::string shingleStr = std::to_string(playerShingles) + " Shingles";

    FontManager::DrawText(shingleStr, (int)shingleBadgeX + 34, (int)shingleBadgeY + 8, 12, Color{ 230, 245, 255, 255 }, GameFont::BODY);



    // 2. Ore Scraps Badge (Amber / Bronze)

    float scrapBadgeW = 140.0f;

    float scrapBadgeH = 32.0f;

    float scrapBadgeX = shingleBadgeX - scrapBadgeW - 12.0f;

    float scrapBadgeY = modalY + 20.0f;

    DrawRectangleRounded(Rectangle{ scrapBadgeX, scrapBadgeY, scrapBadgeW, scrapBadgeH }, 0.25f, 4, Color{ 26, 22, 16, 240 });

    DrawRectangleRoundedLines(Rectangle{ scrapBadgeX, scrapBadgeY, scrapBadgeW, scrapBadgeH }, 0.25f, 4, Color{ 225, 175, 55, 220 });

    // Amber scrap polygon icon

    DrawPoly(Vector2{ scrapBadgeX + 18.0f, scrapBadgeY + 16.0f }, 4, 7.2f, 45.0f, Color{ 255, 195, 55, 255 });

    DrawPolyLinesEx(Vector2{ scrapBadgeX + 18.0f, scrapBadgeY + 16.0f }, 4, 7.2f, 45.0f, 1.5f, Color{ 255, 235, 140, 255 });

    std::string scrapStr = std::to_string(playerScraps) + " Ore Scraps";

    FontManager::DrawText(scrapStr, (int)scrapBadgeX + 34, (int)scrapBadgeY + 8, 12, Color{ 255, 225, 120, 255 }, GameFont::BODY);



    // Category Tabs: [1] VANGUARD WEAPONS (4) | [2] SACRED RELICS (1)

    if (IsKeyPressed(KEY_ONE)) forgeTab = 0;

    if (IsKeyPressed(KEY_TWO)) forgeTab = 1;



    Rectangle tab1Rec = { modalX + 32.0f, modalY + 52.0f, 210.0f, 24.0f };

    Rectangle tab2Rec = { modalX + 252.0f, modalY + 52.0f, 200.0f, 24.0f };

    bool isT1Hov = CheckCollisionPointRec(mousePos, tab1Rec);

    bool isT2Hov = CheckCollisionPointRec(mousePos, tab2Rec);

    if (isT1Hov && isMousePressed) { forgeTab = 0; SoundFX::PlayButtonClick(); }

    if (isT2Hov && isMousePressed) { forgeTab = 1; SoundFX::PlayButtonClick(); }



    DrawRectangleRounded(tab1Rec, 0.25f, 4, (forgeTab == 0) ? Color{ 40, 56, 82, 255 } : (isT1Hov ? Color{ 28, 38, 56, 220 } : Color{ 18, 24, 36, 180 }));

    DrawRectangleRoundedLines(tab1Rec, 0.25f, 4, (forgeTab == 0) ? Color{ 255, 215, 80, 240 } : Color{ 60, 80, 110, 200 });

    FontManager::DrawText("[1] VANGUARD WEAPONS (4)", (int)tab1Rec.x + 14, (int)tab1Rec.y + 6, 11, (forgeTab == 0) ? Color{ 255, 225, 100, 255 } : Color{ 180, 200, 225, 220 }, GameFont::BODY);



    DrawRectangleRounded(tab2Rec, 0.25f, 4, (forgeTab == 1) ? Color{ 40, 56, 82, 255 } : (isT2Hov ? Color{ 28, 38, 56, 220 } : Color{ 18, 24, 36, 180 }));

    DrawRectangleRoundedLines(tab2Rec, 0.25f, 4, (forgeTab == 1) ? Color{ 255, 215, 80, 240 } : Color{ 60, 80, 110, 200 });

    FontManager::DrawText("[2] SACRED RELICS (1)", (int)tab2Rec.x + 18, (int)tab2Rec.y + 6, 11, (forgeTab == 1) ? Color{ 255, 225, 100, 255 } : Color{ 180, 200, 225, 220 }, GameFont::BODY);



    // Divider Line

    DrawLine((int)modalX + 30, (int)modalY + 82, (int)(modalX + modalW - 30), (int)modalY + 82, Color{ 70, 95, 135, 160 });



    struct ForgeEntry {

        WeaponType type;

        const char* name;

        const char* iconTag;

        int maxTier;

        int costT1;

        int costT2;

        const char* perkT1;

        const char* perkT2;

        Color accentColor;

        int craftScrapCost;

        int craftShingleCost;

        const char* craftDesc;

    };



    std::vector<ForgeEntry> entries;

    if (forgeTab == 0) {

        entries = {

            {

                WeaponType::SWORD,

                "Crusader's Broadsword",

                "BALANCED BLADE",

                2, 50, 120,

                "+4 Damage (22 Light / 80 Heavy)",

                "+8 Damage (26 Light / 90 Heavy), +5% Poise Knockback",

                Color{ 255, 215, 80, 255 },

                0, 0,

                "Tempered steel longsword of the Royal Vanguard."

            },

            {

                WeaponType::MACE,

                "War Mace",

                "CRUSHING HAMMER",

                2, 60, 140,

                "+6 Damage (34 Light / 112 Heavy)",

                "+12 Damage (40 Light / 128 Heavy), +10% Armor Pierce (30% Total)",

                Color{ 255, 150, 60, 255 },

                12, 40,

                "Heavy iron-shod mace. Shatters brittle Melancholic defense shells."

            },

            {

                WeaponType::DAGGERS,

                "Dual Daggers",

                "TWIN FLURRY",

                2, 45, 110,

                "+3 Damage (14 Light / 72 Heavy)",

                "+6 Damage (17 Light / 84 Heavy), +10% Lunge Critical",

                Color{ 80, 230, 190, 255 },

                8, 30,

                "Twin razor daggers. Enables lightning rapid strikes and evasive lunges."

            },

            {

                WeaponType::HALBERD,

                "Royal Halberd",

                "VANGUARD POLEARM",

                2, 65, 130,

                "+6 Damage (30 Light / 104 Heavy)",

                "+12 Damage (36 Light / 120 Heavy), +10% Armor Pierce (30% Total)",

                Color{ 140, 215, 255, 255 },

                10, 35,

                "Long ashwood halberd. Extended linear thrusts & wide 220-degree vault cleaves."

            }

        };

    } else {

        entries = {

            {

                WeaponType::TRUTH_SEEKER,

                "Truth Seeker",

                "HOLY SUNBLADE",

                1, 180, 0,

                "Consecration I (+6 Holy Beam Dmg -> 28, 32 Light / 96 Heavy)",

                "",

                Color{ 140, 220, 255, 255 },

                0, 0,

                "Ancient holy blade of Zil. Overcome his remnant at the Ruined Castle altar."

            }

        };

    }



    for (size_t i = 0; i < entries.size(); i++) {

        int col = (int)i % 2;

        int row = (int)i / 2;

        float cardX = modalX + 28.0f + (float)col * 416.0f;

        float cardY = modalY + 92.0f + (float)row * 224.0f;

        float cardW = 404.0f;

        float cardH = 216.0f;



        Rectangle cardRec = { cardX, cardY, cardW, cardH };

        bool isHov = CheckCollisionPointRec(mousePos, cardRec);



        bool isOwned = player.GetInventory().HasWeapon(entries[i].type);

        int currentTier = player.GetInventory().GetWeaponUpgrade(entries[i].type);

        bool isMax = isOwned && (currentTier >= entries[i].maxTier);

        int upgradeCost = (currentTier == 0) ? entries[i].costT1 : entries[i].costT2;

        bool canAffordUpgrade = isOwned && (playerShingles >= upgradeCost);



        bool canCraft = (!isOwned && entries[i].craftScrapCost > 0);

        bool canAffordCraft = canCraft && (playerScraps >= entries[i].craftScrapCost && playerShingles >= entries[i].craftShingleCost);



        // Card Background

        DrawRectangleRounded(cardRec, 0.08f, 6, isHov ? Color{ 22, 28, 42, 240 } : Color{ 17, 21, 31, 230 });

        DrawRectangleRoundedLines(cardRec, 0.08f, 6, isMax ? Color{ 100, 225, 160, 220 } : (isHov ? entries[i].accentColor : Color{ 55, 75, 105, 180 }));



        // Category Tag

        FontManager::DrawGameTextShadow(entries[i].iconTag, (int)cardX + 16, (int)cardY + 12, 10, Color{ 140, 175, 215, 200 });



        // Weapon Title

        FontManager::DrawText(entries[i].name, (int)cardX + 16, (int)cardY + 26, 15, Color{ 255, 255, 255, 255 }, GameFont::TITLE);



        // Action Button coordinates

        float btnW = cardW - 32.0f;

        float btnH = 36.0f;

        float btnX = cardX + 16.0f;

        float btnY = cardY + cardH - btnH - 12.0f;

        Rectangle btnRec = { btnX, btnY, btnW, btnH };

        bool isBtnHov = CheckCollisionPointRec(mousePos, btnRec);



        if (!isOwned) {

            // Weapon is NOT yet owned (Craftable via Scraps or Unlocked via Quest)

            if (canCraft) {

                // Status Badge: CRAFTABLE

                std::string badge = "[CRAFTABLE]";

                int tbw = FontManager::MeasureText(badge, 11);

                DrawRectangleRounded(Rectangle{ cardX + cardW - (float)tbw - 24.0f, cardY + 12.0f, (float)tbw + 14.0f, 20.0f }, 0.25f, 4, Color{ 45, 30, 16, 230 });

                FontManager::DrawGameTextShadow(badge.c_str(), (int)(cardX + cardW - (float)tbw - 17.0f), (int)cardY + 16, 11, Color{ 255, 185, 60, 255 });



                // Weapon description

                FontManager::DrawGameTextShadow(entries[i].craftDesc, (int)cardX + 16, (int)cardY + 50, 10, Color{ 200, 215, 235, 220 });



                // Crafting Cost Line

                FontManager::DrawGameTextShadow("FORGE CRAFTING COST:", (int)cardX + 16, (int)cardY + 76, 10, Color{ 255, 215, 80, 240 });

                std::string costText = std::to_string(entries[i].craftScrapCost) + " Ore Scraps  +  " + std::to_string(entries[i].craftShingleCost) + " Shingles";

                Color costCol = canAffordCraft ? Color{ 120, 255, 170, 255 } : Color{ 245, 110, 110, 255 };

                FontManager::DrawGameTextShadow(costText.c_str(), (int)cardX + 16, (int)cardY + 92, 11, costCol);



                // Crafting Button

                if (canAffordCraft) {

                    Color btnBg = isBtnHov ? Color{ 75, 55, 20, 255 } : Color{ 50, 36, 15, 240 };

                    Color btnBorder = isBtnHov ? Color{ 255, 220, 95, 255 } : Color{ 225, 175, 55, 220 };

                    DrawRectangleRounded(btnRec, 0.2f, 4, btnBg);

                    DrawRectangleRoundedLines(btnRec, 0.2f, 4, btnBorder);



                    std::string btnStr = "CRAFT " + std::string(entries[i].name);

                    int bw = FontManager::MeasureText(btnStr, 11);

                    FontManager::DrawGameTextShadow(btnStr.c_str(), (int)btnX + (int)(btnW - (float)bw) / 2, (int)btnY + 10, 11, Color{ 255, 235, 140, 255 });



                    if (isBtnHov && isMousePressed) {

                        player.GetInventory().SpendOreScraps(entries[i].craftScrapCost);

                        player.GetInventory().SpendShingles(entries[i].craftShingleCost);

                        player.GetInventory().UnlockWeapon(entries[i].type);

                        player.SetWeapon(entries[i].type);



                        SoundFX::PlayCrusaderClash();

                        SoundFX::PlayCoinPickup();

                        particles.SpawnSparks(player.GetPosition(), entries[i].accentColor, 35, 220.0f);

                        particles.AddScreenShake(4.5f, 0.18f);



                        std::string feedback = std::string(entries[i].name) + " Crafted & Equipped!";

                        particles.SpawnFloatingText(player.GetPosition(), feedback, Color{ 255, 220, 90, 255 }, 24.0f);

                    }

                } else {

                    DrawRectangleRounded(btnRec, 0.2f, 4, Color{ 28, 20, 20, 200 });

                    DrawRectangleRoundedLines(btnRec, 0.2f, 4, Color{ 110, 50, 55, 180 });

                    std::string needStr = "INSUFFICIENT MATERIALS";

                    int bw = FontManager::MeasureText(needStr, 11);

                    FontManager::DrawGameTextShadow(needStr.c_str(), (int)btnX + (int)(btnW - (float)bw) / 2, (int)btnY + 10, 11, Color{ 220, 110, 110, 220 });

                }

            } else {

                // Quest-locked weapon (e.g. Truth Seeker before defeating Zil)

                std::string badge = "[NOT ACQUIRED]";

                int tbw = FontManager::MeasureText(badge, 11);

                DrawRectangleRounded(Rectangle{ cardX + cardW - (float)tbw - 24.0f, cardY + 12.0f, (float)tbw + 14.0f, 20.0f }, 0.25f, 4, Color{ 25, 28, 38, 220 });

                FontManager::DrawGameTextShadow(badge.c_str(), (int)(cardX + cardW - (float)tbw - 17.0f), (int)cardY + 16, 11, Color{ 140, 155, 180, 200 });



                FontManager::DrawGameTextShadow(entries[i].craftDesc, (int)cardX + 16, (int)cardY + 52, 10, Color{ 160, 175, 195, 200 });

                FontManager::DrawGameTextShadow("REQUIRES: Defeat Corrupted Zil in Castle Ruins", (int)cardX + 16, (int)cardY + 84, 10, Color{ 230, 150, 70, 220 });



                DrawRectangleRounded(btnRec, 0.2f, 4, Color{ 20, 22, 28, 180 });

                DrawRectangleRoundedLines(btnRec, 0.2f, 4, Color{ 48, 54, 68, 180 });

                const char* lockLabel = "LOCKED (SEEK ZIL IN RUINS)";

                int bw = FontManager::MeasureText(lockLabel, 11);

                FontManager::DrawGameTextShadow(lockLabel, (int)btnX + (int)(btnW - (float)bw) / 2, (int)btnY + 10, 11, Color{ 130, 140, 155, 190 });

            }

        } else {

            // Weapon is OWNED — Display standard upgrade path!

            std::string tierBadge;

            Color tierCol;

            if (currentTier == 0) {

                tierBadge = "[TIER: BASE]";

                tierCol = Color{ 160, 180, 205, 220 };

            } else if (currentTier == 1) {

                tierBadge = (entries[i].type == WeaponType::TRUTH_SEEKER) ? "[CONSECRATED I]" : "[TIER +1]";

                tierCol = Color{ 120, 215, 255, 255 };

            } else {

                tierBadge = "[MASTERWORK +2]";

                tierCol = Color{ 110, 245, 170, 255 };

            }

            int tbw = FontManager::MeasureText(tierBadge, 11);

            DrawRectangleRounded(Rectangle{ cardX + cardW - (float)tbw - 24.0f, cardY + 12.0f, (float)tbw + 14.0f, 20.0f }, 0.25f, 4, Color{ 20, 30, 45, 220 });

            FontManager::DrawGameTextShadow(tierBadge.c_str(), (int)(cardX + cardW - (float)tbw - 17.0f), (int)cardY + 16, 11, tierCol);



            // Weapon Stats display

            WeaponStats currentStats = GetWeaponStats(entries[i].type, currentTier);

            std::string curDmgStr = "Current: " + std::to_string((int)currentStats.lightDamage) + " Light / " + std::to_string((int)currentStats.heavyDamage) + " Heavy";

            FontManager::DrawGameTextShadow(curDmgStr.c_str(), (int)cardX + 16, (int)cardY + 52, 11, Color{ 200, 220, 245, 240 });



            // Next Tier Perk description

            FontManager::DrawGameTextShadow("NEXT FORGE UPGRADE:", (int)cardX + 16, (int)cardY + 72, 10, Color{ 255, 215, 80, 240 });

            if (isMax) {

                FontManager::DrawGameTextShadow("Max masterwork tier attained. Blade is fully tempered.", (int)cardX + 16, (int)cardY + 88, 11, Color{ 110, 245, 170, 240 });

            } else if (currentTier == 0) {

                FontManager::DrawGameTextShadow(entries[i].perkT1, (int)cardX + 16, (int)cardY + 88, 11, Color{ 160, 235, 255, 240 });

                std::string costStr = "Cost: " + std::to_string(entries[i].costT1) + " Shingles";

                FontManager::DrawGameTextShadow(costStr.c_str(), (int)cardX + 16, (int)cardY + 108, 11, canAffordUpgrade ? Color{ 255, 225, 100, 255 } : Color{ 240, 100, 100, 255 });

            } else if (currentTier == 1) {

                FontManager::DrawGameTextShadow(entries[i].perkT2, (int)cardX + 16, (int)cardY + 88, 11, Color{ 160, 235, 255, 240 });

                std::string costStr = "Cost: " + std::to_string(entries[i].costT2) + " Shingles";

                FontManager::DrawGameTextShadow(costStr.c_str(), (int)cardX + 16, (int)cardY + 108, 11, canAffordUpgrade ? Color{ 255, 225, 100, 255 } : Color{ 240, 100, 100, 255 });

            }



            if (isMax) {

                DrawRectangleRounded(btnRec, 0.2f, 4, Color{ 24, 45, 35, 220 });

                DrawRectangleRoundedLines(btnRec, 0.2f, 4, Color{ 60, 160, 110, 200 });

                int bw = FontManager::MeasureText("MAX TIER TEMPERED", 12);

                FontManager::DrawGameTextShadow("MAX TIER TEMPERED", (int)btnX + (int)(btnW - (float)bw) / 2, (int)btnY + 10, 12, Color{ 110, 245, 170, 220 });

            } else {

                std::string btnLabel = (currentTier == 0) ? ("TEMPER TO +1 (" + std::to_string(upgradeCost) + " SHINGLES)") : ("FORGE TO +2 (" + std::to_string(upgradeCost) + " SHINGLES)");

                if (entries[i].type == WeaponType::TRUTH_SEEKER) {

                    btnLabel = "CONSECRATE BLADE (" + std::to_string(upgradeCost) + " SHINGLES)";

                }



                if (canAffordUpgrade) {

                    Color btnBg = isBtnHov ? Color{ 55, 80, 125, 255 } : Color{ 32, 48, 76, 240 };

                    Color btnBorder = isBtnHov ? Color{ 255, 220, 95, 255 } : Color{ 80, 130, 200, 220 };

                    DrawRectangleRounded(btnRec, 0.2f, 4, btnBg);

                    DrawRectangleRoundedLines(btnRec, 0.2f, 4, btnBorder);

                    int bw = FontManager::MeasureText(btnLabel, 11);

                    FontManager::DrawGameTextShadow(btnLabel.c_str(), (int)btnX + (int)(btnW - (float)bw) / 2, (int)btnY + 10, 11, Color{ 255, 255, 255, 255 });



                    if (isBtnHov && isMousePressed) {

                        player.GetInventory().SpendShingles(upgradeCost);

                        player.GetInventory().UpgradeWeapon(entries[i].type);

                        SoundFX::PlayCrusaderClash();

                        SoundFX::PlayCoinPickup();

                        particles.SpawnSparks(player.GetPosition(), Color{ 255, 215, 80, 255 }, 24, 180.0f);

                        std::string feedback = std::string(entries[i].name) + " Upgraded!";

                        particles.SpawnFloatingText(player.GetPosition(), feedback, Color{ 255, 220, 90, 255 }, 22.0f);

                    }

                } else {

                    DrawRectangleRounded(btnRec, 0.2f, 4, Color{ 28, 20, 24, 200 });

                    DrawRectangleRoundedLines(btnRec, 0.2f, 4, Color{ 110, 50, 55, 180 });

                    std::string needStr = "INSUFFICIENT SHINGLES (" + std::to_string(upgradeCost) + " NEEDED)";

                    DrawText(needStr.c_str(), (int)btnX + (int)(btnW - MeasureText(needStr.c_str(), 11)) / 2, (int)btnY + 12, 11, Color{ 220, 110, 110, 220 });

                }

            }

        }

    }



    // Bottom Footer Buttons

    // 1. Discuss Armorcraft Lore Button

    Rectangle loreBtnRec = { modalX + 32.0f, modalY + modalH - 46.0f, 240.0f, 32.0f };

    bool isLoreHov = CheckCollisionPointRec(mousePos, loreBtnRec);

    DrawRectangleRounded(loreBtnRec, 0.25f, 4, isLoreHov ? Color{ 40, 55, 85, 240 } : Color{ 24, 32, 50, 220 });

    DrawRectangleRoundedLines(loreBtnRec, 0.25f, 4, isLoreHov ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 200 });

    DrawText("[T] Discuss Armorcraft Lore", (int)loreBtnRec.x + 20, (int)loreBtnRec.y + 10, 12, isLoreHov ? Color{ 255, 235, 120, 255 } : Color{ 180, 210, 245, 230 });



    if (isLoreHov && isMousePressed) {

        isForgeOpen = false;

        std::vector<std::string> pages = {

            "That iron on your back looks like it was scavenged from a ditch, wanderer. Don't take offense—I'm a smith, not a courtier. In the wildlands beyond our gates, beast-hide and rusted mail will get you cleft in two before you can draw breath.",

            "The Crusader Plate worn by Captain Lenon was folded a hundred times in wind-bellowed coals. If you want armor that will actually turn a demon's claw aside, you’ll have to earn it from him in the ring. Win or lose, respect the steel."

        };

        OpenDialogue("varek", "BLACKSMITH VAREK", pages, Color{ 255, 140, 50, 255 });

    }



    // 2. Leave Forge (ESC) Button

    Rectangle closeBtnRec = { modalX + modalW - 190.0f, modalY + modalH - 46.0f, 158.0f, 32.0f };

    bool isCloseHov = CheckCollisionPointRec(mousePos, closeBtnRec);

    DrawRectangleRounded(closeBtnRec, 0.25f, 4, isCloseHov ? Color{ 55, 30, 35, 240 } : Color{ 32, 20, 24, 220 });

    DrawRectangleRoundedLines(closeBtnRec, 0.25f, 4, isCloseHov ? Color{ 255, 100, 110, 240 } : Color{ 120, 55, 65, 200 });

    DrawText("LEAVE FORGE [ESC]", (int)closeBtnRec.x + 18, (int)closeBtnRec.y + 10, 12, isCloseHov ? Color{ 255, 180, 180, 255 } : Color{ 220, 140, 145, 220 });



    if ((isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE)) {

        isForgeOpen = false;

        SoundFX::PlayButtonClick();

    }

}



void ZyphdenHandler::DrawProvisionsModal(Player& player, ParticleSystem& particles, int screenWidth, int screenHeight, Vector2 mousePos, bool isMousePressed) {

    // Backdrop dimming

    DrawRectangle(0, 0, screenWidth, screenHeight, Color{ 6, 8, 14, 215 });



    const float modalW = 600.0f;

    const float modalH = 460.0f;

    const float modalX = ((float)screenWidth - modalW) * 0.5f;

    const float modalY = ((float)screenHeight - modalH) * 0.5f;



    Rectangle modalRec = { modalX, modalY, modalW, modalH };



    // Ornate double border panel

    DrawRectangleRounded(modalRec, 0.05f, 8, Color{ 16, 20, 28, 250 });

    DrawRectangleRoundedLines(modalRec, 0.05f, 8, Color{ 215, 175, 65, 255 });

    DrawRectangleLinesEx(Rectangle{ modalX + 4.0f, modalY + 4.0f, modalW - 8.0f, modalH - 8.0f }, 1.5f, Color{ 65, 85, 120, 180 });



    // Corner Filigree Accents

    float cfSize = 12.0f;

    DrawPolyLines(Vector2{ modalX + cfSize + 2, modalY + cfSize + 2 }, 4, cfSize, 45.0f, Color{ 255, 215, 80, 255 });

    DrawPolyLines(Vector2{ modalX + modalW - cfSize - 2, modalY + cfSize + 2 }, 4, cfSize, 45.0f, Color{ 255, 215, 80, 255 });

    DrawPolyLines(Vector2{ modalX + cfSize + 2, modalY + modalH - cfSize - 2 }, 4, cfSize, 45.0f, Color{ 255, 215, 80, 255 });

    DrawPolyLines(Vector2{ modalX + modalW - cfSize - 2, modalY + modalH - cfSize - 2 }, 4, cfSize, 45.0f, Color{ 255, 215, 80, 255 });



    // Header

    FontManager::DrawText("HEARTHSIDE PROVISIONS", (int)modalX + 32, (int)modalY + 22, 22, Color{ 255, 225, 95, 255 }, GameFont::TITLE);

    FontManager::DrawText("Restorative herbal elixirs brewed by Tavernkeep Eldon to sustain pilgrims in battle.", (int)modalX + 32, (int)modalY + 48, 12, Color{ 180, 205, 235, 210 }, GameFont::BODY);



    // Silver Shingles live balance badge in top-right of modal

    int playerShingles = player.GetInventory().GetShingles();

    float shingleBadgeW = 150.0f;

    float shingleBadgeH = 34.0f;

    float shingleBadgeX = modalX + modalW - shingleBadgeW - 32.0f;

    float shingleBadgeY = modalY + 20.0f;

    DrawRectangleRounded(Rectangle{ shingleBadgeX, shingleBadgeY, shingleBadgeW, shingleBadgeH }, 0.25f, 4, Color{ 20, 26, 40, 240 });

    DrawRectangleRoundedLines(Rectangle{ shingleBadgeX, shingleBadgeY, shingleBadgeW, shingleBadgeH }, 0.25f, 4, Color{ 140, 180, 225, 220 });

    DrawRectanglePro(Rectangle{ shingleBadgeX + 18.0f, shingleBadgeY + 17.0f, 10.0f, 10.0f }, Vector2{ 5.0f, 5.0f }, 45.0f, Color{ 220, 235, 255, 255 });

    DrawPolyLines(Vector2{ shingleBadgeX + 18.0f, shingleBadgeY + 17.0f }, 4, 7.0f, 45.0f, Color{ 100, 135, 180, 255 });

    std::string shingleStr = std::to_string(playerShingles) + " Shingles";

    FontManager::DrawText(shingleStr, (int)shingleBadgeX + 34, (int)shingleBadgeY + 9, 13, Color{ 230, 245, 255, 255 }, GameFont::BODY);



    // Divider Line

    DrawLine((int)modalX + 30, (int)modalY + 76, (int)(modalX + modalW - 30), (int)modalY + 76, Color{ 70, 95, 135, 160 });



    // Main Provisions Card: Health Elixir Flask

    float cardX = modalX + 32.0f;

    float cardY = modalY + 96.0f;

    float cardW = modalW - 64.0f;

    float cardH = 260.0f;



    Rectangle cardRec = { cardX, cardY, cardW, cardH };

    DrawRectangleRounded(cardRec, 0.08f, 6, Color{ 18, 24, 34, 230 });

    DrawRectangleRoundedLines(cardRec, 0.08f, 6, Color{ 80, 120, 170, 200 });



    // Potion Flask Drawing

    float flaskCenterX = cardX + 70.0f;

    float flaskCenterY = cardY + 80.0f;

    // Glowing emerald aura

    DrawCircleGradient(Vector2{ flaskCenterX, flaskCenterY }, 38.0f, Color{ 40, 220, 120, 60 }, Color{ 20, 140, 80, 0 });

    // Glass flask bottle

    DrawCircle((int)flaskCenterX, (int)(flaskCenterY + 10), 22.0f, Color{ 25, 160, 95, 220 });

    DrawRectangle((int)(flaskCenterX - 8), (int)(flaskCenterY - 18), 16, 18, Color{ 40, 180, 110, 230 });

    DrawRectangle((int)(flaskCenterX - 11), (int)(flaskCenterY - 24), 22, 6, Color{ 150, 110, 70, 255 }); // Cork

    DrawCircle((int)(flaskCenterX - 6), (int)(flaskCenterY + 4), 5.0f, Color{ 180, 255, 210, 180 }); // Glass highlight



    // Title and Info

    FontManager::DrawText("CONSECRATED HEALTH ELIXIR", (int)cardX + 130, (int)cardY + 24, 17, Color{ 120, 255, 170, 255 }, GameFont::TITLE);

    DrawText("Concentrated restorative draught brewed from dried mountain bellflowers and blossom honey.", (int)cardX + 130, (int)cardY + 48, 11, Color{ 185, 210, 235, 220 });

    DrawText("Restores 45 Vitality upon consumption. Press [1] in combat to drink (7.0s cooldown).", (int)cardX + 130, (int)cardY + 66, 11, Color{ 130, 225, 255, 230 });



    int currentPotions = player.GetInventory().GetHealthPotionCount();

    const int maxPotions = 5;

    bool atMax = (currentPotions >= maxPotions);

    const int flaskCost = 25;

    bool canAfford = (playerShingles >= flaskCost);



    // Stock & Price Bar

    DrawRectangleRounded(Rectangle{ cardX + 24.0f, cardY + 136.0f, cardW - 48.0f, 36.0f }, 0.2f, 4, Color{ 24, 32, 46, 220 });

    DrawRectangleRoundedLines(Rectangle{ cardX + 24.0f, cardY + 136.0f, cardW - 48.0f, 36.0f }, 0.2f, 4, Color{ 60, 85, 120, 180 });



    std::string stockStr = "CARRIED FLASKS: " + std::to_string(currentPotions) + " / " + std::to_string(maxPotions) + (atMax ? " [MAX CAPACITY]" : "");

    DrawText(stockStr.c_str(), (int)cardX + 38, (int)cardY + 148, 12, atMax ? Color{ 110, 245, 170, 255 } : Color{ 220, 235, 255, 240 });



    std::string priceStr = "PRICE: 25 SHINGLES";

    DrawText(priceStr.c_str(), (int)(cardX + cardW - 190.0f), (int)cardY + 148, 12, Color{ 255, 215, 80, 255 });



    // Purchase Button

    float pBtnW = cardW - 48.0f;

    float pBtnH = 44.0f;

    float pBtnX = cardX + 24.0f;

    float pBtnY = cardY + 190.0f;

    Rectangle pBtnRec = { pBtnX, pBtnY, pBtnW, pBtnH };

    bool isPBtnHov = CheckCollisionPointRec(mousePos, pBtnRec);



    if (atMax) {

        DrawRectangleRounded(pBtnRec, 0.2f, 4, Color{ 20, 36, 28, 220 });

        DrawRectangleRoundedLines(pBtnRec, 0.2f, 4, Color{ 60, 140, 95, 200 });

        DrawText("MAXIMUM FLASK CAPACITY ATTAINED (5/5)", (int)pBtnX + (int)(pBtnW - MeasureText("MAXIMUM FLASK CAPACITY ATTAINED (5/5)", 12)) / 2, (int)pBtnY + 15, 12, Color{ 110, 245, 170, 220 });

    } else if (canAfford) {

        Color pBtnBg = isPBtnHov ? Color{ 35, 85, 55, 255 } : Color{ 24, 60, 40, 240 };

        Color pBtnBorder = isPBtnHov ? Color{ 100, 255, 160, 255 } : Color{ 50, 170, 100, 220 };

        DrawRectangleRounded(pBtnRec, 0.2f, 4, pBtnBg);

        DrawRectangleRoundedLines(pBtnRec, 0.2f, 4, pBtnBorder);

        DrawText("PURCHASE HEALTH FLASK [25 SHINGLES]", (int)pBtnX + (int)(pBtnW - MeasureText("PURCHASE HEALTH FLASK [25 SHINGLES]", 12)) / 2, (int)pBtnY + 15, 12, Color{ 255, 255, 255, 255 });



        if (isPBtnHov && isMousePressed) {

            player.GetInventory().SpendShingles(flaskCost);

            player.GetInventory().AddHealthPotions(1);

            SoundFX::PlayCoinPickup();

            SoundFX::PlayHeal();

            particles.SpawnSparks(player.GetPosition(), Color{ 120, 255, 170, 255 }, 20, 140.0f);

            particles.SpawnFloatingText(player.GetPosition(), "+1 ELIXIR FLASK!", Color{ 120, 255, 170, 255 }, 22.0f);

        }

    } else {

        DrawRectangleRounded(pBtnRec, 0.2f, 4, Color{ 28, 20, 24, 200 });

        DrawRectangleRoundedLines(pBtnRec, 0.2f, 4, Color{ 110, 50, 55, 180 });

        DrawText("INSUFFICIENT SHINGLES (25 NEEDED)", (int)pBtnX + (int)(pBtnW - MeasureText("INSUFFICIENT SHINGLES (25 NEEDED)", 12)) / 2, (int)pBtnY + 15, 12, Color{ 220, 110, 110, 220 });

    }



    // Bottom Footer Buttons

    // 1. Hearth Tales / Rumors Button

    Rectangle talesBtnRec = { modalX + 32.0f, modalY + modalH - 52.0f, 210.0f, 32.0f };

    bool isTalesHov = CheckCollisionPointRec(mousePos, talesBtnRec);

    DrawRectangleRounded(talesBtnRec, 0.25f, 4, isTalesHov ? Color{ 40, 55, 85, 240 } : Color{ 24, 32, 50, 220 });

    DrawRectangleRoundedLines(talesBtnRec, 0.25f, 4, isTalesHov ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 200 });

    DrawText("[T] Hear Tavern Tales", (int)talesBtnRec.x + 24, (int)talesBtnRec.y + 10, 12, isTalesHov ? Color{ 255, 235, 120, 255 } : Color{ 180, 210, 245, 230 });



    if (isTalesHov && isMousePressed) {

        isProvisionsOpen = false;

        std::vector<std::string> pages = {

            "Rest your feet as long as you have need, friend. The hearth fire never goes dark so long as I’m drawing breath. If you’re seeking purpose or a warm bed, the Captain and the King have their hands full of both.",

            "The mountain brews I sell aren't fancy, but they will knit broken ribs together faster than any apothecary chant. Stock up before you venture into the tempest."

        };

        OpenDialogue("eldon_idle", "TAVERNKEEP ELDON", pages, Color{ 255, 190, 80, 255 });

    }



    // 2. Leave Counter (ESC) Button

    Rectangle closeBtnRec = { modalX + modalW - 200.0f, modalY + modalH - 52.0f, 168.0f, 32.0f };

    bool isCloseHov = CheckCollisionPointRec(mousePos, closeBtnRec);

    DrawRectangleRounded(closeBtnRec, 0.25f, 4, isCloseHov ? Color{ 55, 30, 35, 240 } : Color{ 32, 20, 24, 220 });

    DrawRectangleRoundedLines(closeBtnRec, 0.25f, 4, isCloseHov ? Color{ 255, 100, 110, 240 } : Color{ 120, 55, 65, 200 });

    DrawText("LEAVE COUNTER [ESC]", (int)closeBtnRec.x + 14, (int)closeBtnRec.y + 10, 12, isCloseHov ? Color{ 255, 180, 180, 255 } : Color{ 220, 140, 145, 220 });



    if ((isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE)) {

        isProvisionsOpen = false;

        SoundFX::PlayButtonClick();

    }

}



void ZyphdenHandler::SetLenonDefeated(bool d) {

    lenonDefeated = d;

    if (d && questPhase < ZyphdenQuestPhase::VICTORIOUS) {

        questPhase = ZyphdenQuestPhase::VICTORIOUS;

        lenonSpawned = true;

        lenonInTavern = false;

        knightLenonNPC.position = { 1490.0f, 700.0f };

    }

}



void ZyphdenHandler::SetQuestPhase(int phase) {

    if (phase < 0) phase = 0;

    questPhase = (ZyphdenQuestPhase)phase;

    if (questPhase == ZyphdenQuestPhase::EXPLORE_REALM) {

        lenonDefeated = true;

        lenonSpawned = true;

        lenonInTavern = false;

        hasTalkedToKing = true;

        knightLenonNPC.position = { 1490.0f, 700.0f };

    } else if (questPhase >= ZyphdenQuestPhase::VICTORIOUS) {

        lenonDefeated = true;

        lenonSpawned = true;

        lenonInTavern = false;

        knightLenonNPC.position = { 1490.0f, 700.0f };

    } else if (questPhase >= ZyphdenQuestPhase::SEEK_KING) {

        lenonDefeated = false;

        lenonSpawned = true;

        lenonInTavern = false;

        knightLenonNPC.position = { 1490.0f, 700.0f };

    } else if (questPhase == ZyphdenQuestPhase::LENON_APPROACHING) {

        lenonDefeated = false;

        lenonSpawned = true;

        lenonInTavern = true;

        knightLenonNPC.position = { 720.0f, 765.0f };

    } else {

        lenonDefeated = false;

        lenonSpawned = false;

        lenonInTavern = false;

    }

}

