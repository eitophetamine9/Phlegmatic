#include "Player.hpp"
#include "Config.hpp"
#include "SoundFX.hpp"
#include "AssetManager.hpp"
#include "raymath.h"
#include <cmath>

Player::Player() {
    position = { 1200.0f, 900.0f };
    velocity = { 0.0f, 0.0f };
    radius = Config::PLAYER_RADIUS;
    facingAngle = 0.0f;
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
    counterBuffTimer = 0.0f;
}

void Player::Update(float dt, const GameMap& map, Vector2 mouseWorldPos, ParticleSystem& particles) {
    if (!IsAlive()) return;

    // Face towards mouse
    float dx = mouseWorldPos.x - position.x;
    float dy = mouseWorldPos.y - position.y;
    facingAngle = atan2f(dy, dx) * RAD2DEG;

    // Cooldown updates
    if (lightCooldown > 0.0f) lightCooldown -= dt;
    if (heavyCooldown > 0.0f) heavyCooldown -= dt;
    if (parryCooldown > 0.0f) parryCooldown -= dt;
    if (counterBuffTimer > 0.0f) counterBuffTimer -= dt;

    // Stamina regeneration (when not heavily exerting)
    if (!isParrying && !isHeavyWindingUp) {
        stamina = fminf(maxStamina, stamina + Config::STAMINA_REGEN_RATE * dt);
    }

    // Parry timer update
    if (isParrying) {
        parryTimer -= dt;
        if (parryTimer <= 0.0f) {
            isParrying = false;
        }
    }

    // Heavy attack windup update
    if (isHeavyWindingUp) {
        heavyWindupTimer -= dt;
        // Particle charge effect
        particles.SpawnSparks(position, Color{ 255, 200, 60, 200 }, 1, 40.0f);

        if (heavyWindupTimer <= 0.0f) {
            isHeavyWindingUp = false;
            // Execute heavy attack swing
            currentAttack.type = AttackType::HEAVY;
            currentAttack.origin = position;
            currentAttack.angle = facingAngle;
            currentAttack.range = Config::HEAVY_ATTACK_RANGE;
            currentAttack.arcAngle = Config::HEAVY_ATTACK_ARC;
            currentAttack.damage = counterBuffTimer > 0.0f ? (Config::HEAVY_ATTACK_DAMAGE * 1.6f) : Config::HEAVY_ATTACK_DAMAGE;
            currentAttack.knockback = 280.0f;
            currentAttack.duration = Config::HEAVY_ATTACK_DURATION;
            currentAttack.timer = Config::HEAVY_ATTACK_DURATION;
            currentAttack.hasHit = false;

            SoundFX::PlayHeavyAttack();

            Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
            particles.SpawnSlashTrail(position, forwardDir, Color{ 255, 180, 50, 255 });
            particles.AddScreenShake(3.5f, 0.12f);

            // Lunging forward impulse
            velocity.x += forwardDir.x * 220.0f;
            velocity.y += forwardDir.y * 220.0f;
        }
    }

    // Current attack duration update
    if (currentAttack.type != AttackType::NONE) {
        currentAttack.timer -= dt;
        currentAttack.origin = position; // follow player
        if (currentAttack.timer <= 0.0f) {
            currentAttack.type = AttackType::NONE;
        }
    }

    // WASD or Arrow Keys movement
    Vector2 moveDir = { 0.0f, 0.0f };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    moveDir.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  moveDir.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  moveDir.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDir.x += 1.0f;

    // Movement speed modifier during attacks / parry
    float currentSpeed = Config::PLAYER_SPEED;
    if (isParrying) currentSpeed *= 0.35f;
    if (isHeavyWindingUp) currentSpeed *= 0.25f;
    if (currentAttack.type == AttackType::LIGHT) currentSpeed *= 0.65f;

    if (moveDir.x != 0.0f || moveDir.y != 0.0f) {
        moveDir = Vector2Normalize(moveDir);
        velocity.x = moveDir.x * currentSpeed;
        velocity.y = moveDir.y * currentSpeed;
        walkCycle += dt * 10.0f;

        // Dust particle occasionally
        if (GetRandomValue(0, 10) == 0) {
            particles.SpawnSparks(position, Color{ 60, 70, 90, 150 }, 1, 20.0f);
        }
    } else {
        velocity.x *= 0.80f;
        velocity.y *= 0.80f;
    }

    // Move & collide with map
    Vector2 nextPos = { position.x + velocity.x * dt, position.y + velocity.y * dt };
    position = map.ResolveCollision(position, nextPos, radius);
}

void Player::TriggerLightAttack(ParticleSystem& particles) {
    if (isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;
    if (lightCooldown > 0.0f || stamina < Config::LIGHT_ATTACK_STAMINA) return;

    stamina -= Config::LIGHT_ATTACK_STAMINA;
    lightCooldown = Config::LIGHT_ATTACK_COOLDOWN;

    currentAttack.type = AttackType::LIGHT;
    currentAttack.origin = position;
    currentAttack.angle = facingAngle;
    currentAttack.range = Config::LIGHT_ATTACK_RANGE;
    currentAttack.arcAngle = Config::LIGHT_ATTACK_ARC;
    currentAttack.damage = counterBuffTimer > 0.0f ? (Config::LIGHT_ATTACK_DAMAGE * 1.5f) : Config::LIGHT_ATTACK_DAMAGE;
    currentAttack.knockback = 120.0f;
    currentAttack.duration = Config::LIGHT_ATTACK_DURATION;
    currentAttack.timer = Config::LIGHT_ATTACK_DURATION;
    currentAttack.hasHit = false;

    SoundFX::PlayLightAttack();

    Vector2 forwardDir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
    particles.SpawnSlashTrail(position, forwardDir, Color{ 80, 220, 255, 255 });

    // Slight step forward
    velocity.x += forwardDir.x * 120.0f;
    velocity.y += forwardDir.y * 120.0f;
}

void Player::TriggerHeavyAttack(ParticleSystem& particles) {
    if (isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;
    if (heavyCooldown > 0.0f || stamina < Config::HEAVY_ATTACK_STAMINA) return;

    stamina -= Config::HEAVY_ATTACK_STAMINA;
    heavyCooldown = Config::HEAVY_ATTACK_COOLDOWN;
    isHeavyWindingUp = true;
    heavyWindupTimer = Config::HEAVY_ATTACK_WINDUP;

    particles.SpawnFloatingText(position, "CHARGE!", Color{ 255, 200, 60, 255 }, 18.0f);
}

void Player::TriggerParry(ParticleSystem& particles) {
    if (isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;
    if (parryCooldown > 0.0f || stamina < Config::PARRY_STAMINA_COST) return;

    stamina -= Config::PARRY_STAMINA_COST;
    isParrying = true;
    parryTimer = Config::PARRY_WINDOW;
    parryCooldown = Config::PARRY_COOLDOWN;

    SoundFX::PlayParryMiss(); // Wind sound on stance activate
    particles.SpawnFloatingText(position, "PARRY STANCE", Color{ 100, 240, 255, 255 }, 18.0f);
}

void Player::OnParrySuccessful(ParticleSystem& particles) {
    isParrying = false;
    parryTimer = 0.0f;
    parryCooldown = 0.2f; // reset cooldown so player isn't locked
    counterBuffTimer = Config::COUNTER_BUFF_DURATION; // Empower next attacks!
    stamina = fminf(maxStamina, stamina + 35.0f);     // Reward stamina on successful parry

    SoundFX::PlayParrySuccess();
    particles.SpawnParryBurst(position);
    particles.SpawnFloatingText(position, "PERFECT PARRY!", Color{ 255, 230, 80, 255 }, 26.0f);
    particles.SpawnFloatingText({ position.x, position.y + 24.0f }, "COUNTER BUFF ACTIVE!", Color{ 255, 140, 50, 255 }, 18.0f);
}

bool Player::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles) {
    if (isParrying) {
        // Player successfully parried the incoming attack!
        OnParrySuccessful(particles);
        return false; // Took no damage!
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
    Color bodyColor = Color{ 40, 110, 210, 255 }; // Knight azure
    if (counterBuffTimer > 0.0f) {
        bodyColor = Color{ 220, 140, 40, 255 }; // Empowered golden glow
    }

    AssetManager::DrawPlayerSilhouette(position, facingAngle, bodyColor, isParrying, IsAttacking());

    // Draw Sword / Weapon in hand
    float swordAngle = facingAngle + 35.0f;
    Vector2 swordOrigin = {
        position.x + cosf(facingAngle * DEG2RAD) * 12.0f - sinf(facingAngle * DEG2RAD) * 10.0f,
        position.y + sinf(facingAngle * DEG2RAD) * 12.0f + cosf(facingAngle * DEG2RAD) * 10.0f
    };

    if (currentAttack.type == AttackType::LIGHT) {
        float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
        swordAngle = facingAngle - 50.0f + progress * Config::LIGHT_ATTACK_ARC;
        AssetManager::DrawSword(swordOrigin, swordAngle, 38.0f, Color{ 230, 240, 255, 255 }, Color{ 0, 229, 255, 200 });

        // Draw light slash arc blade trail
        float startAngle = facingAngle - 50.0f;
        float sweepAngle = progress * Config::LIGHT_ATTACK_ARC;
        DrawCircleSector(position, Config::LIGHT_ATTACK_RANGE, startAngle, startAngle + sweepAngle, 16, Color{ 0, 220, 255, 60 });
        DrawCircleSectorLines(position, Config::LIGHT_ATTACK_RANGE, startAngle, startAngle + sweepAngle, 16, Color{ 120, 245, 255, 180 });
    } else if (currentAttack.type == AttackType::HEAVY) {
        float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
        swordAngle = facingAngle - 60.0f + progress * Config::HEAVY_ATTACK_ARC;
        AssetManager::DrawSword(swordOrigin, swordAngle, 50.0f, Color{ 255, 240, 190, 255 }, Color{ 255, 180, 40, 220 });

        // Draw heavy slash golden arc
        float startAngle = facingAngle - 60.0f;
        float sweepAngle = progress * Config::HEAVY_ATTACK_ARC;
        DrawCircleSector(position, Config::HEAVY_ATTACK_RANGE, startAngle, startAngle + sweepAngle, 20, Color{ 255, 180, 40, 80 });
        DrawCircleSectorLines(position, Config::HEAVY_ATTACK_RANGE, startAngle, startAngle + sweepAngle, 20, Color{ 255, 230, 100, 220 });
    } else if (isHeavyWindingUp) {
        swordAngle = facingAngle - 90.0f; // Pulled back for thrust
        AssetManager::DrawSword(swordOrigin, swordAngle, 45.0f, Color{ 255, 200, 80, 255 }, Color{ 255, 120, 20, 200 });
    } else {
        AssetManager::DrawSword(swordOrigin, swordAngle, 34.0f, Color{ 210, 220, 235, 255 }, Color{ 80, 140, 200, 120 });
    }
}
