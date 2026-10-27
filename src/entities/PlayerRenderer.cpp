#include "PlayerRenderer.hpp"
#include "Player.hpp"
#include "../rendering/AssetManager.hpp"
#include "raymath.h"
#include <cmath>

void PlayerRenderer::Draw(const Player& player) {
    ArmorStats armorStats = GetArmorStats(player.currentArmor);
    Color bodyColor = armorStats.primaryColor;
    if (player.isInPhlegmaticTrance) bodyColor = Color{ 160, 230, 255, 255 };
    else if (player.counterBuffTimer > 0.0f) bodyColor = Color{ 220, 140, 40, 255 };
    else if (player.isDashing) bodyColor = Color{ 100, 200, 255, 180 };
    else if (player.staggerTimer > 0.0f) bodyColor = Color{ 240, 180, 60, 240 };

    // Phlegmatic Composure Visual Effects
    if (player.IsInPhlegmaticTrance()) {
        float pulse = (sinf((float)GetTime() * 8.0f) + 1.0f) * 0.5f;
        // Radiant Ice Aura
        DrawCircleGradient(player.position, player.radius + 22.0f, Color{ 140, 230, 255, (unsigned char)(60 + 50 * pulse) }, Color{ 80, 180, 255, 0 });
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 14.0f + 4.0f * pulse, Color{ 160, 240, 255, 200 });
        // Orbital frost sparks
        for (int i = 0; i < 3; ++i) {
            float orbAngle = (float)GetTime() * 4.0f + i * (2.0f * 3.14159265f / 3.0f);
            float orbDist = player.radius + 18.0f;
            Vector2 orbPos = { player.position.x + cosf(orbAngle) * orbDist, player.position.y + sinf(orbAngle) * orbDist };
            DrawCircle((int)orbPos.x, (int)orbPos.y, 3.5f, Color{ 220, 250, 255, 255 });
            DrawCircleLines((int)orbPos.x, (int)orbPos.y, 5.0f, Color{ 100, 220, 255, 200 });
        }
    } else if (player.HasStoicPoise()) {
        // Subtle ice-blue poise halo
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 5.0f, Color{ 90, 200, 255, 140 });
    } else if (player.IsDiscomposed()) {
        // Subtle amber discomposed jitter ring
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 4.0f, Color{ 240, 140, 60, 140 });
    }

    AssetManager::DrawPlayerSilhouette(player.position, player.facingAngle, bodyColor, player.isParrying, player.isDashing, armorStats.trimColor, armorStats.helmColor, armorStats.visorColor);

    Vector2 dir = { cosf(player.facingAngle * DEG2RAD), sinf(player.facingAngle * DEG2RAD) };
    Vector2 normal = { -dir.y, dir.x };

    Vector2 rightHand = {
        player.position.x + dir.x * 12.0f + normal.x * 11.0f,
        player.position.y + dir.y * 12.0f + normal.y * 11.0f
    };
    Vector2 leftHand = {
        player.position.x + dir.x * 12.0f - normal.x * 11.0f,
        player.position.y + dir.y * 12.0f - normal.y * 11.0f
    };

    if (player.currentWeapon == WeaponType::SWORD) {
        float swordAngle = player.facingAngle + 35.0f;
        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            swordAngle = player.facingAngle - 50.0f + progress * player.currentAttack.arcAngle;
            AssetManager::DrawSword(rightHand, swordAngle, 38.0f, Color{ 230, 240, 255, 255 }, Color{ 0, 229, 255, 200 });

            float startAngle = player.facingAngle - 50.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 0, 220, 255, 60 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 120, 245, 255, 180 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            if (player.heavyStage == 2) {
                // Stage 2: Upward shock slash (LINE / BOX AoE)
                swordAngle = player.facingAngle + 60.0f - progress * 120.0f;
                AssetManager::DrawSword(rightHand, swordAngle, 46.0f, Color{ 255, 245, 200, 255 }, Color{ 255, 200, 50, 240 });

                float rad = player.facingAngle * DEG2RAD;
                Vector2 dir = { cosf(rad), sinf(rad) };
                Vector2 normal = { -dir.y, dir.x };
                float halfWidth = (player.currentAttack.width > 0.0f ? player.currentAttack.width : 50.0f) * 0.5f;
                float length = player.currentAttack.range;

                Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };
                Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };
                Vector2 c3 = { c2.x + dir.x * length, c2.y + dir.y * length };
                Vector2 c4 = { c1.x + dir.x * length, c1.y + dir.y * length };

                // Filled luminous box and crisp shockwave outlines
                DrawTriangle(c1, c2, c3, Color{ 255, 190, 50, (unsigned char)(80 * (1.0f - progress * 0.5f)) });
                DrawTriangle(c1, c3, c4, Color{ 255, 190, 50, (unsigned char)(80 * (1.0f - progress * 0.5f)) });
                DrawLineEx(c1, c2, 2.5f, Color{ 255, 230, 110, 230 });
                DrawLineEx(c2, c3, 2.5f, Color{ 255, 230, 110, 230 });
                DrawLineEx(c3, c4, 2.5f, Color{ 255, 230, 110, 230 });
                DrawLineEx(c4, c1, 2.5f, Color{ 255, 230, 110, 230 });

                // Central upward shock slash line
                Vector2 beamEnd = { player.position.x + dir.x * length * progress, player.position.y + dir.y * length * progress };
                DrawLineEx(player.position, beamEnd, 4.0f, Color{ 255, 250, 180, 255 });
            } else {
                // Stage 1: Wide cleave
                swordAngle = player.facingAngle - 80.0f + progress * player.currentAttack.arcAngle;
                AssetManager::DrawSword(rightHand, swordAngle, 50.0f, Color{ 255, 230, 170, 255 }, Color{ 255, 170, 40, 220 });

                float startAngle = player.facingAngle - 80.0f;
                float sweepAngle = progress * player.currentAttack.arcAngle;
                DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 255, 170, 40, 80 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 255, 225, 100, 220 });
            }
        } else if (player.isHeavyWindingUp) {
            swordAngle = player.facingAngle - 85.0f;
            AssetManager::DrawSword(rightHand, swordAngle, 42.0f, Color{ 255, 200, 80, 255 }, Color{ 255, 120, 20, 200 });
        } else {
            AssetManager::DrawSword(rightHand, swordAngle, 34.0f, Color{ 210, 220, 235, 255 }, Color{ 80, 140, 200, 120 });
        }
    } else if (player.currentWeapon == WeaponType::DAGGERS) {
        float angleL = player.facingAngle - 25.0f;
        float angleR = player.facingAngle + 25.0f;

        if (player.isDaggerLunging) {
            // Forward twin thrust blades and piercing energy slash during lunge
            angleL = player.facingAngle - 15.0f;
            angleR = player.facingAngle + 15.0f;
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 160, 255, 210, 255 }, Color{ 80, 255, 170, 240 });

            float rad = player.facingAngle * DEG2RAD;
            Vector2 dir = { cosf(rad), sinf(rad) };
            Vector2 normal = { -dir.y, dir.x };
            float halfWidth = 24.0f;
            float lungeRange = 85.0f;

            // Piercing forward wind-cut box
            Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };
            Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };
            Vector2 c3 = { c2.x + dir.x * lungeRange, c2.y + dir.y * lungeRange };
            Vector2 c4 = { c1.x + dir.x * lungeRange, c1.y + dir.y * lungeRange };

            DrawTriangle(c1, c2, c3, Color{ 80, 255, 170, 55 });
            DrawTriangle(c1, c3, c4, Color{ 80, 255, 170, 55 });
            DrawLineEx(c1, c4, 2.0f, Color{ 120, 255, 190, 220 });
            DrawLineEx(c2, c3, 2.0f, Color{ 120, 255, 190, 220 });
            DrawLineEx(c4, c3, 2.5f, Color{ 160, 255, 210, 240 });

            // Trailing dash lines
            Vector2 backDir = { -dir.x, -dir.y };
            DrawLineEx(player.position, { player.position.x + backDir.x * 45.0f, player.position.y + backDir.y * 45.0f }, 3.5f, Color{ 100, 255, 180, 200 });
            DrawLineEx({ player.position.x - normal.x * 12.0f, player.position.y - normal.y * 12.0f },
                       { player.position.x + backDir.x * 35.0f - normal.x * 12.0f, player.position.y + backDir.y * 35.0f - normal.y * 12.0f }, 2.0f, Color{ 80, 240, 160, 150 });
            DrawLineEx({ player.position.x + normal.x * 12.0f, player.position.y + normal.y * 12.0f },
                       { player.position.x + backDir.x * 35.0f + normal.x * 12.0f, player.position.y + backDir.y * 35.0f + normal.y * 12.0f }, 2.0f, Color{ 80, 240, 160, 150 });
        } else if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            angleR = player.facingAngle - 30.0f + progress * 80.0f;
            angleL = player.facingAngle + 30.0f - progress * 80.0f;

            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 180, 255, 220, 255 }, Color{ 60, 240, 180, 200 });

            float startAngle = player.facingAngle - 45.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 14, Color{ 60, 240, 180, 60 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 14, Color{ 120, 255, 200, 180 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            // 360 Whirlwind spin
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            float spinAngle = player.facingAngle + progress * 360.0f;
            angleR = spinAngle;
            angleL = spinAngle + 180.0f;

            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 220, 255, 200, 255 }, Color{ 80, 255, 160, 220 });
            DrawCircleLines((int)player.position.x, (int)player.position.y, player.currentAttack.range, Color{ 80, 255, 160, 180 });
            DrawCircle((int)player.position.x, (int)player.position.y, player.currentAttack.range * progress, Color{ 60, 240, 180, 40 });
        } else {
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 180, 240, 220, 255 }, Color{ 60, 200, 160, 140 });
        }
    } else if (player.currentWeapon == WeaponType::MACE) {
        float maceAngle = player.facingAngle + 40.0f;
        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            maceAngle = player.facingAngle - 30.0f + progress * 90.0f;
            AssetManager::DrawMace(rightHand, maceAngle, 44.0f, Color{ 240, 190, 130, 255 }, Color{ 255, 150, 50, 220 });

            // Frontal Circular Smack Area
            DrawCircleV(player.currentAttack.circleCenter, player.currentAttack.circleRadius * (0.6f + 0.4f * progress), Color{ 255, 160, 50, (unsigned char)(70 * (1.0f - progress)) });
            DrawCircleLines((int)player.currentAttack.circleCenter.x, (int)player.currentAttack.circleCenter.y, player.currentAttack.circleRadius, Color{ 255, 190, 80, 200 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            maceAngle = player.facingAngle - 80.0f + progress * 160.0f;
            AssetManager::DrawMace(rightHand, maceAngle, 54.0f, Color{ 255, 200, 140, 255 }, Color{ 255, 100, 30, 240 });

            if (player.heavyStage == 1 && player.currentAttack.isFrontalCircle) {
                // Giant Frontal Smack Area
                DrawCircleV(player.currentAttack.circleCenter, player.currentAttack.circleRadius * (0.6f + 0.4f * progress), Color{ 255, 120, 30, (unsigned char)(90 * (1.0f - progress)) });
                DrawCircleLines((int)player.currentAttack.circleCenter.x, (int)player.currentAttack.circleCenter.y, player.currentAttack.circleRadius, Color{ 255, 180, 60, 230 });
            } else {
                // Stage 2: Earthshattering 360 slam shockwave
                DrawCircleLines((int)player.position.x, (int)player.position.y, player.currentAttack.range * progress, Color{ 255, 140, 40, 220 });
                DrawCircle((int)player.position.x, (int)player.position.y, player.currentAttack.range * progress, Color{ 255, 90, 20, 50 });
            }
        } else if (player.isHeavyWindingUp) {
            maceAngle = player.facingAngle - 100.0f;
            AssetManager::DrawMace(rightHand, maceAngle, 48.0f, Color{ 255, 160, 60, 255 }, Color{ 255, 60, 20, 220 });
        } else {
            AssetManager::DrawMace(rightHand, maceAngle, 38.0f, Color{ 210, 170, 130, 255 }, Color{ 200, 120, 50, 120 });
        }
    } else if (player.currentWeapon == WeaponType::TRUTH_SEEKER) {
        float swordAngle = player.facingAngle + 35.0f;
        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);

            if (player.comboStep == 2) {
                // 2nd Light Attack: Upward radiant slice (LINE / BOX AoE)
                swordAngle = player.facingAngle + 50.0f - progress * 100.0f;
                AssetManager::DrawSword(rightHand, swordAngle, 44.0f, Color{ 210, 245, 255, 255 }, Color{ 60, 220, 255, 250 });

                float rad = player.facingAngle * DEG2RAD;
                Vector2 dir = { cosf(rad), sinf(rad) };
                Vector2 normal = { -dir.y, dir.x };
                float halfWidth = (player.currentAttack.width > 0.0f ? player.currentAttack.width : 40.0f) * 0.5f;
                float length = player.currentAttack.range;

                Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };
                Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };
                Vector2 c3 = { c2.x + dir.x * length, c2.y + dir.y * length };
                Vector2 c4 = { c1.x + dir.x * length, c1.y + dir.y * length };

                // Filled radiant azure box and crisp outlines
                DrawTriangle(c1, c2, c3, Color{ 60, 220, 255, (unsigned char)(75 * (1.0f - progress * 0.5f)) });
                DrawTriangle(c1, c3, c4, Color{ 60, 220, 255, (unsigned char)(75 * (1.0f - progress * 0.5f)) });
                DrawLineEx(c1, c2, 2.5f, Color{ 140, 245, 255, 230 });
                DrawLineEx(c2, c3, 2.5f, Color{ 140, 245, 255, 230 });
                DrawLineEx(c3, c4, 2.5f, Color{ 140, 245, 255, 230 });
                DrawLineEx(c4, c1, 2.5f, Color{ 140, 245, 255, 230 });

                // Central radiant upward slice line
                Vector2 beamEnd = { player.position.x + dir.x * length * progress, player.position.y + dir.y * length * progress };
                DrawLineEx(player.position, beamEnd, 3.5f, Color{ 230, 255, 255, 255 });
            } else {
                // 1st Light Attack: Downward golden sweep
                swordAngle = player.facingAngle - 55.0f + progress * player.currentAttack.arcAngle;
                AssetManager::DrawSword(rightHand, swordAngle, 46.0f, Color{ 255, 235, 140, 255 }, Color{ 255, 210, 50, 240 });

                float startAngle = player.facingAngle - 55.0f;
                float sweepAngle = progress * player.currentAttack.arcAngle;
                DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 18, Color{ 255, 215, 60, 75 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 18, Color{ 255, 240, 120, 220 });
            }
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            swordAngle = player.facingAngle + progress * 360.0f;
            AssetManager::DrawSword(rightHand, swordAngle, 56.0f, Color{ 255, 245, 180, 255 }, Color{ 255, 180, 30, 255 });

            // Full 360 circle AoE swing
            DrawCircleLines((int)player.position.x, (int)player.position.y, player.currentAttack.range, Color{ 255, 220, 70, 230 });
            DrawCircle((int)player.position.x, (int)player.position.y, player.currentAttack.range * (0.4f + 0.6f * progress), Color{ 255, 190, 40, (unsigned char)(60 * (1.0f - progress)) });
        } else if (player.isHeavyWindingUp) {
            swordAngle = player.facingAngle - 95.0f;
            AssetManager::DrawSword(rightHand, swordAngle, 52.0f, Color{ 255, 220, 80, 255 }, Color{ 255, 140, 10, 240 });

            // Glowing holy charging aura
            float pulse = 0.5f + 0.5f * sinf((float)GetTime() * 12.0f);
            DrawCircleLines((int)player.position.x, (int)player.position.y, 45.0f + pulse * 8.0f, Color{ 255, 215, 60, 220 });
            DrawCircle((int)player.position.x, (int)player.position.y, 20.0f, Color{ 255, 180, 40, (unsigned char)(100 * pulse) });
        } else {
            AssetManager::DrawSword(rightHand, swordAngle, 42.0f, Color{ 255, 225, 120, 255 }, Color{ 255, 190, 40, 180 });
        }

        // Draw active shockwave ring if expanding
        if (player.shockwaveActive) {
            float shockProgress = 1.0f - (player.shockwaveTimer / 0.38f);
            float curRadius = player.shockwaveRadius * shockProgress;
            unsigned char alpha = (unsigned char)(255 * (1.0f - shockProgress));
            DrawCircleLines((int)player.shockwavePos.x, (int)player.shockwavePos.y, curRadius, Color{ 255, 180, 50, alpha });
            DrawCircle((int)player.shockwavePos.x, (int)player.shockwavePos.y, curRadius * 0.4f, Color{ 255, 140, 40, (unsigned char)(alpha * 0.25f) });
        }
    }
}
