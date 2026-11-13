#pragma once
#include "raylib.h"

enum class WeaponType {
    NONE,
    SWORD,
    DAGGERS,
    MACE,
    TRUTH_SEEKER,
    HALBERD,
    ZEPHYR_DAGGERS,
    WINDRUNNER_RECURVE,
    SUN_CLAYMORE,
    STORM_GLAIVE,
    GLACIAL_CLEAVER
};

enum class ArmorType {
    NONE,
    COMMONERS_SCRAP,
    CRUSADER_PLATE,
    VANGUARD_MAIL,
    SHADOW_LEATHER
};

enum class HelmetType {
    NONE,
    COMMONERS_HOOD,
    CRUSADER_HELM,
    VANGUARD_GREATHELM,
    SHADOW_COWL,
    SCRAP_SALLET,
    BASALT_GREATHELM,
    FROSTWARDEN_HOOD
};

enum class ChestplateType {
    NONE,
    COMMONERS_TUNIC,
    CRUSADER_CUIRASS,
    VANGUARD_CHEST,
    SHADOW_GARB,
    SCRAP_CUIRASS,
    BASALT_CUIRASS,
    FROSTWARDEN_HAUBERK
};

enum class LeggingsType {
    NONE,
    COMMONERS_BREECHES,
    CRUSADER_GREAVES,
    VANGUARD_GREAVES,
    SHADOW_LEGGINGS,
    SCRAP_GREAVES,
    BASALT_GREAVES,
    FROSTWARDEN_GREAVES
};

enum class ArmorSet {
    NONE,
    COMMONERS_SCRAP,
    CRUSADER_PLATE,
    VANGUARD_BASTION,
    SHADOWSTRIDER,
    FORGED_SCRAP_IRON,
    BASALT_GOLEM,
    FROSTWARDEN
};

enum class TotemType {
    NONE,
    TOTEM_SANITY,          // Crossroads: +15% overall damage, +15 Will, no nerfs
    TOTEM_INSANITY,        // Northern Crags Boss: +65% DMG (+135% <40% HP), +18% move speed, +25% atk speed, -50% DEF, x1.5 taken
    TOTEM_MIST,            // Mist Forest Boss: Mist Fairy (+20% crit, +35% crit dmg, +30 HP orbs), +10% stam regen, -5% speed
    CRUSADER_TOTEM,        // Zyphden Spar: +25% defense, +35 max HP, +18% attack, Will of Atlas, -12% speed
    ELEMENTAL_CORE_TOTEM,  // Quarry Boss: +25% DEF, Mini Golem (460 HP, 55 DEF, 3 patterns), 54 DMG bolt (3s CD), -10% speed
    TOTEM_PHLEGMATIC_CORE, // Northern Highland: -80% knockback, gray health recovery, chill counter
    CHAMPION_TOTEM         // Royal Arena: +35% DMG, +30% DEF, +15% speed, +35% stam, -20% CDs, +40 Will, +50 HP, +14 HP/s regen (+2 in combat), -70% external healing
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
