#include "Tutorial.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/AssetManager.hpp"
#include <cmath>
#include <sstream>

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

void LoreCollectible::Draw(float time, bool isInteractable) const {
    // Drop shadow
    DrawEllipse((int)position.x, (int)(position.y + 12), radius * 1.15f, radius * 0.55f, Color{ 10, 12, 18, 120 });

    if (type == LoreType::STELE) {
        // Ancient Stone Stele
        Rectangle steleRec = { position.x - 16.0f, position.y - 28.0f, 32.0f, 42.0f };
        DrawRectangleRounded(steleRec, 0.35f, 4, Color{ 36, 42, 54, 255 });
        DrawRectangleRoundedLines(steleRec, 0.35f, 4, collected ? Color{ 65, 78, 100, 200 } : (isInteractable ? Color{ 120, 145, 185, 255 } : Color{ 60, 68, 85, 200 }));

        if (!collected) {
            if (isInteractable) {
                float pulse = 0.5f + 0.5f * sinf(time * 3.5f);
                DrawCircleGradient(position, 34.0f * (0.8f + 0.2f * pulse), Color{ 255, 215, 80, (unsigned char)(60 * pulse) }, Color{ 0, 0, 0, 0 });
                // Inscribed glowing runes
                DrawLineEx({ position.x - 8, position.y - 18 }, { position.x + 8, position.y - 18 }, 2.0f, Color{ 255, 220, 90, 255 });
                DrawLineEx({ position.x, position.y - 18 }, { position.x, position.y + 4 }, 2.0f, Color{ 255, 220, 90, 255 });
                DrawCircle((int)position.x, (int)position.y - 4, 3.0f, Color{ 255, 240, 140, 255 });
            } else {
                // Inactive weathered runes
                DrawLineEx({ position.x - 8, position.y - 18 }, { position.x + 8, position.y - 18 }, 1.5f, Color{ 70, 85, 110, 140 });
                DrawLineEx({ position.x, position.y - 18 }, { position.x, position.y + 4 }, 1.5f, Color{ 70, 85, 110, 140 });
            }
        } else {
            // Weathered inactive runes
            DrawLineEx({ position.x - 8, position.y - 18 }, { position.x + 8, position.y - 18 }, 1.5f, Color{ 70, 85, 110, 180 });
            DrawLineEx({ position.x, position.y - 18 }, { position.x, position.y + 4 }, 1.5f, Color{ 70, 85, 110, 180 });
        }
    } else if (type == LoreType::CHEST) {
        // Ancient Wooden Chest
        if (!collected) {
            DrawRectangle((int)(position.x - 22), (int)(position.y - 14), 44, 28, Color{ 68, 48, 30, 255 });
            DrawRectangleLines((int)(position.x - 22), (int)(position.y - 14), 44, 28, isInteractable ? Color{ 40, 28, 18, 255 } : Color{ 65, 30, 30, 255 });
            DrawRectangle((int)(position.x - 16), (int)(position.y - 14), 6, 28, Color{ 90, 105, 125, 255 });
            DrawRectangle((int)(position.x + 10), (int)(position.y - 14), 6, 28, Color{ 90, 105, 125, 255 });

            if (isInteractable) {
                float pulse = 0.5f + 0.5f * sinf(time * 4.0f);
                DrawCircle((int)position.x, (int)position.y, 5.0f, Color{ 255, 215, 80, 255 });
                DrawCircleGradient(position, 24.0f * pulse, Color{ 255, 215, 80, 50 }, Color{ 0, 0, 0, 0 });
            } else {
                // Red sealed padlock
                DrawRectangle((int)(position.x - 5), (int)(position.y - 5), 10, 10, Color{ 160, 45, 45, 230 });
                DrawRectangleLines((int)(position.x - 5), (int)(position.y - 5), 10, 10, Color{ 230, 80, 80, 255 });
            }
        } else {
            DrawRectangle((int)(position.x - 22), (int)(position.y - 8), 44, 22, Color{ 55, 38, 24, 255 });
            DrawRectangleLines((int)(position.x - 22), (int)(position.y - 8), 44, 22, Color{ 40, 28, 18, 255 });
            DrawRectangle((int)(position.x - 24), (int)(position.y - 20), 48, 12, Color{ 80, 58, 36, 255 });
            DrawRectangleLines((int)(position.x - 24), (int)(position.y - 20), 48, 12, Color{ 110, 85, 55, 255 });
            DrawCircleGradient(position, 18.0f, Color{ 100, 220, 255, 35 }, Color{ 0, 0, 0, 0 });
        }
    } else if (type == LoreType::SHIELD) {
        // Sun-Crusader Inscribed Shield & Sword Relic
        if (!collected) {
            if (isInteractable) {
                float pulse = 0.5f + 0.5f * sinf(time * 3.2f);
                DrawCircleGradient(position, 34.0f * (0.8f + 0.2f * pulse), Color{ 255, 170, 50, (unsigned char)(65 * pulse) }, Color{ 0, 0, 0, 0 });
            }

            // Crossed broadsword blade
            DrawLineEx({ position.x - 14, position.y + 10 }, { position.x + 14, position.y - 18 }, 3.0f, isInteractable ? Color{ 180, 195, 215, 255 } : Color{ 110, 120, 135, 220 });
            // Heater shield
            DrawCircleV({ position.x, position.y - 2 }, 15.0f, isInteractable ? Color{ 160, 120, 40, 255 } : Color{ 85, 70, 45, 255 });
            DrawCircleLines((int)position.x, (int)position.y - 2, 15.0f, isInteractable ? Color{ 255, 215, 80, 255 } : Color{ 130, 115, 75, 200 });
            DrawCircleV({ position.x, position.y - 2 }, 8.0f, Color{ 220, 160, 30, 255 });
            // Golden cross
            DrawLineEx({ position.x - 6, position.y - 2 }, { position.x + 6, position.y - 2 }, 2.0f, Color{ 255, 240, 160, 255 });
            DrawLineEx({ position.x, position.y - 8 }, { position.x, position.y + 4 }, 2.0f, Color{ 255, 240, 160, 255 });
        } else {
            // Weathered resting shield
            DrawLineEx({ position.x - 14, position.y + 10 }, { position.x + 14, position.y - 18 }, 2.5f, Color{ 100, 115, 130, 200 });
            DrawCircleV({ position.x, position.y - 2 }, 15.0f, Color{ 55, 62, 75, 255 });
            DrawCircleLines((int)position.x, (int)position.y - 2, 15.0f, Color{ 85, 95, 115, 220 });
            DrawCircleGradient(position, 18.0f, Color{ 150, 120, 60, 30 }, Color{ 0, 0, 0, 0 });
        }
    } else if (type == LoreType::RELIQUARY) {
        // Royal Obsidian & Gold Reliquary
        Rectangle boxRec = { position.x - 18.0f, position.y - 14.0f, 36.0f, 26.0f };
        if (!collected) {
            if (isInteractable) {
                float pulse = 0.5f + 0.5f * sinf(time * 3.8f);
                DrawCircleGradient(position, 38.0f * (0.8f + 0.2f * pulse), Color{ 180, 100, 255, (unsigned char)(60 * pulse) }, Color{ 0, 0, 0, 0 });
            }

            DrawRectangleRounded(boxRec, 0.2f, 4, Color{ 26, 20, 38, 255 });
            DrawRectangleRoundedLines(boxRec, 0.2f, 4, isInteractable ? Color{ 200, 140, 255, 255 } : Color{ 110, 85, 140, 200 });

            // Gold filigree corner accents
            DrawCircle((int)boxRec.x + 4, (int)boxRec.y + 4, 3.0f, Color{ 255, 215, 80, 240 });
            DrawCircle((int)(boxRec.x + boxRec.width - 4), (int)boxRec.y + 4, 3.0f, Color{ 255, 215, 80, 240 });
            // Arcane seal
            DrawCircle((int)position.x, (int)position.y - 1, 6.0f, isInteractable ? Color{ 190, 80, 255, 240 } : Color{ 110, 55, 140, 200 });
            DrawCircleLines((int)position.x, (int)position.y - 1, 8.0f, isInteractable ? Color{ 240, 180, 255, 240 } : Color{ 130, 85, 160, 200 });
        } else {
            DrawRectangleRounded(boxRec, 0.2f, 4, Color{ 30, 26, 42, 255 });
            DrawRectangleRoundedLines(boxRec, 0.2f, 4, Color{ 90, 75, 120, 200 });
            DrawCircleGradient(position, 18.0f, Color{ 140, 80, 220, 30 }, Color{ 0, 0, 0, 0 });
        }
    }

    if (!collected) {
        std::string displayLabel = isInteractable ? label : ("[SEALED] " + label);
        int lw = MeasureText(displayLabel.c_str(), 10);
        DrawText(displayLabel.c_str(), (int)(position.x - lw * 0.5f), (int)(position.y - radius - 16), 10, isInteractable ? Color{ 255, 230, 120, 240 } : Color{ 190, 150, 150, 190 });
    }
}

TutorialManager::TutorialManager() {
    Init();
}

void TutorialManager::TriggerStepPopup(TutorialStep step) {
    showPopup = true;
    switch (step) {
        case TutorialStep::AWAKENING:
            popupTitle = "AWAKENING IN THE RUINS";
            popupBody = "You awaken upon the weathered flagstones of the Ruined Castle of Zyphden.\n\nUse your movement keys to regain your footing and traverse the sanctuary courtyard.";
            popupHint = "[W, A, S, D] or Arrow Keys to Move";
            break;
        case TutorialStep::WEAPON_STAND:
            popupTitle = "THE ANCIENT WEAPON STAND";
            popupBody = "An illuminated stone altar stands ahead in the courtyard center.\n\nApproach the glowing blade and press [E] to claim the Crusader's Broadsword.";
            popupHint = "Walk up and press [E] to Claim Broadsword";
            break;
        case TutorialStep::COMBAT_BASICS:
            popupTitle = "SWORDPLAY & MARTIAL ARTS";
            popupBody = "The broadsword is balanced for both rapid slashes and heavy cleaves.\n\nTest both attack styles now to prepare for hostile fortress sentinels.";
            popupHint = "[Left Click] Light Attack  |  [Right Click] Heavy Cleave";
            break;
        case TutorialStep::INVENTORY_CHECK:
            popupTitle = "EQUIPMENT & ATTRIBUTES";
            popupBody = "Your armaments are equipped and ready.\n\nOpen your Journal & Inventory [I] to inspect your character paper doll, armor defense, and martial stats.";
            popupHint = "Press [I] or click Journal to Open Inventory";
            break;
        case TutorialStep::LORE_DISCOVERY:
            popupTitle = "THE SCATTERED CHRONICLES";
            popupBody = "The history of Tydeshire was shattered into fragments when King Charlz went mad.\n\nCross the eastern stone bridge to inspect the ancient relic chest. All 4 chronicles can be discovered across the castle ruins!";
            popupHint = "[E] Open Relic Chest  |  [I] Open Lore Journal";
            break;
        case TutorialStep::DEFENSE_SENTINEL:
            popupTitle = "DEFENSE TRIAL: DASH & PARRY";
            popupBody = "A Ruin Sentinel has awakened to test your vigilance!\n\n• Dash [Q]: Swift evasion with invulnerability frames.\n• Parry [F]: Timed deflection right before impact staggers foes and grants +50% Counter Damage!\n• Defeat the Sentinel to break the ward sealing the High Altar.";
            popupHint = "[Q] Dash (Evade)  |  [F] Parry (Timed Deflection)  |  Defeat Sentinel";
            break;
        case TutorialStep::GATE_OPENED:
            popupTitle = "THE UPPER SANCTUARY UNSEALED";
            popupBody = "With the Ruin Sentinel vanquished, the ward across the High Altar steps has dissipated.\n\nClimb the carved stone steps north onto the elevated high dais to face the keeper of Truth Seeker.";
            popupHint = "Advance north up the steps [Y <= 420] to confront Zil's Fragment";
            break;
        case TutorialStep::COMPLETED:
            showPopup = false;
            break;
    }
    SoundFX::PlayCutsceneChime();
}

void TutorialManager::Init() {
    currentStep = TutorialStep::AWAKENING;
    initialPlayerPos = { 800.0f, 1180.0f };
    movementDistance = 0.0f;

    weaponStandPos = { 800.0f, 980.0f };
    hasClaimedWeapon = false;

    armorStandPos = { 870.0f, 980.0f };
    hasClaimedArmor = false;

    lightAttacksLanded = 0;
    heavyAttacksLanded = 0;
    hasOpenedInventory = false;

    chest.position = { 1100.0f, 840.0f };
    chest.opened = false;
    hasOpenedChest = false;

    sentinelSpawned = false;
    tutorialEnemyDefeated = false;
    parriesAchieved = 0;
    stepTransitionTimer = 0.0f;

    // 4 Tutorial Lore Collectibles across Ruined Castle of Zyphden
    loreItems.clear();
    loreItems.push_back({
        "frag_01",
        "Fragment I: The Great Unification",
        "CHRONICLE STELE [E]",
        Vector2{ 710.0f, 1160.0f },
        28.0f,
        false,
        LoreType::STELE
    });
    loreItems.push_back({
        "frag_02",
        "Fragment II: The Shadow over Zyphden",
        "ANCIENT RELIC CHEST [E]",
        Vector2{ 1100.0f, 840.0f },
        30.0f,
        false,
        LoreType::CHEST
    });
    loreItems.push_back({
        "frag_03",
        "Fragment III: Zil's Holy Oath",
        "CRUSADER RELIC [E]",
        Vector2{ 380.0f, 740.0f },
        28.0f,
        false,
        LoreType::SHIELD
    });
    loreItems.push_back({
        "frag_04",
        "Fragment IV: The Five Broken Seals",
        "ROYAL RELIQUARY [E]",
        Vector2{ 380.0f, 470.0f },
        28.0f,
        false,
        LoreType::RELIQUARY
    });

    currentObjectiveText = "AWAKENING: Move through the ruined courtyard using [W, A, S, D].";
    currentControlHint = "[W, A, S, D] / Arrow Keys to Move";

    TriggerStepPopup(TutorialStep::AWAKENING);
}

void TutorialManager::Update(float dt, Player& player, GameMap& map, ParticleSystem& particles, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles) {
    (void)dt;
    (void)map;
    (void)projectiles;
    Vector2 playerPos = player.GetPosition();

    // 1. Sync inventory discovery state with loreItems
    for (auto& item : loreItems) {
        const auto* frag = player.GetInventory().GetFragment(item.fragmentId);
        if (frag && frag->discovered) {
            item.collected = true;
            if (item.fragmentId == "frag_02") {
                chest.opened = true;
                hasOpenedChest = true;
            }
        }
    }

    // 2. Interactive Lore Collectibles (press [E] or Left Click nearby)
    for (auto& item : loreItems) {
        if (!item.collected && item.CheckInteraction(playerPos)) {
            if (currentStep < TutorialStep::LORE_DISCOVERY) {
                if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                    particles.SpawnFloatingText(item.position, "SEALED - Complete current objective first!", Color{ 255, 90, 90, 255 }, 18.0f);
                    SoundFX::PlayHitImpact();
                }
            } else {
                if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                    item.collected = true;
                    player.GetInventory().UnlockFragment(item.fragmentId);
                    SoundFX::PlayLoreDiscovery();

                    int discovered = player.GetInventory().GetDiscoveredFragmentCount();
                    int total = player.GetInventory().GetTotalFragmentCount();
                    std::string announce = "LORE DISCOVERED (" + std::to_string(discovered) + "/" + std::to_string(total) + "): " + item.title;
                    particles.SpawnFloatingText(item.position, announce, Color{ 255, 220, 80, 255 }, 20.0f);
                    particles.AddScreenShake(3.0f, 0.15f);

                    if (item.fragmentId == "frag_02") {
                        chest.opened = true;
                        hasOpenedChest = true;
                        if (currentStep == TutorialStep::LORE_DISCOVERY) {
                            currentStep = TutorialStep::DEFENSE_SENTINEL;
                            TriggerStepPopup(TutorialStep::DEFENSE_SENTINEL);
                        }
                    }
                }
            }
        }
    }

    // Persistent Equipment Stand Checks (Armor and Weapon remain interactable until claimed)
    float distToArmor = Vector2Distance(playerPos, armorStandPos);
    if (!hasClaimedArmor && distToArmor <= 55.0f && (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))) {
        hasClaimedArmor = true;
        player.GetInventory().UnlockArmor(ArmorType::COMMONERS_SCRAP);
        player.SetArmor(ArmorType::COMMONERS_SCRAP);
        SoundFX::PlayEquipWeapon();
        particles.SpawnFloatingText(armorStandPos, "EQUIPPED: Commoner's Scrap (+6 DEF)!", Color{ 255, 215, 80, 255 }, 20.0f);
        particles.AddScreenShake(3.0f, 0.15f);
    }

    switch (currentStep) {
        case TutorialStep::AWAKENING: {
            currentObjectiveText = "AWAKENING: Move through the ruined courtyard using [W, A, S, D].";
            currentControlHint = "[W, A, S, D] to Move";

            float distFromStart = Vector2Distance(playerPos, initialPlayerPos);
            if (distFromStart > 85.0f) {
                currentStep = TutorialStep::WEAPON_STAND;
                TriggerStepPopup(TutorialStep::WEAPON_STAND);
            }
            break;
        }

        case TutorialStep::WEAPON_STAND: {
            currentObjectiveText = hasClaimedArmor ?
                "WEAPON: Claim the Crusader's Broadsword from the central plinth [E]." :
                "WEAPON & ARMOR: Claim the Broadsword and Commoner's Scrap armor from the plinths [E].";
            currentControlHint = "Walk to glowing plinths and press [E] to Claim Equipment";

            float distToStand = Vector2Distance(playerPos, weaponStandPos);
            if (distToStand <= 55.0f && (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))) {
                hasClaimedWeapon = true;
                player.SetWeapon(WeaponType::SWORD);
                player.GetInventory().UnlockWeapon(WeaponType::SWORD);
                SoundFX::PlayEquipWeapon();
                particles.SpawnFloatingText(weaponStandPos, "CLAIMED: Crusader's Broadsword!", Color{ 255, 215, 80, 255 }, 20.0f);
                particles.AddScreenShake(3.0f, 0.15f);
                currentStep = TutorialStep::COMBAT_BASICS;
                TriggerStepPopup(TutorialStep::COMBAT_BASICS);
            }
            break;
        }

        case TutorialStep::COMBAT_BASICS: {
            currentObjectiveText = "MARTIAL PROWESS: Test your blade: [Left Click] Light Attack, [Right Click] Heavy Cleave.";
            currentControlHint = "[Left Click] Light Attack  |  [Right Click] Heavy Cleave (cooldown)";

            if (player.GetCurrentAttack().type == AttackType::LIGHT && lightAttacksLanded == 0) {
                lightAttacksLanded = 1;
            }
            if (player.GetCurrentAttack().type == AttackType::HEAVY && heavyAttacksLanded == 0) {
                heavyAttacksLanded = 1;
            }

            if (lightAttacksLanded > 0 && heavyAttacksLanded > 0) {
                currentStep = TutorialStep::INVENTORY_CHECK;
                TriggerStepPopup(TutorialStep::INVENTORY_CHECK);
            }
            break;
        }

        case TutorialStep::INVENTORY_CHECK: {
            currentObjectiveText = "EQUIPMENT & STATS: Open Journal / Inventory [I] to inspect equipment and attributes.";
            currentControlHint = "Press [I] or click Journal to inspect Character Paper Doll & Stats";

            if (hasOpenedInventory || IsKeyPressed(KEY_I) || IsKeyPressed(KEY_TAB)) {
                hasOpenedInventory = true;
                currentStep = TutorialStep::LORE_DISCOVERY;
                TriggerStepPopup(TutorialStep::LORE_DISCOVERY);
            }
            break;
        }

        case TutorialStep::LORE_DISCOVERY: {
            currentObjectiveText = "LOST CHRONICLES: Cross the eastern bridge to inspect the glowing ancient chest [E].";
            currentControlHint = "Press [E] to Open Chest  |  Press [I] to read Lore Journal";

            if (chest.CheckInteraction(playerPos) && (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))) {
                chest.opened = true;
                hasOpenedChest = true;
                player.GetInventory().UnlockFragment("frag_02");
                SoundFX::PlayLoreDiscovery();
                for (auto& item : loreItems) {
                    if (item.fragmentId == "frag_02") item.collected = true;
                }
                currentStep = TutorialStep::DEFENSE_SENTINEL;
                TriggerStepPopup(TutorialStep::DEFENSE_SENTINEL);
            }
            break;
        }

        case TutorialStep::DEFENSE_SENTINEL: {
            currentObjectiveText = "SANCTUARY SENTINEL: Defeat the corrupt Slasher! Evade with [Q] Dash & deflect with [F] Parry.";
            currentControlHint = "[Q] Dash (i-frames)  |  [F] Parry to stagger  |  Defeat the Slasher";

            if (!sentinelSpawned) {
                sentinelSpawned = true;
                enemies.emplace_back(EnemyType::SLASHER, Vector2{ 800.0f, 620.0f });
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

            // Must officially beat (kill) the sentinel to proceed
            if (sentinelSpawned && !sentinelAlive) {
                tutorialEnemyDefeated = true;
                currentStep = TutorialStep::GATE_OPENED;
                TriggerStepPopup(TutorialStep::GATE_OPENED);
                SoundFX::PlayGateOpen();
                particles.SpawnFloatingText(playerPos, "SENTINEL DEFEATED — ALTAR STEPS CLEARED!", Color{ 120, 240, 180, 255 }, 22.0f);
            }
            break;
        }

        case TutorialStep::GATE_OPENED: {
            currentObjectiveText = "THE HIGH ALTAR: The sanctuary steps are clear! Advance north to the High Altar.";
            currentControlHint = "Climb the steps north [Y <= 420] to confront Zil's Fragment";

            if (playerPos.y <= 420.0f) {
                currentStep = TutorialStep::COMPLETED;
                SoundFX::PlayButtonClick();
            }
            break;
        }

        case TutorialStep::COMPLETED: {
            currentObjectiveText = "ZIL'S SANCTUARY: Prepare for battle against Zil's Fragment [1% True Power].";
            currentControlHint = "Combat ready";
            break;
        }
    }
}

void TutorialManager::SkipToBoss(GameMap& map) {
    (void)map;
    hasClaimedWeapon = true;
    chest.opened = true;
    tutorialEnemyDefeated = true;
    for (auto& item : loreItems) {
        item.collected = true;
    }
    currentStep = TutorialStep::COMPLETED;
}

void TutorialManager::DrawWorldElements(float time) const {
    // 1a. Weapon Stand Altar at (800, 980)
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
        int lw = MeasureText(label, 11);
        DrawText(label, (int)(weaponStandPos.x - lw * 0.5f), (int)(weaponStandPos.y + 36), 11, Color{ 255, 220, 90, 255 });
    }

    // 1b. Commoner's Scrap Armor Stand at (870, 980)
    DrawCircle((int)armorStandPos.x, (int)(armorStandPos.y + 12), 34.0f, Color{ 10, 12, 18, 120 });
    DrawCircle((int)armorStandPos.x, (int)armorStandPos.y, 30.0f, Color{ 32, 38, 52, 255 });
    DrawCircleLines((int)armorStandPos.x, (int)armorStandPos.y, 30.0f, Color{ 68, 85, 115, 240 });

    if (!hasClaimedArmor) {
        float floatOffset = sinf(time * 3.2f + 1.2f) * 5.0f - 16.0f;
        Vector2 armorCenter = { armorStandPos.x, armorStandPos.y + floatOffset };

        // Armor cuirass icon preview
        DrawRectangleRounded(Rectangle{ armorCenter.x - 13.0f, armorCenter.y - 12.0f, 26.0f, 24.0f }, 0.25f, 4, Color{ 130, 90, 60, 255 });
        DrawRectangleRoundedLines(Rectangle{ armorCenter.x - 13.0f, armorCenter.y - 12.0f, 26.0f, 24.0f }, 0.25f, 4, Color{ 180, 195, 215, 255 });
        DrawCircleV({ armorCenter.x - 11.0f, armorCenter.y - 8.0f }, 4.5f, Color{ 85, 65, 48, 255 });
        DrawCircleV({ armorCenter.x + 11.0f, armorCenter.y - 8.0f }, 4.5f, Color{ 85, 65, 48, 255 });
        DrawLineEx({ armorCenter.x, armorCenter.y - 10.0f }, { armorCenter.x, armorCenter.y + 10.0f }, 2.0f, Color{ 220, 200, 140, 255 });

        float pulse = 0.5f + 0.5f * sinf(time * 3.2f);
        DrawCircleGradient(armorCenter, 34.0f, Color{ 255, 200, 100, (unsigned char)(60 * pulse) }, Color{ 0, 0, 0, 0 });

        const char* label = "[E] Commoner's Scrap";
        int lw = MeasureText(label, 11);
        DrawText(label, (int)(armorStandPos.x - lw * 0.5f), (int)(armorStandPos.y + 36), 11, Color{ 255, 220, 90, 255 });
    }

    // 2. All 4 Tutorial Lore Collectibles
    bool loreInteractable = (currentStep >= TutorialStep::LORE_DISCOVERY);
    for (const auto& item : loreItems) {
        item.Draw(time, loreInteractable);
    }
}

void TutorialManager::DrawHUD(int windowWidth, int windowHeight) const {
    if (currentStep == TutorialStep::COMPLETED) return;

    // Toggle shortcut: [O] Key
    if (IsKeyPressed(KEY_O)) {
        showObjectives = !showObjectives;
    }

    float bannerX = 24.0f;
    float bannerY = (float)windowHeight * 0.38f; // Middle-left of the screen

    Vector2 mousePos = GetMousePosition();
    bool isPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (!showObjectives) {
        // Collapsed Sleek Bookmark Tab
        const float tabW = 160.0f;
        const float tabH = 34.0f;
        Rectangle tabRec = { bannerX, bannerY, tabW, tabH };
        bool isHovered = CheckCollisionPointRec(mousePos, tabRec);

        if (isHovered && isPressed) {
            showObjectives = true;
            SoundFX::PlayButtonClick();
        }

        DrawRectangleRounded(tabRec, 0.25f, 4, isHovered ? Color{ 28, 38, 56, 235 } : Color{ 16, 22, 34, 215 });
        DrawRectangleRoundedLines(tabRec, 0.25f, 4, isHovered ? Color{ 255, 215, 80, 240 } : Color{ 65, 85, 120, 200 });
        DrawCircle((int)bannerX + 16, (int)bannerY + 17, 4.0f, Color{ 255, 215, 80, 255 });
        DrawText("[O] OBJECTIVES", (int)bannerX + 28, (int)bannerY + 11, 11, isHovered ? Color{ 255, 255, 255, 255 } : Color{ 200, 220, 245, 220 });
        return;
    }

    // Expanded Middle-Left Objectives Card
    const float bannerW = 460.0f;
    const float bannerH = 76.0f;
    Rectangle cardRec = { bannerX, bannerY, bannerW, bannerH };

    DrawRectangleRounded(cardRec, 0.15f, 6, Color{ 14, 18, 28, 235 });
    DrawRectangleRoundedLines(cardRec, 0.15f, 6, Color{ 205, 165, 75, 240 });
    DrawRectangleRoundedLines(Rectangle{ bannerX + 3, bannerY + 3, bannerW - 6, bannerH - 6 }, 0.12f, 6, Color{ 45, 58, 85, 160 });

    int stepNum = 1 + (int)currentStep;
    if (stepNum > 6) stepNum = 6;
    std::string chapterHeader = "OBJECTIVES  (" + std::to_string(stepNum) + "/6)";
    DrawText(chapterHeader.c_str(), (int)bannerX + 16, (int)bannerY + 10, 11, Color{ 255, 215, 80, 255 });

    // Toggle button on top right of the card
    Rectangle toggleBtn = { bannerX + bannerW - 92.0f, bannerY + 8.0f, 80.0f, 22.0f };
    bool isTogHov = CheckCollisionPointRec(mousePos, toggleBtn);
    if (isTogHov && isPressed) {
        showObjectives = false;
        SoundFX::PlayButtonClick();
    }
    DrawRectangleRounded(toggleBtn, 0.3f, 3, isTogHov ? Color{ 40, 52, 75, 220 } : Color{ 22, 28, 42, 180 });
    DrawRectangleRoundedLines(toggleBtn, 0.3f, 3, isTogHov ? Color{ 140, 210, 255, 240 } : Color{ 60, 80, 115, 200 });
    DrawText("HIDE [O]", (int)toggleBtn.x + 14, (int)toggleBtn.y + 5, 10, isTogHov ? Color{ 255, 255, 255, 255 } : Color{ 180, 205, 235, 220 });

    DrawText(currentObjectiveText.c_str(), (int)bannerX + 16, (int)bannerY + 30, 12, Color{ 255, 255, 255, 255 });
    DrawText(currentControlHint.c_str(), (int)bannerX + 16, (int)bannerY + 50, 10, Color{ 120, 220, 255, 220 });
}

bool TutorialManager::DrawPopup(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed) {
    if (!showPopup) return false;

    // Dark backdrop overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 6, 8, 14, 215 });

    float cardW = 680.0f;
    float cardH = 370.0f;
    float cardX = ((float)windowWidth - cardW) * 0.5f;
    float cardY = ((float)windowHeight - cardH) * 0.5f;

    // Outer backing card
    DrawRectangleRounded(Rectangle{ cardX, cardY, cardW, cardH }, 0.05f, 8, Color{ 16, 20, 30, 252 });
    DrawRectangleRoundedLines(Rectangle{ cardX, cardY, cardW, cardH }, 0.05f, 8, Color{ 215, 175, 75, 240 });
    DrawRectangleRoundedLines(Rectangle{ cardX + 4, cardY + 4, cardW - 8, cardH - 8 }, 0.045f, 8, Color{ 50, 68, 98, 180 });

    // Gold corner accents
    Color cornerCol = Color{ 255, 215, 80, 240 };
    DrawLineEx({ cardX + 10, cardY + 10 }, { cardX + 30, cardY + 10 }, 2.0f, cornerCol);
    DrawLineEx({ cardX + 10, cardY + 10 }, { cardX + 10, cardY + 30 }, 2.0f, cornerCol);
    DrawLineEx({ cardX + cardW - 10, cardY + 10 }, { cardX + cardW - 30, cardY + 10 }, 2.0f, cornerCol);
    DrawLineEx({ cardX + cardW - 10, cardY + 10 }, { cardX + cardW - 10, cardY + 30 }, 2.0f, cornerCol);
    DrawLineEx({ cardX + 10, cardY + cardH - 10 }, { cardX + 30, cardY + cardH - 10 }, 2.0f, cornerCol);
    DrawLineEx({ cardX + 10, cardY + cardH - 10 }, { cardX + 10, cardY + cardH - 30 }, 2.0f, cornerCol);
    DrawLineEx({ cardX + cardW - 10, cardY + cardH - 10 }, { cardX + cardW - 30, cardY + cardH - 10 }, 2.0f, cornerCol);
    DrawLineEx({ cardX + cardW - 10, cardY + cardH - 10 }, { cardX + cardW - 10, cardY + cardH - 30 }, 2.0f, cornerCol);

    // Illuminated Category
    DrawText("TUTORIAL ARCHIVES // CHAPTER 1", (int)cardX + 36, (int)cardY + 24, 11, Color{ 120, 220, 255, 240 });

    // Title in Gold
    DrawText(popupTitle.c_str(), (int)cardX + 36, (int)cardY + 44, 20, Color{ 255, 215, 80, 255 });
    DrawLine((int)cardX + 36, (int)cardY + 74, (int)(cardX + cardW - 36), (int)cardY + 74, Color{ 140, 110, 50, 200 });

    // Body Text
    std::istringstream stream(popupBody);
    std::string paragraph;
    int currentY = (int)cardY + 92;
    while (std::getline(stream, paragraph)) {
        if (paragraph.empty()) {
            currentY += 12;
            continue;
        }
        DrawText(paragraph.c_str(), (int)cardX + 36, currentY, 14, Color{ 235, 240, 250, 245 });
        currentY += 22;
    }

    // Keybindings highlight box
    Rectangle hintRec = { cardX + 36.0f, cardY + 220.0f, cardW - 72.0f, 44.0f };
    DrawRectangleRounded(hintRec, 0.2f, 4, Color{ 24, 32, 48, 240 });
    DrawRectangleRoundedLines(hintRec, 0.2f, 4, Color{ 80, 120, 175, 200 });
    DrawText(popupHint.c_str(), (int)cardX + 54, (int)cardY + 234, 13, Color{ 140, 230, 255, 255 });

    // Button: Resume / Proceed
    Rectangle btnRec = { cardX + (cardW - 300.0f) * 0.5f, cardY + 290.0f, 300.0f, 48.0f };
    bool hovered = CheckCollisionPointRec(mousePos, btnRec);
    DrawRectangleRounded(btnRec, 0.2f, 6, hovered ? Color{ 55, 85, 135, 255 } : Color{ 30, 48, 75, 240 });
    DrawRectangleRoundedLines(btnRec, 0.2f, 6, hovered ? Color{ 180, 230, 255, 255 } : Color{ 120, 180, 240, 220 });

    const char* btnLabel = "RESUME [SPACE / CLICK]";
    int bw = MeasureText(btnLabel, 14);
    DrawText(btnLabel, (int)(btnRec.x + (btnRec.width - bw) * 0.5f), (int)btnRec.y + 16, 14, hovered ? Color{ 255, 255, 255, 255 } : Color{ 220, 240, 255, 240 });

    bool dismiss = (isMousePressed && hovered) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
    if (dismiss) {
        showPopup = false;
        SoundFX::PlayButtonClick();
        return true;
    }

    return false;
}
