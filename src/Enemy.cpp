#include "Enemy.hpp"
#include "SoundFX.hpp"
#include "AssetManager.hpp"
#include "raymath.h"
#include <cmath>

Enemy::Enemy(EnemyType type, Vector2 startPos) : type(type), position(startPos) {
    velocity = { 0.0f, 0.0f };
    state = EnemyState::IDLE_CHASE;
    stateTimer = 0.0f;
    hasDamagedPlayer = false;
    warnedSound = false;
    facingAngle = 0.0f;

    switch (type) {
        case EnemyType::SLASHER:
            radius = 20.0f;
            maxHealth = 80.0f;
            moveSpeed = 165.0f;
            attackRange = 115.0f;
            windupDuration = 0.85f;
            strikeDuration = 0.16f;
            recoveryDuration = 0.65f;
            stunDuration = 2.0f;
            telegraph.shape = TelegraphShape::CONE;
            telegraph.arcAngle = 95.0f;
            telegraph.radius = attackRange;
            telegraph.warningColor = Color{ 255, 75, 45, 140 };
            break;

        case EnemyType::BRUTE:
            radius = 30.0f;
            maxHealth = 190.0f;
            moveSpeed = 100.0f;
            attackRange = 150.0f;
            windupDuration = 1.25f;
            strikeDuration = 0.22f;
            recoveryDuration = 0.90f;
            stunDuration = 2.4f;
            telegraph.shape = TelegraphShape::CIRCLE;
            telegraph.radius = attackRange;
            telegraph.warningColor = Color{ 255, 40, 70, 160 };
            break;

        case EnemyType::CHARGER:
            radius = 18.0f;
            maxHealth = 65.0f;
            moveSpeed = 190.0f;
            attackRange = 240.0f;
            windupDuration = 0.70f;
            strikeDuration = 0.18f;
            recoveryDuration = 0.55f;
            stunDuration = 1.8f;
            telegraph.shape = TelegraphShape::LINE;
            telegraph.radius = attackRange;
            telegraph.width = 50.0f;
            telegraph.warningColor = Color{ 255, 160, 20, 150 };
            break;
    }

    health = maxHealth;
}

void Enemy::Update(float dt, Vector2 playerPos, const GameMap& map, ParticleSystem& particles) {
    if (state == EnemyState::DEAD) return;

    float dx = playerPos.x - position.x;
    float dy = playerPos.y - position.y;
    float distToPlayer = sqrtf(dx * dx + dy * dy);

    // Update facing towards player unless mid-strike
    if (state == EnemyState::IDLE_CHASE || state == EnemyState::TELEGRAPH_WINDUP) {
        if (state == EnemyState::TELEGRAPH_WINDUP) {
            // Slower tracking during windup so player can sidestep or time parry
            float targetAngle = atan2f(dy, dx) * RAD2DEG;
            float angleDiff = targetAngle - facingAngle;
            while (angleDiff > 180.0f) angleDiff -= 360.0f;
            while (angleDiff < -180.0f) angleDiff += 360.0f;
            facingAngle += angleDiff * dt * 3.5f;
        } else {
            facingAngle = atan2f(dy, dx) * RAD2DEG;
        }
    }

    // State Machine
    switch (state) {
        case EnemyState::IDLE_CHASE: {
            if (distToPlayer <= attackRange * 0.95f) {
                // Start Telegraph Windup!
                state = EnemyState::TELEGRAPH_WINDUP;
                stateTimer = windupDuration;
                hasDamagedPlayer = false;
                warnedSound = false;
                velocity = { 0.0f, 0.0f };
            } else {
                // Chase player
                Vector2 moveDir = Vector2Normalize({ dx, dy });
                velocity.x = moveDir.x * moveSpeed;
                velocity.y = moveDir.y * moveSpeed;
            }
            break;
        }

        case EnemyState::TELEGRAPH_WINDUP: {
            stateTimer -= dt;
            float progress = 1.0f - (stateTimer / windupDuration);
            telegraph.progress = fmaxf(0.0f, fminf(1.0f, progress));
            telegraph.origin = position;
            telegraph.angle = facingAngle;

            if (!warnedSound && progress > 0.1f) {
                SoundFX::PlayTelegraphWarning();
                warnedSound = true;
            }

            // Windup sparks near full charge
            if (progress > 0.7f && GetRandomValue(0, 3) == 0) {
                particles.SpawnSparks(position, Color{ 255, 80, 50, 200 }, 1, 30.0f);
            }

            if (stateTimer <= 0.0f) {
                // Strike begins!
                state = EnemyState::STRIKE;
                stateTimer = strikeDuration;
                hasDamagedPlayer = false;

                if (type == EnemyType::CHARGER) {
                    // Dash forward along line
                    float rad = facingAngle * DEG2RAD;
                    velocity = { cosf(rad) * 600.0f, sinf(rad) * 600.0f };
                    particles.SpawnSlashTrail(position, { cosf(rad), sinf(rad) }, Color{ 255, 140, 40, 255 });
                } else if (type == EnemyType::BRUTE) {
                    particles.AddScreenShake(5.0f, 0.18f);
                    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 20, 180.0f);
                } else if (type == EnemyType::SLASHER) {
                    float rad = facingAngle * DEG2RAD;
                    particles.SpawnSlashTrail(position, { cosf(rad), sinf(rad) }, Color{ 255, 70, 70, 255 });
                }
            }
            break;
        }

        case EnemyState::STRIKE: {
            stateTimer -= dt;
            if (type != EnemyType::CHARGER) {
                velocity.x *= 0.85f;
                velocity.y *= 0.85f;
            }
            if (stateTimer <= 0.0f) {
                state = EnemyState::RECOVERY;
                stateTimer = recoveryDuration;
                velocity = { 0.0f, 0.0f };
            }
            break;
        }

        case EnemyState::RECOVERY: {
            stateTimer -= dt;
            velocity.x *= 0.85f;
            velocity.y *= 0.85f;
            if (stateTimer <= 0.0f) {
                state = EnemyState::IDLE_CHASE;
            }
            break;
        }

        case EnemyState::STUNNED: {
            stateTimer -= dt;
            velocity.x *= 0.80f;
            velocity.y *= 0.80f;
            if (stateTimer <= 0.0f) {
                state = EnemyState::IDLE_CHASE;
            }
            break;
        }

        case EnemyState::DEAD:
            break;
    }

    // Apply movement & resolve collisions with map
    Vector2 nextPos = { position.x + velocity.x * dt, position.y + velocity.y * dt };
    position = map.ResolveCollision(position, nextPos, radius);
}

void Enemy::OnParried(ParticleSystem& particles) {
    state = EnemyState::STUNNED;
    stateTimer = stunDuration;
    hasDamagedPlayer = true; // Prevents dealing damage to player
    velocity = { 0.0f, 0.0f };

    particles.SpawnFloatingText({ position.x, position.y - 25.0f }, "STAGGERED!", Color{ 255, 230, 80, 255 }, 24.0f);
    particles.SpawnFloatingText({ position.x, position.y - 5.0f }, "DEFENSE BROKEN!", Color{ 255, 120, 60, 255 }, 16.0f);
}

bool Enemy::TakeDamage(float amount, Vector2 knockbackDir, ParticleSystem& particles) {
    if (state == EnemyState::DEAD) return false;

    // Bonus damage if attacked while stunned
    if (state == EnemyState::STUNNED) {
        amount *= 1.5f;
        particles.SpawnFloatingText({ position.x, position.y - 30.0f }, "CRIT!", Color{ 255, 220, 0, 255 }, 20.0f);
    }

    health -= amount;
    SoundFX::PlayHitImpact();
    particles.SpawnSparks(position, Color{ 255, 60, 60, 255 }, 12, 120.0f);
    particles.SpawnFloatingText(position, "-" + std::to_string((int)amount), Color{ 255, 240, 240, 255 }, 20.0f);

    velocity.x += knockbackDir.x * 180.0f;
    velocity.y += knockbackDir.y * 180.0f;

    if (health <= 0.0f) {
        health = 0.0f;
        state = EnemyState::DEAD;
        SoundFX::PlayEnemyDeath();
        particles.SpawnSparks(position, Color{ 200, 40, 40, 255 }, 25, 220.0f);
        particles.SpawnFloatingText({ position.x, position.y - 10.0f }, "DEFEATED", Color{ 255, 100, 100, 255 }, 22.0f);
        return true;
    }

    return false;
}

bool Enemy::CheckHit(const AttackInfo& attack) const {
    if (state == EnemyState::DEAD || attack.type == AttackType::NONE) return false;

    float dx = position.x - attack.origin.x;
    float dy = position.y - attack.origin.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > attack.range + radius) return false;

    // Arc check
    float hitAngle = atan2f(dy, dx) * RAD2DEG;
    float diff = fabsf(hitAngle - attack.angle);
    while (diff > 180.0f) diff = 360.0f - diff;

    return diff <= (attack.arcAngle * 0.5f + 15.0f);
}

void Enemy::DrawTelegraph() const {
    if (state != EnemyState::TELEGRAPH_WINDUP && state != EnemyState::STRIKE) return;

    float alphaProgress = (state == EnemyState::STRIKE) ? 0.95f : telegraph.progress;
    Color activeOutline = Color{ 255, 230, 90, 240 };
    Color dangerFill = telegraph.warningColor;
    dangerFill.a = (unsigned char)(dangerFill.a * alphaProgress);

    switch (telegraph.shape) {
        case TelegraphShape::CONE: {
            float startAngle = telegraph.angle - telegraph.arcAngle * 0.5f;
            float endAngle = telegraph.angle + telegraph.arcAngle * 0.5f;

            // Outer telegraph perimeter
            DrawCircleSectorLines(position, telegraph.radius, startAngle, endAngle, 20, activeOutline);
            // Filled danger area that expands/intensifies with progress
            float currentRadius = telegraph.radius * (0.3f + 0.7f * telegraph.progress);
            DrawCircleSector(position, currentRadius, startAngle, endAngle, 20, dangerFill);

            // Warning range boundary arc
            float rad1 = startAngle * DEG2RAD;
            float rad2 = endAngle * DEG2RAD;
            Vector2 p1 = { position.x + cosf(rad1) * telegraph.radius, position.y + sinf(rad1) * telegraph.radius };
            Vector2 p2 = { position.x + cosf(rad2) * telegraph.radius, position.y + sinf(rad2) * telegraph.radius };
            DrawLineEx(position, p1, 2.0f, activeOutline);
            DrawLineEx(position, p2, 2.0f, activeOutline);
            break;
        }

        case TelegraphShape::CIRCLE: {
            // Ground slam expanding ring
            DrawCircleLines((int)position.x, (int)position.y, telegraph.radius, activeOutline);
            DrawCircleLines((int)position.x, (int)position.y, telegraph.radius + 2.0f, Color{ 255, 60, 60, 180 });

            // Expanding inner danger fill
            float currentRadius = telegraph.radius * telegraph.progress;
            DrawCircle((int)position.x, (int)position.y, currentRadius, dangerFill);
            break;
        }

        case TelegraphShape::LINE: {
            float rad = telegraph.angle * DEG2RAD;
            Vector2 dir = { cosf(rad), sinf(rad) };
            Vector2 normal = { -dir.y, dir.x };
            float halfWidth = telegraph.width * 0.5f;

            Vector2 c1 = { position.x - normal.x * halfWidth, position.y - normal.y * halfWidth };
            Vector2 c2 = { position.x + normal.x * halfWidth, position.y + normal.y * halfWidth };
            Vector2 c3 = { c2.x + dir.x * telegraph.radius, c2.y + dir.y * telegraph.radius };
            Vector2 c4 = { c1.x + dir.x * telegraph.radius, c1.y + dir.y * telegraph.radius };

            // Outline
            DrawLineEx(c1, c2, 2.0f, activeOutline);
            DrawLineEx(c2, c3, 2.0f, activeOutline);
            DrawLineEx(c3, c4, 2.0f, activeOutline);
            DrawLineEx(c4, c1, 2.0f, activeOutline);

            // Filling beam
            float fillLength = telegraph.radius * telegraph.progress;
            Vector2 f3 = { c2.x + dir.x * fillLength, c2.y + dir.y * fillLength };
            Vector2 f4 = { c1.x + dir.x * fillLength, c1.y + dir.y * fillLength };
            DrawTriangle(c1, c2, f3, dangerFill);
            DrawTriangle(c1, f3, f4, dangerFill);
            break;
        }
    }
}

void Enemy::Draw() const {
    if (state == EnemyState::DEAD) return;

    Color bodyColor = Color{ 160, 40, 50, 255 }; // Slasher crimson
    if (type == EnemyType::BRUTE) {
        bodyColor = Color{ 110, 30, 45, 255 }; // Brute deep dark red
    } else if (type == EnemyType::CHARGER) {
        bodyColor = Color{ 180, 80, 30, 255 }; // Charger rust orange
    }

    bool isStunned = (state == EnemyState::STUNNED);
    AssetManager::DrawEnemySilhouette(position, facingAngle, bodyColor, radius, isStunned);

    // Health Bar above enemy
    const float barWidth = radius * 2.2f;
    const float barHeight = 5.0f;
    float barX = position.x - barWidth * 0.5f;
    float barY = position.y - radius - 16.0f;

    // Background bar
    DrawRectangle((int)barX, (int)barY, (int)barWidth, (int)barHeight, Color{ 20, 20, 26, 220 });
    // Health fill
    float healthPercent = fmaxf(0.0f, health / maxHealth);
    Color hpColor = (state == EnemyState::STUNNED) ? Color{ 255, 220, 40, 255 } : Color{ 220, 50, 60, 255 };
    DrawRectangle((int)barX, (int)barY, (int)(barWidth * healthPercent), (int)barHeight, hpColor);
    DrawRectangleLines((int)barX, (int)barY, (int)barWidth, (int)barHeight, Color{ 40, 45, 60, 200 });
}
