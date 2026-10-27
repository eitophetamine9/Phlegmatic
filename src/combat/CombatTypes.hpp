#pragma once
#include "raylib.h"

enum class WeaponType {
    NONE,
    SWORD,
    DAGGERS,
    MACE,
    TRUTH_SEEKER
};

enum class ArmorType {
    NONE,
    COMMONERS_SCRAP,
    CRUSADER_PLATE,
    VANGUARD_MAIL,
    SHADOW_LEATHER
};

enum class TotemType {
    NONE,
    TOTEM_SANITY,      // Campaign: +15% overall damage, no nerfs
    TOTEM_INSANITY,    // Sandbox: +30% overall damage, -10% armor
    TOTEM_MIST,        // Sandbox: +20% stamina recovery, +5% cd reduction, -5% damage
    CRUSADER_TOTEM     // Sandbox: +30% defense, -10% movement speed
};

// The Medieval Four Humors Alignment
enum class HumorType {
    PHLEGMATIC, // Player & Solsden Sanctuary (Cold & Moist: Water, Stoic Poise, Quenching)
    CHOLERIC,   // Cultists & Pyromancers & Inquisitor (Hot & Dry: Fire, Yellow Bile, Frenzy)
    MELANCHOLIC,// Iron Brutes & Void Casters & Zil Boss (Cold & Dry: Earth, Black Bile, Brittle Shells)
    SANGUINE    // Deathseeker Archers & Chargers & Knight Lenon (Hot & Moist: Air, Blood, Piercing Bleeds)
};

enum class AttackType {
    NONE,
    LIGHT,
    HEAVY
};

enum class TelegraphShape {
    CONE,
    CIRCLE,
    LINE,
    PROJECTILE_SPREAD
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
    int comboStep = 1;
    bool hasShockwave = false;
    Vector2 shockwavePos = { 0.0f, 0.0f };
    float shockwaveRadius = 0.0f;
    float defensePierce = 0.0f;
    bool isFrontalCircle = false;
    Vector2 circleCenter = { 0.0f, 0.0f };
    float circleRadius = 0.0f;
    bool isLineBox = false;
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
    int spreadCount = 3;
    float spreadAngle = 0.25f; // in radians
};

// Centralized Combat Math & Will Scaling Helpers
inline float CalculateStaggerDuration(float baseStun, float will, bool isPlayer = false) {
    if (baseStun <= 0.0f) return 0.0f;
    float effectiveWill = (will < 0.0f) ? 0.0f : will;
    float duration = baseStun * (100.0f / (100.0f + effectiveWill * 1.25f));
    if (isPlayer) {
        if (duration > 0.85f) duration = 0.85f;
        if (duration < 0.35f) duration = 0.35f;
    } else {
        if (duration < 0.20f) duration = 0.20f;
    }
    return duration;
}
