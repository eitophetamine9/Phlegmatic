#include "Player.hpp"
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

void Player::SetWeapon(WeaponType newWeapon) {
    currentWeapon = newWeapon;
    inventory.UnlockWeapon(newWeapon);
    SoundFX::PlayEquipWeapon();
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

    if (dashCooldown > 0.0f) {
        particles.SpawnFloatingText(position, "DASH CD!", Color{ 140, 210, 255, 240 }, 14.0f);
        return;
    }

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
    particles.SpawnFloatingText(position, "DASH", Color{ 120, 230, 255, 255 }, 16.0f);
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

    if (heavyCooldown > 0.0f) {
        particles.SpawnFloatingText(position, "HEAVY CD!", Color{ 255, 140, 60, 240 }, 14.0f);
        return;
    }

    WeaponStats stats = GetWeaponStats(currentWeapon);
    if (stamina < stats.heavyStamina) return;

    stamina -= stats.heavyStamina;
    heavyCooldown = stats.heavyCooldown;
    isHeavyWindingUp = true;
    heavyWindupTimer = stats.heavyWindup;

    particles.SpawnFloatingText(position, "CHARGE!", Color{ 255, 200, 60, 255 }, 18.0f);
}

void Player::TriggerParry(ParticleSystem& particles) {
    if (isDashing || isParrying || isHeavyWindingUp || currentAttack.type != AttackType::NONE) return;
    if (parryCooldown > 0.0f || stamina < Config::PARRY_STAMINA_COST) return;

    stamina -= Config::PARRY_STAMINA_COST;
    isParrying = true;
    parryTimer = Config::PARRY_WINDOW;
    parryCooldown = Config::PARRY_COOLDOWN;

    SoundFX::PlayParryMiss();
    particles.SpawnFloatingText(position, "PARRY STANCE", Color{ 100, 240, 255, 255 }, 18.0f);
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
    Color bodyColor = Color{ 40, 110, 210, 255 };
    if (counterBuffTimer > 0.0f) bodyColor = Color{ 220, 140, 40, 255 };
    if (isDashing) bodyColor = Color{ 100, 200, 255, 180 };

    AssetManager::DrawPlayerSilhouette(position, facingAngle, bodyColor, isParrying, isDashing);

    Vector2 dir = { cosf(facingAngle * DEG2RAD), sinf(facingAngle * DEG2RAD) };
    Vector2 normal = { -dir.y, dir.x };

    Vector2 rightHand = {
        position.x + dir.x * 12.0f + normal.x * 11.0f,
        position.y + dir.y * 12.0f + normal.y * 11.0f
    };
    Vector2 leftHand = {
        position.x + dir.x * 12.0f - normal.x * 11.0f,
        position.y + dir.y * 12.0f - normal.y * 11.0f
    };

    if (currentWeapon == WeaponType::SWORD) {
        float swordAngle = facingAngle + 35.0f;
        if (currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
            swordAngle = facingAngle - 50.0f + progress * currentAttack.arcAngle;
            AssetManager::DrawSword(rightHand, swordAngle, 38.0f, Color{ 230, 240, 255, 255 }, Color{ 0, 229, 255, 200 });

            float startAngle = facingAngle - 50.0f;
            float sweepAngle = progress * currentAttack.arcAngle;
            DrawCircleSector(position, currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 0, 220, 255, 60 });
            DrawCircleSectorLines(position, currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 120, 245, 255, 180 });
        } else if (currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
            swordAngle = facingAngle - 60.0f + progress * currentAttack.arcAngle;
            AssetManager::DrawSword(rightHand, swordAngle, 48.0f, Color{ 255, 240, 190, 255 }, Color{ 255, 180, 40, 220 });

            float startAngle = facingAngle - 60.0f;
            float sweepAngle = progress * currentAttack.arcAngle;
            DrawCircleSector(position, currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 180, 40, 80 });
            DrawCircleSectorLines(position, currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 230, 100, 220 });
        } else if (isHeavyWindingUp) {
            swordAngle = facingAngle - 85.0f;
            AssetManager::DrawSword(rightHand, swordAngle, 42.0f, Color{ 255, 200, 80, 255 }, Color{ 255, 120, 20, 200 });
        } else {
            AssetManager::DrawSword(rightHand, swordAngle, 34.0f, Color{ 210, 220, 235, 255 }, Color{ 80, 140, 200, 120 });
        }
    } else if (currentWeapon == WeaponType::DAGGERS) {
        float angleL = facingAngle - 25.0f;
        float angleR = facingAngle + 25.0f;

        if (currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
            angleR = facingAngle - 30.0f + progress * 80.0f;
            angleL = facingAngle + 30.0f - progress * 80.0f;

            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 180, 255, 220, 255 }, Color{ 60, 240, 180, 200 });

            float startAngle = facingAngle - 45.0f;
            float sweepAngle = progress * currentAttack.arcAngle;
            DrawCircleSector(position, currentAttack.range, startAngle, startAngle + sweepAngle, 14, Color{ 60, 240, 180, 60 });
            DrawCircleSectorLines(position, currentAttack.range, startAngle, startAngle + sweepAngle, 14, Color{ 120, 255, 200, 180 });
        } else if (currentAttack.type == AttackType::HEAVY) {
            // 360 Whirlwind spin
            float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
            float spinAngle = facingAngle + progress * 360.0f;
            angleR = spinAngle;
            angleL = spinAngle + 180.0f;

            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 220, 255, 200, 255 }, Color{ 80, 255, 160, 220 });
            DrawCircleLines((int)position.x, (int)position.y, currentAttack.range, Color{ 80, 255, 160, 180 });
            DrawCircle((int)position.x, (int)position.y, currentAttack.range * progress, Color{ 60, 240, 180, 40 });
        } else if (isHeavyWindingUp) {
            angleL = facingAngle - 70.0f;
            angleR = facingAngle - 50.0f;
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 220, 255, 140, 255 }, Color{ 140, 240, 60, 200 });
        } else {
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 180, 240, 220, 255 }, Color{ 60, 200, 160, 140 });
        }
    } else if (currentWeapon == WeaponType::MACE) {
        float maceAngle = facingAngle + 40.0f;
        if (currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
            maceAngle = facingAngle - 40.0f + progress * currentAttack.arcAngle;
            AssetManager::DrawMace(rightHand, maceAngle, 42.0f, Color{ 230, 180, 120, 255 }, Color{ 255, 140, 40, 200 });

            float startAngle = facingAngle - 40.0f;
            float sweepAngle = progress * currentAttack.arcAngle;
            DrawCircleSector(position, currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 255, 140, 40, 60 });
            DrawCircleSectorLines(position, currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 255, 180, 80, 180 });
        } else if (currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
            maceAngle = facingAngle - 70.0f + progress * currentAttack.arcAngle;
            AssetManager::DrawMace(rightHand, maceAngle, 52.0f, Color{ 255, 200, 140, 255 }, Color{ 255, 90, 30, 240 });

            // Earthshattering slam shockwave
            float startAngle = facingAngle - 70.0f;
            float sweepAngle = progress * currentAttack.arcAngle;
            DrawCircleSector(position, currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 90, 30, 80 });
            DrawCircleSectorLines(position, currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 140, 50, 220 });
        } else if (isHeavyWindingUp) {
            maceAngle = facingAngle - 100.0f;
            AssetManager::DrawMace(rightHand, maceAngle, 46.0f, Color{ 255, 160, 60, 255 }, Color{ 255, 60, 20, 220 });
        } else {
            AssetManager::DrawMace(rightHand, maceAngle, 38.0f, Color{ 210, 170, 130, 255 }, Color{ 200, 120, 50, 120 });
        }
    } else if (currentWeapon == WeaponType::TRUTH_SEEKER) {
        float swordAngle = facingAngle + 35.0f;
        if (currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
            swordAngle = facingAngle - 55.0f + progress * currentAttack.arcAngle;
            AssetManager::DrawSword(rightHand, swordAngle, 46.0f, Color{ 255, 235, 140, 255 }, Color{ 255, 210, 50, 240 });

            float startAngle = facingAngle - 55.0f;
            float sweepAngle = progress * currentAttack.arcAngle;
            DrawCircleSector(position, currentAttack.range, startAngle, startAngle + sweepAngle, 18, Color{ 255, 215, 60, 75 });
            DrawCircleSectorLines(position, currentAttack.range, startAngle, startAngle + sweepAngle, 18, Color{ 255, 240, 120, 220 });
        } else if (currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (currentAttack.timer / currentAttack.duration);
            swordAngle = facingAngle - 70.0f + progress * currentAttack.arcAngle;
            AssetManager::DrawSword(rightHand, swordAngle, 56.0f, Color{ 255, 245, 180, 255 }, Color{ 255, 170, 20, 255 });

            float startAngle = facingAngle - 70.0f;
            float sweepAngle = progress * currentAttack.arcAngle;
            DrawCircleSector(position, currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 255, 180, 30, 90 });
            DrawCircleSectorLines(position, currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 255, 235, 100, 240 });
        } else if (isHeavyWindingUp) {
            swordAngle = facingAngle - 90.0f;
            AssetManager::DrawSword(rightHand, swordAngle, 50.0f, Color{ 255, 220, 80, 255 }, Color{ 255, 140, 10, 240 });
        } else {
            AssetManager::DrawSword(rightHand, swordAngle, 42.0f, Color{ 255, 225, 120, 255 }, Color{ 255, 190, 40, 180 });
        }
    }
}
