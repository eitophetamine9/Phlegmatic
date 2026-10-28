#pragma once
#include "raylib.h"
#include <vector>

enum class LootType {
    SHINGLES,      // Ancient hammered silver shingles
    ELIXIR_FLASK,  // Restorative health potion flask
    ORE_SCRAP      // Raw metallic quarry ore scrap used for blacksmith forging
};

class Player;
class ParticleSystem;
class Enemy;

class LootDrop {
public:
    LootDrop(LootType type, Vector2 startPos, int amount = 1);

    void Update(float dt, Vector2 playerPos, Player& player, ParticleSystem& particles);
    void Draw(float time) const;

    bool IsCollected() const { return collected; }
    Vector2 GetPosition() const { return position; }
    LootType GetType() const { return type; }
    int GetAmount() const { return amount; }

    // Drops generator based on defeated enemy type
    static void SpawnFromEnemy(const Enemy& enemy, std::vector<LootDrop>& outDrops);

private:
    LootType type;
    Vector2 position;
    Vector2 velocity;
    float zHeight;
    float zVelocity;
    int amount;
    bool collected;
    float age;
    int bounceCount;
    float bobOffset;
};
