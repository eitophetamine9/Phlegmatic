#include "LootDrop.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../core/Particles.hpp"
#include "../core/SoundFX.hpp"
#include "raymath.h"
#include <cmath>
#include <cstdlib>

LootDrop::LootDrop(LootType type, Vector2 startPos, int amount)
    : type(type), position(startPos), amount(amount), collected(false), age(0.0f), bounceCount(0), bobOffset(0.0f) {
    float angle = (float)(rand() % 360) * DEG2RAD;
    float speed = 35.0f + (float)(rand() % 45);
    velocity = { cosf(angle) * speed, sinf(angle) * speed };
    zHeight = 0.0f;
    zVelocity = -120.0f - (float)(rand() % 50);
}

void LootDrop::Update(float dt, Vector2 playerPos, Player& player, ParticleSystem& particles) {
    if (collected) return;
    age += dt;

    // 1. Vertical bounce & arc physics
    if (bounceCount < 2) {
        zVelocity += 480.0f * dt;
        zHeight -= zVelocity * dt;
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
        velocity.x *= (1.0f - 3.0f * dt);
        velocity.y *= (1.0f - 3.0f * dt);

        if (zHeight <= 0.0f) {
            zHeight = 0.0f;
            bounceCount++;
            zVelocity = -zVelocity * 0.40f;
            if (fabsf(zVelocity) < 20.0f) {
                bounceCount = 2;
                zVelocity = 0.0f;
            }
        }
    } else {
        zHeight = 0.0f;
        bobOffset = sinf(age * 4.5f) * 3.5f;
    }

    // 2. Magnetic attraction
    float dist = Vector2Distance(position, playerPos);
    const float magnetRadius = 75.0f;
    if (dist <= magnetRadius && dist > 1.0f) {
        Vector2 dir = { (playerPos.x - position.x) / dist, (playerPos.y - position.y) / dist };
        float magnetSpeed = 140.0f + (magnetRadius - dist) * 4.0f;
        position.x += dir.x * magnetSpeed * dt;
        position.y += dir.y * magnetSpeed * dt;
    }

    // 3. Pickup collision
    const float pickupRadius = 26.0f;
    if (dist <= pickupRadius) {
        collected = true;
        int finalAmount = amount;
        if (player.HasFullArmorSet(ArmorSet::COMMONERS_SCRAP) && (type == LootType::SHINGLES || type == LootType::ORE_SCRAP)) {
            finalAmount = (int)ceilf((float)amount * 1.15f);
        }

        if (type == LootType::SHINGLES) {
            player.GetInventory().AddShingles(finalAmount);
            std::string label = "+" + std::to_string(finalAmount) + " Shingles";
            particles.SpawnFloatingText({ playerPos.x, playerPos.y - 18.0f }, label, Color{ 230, 242, 255, 255 }, 18.0f);
            SoundFX::PlayCoinPickup();
            particles.SpawnSparks(position, Color{ 210, 230, 255, 255 }, 10, 100.0f);
        } else if (type == LootType::ELIXIR_FLASK) {
            player.GetInventory().AddHealthPotions(amount);
            std::string label = "+" + std::to_string(amount) + " Elixir Flask";
            particles.SpawnFloatingText({ playerPos.x, playerPos.y - 18.0f }, label, Color{ 120, 255, 180, 255 }, 20.0f);
            SoundFX::PlayHeal();
            particles.SpawnSparks(position, Color{ 100, 255, 160, 255 }, 12, 120.0f);
        } else if (type == LootType::ORE_SCRAP) {
            player.GetInventory().AddOreScraps(finalAmount);
            std::string label = "+" + std::to_string(finalAmount) + " Ore Scrap" + (finalAmount > 1 ? "s" : "");
            particles.SpawnFloatingText({ playerPos.x, playerPos.y - 18.0f }, label, Color{ 255, 195, 60, 255 }, 20.0f);
            SoundFX::PlayParrySuccess();
            particles.SpawnSparks(position, Color{ 255, 185, 40, 255 }, 14, 110.0f);
        } else if (type == LootType::MIST_ESSENCE) {
            player.GetInventory().AddEssenceOfMist(finalAmount);
            std::string label = "+" + std::to_string(finalAmount) + " Essence of Mist";
            particles.SpawnFloatingText({ playerPos.x, playerPos.y - 18.0f }, label, Color{ 90, 235, 215, 255 }, 20.0f);
            SoundFX::PlayParrySuccess();
            particles.SpawnSparks(position, Color{ 90, 235, 215, 255 }, 16, 120.0f);
        } else if (type == LootType::SHADOW_ESSENCE) {
            player.GetInventory().AddShadowEssence(finalAmount);
            std::string label = "+" + std::to_string(finalAmount) + " Shadow Essence";
            particles.SpawnFloatingText({ playerPos.x, playerPos.y - 18.0f }, label, Color{ 190, 110, 255, 255 }, 22.0f);
            SoundFX::PlayParrySuccess();
            particles.SpawnSparks(position, Color{ 190, 110, 255, 255 }, 20, 150.0f);
        }
    }
}

void LootDrop::Draw(float time) const {
    if (collected) return;

    // 1. Soft ground shadow
    float shadowScale = fmaxf(0.3f, 1.0f - zHeight / 80.0f);
    DrawEllipse((int)position.x, (int)position.y + 4, 10.0f * shadowScale, 5.0f * shadowScale, Color{ 10, 14, 20, (unsigned char)(110 * shadowScale) });

    float drawY = position.y - zHeight - bobOffset;

    if (type == LootType::SHINGLES) {
        // Silver Shingle Cluster
        float pulse = 0.6f + 0.4f * sinf(age * 5.0f);
        DrawCircleGradient(Vector2{ position.x, drawY }, 18.0f * pulse, Color{ 210, 235, 255, (unsigned char)(50 * pulse) }, Color{ 0, 0, 0, 0 });

        // Base silver plate
        Rectangle shingleRec = { position.x - 7.0f, drawY - 4.0f, 14.0f, 9.0f };
        DrawRectangleRounded(shingleRec, 0.2f, 3, Color{ 190, 210, 235, 255 });
        DrawRectangleRoundedLines(shingleRec, 0.2f, 3, Color{ 80, 110, 150, 230 });
        DrawLine((int)(position.x - 6), (int)(drawY - 3), (int)(position.x + 6), (int)(drawY - 3), Color{ 255, 255, 255, 255 });

        // Multi-shingle stack if amount >= 5
        if (amount >= 5) {
            Rectangle topRec = { position.x - 5.0f, drawY - 8.0f, 13.0f, 8.0f };
            DrawRectangleRounded(topRec, 0.2f, 3, Color{ 220, 235, 255, 255 });
            DrawRectangleRoundedLines(topRec, 0.2f, 3, Color{ 90, 120, 165, 240 });
            DrawLine((int)(position.x - 4), (int)(drawY - 7), (int)(position.x + 5), (int)(drawY - 7), Color{ 255, 255, 255, 255 });
        }

        // Glint highlight
        DrawPixel((int)position.x - 3, (int)drawY - 2, WHITE);
        DrawPixel((int)position.x - 2, (int)drawY - 2, WHITE);
    } else if (type == LootType::ELIXIR_FLASK) {
        // Emerald Elixir Phial
        float pulse = 0.7f + 0.3f * sinf(age * 4.0f);
        DrawCircleGradient(Vector2{ position.x, drawY }, 24.0f * pulse, Color{ 100, 255, 170, (unsigned char)(60 * pulse) }, Color{ 0, 0, 0, 0 });

        // Glass phial body
        Rectangle flaskRec = { position.x - 5.0f, drawY - 6.0f, 10.0f, 14.0f };
        DrawRectangleRounded(flaskRec, 0.4f, 4, Color{ 35, 175, 95, 225 });
        DrawRectangleRoundedLines(flaskRec, 0.4f, 4, Color{ 80, 255, 160, 255 });

        // Stopper & neck
        DrawRectangle((int)position.x - 2, (int)drawY - 10, 4, 4, Color{ 55, 200, 120, 240 });
        DrawRectangle((int)position.x - 3, (int)drawY - 13, 6, 3, Color{ 220, 180, 90, 255 });

        // Glass gleam highlight
        DrawLine((int)position.x - 3, (int)drawY - 4, (int)position.x - 3, (int)drawY + 4, Color{ 210, 255, 230, 220 });
    } else if (type == LootType::ORE_SCRAP) {
        // Glowing Amber Ore Scrap Cluster
        float pulse = 0.7f + 0.3f * sinf(age * 5.0f);
        DrawCircleGradient(Vector2{ position.x, drawY }, 20.0f * pulse, Color{ 245, 175, 45, (unsigned char)(70 * pulse) }, Color{ 0, 0, 0, 0 });

        // Jagged metallic ore chunk polygon
        Vector2 v1 = { position.x - 7.0f, drawY + 3.0f };
        Vector2 v2 = { position.x, drawY - 7.0f };
        Vector2 v3 = { position.x + 8.0f, drawY - 1.0f };
        Vector2 v4 = { position.x + 4.0f, drawY + 6.0f };
        DrawTriangle(v1, v2, v3, Color{ 190, 125, 35, 255 });
        DrawTriangle(v1, v3, v4, Color{ 145, 90, 25, 255 });

        // Golden mineral veins
        DrawLineEx(v2, { position.x + 2.0f, drawY }, 2.0f, Color{ 255, 225, 110, 255 });
        DrawLineEx({ position.x - 3.0f, drawY }, v4, 1.5f, Color{ 255, 200, 70, 240 });

        // Metallic glint
        DrawPixel((int)position.x, (int)drawY - 4, WHITE);
        DrawPixel((int)position.x + 1, (int)drawY - 4, WHITE);
    } else if (type == LootType::MIST_ESSENCE) {
        // Swirling Cyan Mist Essence Orb
        float pulse = 0.75f + 0.25f * sinf(age * 5.0f);
        DrawCircleGradient(Vector2{ position.x, drawY }, 24.0f * pulse, Color{ 90, 235, 215, (unsigned char)(90 * pulse) }, Color{ 0, 0, 0, 0 });
        DrawCircleV(Vector2{ position.x, drawY }, 6.5f, Color{ 40, 120, 110, 230 });
        DrawCircleV(Vector2{ position.x, drawY }, 4.5f, Color{ 140, 255, 240, 255 });
        DrawCircleLines((int)position.x, (int)drawY, 9.0f * pulse, Color{ 100, 245, 225, 200 });
    } else if (type == LootType::SHADOW_ESSENCE) {
        // Abyssal Violet Shadow Essence Core
        float pulse = 0.75f + 0.25f * sinf(age * 4.5f);
        DrawCircleGradient(Vector2{ position.x, drawY }, 26.0f * pulse, Color{ 175, 75, 255, (unsigned char)(95 * pulse) }, Color{ 0, 0, 0, 0 });
        DrawCircleV(Vector2{ position.x, drawY }, 7.5f, Color{ 35, 18, 55, 240 });
        DrawCircleV(Vector2{ position.x, drawY }, 5.0f, Color{ 220, 160, 255, 255 });
        DrawCircleLines((int)position.x, (int)drawY, 11.0f * pulse, Color{ 190, 120, 255, 220 });
    }
}

void LootDrop::SpawnFromEnemy(const Enemy& enemy, std::vector<LootDrop>& outDrops) {
    Vector2 pos = enemy.GetPosition();
    EnemyType type = enemy.GetType();

    if (type == EnemyType::ELEMENTAL_GOLEM) {
        // Secret Boss Elemental Golem: 12-18 Ore Scraps + 150-250 Shingles + 3 Elixirs
        int scrapCount = 12 + (rand() % 7);
        outDrops.emplace_back(LootType::ORE_SCRAP, pos, scrapCount);
        int shingleCount = 150 + (rand() % 101);
        outDrops.emplace_back(LootType::SHINGLES, Vector2{ pos.x + 16.0f, pos.y }, shingleCount);
        outDrops.emplace_back(LootType::ELIXIR_FLASK, Vector2{ pos.x - 18.0f, pos.y }, 1);
        outDrops.emplace_back(LootType::ELIXIR_FLASK, Vector2{ pos.x + 18.0f, pos.y }, 1);
        outDrops.emplace_back(LootType::ELIXIR_FLASK, Vector2{ pos.x, pos.y - 18.0f }, 1);
    } else if (type == EnemyType::ORE_GOLEM) {
        // Ore Golem: 2-4 Ore Scraps + 15-30 Shingles + 40% Elixir
        int scrapCount = 2 + (rand() % 3);
        outDrops.emplace_back(LootType::ORE_SCRAP, pos, scrapCount);
        int shingleCount = 15 + (rand() % 16);
        outDrops.emplace_back(LootType::SHINGLES, Vector2{ pos.x + 12.0f, pos.y }, shingleCount);
        if ((rand() % 100) < 40) {
            outDrops.emplace_back(LootType::ELIXIR_FLASK, Vector2{ pos.x - 12.0f, pos.y }, 1);
        }
    } else if (type == EnemyType::INQUISITOR_BOSS || type == EnemyType::ZIL_BOSS || type == EnemyType::KNIGHT_LENON_BOSS) {
        // Boss Drops: 100-150 Shingles + 2 Guaranteed Elixirs
        int shingleCount = 100 + (rand() % 51);
        outDrops.emplace_back(LootType::SHINGLES, pos, shingleCount);
        outDrops.emplace_back(LootType::ELIXIR_FLASK, Vector2{ pos.x - 16.0f, pos.y }, 1);
        outDrops.emplace_back(LootType::ELIXIR_FLASK, Vector2{ pos.x + 16.0f, pos.y }, 1);
    } else if (type == EnemyType::GALE_BANDIT) {
        // Gale Bandit: 8-16 Shingles + 35% Essence of Mist (STRICTLY NO Ore Scraps!)
        int shingleCount = 8 + (rand() % 9);
        outDrops.emplace_back(LootType::SHINGLES, pos, shingleCount);
        if ((rand() % 100) < 35) {
            outDrops.emplace_back(LootType::MIST_ESSENCE, Vector2{ pos.x + 12.0f, pos.y }, 1);
        }
    } else if (type == EnemyType::GALE_ARCHER_BOSS) {
        // Gale Archer Boss: 50-80 Shingles + Guaranteed 1-2 Shadow Essence + 1 Elixir (STRICTLY NO Ore Scraps!)
        int shingleCount = 50 + (rand() % 31);
        outDrops.emplace_back(LootType::SHINGLES, pos, shingleCount);
        int shadowCount = 1 + (rand() % 2);
        outDrops.emplace_back(LootType::SHADOW_ESSENCE, Vector2{ pos.x - 14.0f, pos.y }, shadowCount);
        outDrops.emplace_back(LootType::ELIXIR_FLASK, Vector2{ pos.x + 14.0f, pos.y }, 1);
    } else if (type == EnemyType::BRUTE || type == EnemyType::CHARGER || type == EnemyType::PYROMANCER) {
        // Elite / Heavy Mobs: 10-25 Shingles + 35% Elixir
        int shingleCount = 10 + (rand() % 16);
        outDrops.emplace_back(LootType::SHINGLES, pos, shingleCount);
        if ((rand() % 100) < 35) {
            outDrops.emplace_back(LootType::ELIXIR_FLASK, pos, 1);
        }
    } else {
        // Standard Mobs (Slasher, Archer): 1-5 Shingles + 15% Elixir
        int shingleCount = 1 + (rand() % 5);
        outDrops.emplace_back(LootType::SHINGLES, pos, shingleCount);
        if ((rand() % 100) < 15) {
            outDrops.emplace_back(LootType::ELIXIR_FLASK, pos, 1);
        }
    }
}
