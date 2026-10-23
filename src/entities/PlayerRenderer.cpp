#include "PlayerRenderer.hpp"
#include "Player.hpp"
#include "../rendering/AssetManager.hpp"
#include "raymath.h"
#include <cmath>

void PlayerRenderer::Draw(const Player& player) {
    Color bodyColor = Color{ 40, 110, 210, 255 };
    if (player.counterBuffTimer > 0.0f) bodyColor = Color{ 220, 140, 40, 255 };
    if (player.isDashing) bodyColor = Color{ 100, 200, 255, 180 };

    AssetManager::DrawPlayerSilhouette(player.position, player.facingAngle, bodyColor, player.isParrying, player.isDashing);

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
            swordAngle = player.facingAngle - 60.0f + progress * player.currentAttack.arcAngle;
            AssetManager::DrawSword(rightHand, swordAngle, 48.0f, Color{ 255, 240, 190, 255 }, Color{ 255, 180, 40, 220 });

            float startAngle = player.facingAngle - 60.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 180, 40, 80 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 230, 100, 220 });
        } else if (player.isHeavyWindingUp) {
            swordAngle = player.facingAngle - 85.0f;
            AssetManager::DrawSword(rightHand, swordAngle, 42.0f, Color{ 255, 200, 80, 255 }, Color{ 255, 120, 20, 200 });
        } else {
            AssetManager::DrawSword(rightHand, swordAngle, 34.0f, Color{ 210, 220, 235, 255 }, Color{ 80, 140, 200, 120 });
        }
    } else if (player.currentWeapon == WeaponType::DAGGERS) {
        float angleL = player.facingAngle - 25.0f;
        float angleR = player.facingAngle + 25.0f;

        if (player.currentAttack.type == AttackType::LIGHT) {
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
        } else if (player.isHeavyWindingUp) {
            angleL = player.facingAngle - 70.0f;
            angleR = player.facingAngle - 50.0f;
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 220, 255, 140, 255 }, Color{ 140, 240, 60, 200 });
        } else {
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 180, 240, 220, 255 }, Color{ 60, 200, 160, 140 });
        }
    } else if (player.currentWeapon == WeaponType::MACE) {
        float maceAngle = player.facingAngle + 40.0f;
        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            maceAngle = player.facingAngle - 40.0f + progress * player.currentAttack.arcAngle;
            AssetManager::DrawMace(rightHand, maceAngle, 42.0f, Color{ 230, 180, 120, 255 }, Color{ 255, 140, 40, 200 });

            float startAngle = player.facingAngle - 40.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 255, 140, 40, 60 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 255, 180, 80, 180 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            maceAngle = player.facingAngle - 70.0f + progress * player.currentAttack.arcAngle;
            AssetManager::DrawMace(rightHand, maceAngle, 52.0f, Color{ 255, 200, 140, 255 }, Color{ 255, 90, 30, 240 });

            // Earthshattering slam shockwave
            float startAngle = player.facingAngle - 70.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 90, 30, 80 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 140, 50, 220 });
        } else if (player.isHeavyWindingUp) {
            maceAngle = player.facingAngle - 100.0f;
            AssetManager::DrawMace(rightHand, maceAngle, 46.0f, Color{ 255, 160, 60, 255 }, Color{ 255, 60, 20, 220 });
        } else {
            AssetManager::DrawMace(rightHand, maceAngle, 38.0f, Color{ 210, 170, 130, 255 }, Color{ 200, 120, 50, 120 });
        }
    } else if (player.currentWeapon == WeaponType::TRUTH_SEEKER) {
        float swordAngle = player.facingAngle + 35.0f;
        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            swordAngle = player.facingAngle - 55.0f + progress * player.currentAttack.arcAngle;
            AssetManager::DrawSword(rightHand, swordAngle, 46.0f, Color{ 255, 235, 140, 255 }, Color{ 255, 210, 50, 240 });

            float startAngle = player.facingAngle - 55.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 18, Color{ 255, 215, 60, 75 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 18, Color{ 255, 240, 120, 220 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            swordAngle = player.facingAngle - 70.0f + progress * player.currentAttack.arcAngle;
            AssetManager::DrawSword(rightHand, swordAngle, 56.0f, Color{ 255, 245, 180, 255 }, Color{ 255, 170, 20, 255 });

            float startAngle = player.facingAngle - 70.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 255, 180, 30, 90 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 255, 235, 100, 240 });
        } else if (player.isHeavyWindingUp) {
            swordAngle = player.facingAngle - 90.0f;
            AssetManager::DrawSword(rightHand, swordAngle, 50.0f, Color{ 255, 220, 80, 255 }, Color{ 255, 140, 10, 240 });
        } else {
            AssetManager::DrawSword(rightHand, swordAngle, 42.0f, Color{ 255, 225, 120, 255 }, Color{ 255, 190, 40, 180 });
        }
    }
}
