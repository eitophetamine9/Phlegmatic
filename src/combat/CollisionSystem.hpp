#pragma once
#include "raylib.h"
#include <vector>

class Player;
class Enemy;
class Projectile;
class ParticleSystem;
class LootDrop;

class CollisionSystem {
public:
    static void CheckCombatCollisions(Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, ParticleSystem& particles, Camera2D camera, std::vector<LootDrop>* outLootDrops = nullptr);
};
