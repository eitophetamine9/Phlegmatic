#include "NorthZyphdenHandler.hpp"
#include "UI.hpp"
#include "../ui/HUD.hpp"
#include "../ui/ModalUI.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/CollisionSystem.hpp"
#include <algorithm>

NorthZyphdenHandler::NorthZyphdenHandler() {
    berserkerTrollSummoned = false;
    berserkerTrollDefeated = false;
    bossRematchCooldown = 0.0f;
    entryBannerTimer = 0.0f;
    exitCooldown = 0.0f;
    highlandCampfire = { 6, "Highland Crags Bonfire", { 1200.0f, 1300.0f } };
    torvaldPos = { 1115.0f, 1295.0f };
    torvaldRadius = 24.0f;
    isDialogueOpen = false;
    currentDialoguePage = 0;
    dialogueSpeaker = "";
    dialogueColor = Color{ 140, 220, 160, 255 };
    InitSpawners();
    InitFrostCrystals();
    InitBlizzardParticles();
    InitBounties();
}

void NorthZyphdenHandler::InitBounties() {
    bounties.clear();
    bounties.push_back({
        "bounty_trolls",
        "Frostfang Cull",
        "Slay 5 Highland Frost Trolls",
        0, 5, 8, 250, false, false
    });
    bounties.push_back({
        "bounty_crystals",
        "Glacial Shard Harvest",
        "Mine 3 Highland Frost Crystals",
        0, 3, 12, 350, false, false
    });
    bounties.push_back({
        "bounty_berserker",
        "Highland Slayer",
        "Vanquish the Berserker Troll Warlord",
        0, 1, 25, 800, false, false
    });
}

void NorthZyphdenHandler::OpenTorvaldDialogue(const Player& player) {
    (void)player;
    isDialogueOpen = true;
    dialogueSpeaker = "OUTCAST RANGER TORVALD";
    dialogueColor = Color{ 130, 215, 170, 255 };
    currentDialoguePage = 0;
    dialoguePages.clear();

    int claimableCount = 0;
    for (const auto& b : bounties) {
        if (b.completed && !b.claimed) claimableCount++;
    }

    if (claimableCount > 0) {
        std::string claimPage = "You've weathered the frost and brought low the beasts of the crags, traveler! By the old Vanguard oath, take your share of our salvaged stores:\n\n";
        for (const auto& b : bounties) {
            if (b.completed && !b.claimed) {
                claimPage += "- " + b.title + ": +" + std::to_string(b.oreReward) + " Ore Scraps, +" + std::to_string(b.shingleReward) + " Shingles (READY)\n";
            }
        }
        claimPage += "\nPress [SPACE] or Click to claim your rewards and continue.";
        dialoguePages.push_back(claimPage);
    } else if (bounties[0].claimed && bounties[1].claimed && bounties[2].claimed) {
        dialoguePages.push_back(
            "The Berserker Troll is slain, and the highland wind blows still for the first time in an age.\n\n"
            "You possess the unbroken poise of the ancient Gale Monks, friend. When the way further north beyond the crags clears, you will be the one to challenge whatever lies in the deep permafrost."
        );
    } else {
        dialoguePages.push_back(
            "Greetings, traveler. Stand close to the blaze... few brave these northern crags without heavy armor and a stubborn heart. I am Torvald, once a scout under King Charlz's vanguard, now an exile.\n\n"
            "The permafrost breeds vicious beasts. The Frost Trolls hunt in brutal packs, led by a blood-maddened Berserker Troll upon the northern ritual dais."
        );

        std::string statusPage = "Here are the hunting contracts I can reward you for:\n\n";
        for (size_t i = 0; i < bounties.size(); ++i) {
            const auto& b = bounties[i];
            statusPage += std::to_string(i + 1) + ". " + b.title + " (" + b.targetDesc + ")\n";
            statusPage += "   Progress: [" + std::to_string(b.progress) + "/" + std::to_string(b.target) + "]";
            if (b.claimed) {
                statusPage += " - [COMPLETED & CLAIMED]\n";
            } else if (b.completed) {
                statusPage += " - [READY TO CLAIM!]\n";
            } else {
                statusPage += " - Reward: " + std::to_string(b.oreReward) + " Ore, " + std::to_string(b.shingleReward) + " Shingles\n";
            }
        }
        dialoguePages.push_back(statusPage);
    }
}

void NorthZyphdenHandler::CloseDialogue(Player& player, ParticleSystem& particles) {
    isDialogueOpen = false;
    dialoguePages.clear();
    currentDialoguePage = 0;

    int totalOre = 0;
    int totalShingles = 0;
    bool allCompleted = true;
    for (auto& b : bounties) {
        if (!b.completed) allCompleted = false;
        if (b.completed && !b.claimed) {
            b.claimed = true;
            totalOre += b.oreReward;
            totalShingles += b.shingleReward;
        }
    }
    if (totalOre > 0 || totalShingles > 0) {
        player.GetInventory().AddOreScraps(totalOre);
        player.GetInventory().AddShingles(totalShingles);
        SoundFX::PlayCoinPickup();
        particles.SpawnSparks(player.GetPosition(), Color{ 255, 215, 80, 255 }, 35, 170.0f);
        char rewBuf[64];
        snprintf(rewBuf, sizeof(rewBuf), "REWARDS: +%d ORE SCRAPS, +%d SHINGLES!", totalOre, totalShingles);
        particles.SpawnFloatingText(player.GetPosition(), rewBuf, Color{ 255, 225, 90, 255 }, 24.0f);
    }
    if (allCompleted && !player.GetInventory().HasTotem(TotemType::TOTEM_PHLEGMATIC_CORE)) {
        player.GetInventory().UnlockTotem(TotemType::TOTEM_PHLEGMATIC_CORE);
        SoundFX::PlayParrySuccess();
        particles.SpawnFloatingText(player.GetPosition(), "RELIC UNLOCKED: TOTEM OF THE PHLEGMATIC CORE!", Color{ 140, 215, 255, 255 }, 24.0f);
        particles.SpawnSparks(player.GetPosition(), Color{ 120, 210, 255, 255 }, 45, 200.0f);
        particles.AddScreenShake(6.0f, 0.35f);
    }
}

void NorthZyphdenHandler::DrawTorvald(float time, Vector2 playerPos) const {
    (void)time;
    Vector2 pos = torvaldPos;

    // Drop shadow
    DrawEllipse((int)pos.x, (int)(pos.y + 12), torvaldRadius * 1.05f, torvaldRadius * 0.45f, Color{ 10, 16, 24, 130 });

    // Weathered hunter cloak & torso
    DrawRectangleRounded(Rectangle{ pos.x - 13, pos.y - 12, 26, 28 }, 0.4f, 4, Color{ 34, 56, 42, 255 });
    DrawRectangleRoundedLines(Rectangle{ pos.x - 13, pos.y - 12, 26, 28 }, 0.4f, 4, Color{ 48, 76, 58, 255 });

    // Fur mantle trim
    DrawCircle((int)pos.x, (int)(pos.y - 8), 15.0f, Color{ 82, 68, 54, 255 });

    // Hooded cowl
    DrawCircle((int)pos.x, (int)(pos.y - 14), 11.0f, Color{ 42, 66, 50, 255 });
    DrawRectangle((int)(pos.x - 5), (int)(pos.y - 15), 10, 4, Color{ 20, 26, 22, 255 });
    DrawCircle((int)(pos.x + 2), (int)(pos.y - 13), 1.5f, Color{ 140, 230, 190, 255 });

    // Recurve hunting bow strapped to back
    DrawLineEx(Vector2{ pos.x + 10.0f, pos.y - 24.0f }, Vector2{ pos.x + 14.0f, pos.y + 12.0f }, 3.0f, Color{ 115, 80, 48, 255 });
    DrawLineEx(Vector2{ pos.x + 10.0f, pos.y - 24.0f }, Vector2{ pos.x + 14.0f, pos.y + 12.0f }, 1.0f, Color{ 220, 220, 205, 200 });

    // Leather quiver with fletched arrows
    DrawRectangle((int)(pos.x - 15), (int)(pos.y - 8), 5, 16, Color{ 65, 45, 30, 255 });
    DrawLine((int)(pos.x - 13), (int)(pos.y - 14), (int)(pos.x - 13), (int)(pos.y - 8), Color{ 215, 215, 200, 255 });

    // Overhead status badge
    bool anyClaimable = false;
    for (const auto& b : bounties) {
        if (b.completed && !b.claimed) { anyClaimable = true; break; }
    }

    if (anyClaimable) {
        float pulse = 0.5f + 0.5f * sinf(time * 4.0f);
        Color badgeCol = Color{ 255, 215, 70, (unsigned char)(180 + 75 * pulse) };
        FontManager::DrawGameTextShadow("[!] BOUNTY CLAIMABLE", (int)pos.x - 60, (int)pos.y - 36, 10, badgeCol);
    } else {
        FontManager::DrawGameTextShadow("RANGER TORVALD", (int)pos.x - 44, (int)pos.y - 32, 9, Color{ 175, 225, 185, 220 });
    }

    // Interaction prompt when near
    if (Vector2Distance(playerPos, pos) <= 65.0f) {
        FontManager::DrawGameTextShadow("[E] SPEAK WITH TORVALD", (int)pos.x - 64, (int)pos.y - 48, 10, Color{ 255, 235, 120, 240 });
    }
}

void NorthZyphdenHandler::InitFrostCrystals() {
    frostCrystals.clear();
    frostCrystals.push_back({ { 480.0f, 750.0f }, 35.0f, 35.0f, false, 0.0f, 0.0f, 24.0f });
    frostCrystals.push_back({ { 1920.0f, 750.0f }, 35.0f, 35.0f, false, 0.0f, 0.0f, 24.0f });
    frostCrystals.push_back({ { 800.0f, 520.0f }, 35.0f, 35.0f, false, 0.0f, 0.0f, 24.0f });
    frostCrystals.push_back({ { 1600.0f, 520.0f }, 35.0f, 35.0f, false, 0.0f, 0.0f, 24.0f });
}

void NorthZyphdenHandler::InitBlizzardParticles() {
    blizzardSnowflakes.clear();
    blizzardSnowflakes.reserve(160);
    for (int i = 0; i < 160; i++) {
        BlizzardSnowflake flake;
        flake.position = { (float)(rand() % 2600 - 100), (float)(rand() % 2000 - 100) };
        flake.velocity = { -220.0f - (float)(rand() % 140), 90.0f + (float)(rand() % 80) };
        flake.size = 1.5f + ((float)rand() / (float)RAND_MAX) * 2.5f;
        flake.alpha = 0.35f + ((float)rand() / (float)RAND_MAX) * 0.45f;
        blizzardSnowflakes.push_back(flake);
    }
}

void NorthZyphdenHandler::InitSpawners() {
    spawners.clear();
    // Frost Troll highland packs
    spawners.push_back({ { 650.0f, 600.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 1550.0f, 600.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 900.0f, 850.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 1500.0f, 850.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 500.0f, 1050.0f }, 0.0f, false, EnemyType::FROST_TROLL });
    spawners.push_back({ { 1850.0f, 1050.0f }, 0.0f, false, EnemyType::FROST_TROLL });

    // Heavy support brutes & scouts
    spawners.push_back({ { 700.0f, 450.0f }, 0.0f, false, EnemyType::BRUTE });
    spawners.push_back({ { 1700.0f, 450.0f }, 0.0f, false, EnemyType::BRUTE });
    spawners.push_back({ { 1080.0f, 1050.0f }, 0.0f, false, EnemyType::SLASHER });
    spawners.push_back({ { 1320.0f, 1050.0f }, 0.0f, false, EnemyType::SLASHER });

    // 4 Lore Steles across Northern Crags
    loreSteles.clear();
    loreSteles.push_back({
        "frag_11",
        "Fragment XI: The Frigid Highland War",
        "[E] Read Ancient Outpost Monolith",
        Vector2{ 450.0f, 400.0f },
        36.0f,
        false
    });
    loreSteles.push_back({
        "frag_12",
        "Fragment XII: Curse of the Berserker",
        "[E] Inspect Troll Burial Mound",
        Vector2{ 1850.0f, 420.0f },
        36.0f,
        false
    });
    loreSteles.push_back({
        "frag_13",
        "Fragment XIII: The Shattered Totem of Blood",
        "[E] Decipher Blood Altar Inscription",
        Vector2{ 1200.0f, 240.0f },
        42.0f,
        false
    });
    loreSteles.push_back({
        "frag_14",
        "Fragment XIV: Watch of the Northern Vanguard",
        "[E] Inspect Vanguard Lookout Cairn",
        Vector2{ 1750.0f, 1050.0f },
        36.0f,
        false
    });
}

void NorthZyphdenHandler::Init(Player& player, GameMap& map, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, Camera2D& camera) {
    map.Init(MapEnvironment::NORTH_ZYPHDEN);
    player.SetPosition({ 1200.0f, 1380.0f }); // Enters from southern pass
    camera.target = player.GetPosition();
    projectiles.clear();
    enemies.clear();

    InitSpawners();
    InitFrostCrystals();
    InitBlizzardParticles();

    // Spawn initial roaming mobs
    for (auto& spawner : spawners) {
        enemies.emplace_back(spawner.type, spawner.spawnPos);
        spawner.isAlive = true;
        spawner.respawnTimer = 0.0f;
    }

    berserkerTrollSummoned = false;
    entryBannerTimer = 4.0f;
    exitCooldown = 1.0f;
}

void NorthZyphdenHandler::Update(
    float dt,
    Player& player,
    GameMap& map,
    ParticleSystem& particles,
    std::vector<Enemy>& enemies,
    std::vector<Projectile>& projectiles,
    std::vector<LootDrop>& lootDrops,
    Camera2D& camera,
    GameState& currentState,
    const std::function<void()>& onReturnToZyphden,
    const std::function<void()>& onEnterLobby,
    const std::function<void()>& onEnterFrostCaverns
) {
    if (entryBannerTimer > 0.0f) entryBannerTimer -= dt;
    if (exitCooldown > 0.0f) exitCooldown -= dt;
    if (bossRematchCooldown > 0.0f) bossRematchCooldown -= dt;

    // Player Death & Respawn
    if (!player.IsAlive()) {
        if (IsKeyPressed(KEY_R)) {
            player.Revive({ 1200.0f, 1380.0f });
            camera.target = player.GetPosition();
            enemies.clear();
            InitSpawners();
            for (auto& spawner : spawners) {
                enemies.emplace_back(spawner.type, spawner.spawnPos);
                spawner.isAlive = true;
                spawner.respawnTimer = 0.0f;
            }
            berserkerTrollSummoned = false;
        }
        if (IsKeyPressed(KEY_L) || IsKeyPressed(KEY_T)) {
            if (onReturnToZyphden) onReturnToZyphden();
        }
        return;
    }

    Vector2 mouseScreen = GetMousePosition();
    Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

    if (isDialogueOpen) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseDialogue(player, particles);
            return;
        }
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            currentDialoguePage++;
            if (currentDialoguePage >= (int)dialoguePages.size()) {
                CloseDialogue(player, particles);
                return;
            }
        }
        return;
    }

    // Player Combat Inputs
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
    if (IsKeyPressed(KEY_H) || IsKeyPressed(KEY_ONE)) {
        player.UseHealthPotion(particles);
    }
    if (IsKeyPressed(KEY_V)) {
        player.TriggerArmorSkill(particles);
    }
    if (IsKeyPressed(KEY_TAB)) {
        player.QuickSwapWeapon(particles);
    }

    // Shortcuts
    if (IsKeyPressed(KEY_L)) {
        if (onEnterLobby) onEnterLobby();
        return;
    }

    Vector2 playerPos = player.GetPosition();

    // Highland Campfire Waypoint Rest & Frostbite Thaw
    highlandCampfire.Update(player, particles);
    float distToCampfire = Vector2Distance(playerPos, highlandCampfire.position);
    if (distToCampfire <= 200.0f) {
        player.ThawFrostbite(25.0f * dt);
    } else {
        player.AddFrostbite(5.0f * dt, &particles);
    }
    player.UpdateSnowFootsteps(dt, particles);

    // Ranger Torvald interaction
    if (Vector2Distance(playerPos, torvaldPos) <= 65.0f) {
        if (IsKeyPressed(KEY_E)) {
            OpenTorvaldDialogue(player);
            SoundFX::PlayButtonClick();
            return;
        }
    }

    // Update Blizzard Snowflakes (wrap around camera view)
    float camLeft = camera.target.x - 900.0f;
    float camRight = camera.target.x + 900.0f;
    float camTop = camera.target.y - 600.0f;
    float camBottom = camera.target.y + 600.0f;
    for (auto& flake : blizzardSnowflakes) {
        flake.position.x += flake.velocity.x * dt;
        flake.position.y += flake.velocity.y * dt;
        if (flake.position.x < camLeft) {
            flake.position.x = camRight + (rand() % 100);
            flake.position.y = camTop + (rand() % (int)(fmaxf(10.0f, camBottom - camTop + 200.0f))) - 100.0f;
        }
        if (flake.position.y > camBottom) {
            flake.position.y = camTop - (rand() % 60);
            flake.position.x = camLeft + (rand() % (int)(fmaxf(10.0f, camRight - camLeft + 200.0f)));
        }
    }

    // Breakable Frost Crystal Deposits
    for (auto& crystal : frostCrystals) {
        if (crystal.shakeTimer > 0.0f) {
            crystal.shakeTimer -= dt;
        }
        if (crystal.broken) {
            crystal.respawnTimer -= dt;
            if (crystal.respawnTimer <= 0.0f) {
                crystal.broken = false;
                crystal.health = crystal.maxHealth;
                particles.SpawnSparks(crystal.position, Color{ 140, 220, 255, 255 }, 12, 60.0f);
            }
            continue;
        }

        // Check player attack collision
        const AttackInfo& atk = player.GetCurrentAttack();
        if (atk.type != AttackType::NONE && !crystal.broken) {
            float dist = Vector2Distance(crystal.position, atk.origin);
            if (dist <= atk.range + crystal.radius) {
                float dx = crystal.position.x - atk.origin.x;
                float dy = crystal.position.y - atk.origin.y;
                float hitAngle = atan2f(dy, dx) * RAD2DEG;
                float diff = fabsf(hitAngle - atk.angle);
                while (diff > 180.0f) diff = 360.0f - diff;

                if (diff <= (atk.arcAngle * 0.5f + 25.0f)) {
                    crystal.shakeTimer = 0.16f;
                    crystal.health -= atk.damage;
                    SoundFX::PlayHitImpact();
                    particles.SpawnSparks(crystal.position, Color{ 160, 230, 255, 255 }, 14, 120.0f);
                    particles.SpawnFloatingText(crystal.position, "-" + std::to_string((int)atk.damage), Color{ 180, 240, 255, 255 }, 20.0f);

                    if (crystal.health <= 0.0f) {
                        crystal.broken = true;
                        crystal.respawnTimer = 45.0f;
                        SoundFX::PlayParrySuccess();
                        particles.AddScreenShake(3.0f, 0.15f);
                        particles.SpawnSparks(crystal.position, Color{ 120, 215, 255, 255 }, 25, 180.0f);

                        int oreGained = rand() % 2 + 1; // 1-2 Ore Scraps
                        int shinglesGained = rand() % 16 + 15; // 15-30 Shingles
                        int frostShardsGained = rand() % 2 + 2; // 2-3 Frost Shards
                        if (player.HasGolemBreakerT2()) {
                            oreGained += 1;
                            frostShardsGained += 1;
                            particles.SpawnFloatingText(Vector2{ crystal.position.x, crystal.position.y - 20.0f }, "GOLEM BREAKER: +1 BONUS SCRAP!", Color{ 255, 215, 80, 255 }, 18.0f);
                        }
                        player.GetInventory().AddOreScraps(oreGained);
                        player.GetInventory().AddShingles(shinglesGained);
                        player.GetInventory().AddFrostShards(frostShardsGained);
                        particles.SpawnFloatingText(crystal.position, "+" + std::to_string(frostShardsGained) + " Frost Shards", Color{ 140, 235, 255, 255 }, 20.0f);

                        if (bounties.size() > 1 && bounties[1].progress < bounties[1].target) {
                            bounties[1].progress++;
                            if (bounties[1].progress >= bounties[1].target) {
                                bounties[1].completed = true;
                                particles.SpawnFloatingText(playerPos, "BOUNTY COMPLETE: Glacial Shard Harvest!", Color{ 255, 215, 80, 255 }, 22.0f);
                                SoundFX::PlayLoreDiscovery();
                            }
                        }

                        char rewardBuf[64];
                        snprintf(rewardBuf, sizeof(rewardBuf), "+%d ORE SCRAP, +%d SHINGLES", oreGained, shinglesGained);
                        particles.SpawnFloatingText(crystal.position, rewardBuf, Color{ 120, 235, 255, 255 }, 22.0f);
                    }
                }
            }
        }
    }

    // Southern Return Gate Interaction
    Vector2 returnGatePos = { 1200.0f, 1450.0f };
    float distToReturnGate = Vector2Distance(playerPos, returnGatePos);
    if (distToReturnGate <= 95.0f && exitCooldown <= 0.0f) {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_T)) {
            exitCooldown = 1.0f;
            if (onReturnToZyphden) onReturnToZyphden();
            return;
        }
    }

    // Descent into Permafrost Crypts Interaction
    Vector2 cavernsEntrancePos = { 2200.0f, 260.0f };
    float distToCaverns = Vector2Distance(playerPos, cavernsEntrancePos);
    if (distToCaverns <= 95.0f && exitCooldown <= 0.0f) {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_T)) {
            exitCooldown = 1.0f;
            if (onEnterFrostCaverns) onEnterFrostCaverns();
            return;
        }
    }

    // Lore Steles Interaction
    for (auto& stele : loreSteles) {
        const auto* frag = player.GetInventory().GetFragment(stele.fragmentId);
        stele.collected = (frag != nullptr && frag->discovered);

        if (!stele.collected && Vector2Distance(playerPos, stele.position) <= stele.radius) {
            if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                stele.collected = true;
                player.GetInventory().UnlockFragment(stele.fragmentId);
                SoundFX::PlayParrySuccess();
                particles.SpawnFloatingText(stele.position, "DISCOVERED: " + stele.title + "!", Color{ 255, 215, 80, 255 }, 24.0f);
                particles.SpawnSparks(stele.position, Color{ 255, 220, 90, 255 }, 25, 140.0f);
                particles.AddScreenShake(4.0f, 0.2f);
            }
        }
    }

    // Altar of Cold Solitude Interaction (Totem of the Phlegmatic Core)
    Vector2 phlegmaticAltarPos = { 1400.0f, 320.0f };
    if (!player.GetInventory().HasTotem(TotemType::TOTEM_PHLEGMATIC_CORE) && Vector2Distance(playerPos, phlegmaticAltarPos) <= 48.0f) {
        if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            player.GetInventory().UnlockTotem(TotemType::TOTEM_PHLEGMATIC_CORE);
            SoundFX::PlayParrySuccess();
            particles.SpawnFloatingText(phlegmaticAltarPos, "RELIC UNLOCKED: TOTEM OF THE PHLEGMATIC CORE!", Color{ 140, 215, 255, 255 }, 24.0f);
            particles.SpawnSparks(phlegmaticAltarPos, Color{ 120, 210, 255, 255 }, 50, 220.0f);
            particles.AddScreenShake(6.0f, 0.35f);
        }
    }

    // Berserker Troll Arena Trigger & Altar Rematch
    Vector2 arenaCenter = { 1200.0f, 440.0f };
    float distToArena = Vector2Distance(playerPos, arenaCenter);

    bool bossAlive = false;
    for (const auto& en : enemies) {
        if (en.GetType() == EnemyType::BERSERKER_TROLL && en.IsAlive()) {
            bossAlive = true;
            break;
        }
    }

    // Spawn Berserker Troll when entering northern ritual dais
    if (!berserkerTrollSummoned && !berserkerTrollDefeated && !bossAlive && distToArena <= 260.0f) {
        berserkerTrollSummoned = true;
        enemies.emplace_back(EnemyType::BERSERKER_TROLL, Vector2{ 1200.0f, 400.0f });
        particles.SpawnFloatingText(Vector2{ 1200.0f, 400.0f }, "BERSERKER TROLL AWAKENS!", Color{ 255, 45, 60, 255 }, 26.0f);
        particles.AddScreenShake(6.0f, 0.35f);
        SoundFX::PlayHitImpact();
    }

    // Rematch at Altar
    Vector2 altarPos = { 1200.0f, 240.0f };
    float distToAltar = Vector2Distance(playerPos, altarPos);
    if (distToAltar <= 85.0f && !bossAlive && bossRematchCooldown <= 0.0f && berserkerTrollDefeated) {
        if (IsKeyPressed(KEY_E)) {
            enemies.emplace_back(EnemyType::BERSERKER_TROLL, Vector2{ 1200.0f, 400.0f });
            berserkerTrollSummoned = true;
            particles.SpawnFloatingText(Vector2{ 1200.0f, 400.0f }, "BERSERKER TROLL REMATCH COMMENCED!", Color{ 255, 45, 60, 255 }, 26.0f);
            particles.AddScreenShake(6.0f, 0.35f);
            SoundFX::PlayParrySuccess();
        }
    }

    // Entity updates
    player.Update(dt, map, mouseWorld, particles);

    for (const auto& p : player.GetSpawnedProjectiles()) {
        projectiles.push_back(p);
    }
    player.GetSpawnedProjectiles().clear();

    for (auto& enemy : enemies) {
        enemy.Update(dt, player.GetTargetPosition(), map, particles, projectiles);
    }

    for (auto& proj : projectiles) {
        if (proj.IsActive()) proj.Update(dt, map, particles);
    }
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) { return !p.IsActive(); }), projectiles.end());

    // Mob Spawner Respawn Timers
    for (auto& spawner : spawners) {
        if (!spawner.isAlive) {
            spawner.respawnTimer -= dt;
            if (spawner.respawnTimer <= 0.0f) {
                enemies.emplace_back(spawner.type, spawner.spawnPos);
                spawner.isAlive = true;
            }
        }
    }

    // Combat Collisions
    CollisionSystem::CheckCombatCollisions(player, enemies, projectiles, particles, camera, &lootDrops);

    // Enemy Death & Boss Loot Checks
    for (auto it = enemies.begin(); it != enemies.end();) {
        if (!it->IsAlive()) {
            if (it->GetType() == EnemyType::BERSERKER_TROLL) {
                berserkerTrollDefeated = true;
                berserkerTrollSummoned = false;
                bossRematchCooldown = 30.0f;

                // 25% drop chance for Totem of Insanity
                int roll = rand() % 100;
                if (roll < 25 && !player.GetInventory().HasTotem(TotemType::TOTEM_INSANITY)) {
                    player.GetInventory().UnlockTotem(TotemType::TOTEM_INSANITY);
                    SoundFX::PlayGateOpen();
                    SoundFX::PlayCoinPickup();
                    particles.AddScreenShake(12.0f, 0.45f);
                    particles.SpawnSparks(it->GetPosition(), Color{ 255, 45, 60, 255 }, 50, 260.0f);
                    particles.SpawnFloatingText(player.GetPosition(), "RARE DROP: TOTEM OF INSANITY! (25%)", Color{ 255, 45, 60, 255 }, 26.0f);
                } else {
                    player.GetInventory().AddShingles(100);
                    player.GetInventory().AddOreScraps(5);
                    SoundFX::PlayCoinPickup();
                    particles.SpawnFloatingText(player.GetPosition(), "BERSERKER TROLL SLAIN! (+100 SHINGLES, +5 SCRAPS)", Color{ 255, 90, 90, 255 }, 24.0f);
                }
                SoundFX::PlayParrySuccess();
                particles.AddScreenShake(7.0f, 0.4f);

                if (bounties.size() > 2 && !bounties[2].completed) {
                    bounties[2].progress = 1;
                    bounties[2].completed = true;
                    particles.SpawnFloatingText(player.GetPosition(), "BOUNTY COMPLETE: Highland Slayer!", Color{ 255, 215, 80, 255 }, 24.0f);
                    SoundFX::PlayLoreDiscovery();
                }
            } else if (it->GetType() == EnemyType::FROST_TROLL) {
                if (!bounties.empty() && bounties[0].progress < bounties[0].target) {
                    bounties[0].progress++;
                    if (bounties[0].progress >= bounties[0].target) {
                        bounties[0].completed = true;
                        particles.SpawnFloatingText(player.GetPosition(), "BOUNTY COMPLETE: Frostfang Cull!", Color{ 255, 215, 80, 255 }, 22.0f);
                        SoundFX::PlayLoreDiscovery();
                    }
                }
            }

            // Sync with spawner tracking
            for (auto& spawner : spawners) {
                if (spawner.isAlive && Vector2Distance(it->GetPosition(), spawner.spawnPos) <= 120.0f) {
                    spawner.isAlive = false;
                    spawner.respawnTimer = 25.0f;
                    break;
                }
            }

            // Record bestiary kill
            int kIdx = -1;
            if (it->GetType() == EnemyType::FROST_TROLL) kIdx = 9;
            else if (it->GetType() == EnemyType::BERSERKER_TROLL) kIdx = 10;
            else if (it->GetType() == EnemyType::BRUTE) kIdx = 3;
            else if (it->GetType() == EnemyType::SLASHER) kIdx = 0;
            if (kIdx >= 0) player.GetInventory().RecordEnemyKill(kIdx);

            LootDrop::SpawnFromEnemy(*it, lootDrops);
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    // Loot drops update
    for (auto& loot : lootDrops) {
        loot.Update(dt, playerPos, player, particles);
    }
    lootDrops.erase(std::remove_if(lootDrops.begin(), lootDrops.end(), [](const LootDrop& l) { return l.IsCollected(); }), lootDrops.end());

    particles.Update(dt);

    // Camera smoothing
    Vector2 shake = particles.GetScreenShakeOffset();
    Vector2 desiredTarget = { playerPos.x + shake.x, playerPos.y + shake.y };
    camera.target = Vector2Lerp(camera.target, desiredTarget, 10.0f * dt);
}

void NorthZyphdenHandler::Render(
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
    (void)showControlsModal;
    (void)showInventoryModal;
    (void)inventoryActiveTab;
    (void)inventorySelectedFragment;

    BeginMode2D(camera);

    map.DrawFloor(player.GetPosition());

    // Draw Lore Steles in world
    float time = (float)GetTime();
    Vector2 playerPos = player.GetPosition();

    // Highland Campfire Waypoint Rest
    highlandCampfire.Draw(time, player);
    DrawTorvald(time, playerPos);

    // Altar of Cold Solitude (Totem of the Phlegmatic Core Shrine)
    Vector2 phlegmaticAltarPos = { 1400.0f, 320.0f };
    DrawEllipse((int)phlegmaticAltarPos.x, (int)(phlegmaticAltarPos.y + 14), 32.0f, 16.0f, Color{ 8, 14, 22, 140 });
    DrawRectangleRounded(Rectangle{ phlegmaticAltarPos.x - 24, phlegmaticAltarPos.y - 12, 48, 26 }, 0.25f, 4, Color{ 36, 48, 66, 255 });
    DrawRectangleRoundedLines(Rectangle{ phlegmaticAltarPos.x - 24, phlegmaticAltarPos.y - 12, 48, 26 }, 0.25f, 4, Color{ 68, 92, 124, 255 });
    DrawRectangleRounded(Rectangle{ phlegmaticAltarPos.x - 16, phlegmaticAltarPos.y - 20, 32, 14 }, 0.25f, 4, Color{ 48, 64, 86, 255 });

    float altarBob = sinf(time * 2.8f) * 4.0f;
    Vector2 corePos = { phlegmaticAltarPos.x, phlegmaticAltarPos.y - 32.0f + altarBob };
    bool hasRelic = player.GetInventory().HasTotem(TotemType::TOTEM_PHLEGMATIC_CORE);
    Color coreGlow = hasRelic ? Color{ 80, 120, 160, 160 } : Color{ 140, 220, 255, 255 };
    DrawCircleGradient(corePos, 18.0f, Color{ coreGlow.r, coreGlow.g, coreGlow.b, 80 }, Color{ 0, 0, 0, 0 });
    DrawPoly(corePos, 4, 10.0f, 45.0f, coreGlow);
    DrawPoly(corePos, 4, 6.0f, 45.0f, Color{ 230, 250, 255, 255 });

    if (!hasRelic && Vector2Distance(playerPos, phlegmaticAltarPos) <= 52.0f) {
        std::string prompt = "[E] Commune with Altar of Cold Solitude";
        int pw = FontManager::MeasureText(prompt, 10);
        FontManager::DrawGameTextShadow(prompt, (int)phlegmaticAltarPos.x - pw / 2, (int)phlegmaticAltarPos.y - 56, 10, Color{ 180, 235, 255, 255 });
    }

    for (const auto& stele : loreSteles) {
        // Drop shadow
        DrawEllipse((int)stele.position.x, (int)(stele.position.y + 12), stele.radius * 1.15f, stele.radius * 0.55f, Color{ 10, 14, 20, 140 });

        // Carved Monolith Stele
        DrawRectangle((int)stele.position.x - 18, (int)stele.position.y - 28, 36, 56, Color{ 42, 50, 65, 255 });
        DrawRectangleLines((int)stele.position.x - 18, (int)stele.position.y - 28, 36, 56, Color{ 75, 95, 125, 255 });

        float pulse = 0.5f + 0.5f * sinf(time * 3.5f + stele.position.x);
        Color runeGlow = stele.collected ? Color{ 120, 180, 240, 140 } : Color{ 255, 215, 80, (unsigned char)(160 + 80 * pulse) };
        DrawCircleGradient(stele.position, 28.0f * pulse, runeGlow, Color{ 0, 0, 0, 0 });
        DrawCircle((int)stele.position.x, (int)stele.position.y, 4.0f, runeGlow);

        if (Vector2Distance(playerPos, stele.position) <= stele.radius) {
            std::string prompt = stele.collected ? "[E] Re-read " + stele.title : stele.prompt;
            int pw = FontManager::MeasureText(prompt, 10);
            FontManager::DrawGameTextShadow(prompt, (int)stele.position.x - pw / 2, (int)stele.position.y - 42, 10, Color{ 255, 230, 120, 240 });
        }
    }

    // Breakable Frost Crystal Deposits
    for (const auto& crystal : frostCrystals) {
        Vector2 cPos = crystal.position;
        if (crystal.shakeTimer > 0.0f) {
            cPos.x += (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 3.0f;
            cPos.y += (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 3.0f;
        }

        if (crystal.broken) {
            DrawEllipse((int)cPos.x, (int)cPos.y + 4, 18.0f, 8.0f, Color{ 140, 200, 240, 90 });
            DrawCircle((int)cPos.x - 6, (int)cPos.y + 2, 3.0f, Color{ 160, 225, 255, 160 });
            DrawCircle((int)cPos.x + 5, (int)cPos.y + 1, 2.5f, Color{ 180, 235, 255, 160 });
            continue;
        }

        DrawEllipse((int)cPos.x, (int)cPos.y + 10, 22.0f, 10.0f, Color{ 10, 20, 32, 130 });
        float pulse = 0.5f + 0.5f * sinf(time * 3.0f + cPos.x * 0.1f);
        DrawCircleGradient(cPos, 26.0f + 6.0f * pulse, Color{ 130, 215, 255, (unsigned char)(70 + 40 * pulse) }, Color{ 0, 0, 0, 0 });

        Vector2 p1 = { cPos.x, cPos.y - 28.0f };
        Vector2 p2 = { cPos.x - 10.0f, cPos.y + 4.0f };
        Vector2 p3 = { cPos.x + 10.0f, cPos.y + 4.0f };
        DrawTriangle(p1, p2, p3, Color{ 165, 230, 255, 240 });
        DrawTriangleLines(p1, p2, p3, Color{ 220, 245, 255, 255 });

        Vector2 l1 = { cPos.x - 8.0f, cPos.y - 18.0f };
        Vector2 l2 = { cPos.x - 18.0f, cPos.y + 6.0f };
        Vector2 l3 = { cPos.x - 4.0f, cPos.y + 8.0f };
        DrawTriangle(l1, l2, l3, Color{ 135, 205, 245, 230 });
        DrawTriangleLines(l1, l2, l3, Color{ 200, 240, 255, 255 });

        Vector2 r1 = { cPos.x + 8.0f, cPos.y - 20.0f };
        Vector2 r2 = { cPos.x + 4.0f, cPos.y + 8.0f };
        Vector2 r3 = { cPos.x + 18.0f, cPos.y + 6.0f };
        DrawTriangle(r1, r2, r3, Color{ 145, 215, 250, 230 });
        DrawTriangleLines(r1, r2, r3, Color{ 200, 240, 255, 255 });

        if (crystal.health < crystal.maxHealth) {
            float barW = 36.0f;
            float barH = 4.0f;
            float barX = cPos.x - barW * 0.5f;
            float barY = cPos.y - 36.0f;
            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 18, 24, 32, 220 });
            float pct = fmaxf(0.0f, crystal.health / crystal.maxHealth);
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 130, 225, 255, 240 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 180, 240, 255, 200 });
        }
    }

    // Rematch Altar Inscription prompt
    Vector2 altarPos = { 1200.0f, 240.0f };
    float distToAltar = Vector2Distance(playerPos, altarPos);

    // Floating recovery cooldown banner above Blood Altar
    if (berserkerTrollDefeated && bossRematchCooldown > 0.0f) {
        float pillW = 160.0f;
        float pillH = 22.0f;
        Rectangle cdRec = { altarPos.x - pillW * 0.5f, altarPos.y - 70.0f, pillW, pillH };
        DrawRectangleRounded(cdRec, 0.4f, 4, Color{ 25, 20, 28, 220 });
        DrawRectangleRoundedLines(cdRec, 0.4f, 4, Color{ 220, 80, 80, 200 });
        std::string cdMsg = "ALTAR REMATCH: " + std::to_string((int)ceilf(bossRematchCooldown)) + "s";
        int tw = FontManager::MeasureText(cdMsg, 10);
        FontManager::DrawGameTextShadow(cdMsg, (int)(altarPos.x - tw * 0.5f), (int)cdRec.y + 5, 10, Color{ 255, 140, 140, 240 });
    }

    if (distToAltar <= 120.0f && berserkerTrollDefeated) {
        bool bossAlive = false;
        for (const auto& en : enemies) {
            if (en.GetType() == EnemyType::BERSERKER_TROLL && en.IsAlive()) { bossAlive = true; break; }
        }
        if (!bossAlive) {
            if (bossRematchCooldown <= 0.0f) {
                FontManager::DrawGameTextShadow("[E] CHALLENGE BERSERKER TROLL REMATCH", (int)altarPos.x - 130, (int)altarPos.y - 46, 11, Color{ 255, 80, 95, 255 });
            }
        }
    }

    // Southern Return Gate prompt
    Vector2 returnGatePos = { 1200.0f, 1450.0f };
    if (Vector2Distance(playerPos, returnGatePos) <= 120.0f) {
        FontManager::DrawGameTextShadow("[E] RETURN TO ZYPHDEN OVERWORLD", (int)returnGatePos.x - 100, (int)returnGatePos.y - 40, 11, Color{ 160, 220, 255, 240 });
    }

    // Permafrost Crypts Cavern Mouth
    Vector2 cavernsEntrancePos = { 2200.0f, 260.0f };
    DrawCircle((int)cavernsEntrancePos.x, (int)cavernsEntrancePos.y, 35.0f, Color{ 14, 22, 36, 255 });
    DrawCircleLines((int)cavernsEntrancePos.x, (int)cavernsEntrancePos.y, 35.0f, Color{ 80, 190, 255, 230 });
    float cPulse = 0.5f + 0.5f * sinf(time * 3.0f);
    DrawCircleGradient(cavernsEntrancePos, 50.0f, Color{ 60, 180, 255, (unsigned char)(60 + 50 * cPulse) }, Color{ 0, 0, 0, 0 });
    DrawText("PERMAFROST CRYPTS", (int)cavernsEntrancePos.x - 52, (int)cavernsEntrancePos.y - 48, 10, Color{ 150, 225, 255, 240 });
    if (Vector2Distance(playerPos, cavernsEntrancePos) <= 110.0f) {
        FontManager::DrawGameTextShadow("[E] DESCEND INTO THE PERMAFROST CRYPTS", (int)cavernsEntrancePos.x - 120, (int)cavernsEntrancePos.y - 28, 11, Color{ 140, 225, 255, 255 });
    }

    for (const auto& drop : lootDrops) {
        drop.Draw(time);
    }

    for (const auto& enemy : enemies) {
        enemy.DrawTelegraph();
        enemy.Draw();
    }

    player.Draw();

    for (const auto& proj : projectiles) {
        proj.Draw();
    }

    map.DrawWallsAndPillars(player.GetPosition());

    particles.Draw();

    // Draw Blizzard Snowflakes stream
    for (const auto& flake : blizzardSnowflakes) {
        Vector2 p2 = { flake.position.x - 9.0f * (flake.size / 2.0f), flake.position.y + 4.5f * (flake.size / 2.0f) };
        DrawLineEx(flake.position, p2, flake.size * 0.75f, Color{ 230, 245, 255, (unsigned char)(flake.alpha * 240) });
    }

    map.DrawLighting(camera, player.GetPosition());

    EndMode2D();

    // Frostbite Screen Edge Vignette
    float frostMeter = player.GetFrostbiteMeter();
    if (frostMeter > 10.0f) {
        float maxAlpha = (frostMeter / 100.0f) * 110.0f;
        if (player.IsFrostbitten()) {
            float pulse = 0.7f + 0.3f * sinf(time * 4.0f);
            maxAlpha = 140.0f * pulse;
        }
        DrawRectangleGradientV(0, 0, screenWidth, 54, Color{ 130, 210, 255, (unsigned char)maxAlpha }, Color{ 130, 210, 255, 0 });
        DrawRectangleGradientV(0, screenHeight - 54, screenWidth, 54, Color{ 130, 210, 255, 0 }, Color{ 130, 210, 255, (unsigned char)maxAlpha });
        DrawRectangleGradientH(0, 0, 54, screenHeight, Color{ 130, 210, 255, (unsigned char)maxAlpha }, Color{ 130, 210, 255, 0 });
        DrawRectangleGradientH(screenWidth - 54, 0, 54, screenHeight, Color{ 130, 210, 255, 0 }, Color{ 130, 210, 255, (unsigned char)maxAlpha });
    }

    // Crosshair
    Vector2 mousePos = GetMousePosition();
    DrawCircleLines((int)mousePos.x, (int)mousePos.y, 8.0f, Color{ 255, 120, 120, 180 });
    DrawCircle((int)mousePos.x, (int)mousePos.y, 2.0f, Color{ 255, 80, 80, 220 });

    // Campaign HUD
    bool hasUnread = player.GetInventory().HasUnreadFragments();
    GameUI::DrawCampaignHUD(screenWidth, screenHeight, player, "NORTHERN CRAGS OF ZYPHDEN", hasUnread, &map.GetTimeOfDay());

    // Tactical Corner Mini-Map Radar & Compass HUD
    GameUI::DrawTacticalRadar(screenWidth, screenHeight, player, enemies, { highlandCampfire.position }, { torvaldPos }, { { 1200.0f, 240.0f }, phlegmaticAltarPos, { 2200.0f, 260.0f } });

    // Boss Health Bar for Berserker Troll
    for (const auto& enemy : enemies) {
        if (enemy.GetType() == EnemyType::BERSERKER_TROLL && enemy.IsAlive()) {
            float barW = 640.0f;
            float barH = 18.0f;
            float barX = ((float)screenWidth - barW) * 0.5f;
            float barY = 56.0f;

            DrawRectangleRounded(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 18, 12, 14, 240 });
            DrawRectangleRoundedLines(Rectangle{ barX - 12.0f, barY - 24.0f, barW + 24.0f, barH + 46.0f }, 0.2f, 4, Color{ 255, 55, 70, 240 });

            FontManager::DrawTextTitleCentered("THE BERSERKER TROLL (CRAG WARLORD)", screenWidth / 2, (int)barY - 14, 15, Color{ 255, 85, 95, 255 });

            char defBuf[64];
            snprintf(defBuf, sizeof(defBuf), "ARMOR: %d  |  BLOOD FRENZY: ACTIVE", (int)enemy.GetDefense());
            int dw = FontManager::MeasureText(defBuf, 10);
            FontManager::DrawGameTextShadow(defBuf, (screenWidth - dw) / 2, (int)barY + (int)barH + 4, 10, Color{ 240, 180, 180, 220 });

            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 28, 14, 16, 255 });
            float pct = fmaxf(0.0f, enemy.GetHealth() / enemy.GetMaxHealth());
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 235, 45, 60, 255 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 255, 120, 130, 220 });
            break;
        }
    }

    // Interactive Paginated NPC Dialogue Box
    if (isDialogueOpen) {
        bool stillOpen = GameUI::DrawNPCDialogueBox(
            screenWidth, screenHeight, mousePos,
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
            dialogueSpeaker, dialoguePages, currentDialoguePage,
            dialogueColor
        );
        if (!stillOpen) {
            CloseDialogue(const_cast<Player&>(player), const_cast<ParticleSystem&>(particles));
        }
    }
}
