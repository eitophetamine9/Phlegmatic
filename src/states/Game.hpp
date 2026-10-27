#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "UI.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/Map.hpp"
#include "../world/Interactable.hpp"
#include "../core/Particles.hpp"
#include "../rendering/AssetManager.hpp"
#include "../combat/Projectile.hpp"
#include "Cutscene.hpp"
#include "Tutorial.hpp"
#include <vector>

#include "../combat/CollisionSystem.hpp"
#include "../world/WaveManager.hpp"
#include "RuinedCastleHandler.hpp"
#include "CrossroadsHandler.hpp"
#include "ZyphdenHandler.hpp"

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    void Init();
    void Update(float dt);
    void Render();

    void EnterRuinedCastle();
    void EnterCrossroads();
    void EnterZyphden();
    void EnterLobby();
    void EnterArena();
    void SaveActiveGame();
    void LoadActiveGame(int slotIndex);

    GameState currentState;
    bool showControlsModal;
    bool showInventoryModal;
    bool showSlotModal;
    int deletingSlotIndex;
    bool isRestoringSave;
    int inventoryActiveTab;
    int inventorySelectedFragment;

    Camera2D camera;
    Player player;
    GameMap map;
    ParticleSystem particles;
    AssetManager assetManager;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;

    // Lobby Elements
    std::vector<WeaponPedestal> pedestals;
    TrainingDummy dummy;
    ArenaPortal portal;

    // Narrative & Cutscenes
    Cutscene cutscene;
    TutorialManager tutorial;

    // Combat & Arena Managers
    WaveManager waveManager;
    RuinedCastleHandler ruinedCastleHandler;
    CrossroadsHandler crossroadsHandler;
    ZyphdenHandler zyphdenHandler;
    std::vector<AdhesiveBandage> arenaBandages;
    float bandageSpawnTimer;

    // Test Arena Unlock
    bool testArenaUnlocked;
    void SaveArenaUnlock();
    bool LoadArenaUnlock();
};
