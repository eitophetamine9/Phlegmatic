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
#include "../combat/LootDrop.hpp"
#include "Cutscene.hpp"
#include "Tutorial.hpp"
#include <vector>

#include "../combat/CollisionSystem.hpp"
#include "../world/WaveManager.hpp"
#include "RuinedCastleHandler.hpp"
#include "CrossroadsHandler.hpp"
#include "ZyphdenHandler.hpp"
#include "QuarryHandler.hpp"
#include "RestlessMistForestHandler.hpp"
#include <functional>
#include <string>

enum class ScreenFadeMode {
    NONE,
    FADE_OUT,
    HOLD,
    FADE_IN
};

struct RealmTransition {
    ScreenFadeMode mode = ScreenFadeMode::NONE;
    float alpha = 0.0f;
    float timer = 0.0f;
    float fadeOutDuration = 0.35f;
    float holdDuration = 0.25f;
    float fadeInDuration = 0.40f;
    std::function<void()> onMidpointSwitch = nullptr;
    std::string realmTitle = "";
    std::string realmSubtitle = "";
};

class Game {
public:
    void StartRealmTransition(const std::function<void()>& onSwitch, const std::string& title = "", const std::string& subtitle = "", float fadeOut = 0.35f, float hold = 0.25f, float fadeIn = 0.40f);
    bool IsTransitionActive() const;
public:
    Game();
    ~Game();

    void Run();

private:
    void Init();
    void Update(float dt);
    void Render();
    void RenderScene();
    void ToggleGameFullscreen();

    void EnterRuinedCastle();
    void EnterCrossroads();
    void EnterZyphden();
    void EnterQuarryDepths();
    void EnterRestlessMistForest();
    void ReturnFromRestlessMistForest();
    void EnterLobby();
    void EnterArena();
    void ReturnFromArena();
    void SaveActiveGame();
    void LoadActiveGame(int slotIndex);

    GameState currentState;
    bool showControlsModal;
    bool showInventoryModal;
    bool showMapModal;
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
    std::vector<LootDrop> lootDrops;

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
    QuarryHandler quarryHandler;
    RestlessMistForestHandler restlessMistForestHandler;
    std::vector<AdhesiveBandage> arenaBandages;
    float bandageSpawnTimer;

    // Test Arena Unlock
    bool testArenaUnlocked;
    void SaveArenaUnlock();
    bool LoadArenaUnlock();

    // Virtual Viewport & Window State
    RenderTexture2D virtualTarget;
    bool isFullscreen;

    // Universal Realm Transition & Fade
    RealmTransition screenTransition;
    void UpdateScreenTransition(float dt);
    void RenderScreenTransition();
};
