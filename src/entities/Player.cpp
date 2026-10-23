#include "Player.hpp"
#include "PlayerRenderer.hpp"
#include "../core/Config.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/AssetManager.hpp"
#include "raymath.h"
#include <cmath>

Player::Player() {
    position = { 1200.0f, 900.0f };
    velocity = { 0.0f, 0.0f };
    radius = Config::PLAYER_RADIUS;
    facingAngle = 0.0f;
    currentWeapon = WeaponType::SWORD;

    health = Config::PLAYER_MAX_HEALTH;
    maxHealth = Config::PLAYER_MAX_HEALTH;
    stamina = Config::PLAYER_MAX_STAMINA;
    maxStamina = Config::PLAYER_MAX_STAMINA;

    lightCooldown = 0.0f;
    heavyCooldown = 0.0f;
    parryCooldown = 0.0f;
    parryTimer = 0.0f;
    isParrying = false;
    counterBuffTimer = 0.0f;

    dashCooldown = 0.0f;
    dashTimer = 0.0f;
    isDashing = false;
    dashDir = { 0.0f, 0.0f };

    heavyWindupTimer = 0.0f;
    isHeavyWindingUp = false;
    walkCycle = 0.0f;
}

void Player::Init(Vector2 startPos) {
    position = startPos;
    health = maxHealth;
    stamina = maxStamina;
    velocity = { 0.0f, 0.0f };
    currentAttack.type = AttackType::NONE;
    isParrying = false;
    isDashing = false;
    isHeavyWindingUp = false;
    counterBuffTimer = 0.0f;
}

void Player::Revive(Vector2 startPos, float hpPercent) {
    position = startPos;
    health = maxHealth * hpPercent;
    stamina = maxStamina;
    velocity = { 0.0f, 0.0f };
    currentAttack.type = AttackType::NONE;
    isParrying = false;
    isDashing = false;
    isHeavyWindingUp = false;
    counterBuffTimer = 0.0f;
}

void Player::SetWeapon(WeaponType newWeapon) {
    currentWeapon = newWeapon;
    inventory.UnlockWeapon(newWeapon);
    if (newWeapon == WeaponType::TRUTH_SEEKER) {
        SoundFX::PlayTruthSeekerResonance();
    } else {
        SoundFX::PlayEquipWeapon();
    }
}

void Player::Update(float dt, const GameMap& map, Vector2 mouseWorldPos, ParticleSystem& particles) {
    if (!IsAlive()) return;

    // Face towards mouse unless locked in dash
    if (!isDashing) {
        float dx = mouseWorldPos.x - position.x;
        float dy = mouseWorldPos.y - position.y;
        facingAngle = atan2f(dy, dx) * RAD2DEG;
    }

    // Cooldown updates
    if (lightCooldown > 0.0f) lightCooldown -= dt;
    if (heavyCooldown > 0.0f) heavyCooldown -= dt;
    if (parryCooldown > 0.0f) parryCooldown -= dt;
    if (dashCooldown > 0.0f) dashCooldown -= dt;
    if (counterBuffTimer > 0.0f) counterBuffTimer -= dt;

    // Stamina regeneration
    if (!isParrying && !isHeavyWindingUp && !isDashing) {
        stamina = fminf(maxStamina, stamina + Config::STAMINA_REGEN_RATE * dt);
    }

    // Dash update (Q Key)
    if (isDashing) {
        dashTimer -= dt;
        velocity.x = dashDir.x * Config::DASH_SPEED;
        velocity.y = dashDir.y * Config::DASH_SPEED;

        particles.SpawnDashTrail(position, dashDir, Color{ 100, 220, 255, 180 });

        if (dashTimer <= 0.0f) {
            isDashing = false;
            velocity.x *= 0.3f;
            velocity.y *= 0.3f;
        }
    }

    // Parry update (F Key)
    if (isParrying) {
        parryTimer -= dt;
        if (parryTimer <= 0.0f) {
            isParrying = false;
        }
    }

    // Heavy attack windup
    if (isHeavyWindingUp) {
        heavyWindupTimer -= dt;
        particles.SpawnSparks(position, Color{ 255, 200, 60, 200 }, 1, 40.0f);

        if (heavyWindupTimer <= 0.0f) {
            isHeavyWindingUp = false;
            WeaponStats stats = GetWeaponStats(currentWeapon);

            currentAttack.type = AttackType::HEAVY;
            currentAttack.weapon = currentWeapon;
            currentAttack.origin = position;
            currentAttack.angle = facingAngle;
            currentAttack.range = stats.heavyRange;
            currentAttack.arcAngle = stats.heavyArc;
            currentAttack.damage = counterBuffTimer > 0.0f ? (stats.heavyDamage * 1.5f) : stats.heavyDamage;
            currentAttack.knockback = (currentWeapon == WeaponType::MACE || currentWeapon == WeaponType::TRUTH_SEEKER) ? 400.0f : 260.0f;
            currentAttack.duration = stats.heavyDuration;
            currentAttack.timer = stats.heavyDuration;
            currentAttack.hasHit = false;

            SoundFX::PlayHeavyAttack();

            Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
            particles.SpawnSlashTrail(position, forwardDir, (currentWeapon == WeaponType::TRUTH_SEEKER) ? Color{ 255, 220, 60, 255 } : Color{ 255, 180, 50, 255 });
            particles.AddScreenShake((currentWeapon == WeaponType::MACE || currentWeapon == WeaponType::TRUTH_SEEKER) ? 6.0f : 3.5f, 0.14f);

            velocity.x += forwardDir.x * 220.0f;
            velocity.y += forwardDir.y * 220.0f;
        }
    }

    // Current attack duration
    if (currentAttack.type != AttackType::NONE) {
        currentAttack.timer -= dt;
        currentAttack.origin = position;
        if (currentAttack.timer <= 0.0f) {
            currentAttack.type = AttackType::NONE;
        }
    }

    // Normal movement when not dashing
    if (!isDashing) {
        Vector2 moveDir = { 0.0f, 0.0f };
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    moveDir.y -= 1.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  moveDir.y += 1.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  moveDir.x -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDir.x += 1.0f;

        float currentSpeed = Config::PLAYER_SPEED;
        if (isParrying) currentSpeed *= 0.35f;
        if (isHeavyWindingUp) currentSpeed *= 0.25f;
        if (currentAttack.type == AttackType::LIGHT) currentSpeed *= 0.65f;

        if (moveDir.x != 0.0f || moveDir.y != 0.0f) {
            moveDir = Vector2Normalize(moveDir);
            velocity.x = moveDir.x * currentSpeed;
            velocity.y = moveDir.y * currentSpeed;
            walkCycle += dt * 10.0f;
        } else {
            velocity.x *= 0.80f;
            velocity.y *= 0.80f;
        }
    }

    // Collision
    Vector2 nextPos = { position.x + velocity.x * dt, position.y + velocity.y * dt };
    position = map.ResolveCollision(position, nextPos, radius);
}

void Player::TriggerDash(ParticleSystem& particles) {
    if (isDashing) return;
    if (dashCooldown > 0.0f) return;

    WeaponStats stats = GetWeaponStats(currentWeapon);
    if (stamina < stats.dashStaminaCost) return;

    stamina -= stats.dashStaminaCost;
    dashCooldown = stats.dashCooldown;
    dashTimer = Config::DASH_DURATION;
    isDashing = true;

    // Movement direction or facing direction
    Vector2 inputDir = { 0.0f, 0.0f };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    inputDir.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  inputDir.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  inputDir.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) inputDir.x += 1.0f;

    if (inputDir.x != 0.0f || inputDir.y != 0.0f) {
        dashDir = Vector2Normalize(inputDir);
    } else {
        dashDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
    }

    SoundFX::PlayDash();
}

void Player::TriggerLightAttack(ParticleSystem& particles) {
    if (isDashing || isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;

    WeaponStats stats = GetWeaponStats(currentWeapon);
    if (lightCooldown > 0.0f || stamina < stats.lightStamina) return;

    stamina -= stats.lightStamina;
    lightCooldown = stats.lightCooldown;

    currentAttack.type = AttackType::LIGHT;
    currentAttack.weapon = currentWeapon;
    currentAttack.origin = position;
    currentAttack.angle = facingAngle;
    currentAttack.range = stats.lightRange;
    currentAttack.arcAngle = stats.lightArc;
    currentAttack.damage = counterBuffTimer > 0.0f ? (stats.lightDamage * 1.5f) : stats.lightDamage;
    currentAttack.knockback = (currentWeapon == WeaponType::MACE || currentWeapon == WeaponType::TRUTH_SEEKER) ? 200.0f : 120.0f;
    currentAttack.duration = stats.lightDuration;
    currentAttack.timer = stats.lightDuration;
    currentAttack.hasHit = false;

    SoundFX::PlayLightAttack();

    Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
    particles.SpawnSlashTrail(position, forwardDir, Color{ 80, 220, 255, 255 });

    velocity.x += forwardDir.x * 110.0f;
    velocity.y += forwardDir.y * 110.0f;
}

void Player::TriggerHeavyAttack(ParticleSystem& particles) {
    if (isDashing || isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;
    if (heavyCooldown > 0.0f) return;

    WeaponStats stats = GetWeaponStats(currentWeapon);
    if (stamina < stats.heavyStamina) return;

    stamina -= stats.heavyStamina;
    heavyCooldown = stats.heavyCooldown;
    isHeavyWindingUp = true;
    heavyWindupTimer = stats.heavyWindup;
}

void Player::TriggerParry(ParticleSystem& particles) {
    if (isDashing || isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;
    if (parryCooldown > 0.0f || stamina < Config::PARRY_STAMINA_COST) return;

    stamina -= Config::PARRY_STAMINA_COST;
    isParrying = true;
    parryTimer = Config::PARRY_WINDOW;
    parryCooldown = Config::PARRY_COOLDOWN;

    SoundFX::PlayParryMiss();
}

void Player::OnParrySuccessful(ParticleSystem& particles) {
    isParrying = false;
    parryTimer = 0.0f;
    parryCooldown = 0.2f;
    counterBuffTimer = Config::COUNTER_BUFF_DURATION;
    stamina = fminf(maxStamina, stamina + 35.0f);

    SoundFX::PlayParrySuccess();
    particles.SpawnParryBurst(position);
    particles.SpawnFloatingText(position, "PERFECT PARRY!", Color{ 255, 230, 80, 255 }, 26.0f);
}

bool Player::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles) {
    // Invulnerable during Dash
    if (isDashing) {
        particles.SpawnFloatingText(position, "EVADED", Color{ 120, 240, 255, 255 }, 18.0f);
        return false;
    }

    // Parried
    if (isParrying) {
        OnParrySuccessful(particles);
        return false;
    }

    health -= amount;
    if (health < 0.0f) health = 0.0f;

    SoundFX::PlayHitImpact();
    particles.AddScreenShake(6.0f, 0.18f);
    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 15, 160.0f);
    particles.SpawnFloatingText(position, "-" + std::to_string((int)amount), Color{ 255, 60, 60, 255 }, 22.0f);

    velocity.x += knockbackDir.x * 240.0f;
    velocity.y += knockbackDir.y * 240.0f;
    return true;
}

void Player::Heal(float amount) {
    health = fminf(maxHealth, health + amount);
}

void Player::Draw() const {
    PlayerRenderer::Draw(*this);
}

