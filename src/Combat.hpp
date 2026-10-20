#pragma once
#include "raylib.h"

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
    Vector2 origin = { 0.0f, 0.0f };
    float angle = 0.0f; // degrees
    float range = 0.0f;
    float arcAngle = 0.0f; // degrees for cone
    float width = 0.0f;    // for line
    float damage = 0.0f;
    float knockback = 0.0f;
    float timer = 0.0f;
    float duration = 0.0f;
    bool hasHit = false;
};

struct TelegraphIndicator {
    TelegraphShape shape = TelegraphShape::CONE;
    Vector2 origin = { 0.0f, 0.0f };
    float angle = 0.0f;     // facing direction in degrees
    float radius = 0.0f;    // range or circle radius
    float arcAngle = 0.0f;  // sector spread in degrees
    float width = 0.0f;     // width if line
    float progress = 0.0f;  // 0.0f to 1.0f (filling warning)
    Color warningColor = Color{ 255, 60, 60, 160 };
};
