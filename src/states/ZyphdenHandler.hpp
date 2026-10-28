#pragma once
#include "raylib.h"
#include "raymath.h"
#include "GameState.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../core/Particles.hpp"
#include "../combat/Projectile.hpp"
#include "../world/Interactable.hpp"
#include <vector>
#include <string>
#include <functional>

struct ZyphdenNPC {
    std::string id;
    std::string name;
    Vector2 position;
    float radius;
    Color accentColor;

    bool CheckInteraction(Vector2 playerPos) const {
        if (id == "king") {
            return Vector2Distance(playerPos, position) <= 95.0f;
        }
        return Vector2Distance(playerPos, position) <= (radius + 36.0f);
    }

    void Draw(float time, Vector2 playerPos = { -9999.0f, -9999.0f }) const;
};

struct ZyphdenLoreItem {
    std::string fragmentId;
    std::string name;
    Vector2 position;
    float radius;
    bool collected;

    ZyphdenLoreItem() : fragmentId(""), name(""), position{ 0.0f, 0.0f }, radius(24.0f), collected(false) {}
    ZyphdenLoreItem(const std::string& id, const std::string& name, Vector2 pos, float r, bool coll)
        : fragmentId(id), name(name), position(pos), radius(r), collected(coll) {}

    bool CheckInteraction(Vector2 playerPos) const {
        return !collected && Vector2Distance(playerPos, position) <= (radius + 32.0f);
    }

    void Draw(float time, Vector2 playerPos = { -9999.0f, -9999.0f }) const;
};

enum class ZyphdenQuestPhase {
    SEEK_TAVERN,      // 1. Visit Hearthside Tavern & speak with Tavernkeep Eldon
    LENON_APPROACHING,// 2. Lenon spawned outside tavern, walking up to player
    SEEK_KING,        // 3. Lenon spoke of Zil, directed player to King Kreptu
    CHALLENGE_READY,  // 4. King Kreptu granted audience; Lenon awaits in courtyard
    SPARRING,         // 5. Active courtyard boss duel
    VICTORIOUS,       // 6. Lenon defeated; Crusader Plate awarded! Report to King
    EXPLORE_REALM     // 7. King granted border commission; explore realm openly
};

class LootDrop;

class ZyphdenHandler {
public:
    ZyphdenHandler();

    void Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera);

    void Update(
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
    );

    void Render(
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
    );

    bool HasDefeatedLenon() const { return lenonDefeated; }
    void SetLenonDefeated(bool d) { lenonDefeated = d; }

private:
    // Core NPCs
    ZyphdenNPC eldon;
    ZyphdenNPC noticeboard;
    ZyphdenNPC kingKreptu;
    ZyphdenNPC knightLenonNPC;

    // Dedicated Lore NPCs of Zyphden
    ZyphdenNPC cedric; // Scholar Cedric - History of Zyphden & the Gale Winds
    ZyphdenNPC varek;  // Blacksmith Varek - Crusader Plate & Armorcraft Lore
    ZyphdenNPC maeve;  // Elder Maeve - Lore of the Border Refugees & Madness

    // 5 Throne Room NPCs in Expanded Royal Keep
    ZyphdenNPC malakor;   // Chancellor Malakor - Kingdom governance & lord liege lore
    ZyphdenNPC donald;    // Sentinel Donald - High Dais Guard (Halberd)
    ZyphdenNPC garrick;   // Sentinel Garrick - High Dais Guard (Shield & Parry Master)
    ZyphdenNPC thaddeus;  // Archivist Thaddeus - Western Archives Custodian
    ZyphdenNPC genevieve; // Lady Genevieve - Eastern Armory Weapons Patron

    // Royal Keep Lore & Supply Interactables
    InscribedPillar keepPillar;
    RelicChest keepChest;
    RelicChest keepPantryChest;

    // Journal Lore Collectibles in Zyphden
    ZyphdenLoreItem windLectern;   // frag_06: Rite of the Gale Winds
    ZyphdenLoreItem vanguardLog;   // frag_07: Lenon's Field Log
    ZyphdenLoreItem royalArchives; // frag_08: The Sovereign's Burden

    // Map & Interior State
    bool inTavern = false;
    bool inForge = false;
    bool inRoyalKeep = false;
    float doorCooldown = 0.0f;
    Vector2 tavernExitSpawnPos = { 635.0f, 885.0f };
    Vector2 forgeExitSpawnPos = { 430.0f, 680.0f };

    // Quest Progression State Machine
    ZyphdenQuestPhase questPhase = ZyphdenQuestPhase::SEEK_TAVERN;
    bool hasTalkedToKing = false;
    bool sparActive = false;
    bool sparPendingStart = false;
    bool hasSparredOnce = false;
    bool lenonDefeated = false;
    bool lenonSpawned = false;
    bool lenonInTavern = false;
    float lenonSpawnTimer = 0.0f;

    // HUD & Banners
    mutable bool showObjectives = true;
    float entryBannerTimer = 0.0f;

    // Paginated Dialogue Box State
    bool isDialogueOpen = false;
    std::string currentDialogueNPCId = "";
    std::string dialogueSpeaker = "";
    std::vector<std::string> dialoguePages;
    int currentDialoguePage = 0;
    Color dialogueColor = WHITE;

    // Forge & Provisions Modal State
    bool isForgeOpen = false;
    bool isProvisionsOpen = false;

    void OpenDialogue(const std::string& npcId, const std::string& speaker, const std::vector<std::string>& pages, Color color);
    void CloseDialogue();
    void DrawForgeModal(Player& player, ParticleSystem& particles, int screenWidth, int screenHeight, Vector2 mousePos, bool isMousePressed);
    void DrawProvisionsModal(Player& player, ParticleSystem& particles, int screenWidth, int screenHeight, Vector2 mousePos, bool isMousePressed);
};
