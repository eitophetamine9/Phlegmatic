#include "EnemyRenderer.hpp"
#include "Enemy.hpp"
#include "../rendering/AssetManager.hpp"
#include "raymath.h"
#include <cmath>

void EnemyRenderer::DrawTelegraph(const Enemy& enemy) {
    if (enemy.state != EnemyState::TELEGRAPH_WINDUP && enemy.state != EnemyState::STRIKE) return;

    float alphaProgress = (enemy.state == EnemyState::STRIKE) ? 0.95f : enemy.telegraph.progress;
    Color activeOutline = Color{ 255, 230, 90, 240 };
    Color dangerFill = enemy.telegraph.warningColor;
    dangerFill.a = (unsigned char)(dangerFill.a * alphaProgress);

    switch (enemy.telegraph.shape) {
        case TelegraphShape::CONE: {
            float startAngle = enemy.telegraph.angle - enemy.telegraph.arcAngle * 0.5f;
            float endAngle = enemy.telegraph.angle + enemy.telegraph.arcAngle * 0.5f;

            DrawCircleSectorLines(enemy.position, enemy.telegraph.radius, startAngle, endAngle, 20, activeOutline);
            float currentRadius = enemy.telegraph.radius * (0.3f + 0.7f * enemy.telegraph.progress);
            DrawCircleSector(enemy.position, currentRadius, startAngle, endAngle, 20, dangerFill);

            float rad1 = startAngle * DEG2RAD;
            float rad2 = endAngle * DEG2RAD;
            Vector2 p1 = { enemy.position.x + cosf(rad1) * enemy.telegraph.radius, enemy.position.y + sinf(rad1) * enemy.telegraph.radius };
            Vector2 p2 = { enemy.position.x + cosf(rad2) * enemy.telegraph.radius, enemy.position.y + sinf(rad2) * enemy.telegraph.radius };
            DrawLineEx(enemy.position, p1, 2.0f, activeOutline);
            DrawLineEx(enemy.position, p2, 2.0f, activeOutline);
            break;
        }

        case TelegraphShape::CIRCLE: {
            DrawCircleLines((int)enemy.position.x, (int)enemy.position.y, enemy.telegraph.radius, activeOutline);
            DrawCircleLines((int)enemy.position.x, (int)enemy.position.y, enemy.telegraph.radius + 2.0f, Color{ 255, 60, 60, 180 });

            float currentRadius = enemy.telegraph.radius * enemy.telegraph.progress;
            DrawCircle((int)enemy.position.x, (int)enemy.position.y, currentRadius, dangerFill);
            break;
        }

        case TelegraphShape::LINE: {
            float rad = enemy.telegraph.angle * DEG2RAD;
            Vector2 dir = { cosf(rad), sinf(rad) };
            Vector2 normal = { -dir.y, dir.x };
            float halfWidth = enemy.telegraph.width * 0.5f;

            Vector2 c1 = { enemy.position.x - normal.x * halfWidth, enemy.position.y - normal.y * halfWidth };
            Vector2 c2 = { enemy.position.x + normal.x * halfWidth, enemy.position.y + normal.y * halfWidth };
            Vector2 c3 = { c2.x + dir.x * enemy.telegraph.radius, c2.y + dir.y * enemy.telegraph.radius };
            Vector2 c4 = { c1.x + dir.x * enemy.telegraph.radius, c1.y + dir.y * enemy.telegraph.radius };

            DrawLineEx(c1, c2, 2.0f, activeOutline);
            DrawLineEx(c2, c3, 2.0f, activeOutline);
            DrawLineEx(c3, c4, 2.0f, activeOutline);
            DrawLineEx(c4, c1, 2.0f, activeOutline);

            float fillLength = enemy.telegraph.radius * enemy.telegraph.progress;
            Vector2 f3 = { c2.x + dir.x * fillLength, c2.y + dir.y * fillLength };
            Vector2 f4 = { c1.x + dir.x * fillLength, c1.y + dir.y * fillLength };
            DrawTriangle(c1, c2, f3, dangerFill);
            DrawTriangle(c1, f3, f4, dangerFill);
            break;
        }

        case TelegraphShape::PROJECTILE_SPREAD: {
            // Precise trajectory aim lines for each projectile instead of confusing cone AoE
            float baseRad = enemy.telegraph.angle * DEG2RAD;
            int count = enemy.telegraph.spreadCount > 0 ? enemy.telegraph.spreadCount : 3;
            float step = enemy.telegraph.spreadAngle;

            for (int i = 0; i < count; i++) {
                float curAngle;
                if (count == 1) {
                    curAngle = baseRad;
                } else if (count == 12) {
                    // Full radial nova
                    curAngle = ((float)i * 30.0f) * DEG2RAD;
                } else {
                    // Centered fan spread
                    float offset = (float)i - (float)(count - 1) * 0.5f;
                    curAngle = baseRad + offset * step;
                }

                Vector2 dir = { cosf(curAngle), sinf(curAngle) };
                Vector2 start = { enemy.position.x + dir.x * (enemy.radius + 6.0f), enemy.position.y + dir.y * (enemy.radius + 6.0f) };
                Vector2 end = { start.x + dir.x * enemy.telegraph.radius, start.y + dir.y * enemy.telegraph.radius };

                Color laserCol = enemy.telegraph.warningColor;
                laserCol.a = (unsigned char)(100 * alphaProgress);
                DrawLineEx(start, end, 1.5f, laserCol);

                // Subtle aim guide dots along trajectory
                for (int d = 1; d <= 4; d++) {
                    float dotFrac = (float)d * 0.25f;
                    Vector2 midDot = { start.x + dir.x * (enemy.telegraph.radius * dotFrac), start.y + dir.y * (enemy.telegraph.radius * dotFrac) };
                    DrawCircleV(midDot, 1.5f, laserCol);
                }

                // Landing tinge where attack will land (no traveling orb)
                // Gets significantly brighter as the attack is about to be casted
                float progress = enemy.telegraph.progress;
                float brightness = progress * progress; // Quadratic flare right before release
                float tingeRadius = 12.0f + 8.0f * progress;

                Color tingeOuter = enemy.telegraph.warningColor;
                tingeOuter.a = 0;

                Color tingeGlow = enemy.telegraph.warningColor;
                tingeGlow.a = (unsigned char)(30 + 190 * brightness);

                // Soft ground gradient tinge where attack lands
                DrawCircleGradient(end, tingeRadius, tingeGlow, tingeOuter);
                DrawCircleLines((int)end.x, (int)end.y, tingeRadius * 0.7f, Color{ tingeGlow.r, tingeGlow.g, tingeGlow.b, (unsigned char)(50 + 190 * brightness) });

                // Core illumination that flares bright right before cast
                Color coreColor = Color{ 255, 255, 255, (unsigned char)(50 + 205 * brightness) };
                DrawCircleV(end, 2.0f + 3.0f * brightness, coreColor);
            }
            break;
        }
    }
}

void EnemyRenderer::Draw(const Enemy& enemy) {
    if (enemy.state == EnemyState::DEAD) return;

    Color bodyColor = Color{ 160, 40, 50, 255 };
    if (enemy.type == EnemyType::BRUTE) {
        bodyColor = Color{ 110, 30, 45, 255 };
    } else if (enemy.type == EnemyType::CHARGER) {
        bodyColor = Color{ 180, 80, 30, 255 };
    } else if (enemy.type == EnemyType::ARCHER) {
        bodyColor = Color{ 40, 110, 85, 255 }; // Forest stealth hunter
    } else if (enemy.type == EnemyType::PYROMANCER) {
        bodyColor = Color{ 95, 30, 140, 255 }; // Occult purple void
    } else if (enemy.type == EnemyType::INQUISITOR_BOSS) {
        bodyColor = Color{ 60, 20, 35, 255 }; // Imperial dark crimson & gold
    } else if (enemy.type == EnemyType::ZIL_BOSS) {
        bodyColor = Color{ 48, 54, 68, 255 }; // Corrupted knight steel
    } else if (enemy.type == EnemyType::KNIGHT_LENON_BOSS) {
        bodyColor = Color{ 32, 50, 85, 255 }; // Vanguard Crusader royal navy
    }

    bool isStunned = (enemy.state == EnemyState::STUNNED);
    AssetManager::DrawEnemySilhouette(enemy.position, enemy.facingAngle, bodyColor, enemy.radius, isStunned);

    // Boss special crown and aura
    if (enemy.type == EnemyType::INQUISITOR_BOSS) {
        float time = (float)GetTime() * 4.0f;
        float auraPulse = enemy.radius + 8.0f + 3.0f * sinf(time);
        DrawCircleLines((int)enemy.position.x, (int)enemy.position.y, auraPulse, Color{ 255, 215, 60, 140 });

        // Boss runic crown spikes
        float rad = enemy.facingAngle * DEG2RAD;
        Vector2 dir = { cosf(rad), sinf(rad) };
        Vector2 normal = { -dir.y, dir.x };
        Vector2 crownCenter = { enemy.position.x + dir.x * 8.0f, enemy.position.y + dir.y * 8.0f };
        DrawCircleV(crownCenter, 6.0f, Color{ 255, 215, 60, 255 });
        DrawCircleV({ crownCenter.x - normal.x * 9.0f, crownCenter.y - normal.y * 9.0f }, 4.0f, Color{ 255, 180, 40, 240 });
        DrawCircleV({ crownCenter.x + normal.x * 9.0f, crownCenter.y + normal.y * 9.0f }, 4.0f, Color{ 255, 180, 40, 240 });
    } else if (enemy.type == EnemyType::ZIL_BOSS) {
        float time = (float)GetTime() * 3.5f;
        float auraPulse = enemy.radius + 6.0f + 2.5f * sinf(time);
        DrawCircleLines((int)enemy.position.x, (int)enemy.position.y, auraPulse, Color{ 255, 140, 30, 160 });

        float rad = enemy.facingAngle * DEG2RAD;
        Vector2 dir = { cosf(rad), sinf(rad) };
        Vector2 normal = { -dir.y, dir.x };
        Vector2 swordHand = { enemy.position.x + normal.x * (enemy.radius * 0.75f), enemy.position.y + normal.y * (enemy.radius * 0.75f) };

        // Zil's Fragment wields Truth Seeker (Partial)!
        AssetManager::DrawSword(swordHand, enemy.facingAngle + 35.0f, 44.0f, Color{ 255, 235, 140, 255 }, Color{ 255, 170, 30, 220 });

        // Crusader shoulder pauldrons
        DrawCircleV({ enemy.position.x - normal.x * (enemy.radius * 0.8f), enemy.position.y - normal.y * (enemy.radius * 0.8f) }, 5.0f, Color{ 80, 85, 105, 255 });
        DrawCircleV({ enemy.position.x + normal.x * (enemy.radius * 0.8f), enemy.position.y + normal.y * (enemy.radius * 0.8f) }, 5.0f, Color{ 80, 85, 105, 255 });
    } else if (enemy.type == EnemyType::KNIGHT_LENON_BOSS) {
        float time = (float)GetTime() * 3.5f;
        float auraPulse = enemy.radius + 6.0f + 2.0f * sinf(time);
        DrawCircleLines((int)enemy.position.x, (int)enemy.position.y, auraPulse, Color{ 255, 215, 65, 160 });

        float rad = enemy.facingAngle * DEG2RAD;
        Vector2 dir = { cosf(rad), sinf(rad) };
        Vector2 normal = { -dir.y, dir.x };
        Vector2 swordHand = { enemy.position.x + normal.x * (enemy.radius * 0.75f), enemy.position.y + normal.y * (enemy.radius * 0.75f) };

        // Royal Greatsword
        AssetManager::DrawSword(swordHand, enemy.facingAngle + 35.0f, 46.0f, Color{ 220, 230, 245, 255 }, Color{ 225, 185, 65, 220 });

        // Golden pauldrons & Crusader visor
        DrawCircleV({ enemy.position.x - normal.x * (enemy.radius * 0.8f), enemy.position.y - normal.y * (enemy.radius * 0.8f) }, 6.0f, Color{ 225, 185, 65, 255 });
        DrawCircleV({ enemy.position.x + normal.x * (enemy.radius * 0.8f), enemy.position.y + normal.y * (enemy.radius * 0.8f) }, 6.0f, Color{ 225, 185, 65, 255 });
        DrawCircleV({ enemy.position.x + dir.x * (enemy.radius * 0.6f), enemy.position.y + dir.y * (enemy.radius * 0.6f) }, 4.0f, Color{ 255, 230, 90, 255 });
    }

    // Overhead Health Bar (for normal enemies; boss has master top bar)
    if (enemy.type != EnemyType::INQUISITOR_BOSS && enemy.type != EnemyType::ZIL_BOSS && enemy.type != EnemyType::KNIGHT_LENON_BOSS) {
        const float barWidth = enemy.radius * 2.2f;
        const float barHeight = 5.0f;
        float barX = enemy.position.x - barWidth * 0.5f;
        float barY = enemy.position.y - enemy.radius - 16.0f;

        DrawRectangle((int)barX, (int)barY, (int)barWidth, (int)barHeight, Color{ 20, 20, 26, 220 });
        float healthPercent = fmaxf(0.0f, enemy.health / enemy.maxHealth);
        Color hpColor = (enemy.state == EnemyState::STUNNED) ? Color{ 255, 220, 40, 255 } : Color{ 220, 50, 60, 255 };
        DrawRectangle((int)barX, (int)barY, (int)(barWidth * healthPercent), (int)barHeight, hpColor);
        DrawRectangleLines((int)barX, (int)barY, (int)barWidth, (int)barHeight, Color{ 40, 45, 60, 200 });
    }
}
