#include "PlayerRenderer.hpp"

#include "Player.hpp"
#include "Enemy.hpp"

#include "../rendering/AssetManager.hpp"

#include "raymath.h"

#include <cmath>



void PlayerRenderer::Draw(const Player& player) {
    ArmorStats armorStats = GetCompositeArmorVisuals(player.equippedHelmet, player.equippedChestplate, player.equippedLeggings);

    // Render active Basalt Pillars
    for (const auto& pillar : player.basaltPillars) {
        if (!pillar.active) continue;
        float alpha = fminf(1.0f, pillar.timer / 0.5f);
        Color pillarColor = Color{ 55, 60, 70, (unsigned char)(255 * alpha) };
        Color edgeColor = Color{ 220, 130, 40, (unsigned char)(200 * alpha) };
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, pillar.radius, pillarColor);
        DrawCircleLines((int)pillar.position.x, (int)pillar.position.y, pillar.radius, edgeColor);
        DrawCircleLines((int)pillar.position.x, (int)pillar.position.y, pillar.radius - 4.0f, Color{ 40, 45, 52, (unsigned char)(255 * alpha) });
        DrawCircle((int)pillar.position.x, (int)pillar.position.y, 6.0f, Color{ 255, 160, 50, (unsigned char)(220 * alpha) });
    }

    // Render active Shadow Decoy
    if (player.shadowDecoy.active) {
        float flicker = 0.6f + 0.3f * sinf((float)GetTime() * 12.0f);
        Color shadowColor = Color{ 110, 60, 180, (unsigned char)(160 * flicker) };
        DrawCircle((int)player.shadowDecoy.position.x, (int)player.shadowDecoy.position.y, player.radius, shadowColor);
        DrawCircleLines((int)player.shadowDecoy.position.x, (int)player.shadowDecoy.position.y, player.radius + 3.0f, Color{ 180, 120, 255, (unsigned char)(200 * flicker) });
    }

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

    // Will of Atlas Invulnerability Divine Aegis
    if (player.HasWillOfAtlasActive()) {
        float pulse = 0.8f + 0.2f * sinf((float)GetTime() * 10.0f);
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 14.0f, Color{ 255, 235, 120, (unsigned char)(255 * pulse) });
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 18.0f, Color{ 255, 205, 50, (unsigned char)(180 * pulse) });
        for (int i = 0; i < 6; ++i) {
            float ang = i * (2.0f * 3.14159265f / 6.0f) + (float)GetTime() * 3.0f;
            Vector2 runePos = { player.position.x + cosf(ang) * (player.radius + 16.0f), player.position.y + sinf(ang) * (player.radius + 16.0f) };
            DrawCircleV(runePos, 2.5f, Color{ 255, 240, 150, 255 });
        }
    }

    // Totem of Insanity Blood Frenzy Crimson Aura
    if (player.GetTotem() == TotemType::TOTEM_INSANITY && (player.GetHealth() / player.GetMaxHealth() <= 0.40f)) {
        float pulse = 0.5f + 0.5f * sinf((float)GetTime() * 8.0f);
        DrawCircleGradient(player.position, player.radius + 16.0f + 4.0f * pulse, Color{ 255, 30, 45, (unsigned char)(110 * pulse) }, Color{ 0, 0, 0, 0 });
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 12.0f + 2.0f * pulse, Color{ 255, 50, 70, (unsigned char)(180 * pulse) });
    }

    // Frostwarden Frost Nova (12 radiating crystalline ice spires)
    if (player.frostNovaVisualTimer > 0.0f) {
        float novaProgress = fminf(1.0f, fmaxf(0.0f, 1.0f - (player.frostNovaVisualTimer / 0.65f)));
        float curR = 26.0f + (190.0f - 26.0f) * novaProgress;
        unsigned char alpha = (unsigned char)(240 * (1.0f - novaProgress));
        DrawCircleGradient(player.position, curR, Color{ 100, 215, 255, (unsigned char)(alpha * 0.28f) }, Color{ 60, 170, 240, 0 });
        DrawCircleLines((int)player.position.x, (int)player.position.y, curR, Color{ 165, 240, 255, alpha });
        DrawCircleLines((int)player.position.x, (int)player.position.y, curR * 0.72f, Color{ 110, 220, 255, (unsigned char)(alpha * 0.85f) });
        for (int s = 0; s < 12; ++s) {
            float rad = ((float)s * 30.0f + novaProgress * 12.0f) * DEG2RAD;
            Vector2 dirS = { cosf(rad), sinf(rad) };
            Vector2 perpS = { -dirS.y, dirS.x };
            float halfW = 10.5f * (1.0f - 0.35f * novaProgress);
            Vector2 baseMid = { player.position.x + dirS.x * (curR * 0.42f), player.position.y + dirS.y * (curR * 0.42f) };
            Vector2 baseL = { baseMid.x - perpS.x * halfW, baseMid.y - perpS.y * halfW };
            Vector2 baseR = { baseMid.x + perpS.x * halfW, baseMid.y + perpS.y * halfW };
            Vector2 tip = { player.position.x + dirS.x * (curR + 18.0f), player.position.y + dirS.y * (curR + 18.0f) };
            DrawTriangle(baseL, tip, baseR, Color{ 130, 232, 255, (unsigned char)(alpha * 0.85f) });
            DrawTriangleLines(baseL, tip, baseR, Color{ 225, 250, 255, alpha });
            DrawLineEx(baseMid, tip, 2.0f, Color{ 245, 255, 255, alpha });
        }
    }

    // Active Armor Set Shield Bubble (Frostwarden Permafrost Ice Ward vs Crusader Dawnfire Aegis)
    if (player.HasFullArmorSet(ArmorSet::FROSTWARDEN) && (player.armorSkillActiveTimer > 0.0f || player.armorSkillShieldHp > 0.0f)) {
        float pulse = 0.75f + 0.25f * sinf((float)GetTime() * 7.0f);
        if (player.armorSkillShieldHp > 0.0f) {
            DrawCircleGradient(player.position, player.radius + 18.0f, Color{ 120, 225, 255, (unsigned char)(45 * pulse) }, Color{ 70, 185, 245, 0 });
            DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 13.0f, Color{ 160, 240, 255, (unsigned char)(230 * pulse) });
        }
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 18.0f, Color{ 110, 220, 255, (unsigned char)(175 * pulse) });
        for (int i = 0; i < 6; ++i) {
            float ang = i * (2.0f * 3.14159265f / 6.0f) + (float)GetTime() * 2.6f;
            float orbitR = player.radius + 20.0f;
            Vector2 c = { player.position.x + cosf(ang) * orbitR, player.position.y + sinf(ang) * orbitR };
            Vector2 outV = { c.x + cosf(ang) * 5.0f, c.y + sinf(ang) * 5.0f };
            Vector2 inV = { c.x - cosf(ang) * 5.0f, c.y - sinf(ang) * 5.0f };
            Vector2 sideA = { c.x - sinf(ang) * 3.2f, c.y + cosf(ang) * 3.2f };
            Vector2 sideB = { c.x + sinf(ang) * 3.2f, c.y - cosf(ang) * 3.2f };
            DrawTriangle(outV, sideA, sideB, Color{ 195, 248, 255, 235 });
            DrawTriangle(inV, sideB, sideA, Color{ 110, 220, 255, 235 });
        }
    } else if (player.armorSkillShieldHp > 0.0f) {
        float pulse = 0.7f + 0.3f * sinf((float)GetTime() * 6.0f);
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 12.0f, Color{ 255, 230, 90, (unsigned char)(220 * pulse) });
        DrawCircleLines((int)player.position.x, (int)player.position.y, player.radius + 16.0f, Color{ 255, 200, 60, (unsigned char)(140 * pulse) });
    }

    // Forged Scrap Ironclad Spike Wall
    if (player.armorSkillActiveTimer > 0.0f && player.HasFullArmorSet(ArmorSet::FORGED_SCRAP_IRON)) {
        for (int i = 0; i < 8; ++i) {
            float ang = i * (2.0f * 3.14159265f / 8.0f) + (float)GetTime() * 2.0f;
            Vector2 p1 = { player.position.x + cosf(ang) * (player.radius + 2.0f), player.position.y + sinf(ang) * (player.radius + 2.0f) };
            Vector2 p2 = { player.position.x + cosf(ang) * (player.radius + 14.0f), player.position.y + sinf(ang) * (player.radius + 14.0f) };
            DrawLineEx(p1, p2, 2.5f, Color{ 220, 225, 235, 240 });
        }
    }



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

            // Enhanced multi-layer luminous swing arc
            DrawCircleSector(player.position, player.currentAttack.range * 1.06f, startAngle, startAngle + sweepAngle, 18, Color{ 0, 180, 255, 30 });
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 18, Color{ 40, 220, 255, 95 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 18, Color{ 160, 245, 255, 220 });
            float leadRad = (startAngle + sweepAngle) * DEG2RAD;
            Vector2 tipFlare = { player.position.x + cosf(leadRad) * player.currentAttack.range, player.position.y + sinf(leadRad) * player.currentAttack.range };
            DrawCircleV(tipFlare, 3.5f, Color{ 255, 255, 255, 240 });

        } else if (player.currentAttack.type == AttackType::HEAVY) {

            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);

            if (player.heavyStage == 3) {
                // Stage 3: Radiant Execution (downward holy cleave)
                swordAngle = player.facingAngle - 90.0f + progress * 180.0f;
                AssetManager::DrawSword(rightHand, swordAngle, 56.0f, Color{ 255, 255, 220, 255 }, Color{ 255, 215, 60, 255 });

                float rad = player.facingAngle * DEG2RAD;
                Vector2 dir = { cosf(rad), sinf(rad) };
                Vector2 normal = { -dir.y, dir.x };
                float halfWidth = (player.currentAttack.width > 0.0f ? player.currentAttack.width : 56.0f) * 0.5f;
                float length = player.currentAttack.range;

                Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };
                Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };
                Vector2 c3 = { c2.x + dir.x * length, c2.y + dir.y * length };
                Vector2 c4 = { c1.x + dir.x * length, c1.y + dir.y * length };

                DrawTriangle(c1, c2, c3, Color{ 255, 230, 80, (unsigned char)(100 * (1.0f - progress * 0.4f)) });
                DrawTriangle(c1, c3, c4, Color{ 255, 230, 80, (unsigned char)(100 * (1.0f - progress * 0.4f)) });
                DrawLineEx(c1, c4, 3.0f, Color{ 255, 255, 180, 240 });
                DrawLineEx(c2, c3, 3.0f, Color{ 255, 255, 180, 240 });
                DrawLineEx(c4, c3, 3.5f, Color{ 255, 255, 220, 255 });
            } else if (player.heavyStage == 2) {

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
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            // Dual Daggers Heavy Stages 2 & 3
            if (player.heavyStage == 3) {
                // Stage 3: Vault Kick (kick arc & backward disengage)
                DrawCircleSector(player.position, player.currentAttack.range, player.facingAngle - 35.0f, player.facingAngle + 35.0f, 10, Color{ 80, 255, 170, (unsigned char)(80 * (1.0f - progress)) });
                DrawCircleSectorLines(player.position, player.currentAttack.range, player.facingAngle - 35.0f, player.facingAngle + 35.0f, 10, Color{ 140, 255, 200, 230 });
                AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 180, 255, 220, 255 }, Color{ 60, 240, 180, 200 });
            } else {
                // Stage 2: Backstab Cross-Slash 360 spin
                float spinAngle = player.facingAngle + progress * 360.0f;
                angleR = spinAngle;
                angleL = spinAngle + 180.0f;
                AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 220, 255, 200, 255 }, Color{ 80, 255, 160, 220 });
                DrawCircleLines((int)player.position.x, (int)player.position.y, player.currentAttack.range, Color{ 80, 255, 160, 180 });
                DrawCircle((int)player.position.x, (int)player.position.y, player.currentAttack.range * progress, Color{ 60, 240, 180, 40 });
            }} else {

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
            if (player.heavyStage == 2) {
                // Stage 2: Sacred Judgment Pillar (sunray beam from above)
                swordAngle = player.facingAngle - 40.0f;
                AssetManager::DrawSword(rightHand, swordAngle, 56.0f, Color{ 255, 255, 200, 255 }, Color{ 255, 220, 60, 255 });

                Vector2 center = player.currentAttack.circleCenter;
                float pillarRadius = player.currentAttack.circleRadius;
                Vector2 beamTop = { center.x, center.y - 280.0f };
                DrawLineEx(beamTop, center, 14.0f, Color{ 255, 255, 210, (unsigned char)(200 * (1.0f - progress * 0.4f)) });
                DrawCircleV(center, pillarRadius * (0.6f + 0.4f * progress), Color{ 255, 230, 80, (unsigned char)(90 * (1.0f - progress)) });
                DrawCircleLines((int)center.x, (int)center.y, pillarRadius, Color{ 255, 245, 140, 240 });
            } else {
                // Stage 1: Dawnfire Ring 360 circle AoE swing
                swordAngle = player.facingAngle + progress * 360.0f;
                AssetManager::DrawSword(rightHand, swordAngle, 56.0f, Color{ 255, 245, 180, 255 }, Color{ 255, 180, 30, 255 });
                DrawCircleLines((int)player.position.x, (int)player.position.y, player.currentAttack.range, Color{ 255, 220, 70, 230 });
                DrawCircle((int)player.position.x, (int)player.position.y, player.currentAttack.range * (0.4f + 0.6f * progress), Color{ 255, 190, 40, (unsigned char)(60 * (1.0f - progress)) });
            }} else if (player.isHeavyWindingUp) {

            swordAngle = player.facingAngle - 95.0f;

            AssetManager::DrawSword(rightHand, swordAngle, 52.0f, Color{ 255, 220, 80, 255 }, Color{ 255, 140, 10, 240 });



            // Glowing holy charging aura

            float pulse = 0.5f + 0.5f * sinf((float)GetTime() * 12.0f);

            DrawCircleLines((int)player.position.x, (int)player.position.y, 45.0f + pulse * 8.0f, Color{ 255, 215, 60, 220 });

            DrawCircle((int)player.position.x, (int)player.position.y, 20.0f, Color{ 255, 180, 40, (unsigned char)(100 * pulse) });

        } else {

            AssetManager::DrawSword(rightHand, swordAngle, 42.0f, Color{ 255, 225, 120, 255 }, Color{ 255, 190, 40, 180 });

        }

    } else if (player.currentWeapon == WeaponType::HALBERD) {

        float halberdAngle = player.facingAngle + 30.0f;

        if (player.currentAttack.type == AttackType::LIGHT) {

            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);

            float thrustExt = sinf(progress * 3.14159f) * 32.0f;

            Vector2 thrustOrigin = { rightHand.x + cosf(player.facingAngle * DEG2RAD) * thrustExt, rightHand.y + sinf(player.facingAngle * DEG2RAD) * thrustExt };

            halberdAngle = player.facingAngle;

            AssetManager::DrawHalberd(thrustOrigin, halberdAngle, 56.0f, Color{ 235, 245, 255, 255 }, Color{ 140, 200, 255, 240 });



            // Linear thrust LineBox AoE preview

            float rad = player.facingAngle * DEG2RAD;

            Vector2 dir = { cosf(rad), sinf(rad) };

            Vector2 normal = { -dir.y, dir.x };

            float halfWidth = 22.0f;

            float length = player.currentAttack.range;



            Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };

            Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };

            Vector2 c3 = { c2.x + dir.x * length, c2.y + dir.y * length };

            Vector2 c4 = { c1.x + dir.x * length, c1.y + dir.y * length };



            DrawTriangle(c1, c2, c3, Color{ 140, 200, 255, (unsigned char)(70 * (1.0f - progress * 0.5f)) });

            DrawTriangle(c1, c3, c4, Color{ 140, 200, 255, (unsigned char)(70 * (1.0f - progress * 0.5f)) });

            DrawLineEx(c1, c2, 2.0f, Color{ 180, 225, 255, 220 });

            DrawLineEx(c2, c3, 2.0f, Color{ 180, 225, 255, 220 });

            DrawLineEx(c3, c4, 2.0f, Color{ 180, 225, 255, 220 });

            DrawLineEx(c4, c1, 2.0f, Color{ 180, 225, 255, 220 });

        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            if (player.heavyStage == 1) {
                // Stage 1: Long Lunge Thrust (LineBox) with Invulnerability frames & cyan blur trail
                float thrustExt = sinf(progress * 3.14159f) * 48.0f;
                Vector2 thrustOrigin = { rightHand.x + cosf(player.facingAngle * DEG2RAD) * thrustExt, rightHand.y + sinf(player.facingAngle * DEG2RAD) * thrustExt };
                halberdAngle = player.facingAngle;
                AssetManager::DrawHalberd(thrustOrigin, halberdAngle, 64.0f, Color{ 230, 245, 255, 255 }, Color{ 120, 200, 255, 240 });

                float rad = player.facingAngle * DEG2RAD;
                Vector2 dir = { cosf(rad), sinf(rad) };
                Vector2 normal = { -dir.y, dir.x };
                float halfWidth = 26.0f;
                float length = player.currentAttack.range;

                Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };
                Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };
                Vector2 c3 = { c2.x + dir.x * length, c2.y + dir.y * length };
                Vector2 c4 = { c1.x + dir.x * length, c1.y + dir.y * length };

                DrawTriangle(c1, c2, c3, Color{ 140, 220, 255, (unsigned char)(70 * (1.0f - progress * 0.5f)) });
                DrawTriangle(c1, c3, c4, Color{ 140, 220, 255, (unsigned char)(70 * (1.0f - progress * 0.5f)) });
                DrawLineEx(c1, c2, 2.0f, Color{ 180, 240, 255, 220 });
                DrawLineEx(c2, c3, 2.0f, Color{ 180, 240, 255, 220 });
                DrawLineEx(c3, c4, 2.0f, Color{ 180, 240, 255, 220 });
                DrawLineEx(c4, c1, 2.0f, Color{ 180, 240, 255, 220 });

                // Invulnerability ethereal shimmer ring during lunge
                DrawCircleLines((int)player.position.x, (int)player.position.y, 28.0f + sinf(progress * 3.14f) * 6.0f, Color{ 180, 240, 255, (unsigned char)(160 * (1.0f - progress)) });
            } else if (player.heavyStage == 2) {
                // Stage 2: Wide 220-deg Sweep Cleave
                halberdAngle = player.facingAngle - 110.0f + progress * player.currentAttack.arcAngle;
                AssetManager::DrawHalberd(rightHand, halberdAngle, 60.0f, Color{ 220, 240, 255, 255 }, Color{ 120, 195, 255, 240 });

                float startAngle = player.facingAngle - 110.0f;
                float sweepAngle = progress * player.currentAttack.arcAngle;
                DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 120, 200, 255, 75 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 180, 230, 255, 220 });
            } else if (player.heavyStage == 3) {
                // Stage 3: Flurry of 3 Rapid Consecutive Stabs
                float stabExt = sinf(progress * 3.14159f) * 36.0f;
                Vector2 thrustOrigin = { rightHand.x + cosf(player.facingAngle * DEG2RAD) * stabExt, rightHand.y + sinf(player.facingAngle * DEG2RAD) * stabExt };
                halberdAngle = player.facingAngle;
                AssetManager::DrawHalberd(thrustOrigin, halberdAngle, 58.0f, Color{ 255, 250, 230, 255 }, Color{ 255, 200, 80, 255 });

                float rad = player.facingAngle * DEG2RAD;
                Vector2 dir = { cosf(rad), sinf(rad) };
                Vector2 normal = { -dir.y, dir.x };
                float halfWidth = 23.0f;
                float length = player.currentAttack.range;

                Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };
                Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };
                Vector2 c3 = { c2.x + dir.x * length, c2.y + dir.y * length };
                Vector2 c4 = { c1.x + dir.x * length, c1.y + dir.y * length };

                DrawTriangle(c1, c2, c3, Color{ 255, 220, 100, (unsigned char)(60 * (1.0f - progress * 0.5f)) });
                DrawTriangle(c1, c3, c4, Color{ 255, 220, 100, (unsigned char)(60 * (1.0f - progress * 0.5f)) });
                DrawLineEx(c1, c2, 1.8f, Color{ 255, 230, 140, 220 });
                DrawLineEx(c2, c3, 1.8f, Color{ 255, 230, 140, 220 });
                DrawLineEx(c3, c4, 1.8f, Color{ 255, 230, 140, 220 });
                DrawLineEx(c4, c1, 1.8f, Color{ 255, 230, 140, 220 });
            }
        } else if (player.isHeavyWindingUp) {

            halberdAngle = player.facingAngle - 115.0f;

            AssetManager::DrawHalberd(rightHand, halberdAngle, 56.0f, Color{ 255, 220, 100, 255 }, Color{ 255, 160, 40, 220 });

        } else {

            AssetManager::DrawHalberd(rightHand, halberdAngle, 48.0f, Color{ 210, 225, 245, 255 }, Color{ 120, 160, 210, 160 });
        }
    } else if (player.currentWeapon == WeaponType::ZEPHYR_DAGGERS) {
        float angleL = player.facingAngle - 25.0f;
        float angleR = player.facingAngle + 25.0f;
        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            angleR = player.facingAngle - 35.0f + progress * 85.0f;
            angleL = player.facingAngle + 35.0f - progress * 85.0f;
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 140, 255, 235, 255 }, Color{ 90, 240, 215, 220 });
            float startAngle = player.facingAngle - 45.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 14, Color{ 90, 240, 215, 60 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            float spinAngle = player.facingAngle + progress * 360.0f;
            angleR = spinAngle;
            angleL = spinAngle + 180.0f;
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 180, 255, 245, 255 }, Color{ 110, 255, 230, 240 });
            DrawCircleLines((int)player.position.x, (int)player.position.y, player.currentAttack.range, Color{ 110, 255, 230, 180 });
        } else {
            AssetManager::DrawDaggers(leftHand, rightHand, angleL, angleR, Color{ 120, 235, 215, 255 }, Color{ 80, 210, 190, 180 });
        }
    } else if (player.currentWeapon == WeaponType::WINDRUNNER_RECURVE) {
        float rad = player.facingAngle * DEG2RAD;
        Vector2 bowDir = { cosf(rad), sinf(rad) };
        Vector2 bowNorm = { -bowDir.y, bowDir.x };
        Vector2 bowCenter = { rightHand.x + bowDir.x * 12.0f, rightHand.y + bowDir.y * 12.0f };
        Vector2 bowTop = { bowCenter.x + bowNorm.x * 20.0f - bowDir.x * 6.0f, bowCenter.y + bowNorm.y * 20.0f - bowDir.y * 6.0f };
        Vector2 bowBot = { bowCenter.x - bowNorm.x * 20.0f - bowDir.x * 6.0f, bowCenter.y - bowNorm.y * 20.0f - bowDir.y * 6.0f };
        DrawLineEx(bowTop, bowCenter, 2.5f, Color{ 255, 215, 80, 255 });
        DrawLineEx(bowCenter, bowBot, 2.5f, Color{ 255, 215, 80, 255 });
        DrawLineEx(bowTop, bowBot, 1.2f, Color{ 120, 245, 225, 220 });
        if (player.currentAttack.type == AttackType::LIGHT || player.currentAttack.type == AttackType::HEAVY) {
            DrawLineEx(bowCenter, { bowCenter.x + bowDir.x * 22.0f, bowCenter.y + bowDir.y * 22.0f }, 2.0f, Color{ 180, 255, 240, 255 });
        }
    } else if (player.currentWeapon == WeaponType::SUN_CLAYMORE) {
        float claymoreAngle = player.facingAngle + 35.0f;
        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            claymoreAngle = player.facingAngle - 70.0f + progress * player.currentAttack.arcAngle;
            AssetManager::DrawClaymore(rightHand, claymoreAngle, 56.0f, Color{ 255, 220, 100, 255 }, Color{ 255, 140, 30, 240 });

            float startAngle = player.facingAngle - 70.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range * 1.08f, startAngle, startAngle + sweepAngle, 20, Color{ 255, 140, 30, 45 });
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 195, 60, 110 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 20, Color{ 255, 245, 160, 240 });
            float leadRad = (startAngle + sweepAngle) * DEG2RAD;
            Vector2 tipFlare = { player.position.x + cosf(leadRad) * player.currentAttack.range, player.position.y + sinf(leadRad) * player.currentAttack.range };
            DrawCircleV(tipFlare, 4.5f, Color{ 255, 255, 220, 255 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            if (player.heavyStage == 1) {
                // Stage 1: Sunfire Crescent (180-deg golden sweep)
                claymoreAngle = player.facingAngle - 90.0f + progress * 180.0f;
                AssetManager::DrawClaymore(rightHand, claymoreAngle, 64.0f, Color{ 255, 245, 180, 255 }, Color{ 255, 160, 40, 255 });

                float startAngle = player.facingAngle - 90.0f;
                float sweepAngle = progress * 180.0f;
                DrawCircleSector(player.position, player.currentAttack.range * 1.08f, startAngle, startAngle + sweepAngle, 22, Color{ 255, 110, 20, 65 });
                DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 255, 200, 60, 130 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 22, Color{ 255, 250, 200, 255 });
                float leadRad = (startAngle + sweepAngle) * DEG2RAD;
                Vector2 tipFlare = { player.position.x + cosf(leadRad) * player.currentAttack.range, player.position.y + sinf(leadRad) * player.currentAttack.range };
                DrawCircleV(tipFlare, 6.0f, Color{ 255, 255, 210, 255 });
            } else {
                // Stage 2: Solar Cataclysm Slam (Blazing sky-beam pillar & multi-ring sun-sigil detonation)
                claymoreAngle = player.facingAngle;
                Vector2 plungeOrigin = { rightHand.x + cosf(claymoreAngle * DEG2RAD) * 26.0f, rightHand.y + sinf(claymoreAngle * DEG2RAD) * 26.0f };
                AssetManager::DrawClaymore(plungeOrigin, claymoreAngle, 66.0f, Color{ 255, 255, 210, 255 }, Color{ 255, 190, 50, 255 });

                Vector2 center = player.currentAttack.circleCenter;
                float pillarR = player.currentAttack.circleRadius;
                float fade = 1.0f - progress * 0.45f;
                Vector2 beamTop = { center.x, center.y - 260.0f };
                DrawLineEx(beamTop, center, 24.0f, Color{ 255, 175, 45, (unsigned char)(135 * fade) });
                DrawLineEx(beamTop, center, 10.0f, Color{ 255, 250, 200, (unsigned char)(230 * fade) });
                DrawCircleV(center, pillarR * (0.45f + 0.55f * progress), Color{ 255, 170, 35, (unsigned char)(95 * (1.0f - progress)) });
                DrawCircleV(center, pillarR * 0.42f, Color{ 255, 235, 130, (unsigned char)(140 * (1.0f - progress * 0.6f)) });
                DrawCircleLines((int)center.x, (int)center.y, pillarR, Color{ 255, 230, 90, 240 });
                DrawCircleLines((int)center.x, (int)center.y, pillarR * (0.45f + 0.55f * progress), Color{ 255, 255, 210, 255 });
                for (int r = 0; r < 8; r++) {
                    float rayAng = (r * 45.0f + progress * 35.0f) * DEG2RAD;
                    Vector2 rOut = { center.x + cosf(rayAng) * pillarR * 1.15f, center.y + sinf(rayAng) * pillarR * 1.15f };
                    DrawLineEx(center, rOut, 2.5f, Color{ 255, 225, 110, (unsigned char)(195 * fade) });
                }
            }
        } else if (player.isHeavyWindingUp) {
            claymoreAngle = player.facingAngle - 110.0f;
            AssetManager::DrawClaymore(rightHand, claymoreAngle, 58.0f, Color{ 255, 210, 80, 255 }, Color{ 255, 120, 20, 230 });
        } else {
            AssetManager::DrawClaymore(rightHand, claymoreAngle, 52.0f, Color{ 245, 205, 90, 255 }, Color{ 200, 120, 30, 180 });
        }
    } else if (player.currentWeapon == WeaponType::STORM_GLAIVE) {
        float glaiveAngle = player.facingAngle + 40.0f;
        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            glaiveAngle = player.facingAngle;
            AssetManager::DrawGlaive(rightHand, glaiveAngle, 56.0f, Color{ 160, 255, 240, 255 }, Color{ 80, 235, 215, 230 });

            // Linear thrust box & wind trail
            float rad = player.facingAngle * DEG2RAD;
            Vector2 dir = { cosf(rad), sinf(rad) };
            Vector2 normal = { -dir.y, dir.x };
            float halfWidth = 23.0f;
            float length = player.currentAttack.range;
            Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };
            Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };
            Vector2 c3 = { c2.x + dir.x * length, c2.y + dir.y * length };
            Vector2 c4 = { c1.x + dir.x * length, c1.y + dir.y * length };
            DrawTriangle(c1, c2, c3, Color{ 100, 245, 230, (unsigned char)(70 * (1.0f - progress * 0.4f)) });
            DrawTriangle(c1, c3, c4, Color{ 100, 245, 230, (unsigned char)(70 * (1.0f - progress * 0.4f)) });
            DrawLineEx(c4, c3, 3.0f, Color{ 180, 255, 245, 240 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            if (player.heavyStage == 1) {
                // Stage 1: Zephyr Surge Step (High-speed forward thrust + i-frame gale corridor)
                float thrustExt = sinf(progress * 3.14159f) * 44.0f;
                Vector2 thrustOrigin = { rightHand.x + cosf(player.facingAngle * DEG2RAD) * thrustExt, rightHand.y + sinf(player.facingAngle * DEG2RAD) * thrustExt };
                glaiveAngle = player.facingAngle;
                AssetManager::DrawGlaive(thrustOrigin, glaiveAngle, 64.0f, Color{ 180, 255, 245, 255 }, Color{ 90, 245, 225, 240 });

                float rad = player.facingAngle * DEG2RAD;
                Vector2 dir = { cosf(rad), sinf(rad) };
                Vector2 normal = { -dir.y, dir.x };
                float halfWidth = 26.0f;
                float length = player.currentAttack.range;
                Vector2 c1 = { player.position.x - normal.x * halfWidth, player.position.y - normal.y * halfWidth };
                Vector2 c2 = { player.position.x + normal.x * halfWidth, player.position.y + normal.y * halfWidth };
                Vector2 c3 = { c2.x + dir.x * length, c2.y + dir.y * length };
                Vector2 c4 = { c1.x + dir.x * length, c1.y + dir.y * length };
                DrawTriangle(c1, c2, c3, Color{ 100, 245, 230, (unsigned char)(85 * (1.0f - progress * 0.4f)) });
                DrawTriangle(c1, c3, c4, Color{ 100, 245, 230, (unsigned char)(85 * (1.0f - progress * 0.4f)) });
                DrawLineEx(c1, c4, 2.2f, Color{ 190, 255, 245, 230 });
                DrawLineEx(c2, c3, 2.2f, Color{ 190, 255, 245, 230 });
                DrawLineEx(c4, c3, 3.5f, WHITE);
                DrawCircleLines((int)player.position.x, (int)player.position.y, 28.0f + progress * 10.0f, Color{ 160, 255, 245, (unsigned char)(190 * (1.0f - progress)) });
            } else {
                // Stage 2: Tempest Cyclone (360-deg spinning wind vortex + layered wind-crescent trails)
                glaiveAngle = player.facingAngle + progress * 420.0f;
                AssetManager::DrawGlaive(rightHand, glaiveAngle, 64.0f, Color{ 200, 255, 250, 255 }, Color{ 100, 245, 230, 255 });
                float r = player.currentAttack.range;
                unsigned char a = (unsigned char)(230 * (1.0f - progress * 0.45f));
                DrawCircleV(player.position, r * (0.4f + 0.6f * progress), Color{ 90, 240, 225, (unsigned char)(55 * (1.0f - progress)) });
                DrawCircleLines((int)player.position.x, (int)player.position.y, r, Color{ 160, 255, 245, a });
                DrawCircleLines((int)player.position.x, (int)player.position.y, r * 0.68f, Color{ 110, 245, 230, (unsigned char)(a * 0.85f) });
                DrawCircleLines((int)player.position.x, (int)player.position.y, r * 0.38f, Color{ 220, 255, 250, a });
                for (int w = 0; w < 4; w++) {
                    float aStart = player.facingAngle + progress * 360.0f + w * 90.0f;
                    DrawCircleSectorLines(player.position, r * (0.76f + 0.08f * (w % 2)), aStart, aStart + 58.0f, 10, Color{ 210, 255, 250, a });
                }
            }
        } else if (player.isHeavyWindingUp) {
            glaiveAngle = player.facingAngle - 120.0f;
            AssetManager::DrawGlaive(rightHand, glaiveAngle, 54.0f, Color{ 140, 240, 225, 255 }, Color{ 70, 210, 195, 220 });
        } else {
            AssetManager::DrawGlaive(rightHand, glaiveAngle, 48.0f, Color{ 120, 230, 215, 255 }, Color{ 60, 180, 165, 180 });
        }
    } else if (player.currentWeapon == WeaponType::GLACIAL_CLEAVER) {
        // Dual-wielding Twin Glacial Cleavers
        float angleR = player.facingAngle + 32.0f;
        float angleL = player.facingAngle - 32.0f;
        Color iceHead = Color{ 170, 232, 255, 255 };
        Color iceGlow = Color{ 85, 198, 245, 235 };

        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            if (player.comboStep == 2) {
                // Left cleaver upward slash, right held ready
                angleL = player.facingAngle + 65.0f - progress * 130.0f;
                AssetManager::DrawCleaver(rightHand, angleR, 44.0f, Color{ 140, 210, 245, 255 }, Color{ 70, 175, 225, 190 });
                AssetManager::DrawCleaver(leftHand, angleL, 52.0f, iceHead, iceGlow);
                float sweepStart = player.facingAngle + 65.0f - progress * player.currentAttack.arcAngle;
                DrawCircleSector(player.position, player.currentAttack.range, sweepStart, player.facingAngle + 65.0f, 16, Color{ 130, 225, 255, 65 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, sweepStart, player.facingAngle + 65.0f, 16, Color{ 195, 245, 255, 220 });
            } else {
                // Right cleaver downward slash, left held ready
                angleR = player.facingAngle - 65.0f + progress * 130.0f;
                AssetManager::DrawCleaver(leftHand, angleL, 44.0f, Color{ 140, 210, 245, 255 }, Color{ 70, 175, 225, 190 });
                AssetManager::DrawCleaver(rightHand, angleR, 52.0f, iceHead, iceGlow);
                float sweepStart = player.facingAngle - 65.0f;
                DrawCircleSector(player.position, player.currentAttack.range, sweepStart, sweepStart + progress * player.currentAttack.arcAngle, 16, Color{ 130, 225, 255, 65 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, sweepStart, sweepStart + progress * player.currentAttack.arcAngle, 16, Color{ 195, 245, 255, 220 });
            }
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            if (player.heavyStage == 1) {
                angleR = player.facingAngle - 82.0f + progress * 165.0f;
                AssetManager::DrawCleaver(leftHand, angleL, 46.0f, iceHead, iceGlow);
                AssetManager::DrawCleaver(rightHand, angleR, 56.0f, Color{ 200, 245, 255, 255 }, Color{ 110, 220, 255, 255 });
                float startA = player.facingAngle - 82.0f;
                DrawCircleSector(player.position, player.currentAttack.range, startA, startA + progress * 165.0f, 18, Color{ 110, 215, 255, 75 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, startA, startA + progress * 165.0f, 18, Color{ 195, 245, 255, 235 });
            } else if (player.heavyStage == 2) {
                angleL = player.facingAngle + 82.0f - progress * 165.0f;
                AssetManager::DrawCleaver(rightHand, angleR, 46.0f, iceHead, iceGlow);
                AssetManager::DrawCleaver(leftHand, angleL, 56.0f, Color{ 200, 245, 255, 255 }, Color{ 110, 220, 255, 255 });
                float endA = player.facingAngle + 82.0f;
                DrawCircleSector(player.position, player.currentAttack.range, endA - progress * 165.0f, endA, 18, Color{ 110, 215, 255, 75 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, endA - progress * 165.0f, endA, 18, Color{ 195, 245, 255, 235 });
            } else if (player.heavyStage == 3) {
                // Twin X-cross flurry
                angleR = player.facingAngle - 88.0f + progress * 175.0f;
                angleL = player.facingAngle + 88.0f - progress * 175.0f;
                AssetManager::DrawCleaver(rightHand, angleR, 58.0f, Color{ 210, 248, 255, 255 }, Color{ 125, 225, 255, 255 });
                AssetManager::DrawCleaver(leftHand, angleL, 58.0f, Color{ 210, 248, 255, 255 }, Color{ 125, 225, 255, 255 });
                DrawCircleSector(player.position, player.currentAttack.range, player.facingAngle - 88.0f, player.facingAngle - 88.0f + progress * 175.0f, 18, Color{ 120, 225, 255, 70 });
                DrawCircleSector(player.position, player.currentAttack.range, player.facingAngle + 88.0f - progress * 175.0f, player.facingAngle + 88.0f, 18, Color{ 160, 238, 255, 70 });
                DrawCircleSectorLines(player.position, player.currentAttack.range, player.facingAngle - 88.0f, player.facingAngle + 88.0f, 18, Color{ 210, 250, 255, 240 });
            } else {
                // Stage 4 Finisher: Glacial Cross-Rupture slam
                angleR = player.facingAngle + 55.0f - progress * 55.0f;
                angleL = player.facingAngle - 55.0f + progress * 55.0f;
                AssetManager::DrawCleaver(rightHand, angleR, 62.0f, Color{ 225, 252, 255, 255 }, Color{ 130, 230, 255, 255 });
                AssetManager::DrawCleaver(leftHand, angleL, 62.0f, Color{ 225, 252, 255, 255 }, Color{ 130, 230, 255, 255 });
                float startA = player.facingAngle - 110.0f;
                DrawCircleSector(player.position, player.currentAttack.range, startA, startA + 220.0f, 22, Color{ 120, 225, 255, (unsigned char)(95 * (1.0f - progress * 0.4f)) });
                DrawCircleSectorLines(player.position, player.currentAttack.range, startA, startA + 220.0f, 22, Color{ 210, 250, 255, 250 });
                Vector2 rupPos = player.currentAttack.shockwavePos;
                float rupR = player.currentAttack.shockwaveRadius;
                DrawCircleV(rupPos, rupR * (0.35f + 0.55f * progress), Color{ 140, 235, 255, (unsigned char)(110 * (1.0f - progress)) });
                DrawCircleLines((int)rupPos.x, (int)rupPos.y, rupR, Color{ 200, 248, 255, 235 });
            }
        } else if (player.isHeavyWindingUp) {
            angleR = player.facingAngle - 105.0f;
            angleL = player.facingAngle + 105.0f;
            AssetManager::DrawCleaver(rightHand, angleR, 52.0f, Color{ 180, 235, 255, 255 }, Color{ 90, 205, 245, 220 });
            AssetManager::DrawCleaver(leftHand, angleL, 52.0f, Color{ 180, 235, 255, 255 }, Color{ 90, 205, 245, 220 });
            DrawCircleLines((int)player.position.x, (int)player.position.y, 34.0f, Color{ 150, 230, 255, 190 });
        } else {
            AssetManager::DrawCleaver(rightHand, angleR, 45.0f, Color{ 145, 215, 245, 255 }, Color{ 65, 170, 220, 185 });
            AssetManager::DrawCleaver(leftHand, angleL, 45.0f, Color{ 145, 215, 245, 255 }, Color{ 65, 170, 220, 185 });
        }
    } else if (player.currentWeapon == WeaponType::SOVEREIGN_SCEPTER) {
        float scepterAngle = player.facingAngle + 35.0f;
        Color headCol = Color{ 140, 240, 255, 255 };
        Color glowCol = Color{ 80, 215, 255, 235 };

        if (player.currentAttack.type == AttackType::LIGHT) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            scepterAngle = player.facingAngle - 40.0f + progress * 80.0f;
            AssetManager::DrawScepter(rightHand, scepterAngle, 44.0f, headCol, glowCol);

            float startAngle = player.facingAngle - 45.0f;
            float sweepAngle = progress * player.currentAttack.arcAngle;
            DrawCircleSector(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 120, 235, 255, 50 });
            DrawCircleSectorLines(player.position, player.currentAttack.range, startAngle, startAngle + sweepAngle, 16, Color{ 180, 245, 255, 220 });

            // Forward holy beam pulse
            float rad = player.facingAngle * DEG2RAD;
            Vector2 tipPos = { player.position.x + cosf(rad) * player.currentAttack.range * progress, player.position.y + sinf(rad) * player.currentAttack.range * progress };
            DrawCircleV(tipPos, 4.0f, Color{ 255, 255, 255, 240 });
        } else if (player.currentAttack.type == AttackType::HEAVY) {
            float progress = 1.0f - (player.currentAttack.timer / player.currentAttack.duration);
            scepterAngle = player.facingAngle - 75.0f; // Held aloft
            AssetManager::DrawScepter(rightHand, scepterAngle, 50.0f, Color{ 255, 245, 180, 255 }, Color{ 100, 230, 255, 255 });

            // Expanding 360-degree Sanctuary Radiance circle (up to 280px radius)
            float curR = player.currentAttack.range * (0.30f + 0.70f * progress);
            unsigned char alpha = (unsigned char)(220 * (1.0f - progress * 0.4f));
            DrawCircleV(player.position, curR, Color{ 100, 225, 255, (unsigned char)(35 * (1.0f - progress * 0.5f)) });
            DrawCircleLines((int)player.position.x, (int)player.position.y, curR, Color{ 160, 245, 255, alpha });
            DrawCircleLines((int)player.position.x, (int)player.position.y, curR * 0.85f, Color{ 255, 235, 140, (unsigned char)(alpha * 0.8f) });
            DrawCircleLines((int)player.position.x, (int)player.position.y, curR * 0.55f, Color{ 120, 220, 255, (unsigned char)(alpha * 0.6f) });

            // Runic petals / rays in circular sanctuary
            for (int r = 0; r < 8; r++) {
                float rayAng = (r * 45.0f + progress * 40.0f) * DEG2RAD;
                Vector2 rayInner = { player.position.x + cosf(rayAng) * (curR * 0.4f), player.position.y + sinf(rayAng) * (curR * 0.4f) };
                Vector2 rayOuter = { player.position.x + cosf(rayAng) * (curR * 0.95f), player.position.y + sinf(rayAng) * (curR * 0.95f) };
                DrawLineEx(rayInner, rayOuter, 2.0f, Color{ 220, 245, 255, (unsigned char)(180 * (1.0f - progress)) });
            }
        } else if (player.isHeavyWindingUp) {
            scepterAngle = player.facingAngle - 90.0f;
            AssetManager::DrawScepter(rightHand, scepterAngle, 46.0f, Color{ 255, 235, 160, 255 }, Color{ 120, 240, 255, 240 });
            DrawCircleLines((int)player.position.x, (int)player.position.y, 38.0f, Color{ 120, 230, 255, 180 });
        } else {
            AssetManager::DrawScepter(rightHand, scepterAngle, 40.0f, headCol, glowCol);
        }
    }

    // Active heavy finisher shockwave ring (extracted globally across all weapons & armor sets)
    if (player.shockwaveActive) {
        float shockProgress = fminf(1.0f, fmaxf(0.0f, 1.0f - (player.shockwaveTimer / 0.38f)));
        float curRadius = player.shockwaveRadius * shockProgress;
        unsigned char alpha = (unsigned char)(255 * (1.0f - shockProgress));
        Color outerCol = Color{ 255, 180, 50, alpha };
        Color innerCol = Color{ 255, 140, 40, (unsigned char)(alpha * 0.25f) };
        if (player.currentWeapon == WeaponType::GLACIAL_CLEAVER || player.HasFullArmorSet(ArmorSet::FROSTWARDEN)) {
            outerCol = Color{ 140, 230, 255, alpha };
            innerCol = Color{ 100, 205, 255, (unsigned char)(alpha * 0.28f) };
        } else if (player.currentWeapon == WeaponType::STORM_GLAIVE) {
            outerCol = Color{ 110, 245, 230, alpha };
            innerCol = Color{ 80, 220, 205, (unsigned char)(alpha * 0.25f) };
        } else if (player.currentWeapon == WeaponType::SOVEREIGN_SCEPTER) {
            outerCol = Color{ 140, 240, 255, alpha };
            innerCol = Color{ 180, 255, 240, (unsigned char)(alpha * 0.35f) };
        }
        DrawCircleLines((int)player.shockwavePos.x, (int)player.shockwavePos.y, curRadius, outerCol);
        DrawCircleLines((int)player.shockwavePos.x, (int)player.shockwavePos.y, curRadius * 0.78f, outerCol);
        DrawCircle((int)player.shockwavePos.x, (int)player.shockwavePos.y, curRadius * 0.4f, innerCol);
    }

    // Friendly Mini Elemental Golem / Commanding Titan Companion
    if (player.GetTotem() == TotemType::ELEMENTAL_CORE_TOTEM && player.GetCompanion().active) {
        const auto& comp = player.GetCompanion();
        DrawEllipse((int)comp.position.x, (int)(comp.position.y + comp.radius * 0.7f), comp.radius * 0.9f, comp.radius * 0.45f, Color{ 10, 10, 15, 140 });

        if (comp.isCommandingTitan) {
            // Evolved Commanding Titan: Heavy basalt armor plates, glowing runic crown & magma core
            DrawCircleV(comp.position, comp.radius, Color{ 36, 40, 48, 255 });
            DrawCircleLines((int)comp.position.x, (int)comp.position.y, comp.radius, Color{ 255, 160, 50, 240 });
            DrawCircleLines((int)comp.position.x, (int)comp.position.y, comp.radius + 3.0f, Color{ 255, 215, 80, 180 });
            for (int k = 0; k < 5; ++k) {
                float a = (float)k * (3.14159265f / 4.0f) - 3.14159265f * 0.5f;
                Vector2 spk = { comp.position.x + cosf(a) * (comp.radius + 5.0f), comp.position.y + sinf(a) * (comp.radius + 5.0f) };
                DrawCircleV(spk, 3.0f, Color{ 255, 180, 60, 255 });
            }
            float cPulse = 0.8f + 0.2f * sinf((float)GetTime() * 5.0f);
            DrawCircleV(comp.position, 8.0f * cPulse, Color{ 255, 160, 40, 255 });
            DrawCircleV(comp.position, 4.0f, Color{ 255, 240, 180, 255 });

            float barW = 38.0f;
            float barH = 5.0f;
            float barX = comp.position.x - barW * 0.5f;
            float barY = comp.position.y - comp.radius - 12.0f;
            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 20, 20, 26, 220 });
            float pct = fmaxf(0.0f, comp.health / comp.maxHealth);
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 255, 160, 50, 255 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 45, 25, 20, 200 });

            // Render 3 Mini Golems in escort formation
            const auto* subMinions = player.GetGolemSubMinions();
            for (int s = 0; s < 3; ++s) {
                const auto& sm = subMinions[s];
                if (!sm.active || sm.health <= 0.0f) continue;
                DrawEllipse((int)sm.position.x, (int)(sm.position.y + sm.radius * 0.7f), sm.radius * 0.85f, sm.radius * 0.4f, Color{ 10, 10, 15, 120 });
                DrawCircleV(sm.position, sm.radius * 0.9f, Color{ 52, 56, 65, 255 });
                DrawCircleLines((int)sm.position.x, (int)sm.position.y, sm.radius, Color{ 100, 240, 180, 220 });
                float smPulse = 0.8f + 0.2f * sinf((float)GetTime() * 7.0f + s * 2.0f);
                DrawCircleV(sm.position, 3.5f * smPulse, Color{ 100, 255, 170, 255 });

                float sBarW = 20.0f;
                float sBarH = 3.0f;
                float sBarX = sm.position.x - sBarW * 0.5f;
                float sBarY = sm.position.y - sm.radius - 6.0f;
                DrawRectangle((int)sBarX, (int)sBarY, (int)sBarW, (int)sBarH, Color{ 20, 20, 26, 220 });
                float smPct = fmaxf(0.0f, sm.health / sm.maxHealth);
                DrawRectangle((int)sBarX, (int)sBarY, (int)(sBarW * smPct), (int)sBarH, Color{ 80, 240, 160, 255 });
                DrawRectangleLines((int)sBarX, (int)sBarY, (int)sBarW, (int)sBarH, Color{ 30, 45, 60, 200 });
            }
        } else {
            DrawCircleV(comp.position, comp.radius * 0.9f, Color{ 48, 52, 60, 255 });
            DrawCircleLines((int)comp.position.x, (int)comp.position.y, comp.radius, Color{ 100, 240, 180, 230 });
            float cPulse = 0.8f + 0.2f * sinf((float)GetTime() * 6.0f);
            DrawCircleV(comp.position, 5.0f * cPulse, Color{ 100, 255, 170, 255 });

            float barW = 28.0f;
            float barH = 4.0f;
            float barX = comp.position.x - barW * 0.5f;
            float barY = comp.position.y - comp.radius - 8.0f;
            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 20, 20, 26, 220 });
            float pct = fmaxf(0.0f, comp.health / comp.maxHealth);
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 80, 240, 160, 255 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 30, 45, 60, 200 });
        }
    }

    // Friendly Tower Companions Squad (Conqueror's Totem: up to 7 gladiators)
    if (player.GetTotem() == TotemType::CONQUEROR_TOTEM) {
        const auto& squad = player.GetTowerCompanions();
        for (const auto& comp : squad) {
            if (!comp.active || comp.health <= 0.0f) continue;

            // Ground shadow
            DrawEllipse((int)comp.position.x, (int)(comp.position.y + comp.radius * 0.7f), comp.radius * 0.85f, comp.radius * 0.4f, Color{ 10, 10, 15, 120 });

            // Unit base silhouette color
            Color unitCol = Color{ 48, 50, 58, 255 };
            switch (comp.unitType) {
                case EnemyType::SLASHER: unitCol = Color{ 65, 32, 42, 255 }; break;
                case EnemyType::BRUTE: unitCol = Color{ 45, 45, 54, 255 }; break;
                case EnemyType::CHARGER: unitCol = Color{ 38, 44, 62, 255 }; break;
                case EnemyType::ARCHER: unitCol = Color{ 38, 54, 44, 255 }; break;
                case EnemyType::PYROMANCER: unitCol = Color{ 52, 32, 68, 255 }; break;
                case EnemyType::ORE_GOLEM: unitCol = Color{ 58, 52, 46, 255 }; break;
                case EnemyType::FROST_TROLL: unitCol = Color{ 42, 56, 68, 255 }; break;
                default: break;
            }

            DrawCircleV(comp.position, comp.radius * 0.85f, unitCol);

            // Imperial Conqueror purple and gold heraldic crest
            DrawCircleLines((int)comp.position.x, (int)comp.position.y, comp.radius, Color{ 215, 75, 160, 230 });
            DrawCircleLines((int)comp.position.x, (int)comp.position.y, comp.radius - 2.0f, Color{ 255, 215, 80, 180 });

            // Active attack flash
            if (comp.attackVisualTimer > 0.0f) {
                DrawCircleLines((int)comp.position.x, (int)comp.position.y, comp.radius + 5.0f, Color{ 255, 235, 120, 240 });
            }

            // Overhead health bar
            float barW = comp.radius * 1.6f;
            float barH = 3.5f;
            float barX = comp.position.x - barW * 0.5f;
            float barY = comp.position.y - comp.radius - 7.0f;
            DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, Color{ 20, 20, 26, 220 });
            float pct = fmaxf(0.0f, comp.health / comp.maxHealth);
            DrawRectangle((int)barX, (int)barY, (int)(barW * pct), (int)barH, Color{ 215, 90, 175, 255 });
            DrawRectangleLines((int)barX, (int)barY, (int)barW, (int)barH, Color{ 45, 28, 52, 200 });
        }

        // Squad wiped out: show overhead 2-minute respawn timer near player
        if (player.GetActiveTowerCompanionCount() == 0 && player.GetTowerSquadRespawnTimer() > 0.0f) {
            int remainingSec = (int)ceilf(player.GetTowerSquadRespawnTimer());
            int min = remainingSec / 60;
            int sec = remainingSec % 60;
            char timerBuf[64];
            snprintf(timerBuf, sizeof(timerBuf), "SQUAD RESPAWN: %dm %02ds", min, sec);
            int textW = FontManager::MeasureGameText(timerBuf, 11, GameFont::BODY);
            Vector2 pPos = player.GetPosition();
            DrawRectangle((int)(pPos.x - textW * 0.5f - 6), (int)(pPos.y - player.GetRadius() - 24), textW + 12, 16, Color{ 20, 12, 25, 210 });
            DrawRectangleLines((int)(pPos.x - textW * 0.5f - 6), (int)(pPos.y - player.GetRadius() - 24), textW + 12, 16, Color{ 215, 80, 160, 220 });
            FontManager::DrawGameText(timerBuf, pPos.x - textW * 0.5f, pPos.y - player.GetRadius() - 22, 11, Color{ 255, 215, 120, 255 }, GameFont::BODY);
        }
    }

    // Friendly Mist Fairy Companion (Totem of Mist)
    if (player.GetTotem() == TotemType::TOTEM_MIST) {
        Vector2 fPos = player.GetMistFairy().position;
        if (fPos.x == 0.0f && fPos.y == 0.0f) {
            fPos = { player.GetPosition().x - 32.0f, player.GetPosition().y - 20.0f };
        }
        float time = (float)GetTime();
        float wingFlap = sinf(time * 18.0f) * 8.0f;

        DrawCircleGradient(fPos, 22.0f, Color{ 80, 240, 220, 90 }, Color{ 0, 0, 0, 0 });

        Vector2 w1L = { fPos.x - 2.0f, fPos.y - 3.0f };
        Vector2 w2L = { fPos.x - 12.0f, fPos.y - 12.0f + wingFlap };
        Vector2 w3L = { fPos.x - 10.0f, fPos.y + 4.0f };
        DrawTriangle(w1L, w2L, w3L, Color{ 140, 255, 235, 180 });

        Vector2 w1R = { fPos.x + 2.0f, fPos.y - 3.0f };
        Vector2 w2R = { fPos.x + 12.0f, fPos.y - 12.0f - wingFlap };
        Vector2 w3R = { fPos.x + 10.0f, fPos.y + 4.0f };
        DrawTriangle(w1R, w3R, w2R, Color{ 140, 255, 235, 180 });

        DrawCircleV(fPos, 4.5f, Color{ 60, 210, 195, 240 });
        DrawCircleV(fPos, 2.5f, Color{ 255, 255, 255, 255 });
    }

    // Active Mist Healing Orbs on ground
    for (const auto& orb : player.GetMistHealingOrbs()) {
        if (!orb.active) continue;
        float pulse = 0.8f + 0.2f * sinf((float)GetTime() * 5.0f);
        DrawCircleGradient(orb.position, 28.0f * pulse, Color{ 90, 255, 190, 100 }, Color{ 0, 0, 0, 0 });
        DrawCircleV(orb.position, 7.0f * pulse, Color{ 40, 180, 140, 240 });
        DrawCircleV(orb.position, 4.5f, Color{ 140, 255, 220, 255 });
        DrawCircleV(orb.position, 2.0f, Color{ 255, 255, 255, 255 });
        DrawCircleLines((int)orb.position.x, (int)orb.position.y, 9.0f * pulse, Color{ 120, 255, 210, 220 });
    }

    // Shadowstrider Evasion Smoke Mist Zones
    for (const auto& zone : player.GetSmokeMistZones()) {
        if (!zone.active) continue;
        float alpha = fminf(1.0f, zone.duration / 2.0f);
        Color smokeCol = Color{ 70, 40, 110, (unsigned char)(110 * alpha) };
        DrawCircleGradient(zone.position, zone.radius, smokeCol, Color{ 0, 0, 0, 0 });
        DrawCircleLines((int)zone.position.x, (int)zone.position.y, zone.radius * 0.85f, Color{ 140, 80, 220, (unsigned char)(130 * alpha) });
        // Poisonous inner toxic mist ring
        float pulse = 0.85f + 0.15f * sinf((float)GetTime() * 4.0f + zone.position.x);
        DrawCircleLines((int)zone.position.x, (int)zone.position.y, zone.radius * 0.45f * pulse, Color{ 80, 230, 130, (unsigned char)(110 * alpha) });
    }
}

