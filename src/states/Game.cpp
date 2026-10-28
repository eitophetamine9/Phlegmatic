#include "Game.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/FontManager.hpp"
#include "../core/SaveManager.hpp"
#include "raymath.h"
#include <cmath>
#include <string>
#include <algorithm>

Game::Game() : currentState(GameState::TITLE_SCREEN), showControlsModal(false), showInventoryModal(false), showMapModal(false), showSlotModal(false), deletingSlotIndex(0), isRestoringSave(false), inventoryActiveTab(0), inventorySelectedFragment(0), isFullscreen(false) {
}

Game::~Game() {
    UnloadRenderTexture(virtualTarget);
    SoundFX::Close();
    assetManager.Unload();
    FontManager::Unload();
    CloseWindow();
}

void Game::ToggleGameFullscreen() {
    if (!isFullscreen) {
        int monitor = GetCurrentMonitor();
        int mWidth = GetMonitorWidth(monitor);
        int mHeight = GetMonitorHeight(monitor);
        SetWindowSize(mWidth, mHeight);
        ToggleFullscreen();
        isFullscreen = true;
    } else {
        ToggleFullscreen();
        SetWindowSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
        isFullscreen = false;
    }
}

void Game::Init() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE);
    SetExitKey(KEY_NULL); // Prevent ESC from closing the program
    SetTargetFPS(Config::TARGET_FPS);

    virtualTarget = LoadRenderTexture(Config::VIRTUAL_WIDTH, Config::VIRTUAL_HEIGHT);
    SetTextureFilter(virtualTarget.texture, TEXTURE_FILTER_BILINEAR);
    isFullscreen = false;

    SoundFX::Init();
    FontManager::Init();
    assetManager.Init();

    pedestals.clear();
    pedestals.push_back({ { 600.0f, 650.0f }, WeaponType::SWORD });
    pedestals.push_back({ { 730.0f, 590.0f }, WeaponType::DAGGERS });
    pedestals.push_back({ { 870.0f, 590.0f }, WeaponType::MACE });
    pedestals.push_back({ { 1000.0f, 650.0f }, WeaponType::TRUTH_SEEKER });

    dummy.position = { 800.0f, 810.0f };
    portal.position = { 800.0f, 430.0f };

    camera.target = { 800.0f, 600.0f };
    camera.offset = { (float)Config::WINDOW_WIDTH * 0.5f, (float)Config::WINDOW_HEIGHT * 0.5f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    currentState = GameState::TITLE_SCREEN;
    showControlsModal = false;
    showSlotModal = false;
    deletingSlotIndex = 0;
    isRestoringSave = false;
    ruinedCastleHandler.Reset();
    testArenaUnlocked = LoadArenaUnlock();
    bandageSpawnTimer = 14.0f;
}

void Game::SaveArenaUnlock() {
    FILE* f = fopen("arena_unlock.dat", "w");
    if (f) {
        fprintf(f, "1\n");
        fclose(f);
    }
}

bool Game::LoadArenaUnlock() {
    FILE* f = fopen("arena_unlock.dat", "r");
    if (f) {
        int v = 0;
        if (fscanf(f, "%d", &v) == 1 && v == 1) {
            fclose(f);
            return true;
        }
        fclose(f);
    }
    return false;
}

void Game::EnterRuinedCastle() {
    map.Init(MapEnvironment::RUINED_CASTLE);
    player.Init({ 800.0f, 1180.0f });
    player.SetWeapon(WeaponType::NONE); // Start unarmed in tutorial!
    player.SetArmor(ArmorType::NONE);   // Start unarmored in tutorial!
    player.SetTotem(TotemType::NONE);   // Start with no totem in tutorial!
    player.GetInventory().ClearWeapons(); // No starting weapons in tutorial!
    player.GetInventory().ClearArmors();  // Only NONE available until plinth pickup!
    player.GetInventory().ClearTotems();  // Only NONE available until Crossroads chest!
    camera.target = player.GetPosition();
    currentState = GameState::RUINED_CASTLE;
    showControlsModal = false;
    showInventoryModal = false;
    ruinedCastleHandler.Reset();
    enemies.clear();
    projectiles.clear();
    lootDrops.clear();
    particles.Clear();
    tutorial.Init();
    SaveActiveGame();
}

void Game::EnterCrossroads() {
    crossroadsHandler.Init(player, map, enemies, projectiles, camera);
    currentState = GameState::CROSSROADS;
    showControlsModal = false;
    showInventoryModal = false;
    lootDrops.clear();
    particles.Clear();
    SaveActiveGame();
}

void Game::EnterZyphden() {
    zyphdenHandler.Init(player, map, enemies, projectiles, camera);
    currentState = GameState::ZYPHDEN_REALM;
    showControlsModal = false;
    showInventoryModal = false;
    lootDrops.clear();
    particles.Clear();
    SaveActiveGame();
}

void Game::EnterQuarryDepths() {
    quarryHandler.Init(player, map, enemies, projectiles, camera);
    currentState = GameState::QUARRY_DEPTHS;
    showControlsModal = false;
    showInventoryModal = false;
    showMapModal = false;
    lootDrops.clear();
    particles.Clear();
    SaveActiveGame();
}

void Game::EnterLobby() {
    map.Init(MapEnvironment::LOBBY);
    WeaponType keepWeapon = player.GetWeapon();
    player.Init({ 800.0f, 740.0f });
    if (keepWeapon != WeaponType::NONE) {
        player.SetWeapon(keepWeapon);
    } else if (player.GetInventory().HasWeapon(WeaponType::TRUTH_SEEKER)) {
        player.SetWeapon(WeaponType::TRUTH_SEEKER);
    } else if (player.GetInventory().HasWeapon(WeaponType::SWORD)) {
        player.SetWeapon(WeaponType::SWORD);
    }
    camera.target = player.GetPosition();
    currentState = GameState::LOBBY;
    showControlsModal = false;
    projectiles.clear();
    lootDrops.clear();
    particles.Clear();
    particles.SpawnFloatingText(player.GetPosition(), "LOBBY SANCTUARY", Color{ 255, 220, 100, 255 }, 24.0f);
    SaveActiveGame();
}

void Game::EnterArena() {
    map.Init(MapEnvironment::ARENA);
    WeaponType keepWeapon = player.GetWeapon();
    player.Init({ 1200.0f, 900.0f });
    if (keepWeapon != WeaponType::NONE) {
        player.SetWeapon(keepWeapon);
    } else if (player.GetInventory().HasWeapon(WeaponType::TRUTH_SEEKER)) {
        player.SetWeapon(WeaponType::TRUTH_SEEKER);
    } else if (player.GetInventory().HasWeapon(WeaponType::SWORD)) {
        player.SetWeapon(WeaponType::SWORD);
    }
    camera.target = player.GetPosition();
    projectiles.clear();
    lootDrops.clear();
    particles.Clear();
    arenaBandages.clear();
    bandageSpawnTimer = 14.0f;
    AdhesiveBandage initBandage;
    initBandage.position = { 1200.0f, 1050.0f };
    arenaBandages.push_back(initBandage);

    waveManager.SpawnWave(1, enemies, projectiles, particles);
    currentState = GameState::ARENA;
    showControlsModal = false;
    SoundFX::PlayEquipWeapon();
    particles.SpawnSparks(player.GetPosition(), Color{ 100, 220, 255, 255 }, 12, 180.0f);
}

void Game::SaveActiveGame() {
    if (isRestoringSave) return;
    int slot = SaveManager::GetActiveSlot();
    if (slot < 1 || slot > 3) slot = 1;

    GameSaveData data;
    data.slotIndex = slot;
    data.isValid = true;
    data.gameState = (int)currentState;
    data.realmName = SaveManager::GetRealmNameFromState((int)currentState);
    data.playerX = player.GetPosition().x;
    data.playerY = player.GetPosition().y;
    data.playerHealth = player.GetHealth();
    data.playerMaxHealth = player.GetMaxHealth();
    data.playerComposure = player.GetComposure();
    data.potionCount = player.GetInventory().GetHealthPotionCount();
    data.shingles = player.GetInventory().GetShingles();
    data.oreScraps = player.GetInventory().GetOreScraps();
    data.equippedWeapon = (int)player.GetWeapon();
    data.equippedArmor = (int)player.GetArmor();
    data.equippedTotem = (int)player.GetTotem();

    for (int i = 0; i < 5; ++i) {
        data.weaponUpgrades[i] = player.GetInventory().GetWeaponUpgrade((WeaponType)i);
        data.unlockedWeapons[i] = player.GetInventory().HasWeapon((WeaponType)i);
    }

    const auto& frags = player.GetInventory().GetFragments();
    for (size_t i = 0; i < frags.size() && i < 10; ++i) {
        data.unlockedFragments[i] = frags[i].discovered;
    }

    data.bossZilDefeated = ruinedCastleHandler.IsZilDefeated();
    data.knightLenonSparCompleted = zyphdenHandler.HasDefeatedLenon();
    data.zyphdenQuestPhase = zyphdenHandler.GetQuestPhase();
    data.arenaHighestWave = waveManager.GetCurrentWave();

    SaveManager::SaveGame(slot, data);
}

void Game::LoadActiveGame(int slotIndex) {
    if (slotIndex < 1 || slotIndex > 3) return;

    GameSaveData data;
    if (!SaveManager::LoadGame(slotIndex, data)) {
        return;
    }

    SaveManager::SetActiveSlot(slotIndex);

    isRestoringSave = true;

    // Restore inventory state BEFORE realm transition so Handler::Init can inspect player inventory correctly
    player.GetInventory().SetHealthPotionCount(data.potionCount);
    player.GetInventory().SetShingles(data.shingles);
    player.GetInventory().SetOreScraps(data.oreScraps);

    for (int i = 0; i < 5; ++i) {
        player.GetInventory().SetWeaponUpgrade((WeaponType)i, data.weaponUpgrades[i]);
        if (data.unlockedWeapons[i]) {
            player.GetInventory().UnlockWeapon((WeaponType)i);
        }
    }

    const auto& frags = player.GetInventory().GetFragments();
    for (size_t i = 0; i < frags.size() && i < 10; ++i) {
        if (data.unlockedFragments[i]) {
            player.GetInventory().UnlockFragment(frags[i].id);
        }
    }

    player.SetWeapon((WeaponType)data.equippedWeapon);
    player.SetArmor((ArmorType)data.equippedArmor);
    player.SetTotem((TotemType)data.equippedTotem);

    // Transition to saved realm/gameState
    GameState targetState = (GameState)data.gameState;
    if (targetState == GameState::CROSSROADS) {
        EnterCrossroads();
    } else if (targetState == GameState::ZYPHDEN_REALM) {
        EnterZyphden();
    } else if (targetState == GameState::QUARRY_DEPTHS) {
        EnterQuarryDepths();
    } else if (targetState == GameState::LOBBY) {
        EnterLobby();
    } else if (targetState == GameState::ARENA) {
        EnterArena();
    } else {
        EnterRuinedCastle();
        if (data.bossZilDefeated || data.equippedWeapon != 0) {
            tutorial.SkipToBoss(map);
        }
    }

    // Restore boss and quest flags
    ruinedCastleHandler.SetZilDefeated(data.bossZilDefeated);
    zyphdenHandler.SetQuestPhase(data.zyphdenQuestPhase);
    if (data.knightLenonSparCompleted) {
        zyphdenHandler.SetLenonDefeated(true);
    }

    // Restore player position and stats
    player.SetWeapon((WeaponType)data.equippedWeapon);
    player.SetArmor((ArmorType)data.equippedArmor);
    player.SetTotem((TotemType)data.equippedTotem);
    player.SetPosition({ data.playerX, data.playerY });
    player.SetHealth(data.playerHealth);
    player.SetComposure(data.playerComposure);
    camera.target = player.GetPosition();

    showControlsModal = false;
    showInventoryModal = false;
    showSlotModal = false;
    deletingSlotIndex = 0;

    isRestoringSave = false;
}

void Game::Update(float dt) {
    particles.Update(dt);
    camera.offset = { (float)GetScreenWidth() * 0.5f, (float)GetScreenHeight() * 0.5f };
    Vector2 mouseScreen = GetMousePosition();

    // Fullscreen Shortcuts: F11 or Alt+Enter
    if (IsKeyPressed(KEY_F11) || ((IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) && IsKeyPressed(KEY_ENTER))) {
        ToggleGameFullscreen();
    }

    // Unlock arena if current storyline progress was completed (player possesses Crusader Plate)
    if (!testArenaUnlocked && player.GetInventory().HasArmor(ArmorType::CRUSADER_PLATE)) {
        testArenaUnlocked = true;
        SaveArenaUnlock();
    }

    // Global Shortcut: '~' (KEY_GRAVE) returns immediately to Title Screen from any map/state
    if (IsKeyPressed(KEY_GRAVE)) {
        if (currentState != GameState::TITLE_SCREEN && currentState != GameState::PROLOGUE_CUTSCENE) {
            SaveActiveGame();
        }
        currentState = GameState::TITLE_SCREEN;
        showControlsModal = false;
        showInventoryModal = false;
        showSlotModal = false;
        deletingSlotIndex = 0;
        SoundFX::PlayButtonClick();
        return;
    }

    // If Controls Modal is active
    if (showControlsModal) {
        if (IsKeyPressed(KEY_C) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_TAB)) {
            showControlsModal = false;
            SoundFX::PlayButtonClick();
            return;
        }
        bool close = GameUI::DrawControlsModal(GetScreenWidth(), GetScreenHeight(), mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        if (close) {
            showControlsModal = false;
        }
        return;
    }

    // If Inventory & Lore Journal Modal is active
    if (showInventoryModal) {
        if (currentState == GameState::RUINED_CASTLE && tutorial.GetStep() == TutorialStep::INVENTORY_CHECK) {
            tutorial.NotifyInventoryOpened();
        }
        if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ESCAPE)) {
            showInventoryModal = false;
            SoundFX::PlayButtonClick();
            return;
        }
        bool close = GameUI::DrawInventoryModal(GetScreenWidth(), GetScreenHeight(), mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), player, inventoryActiveTab, inventorySelectedFragment);
        if (close) {
            showInventoryModal = false;
        }
        if (inventoryActiveTab == 2) {
            showInventoryModal = false;
            showMapModal = true;
            inventoryActiveTab = 0;
            return;
        }
        return;
    }

    // If Interactive Realm Map Modal is active
    if (showMapModal) {
        if (IsKeyPressed(KEY_M) || IsKeyPressed(KEY_ESCAPE)) {
            showMapModal = false;
            SoundFX::PlayButtonClick();
            return;
        }
        bool close = GameUI::DrawMapModal(GetScreenWidth(), GetScreenHeight(), mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), player, currentState);
        if (close) {
            showMapModal = false;
        }
        return;
    }

    // Toggle Controls Menu modal when closed
    if (IsKeyPressed(KEY_C)) {
        showControlsModal = true;
        showInventoryModal = false;
        showMapModal = false;
        SoundFX::PlayButtonClick();
        return;
    }

    // Toggle Inventory & Lore Journal modal when closed
    if (IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TAB)) {
        showInventoryModal = true;
        showControlsModal = false;
        showMapModal = false;
        inventoryActiveTab = 0; // Direct to Equipment & Character Doll
        if (currentState == GameState::RUINED_CASTLE && tutorial.GetStep() == TutorialStep::INVENTORY_CHECK) {
            tutorial.NotifyInventoryOpened();
        }
        SoundFX::PlayButtonClick();
        return;
    }

    // Toggle Interactive Realm Map modal when closed
    if (IsKeyPressed(KEY_M)) {
        showMapModal = true;
        showInventoryModal = false;
        showControlsModal = false;
        SoundFX::PlayButtonClick();
        return;
    }

    // Check HUD top right button clicks in Ruined Castle, Lobby, Arena, Crossroads, Zyphden
    if (!showControlsModal && !showInventoryModal && !showMapModal) {
        if (currentState == GameState::RUINED_CASTLE || currentState == GameState::LOBBY || currentState == GameState::ARENA || currentState == GameState::CROSSROADS || currentState == GameState::ZYPHDEN_REALM || currentState == GameState::QUARRY_DEPTHS) {
            Rectangle mapBtnRec = { (float)(GetScreenWidth() - 380), 16.0f, 120.0f, 32.0f };
            Rectangle journalBtnRec = { (float)(GetScreenWidth() - 250), 16.0f, 110.0f, 32.0f };
            Rectangle controlsBtnRec = { (float)(GetScreenWidth() - 130), 16.0f, 110.0f, 32.0f };
            if (CheckCollisionPointRec(mouseScreen, mapBtnRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                showMapModal = true;
                showInventoryModal = false;
                showControlsModal = false;
                SoundFX::PlayButtonClick();
                return;
            } else if (CheckCollisionPointRec(mouseScreen, journalBtnRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                showInventoryModal = true;
                showControlsModal = false;
                showMapModal = false;
                inventoryActiveTab = 0;
                if (currentState == GameState::RUINED_CASTLE && tutorial.GetStep() == TutorialStep::INVENTORY_CHECK) {
                    tutorial.NotifyInventoryOpened();
                }
                SoundFX::PlayButtonClick();
                return;
            } else if (CheckCollisionPointRec(mouseScreen, controlsBtnRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                showControlsModal = true;
                showInventoryModal = false;
                showMapModal = false;
                SoundFX::PlayButtonClick();
                return;
            }
        }
    }

    // 1. TITLE SCREEN STATE
    if (currentState == GameState::TITLE_SCREEN) {
        int action = GameUI::DrawTitleScreen(GetScreenWidth(), GetScreenHeight(), mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), testArenaUnlocked, showSlotModal, deletingSlotIndex);
        if (action == 1) {
            showControlsModal = true;
        } else if (action == 2) {
            if (testArenaUnlocked) {
                EnterArena();
            }
        } else if (action >= 11 && action <= 13) {
            int slot = action - 10;
            LoadActiveGame(slot);
        } else if (action >= 21 && action <= 23) {
            int slot = action - 20;
            SaveManager::SetActiveSlot(slot);
            player.GetInventory().InitDefault();
            player.SetWeapon(WeaponType::NONE);
            player.SetArmor(ArmorType::NONE);
            player.SetTotem(TotemType::NONE);
            player.GetInventory().ClearWeapons();
            player.GetInventory().ClearArmors();
            player.GetInventory().ClearTotems();
            cutscene.Reset();
            currentState = GameState::PROLOGUE_CUTSCENE;
            showSlotModal = false;
            SaveActiveGame();
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

    // Hit-stop micro-pause: freeze entity simulations during combat impact pause (parry deflection / heavy cleave)
    if (particles.IsHitStopped() && currentState != GameState::TITLE_SCREEN && currentState != GameState::PROLOGUE_CUTSCENE) {
        Vector2 shake = particles.GetScreenShakeOffset();
        Vector2 desiredTarget = { player.GetPosition().x + shake.x, player.GetPosition().y + shake.y };
        camera.target = Vector2Lerp(camera.target, desiredTarget, 10.0f * dt);
        return;
    }

    // 1.8 RUINED CASTLE CHAPTER 1 STATE
    if (currentState == GameState::RUINED_CASTLE) {
        ruinedCastleHandler.Update(
            dt, player, map, particles, enemies, projectiles, lootDrops, tutorial, camera, currentState, testArenaUnlocked,
            [this]() { SaveArenaUnlock(); },
            [this]() { EnterLobby(); },
            [this]() { EnterRuinedCastle(); }
        );
        return;
    }

    // 1.85 CROSSROADS STATE (Path to Zyphden)
    if (currentState == GameState::CROSSROADS) {
        if (map.GetEnvironment() != MapEnvironment::CROSSROADS) {
            EnterCrossroads();
        }
        crossroadsHandler.Update(
            dt, player, map, particles, enemies, projectiles, lootDrops, camera, currentState,
            [this]() { EnterLobby(); }
        );
        return;
    }

    // 1.86 ZYPHDEN TRANSITION PLACEHOLDER SCREEN
    if (currentState == GameState::ZYPHDEN_TRANSITION) {
        int w = GetScreenWidth();
        int h = GetScreenHeight();
        float cardW = fminf((float)w - 80.0f, 1140.0f);
        float cardH = fminf((float)h - 80.0f, 760.0f);
        float cardX = ((float)w - cardW) * 0.5f;
        float cardY = ((float)h - cardH) * 0.5f;

        float btnW = 380.0f;
        float btnH = 50.0f;
        float btnX = cardX + (cardW - btnW) * 0.5f;
        float btnY = cardY + cardH - 68.0f;

        Rectangle btnRec = { btnX, btnY, btnW, btnH };
        bool isHover = CheckCollisionPointRec(mouseScreen, btnRec);
        bool proceed = (isHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ||
                       IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_E);
        if (proceed) {
            SoundFX::PlayButtonClick();
            EnterZyphden();
            currentState = GameState::ZYPHDEN_REALM;
        }
        return;
    }

    // 1.87 QUARRY TRANSITION SCREEN
    if (currentState == GameState::QUARRY_TRANSITION) {
        int w = GetScreenWidth();
        int h = GetScreenHeight();
        float cardW = fminf((float)w - 80.0f, 1140.0f);
        float cardH = fminf((float)h - 80.0f, 760.0f);
        float cardX = ((float)w - cardW) * 0.5f;
        float cardY = ((float)h - cardH) * 0.5f;

        float btnW = 380.0f;
        float btnH = 50.0f;
        float btnX = cardX + (cardW - btnW) * 0.5f;
        float btnY = cardY + cardH - 68.0f;

        Rectangle btnRec = { btnX, btnY, btnW, btnH };
        bool isHover = CheckCollisionPointRec(mouseScreen, btnRec);
        bool proceed = (isHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ||
                       IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_E);
        if (proceed) {
            SoundFX::PlayButtonClick();
            EnterQuarryDepths();
        }
        return;
    }

    // 1.88 ZYPHDEN REALM STATE (Chapter 1)
    if (currentState == GameState::ZYPHDEN_REALM) {
        if (map.GetEnvironment() != MapEnvironment::ZYPHDEN_REALM &&
            map.GetEnvironment() != MapEnvironment::TAVERN_INTERIOR &&
            map.GetEnvironment() != MapEnvironment::FORGE_INTERIOR &&
            map.GetEnvironment() != MapEnvironment::ROYAL_KEEP) {
            EnterZyphden();
        }
        zyphdenHandler.Update(
            dt, player, map, particles, enemies, projectiles, lootDrops, camera, currentState, testArenaUnlocked,
            [this]() { SaveArenaUnlock(); },
            [this]() { EnterLobby(); }
        );
        return;
    }

    // 1.89 QUARRY DEPTHS & GOLEM MINES STATE
    if (currentState == GameState::QUARRY_DEPTHS) {
        if (map.GetEnvironment() != MapEnvironment::QUARRY_DEPTHS) {
            map.Init(MapEnvironment::QUARRY_DEPTHS);
        }
        quarryHandler.Update(
            dt, player, map, particles, enemies, projectiles, lootDrops, camera, currentState,
            [this]() {
                map.Init(MapEnvironment::ZYPHDEN_REALM);
                player.SetPosition({ 1620.0f, 700.0f });
                camera.target = player.GetPosition();
                currentState = GameState::ZYPHDEN_REALM;
                enemies.clear();
                projectiles.clear();
                particles.Clear();
            },
            [this]() { EnterLobby(); }
        );
        return;
    }

    // 1.9 BORDERLANDS GATEWAY EPILOGUE STATE
    if (currentState == GameState::BORDERLANDS_GATE) {
        int choice = GameUI::DrawBorderlandsGateway(GetScreenWidth(), GetScreenHeight(), mouseScreen, IsMouseButtonPressed(MOUSE_BUTTON_LEFT), player);
        if (choice == 1 || IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) { // Arena Test
            if (testArenaUnlocked) {
                EnterArena();
                return;
            } else {
                particles.SpawnFloatingText(player.GetPosition(), "ARENA LOCKED — COMPLETE CHAPTER 1 FIRST!", Color{ 255, 90, 90, 255 }, 20.0f);
                SoundFX::PlayHitImpact();
            }
        } else if (choice == 2 || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_H) || IsKeyPressed(KEY_L) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) { // Back to Hub
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
            player.TriggerHeavyAttack(particles, mouseWorld);
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
                if (player.GetCurrentAttack().type == AttackType::HEAVY) {
                    particles.TriggerHitStop(0.05f);
                    float rad = player.GetCurrentAttack().angle * DEG2RAD;
                    particles.AddDirectionalShake({ cosf(rad), sinf(rad) }, 7.0f, 0.20f);
                } else {
                    particles.AddScreenShake(2.5f, 0.1f);
                }
            }
        }

        for (const auto& ped : pedestals) {
            bool near = ped.CheckInteraction(player.GetPosition());
            bool clicked = CheckCollisionPointCircle(mouseWorld, ped.position, ped.radius) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if ((near && IsKeyPressed(KEY_E)) || clicked) {
                if (ped.weapon == WeaponType::TRUTH_SEEKER && !player.GetInventory().HasWeapon(WeaponType::TRUTH_SEEKER)) {
                    particles.SpawnFloatingText(player.GetPosition(), "LOCKED - DEFEAT ZIL'S FRAGMENT IN CHAPTER 1!", Color{ 255, 90, 90, 255 }, 20.0f);
                    SoundFX::PlayHitImpact();
                } else if (player.GetWeapon() != ped.weapon) {
                    player.SetWeapon(ped.weapon);
                    WeaponStats st = GetWeaponStats(ped.weapon);
                    particles.SpawnFloatingText(player.GetPosition(), "EQUIPPED: " + st.name, Color{ 255, 220, 80, 255 }, 22.0f);
                    particles.SpawnSparks(ped.position, Color{ 100, 220, 255, 255 }, 16, 120.0f);
                }
            }
        }

        if (portal.CheckPlayerInside(player.GetPosition()) || (Vector2Distance(player.GetPosition(), portal.position) < 80.0f && IsKeyPressed(KEY_E))) {
            if (!testArenaUnlocked) {
                particles.SpawnFloatingText(player.GetPosition(), "ARENA LOCKED — COMPLETE CHAPTER 1 FIRST!", Color{ 255, 90, 90, 255 }, 20.0f);
                SoundFX::PlayHitImpact();
            } else {
                EnterArena();
                return;
            }
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            currentState = GameState::TITLE_SCREEN;
            return;
        }

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
        player.TriggerHeavyAttack(particles, mouseWorld);
    }
    if (IsKeyPressed(KEY_Q)) {
        player.TriggerDash(particles);
    }
    if (IsKeyPressed(KEY_F)) {
        player.TriggerParry(particles);
    }

    player.Update(dt, map, mouseWorld, particles);

    // Flush player spawned projectiles (e.g. Truth Seeker Holy Beam)
    for (const auto& p : player.GetSpawnedProjectiles()) {
        projectiles.push_back(p);
    }
    player.GetSpawnedProjectiles().clear();

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

    bool victory = false;
    if (waveManager.Update(dt, aliveCount, enemies, projectiles, particles, { 1200.0f, 900.0f }, victory)) {
        if (victory) {
            currentState = GameState::VICTORY;
            particles.SpawnFloatingText(player.GetPosition(), "ARENA CLEARED!", Color{ 255, 230, 80, 255 }, 30.0f);
        }
    }

    std::vector<bool> enemyAliveBefore(enemies.size());
    for (size_t i = 0; i < enemies.size(); i++) {
        enemyAliveBefore[i] = enemies[i].IsAlive();
    }

    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera, &lootDrops);

    // Update active loot drops in arena
    for (auto& drop : lootDrops) {
        drop.Update(dt, player.GetPosition(), player, particles);
    }
    lootDrops.erase(std::remove_if(lootDrops.begin(), lootDrops.end(), [](const LootDrop& d) {
        return d.IsCollected();
    }), lootDrops.end());

    // Chance to drop adhesive bandage on enemy defeat
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemyAliveBefore[i] && !enemies[i].IsAlive()) {
            if (arenaBandages.size() < 4 && (rand() % 100) < 35) {
                AdhesiveBandage b;
                b.position = enemies[i].GetPosition();
                arenaBandages.push_back(b);
                particles.SpawnSparks(b.position, Color{ 80, 255, 140, 255 }, 14, 110.0f);
            }
        }
    }

    // Periodic Arena Adhesive Bandage Spawning
    bandageSpawnTimer -= dt;
    if (bandageSpawnTimer <= 0.0f) {
        bandageSpawnTimer = 18.0f;
        if (arenaBandages.size() < 4) {
            float rx = 900.0f + (float)(rand() % 600);
            float ry = 650.0f + (float)(rand() % 500);
            AdhesiveBandage b;
            b.position = { rx, ry };
            arenaBandages.push_back(b);
            particles.SpawnSparks(b.position, Color{ 100, 255, 160, 255 }, 16, 120.0f);
            particles.SpawnFloatingText(b.position, "+ BANDAGE SPAWNED +", Color{ 120, 255, 170, 240 }, 18.0f);
        }
    }

    // Update & Pickup Adhesive Bandages
    for (auto& b : arenaBandages) {
        if (!b.active) continue;
        b.Update(dt);
        if (b.CheckPickup(player.GetPosition())) {
            player.Heal(b.healAmount);
            particles.SpawnFloatingText(player.GetPosition(), "+30 HP [BANDAGE]", Color{ 80, 255, 140, 255 }, 24.0f);
            particles.SpawnSparks(b.position, Color{ 100, 255, 160, 255 }, 20, 150.0f);
            SoundFX::PlayParrySuccess();
            b.active = false;
        }
    }
    arenaBandages.erase(std::remove_if(arenaBandages.begin(), arenaBandages.end(), [](const AdhesiveBandage& b) {
        return !b.active;
    }), arenaBandages.end());

    Vector2 shake = particles.GetScreenShakeOffset();
    Vector2 desiredTarget = { player.GetPosition().x + shake.x, player.GetPosition().y + shake.y };
    camera.target = Vector2Lerp(camera.target, desiredTarget, 8.0f * dt);
}

void Game::RenderScene() {
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    float time = (float)GetTime();

    if (currentState == GameState::TITLE_SCREEN) {
        Vector2 mouseScreen = GetMousePosition();
        GameUI::DrawTitleScreen(w, h, mouseScreen, false, testArenaUnlocked, showSlotModal, deletingSlotIndex);

        if (showControlsModal) {
            GameUI::DrawControlsModal(w, h, mouseScreen, false);
        }
        return;
    }

    if (currentState == GameState::PROLOGUE_CUTSCENE) {
        cutscene.Draw(w, h);
        return;
    }

    if (currentState == GameState::BORDERLANDS_GATE) {
        ruinedCastleHandler.Render(
            camera, map, enemies, projectiles, lootDrops, player, particles, tutorial,
            w, h, false, false, 0, 0
        );
        Vector2 mousePos = GetMousePosition();
        GameUI::DrawBorderlandsGateway(w, h, mousePos, false, player);
        return;
    }

    if (currentState == GameState::RUINED_CASTLE) {
        ruinedCastleHandler.Render(
            camera, map, enemies, projectiles, lootDrops, player, particles, tutorial,
            w, h, showControlsModal, showInventoryModal, inventoryActiveTab, inventorySelectedFragment
        );
        if (showMapModal) {
            Vector2 mousePos = GetMousePosition();
            GameUI::DrawMapModal(w, h, mousePos, false, player, currentState);
        }
        return;
    }

    if (currentState == GameState::CROSSROADS) {
        crossroadsHandler.Render(
            camera, map, enemies, projectiles, lootDrops, player, particles,
            w, h, showControlsModal, showInventoryModal, inventoryActiveTab, inventorySelectedFragment
        );
        if (showMapModal) {
            Vector2 mousePos = GetMousePosition();
            GameUI::DrawMapModal(w, h, mousePos, false, player, currentState);
        }
        return;
    }

    if (currentState == GameState::ZYPHDEN_TRANSITION) {
        GameUI::DrawZyphdenTransition(
            w, h, GetMousePosition(), false, GetFrameTime()
        );
        return;
    }

    if (currentState == GameState::QUARRY_TRANSITION) {
        GameUI::DrawQuarryTransition(
            w, h, GetMousePosition(), false, GetFrameTime()
        );
        return;
    }

    if (currentState == GameState::ZYPHDEN_REALM) {
        zyphdenHandler.Render(
            camera, map, enemies, projectiles, lootDrops, player, particles,
            w, h, showControlsModal, showInventoryModal, inventoryActiveTab, inventorySelectedFragment
        );
        if (showMapModal) {
            Vector2 mousePos = GetMousePosition();
            GameUI::DrawMapModal(w, h, mousePos, false, player, currentState);
        }
        return;
    }

    if (currentState == GameState::QUARRY_DEPTHS) {
        quarryHandler.Render(
            camera, map, enemies, projectiles, lootDrops, player, particles,
            w, h, showControlsModal, showInventoryModal, inventoryActiveTab, inventorySelectedFragment
        );
        if (showMapModal) {
            Vector2 mousePos = GetMousePosition();
            GameUI::DrawMapModal(w, h, mousePos, false, player, currentState);
        }
        return;
    }

    if (currentState == GameState::LOBBY) {
        BeginMode2D(camera);
        map.DrawFloor();

        portal.Draw(time, testArenaUnlocked, player.GetPosition());

        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        for (const auto& ped : pedestals) {
            bool isSel = (player.GetWeapon() == ped.weapon);
            bool isHov = CheckCollisionPointCircle(mouseWorld, ped.position, ped.radius) || ped.CheckInteraction(player.GetPosition());
            AssetManager::DrawWeaponPedestal(ped.position, ped.weapon, isSel, isHov, time);
        }

        dummy.Draw(player.GetPosition());
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
        GameUI::DrawLobbyHUD(w, h, player, nearAny, nearName, hasUnread);

        if (showControlsModal) {
            GameUI::DrawControlsModal(w, h, mousePos, false);
        } else if (showInventoryModal) {
            GameUI::DrawInventoryModal(w, h, mousePos, false, player, inventoryActiveTab, inventorySelectedFragment);
        } else if (showMapModal) {
            GameUI::DrawMapModal(w, h, mousePos, false, player, currentState);
        }
        return;
    }

    // ARENA RENDERING
    BeginMode2D(camera);
    map.DrawFloor();

    for (const auto& enemy : enemies) {
        enemy.DrawTelegraph();
    }

    map.DrawWallsAndPillars();

    for (const auto& b : arenaBandages) {
        b.Draw(player.GetPosition());
    }

    for (const auto& drop : lootDrops) {
        drop.Draw(time);
    }

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
    GameUI::DrawCombatHUD(w, h, player, waveManager.GetCurrentWave(), waveManager.GetTotalWaves(), hasUnread);

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
    } else if (showMapModal) {
        GameUI::DrawMapModal(w, h, mousePos, false, player, currentState);
    }
}

void Game::Render() {
    BeginDrawing();
    ClearBackground(BLACK);
    RenderScene();
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
