#pragma once
#include "raylib.h"

enum class WeaponType {
    SWORD,
    DAGGERS,
    MACE,
    TRUTH_SEEKER
};

enum class AttackType {
    NONE,
    LIGHT,
    HEAVY
};

enum class TelegraphShape {
    CONE,
    CIRCLE,
    LINE
};

struct AttackInfo {
    AttackType type = AttackType::NONE;
    WeaponType weapon = WeaponType::SWORD;
    Vector2 origin = { 0.0f, 0.0f };
    float angle = 0.0f;
    float range = 0.0f;
    float arcAngle = 0.0f;
    float width = 0.0f;
    float damage = 0.0f;
    float knockback = 0.0f;
    float timer = 0.0f;
    float duration = 0.0f;
    bool hasHit = false;
};

struct TelegraphIndicator {
    TelegraphShape shape = TelegraphShape::CONE;
    Vector2 origin = { 0.0f, 0.0f };
    float angle = 0.0f;
    float radius = 0.0f;
    float arcAngle = 0.0f;
    float width = 0.0f;
    float progress = 0.0f;
    Color warningColor = Color{ 255, 60, 60, 160 };
};
