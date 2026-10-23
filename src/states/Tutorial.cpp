#include "Tutorial.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/AssetManager.hpp"
#include <cmath>

void AncientChest::Draw(float time) const {
    // Drop shadow
    DrawEllipse((int)position.x, (int)(position.y + 12), radius * 1.15f, radius * 0.55f, Color{ 10, 12, 18, 120 });

    if (!opened) {
        // Closed Chest Box
        DrawRectangle((int)(position.x - 22), (int)(position.y - 14), 44, 28, Color{ 68, 48, 30, 255 });
        DrawRectangleLines((int)(position.x - 22), (int)(position.y - 14), 44, 28, Color{ 40, 28, 18, 255 });

        // Iron bands
        DrawRectangle((int)(position.x - 16), (int)(position.y - 14), 6, 28, Color{ 90, 105, 125, 255 });
        DrawRectangle((int)(position.x + 10), (int)(position.y - 14), 6, 28, Color{ 90, 105, 125, 255 });

        // Golden lock
        float pulse = 0.5f + 0.5f * sinf(time * 4.0f);
        DrawCircle((int)position.x, (int)position.y, 5.0f, Color{ 255, 215, 80, 255 });
        DrawCircleGradient(position, 24.0f * pulse, Color{ 255, 215, 80, 50 }, Color{ 0, 0, 0, 0 });
    } else {
        // Open Chest Box
        DrawRectangle((int)(position.x - 22), (int)(position.y - 8), 44, 22, Color{ 55, 38, 24, 255 });
        DrawRectangleLines((int)(position.x - 22), (int)(position.y - 8), 44, 22, Color{ 40, 28, 18, 255 });

        // Open Lid
        DrawRectangle((int)(position.x - 24), (int)(position.y - 20), 48, 12, Color{ 80, 58, 36, 255 });
        DrawRectangleLines((int)(position.x - 24), (int)(position.y - 20), 48, 12, Color{ 110, 85, 55, 255 });

        // Faint residual glow
        DrawCircleGradient(position, 18.0f, Color{ 100, 220, 255, 35 }, Color{ 0, 0, 0, 0 });
    }
}

TutorialManager::TutorialManager() {
    Init();
}

void TutorialManager::Init() {
    currentStep = TutorialStep::AWAKENING;
    initialPlayerPos = { 800.0f, 1180.0f };
    movementDistance = 0.0f;

    weaponStandPos = { 800.0f, 980.0f };
    hasClaimedWeapon = false;

    lightAttacksLanded = 0;
    heavyAttacksLanded = 0;

    chest.position = { 1100.0f, 840.0f };
    chest.opened = false;
    hasOpenedChest = false;

    sentinelSpawned = false;
    parriesAchieved = 0;
    stepTransitionTimer = 0.0f;

    currentObjectiveText = "AWAKENING: Move through the ruined courtyard using [W, A, S, D].";
    currentControlHint = "[W, A, S, D] / Arrow Keys to Move";
}

void TutorialManager::Update(float dt, Player& player, GameMap& map, ParticleSystem& particles, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles) {
    Vector2 playerPos = player.GetPosition();

    switch (currentStep) {
        case TutorialStep::AWAKENING: {
            currentObjectiveText = "AWAKENING: Move through the ruined courtyard using [W, A, S, D].";
            currentControlHint = "[W, A, S, D] to Move";

            float distFromStart = Vector2Distance(playerPos, initialPlayerPos);
            if (distFromStart > 85.0f) {
                currentStep = TutorialStep::WEAPON_STAND;
                particles.SpawnFloatingText(playerPos, "OBJECTIVE UPDATED!", Color{ 255, 220, 80, 255 }, 22.0f);
                SoundFX::PlayButtonClick();
            }
            break;
        }

        case TutorialStep::WEAPON_STAND: {
            currentObjectiveText = "WEAPON STAND: Approach the stone plinth ahead to claim the Broadsword [E].";
            currentControlHint = "Walk to the glowing blade and press [E] to Claim";

            float distToStand = Vector2Distance(playerPos, weaponStandPos);
            if (distToStand <= 55.0f && IsKeyPressed(KEY_E)) {
                hasClaimedWeapon = true;
                player.SetWeapon(WeaponType::SWORD);
                SoundFX::PlayEquipWeapon();
                particles.SpawnSparks(weaponStandPos, Color{ 255, 215, 80, 255 }, 25, 170.0f);
                particles.SpawnFloatingText(playerPos, "BROADSWORD CLAIMED!", Color{ 255, 220, 80, 255 }, 24.0f);
                currentStep = TutorialStep::COMBAT_BASICS;
            }
            break;
        }

        case TutorialStep::COMBAT_BASICS: {
            currentObjectiveText = "MARTIAL PROWESS: Test your blade: [Left Click] Light Attack, [Right Click] Heavy Cleave.";
            currentControlHint = "[Left Click] Light Attack  |  [Right Click] Heavy Cleave (cooldown)";

            if (player.GetCurrentAttack().type == AttackType::LIGHT && lightAttacksLanded == 0) {
                lightAttacksLanded = 1;
                particles.SpawnFloatingText(playerPos, "LIGHT ATTACK TESTED!", Color{ 100, 220, 255, 255 }, 20.0f);
            }
            if (player.GetCurrentAttack().type == AttackType::HEAVY && heavyAttacksLanded == 0) {
                heavyAttacksLanded = 1;
                particles.SpawnFloatingText(playerPos, "HEAVY CLEAVE TESTED!", Color{ 255, 180, 60, 255 }, 20.0f);
            }

            if (lightAttacksLanded > 0 && heavyAttacksLanded > 0) {
                currentStep = TutorialStep::LORE_DISCOVERY;
                particles.SpawnFloatingText(playerPos, "OBJECTIVE COMPLETED!", Color{ 255, 220, 80, 255 }, 22.0f);
                SoundFX::PlayButtonClick();
            }
            break;
        }

        case TutorialStep::LORE_DISCOVERY: {
            currentObjectiveText = "LOST CHRONICLES: Cross the eastern bridge to inspect the glowing ancient chest [E].";
            currentControlHint = "Press [E] to Open Chest  |  Press [I] to read Lore Journal";

            if (chest.CheckInteraction(playerPos) && IsKeyPressed(KEY_E)) {
                chest.opened = true;
                hasOpenedChest = true;
                player.GetInventory().UnlockFragment("frag_02");
                SoundFX::PlayLoreDiscovery();
                particles.SpawnSparks(chest.position, Color{ 255, 215, 80, 255 }, 28, 180.0f);
                particles.SpawnFloatingText(chest.position, "LORE DISCOVERED: FRAGMENT II!", Color{ 255, 220, 80, 255 }, 24.0f);
                particles.SpawnFloatingText(playerPos, "PRESS [I] TO READ JOURNAL", Color{ 120, 230, 255, 255 }, 22.0f);
                currentStep = TutorialStep::DEFENSE_SENTINEL;
            }
            break;
        }

        case TutorialStep::DEFENSE_SENTINEL: {
            currentObjectiveText = "SANCTUARY SENTINEL: Evade strikes with [Q] Dash & deflect with [F] Parry.";
            currentControlHint = "[Q] Dash (i-frames)  |  [F] Parry to stagger & gain +50% counter bonus";

            if (!sentinelSpawned) {
                sentinelSpawned = true;
                enemies.emplace_back(EnemyType::SLASHER, Vector2{ 800.0f, 620.0f });
                particles.SpawnSparks({ 800.0f, 620.0f }, Color{ 140, 200, 255, 255 }, 22, 160.0f);
                particles.SpawnFloatingText({ 800.0f, 620.0f }, "RUIN SENTINEL AWAKENED", Color{ 140, 200, 255, 255 }, 24.0f);
            }

            if (player.HasCounterBuff()) {
                parriesAchieved++;
            }

            bool sentinelAlive = false;
            for (const auto& en : enemies) {
                if (en.IsAlive()) {
                    sentinelAlive = true;
                    break;
                }
            }

            if (!sentinelAlive || parriesAchieved >= 2) {
                currentStep = TutorialStep::GATE_OPENED;
                map.SetGateOpen(true);
                particles.SpawnFloatingText({ 800.0f, 250.0f }, "PORTCULLIS UNLOCKED!", Color{ 255, 220, 80, 255 }, 28.0f);
                SoundFX::PlayGateOpen();
            }
            break;
        }

        case TutorialStep::GATE_OPENED: {
            currentObjectiveText = "THE HIGH ALTAR: The sanctuary steps are clear! Advance north to the High Altar.";
            currentControlHint = "Climb the steps north [Y <= 420]";

            if (playerPos.y <= 420.0f) {
                currentStep = TutorialStep::COMPLETED;
                particles.SpawnFloatingText(playerPos, "CHAPTER 1: THE INNER SANCTUM", Color{ 255, 215, 80, 255 }, 26.0f);
                SoundFX::PlayButtonClick();
            }
            break;
        }

        case TutorialStep::COMPLETED: {
            currentObjectiveText = "ZIL'S SANCTUARY: Prepare for battle against Zil the Corrupted Crusader.";
            currentControlHint = "Combat ready";
            break;
        }
    }
}

void TutorialManager::SkipToBoss(GameMap& map) {
    hasClaimedWeapon = true;
    chest.opened = true;
    map.SetGateOpen(true);
    currentStep = TutorialStep::COMPLETED;
}

void TutorialManager::DrawWorldElements(float time) const {
    // 1. Weapon Stand Altar at (800, 980)
    DrawCircle((int)weaponStandPos.x, (int)(weaponStandPos.y + 12), 34.0f, Color{ 10, 12, 18, 120 });
    DrawCircle((int)weaponStandPos.x, (int)weaponStandPos.y, 30.0f, Color{ 32, 38, 52, 255 });
    DrawCircleLines((int)weaponStandPos.x, (int)weaponStandPos.y, 30.0f, Color{ 68, 85, 115, 240 });

    if (!hasClaimedWeapon) {
        float floatOffset = sinf(time * 3.5f) * 6.0f - 16.0f;
        Vector2 bladeAnchor = { weaponStandPos.x - 14.0f, weaponStandPos.y + floatOffset + 14.0f };

        // Glowing broadsword preview
        AssetManager::DrawSword(bladeAnchor, -45.0f, 38.0f, Color{ 240, 245, 255, 255 }, Color{ 0, 220, 255, 200 });
        float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
        DrawCircleGradient(Vector2{ weaponStandPos.x, weaponStandPos.y + floatOffset }, 36.0f, Color{ 100, 220, 255, (unsigned char)(70 * pulse) }, Color{ 0, 0, 0, 0 });

        const char* label = "[E] Claim Broadsword";
        int lw = MeasureText(label, 12);
        DrawText(label, (int)(weaponStandPos.x - lw * 0.5f), (int)(weaponStandPos.y + 36), 12, Color{ 255, 220, 90, 255 });
    }

    // 2. Ancient Chest in East Alcove
    chest.Draw(time);

    if (!chest.opened) {
        const char* clabel = "ANCIENT RELIC CHEST";
        int cw = MeasureText(clabel, 11);
        DrawText(clabel, (int)(chest.position.x - cw * 0.5f), (int)(chest.position.y - chest.radius - 16), 11, Color{ 255, 215, 80, 240 });
    }
}

void TutorialManager::DrawHUD(int windowWidth, int windowHeight) const {
    if (currentStep == TutorialStep::COMPLETED) return;

    // Top Cinematic Tutorial Banner
    const float bannerW = 740.0f;
    const float bannerH = 58.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.2f, 6, Color{ 14, 18, 28, 235 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.2f, 6, Color{ 205, 165, 75, 240 });
    DrawRectangleRoundedLines(Rectangle{ bannerX + 4, bannerY + 4, bannerW - 8, bannerH - 8 }, 0.18f, 6, Color{ 45, 58, 85, 160 });

    int stepNum = 1 + (int)currentStep;
    if (stepNum > 6) stepNum = 6;
    std::string chapterHeader = "CHAPTER 1: THE RUINED CASTLE OF ZYPHDEN  |  TUTORIAL STEP " + std::to_string(stepNum) + "/6";
    DrawText(chapterHeader.c_str(), (int)bannerX + 20, (int)bannerY + 10, 11, Color{ 255, 215, 80, 255 });

    DrawText(currentObjectiveText.c_str(), (int)bannerX + 20, (int)bannerY + 26, 13, Color{ 255, 255, 255, 255 });
    DrawText(currentControlHint.c_str(), (int)bannerX + 20, (int)bannerY + 43, 10, Color{ 120, 220, 255, 220 });
}
