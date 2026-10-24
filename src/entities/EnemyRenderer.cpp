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

    float rad = enemy.facingAngle * DEG2RAD;
    Vector2 dir = { cosf(rad), sinf(rad) };
    Vector2 normal = { -dir.y, dir.x };
    Vector2 rightHand = {
        enemy.position.x + normal.x * (enemy.radius * 0.75f) + dir.x * 4.0f,
        enemy.position.y + normal.y * (enemy.radius * 0.75f) + dir.y * 4.0f
    };

    // Enemy Attack Visuals & Weapon Animations
    if (enemy.type == EnemyType::SLASHER) {
        if (enemy.state == EnemyState::STRIKE) {
            float progress = 1.0f - (enemy.stateTimer / enemy.strikeDuration);
            progress = fmaxf(0.0f, fminf(1.0f, progress));
            float startArc = enemy.facingAngle - 65.0f;
            float sweepAngle = progress * 135.0f;
            float swordAngle = startArc + sweepAngle;

            // Crimson slashing arc & glowing edge
            DrawCircleSector(enemy.position, enemy.attackRange, startArc, startArc + sweepAngle, 16, Color{ 230, 40, 50, (unsigned char)(75 * (1.0f - progress * 0.35f)) });
            DrawCircleSectorLines(enemy.position, enemy.attackRange, startArc, startArc + sweepAngle, 16, Color{ 255, 110, 110, 220 });

            // Curved cultist scythe blade
            AssetManager::DrawSword(rightHand, swordAngle, 36.0f, Color{ 240, 50, 60, 255 }, Color{ 255, 90, 90, 230 });
        } else if (enemy.state == EnemyState::TELEGRAPH_WINDUP) {
            float progress = 1.0f - (enemy.stateTimer / enemy.windupDuration);
            float windupAngle = enemy.facingAngle - 45.0f - progress * 25.0f;
            AssetManager::DrawSword(rightHand, windupAngle, 34.0f, Color{ 200, 40, 50, 255 }, Color{ 255, 70, 70, 190 });
            Vector2 bladeTip = { rightHand.x + cosf(windupAngle * DEG2RAD) * 32.0f, rightHand.y + sinf(windupAngle * DEG2RAD) * 32.0f };
            DrawCircleV(bladeTip, 3.5f + 2.5f * progress, Color{ 255, 120, 80, 240 });
        } else {
            AssetManager::DrawSword(rightHand, enemy.facingAngle + 35.0f, 30.0f, Color{ 170, 45, 55, 255 }, Color{ 110, 25, 35, 200 });
        }
    } else if (enemy.type == EnemyType::BRUTE) {
        if (enemy.state == EnemyState::STRIKE) {
            float progress = 1.0f - (enemy.stateTimer / enemy.strikeDuration);
            progress = fmaxf(0.0f, fminf(1.0f, progress));
            float smashAngle = enemy.facingAngle - 85.0f + progress * 115.0f;
            AssetManager::DrawMace(rightHand, smashAngle, 42.0f, Color{ 150, 155, 170, 255 }, Color{ 220, 50, 50, 240 });

            // Expanding ground shockwaves & seismic fractures
            Vector2 impactPos = { enemy.position.x + dir.x * (enemy.radius + 18.0f), enemy.position.y + dir.y * (enemy.radius + 18.0f) };
            float shockR = enemy.attackRange * progress;
            DrawCircleLines((int)impactPos.x, (int)impactPos.y, shockR, Color{ 255, 60, 60, (unsigned char)(230 * (1.0f - progress)) });
            DrawCircleLines((int)impactPos.x, (int)impactPos.y, shockR * 0.7f, Color{ 255, 140, 60, (unsigned char)(190 * (1.0f - progress)) });
            for (int k = 0; k < 6; k++) {
                float fa = ((float)k * 60.0f) * DEG2RAD;
                DrawLineEx(impactPos, { impactPos.x + cosf(fa) * (shockR * 0.9f), impactPos.y + sinf(fa) * (shockR * 0.9f) }, 2.0f, Color{ 255, 100, 60, (unsigned char)(180 * (1.0f - progress)) });
            }
        } else if (enemy.state == EnemyState::TELEGRAPH_WINDUP) {
            float progress = 1.0f - (enemy.stateTimer / enemy.windupDuration);
            float windupAngle = enemy.facingAngle - 85.0f;
            AssetManager::DrawMace(rightHand, windupAngle, 40.0f, Color{ 130, 135, 150, 255 }, Color{ 200, 40, 40, 200 });
            Vector2 maceHead = { rightHand.x + cosf(windupAngle * DEG2RAD) * 36.0f, rightHand.y + sinf(windupAngle * DEG2RAD) * 36.0f };
            DrawCircleV(maceHead, 4.5f + 3.0f * progress, Color{ 255, 70, 70, 220 });
        } else {
            AssetManager::DrawMace(rightHand, enemy.facingAngle + 45.0f, 36.0f, Color{ 110, 115, 130, 255 }, Color{ 60, 25, 30, 200 });
        }
    } else if (enemy.type == EnemyType::CHARGER) {
        if (enemy.state == EnemyState::STRIKE) {
            float progress = 1.0f - (enemy.stateTimer / enemy.strikeDuration);
            progress = fmaxf(0.0f, fminf(1.0f, progress));
            Vector2 thrustHand = { rightHand.x + dir.x * (progress * 26.0f), rightHand.y + dir.y * (progress * 26.0f) };
            DrawLineEx(thrustHand, { thrustHand.x + dir.x * 46.0f, thrustHand.y + dir.y * 46.0f }, 4.0f, Color{ 255, 210, 120, 255 });
            DrawCircleV({ thrustHand.x + dir.x * 48.0f, thrustHand.y + dir.y * 48.0f }, 4.5f, Color{ 255, 240, 180, 255 });

            // Trailing speedlines
            DrawLineEx({ enemy.position.x - normal.x * 12.0f, enemy.position.y - normal.y * 12.0f },
                       { enemy.position.x - normal.x * 16.0f - dir.x * 32.0f, enemy.position.y - normal.y * 16.0f - dir.y * 32.0f }, 2.0f, Color{ 255, 150, 50, 170 });
            DrawLineEx({ enemy.position.x + normal.x * 12.0f, enemy.position.y + normal.y * 12.0f },
                       { enemy.position.x + normal.x * 16.0f - dir.x * 32.0f, enemy.position.y + normal.y * 16.0f - dir.y * 32.0f }, 2.0f, Color{ 255, 150, 50, 170 });
        } else if (enemy.state == EnemyState::TELEGRAPH_WINDUP) {
            DrawLineEx(rightHand, { rightHand.x + dir.x * 38.0f, rightHand.y + dir.y * 38.0f }, 3.0f, Color{ 230, 150, 60, 255 });
            DrawCircleV({ rightHand.x + dir.x * 40.0f, rightHand.y + dir.y * 40.0f }, 4.0f, Color{ 255, 190, 80, 255 });
        } else {
            DrawLineEx(rightHand, { rightHand.x + dir.x * 32.0f + normal.x * 8.0f, rightHand.y + dir.y * 32.0f + normal.y * 8.0f }, 2.5f, Color{ 180, 110, 40, 255 });
        }
    } else if (enemy.type == EnemyType::ARCHER) {
        Vector2 bowCenter = { enemy.position.x + dir.x * (enemy.radius + 6.0f), enemy.position.y + dir.y * (enemy.radius + 6.0f) };
        Vector2 bowTip1 = { bowCenter.x - normal.x * 14.0f, bowCenter.y - normal.y * 14.0f };
        Vector2 bowTip2 = { bowCenter.x + normal.x * 14.0f, bowCenter.y + normal.y * 14.0f };

        if (enemy.state == EnemyState::TELEGRAPH_WINDUP) {
            float progress = 1.0f - (enemy.stateTimer / enemy.windupDuration);
            Vector2 stringPull = { bowCenter.x - dir.x * (6.0f + 10.0f * progress), bowCenter.y - dir.y * (6.0f + 10.0f * progress) };
            DrawLineEx(bowTip1, bowCenter, 2.5f, Color{ 145, 105, 65, 255 });
            DrawLineEx(bowCenter, bowTip2, 2.5f, Color{ 145, 105, 65, 255 });
            DrawLineEx(bowTip1, stringPull, 1.5f, Color{ 240, 240, 240, 220 });
            DrawLineEx(bowTip2, stringPull, 1.5f, Color{ 240, 240, 240, 220 });
            // Nocked glowing arrow
            DrawLineEx(stringPull, { bowCenter.x + dir.x * 18.0f, bowCenter.y + dir.y * 18.0f }, 2.0f, Color{ 255, 180, 60, 255 });
            DrawCircleV({ bowCenter.x + dir.x * 18.0f, bowCenter.y + dir.y * 18.0f }, 3.5f, Color{ 255, 230, 120, 255 });
        } else {
            DrawLineEx(bowTip1, bowCenter, 2.0f, Color{ 120, 85, 50, 255 });
            DrawLineEx(bowCenter, bowTip2, 2.0f, Color{ 120, 85, 50, 255 });
            DrawLineEx(bowTip1, bowTip2, 1.0f, Color{ 200, 200, 200, 160 });
        }
    } else if (enemy.type == EnemyType::PYROMANCER) {
        // Occult void staff
        Vector2 staffBase = { enemy.position.x - dir.x * 6.0f + normal.x * 10.0f, enemy.position.y - dir.y * 6.0f + normal.y * 10.0f };
        Vector2 staffHead = { enemy.position.x + dir.x * 24.0f + normal.x * 10.0f, enemy.position.y + dir.y * 24.0f + normal.y * 10.0f };
        DrawLineEx(staffBase, staffHead, 3.0f, Color{ 65, 45, 75, 255 });

        float pulse = 0.5f + 0.5f * sinf((float)GetTime() * 6.0f);
        if (enemy.state == EnemyState::TELEGRAPH_WINDUP) {
            float progress = 1.0f - (enemy.stateTimer / enemy.windupDuration);
            DrawCircleV(staffHead, 6.0f + 4.0f * progress, Color{ 190, 70, 255, 255 });
            DrawCircleGradient(staffHead, 18.0f * (1.0f + progress), Color{ 170, 50, 240, 140 }, Color{ 0, 0, 0, 0 });
        } else {
            DrawCircleV(staffHead, 4.5f + 1.5f * pulse, Color{ 160, 50, 220, 255 });
        }
    }

    // Boss special crown and aura
    if (enemy.type == EnemyType::INQUISITOR_BOSS) {
        float time = (float)GetTime() * 4.0f;
        float auraPulse = enemy.radius + 8.0f + 3.0f * sinf(time);
        DrawCircleLines((int)enemy.position.x, (int)enemy.position.y, auraPulse, Color{ 255, 215, 60, 140 });

        // Boss runic crown spikes
        Vector2 crownCenter = { enemy.position.x + dir.x * 8.0f, enemy.position.y + dir.y * 8.0f };
        DrawCircleV(crownCenter, 6.0f, Color{ 255, 215, 60, 255 });
        DrawCircleV({ crownCenter.x - normal.x * 9.0f, crownCenter.y - normal.y * 9.0f }, 4.0f, Color{ 255, 180, 40, 240 });
        DrawCircleV({ crownCenter.x + normal.x * 9.0f, crownCenter.y + normal.y * 9.0f }, 4.0f, Color{ 255, 180, 40, 240 });
    } else if (enemy.type == EnemyType::ZIL_BOSS) {
        float time = (float)GetTime() * 3.5f;
        float auraPulse = enemy.radius + 6.0f + 2.5f * sinf(time);
        DrawCircleLines((int)enemy.position.x, (int)enemy.position.y, auraPulse, Color{ 255, 140, 30, 160 });

        Vector2 swordHand = { enemy.position.x + normal.x * (enemy.radius * 0.75f), enemy.position.y + normal.y * (enemy.radius * 0.75f) };

        // Animated Zil Boss Attack Visuals (Truth Seeker)
        if (enemy.state == EnemyState::STRIKE) {
            float progress = 1.0f - (enemy.stateTimer / enemy.strikeDuration);
            progress = fmaxf(0.0f, fminf(1.0f, progress));

            // Upward Radiant Slice & Dawnfire Arc
            float startA = enemy.facingAngle - 70.0f;
            float arcLen = progress * 140.0f;
            DrawCircleSector(enemy.position, 175.0f, startA, startA + arcLen, 20, Color{ 255, 215, 80, (unsigned char)(80 * (1.0f - progress * 0.4f)) });
            DrawCircleSectorLines(enemy.position, 175.0f, startA, startA + arcLen, 20, Color{ 255, 240, 150, 230 });
            float swordA = startA + arcLen;
            AssetManager::DrawSword(swordHand, swordA, 48.0f, Color{ 255, 240, 160, 255 }, Color{ 255, 180, 40, 230 });
        } else if (enemy.state == EnemyState::TELEGRAPH_WINDUP) {
            float progress = 1.0f - (enemy.stateTimer / enemy.windupDuration);
            float windupAngle = enemy.facingAngle - 60.0f;
            AssetManager::DrawSword(swordHand, windupAngle, 46.0f, Color{ 255, 235, 140, 255 }, Color{ 255, 170, 30, 220 });
            Vector2 tip = { swordHand.x + cosf(windupAngle * DEG2RAD) * 44.0f, swordHand.y + sinf(windupAngle * DEG2RAD) * 44.0f };
            DrawCircleV(tip, 5.0f + 3.0f * progress, Color{ 255, 220, 100, 240 });
        } else {
            AssetManager::DrawSword(swordHand, enemy.facingAngle + 35.0f, 44.0f, Color{ 255, 235, 140, 255 }, Color{ 255, 170, 30, 220 });
        }

        // Crusader shoulder pauldrons
        DrawCircleV({ enemy.position.x - normal.x * (enemy.radius * 0.8f), enemy.position.y - normal.y * (enemy.radius * 0.8f) }, 5.0f, Color{ 80, 85, 105, 255 });
        DrawCircleV({ enemy.position.x + normal.x * (enemy.radius * 0.8f), enemy.position.y + normal.y * (enemy.radius * 0.8f) }, 5.0f, Color{ 80, 85, 105, 255 });
    } else if (enemy.type == EnemyType::KNIGHT_LENON_BOSS) {
        float time = (float)GetTime() * 3.5f;
        float auraPulse = enemy.radius + 6.0f + 2.0f * sinf(time);
        DrawCircleLines((int)enemy.position.x, (int)enemy.position.y, auraPulse, Color{ 255, 215, 65, 160 });

        Vector2 swordHand = { enemy.position.x + normal.x * (enemy.radius * 0.75f), enemy.position.y + normal.y * (enemy.radius * 0.75f) };

        // Animated Knight Lenon Attack Visuals (Royal Greatsword)
        if (enemy.state == EnemyState::STRIKE) {
            float progress = 1.0f - (enemy.stateTimer / enemy.strikeDuration);
            progress = fmaxf(0.0f, fminf(1.0f, progress));

            // Royal Greatsword Cleave Arc & Golden Crescent
            float startA = enemy.facingAngle - 65.0f;
            float arcLen = progress * 135.0f;
            DrawCircleSector(enemy.position, 160.0f, startA, startA + arcLen, 20, Color{ 255, 215, 65, (unsigned char)(75 * (1.0f - progress * 0.4f)) });
            DrawCircleSectorLines(enemy.position, 160.0f, startA, startA + arcLen, 20, Color{ 255, 235, 120, 230 });
            float swordA = startA + arcLen;
            AssetManager::DrawSword(swordHand, swordA, 50.0f, Color{ 230, 240, 255, 255 }, Color{ 225, 185, 65, 230 });
        } else if (enemy.state == EnemyState::TELEGRAPH_WINDUP) {
            float progress = 1.0f - (enemy.stateTimer / enemy.windupDuration);
            float windupAngle = enemy.facingAngle - 55.0f;
            AssetManager::DrawSword(swordHand, windupAngle, 48.0f, Color{ 220, 230, 245, 255 }, Color{ 225, 185, 65, 220 });
            Vector2 tip = { swordHand.x + cosf(windupAngle * DEG2RAD) * 46.0f, swordHand.y + sinf(windupAngle * DEG2RAD) * 46.0f };
            DrawCircleV(tip, 5.0f + 3.0f * progress, Color{ 255, 230, 100, 240 });
        } else {
            AssetManager::DrawSword(swordHand, enemy.facingAngle + 35.0f, 46.0f, Color{ 220, 230, 245, 255 }, Color{ 225, 185, 65, 220 });
        }

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
