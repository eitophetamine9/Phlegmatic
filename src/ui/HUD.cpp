#include "HUD.hpp"
#include "../combat/Weapon.hpp"
#include "../entities/Player.hpp"
#include "../entities/Enemy.hpp"
#include "../world/TimeOfDay.hpp"
#include "../rendering/AssetManager.hpp"
#include "../rendering/FontManager.hpp"
#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

static void DrawResourceBar(Rectangle rec, float current, float max, Color fillColor, Color bgColor, Color borderColor, bool showTicks = true) {
    DrawRectangleRounded(rec, 0.25f, 4, bgColor);

    float pct = (max > 0.001f) ? fmaxf(0.0f, fminf(1.0f, current / max)) : 0.0f;
    float fillW = rec.width * pct;

    if (fillW > 0.0f) {
        Rectangle fillRec = { rec.x, rec.y, fillW, rec.height };
        DrawRectangleRounded(fillRec, 0.25f, 4, fillColor);

        // Specular top shine gradient highlight
        if (rec.height >= 8.0f) {
            Rectangle shineRec = { rec.x + 1.0f, rec.y + 1.0f, fmaxf(0.0f, fillW - 2.0f), rec.height * 0.40f };
            DrawRectangleRounded(shineRec, 0.2f, 4, Color{ 255, 255, 255, 45 });
        }
    }

    // Graduation ticks at 25%, 50%, and 75%
    if (showTicks && rec.width >= 60.0f) {
        float t25 = rec.x + rec.width * 0.25f;
        float t50 = rec.x + rec.width * 0.50f;
        float t75 = rec.x + rec.width * 0.75f;

        // 25% tick
        DrawLineEx({ t25, rec.y }, { t25, rec.y + rec.height }, 1.0f, Color{ 0, 0, 0, 150 });
        DrawLineEx({ t25 + 1.0f, rec.y }, { t25 + 1.0f, rec.y + rec.height }, 1.0f, Color{ 255, 255, 255, 35 });

        // 50% tick (accentuated mid-marker)
        DrawLineEx({ t50, rec.y }, { t50, rec.y + rec.height }, 1.5f, Color{ 0, 0, 0, 180 });
        DrawLineEx({ t50 + 1.0f, rec.y }, { t50 + 1.0f, rec.y + rec.height }, 1.0f, Color{ 255, 255, 255, 60 });

        // 75% tick
        DrawLineEx({ t75, rec.y }, { t75, rec.y + rec.height }, 1.0f, Color{ 0, 0, 0, 150 });
        DrawLineEx({ t75 + 1.0f, rec.y }, { t75 + 1.0f, rec.y + rec.height }, 1.0f, Color{ 255, 255, 255, 35 });
    }

    DrawRectangleRoundedLines(rec, 0.25f, 4, borderColor);
}

static void DrawLowHealthVignette(int windowWidth, int windowHeight, float currentHp, float maxHp) {
    if (maxHp <= 0.001f || currentHp <= 0.0f) return;
    float ratio = currentHp / maxHp;
    if (ratio >= 0.25f) return;

    float dangerFactor = 1.0f - (ratio / 0.25f);
    float pulse = (sinf((float)GetTime() * 6.0f) + 1.0f) * 0.5f;
    unsigned char alpha = (unsigned char)((45.0f + 55.0f * pulse) * dangerFactor);

    int bandSize = 48;
    DrawRectangleGradientV(0, 0, windowWidth, bandSize, Color{ 220, 25, 35, alpha }, Color{ 220, 25, 35, 0 });
    DrawRectangleGradientV(0, windowHeight - bandSize, windowWidth, bandSize, Color{ 220, 25, 35, 0 }, Color{ 220, 25, 35, alpha });
    DrawRectangleGradientH(0, 0, bandSize, windowHeight, Color{ 220, 25, 35, alpha }, Color{ 220, 25, 35, 0 });
    DrawRectangleGradientH(windowWidth - bandSize, 0, bandSize, windowHeight, Color{ 220, 25, 35, 0 }, Color{ 220, 25, 35, alpha });
}

static void DrawActionSlotIcon(Vector2 c, ActionIconType type, ArmorSet armorSet, float s, Color tint, bool isDimmed) {
    Color col = isDimmed ? ColorAlpha(tint, 0.42f) : tint;
    Color accent = isDimmed ? Color{ 195, 205, 220, 120 } : Color{ 245, 250, 255, 240 };

    switch (type) {
        case ActionIconType::DASH: {
            // Sweeping motion wake streaks
            DrawLineEx({ c.x - 10.0f * s, c.y - 5.0f * s }, { c.x - 2.0f * s, c.y - 5.0f * s }, 1.6f * s, ColorAlpha(col, 0.65f));
            DrawLineEx({ c.x - 12.0f * s, c.y }, { c.x - 1.0f * s, c.y }, 1.8f * s, ColorAlpha(col, 0.85f));
            DrawLineEx({ c.x - 10.0f * s, c.y + 5.0f * s }, { c.x - 2.0f * s, c.y + 5.0f * s }, 1.6f * s, ColorAlpha(col, 0.65f));
            // Bold directional double chevron
            DrawLineEx({ c.x - 2.0f * s, c.y - 7.5f * s }, { c.x + 5.0f * s, c.y }, 2.2f * s, col);
            DrawLineEx({ c.x - 2.0f * s, c.y + 7.5f * s }, { c.x + 5.0f * s, c.y }, 2.2f * s, col);
            DrawLineEx({ c.x + 3.5f * s, c.y - 6.0f * s }, { c.x + 10.0f * s, c.y }, 2.4f * s, accent);
            DrawLineEx({ c.x + 3.5f * s, c.y + 6.0f * s }, { c.x + 10.0f * s, c.y }, 2.4f * s, accent);
            break;
        }
        case ActionIconType::LIGHT_ATTACK: {
            // Quick diagonal slash blade + crescent arc
            Vector2 hilt = { c.x - 8.5f * s, c.y + 8.5f * s };
            Vector2 tip = { c.x + 8.5f * s, c.y - 8.5f * s };
            DrawLineEx(hilt, tip, 4.0f * s, ColorAlpha(col, 0.45f));
            DrawLineEx(hilt, tip, 2.2f * s, accent);
            DrawLineEx({ c.x - 9.0f * s, c.y + 2.0f * s }, { c.x - 2.0f * s, c.y + 9.0f * s }, 2.2f * s, Color{ 225, 195, 105, col.a });
            DrawCircleSectorLines({ c.x - 1.0f * s, c.y + 1.0f * s }, 11.5f * s, 215.0f, 345.0f, 12, col);
            break;
        }
        case ActionIconType::HEAVY_ATTACK: {
            // Crushing greatsword cleave onto radial impact star
            Vector2 grip = { c.x - 9.5f * s, c.y - 9.5f * s };
            Vector2 tip = { c.x + 6.5f * s, c.y + 6.5f * s };
            DrawLineEx(grip, tip, 5.5f * s, ColorAlpha(col, 0.55f));
            DrawLineEx(grip, tip, 2.8f * s, accent);
            DrawLineEx({ c.x - 10.0f * s, c.y - 2.5f * s }, { c.x - 2.5f * s, c.y - 10.0f * s }, 2.5f * s, col);
            Vector2 impact = { c.x + 5.5f * s, c.y + 5.5f * s };
            DrawLineEx({ impact.x - 5.5f * s, impact.y }, { impact.x + 5.5f * s, impact.y }, 1.6f * s, col);
            DrawLineEx({ impact.x, impact.y - 5.5f * s }, { impact.x, impact.y + 5.5f * s }, 1.6f * s, col);
            DrawCircleLines((int)impact.x, (int)impact.y, 4.5f * s, ColorAlpha(col, 0.75f));
            break;
        }
        case ActionIconType::PARRY: {
            // Kite shield + top deflection spark arc
            Vector2 pTopL = { c.x - 8.5f * s, c.y - 8.0f * s };
            Vector2 pTopR = { c.x + 8.5f * s, c.y - 8.0f * s };
            Vector2 pMidL = { c.x - 7.5f * s, c.y + 1.0f * s };
            Vector2 pMidR = { c.x + 7.5f * s, c.y + 1.0f * s };
            Vector2 pBot  = { c.x, c.y + 10.5f * s };
            Color shieldFill = Color{ (unsigned char)(col.r / 4), (unsigned char)(col.g / 4), (unsigned char)(col.b / 4), col.a };
            DrawTriangle(pTopL, pMidL, pTopR, shieldFill);
            DrawTriangle(pTopR, pMidL, pMidR, shieldFill);
            DrawTriangle(pMidL, pBot, pMidR, shieldFill);
            DrawLineEx(pTopL, pTopR, 2.0f * s, col);
            DrawLineEx(pTopL, pMidL, 2.0f * s, col);
            DrawLineEx(pTopR, pMidR, 2.0f * s, col);
            DrawLineEx(pMidL, pBot, 2.0f * s, col);
            DrawLineEx(pMidR, pBot, 2.0f * s, col);
            // Spark deflect gleam
            DrawLineEx({ c.x - 10.5f * s, c.y - 10.0f * s }, { c.x - 4.0f * s, c.y - 4.0f * s }, 2.0f * s, accent);
            DrawCircleV({ c.x - 4.0f * s, c.y - 4.0f * s }, 2.2f * s, accent);
            break;
        }
        case ActionIconType::POTION_HEAL: {
            // Rounded emerald-rose restorative flask with bold cross
            DrawRectangleRounded(Rectangle{ c.x - 3.0f * s, c.y - 11.5f * s, 6.0f * s, 4.0f * s }, 0.3f, 2, Color{ 165, 130, 85, col.a });
            DrawRectangleRounded(Rectangle{ c.x - 3.5f * s, c.y - 8.0f * s, 7.0f * s, 3.5f * s }, 0.25f, 2, accent);
            DrawCircleV({ c.x, c.y + 2.5f * s }, 8.5f * s, Color{ 18, 42, 30, col.a });
            DrawCircleV({ c.x, c.y + 3.0f * s }, 7.0f * s, col);
            DrawCircleLines((int)c.x, (int)(c.y + 2.5f * s), 8.5f * s, accent);
            DrawLineEx({ c.x - 3.6f * s, c.y + 3.0f * s }, { c.x + 3.6f * s, c.y + 3.0f * s }, 2.2f * s, accent);
            DrawLineEx({ c.x, c.y - 0.6f * s }, { c.x, c.y + 6.6f * s }, 2.2f * s, accent);
            break;
        }
        case ActionIconType::POTION_STONE: {
            // Heavy golden tonic bottle with diamond ward crest
            DrawRectangleRounded(Rectangle{ c.x - 3.0f * s, c.y - 11.5f * s, 6.0f * s, 4.0f * s }, 0.3f, 2, Color{ 165, 130, 85, col.a });
            DrawPoly({ c.x, c.y + 2.5f * s }, 6, 9.0f * s, 30.0f, col);
            DrawPolyLinesEx({ c.x, c.y + 2.5f * s }, 6, 9.0f * s, 30.0f, 1.5f * s, accent);
            DrawPoly({ c.x, c.y + 2.5f * s }, 4, 4.0f * s, 0.0f, Color{ 45, 32, 12, col.a });
            break;
        }
        case ActionIconType::POTION_THAW: {
            // Icy tall phial with 6-point snowflake
            DrawRectangleRounded(Rectangle{ c.x - 2.5f * s, c.y - 11.5f * s, 5.0f * s, 3.5f * s }, 0.3f, 2, accent);
            DrawPoly({ c.x, c.y + 2.5f * s }, 6, 8.8f * s, 0.0f, Color{ 16, 48, 70, col.a });
            DrawPolyLinesEx({ c.x, c.y + 2.5f * s }, 6, 8.8f * s, 0.0f, 1.6f * s, col);
            for (int k = 0; k < 3; ++k) {
                float a = (float)k * 60.0f * DEG2RAD;
                Vector2 p1 = { c.x + cosf(a) * 5.2f * s, c.y + 2.5f * s + sinf(a) * 5.2f * s };
                Vector2 p2 = { c.x - cosf(a) * 5.2f * s, c.y + 2.5f * s - sinf(a) * 5.2f * s };
                DrawLineEx(p1, p2, 1.4f * s, accent);
            }
            break;
        }
        case ActionIconType::POTION_BERSERK: {
            // Crimson flared brew flask with jagged flame crest
            DrawRectangleRounded(Rectangle{ c.x - 3.0f * s, c.y - 11.5f * s, 6.0f * s, 3.8f * s }, 0.3f, 2, Color{ 140, 85, 70, col.a });
            DrawTriangle({ c.x, c.y - 7.5f * s }, { c.x - 8.5f * s, c.y + 9.5f * s }, { c.x + 8.5f * s, c.y + 9.5f * s }, col);
            DrawTriangleLines({ c.x, c.y - 7.5f * s }, { c.x - 8.5f * s, c.y + 9.5f * s }, { c.x + 8.5f * s, c.y + 9.5f * s }, accent);
            DrawTriangle({ c.x, c.y - 2.0f * s }, { c.x - 3.5f * s, c.y + 7.0f * s }, { c.x + 3.5f * s, c.y + 7.0f * s }, Color{ 255, 220, 120, col.a });
            break;
        }
        case ActionIconType::ARMOR_SKILL: {
            if (armorSet == ArmorSet::FROSTWARDEN) {
                // Radiant 8-point glacial ice nova star
                for (int k = 0; k < 4; ++k) {
                    float a = (float)k * 45.0f * DEG2RAD;
                    float len = (k % 2 == 0) ? 10.5f * s : 7.5f * s;
                    DrawLineEx({ c.x + cosf(a) * len, c.y + sinf(a) * len },
                               { c.x - cosf(a) * len, c.y - sinf(a) * len }, 2.0f * s, col);
                }
                DrawPoly({ c.x, c.y }, 4, 4.2f * s, 0.0f, accent);
            } else if (armorSet == ArmorSet::CRUSADER_PLATE) {
                DrawCircleLines((int)c.x, (int)c.y, 9.5f * s, col);
                DrawCircleLines((int)c.x, (int)c.y, 6.0f * s, accent);
                DrawLineEx({ c.x - 10.5f * s, c.y }, { c.x + 10.5f * s, c.y }, 1.6f * s, col);
                DrawLineEx({ c.x, c.y - 10.5f * s }, { c.x, c.y + 10.5f * s }, 1.6f * s, col);
            } else if (armorSet == ArmorSet::VANGUARD_BASTION) {
                DrawTriangle({ c.x + 10.0f * s, c.y }, { c.x - 2.0f * s, c.y - 8.0f * s }, { c.x - 2.0f * s, c.y + 8.0f * s }, col);
                DrawLineEx({ c.x - 9.0f * s, c.y - 6.0f * s }, { c.x - 3.0f * s, c.y }, 2.0f * s, accent);
                DrawLineEx({ c.x - 9.0f * s, c.y + 6.0f * s }, { c.x - 3.0f * s, c.y }, 2.0f * s, accent);
            } else if (armorSet == ArmorSet::SHADOWSTRIDER) {
                DrawTriangle({ c.x, c.y - 9.5f * s }, { c.x - 8.5f * s, c.y + 8.0f * s }, { c.x + 8.5f * s, c.y + 8.0f * s }, ColorAlpha(col, 0.35f));
                DrawCircleSectorLines(c, 8.5f * s, 25.0f, 165.0f, 10, col);
                DrawCircleSectorLines(c, 5.5f * s, 195.0f, 345.0f, 10, accent);
            } else if (armorSet == ArmorSet::BASALT_GOLEM) {
                DrawTriangle({ c.x - 6.5f * s, c.y - 7.0f * s }, { c.x - 10.5f * s, c.y + 8.0f * s }, { c.x - 2.5f * s, c.y + 8.0f * s }, col);
                DrawTriangle({ c.x, c.y - 10.5f * s }, { c.x - 4.0f * s, c.y + 8.0f * s }, { c.x + 4.0f * s, c.y + 8.0f * s }, accent);
                DrawTriangle({ c.x + 6.5f * s, c.y - 7.0f * s }, { c.x + 2.5f * s, c.y + 8.0f * s }, { c.x + 10.5f * s, c.y + 8.0f * s }, col);
            } else if (armorSet == ArmorSet::FORGED_SCRAP_IRON) {
                DrawPolyLinesEx(c, 6, 7.5f * s, 0.0f, 2.0f * s, col);
                for (int k = 0; k < 6; ++k) {
                    float a = (float)k * 60.0f * DEG2RAD;
                    DrawLineEx({ c.x + cosf(a) * 6.5f * s, c.y + sinf(a) * 6.5f * s },
                               { c.x + cosf(a) * 11.0f * s, c.y + sinf(a) * 11.0f * s }, 1.8f * s, accent);
                }
            } else if (armorSet == ArmorSet::COMMONERS_SCRAP) {
                DrawCircleLines((int)(c.x - 3.5f * s), (int)(c.y + 2.0f * s), 5.0f * s, col);
                DrawCircleLines((int)(c.x + 3.5f * s), (int)(c.y + 2.0f * s), 5.0f * s, col);
                DrawCircleLines((int)c.x, (int)(c.y - 3.0f * s), 5.5f * s, accent);
            } else {
                DrawRectangleRoundedLines(Rectangle{ c.x - 7.5f * s, c.y - 7.5f * s, 15.0f * s, 15.0f * s }, 0.3f, 2, ColorAlpha(col, 0.45f));
                DrawLineEx({ c.x - 4.5f * s, c.y }, { c.x + 4.5f * s, c.y }, 1.5f * s, ColorAlpha(col, 0.45f));
            }
            break;
        }
        case ActionIconType::NONE:
        default:
            break;
    }
}

static const char* GetShortArmorSkillLabel(ArmorSet set) {
    switch (set) {
        case ArmorSet::COMMONERS_SCRAP:   return "BURST";
        case ArmorSet::CRUSADER_PLATE:    return "AEGIS";
        case ArmorSet::VANGUARD_BASTION:  return "CHARGE";
        case ArmorSet::SHADOWSTRIDER:     return "DECOY";
        case ArmorSet::FORGED_SCRAP_IRON: return "SPIKES";
        case ArmorSet::BASALT_GOLEM:      return "SPIRES";
        case ArmorSet::FROSTWARDEN:       return "NOVA";
        case ArmorSet::NONE:
        default:                          return "NO SET";
    }
}

void HUD::DrawActionSlot(const ActionSlotInfo& slot) {
    const Rectangle& r = slot.bounds;
    bool onCooldown = slot.currentCooldown > 0.001f;
    bool outOfStock = (slot.stockCount == 0);

    // Background tile
    Color bgColor = outOfStock ? Color{ 24, 14, 18, 225 } : (slot.isActive ? Color{ 20, 32, 46, 240 } : Color{ 14, 18, 26, 232 });
    DrawRectangleRounded(r, 0.20f, 5, bgColor);

    // Subtle top sheen
    DrawRectangleRounded(Rectangle{ r.x + 2.0f, r.y + 2.0f, r.width - 4.0f, r.height * 0.26f }, 0.25f, 3, Color{ 255, 255, 255, 14 });

    // Cooldown wipe overlay
    if (onCooldown && slot.maxCooldown > 0.001f) {
        float ratio = fminf(1.0f, fmaxf(0.0f, slot.currentCooldown / slot.maxCooldown));
        float sweepH = r.height * ratio;
        float sweepY = r.y + (r.height - sweepH);
        DrawRectangleRounded(Rectangle{ r.x, sweepY, r.width, sweepH }, 0.20f, 4, Color{ 8, 10, 16, 200 });
        DrawLineEx({ r.x + 3.0f, sweepY }, { r.x + r.width - 3.0f, sweepY }, 1.5f, Color{ 200, 220, 255, 175 });
    }

    // Center Procedural Vector Icon
    if (slot.iconType != ActionIconType::NONE) {
        float iconScale = fmaxf(0.95f, r.width / 52.0f);
        Vector2 iconCenter = { r.x + r.width * 0.5f, r.y + r.height * 0.49f };
        DrawActionSlotIcon(iconCenter, slot.iconType, slot.armorSet, iconScale, slot.themeColor, onCooldown || outOfStock);
    }

    // Border
    Color borderColor;
    if (slot.isActive) {
        borderColor = slot.themeColor;
        DrawRectangleRoundedLines(Rectangle{ r.x - 2.0f, r.y - 2.0f, r.width + 4.0f, r.height + 4.0f }, 0.22f, 4, Color{ slot.themeColor.r, slot.themeColor.g, slot.themeColor.b, 185 });
    } else if (onCooldown) {
        borderColor = Color{ 52, 64, 84, 210 };
    } else if (outOfStock) {
        borderColor = Color{ 125, 42, 52, 200 };
    } else {
        borderColor = Color{ slot.themeColor.r, slot.themeColor.g, slot.themeColor.b, 205 };
    }
    DrawRectangleRoundedLines(r, 0.20f, 5, borderColor);

    // Keybind pill badge at top-left
    int kw = MeasureText(slot.keybind, 9);
    DrawRectangleRounded(Rectangle{ r.x + 3.0f, r.y + 3.0f, (float)kw + 6.0f, 12.0f }, 0.35f, 3, Color{ 10, 14, 22, 215 });
    DrawText(slot.keybind, (int)r.x + 6, (int)r.y + 4, 9, Color{ 220, 235, 255, 240 });

    // Stock badge at top-right (if consumable)
    if (slot.stockCount >= 0) {
        std::string stockStr = "x" + std::to_string(slot.stockCount);
        int sw = MeasureText(stockStr.c_str(), 9);
        float badgeW = (float)sw + 6.0f;
        float badgeH = 12.0f;
        float badgeX = r.x + r.width - badgeW - 3.0f;
        float badgeY = r.y + 3.0f;
        Color badgeBg = (slot.stockCount > 0) ? Color{ 20, 62, 36, 225 } : Color{ 62, 20, 26, 225 };
        Color badgeTxt = (slot.stockCount > 0) ? Color{ 130, 255, 175, 255 } : Color{ 255, 125, 125, 255 };
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 3, badgeBg);
        DrawText(stockStr.c_str(), (int)badgeX + 3, (int)badgeY + 2, 9, badgeTxt);
    }

    // Cooldown readout centered over dimmed icon
    if (onCooldown) {
        char cdBuf[16];
        if (slot.currentCooldown >= 10.0f) {
            snprintf(cdBuf, sizeof(cdBuf), "%.0fs", slot.currentCooldown);
        } else {
            snprintf(cdBuf, sizeof(cdBuf), "%.1fs", slot.currentCooldown);
        }
        int cw = MeasureText(cdBuf, 14);
        int cx = (int)(r.x + (r.width - cw) * 0.5f);
        int cy = (int)(r.y + (r.height - 14) * 0.48f);
        DrawText(cdBuf, cx + 1, cy + 1, 14, Color{ 0, 0, 0, 235 });
        DrawText(cdBuf, cx, cy, 14, Color{ 255, 242, 155, 255 });
    }

    // Bottom micro-label
    int labelSize = (r.width >= 60.0f) ? 10 : 9;
    int nw = MeasureText(slot.actionName, labelSize);
    int nx = (int)(r.x + (r.width - nw) * 0.5f);
    int ny = (int)(r.y + r.height - labelSize - 3.0f);
    Color txtCol = outOfStock ? Color{ 140, 100, 105, 185 } : (slot.isActive ? Color{ 255, 255, 255, 255 } : ColorAlpha(slot.themeColor, 0.92f));
    DrawText(slot.actionName, nx, ny, labelSize, txtCol);
}

void HUD::DrawActionBar(int windowWidth, int windowHeight, const Player& player) {
    const int numSlots = 9;
    const float slotW = fmaxf(54.0f, fminf(64.0f, (float)windowWidth * 0.043f));
    const float slotH = slotW + 7.0f;
    const float slotSpacing = (windowWidth >= 1400) ? 7.0f : 5.0f;
    const float totalW = numSlots * slotW + (numSlots - 1) * slotSpacing;

    // Keep ActionBar centered while preventing overlap with Bottom-Left PlayerCard (344px) and Bottom-Right Weapon Dock
    const float dockSlotW = (windowWidth >= 1400) ? 72.0f : 64.0f;
    const float dockGap = (windowWidth >= 1400) ? 8.0f : 6.0f;
    const float dockW = 3.0f * dockSlotW + 2.0f * dockGap + 20.0f;
    const float dockLeft = (float)windowWidth - dockW - 20.0f;
    const float playerCardRight = 24.0f + 320.0f;

    float barX = ((float)windowWidth - totalW) * 0.5f;
    if (windowWidth >= 1150) {
        float minBarX = playerCardRight + 18.0f;
        float maxBarX = dockLeft - totalW - 18.0f;
        if (minBarX <= maxBarX) {
            barX = fmaxf(minBarX, fminf(barX, maxBarX));
        }
    }
    const float barY = (float)windowHeight - slotH - 18.0f;

    // Backing glass plate
    Rectangle plate = { barX - 10.0f, barY - 16.0f, totalW + 20.0f, slotH + 24.0f };
    DrawRectangleRounded(plate, 0.20f, 6, Color{ 12, 16, 24, 220 });
    DrawRectangleRoundedLines(plate, 0.20f, 6, Color{ 48, 62, 85, 210 });

    // Active Combat Buff & Relic Status Tray (Positioned directly above Action Bar)
    struct BuffPill {
        std::string text;
        Color borderCol;
        Color textCol;
        Color bgCol;
        bool pulsing;
    };
    std::vector<BuffPill> activeBuffs;

    if (player.HasWillOfAtlasActive()) {
        char b[32];
        snprintf(b, sizeof(b), "WILL OF ATLAS: %.1fs", player.GetWillOfAtlasTimer());
        activeBuffs.push_back({ b, Color{ 255, 220, 75, 255 }, Color{ 255, 235, 130, 255 }, Color{ 50, 38, 12, 240 }, true });
    } else if (player.GetWillOfAtlasCooldown() > 0.0f) {
        char b[32];
        snprintf(b, sizeof(b), "ATLAS CD: %.0fs", player.GetWillOfAtlasCooldown());
        activeBuffs.push_back({ b, Color{ 160, 140, 95, 200 }, Color{ 200, 185, 145, 220 }, Color{ 24, 20, 16, 220 }, false });
    }

    if (player.GetArmorSkillShieldHp() > 0.0f) {
        char b[32];
        snprintf(b, sizeof(b), "WARD SHIELD: %d HP", (int)player.GetArmorSkillShieldHp());
        activeBuffs.push_back({ b, Color{ 90, 225, 255, 255 }, Color{ 160, 245, 255, 255 }, Color{ 14, 36, 48, 240 }, true });
    }

    if (player.IsInsideSmokeMist()) {
        activeBuffs.push_back({ "SMOKE MIST: +40% EVA", Color{ 75, 240, 180, 255 }, Color{ 140, 255, 210, 255 }, Color{ 12, 38, 28, 240 }, false });
    }

    if (player.IsInPhlegmaticTrance()) {
        char b[32];
        snprintf(b, sizeof(b), "PHLEGMATIC TRANCE: %.1fs", player.GetTranceTimer());
        activeBuffs.push_back({ b, Color{ 120, 235, 255, 255 }, Color{ 200, 250, 255, 255 }, Color{ 16, 40, 54, 240 }, true });
    }

    if (player.HasCounterBuff()) {
        activeBuffs.push_back({ "COUNTER BUFF: +50% DMG", Color{ 255, 215, 60, 255 }, Color{ 255, 235, 110, 255 }, Color{ 46, 36, 10, 240 }, true });
    }

    if (player.IsBleeding()) {
        char b[32];
        snprintf(b, sizeof(b), "BLEEDING: %.1fs", player.GetBleedTimer());
        activeBuffs.push_back({ b, Color{ 255, 70, 85, 255 }, Color{ 255, 140, 150, 255 }, Color{ 48, 14, 18, 240 }, true });
    }

    if (player.GetTotem() == TotemType::TOTEM_INSANITY && (player.GetHealth() / player.GetMaxHealth() <= 0.40f)) {
        activeBuffs.push_back({ "BLOOD FRENZY: +135% DMG", Color{ 255, 45, 60, 255 }, Color{ 255, 150, 160, 255 }, Color{ 52, 14, 18, 240 }, true });
    }

    if (player.GetTotem() == TotemType::TOTEM_PHLEGMATIC_CORE) {
        if (player.GetGrayHealth() > 0.0f) {
            char ghBuf[40];
            snprintf(ghBuf, sizeof(ghBuf), "GRAY HEALTH: %d HP", (int)player.GetGrayHealth());
            activeBuffs.push_back({ ghBuf, Color{ 185, 215, 240, 255 }, Color{ 230, 245, 255, 255 }, Color{ 30, 44, 60, 240 }, true });
        }
        activeBuffs.push_back({ "PHLEGMATIC APATHY: -80% KB", Color{ 140, 215, 255, 255 }, Color{ 200, 240, 255, 255 }, Color{ 20, 38, 55, 240 }, false });
    }

    if (player.HasStoneskinActive()) {
        char b[32];
        snprintf(b, sizeof(b), "STONESKIN: %.1fs", player.GetStoneskinTimer());
        activeBuffs.push_back({ b, Color{ 255, 205, 60, 255 }, Color{ 255, 235, 120, 255 }, Color{ 48, 38, 12, 240 }, true });
    }

    if (player.HasGlacialImmunityActive()) {
        char b[32];
        snprintf(b, sizeof(b), "GLACIAL WARMTH: %.1fs", player.GetGlacialImmunityTimer());
        activeBuffs.push_back({ b, Color{ 120, 230, 255, 255 }, Color{ 200, 245, 255, 255 }, Color{ 14, 38, 52, 240 }, true });
    }

    if (player.HasBerserkerActive()) {
        char b[32];
        snprintf(b, sizeof(b), "BERSERKER BREW: %.1fs", player.GetBerserkerTimer());
        activeBuffs.push_back({ b, Color{ 255, 50, 60, 255 }, Color{ 255, 140, 150, 255 }, Color{ 52, 14, 18, 240 }, true });
    }

    if (player.IsFrostbitten()) {
        activeBuffs.push_back({ "FROSTBITE: -20% SPD, -25% STAM REGEN", Color{ 120, 225, 255, 255 }, Color{ 200, 245, 255, 255 }, Color{ 14, 30, 48, 240 }, true });
    } else if (player.GetFrostbiteMeter() > 5.0f) {
        char fb[32];
        snprintf(fb, sizeof(fb), "CHILL: %d%%", (int)player.GetFrostbiteMeter());
        activeBuffs.push_back({ fb, Color{ 150, 215, 250, 255 }, Color{ 210, 240, 255, 255 }, Color{ 18, 35, 52, 220 }, false });
    }

    if (!activeBuffs.empty()) {
        float pillH = 18.0f;
        float pillSpacing = 8.0f;
        float pillY = plate.y - pillH - 6.0f;

        float totalPillsW = 0.0f;
        std::vector<float> pillWidths;
        for (const auto& pill : activeBuffs) {
            float pw = (float)MeasureText(pill.text.c_str(), 10) + 16.0f;
            pillWidths.push_back(pw);
            totalPillsW += pw;
        }
        totalPillsW += (float)(activeBuffs.size() - 1) * pillSpacing;

        float curPillX = plate.x + (plate.width - totalPillsW) * 0.5f;
        for (size_t i = 0; i < activeBuffs.size(); ++i) {
            const auto& pill = activeBuffs[i];
            float pw = pillWidths[i];
            Rectangle pRec = { curPillX, pillY, pw, pillH };

            Color bCol = pill.borderCol;
            if (pill.pulsing) {
                float pulse = (sinf((float)GetTime() * 8.0f) + 1.0f) * 0.5f;
                bCol.a = (unsigned char)(160 + 95 * pulse);
            }

            DrawRectangleRounded(pRec, 0.35f, 4, pill.bgCol);
            DrawRectangleRoundedLines(pRec, 0.35f, 4, bCol);
            DrawText(pill.text.c_str(), (int)pRec.x + 8, (int)pRec.y + 4, 10, pill.textCol);

            curPillX += pw + pillSpacing;
        }
    }

    // Status / Mode Banner centered right above the slots
    if (player.HasCounterBuff()) {
        const char* banner = "COUNTER BUFF ACTIVE: +50% DAMAGE";
        int bw = MeasureText(banner, 10);
        DrawText(banner, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 10, Color{ 255, 225, 80, 255 });
    } else if (player.IsParrying()) {
        const char* banner = "PARRY WINDOW ACTIVE";
        int bw = MeasureText(banner, 10);
        DrawText(banner, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 10, Color{ 80, 240, 255, 255 });
    } else if (player.IsStaggered()) {
        char stagBuf[64];
        snprintf(stagBuf, sizeof(stagBuf), "STAGGERED! (%.1fs)", player.GetStaggerTimer());
        int bw = MeasureText(stagBuf, 10);
        DrawText(stagBuf, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 10, Color{ 255, 180, 60, 255 });
    } else if (player.HasStaggerImmunity()) {
        const char* banner = "POSTURE GUARD (IMMUNITY)";
        int bw = MeasureText(banner, 10);
        DrawText(banner, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 10, Color{ 140, 220, 255, 220 });
    } else {
        const char* title = "ACTION ABILITIES";
        int bw = MeasureText(title, 9);
        DrawText(title, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 9, Color{ 140, 170, 205, 180 });
    }

    // Slot 1: Dash [Q]
    ActionSlotInfo dashSlot;
    dashSlot.bounds = { barX + 0 * (slotW + slotSpacing), barY, slotW, slotH };
    dashSlot.actionName = "DASH";
    dashSlot.keybind = "[Q]";
    dashSlot.currentCooldown = player.GetDashCooldown();
    dashSlot.maxCooldown = player.GetMaxDashCooldown();
    dashSlot.themeColor = Color{ 70, 160, 255, 255 };
    dashSlot.isActive = player.IsDashing();
    dashSlot.stockCount = -1;
    dashSlot.iconType = ActionIconType::DASH;
    DrawActionSlot(dashSlot);

    // Slot 2: Light Attack [LMB]
    ActionSlotInfo lightSlot;
    lightSlot.bounds = { barX + 1 * (slotW + slotSpacing), barY, slotW, slotH };
    lightSlot.actionName = "LIGHT";
    lightSlot.keybind = "[LMB]";
    lightSlot.currentCooldown = player.GetLightCooldown();
    lightSlot.maxCooldown = player.GetMaxLightCooldown();
    lightSlot.themeColor = Color{ 90, 230, 255, 255 };
    lightSlot.isActive = player.IsAttacking() && player.GetCurrentAttack().type == AttackType::LIGHT;
    lightSlot.stockCount = -1;
    lightSlot.iconType = ActionIconType::LIGHT_ATTACK;
    DrawActionSlot(lightSlot);

    // Slot 3: Heavy Attack [RMB]
    ActionSlotInfo heavySlot;
    heavySlot.bounds = { barX + 2 * (slotW + slotSpacing), barY, slotW, slotH };
    heavySlot.actionName = "HEAVY";
    heavySlot.keybind = "[RMB]";
    heavySlot.currentCooldown = player.GetHeavyCooldown();
    heavySlot.maxCooldown = player.GetMaxHeavyCooldown();
    heavySlot.themeColor = Color{ 255, 175, 50, 255 };
    heavySlot.isActive = player.IsHeavyWindingUp() || (player.IsAttacking() && player.GetCurrentAttack().type == AttackType::HEAVY);
    heavySlot.stockCount = -1;
    heavySlot.iconType = ActionIconType::HEAVY_ATTACK;
    DrawActionSlot(heavySlot);

    // Slot 4: Parry [F]
    ActionSlotInfo parrySlot;
    parrySlot.bounds = { barX + 3 * (slotW + slotSpacing), barY, slotW, slotH };
    parrySlot.actionName = "PARRY";
    parrySlot.keybind = "[F]";
    parrySlot.currentCooldown = player.GetParryCooldown();
    parrySlot.maxCooldown = player.GetMaxParryCooldown();
    parrySlot.themeColor = player.HasCounterBuff() ? Color{ 255, 225, 80, 255 } : Color{ 80, 240, 255, 255 };
    parrySlot.isActive = player.IsParrying() || player.HasCounterBuff();
    parrySlot.stockCount = -1;
    parrySlot.iconType = ActionIconType::PARRY;
    DrawActionSlot(parrySlot);

    // Slot 5: Health Potion [H]
    ActionSlotInfo potSlot;
    potSlot.bounds = { barX + 4 * (slotW + slotSpacing), barY, slotW, slotH };
    potSlot.actionName = "HEAL";
    potSlot.keybind = "[H]";
    potSlot.currentCooldown = player.GetPotionCooldown();
    potSlot.maxCooldown = 7.0f;
    potSlot.themeColor = Color{ 70, 230, 130, 255 };
    potSlot.isActive = false;
    potSlot.stockCount = player.GetInventory().GetHealthPotionCount();
    potSlot.iconType = ActionIconType::POTION_HEAL;
    DrawActionSlot(potSlot);

    // Slot 6: Stoneskin Tonic [4]
    ActionSlotInfo stoneSlot;
    stoneSlot.bounds = { barX + 5 * (slotW + slotSpacing), barY, slotW, slotH };
    stoneSlot.actionName = "STONE";
    stoneSlot.keybind = "[4]";
    stoneSlot.currentCooldown = player.GetStoneskinTimer();
    stoneSlot.maxCooldown = 15.0f;
    stoneSlot.themeColor = Color{ 245, 195, 50, 255 };
    stoneSlot.isActive = player.HasStoneskinActive();
    stoneSlot.stockCount = player.GetInventory().GetStoneskinTonicCount();
    stoneSlot.iconType = ActionIconType::POTION_STONE;
    DrawActionSlot(stoneSlot);

    // Slot 7: Glacial Draught [5]
    ActionSlotInfo thawSlot;
    thawSlot.bounds = { barX + 6 * (slotW + slotSpacing), barY, slotW, slotH };
    thawSlot.actionName = "THAW";
    thawSlot.keybind = "[5]";
    thawSlot.currentCooldown = player.GetGlacialImmunityTimer();
    thawSlot.maxCooldown = 90.0f;
    thawSlot.themeColor = Color{ 110, 225, 255, 255 };
    thawSlot.isActive = player.HasGlacialImmunityActive();
    thawSlot.stockCount = player.GetInventory().GetGlacialDraughtCount();
    thawSlot.iconType = ActionIconType::POTION_THAW;
    DrawActionSlot(thawSlot);

    // Slot 8: Berserker Brew [6]
    ActionSlotInfo berserkSlot;
    berserkSlot.bounds = { barX + 7 * (slotW + slotSpacing), barY, slotW, slotH };
    berserkSlot.actionName = "RAGE";
    berserkSlot.keybind = "[6]";
    berserkSlot.currentCooldown = player.GetBerserkerTimer();
    berserkSlot.maxCooldown = 12.0f;
    berserkSlot.themeColor = Color{ 255, 55, 65, 255 };
    berserkSlot.isActive = player.HasBerserkerActive();
    berserkSlot.stockCount = player.GetInventory().GetBerserkerDraughtCount();
    berserkSlot.iconType = ActionIconType::POTION_BERSERK;
    DrawActionSlot(berserkSlot);

    // Slot 9: Armor Skill [V]
    ArmorSet curSet = player.GetActiveArmorSet();
    ArmorSetInfo setInfo = GetArmorSetInfo(curSet);
    ActionSlotInfo armorSkillSlot;
    armorSkillSlot.bounds = { barX + 8 * (slotW + slotSpacing), barY, slotW, slotH };
    armorSkillSlot.actionName = GetShortArmorSkillLabel(curSet);
    armorSkillSlot.keybind = "[V]";
    armorSkillSlot.currentCooldown = player.GetArmorSkillCooldown();
    armorSkillSlot.maxCooldown = (curSet != ArmorSet::NONE) ? setInfo.skillCooldown : 30.0f;
    armorSkillSlot.themeColor = (curSet == ArmorSet::FROSTWARDEN) ? Color{ 120, 230, 255, 255 } :
                                ((curSet != ArmorSet::NONE) ? Color{ 255, 215, 80, 255 } : Color{ 110, 125, 145, 160 });
    armorSkillSlot.isActive = (player.GetArmorSkillShieldHp() > 0.0f) || (player.GetArmorSkillActiveTimer() > 0.0f);
    armorSkillSlot.stockCount = -1;
    armorSkillSlot.iconType = ActionIconType::ARMOR_SKILL;
    armorSkillSlot.armorSet = curSet;
    DrawActionSlot(armorSkillSlot);
}

static void DrawPlayerCard(int windowWidth, int windowHeight, const Player& player) {
    // Low health threat pulse vignette across screen edges
    DrawLowHealthVignette(windowWidth, windowHeight, player.GetHealth(), player.GetMaxHealth());

    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 138.0f;
    const float hudWidth = 320.0f;
    const float hudHeight = 114.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 215 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    // HEALTH BAR (Refined with graduation ticks, glass highlight & metallic borders)
    DrawText("HP", (int)hudX + 14, (int)hudY + 12, 11, Color{ 235, 80, 90, 255 });
    Rectangle hpRec = { hudX + 68, hudY + 12, 170, 12 };
    DrawResourceBar(hpRec, player.GetHealth(), player.GetMaxHealth(), Color{ 235, 55, 75, 255 }, Color{ 32, 20, 24, 220 }, Color{ 85, 40, 50, 220 }, true);
    if (player.GetGrayHealth() > 0.0f && player.GetMaxHealth() > 0.001f) {
        float maxH = player.GetMaxHealth();
        float hpPct = fmaxf(0.0f, fminf(1.0f, player.GetHealth() / maxH));
        float grayPct = fmaxf(0.0f, fminf(1.0f - hpPct, player.GetGrayHealth() / maxH));
        float startX = hpRec.x + hpRec.width * hpPct;
        float grayW = hpRec.width * grayPct;
        if (grayW > 0.0f) {
            float pulse = 0.5f + 0.5f * sinf((float)GetTime() * 8.0f);
            Color grayCol = Color{ (unsigned char)(180 + 30 * pulse), (unsigned char)(195 + 25 * pulse), (unsigned char)(215 + 20 * pulse), 190 };
            DrawRectangleRounded(Rectangle{ startX, hpRec.y + 1, grayW, hpRec.height - 2 }, 0.2f, 4, grayCol);
        }
    }
    char hpBuf[32];
    snprintf(hpBuf, sizeof(hpBuf), "%d/%d", (int)player.GetHealth(), (int)player.GetMaxHealth());
    DrawText(hpBuf, (int)hudX + 246, (int)hudY + 12, 10, Color{ 230, 205, 210, 225 });

    // STAMINA BAR
    DrawText("STAM", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    Rectangle stamRec = { hudX + 68, hudY + 32, 170, 12 };
    DrawResourceBar(stamRec, player.GetStamina(), player.GetMaxStamina(), Color{ 255, 195, 45, 255 }, Color{ 35, 30, 16, 220 }, Color{ 90, 75, 32, 220 }, true);
    char stamBuf[32];
    snprintf(stamBuf, sizeof(stamBuf), "%d/%d", (int)player.GetStamina(), (int)player.GetMaxStamina());
    DrawText(stamBuf, (int)hudX + 246, (int)hudY + 32, 10, Color{ 245, 230, 190, 225 });

    // POISE (COMPOSURE) BAR
    Color poiseLabelCol = Color{ 140, 210, 230, 255 };
    Color poiseFillCol = Color{ 70, 180, 210, 255 };
    if (player.IsInPhlegmaticTrance()) {
        float pulse = (sinf((float)GetTime() * 10.0f) + 1.0f) * 0.5f;
        poiseLabelCol = Color{ 180, 245, 255, 255 };
        poiseFillCol = Color{ (unsigned char)(140 + 80 * pulse), (unsigned char)(220 + 35 * pulse), 255, 255 };
    } else if (player.IsTranceReady()) {
        poiseLabelCol = Color{ 255, 235, 120, 255 };
        poiseFillCol = Color{ 120, 240, 255, 255 };
    } else if (player.HasStoicPoise()) {
        poiseLabelCol = Color{ 100, 215, 255, 255 };
        poiseFillCol = Color{ 80, 195, 255, 255 };
    } else if (player.IsDiscomposed()) {
        poiseLabelCol = Color{ 255, 160, 70, 255 };
        poiseFillCol = Color{ 240, 130, 50, 255 };
    }

    DrawText("POISE", (int)hudX + 14, (int)hudY + 52, 11, poiseLabelCol);
    Rectangle poiseRec = { hudX + 68, hudY + 52, 170, 12 };
    DrawResourceBar(poiseRec, player.GetComposure(), 100.0f, poiseFillCol, Color{ 18, 28, 36, 220 }, Color{ 50, 80, 100, 220 }, true);
    char poiseBuf[32];
    if (player.IsInPhlegmaticTrance()) {
        snprintf(poiseBuf, sizeof(poiseBuf), "%.1fs", player.GetTranceTimer());
    } else {
        snprintf(poiseBuf, sizeof(poiseBuf), "%d%%", (int)player.GetComposure());
    }
    DrawText(poiseBuf, (int)hudX + 246, (int)hudY + 52, 10, poiseLabelCol);

    // ATTRIBUTES ROW: DEFENSE & WILL
    char defBuf[32];
    snprintf(defBuf, sizeof(defBuf), "DEF: %d", (int)player.GetDefense());
    DrawText(defBuf, (int)hudX + 14, (int)hudY + 76, 11, Color{ 140, 210, 255, 240 });

    char willBuf[32];
    snprintf(willBuf, sizeof(willBuf), "WILL: %d", (int)player.GetWill());
    DrawText(willBuf, (int)hudX + 84, (int)hudY + 76, 11, Color{ 255, 205, 110, 240 });

    // Mini Modular Armor Badges
    const char* hStr = "H:-";
    Color hCol = Color{ 140, 160, 185, 200 };
    if (player.GetHelmet() == HelmetType::SCRAP_SALLET) { hStr = "H:Scrap"; hCol = Color{ 240, 180, 70, 255 }; }
    else if (player.GetHelmet() == HelmetType::BASALT_GREATHELM) { hStr = "H:Basalt"; hCol = Color{ 255, 140, 40, 255 }; }
    else if (player.GetHelmet() == HelmetType::SHADOW_COWL) { hStr = "H:Shadow"; hCol = Color{ 110, 245, 220, 255 }; }
    else if (player.GetHelmet() == HelmetType::CRUSADER_HELM) { hStr = "H:Crusader"; hCol = Color{ 255, 215, 80, 255 }; }
    else if (player.GetHelmet() == HelmetType::VANGUARD_GREATHELM) { hStr = "H:Vanguard"; hCol = Color{ 140, 215, 255, 255 }; }
    else if (player.GetHelmet() == HelmetType::FROSTWARDEN_HOOD) { hStr = "H:Frost"; hCol = Color{ 140, 220, 245, 255 }; }
    else if (player.GetHelmet() == HelmetType::COMMONERS_HOOD) { hStr = "H:Common"; hCol = Color{ 180, 200, 225, 255 }; }

    const char* cStr = "C:-";
    Color cCol = Color{ 140, 160, 185, 200 };
    if (player.GetChestplate() == ChestplateType::SCRAP_CUIRASS) { cStr = "C:Scrap"; cCol = Color{ 240, 180, 70, 255 }; }
    else if (player.GetChestplate() == ChestplateType::BASALT_CUIRASS) { cStr = "C:Basalt"; cCol = Color{ 255, 140, 40, 255 }; }
    else if (player.GetChestplate() == ChestplateType::SHADOW_GARB) { cStr = "C:Shadow"; cCol = Color{ 110, 245, 220, 255 }; }
    else if (player.GetChestplate() == ChestplateType::CRUSADER_CUIRASS) { cStr = "C:Crusader"; cCol = Color{ 255, 215, 80, 255 }; }
    else if (player.GetChestplate() == ChestplateType::VANGUARD_CHEST) { cStr = "C:Vanguard"; cCol = Color{ 140, 215, 255, 255 }; }
    else if (player.GetChestplate() == ChestplateType::FROSTWARDEN_HAUBERK) { cStr = "C:Frost"; cCol = Color{ 140, 220, 245, 255 }; }
    else if (player.GetChestplate() == ChestplateType::COMMONERS_TUNIC) { cStr = "C:Common"; cCol = Color{ 180, 200, 225, 255 }; }

    const char* lStr = "L:-";
    Color lCol = Color{ 140, 160, 185, 200 };
    if (player.GetLeggings() == LeggingsType::SCRAP_GREAVES) { lStr = "L:Scrap"; lCol = Color{ 240, 180, 70, 255 }; }
    else if (player.GetLeggings() == LeggingsType::BASALT_GREAVES) { lStr = "L:Basalt"; lCol = Color{ 255, 140, 40, 255 }; }
    else if (player.GetLeggings() == LeggingsType::SHADOW_LEGGINGS) { lStr = "L:Shadow"; lCol = Color{ 110, 245, 220, 255 }; }
    else if (player.GetLeggings() == LeggingsType::CRUSADER_GREAVES) { lStr = "L:Crusader"; lCol = Color{ 255, 215, 80, 255 }; }
    else if (player.GetLeggings() == LeggingsType::VANGUARD_GREAVES) { lStr = "L:Vanguard"; lCol = Color{ 140, 215, 255, 255 }; }
    else if (player.GetLeggings() == LeggingsType::FROSTWARDEN_GREAVES) { lStr = "L:Frost"; lCol = Color{ 140, 220, 245, 255 }; }
    else if (player.GetLeggings() == LeggingsType::COMMONERS_BREECHES) { lStr = "L:Common"; lCol = Color{ 180, 200, 225, 255 }; }

    DrawText(hStr, (int)hudX + 154, (int)hudY + 76, 10, hCol);
    DrawText(cStr, (int)hudX + 208, (int)hudY + 76, 10, cCol);
    DrawText(lStr, (int)hudX + 264, (int)hudY + 76, 10, lCol);

    ArmorSet activeSet = player.GetActiveArmorSet();
    if (activeSet != ArmorSet::NONE) {
        ArmorSetInfo setInfo = GetArmorSetInfo(activeSet);
        std::string setBonusStr = "SET BONUS: " + setInfo.name;
        DrawText(setBonusStr.c_str(), (int)hudX + 14, (int)hudY + 94, 9, Color{ 110, 245, 170, 240 });
    } else {
        DrawText("ARMOR: Modular Wardrobe", (int)hudX + 14, (int)hudY + 94, 9, Color{ 140, 165, 195, 180 });
    }

    // Friendly Mini Elemental Golem Companion Status Widget
    if (player.GetTotem() == TotemType::ELEMENTAL_CORE_TOTEM) {
        const auto& comp = player.GetCompanion();
        float compW = 260.0f;
        float compH = 46.0f;
        float compX = hudX;
        float compY = hudY - compH - 8.0f;

        DrawRectangleRounded(Rectangle{ compX, compY, compW, compH }, 0.2f, 4, Color{ 14, 20, 28, 225 });
        DrawRectangleRoundedLines(Rectangle{ compX, compY, compW, compH }, 0.2f, 4, comp.active ? Color{ 100, 245, 180, 220 } : Color{ 255, 120, 80, 200 });

        // Amber/emerald crystal icon
        DrawCircleV({ compX + 18.0f, compY + 23.0f }, 7.0f, comp.active ? Color{ 100, 245, 180, 255 } : Color{ 220, 80, 60, 255 });
        DrawCircleLines((int)(compX + 18.0f), (int)(compY + 23.0f), 7.0f, WHITE);

        if (comp.active) {
            DrawText("MINI GOLEM COMPANION", (int)compX + 32, (int)compY + 8, 10, Color{ 120, 245, 180, 255 });
            float barW = 120.0f;
            float barH = 8.0f;
            float pct = fmaxf(0.0f, comp.health / comp.maxHealth);
            DrawRectangle((int)compX + 32, (int)compY + 24, (int)barW, (int)barH, Color{ 24, 30, 40, 220 });
            DrawRectangle((int)compX + 32, (int)compY + 24, (int)(barW * pct), (int)barH, Color{ 80, 240, 160, 255 });
            DrawRectangleLines((int)compX + 32, (int)compY + 24, (int)barW, (int)barH, Color{ 40, 60, 80, 200 });

            char buf[32];
            snprintf(buf, sizeof(buf), "%d/%d HP", (int)comp.health, (int)comp.maxHealth);
            DrawText(buf, (int)compX + 160, (int)compY + 23, 10, Color{ 210, 240, 230, 220 });
        } else {
            DrawText("MINI GOLEM (SHATTERED)", (int)compX + 32, (int)compY + 8, 10, Color{ 255, 120, 90, 255 });
            char buf[48];
            snprintf(buf, sizeof(buf), "Respawning in %ds...", (int)ceilf(comp.respawnTimer));
            DrawText(buf, (int)compX + 32, (int)compY + 23, 10, Color{ 255, 200, 140, 240 });
        }
    }

    // Friendly Mist Fairy Companion Status Widget
    if (player.GetTotem() == TotemType::TOTEM_MIST) {
        float fairyW = 280.0f;
        float fairyH = 46.0f;
        float fairyX = hudX;
        float fairyY = hudY - fairyH - 8.0f;

        DrawRectangleRounded(Rectangle{ fairyX, fairyY, fairyW, fairyH }, 0.2f, 4, Color{ 12, 22, 28, 230 });
        DrawRectangleRoundedLines(Rectangle{ fairyX, fairyY, fairyW, fairyH }, 0.2f, 4, Color{ 90, 240, 220, 220 });

        DrawCircleV({ fairyX + 18.0f, fairyY + 23.0f }, 7.0f, Color{ 100, 255, 230, 255 });
        DrawCircleLines((int)(fairyX + 18.0f), (int)(fairyY + 23.0f), 7.0f, WHITE);

        DrawText("MIST FAIRY (+20% CRIT | +35% CRIT DMG)", (int)fairyX + 32, (int)fairyY + 8, 10, Color{ 120, 255, 230, 255 });
        int secRemaining = (int)ceilf(player.GetMistFairy().healOrbTimer);
        std::string orbStatus = "Healing Orb Ready in " + std::to_string(secRemaining) + "s...";
        DrawText(orbStatus.c_str(), (int)fairyX + 32, (int)fairyY + 23, 10, Color{ 180, 245, 225, 220 });
    }

    // STATUS BADGE PILL
    const float badgeX = hudX + 172.0f;
    const float badgeY = hudY + 74.0f;
    const float badgeW = 136.0f;
    const float badgeH = 20.0f;

    if (player.IsInPhlegmaticTrance()) {
        float pulse = (sinf((float)GetTime() * 8.0f) + 1.0f) * 0.5f;
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 20, 60, 85, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ (unsigned char)(140 + 80 * pulse), 240, 255, 255 });
        DrawText("TRANCE ACTIVE", (int)badgeX + 16, (int)badgeY + 5, 10, Color{ 160, 245, 255, 255 });
    } else if (player.IsTranceReady()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 50, 75, 40, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 130, 255, 160, 255 });
        DrawText("[SPACE] TRANCE", (int)badgeX + 14, (int)badgeY + 5, 10, Color{ 180, 255, 190, 255 });
    } else if (player.IsStaggered()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 70, 35, 15, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 255, 180, 50, 240 });
        DrawText("STAGGERED!", (int)badgeX + 24, (int)badgeY + 5, 10, Color{ 255, 210, 90, 255 });
    } else if (player.HasStoicPoise()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 20, 50, 75, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 80, 200, 255, 240 });
        DrawText("STOIC POISE", (int)badgeX + 24, (int)badgeY + 5, 10, Color{ 160, 230, 255, 255 });
    } else if (player.IsDiscomposed()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 60, 30, 20, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 240, 130, 50, 240 });
        DrawText("DISCOMPOSED", (int)badgeX + 20, (int)badgeY + 5, 10, Color{ 255, 170, 90, 255 });
    } else if (player.HasStaggerImmunity()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 20, 50, 70, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 100, 210, 255, 240 });
        DrawText("POISE GUARD", (int)badgeX + 22, (int)badgeY + 5, 10, Color{ 180, 235, 255, 255 });
    } else if (player.HasInvulnerability()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 20, 60, 60, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 80, 250, 210, 240 });
        DrawText("I-FRAMES", (int)badgeX + 36, (int)badgeY + 5, 10, Color{ 140, 255, 230, 255 });
    } else if (player.HasCounterBuff()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 65, 50, 15, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 255, 220, 60, 255 });
        DrawText("+50% COUNTER", (int)badgeX + 18, (int)badgeY + 5, 10, Color{ 255, 235, 100, 255 });
    } else {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 18, 28, 38, 210 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 60, 85, 115, 190 });
        DrawText("COMPOSED", (int)badgeX + 28, (int)badgeY + 5, 10, Color{ 160, 200, 235, 220 });
    }
}

static const char* GetShortWeaponLabel(WeaponType w) {
    switch (w) {
        case WeaponType::SWORD:              return "SWORD";
        case WeaponType::DAGGERS:            return "DAGGERS";
        case WeaponType::MACE:               return "MACE";
        case WeaponType::TRUTH_SEEKER:       return "TRUTH";
        case WeaponType::HALBERD:            return "HALBERD";
        case WeaponType::ZEPHYR_DAGGERS:     return "ZEPHYR";
        case WeaponType::WINDRUNNER_RECURVE: return "RECURVE";
        case WeaponType::SUN_CLAYMORE:       return "CLAYMORE";
        case WeaponType::STORM_GLAIVE:       return "GLAIVE";
        case WeaponType::GLACIAL_CLEAVER:    return "CLEAVERS";
        case WeaponType::NONE:
        default:                             return "EMPTY";
    }
}

void HUD::DrawWeaponHotbarDock(int windowWidth, int windowHeight, const Player& player) {
    const float slotW = (windowWidth >= 1400) ? 72.0f : 64.0f;
    const float slotH = (windowWidth >= 1400) ? 74.0f : 66.0f;
    const float gap = (windowWidth >= 1400) ? 8.0f : 6.0f;
    const float pad = 10.0f;
    const float dockW = 3.0f * slotW + 2.0f * gap + pad * 2.0f;
    const float dockH = slotH + 16.0f;
    const float dockX = (float)windowWidth - dockW - 20.0f;
    const float dockY = (float)windowHeight - dockH - 18.0f;

    // Active Weapon Name & Damage Readout Header Pill directly above dock
    WeaponType equipped = player.GetWeapon();
    int equippedTier = (equipped != WeaponType::NONE) ? player.GetInventory().GetWeaponUpgrade(equipped) : 0;
    WeaponStats activeStats = GetWeaponStats(equipped, equippedTier);

    Rectangle headerRec = { dockX, dockY - 28.0f, dockW, 24.0f };
    DrawRectangleRounded(headerRec, 0.25f, 4, Color{ 12, 16, 24, 225 });
    DrawRectangleRoundedLines(headerRec, 0.25f, 4, (equipped != WeaponType::NONE) ? Color{ 210, 175, 75, 220 } : Color{ 55, 72, 96, 200 });

    if (equipped != WeaponType::NONE) {
        char dmgTag[32];
        snprintf(dmgTag, sizeof(dmgTag), "%d/%d", (int)activeStats.lightDamage, (int)activeStats.heavyDamage);
        int dw = MeasureText(dmgTag, 10);
        int dmgX = (int)(headerRec.x + headerRec.width - (float)dw - 8.0f);
        DrawText(dmgTag, dmgX, (int)headerRec.y + 7, 10, Color{ 120, 230, 255, 245 });

        float maxNameW = (float)dmgX - headerRec.x - 14.0f;
        int nameSize = 10;
        std::string displayName = activeStats.name;
        if ((float)MeasureText(displayName.c_str(), nameSize) > maxNameW) {
            nameSize = 9;
            if ((float)MeasureText(displayName.c_str(), nameSize) > maxNameW) {
                displayName = GetShortWeaponLabel(equipped);
                if (equippedTier >= 1) displayName += " +" + std::to_string(equippedTier);
            }
        }
        DrawText(displayName.c_str(), (int)headerRec.x + 8, (int)headerRec.y + (nameSize == 10 ? 7 : 8), nameSize, Color{ 255, 235, 165, 255 });
    } else {
        DrawText("UNARMED", (int)headerRec.x + 8, (int)headerRec.y + 7, 10, Color{ 160, 175, 200, 200 });
        const char* hint = "HOTBAR [1-3]";
        int hw = MeasureText(hint, 9);
        DrawText(hint, (int)(headerRec.x + headerRec.width - (float)hw - 8.0f), (int)headerRec.y + 8, 9, Color{ 120, 150, 185, 190 });
    }

    // Outer Dock Frame
    Rectangle dockRec = { dockX, dockY, dockW, dockH };
    DrawRectangleRounded(dockRec, 0.20f, 6, Color{ 12, 16, 24, 225 });
    DrawRectangleRoundedLines(dockRec, 0.20f, 6, Color{ 50, 66, 92, 225 });

    int activeSlot = player.GetInventory().GetActiveWeaponSlot();
    float iconScale = (windowWidth >= 1400) ? 1.55f : 1.38f;

    for (int i = 0; i < 3; ++i) {
        WeaponType slotWep = player.GetInventory().GetHotbarWeapon(i);
        bool hasWep = (slotWep != WeaponType::NONE);
        bool isActive = (i == activeSlot);
        int tier = hasWep ? player.GetInventory().GetWeaponUpgrade(slotWep) : 0;

        Rectangle r = { dockX + pad + (float)i * (slotW + gap), dockY + 8.0f, slotW, slotH };

        Color tileBg = isActive ? Color{ 28, 24, 14, 240 } : (hasWep ? Color{ 16, 21, 32, 230 } : Color{ 11, 14, 20, 195 });
        DrawRectangleRounded(r, 0.18f, 5, tileBg);

        if (hasWep) {
            DrawRectangleRounded(Rectangle{ r.x + 2.0f, r.y + 2.0f, r.width - 4.0f, r.height * 0.24f }, 0.22f, 3, Color{ 255, 255, 255, 14 });
        }

        Color borderCol = isActive ? Color{ 255, 215, 80, 255 } : (hasWep ? Color{ 68, 92, 126, 220 } : Color{ 38, 48, 66, 165 });
        if (isActive) {
            DrawRectangleRoundedLines(Rectangle{ r.x - 2.0f, r.y - 2.0f, r.width + 4.0f, r.height + 4.0f }, 0.22f, 4, Color{ 255, 205, 60, 160 });
        }
        DrawRectangleRoundedLines(r, 0.18f, 5, borderCol);

        // Keybind Pill top-left [1], [2], [3]
        char keyStr[8];
        snprintf(keyStr, sizeof(keyStr), "[%d]", i + 1);
        int kw = MeasureText(keyStr, 9);
        DrawRectangleRounded(Rectangle{ r.x + 3.0f, r.y + 3.0f, (float)kw + 6.0f, 12.0f }, 0.35f, 3, isActive ? Color{ 62, 46, 16, 240 } : Color{ 10, 14, 22, 220 });
        DrawText(keyStr, (int)r.x + 6, (int)r.y + 4, 9, isActive ? Color{ 255, 225, 95, 255 } : Color{ 210, 225, 245, 230 });

        // Upgrade Tier Pill top-right (+1 / +2 / C)
        if (hasWep && tier > 0) {
            const char* tStr = (slotWep == WeaponType::TRUTH_SEEKER) ? "C" : (tier >= 2 ? "+2" : "+1");
            Color tCol = (slotWep == WeaponType::TRUTH_SEEKER) ? Color{ 255, 225, 90, 255 } : ((tier >= 2) ? Color{ 110, 245, 170, 255 } : Color{ 120, 225, 255, 255 });
            int tw = MeasureText(tStr, 9);
            float tW = (float)tw + 6.0f;
            float tX = r.x + r.width - tW - 3.0f;
            DrawRectangleRounded(Rectangle{ tX, r.y + 3.0f, tW, 12.0f }, 0.35f, 3, Color{ 14, 24, 38, 225 });
            DrawText(tStr, (int)tX + 3, (int)r.y + 4, 9, tCol);
        }

        // Centered Procedural Weapon Icon
        if (hasWep) {
            Vector2 iconCenter = { r.x + r.width * 0.5f, r.y + r.height * 0.46f };
            AssetManager::DrawWeaponSlotIcon(iconCenter, slotWep, iconScale, true, isActive);
        }

        // Bottom Micro-Label
        const char* shortName = GetShortWeaponLabel(slotWep);
        int lw = MeasureText(shortName, 9);
        int lx = (int)(r.x + (r.width - (float)lw) * 0.5f);
        int ly = (int)(r.y + r.height - 12.0f);
        Color lblCol = hasWep ? (isActive ? Color{ 255, 235, 165, 255 } : Color{ 185, 208, 235, 225 }) : Color{ 95, 110, 130, 175 };
        DrawText(shortName, lx, ly, 9, lblCol);
    }
}

void HUD::DrawLobby(int windowWidth, int windowHeight, const Player& player, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore) {
    // Top Lobby Banner
    const float bannerW = 460.0f;
    const float bannerH = 46.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 16, 20, 28, 220 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 55, 75, 105, 240 });

    const char* lobbyTitle = "ARMORY SANCTUARY (LOBBY)";
    DrawText(lobbyTitle, (int)bannerX + 16, (int)bannerY + 8, 14, Color{ 255, 220, 100, 255 });

    const char* hint = "Equip at pedestals [E]  |  North Portal: Enter Arena";
    DrawText(hint, (int)bannerX + 16, (int)bannerY + 26, 12, Color{ 200, 220, 245, 220 });

    // Silver Shingles and Ore Scraps currency badges in top right
    DrawCurrencyBadge(windowWidth, player.GetInventory().GetShingles(), player.GetInventory().GetOreScraps());

    // Map button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 404), 16.0f, 122.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 404), 16.0f, 122.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[M] Realm Map", windowWidth - 392, 25, 12, Color{ 255, 220, 100, 240 });

    // Inventory & Journal button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[TAB] Inventory", windowWidth - 262, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 150, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 120, 25, 12, Color{ 120, 220, 255, 240 });

    // Interaction prompt popup if standing near pedestal
    if (isNearPedestal) {
        std::string prompt = "[E] Equip " + nearWeaponName;
        int pw = MeasureText(prompt.c_str(), 14);
        float px = ((float)windowWidth - pw - 30) * 0.5f;
        float py = (float)windowHeight * 0.5f + 80.0f;
        DrawRectangleRounded(Rectangle{ px, py, (float)(pw + 30), 32.0f }, 0.3f, 4, Color{ 20, 40, 70, 240 });
        DrawRectangleRoundedLines(Rectangle{ px, py, (float)(pw + 30), 32.0f }, 0.3f, 4, Color{ 100, 220, 255, 255 });
        DrawText(prompt.c_str(), (int)(px + 15), (int)(py + 9), 14, Color{ 255, 255, 255, 255 });
    }

    // Player HUD Panel (Bottom Left)
    DrawPlayerCard(windowWidth, windowHeight, player);

    // Modular Visual Cooldown Action Bar (Bottom Center)
    DrawActionBar(windowWidth, windowHeight, player);

    // 3-Slot Weapon Hotbar Dock (Bottom Right)
    DrawWeaponHotbarDock(windowWidth, windowHeight, player);
}

void HUD::DrawCombat(int windowWidth, int windowHeight, const Player& player, int currentWave, int totalWaves, bool hasUnreadLore) {
    // Player HUD Panel (Bottom Left)
    DrawPlayerCard(windowWidth, windowHeight, player);

    // Modular Visual Cooldown Action Bar (Bottom Center)
    DrawActionBar(windowWidth, windowHeight, player);

    // 3-Slot Weapon Hotbar Dock (Bottom Right)
    DrawWeaponHotbarDock(windowWidth, windowHeight, player);

    // Top Wave Status Banner
    bool isMilestoneWave = (currentWave % 5 == 0);
    const float bannerW = isMilestoneWave ? 380.0f : 300.0f;
    const float bannerH = 42.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    Color bannerBg = isMilestoneWave ? Color{ 36, 18, 24, 235 } : Color{ 16, 20, 28, 210 };
    Color bannerBorder = isMilestoneWave ? Color{ 255, 100, 80, 255 } : Color{ 55, 70, 95, 230 };

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, bannerBg);
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, bannerBorder);

    std::string waveStr = isMilestoneWave ?
        ("COMBAT ARENA - WAVE " + std::to_string(currentWave) + " / " + std::to_string(totalWaves) + " [BOSS MILESTONE]") :
        ("COMBAT ARENA - WAVE " + std::to_string(currentWave) + " / " + std::to_string(totalWaves));
    int waveW = FontManager::MeasureGameText(waveStr, 13, GameFont::BODY);
    FontManager::DrawGameTextShadow(waveStr, (int)(bannerX + (bannerW - waveW) * 0.5f), (int)(bannerY + 12), 13, isMilestoneWave ? Color{ 255, 215, 80, 255 } : Color{ 255, 220, 100, 255 });

    // Silver Shingles and Ore Scraps currency badges in top right
    DrawCurrencyBadge(windowWidth, player.GetInventory().GetShingles(), player.GetInventory().GetOreScraps());

    // Map button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 404), 16.0f, 122.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 404), 16.0f, 122.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[M] Realm Map", windowWidth - 392, 25, 12, Color{ 255, 220, 100, 240 });

    // Inventory & Journal button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[TAB] Inventory", windowWidth - 262, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 150, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 120, 25, 12, Color{ 120, 220, 255, 240 });
}

void HUD::DrawCurrencyBadge(int windowWidth, int shingles, int oreScraps) {
    const float badgeW = 140.0f;
    const float badgeH = 30.0f;
    const float badgeX = (float)windowWidth - badgeW - 20.0f;
    const float badgeY = 56.0f;

    // 1. Silver Shingles Badge
    Rectangle badgeRec = { badgeX, badgeY, badgeW, badgeH };
    DrawRectangleRounded(badgeRec, 0.3f, 4, Color{ 18, 24, 36, 225 });
    DrawRectangleRoundedLines(badgeRec, 0.3f, 4, Color{ 140, 175, 215, 220 });

    float iconCenterX = badgeX + 18.0f;
    float iconCenterY = badgeY + 15.0f;
    DrawRectanglePro(Rectangle{ iconCenterX, iconCenterY, 11.0f, 11.0f }, Vector2{ 5.5f, 5.5f }, 45.0f, Color{ 210, 230, 255, 255 });
    DrawPolyLines(Vector2{ iconCenterX, iconCenterY }, 4, 7.8f, 45.0f, Color{ 90, 125, 170, 255 });
    DrawLine((int)(iconCenterX - 3), (int)(iconCenterY - 3), (int)(iconCenterX + 3), (int)(iconCenterY - 3), WHITE);

    std::string text = std::to_string(shingles) + " Shingles";
    FontManager::DrawText(text, (int)(badgeX + 34), (int)(badgeY + 7), 12, Color{ 230, 242, 255, 245 }, GameFont::BODY);

    // 2. Ore Scraps Badge (Amber / Bronze)
    const float scrapBadgeW = 136.0f;
    const float scrapBadgeX = badgeX - scrapBadgeW - 8.0f;
    Rectangle scrapBadgeRec = { scrapBadgeX, badgeY, scrapBadgeW, badgeH };
    DrawRectangleRounded(scrapBadgeRec, 0.3f, 4, Color{ 26, 22, 16, 225 });
    DrawRectangleRoundedLines(scrapBadgeRec, 0.3f, 4, Color{ 225, 170, 55, 220 });

    float scrapIconX = scrapBadgeX + 18.0f;
    float scrapIconY = badgeY + 15.0f;
    DrawPoly(Vector2{ scrapIconX, scrapIconY }, 4, 7.2f, 45.0f, Color{ 255, 195, 55, 255 });
    DrawPolyLinesEx(Vector2{ scrapIconX, scrapIconY }, 4, 7.2f, 45.0f, 1.5f, Color{ 255, 235, 140, 255 });

    std::string scrapText = std::to_string(oreScraps) + " Scraps";
    FontManager::DrawText(scrapText, (int)(scrapBadgeX + 34), (int)(badgeY + 7), 12, Color{ 255, 225, 130, 245 }, GameFont::BODY);
}

void HUD::DrawCampaignHUD(int windowWidth, int windowHeight, const Player& player, const std::string& locationName, bool hasUnreadLore, const TimeOfDaySystem* tod) {
    // Silver Shingles and Ore Scraps currency badges in top right
    DrawCurrencyBadge(windowWidth, player.GetInventory().GetShingles(), player.GetInventory().GetOreScraps());
    // Player HUD Panel (Bottom Left)
    DrawPlayerCard(windowWidth, windowHeight, player);

    // Modular Visual Cooldown Action Bar (Bottom Center)
    DrawActionBar(windowWidth, windowHeight, player);

    // 3-Slot Weapon Hotbar Dock (Bottom Right)
    DrawWeaponHotbarDock(windowWidth, windowHeight, player);

    // Top Location Header Banner with Time-of-Day Clock Dial
    DrawLocationHeader(windowWidth, locationName, tod);

    // Map button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 404), 16.0f, 122.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 404), 16.0f, 122.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[M] Realm Map", windowWidth - 392, 25, 12, Color{ 255, 220, 100, 240 });

    // Inventory & Journal button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[TAB] Inventory", windowWidth - 262, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 150, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 120, 25, 12, Color{ 120, 220, 255, 240 });
}

void HUD::DrawTacticalRadar(
    int windowWidth,
    int windowHeight,
    const Player& player,
    const std::vector<Enemy>& enemies,
    const std::vector<Vector2>& campfires,
    const std::vector<Vector2>& npcs,
    const std::vector<Vector2>& altars
) {
    (void)windowWidth;
    (void)windowHeight;

    static bool s_showRadar = true;
    if (IsKeyPressed(KEY_N)) {
        s_showRadar = !s_showRadar;
    }

    const float radarCX = 80.0f;
    const float radarCY = 80.0f;
    const float radarR = 56.0f;
    const float radarRange = 600.0f;

    Vector2 mousePos = GetMousePosition();

    if (!s_showRadar) {
        Rectangle pillRec = { 24.0f, 16.0f, 84.0f, 26.0f };
        bool hov = CheckCollisionPointRec(mousePos, pillRec);
        if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            s_showRadar = true;
        }
        DrawRectangleRounded(pillRec, 0.35f, 4, Color{ 14, 18, 28, 220 });
        DrawRectangleRoundedLines(pillRec, 0.35f, 4, hov ? Color{ 140, 215, 255, 255 } : Color{ 55, 80, 115, 220 });
        DrawText("[N] RADAR", (int)pillRec.x + 12, (int)pillRec.y + 7, 10, hov ? Color{ 200, 240, 255, 255 } : Color{ 140, 205, 245, 220 });
        return;
    }

    // Drop shadow
    DrawCircle((int)radarCX, (int)radarCY + 2, radarR + 2.0f, Color{ 6, 8, 14, 150 });

    // Dark glass radar lens
    DrawCircle((int)radarCX, (int)radarCY, radarR, Color{ 10, 16, 26, 230 });
    DrawCircleLines((int)radarCX, (int)radarCY, radarR, Color{ 65, 90, 125, 230 });
    DrawCircleLines((int)radarCX, (int)radarCY, radarR - 2.0f, Color{ 36, 50, 75, 180 });

    // Concentric range rings
    DrawCircleLines((int)radarCX, (int)radarCY, radarR * 0.35f, Color{ 40, 60, 92, 75 });
    DrawCircleLines((int)radarCX, (int)radarCY, radarR * 0.70f, Color{ 40, 60, 92, 75 });

    // Crosshair axis
    DrawLine((int)(radarCX - radarR + 4.0f), (int)radarCY, (int)(radarCX + radarR - 4.0f), (int)radarCY, Color{ 40, 60, 92, 60 });
    DrawLine((int)radarCX, (int)(radarCY - radarR + 4.0f), (int)radarCX, (int)(radarCY + radarR - 4.0f), Color{ 40, 60, 92, 60 });

    // 360-degree radar beam sweep
    float sweepRad = fmodf((float)GetTime() * 160.0f, 360.0f) * DEG2RAD;
    Vector2 sweepVec = { radarCX + cosf(sweepRad) * (radarR - 4.0f), radarCY + sinf(sweepRad) * (radarR - 4.0f) };
    DrawLineEx({ radarCX, radarCY }, sweepVec, 1.5f, Color{ 90, 220, 255, 130 });
    DrawCircleGradient(Vector2{ radarCX, radarCY }, radarR * 0.9f, Color{ 50, 150, 230, 24 }, Color{ 0, 0, 0, 0 });

    // Compass cardinal markers
    DrawText("N", (int)radarCX - 3, (int)(radarCY - radarR + 3.0f), 8, Color{ 255, 215, 80, 240 });
    DrawText("S", (int)radarCX - 3, (int)(radarCY + radarR - 11.0f), 8, Color{ 140, 175, 215, 180 });
    DrawText("E", (int)(radarCX + radarR - 9.0f), (int)radarCY - 4, 8, Color{ 140, 175, 215, 180 });
    DrawText("W", (int)(radarCX - radarR + 3.0f), (int)radarCY - 4, 8, Color{ 140, 175, 215, 180 });

    Vector2 playerPos = player.GetPosition();

    // Helper lambda for projecting world position onto circular radar
    auto ProjectToRadar = [&](Vector2 worldPos, float maxDist) -> Vector2 {
        float dx = worldPos.x - playerPos.x;
        float dy = worldPos.y - playerPos.y;
        float dist = sqrtf(dx * dx + dy * dy);
        float ratio = fminf(1.0f, dist / maxDist);
        float normX = (dist > 0.001f) ? (dx / dist) : 0.0f;
        float normY = (dist > 0.001f) ? (dy / dist) : 0.0f;
        return Vector2{
            radarCX + normX * (ratio * (radarR - 6.0f)),
            radarCY + normY * (ratio * (radarR - 6.0f))
        };
    };

    // 1. Waypoint Altars
    for (const auto& altar : altars) {
        Vector2 pip = ProjectToRadar(altar, radarRange);
        DrawPoly(pip, 4, 3.5f, 45.0f, Color{ 120, 225, 255, 240 });
        DrawCircle((int)pip.x, (int)pip.y, 1.0f, Color{ 255, 255, 255, 255 });
    }

    // 2. Bonfires / Campfires
    for (const auto& cp : campfires) {
        Vector2 pip = ProjectToRadar(cp, radarRange);
        float p = 0.5f + 0.5f * sinf((float)GetTime() * 6.0f);
        DrawPoly(pip, 4, 4.0f + 0.5f * p, 45.0f, Color{ 255, 185, 45, 245 });
        DrawCircle((int)pip.x, (int)pip.y, 1.5f, Color{ 255, 245, 180, 255 });
    }

    // 3. Friendly NPCs
    for (const auto& npc : npcs) {
        Vector2 pip = ProjectToRadar(npc, radarRange);
        DrawCircle((int)pip.x, (int)pip.y, 3.0f, Color{ 80, 240, 150, 245 });
        DrawCircleLines((int)pip.x, (int)pip.y, 4.0f, Color{ 140, 255, 190, 180 });
    }

    // 4. Enemy Threat Pips
    for (const auto& enemy : enemies) {
        if (!enemy.IsAlive()) continue;

        Vector2 pip = ProjectToRadar(enemy.GetPosition(), radarRange);
        if (enemy.IsBoss()) {
            float p = 0.5f + 0.5f * sinf((float)GetTime() * 8.0f);
            DrawCircle((int)pip.x, (int)pip.y, 4.5f, Color{ 255, 30, 45, 255 });
            DrawCircleLines((int)pip.x, (int)pip.y, 5.5f + 2.0f * p, Color{ 255, 80, 95, 210 });
        } else if (enemy.IsNightfallElite()) {
            DrawCircle((int)pip.x, (int)pip.y, 3.8f, Color{ 185, 75, 255, 255 });
            DrawCircleLines((int)pip.x, (int)pip.y, 4.8f, Color{ 220, 140, 255, 220 });
        } else {
            DrawCircle((int)pip.x, (int)pip.y, 2.5f, Color{ 240, 60, 75, 225 });
        }
    }

    // 5. Player Marker in Center with Heading Arrow
    float facingRad = player.GetFacingAngle() * DEG2RAD;
    Vector2 tip = { radarCX + cosf(facingRad) * 8.0f, radarCY + sinf(facingRad) * 8.0f };
    Vector2 left = { radarCX + cosf(facingRad + 2.45f) * 5.5f, radarCY + sinf(facingRad + 2.45f) * 5.5f };
    Vector2 right = { radarCX + cosf(facingRad - 2.45f) * 5.5f, radarCY + sinf(facingRad - 2.45f) * 5.5f };
    DrawTriangle(tip, left, right, Color{ 0, 240, 255, 255 });
    DrawCircle((int)radarCX, (int)radarCY, 2.0f, Color{ 255, 255, 255, 255 });

    // 6. Compass Heading Pill below radar
    Rectangle hRec = { radarCX - 40.0f, radarCY + radarR + 4.0f, 80.0f, 16.0f };
    DrawRectangleRounded(hRec, 0.35f, 3, Color{ 12, 16, 24, 220 });
    DrawRectangleRoundedLines(hRec, 0.35f, 3, Color{ 48, 68, 98, 200 });

    float deg = fmodf(player.GetFacingAngle() + 360.0f, 360.0f);
    const char* dirStr = (deg >= 337.5f || deg < 22.5f) ? "E" :
                         (deg < 67.5f ? "SE" :
                         (deg < 112.5f ? "S" :
                         (deg < 157.5f ? "SW" :
                         (deg < 202.5f ? "W" :
                         (deg < 247.5f ? "NW" :
                         (deg < 292.5f ? "N" : "NE"))))));

    char compBuf[32];
    snprintf(compBuf, sizeof(compBuf), "[N] %s %d'", dirStr, (int)deg);
    int cw = MeasureText(compBuf, 9);
    DrawText(compBuf, (int)(hRec.x + (hRec.width - cw) * 0.5f), (int)hRec.y + 3, 9, Color{ 175, 215, 255, 240 });
}

// Legacy float-based overloads
void HUD::DrawLobby(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore) {
    // Top Lobby Banner
    const float bannerW = 680.0f;
    const float bannerH = 46.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 16, 20, 28, 220 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 55, 75, 105, 240 });

    const char* lobbyTitle = "ARMORY SANCTUARY (LOBBY)";
    DrawText(lobbyTitle, (int)bannerX + 16, (int)bannerY + 8, 14, Color{ 255, 220, 100, 255 });

    const char* hint = "Equip weapons at pedestals [E]  |  Step into North Portal to Enter Arena";
    DrawText(hint, (int)bannerX + 16, (int)bannerY + 26, 12, Color{ 200, 220, 245, 220 });

    // Journal & Lore button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[I] Journal", windowWidth - 263, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 170, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 128, 25, 12, Color{ 120, 220, 255, 240 });

    // Interaction prompt popup if standing near pedestal
    if (isNearPedestal) {
        std::string prompt = "[E] Equip " + nearWeaponName;
        int pw = MeasureText(prompt.c_str(), 14);
        float px = ((float)windowWidth - pw - 30) * 0.5f;
        float py = (float)windowHeight * 0.5f + 80.0f;
        DrawRectangleRounded(Rectangle{ px, py, (float)(pw + 30), 32.0f }, 0.3f, 4, Color{ 20, 40, 70, 240 });
        DrawRectangleRoundedLines(Rectangle{ px, py, (float)(pw + 30), 32.0f }, 0.3f, 4, Color{ 100, 220, 255, 255 });
        DrawText(prompt.c_str(), (int)(px + 15), (int)(py + 9), 14, Color{ 255, 255, 255, 255 });
    }

    // Active Weapon Card (Bottom Right)
    WeaponStats stats = GetWeaponStats(weapon);
    const float cardW = 320.0f;
    const float cardH = 92.0f;
    float cardX = (float)windowWidth - cardW - 24.0f;
    float cardY = (float)windowHeight - cardH - 24.0f;

    DrawRectangleRounded(Rectangle{ cardX, cardY, cardW, cardH }, 0.2f, 6, Color{ 16, 20, 30, 220 });
    DrawRectangleRoundedLines(Rectangle{ cardX, cardY, cardW, cardH }, 0.2f, 6, Color{ 55, 75, 105, 240 });

    DrawText("EQUIPPED WEAPON:", (int)cardX + 14, (int)cardY + 10, 11, Color{ 255, 215, 80, 255 });
    DrawText(stats.name.c_str(), (int)cardX + 14, (int)cardY + 26, 16, Color{ 255, 255, 255, 255 });
    DrawText(stats.description.c_str(), (int)cardX + 14, (int)cardY + 48, 10, Color{ 190, 210, 235, 220 });

    std::string dmgInfo = "L-Atk: " + std::to_string((int)stats.lightDamage) + " dmg  |  H-Atk: " + std::to_string((int)stats.heavyDamage) + " dmg";
    DrawText(dmgInfo.c_str(), (int)cardX + 14, (int)cardY + 70, 11, Color{ 120, 230, 255, 240 });

    // Player HUD Panel (Bottom Left)
    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 110.0f;
    const float hudWidth = 310.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(215 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 80, 40, 50, 220 });

    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(215 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 90, 75, 30, 220 });

    char dashBuf[32];
    char parryBuf[32];
    char heavyBuf[32];
    char statusBuf[128];

    if (dashCooldown <= 0.0f) snprintf(dashBuf, sizeof(dashBuf), "[Q] Dash");
    else snprintf(dashBuf, sizeof(dashBuf), "[Q] %.1fs", dashCooldown);

    if (parryCooldown <= 0.0f) snprintf(parryBuf, sizeof(parryBuf), "[F] Parry");
    else snprintf(parryBuf, sizeof(parryBuf), "[F] %.1fs", parryCooldown);

    if (heavyCooldown <= 0.0f) snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] Heavy");
    else snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] %.1fs", heavyCooldown);

    snprintf(statusBuf, sizeof(statusBuf), "%s  |  %s  |  %s", dashBuf, parryBuf, heavyBuf);
    DrawText(statusBuf, (int)hudX + 14, (int)hudY + 58, 10, Color{ 170, 205, 235, 240 });
}

void HUD::DrawCombat(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves, bool hasUnreadLore) {
    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 110.0f;
    const float hudWidth = 310.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(215 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 80, 40, 50, 220 });

    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(215 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 90, 75, 30, 220 });

    if (hasCounterBuff) {
        DrawText("COUNTER BUFF: +50% DAMAGE!", (int)hudX + 14, (int)hudY + 58, 11, Color{ 255, 230, 80, 255 });
    } else if (isParrying) {
        DrawText("PARRY WINDOW ACTIVE!", (int)hudX + 14, (int)hudY + 58, 11, Color{ 80, 240, 255, 255 });
    } else {
        char dashBuf[32];
        char parryBuf[32];
        char heavyBuf[32];
        char statusBuf[128];

        if (dashCooldown <= 0.0f) snprintf(dashBuf, sizeof(dashBuf), "[Q] Dash");
        else snprintf(dashBuf, sizeof(dashBuf), "[Q] %.1fs", dashCooldown);

        if (parryCooldown <= 0.0f) snprintf(parryBuf, sizeof(parryBuf), "[F] Parry");
        else snprintf(parryBuf, sizeof(parryBuf), "[F] %.1fs", parryCooldown);

        if (heavyCooldown <= 0.0f) snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] Heavy");
        else snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] %.1fs", heavyCooldown);

        snprintf(statusBuf, sizeof(statusBuf), "%s  |  %s  |  %s", dashBuf, parryBuf, heavyBuf);
        DrawText(statusBuf, (int)hudX + 14, (int)hudY + 58, 10, Color{ 170, 205, 235, 240 });
    }

    // Active Weapon Badge (Bottom Right)
    WeaponStats stats = GetWeaponStats(weapon);
    const float wBadgeW = 200.0f;
    const float wBadgeH = 46.0f;
    float wBadgeX = (float)windowWidth - wBadgeW - 24.0f;
    float wBadgeY = (float)windowHeight - wBadgeH - 24.0f;

    DrawRectangleRounded(Rectangle{ wBadgeX, wBadgeY, wBadgeW, wBadgeH }, 0.25f, 6, Color{ 16, 20, 30, 210 });
    DrawRectangleRoundedLines(Rectangle{ wBadgeX, wBadgeY, wBadgeW, wBadgeH }, 0.25f, 6, Color{ 55, 75, 105, 220 });
    DrawText("WEAPON:", (int)wBadgeX + 12, (int)wBadgeY + 8, 10, Color{ 255, 215, 80, 255 });
    DrawText(stats.name.c_str(), (int)wBadgeX + 12, (int)wBadgeY + 22, 14, Color{ 255, 255, 255, 255 });

    // Top Wave Status Banner
    bool isMilestoneWave = (currentWave % 5 == 0);
    const float bannerW = isMilestoneWave ? 380.0f : 300.0f;
    const float bannerH = 42.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    Color bannerBg = isMilestoneWave ? Color{ 36, 18, 24, 235 } : Color{ 16, 20, 28, 210 };
    Color bannerBorder = isMilestoneWave ? Color{ 255, 100, 80, 255 } : Color{ 55, 70, 95, 230 };

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, bannerBg);
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, bannerBorder);

    std::string waveStr = isMilestoneWave ?
        ("COMBAT ARENA - WAVE " + std::to_string(currentWave) + " / " + std::to_string(totalWaves) + " [BOSS MILESTONE]") :
        ("COMBAT ARENA - WAVE " + std::to_string(currentWave) + " / " + std::to_string(totalWaves));
    int waveW = FontManager::MeasureGameText(waveStr, 13, GameFont::BODY);
    FontManager::DrawGameTextShadow(waveStr, (int)(bannerX + (bannerW - waveW) * 0.5f), (int)(bannerY + 12), 13, isMilestoneWave ? Color{ 255, 215, 80, 255 } : Color{ 255, 220, 100, 255 });

    // Inventory & Journal button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[TAB] Inventory", windowWidth - 262, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 150, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 120, 25, 12, Color{ 120, 220, 255, 240 });
}

void HUD::DrawLocationHeader(int windowWidth, const std::string& locationName, const TimeOfDaySystem* tod) {
    const float bannerW = (tod != nullptr) ? 460.0f : 340.0f;
    const float bannerH = 42.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    // Dark glass backing card
    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 14, 18, 28, 225 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 205, 165, 75, 240 });
    DrawRectangleRoundedLines(Rectangle{ bannerX + 3, bannerY + 3, bannerW - 6, bannerH - 6 }, 0.20f, 6, Color{ 48, 62, 90, 160 });

    // Golden corner accents
    Color cornerCol = Color{ 255, 215, 80, 240 };
    DrawLineEx({ bannerX + 8, bannerY + 8 }, { bannerX + 22, bannerY + 8 }, 1.5f, cornerCol);
    DrawLineEx({ bannerX + 8, bannerY + 8 }, { bannerX + 8, bannerY + 22 }, 1.5f, cornerCol);
    DrawLineEx({ bannerX + bannerW - 8, bannerY + 8 }, { bannerX + bannerW - 22, bannerY + 8 }, 1.5f, cornerCol);
    DrawLineEx({ bannerX + bannerW - 8, bannerY + 8 }, { bannerX + bannerW - 8, bannerY + 22 }, 1.5f, cornerCol);

    if (tod != nullptr) {
        // Location text on the left
        DrawText(locationName.c_str(), (int)bannerX + 22, (int)bannerY + 13, 14, Color{ 255, 225, 95, 255 });

        // Celestial Clock Badge on right
        float dialW = 126.0f;
        float dialH = bannerH - 12.0f;
        float dialX = bannerX + bannerW - dialW - 10.0f;
        float dialY = bannerY + 6.0f;

        bool isNight = tod->IsNight();
        Color dialBorder = isNight ? Color{ 100, 160, 240, 200 } : Color{ 210, 175, 70, 200 };
        DrawRectangleRounded(Rectangle{ dialX, dialY, dialW, dialH }, 0.35f, 4, Color{ 10, 14, 22, 230 });
        DrawRectangleRoundedLines(Rectangle{ dialX, dialY, dialW, dialH }, 0.35f, 4, dialBorder);

        // Celestial Orb Icon
        float cx = dialX + 16.0f;
        float cy = dialY + dialH * 0.5f;
        if (tod->GetPhase() == TimePhase::DAY || tod->GetPhase() == TimePhase::DAWN) {
            // Radiant Golden Sun
            DrawCircle((int)cx, (int)cy, 5.0f, Color{ 255, 215, 70, 255 });
            DrawLineEx({ cx - 8.0f, cy }, { cx + 8.0f, cy }, 1.0f, Color{ 255, 235, 120, 220 });
            DrawLineEx({ cx, cy - 8.0f }, { cx, cy + 8.0f }, 1.0f, Color{ 255, 235, 120, 220 });
        } else {
            // Silver Crescent Moon
            DrawCircle((int)cx, (int)cy, 5.5f, Color{ 200, 225, 255, 255 });
            DrawCircle((int)cx + 2, (int)cy - 1, 4.5f, Color{ 10, 14, 22, 255 });
            DrawCircle((int)cx + 6, (int)cy - 4, 1.0f, Color{ 230, 245, 255, 255 });
        }

        // Formatted Time
        std::string timeStr = tod->GetFormattedTime();
        DrawText(timeStr.c_str(), (int)dialX + 30, (int)dialY + 8, 12, Color{ 240, 245, 255, 255 });

        // Phase Tag
        const char* phaseName = tod->GetPhaseName();
        Color phaseCol = isNight ? Color{ 130, 200, 255, 255 } :
                         (tod->GetPhase() == TimePhase::DUSK ? Color{ 235, 140, 180, 255 } :
                         (tod->GetPhase() == TimePhase::DAWN ? Color{ 255, 190, 110, 255 } : Color{ 255, 225, 110, 255 }));
        int pw = MeasureText(phaseName, 9);
        DrawText(phaseName, (int)(dialX + dialW - pw - 8), (int)dialY + 10, 9, phaseCol);
    } else {
        // Map Name centered in Gold
        int nameW = MeasureText(locationName.c_str(), 16);
        DrawText(locationName.c_str(), (int)(bannerX + (bannerW - nameW) * 0.5f), (int)bannerY + 13, 16, Color{ 255, 225, 95, 255 });
    }
}

void HUD::DrawCampaignHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, const std::string& locationName, bool hasUnreadLore, const TimeOfDaySystem* tod) {
    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 110.0f;
    const float hudWidth = 310.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(215 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 80, 40, 50, 220 });

    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(215 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 90, 75, 30, 220 });

    if (hasCounterBuff) {
        DrawText("COUNTER BUFF: +50% DAMAGE!", (int)hudX + 14, (int)hudY + 58, 11, Color{ 255, 230, 80, 255 });
    } else if (isParrying) {
        DrawText("PARRY WINDOW ACTIVE!", (int)hudX + 14, (int)hudY + 58, 11, Color{ 80, 240, 255, 255 });
    } else {
        char dashBuf[32];
        char parryBuf[32];
        char heavyBuf[32];
        char statusBuf[128];

        if (dashCooldown <= 0.0f) snprintf(dashBuf, sizeof(dashBuf), "[Q] Dash");
        else snprintf(dashBuf, sizeof(dashBuf), "[Q] %.1fs", dashCooldown);

        if (parryCooldown <= 0.0f) snprintf(parryBuf, sizeof(parryBuf), "[F] Parry");
        else snprintf(parryBuf, sizeof(parryBuf), "[F] %.1fs", parryCooldown);

        if (heavyCooldown <= 0.0f) snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] Heavy");
        else snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] %.1fs", heavyCooldown);

        snprintf(statusBuf, sizeof(statusBuf), "%s  |  %s  |  %s", dashBuf, parryBuf, heavyBuf);
        DrawText(statusBuf, (int)hudX + 14, (int)hudY + 58, 10, Color{ 170, 205, 235, 240 });
    }

    // Active Weapon Badge (Bottom Right)
    WeaponStats stats = GetWeaponStats(weapon);
    const float wBadgeW = 200.0f;
    const float wBadgeH = 46.0f;
    float wBadgeX = (float)windowWidth - wBadgeW - 24.0f;
    float wBadgeY = (float)windowHeight - wBadgeH - 24.0f;

    DrawRectangleRounded(Rectangle{ wBadgeX, wBadgeY, wBadgeW, wBadgeH }, 0.25f, 6, Color{ 16, 20, 30, 210 });
    DrawRectangleRoundedLines(Rectangle{ wBadgeX, wBadgeY, wBadgeW, wBadgeH }, 0.25f, 6, Color{ 55, 75, 105, 220 });
    DrawText("WEAPON:", (int)wBadgeX + 12, (int)wBadgeY + 8, 10, Color{ 255, 215, 80, 255 });
    DrawText(stats.name.c_str(), (int)wBadgeX + 12, (int)wBadgeY + 22, 14, Color{ 255, 255, 255, 255 });

    // Top Location Header Banner (NO wave counter in campaign!)
    DrawLocationHeader(windowWidth, locationName, tod);

    // Inventory & Journal button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 274), 16.0f, 134.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[TAB] Inventory", windowWidth - 262, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 150, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 132), 16.0f, 112.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 120, 25, 12, Color{ 120, 220, 255, 240 });
}

void HUD::DrawBossHealthBar(int windowWidth, const std::string& bossName, float hp, float maxHp) {
    const float barW = 540.0f;
    const float barH = 18.0f;
    float barX = ((float)windowWidth - barW) * 0.5f;
    float barY = 64.0f;

    // Outer backing plate
    DrawRectangleRounded(Rectangle{ barX - 16.0f, barY - 26.0f, barW + 32.0f, barH + 46.0f }, 0.2f, 4, Color{ 16, 12, 20, 230 });
    DrawRectangleRoundedLines(Rectangle{ barX - 16.0f, barY - 26.0f, barW + 32.0f, barH + 46.0f }, 0.2f, 4, Color{ 140, 90, 40, 240 });

    // Boss Name & Crest
    int nameW = MeasureText(bossName.c_str(), 14);
    DrawText(bossName.c_str(), (int)(barX + (barW - nameW) * 0.5f), (int)barY - 20, 14, Color{ 255, 215, 80, 255 });

    // Humoral alignment badge for boss
    std::string humorBadge = "";
    Color humorCol = WHITE;
    if (bossName.find("INQUISITOR") != std::string::npos) {
        humorBadge = "[CHOLERIC - YELLOW BILE]";
        humorCol = Color{ 255, 150, 40, 240 };
    } else if (bossName.find("ZIL") != std::string::npos) {
        humorBadge = "[MELANCHOLIC - BLACK BILE]";
        humorCol = Color{ 190, 120, 255, 240 };
    } else if (bossName.find("LENON") != std::string::npos) {
        humorBadge = "[SANGUINE - BLOOD]";
        humorCol = Color{ 255, 60, 80, 240 };
    } else if (bossName.find("GALE") != std::string::npos || bossName.find("ARCHER") != std::string::npos) {
        humorBadge = "[PHLEGMATIC - WOODLAND GALE]";
        humorCol = Color{ 90, 220, 255, 240 };
    } else if (bossName.find("GOLEM") != std::string::npos) {
        humorBadge = "[MELANCHOLIC - PRIMORDIAL BASALT]";
        humorCol = Color{ 255, 140, 40, 240 };
    }

    if (!humorBadge.empty()) {
        int hw = MeasureText(humorBadge.c_str(), 10);
        DrawText(humorBadge.c_str(), (int)(barX + barW - hw), (int)barY - 18, 10, humorCol);
    }

    // Health Bar with graduation ticks & specular shine
    Rectangle bRec = { barX, barY, barW, barH };
    DrawResourceBar(bRec, hp, maxHp, Color{ 220, 45, 65, 255 }, Color{ 35, 15, 22, 240 }, Color{ 255, 200, 60, 220 }, true);

    // HP textual value
    std::string valStr = std::to_string((int)hp) + " / " + std::to_string((int)maxHp);
    int valW = MeasureText(valStr.c_str(), 10);
    DrawText(valStr.c_str(), (int)(barX + (barW - valW) * 0.5f), (int)barY + 4, 10, Color{ 255, 255, 255, 240 });

    // Stagger / Posture Resistance Meter
    float stagY = barY + barH + 4.0f;
    float stagH = 5.0f;
    DrawRectangleRounded(Rectangle{ barX, stagY, barW, stagH }, 0.25f, 2, Color{ 20, 24, 34, 220 });
    DrawRectangleRounded(Rectangle{ barX, stagY, barW, stagH }, 0.25f, 2, Color{ 90, 175, 225, 220 });
    DrawRectangleRoundedLines(Rectangle{ barX, stagY, barW, stagH }, 0.25f, 2, Color{ 50, 75, 110, 180 });
    DrawText("POSTURE & COMPOSURE GUARD", (int)barX + 2, (int)stagY + 6, 8, Color{ 140, 180, 215, 200 });
}

void HUD::DrawBossDialogueBanner(int windowWidth, int windowHeight, const std::string& speaker, const std::string& text, Color accentColor) {
    float bannerW = std::min(1140.0f, (float)windowWidth - 64.0f);
    float bannerH = 116.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = (float)windowHeight - bannerH - 36.0f;

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.16f, 6, Color{ 12, 15, 22, 240 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.16f, 6, accentColor);
    DrawRectangleRoundedLines(Rectangle{ bannerX + 3.0f, bannerY + 3.0f, bannerW - 6.0f, bannerH - 6.0f }, 0.14f, 6, Color{ 45, 60, 85, 180 });

    // Speaker Name
    int speakerSize = FontManager::GetScaledSize(16);
    FontManager::DrawTextTitleShadow(speaker, (int)bannerX + 26, (int)bannerY + 14, speakerSize, accentColor);

    // Text Quote (Word wrapped so it never overflows)
    int quoteSize = FontManager::GetScaledSize(17);
    int lineSpacing = FontManager::GetScaledSize(6);
    FontManager::DrawTextWrappedShadow(text, (int)bannerX + 26, (int)bannerY + 40, (int)bannerW - 52, quoteSize, lineSpacing, Color{ 240, 244, 255, 250 });
}

bool HUD::DrawNPCDialogueBox(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const std::string& speaker, const std::vector<std::string>& pages, int& currentPage, Color accentColor) {
    if (pages.empty()) return false;

    // Bounds check current page
    if (currentPage < 0) currentPage = 0;
    if (currentPage >= (int)pages.size()) currentPage = (int)pages.size() - 1;

    // ESC key closes dialogue immediately
    if (IsKeyPressed(KEY_ESCAPE)) {
        return false;
    }

    // Responsive modal dimensions: adapts to windowed and fullscreen without clipping
    const float boxW = std::min(1160.0f, (float)windowWidth - 64.0f);
    const float boxH = std::min(252.0f, (float)windowHeight * 0.36f);
    const float boxX = ((float)windowWidth - boxW) * 0.5f;
    const float boxY = (float)windowHeight - boxH - 22.0f;

    // Dark backdrop card with accent border
    DrawRectangleRounded(Rectangle{ boxX, boxY, boxW, boxH }, 0.14f, 6, Color{ 14, 18, 28, 245 });
    DrawRectangleRoundedLines(Rectangle{ boxX, boxY, boxW, boxH }, 0.14f, 6, accentColor);
    DrawRectangleRoundedLines(Rectangle{ boxX + 3.0f, boxY + 3.0f, boxW - 6.0f, boxH - 6.0f }, 0.12f, 6, Color{ 45, 60, 85, 180 });

    // Speaker Name Header (Golden/Accent) with drop shadow
    int titleSize = FontManager::GetScaledSize(20);
    FontManager::DrawTextTitleShadow(speaker, (int)boxX + 28, (int)boxY + 16, titleSize, accentColor);

    // Page indicator top-right
    char pageBuf[32];
    snprintf(pageBuf, sizeof(pageBuf), "Page %d / %d", currentPage + 1, (int)pages.size());
    int pageIndSize = FontManager::GetScaledSize(13);
    FontManager::DrawGameTextShadow(pageBuf, (int)(boxX + boxW - 140.0f), (int)boxY + 18, pageIndSize, Color{ 170, 190, 220, 230 });

    // Word-wrapped text rendering with authentic medieval typography & drop shadow
    const std::string& text = pages[currentPage];
    const float maxLineWidth = boxW - 56.0f;
    int proseSize = FontManager::GetScaledSize(17);
    int lineSpacing = FontManager::GetScaledSize(7);
    FontManager::DrawTextWrappedShadow(text, (int)boxX + 28, (int)boxY + 48, (int)maxLineWidth, proseSize, lineSpacing, Color{ 235, 240, 252, 250 });

    // Navigation Buttons at Bottom
    const float btnY = boxY + boxH - 44.0f;
    const float btnH = 30.0f;

    // 1. [ < PREV (A) ] Button
    bool hasPrev = (currentPage > 0);
    Rectangle prevBtn = { boxX + 28.0f, btnY, 110.0f, btnH };
    bool prevHover = hasPrev && CheckCollisionPointRec(mousePos, prevBtn);
    Color prevBg = !hasPrev ? Color{ 25, 30, 42, 120 } : (prevHover ? Color{ 55, 75, 115, 240 } : Color{ 32, 44, 65, 220 });
    Color prevBorder = !hasPrev ? Color{ 50, 60, 80, 100 } : (prevHover ? Color{ 140, 195, 255, 255 } : Color{ 70, 95, 140, 200 });
    Color prevText = !hasPrev ? Color{ 100, 110, 130, 120 } : (prevHover ? Color{ 255, 255, 255, 255 } : Color{ 190, 215, 245, 230 });

    DrawRectangleRounded(prevBtn, 0.25f, 4, prevBg);
    DrawRectangleRoundedLines(prevBtn, 0.25f, 4, prevBorder);
    int prevW = FontManager::MeasureText("< PREV [A]", 11);
    FontManager::DrawText("< PREV [A]", (int)(prevBtn.x + (prevBtn.width - prevW) * 0.5f), (int)prevBtn.y + 7, 11, prevText);

    if (hasPrev && ((prevHover && isMousePressed) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))) {
        currentPage--;
    }

    // 2. [ NEXT > (D) ] or [ FINISH (SPACE) ] Button
    bool isLastPage = (currentPage >= (int)pages.size() - 1);
    Rectangle nextBtn = { boxX + 150.0f, btnY, 130.0f, btnH };
    bool nextHover = CheckCollisionPointRec(mousePos, nextBtn);
    Color nextBg = nextHover ? Color{ 70, 90, 135, 240 } : Color{ 38, 52, 80, 220 };
    Color nextBorder = nextHover ? Color{ 255, 220, 110, 255 } : Color{ 180, 150, 70, 220 };
    Color nextText = nextHover ? Color{ 255, 240, 150, 255 } : Color{ 240, 225, 160, 240 };

    DrawRectangleRounded(nextBtn, 0.25f, 4, nextBg);
    DrawRectangleRoundedLines(nextBtn, 0.25f, 4, nextBorder);
    const char* nextLabel = isLastPage ? "FINISH [SPACE]" : "NEXT > [D]";
    int nextW = FontManager::MeasureText(nextLabel, 11);
    FontManager::DrawText(nextLabel, (int)(nextBtn.x + (nextBtn.width - nextW) * 0.5f), (int)nextBtn.y + 7, 11, nextText);

    if ((nextHover && isMousePressed) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_SPACE)) {
        if (!isLastPage) {
            currentPage++;
        } else {
            return false; // Finished reading dialogue!
        }
    }

    // 3. [ CLOSE (ESC) ] Button
    Rectangle closeBtn = { boxX + boxW - 138.0f, btnY, 110.0f, btnH };
    bool closeHover = CheckCollisionPointRec(mousePos, closeBtn);
    Color closeBg = closeHover ? Color{ 130, 40, 50, 240 } : Color{ 58, 28, 35, 220 };
    Color closeBorder = closeHover ? Color{ 255, 110, 120, 255 } : Color{ 150, 60, 75, 200 };
    Color closeText = closeHover ? Color{ 255, 230, 235, 255 } : Color{ 230, 175, 185, 230 };

    DrawRectangleRounded(closeBtn, 0.25f, 4, closeBg);
    DrawRectangleRoundedLines(closeBtn, 0.25f, 4, closeBorder);
    int closeW = FontManager::MeasureText("CLOSE [ESC]", 11);
    FontManager::DrawText("CLOSE [ESC]", (int)(closeBtn.x + (closeBtn.width - closeW) * 0.5f), (int)closeBtn.y + 7, 11, closeText);

    if (closeHover && isMousePressed) {
        return false;
    }

    return true; // Keep open
}

void HUD::DrawGameOver(int windowWidth, int windowHeight) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 20, 5, 8, 210 });
    FontManager::DrawTextTitleCentered("FALLEN IN BATTLE", windowWidth / 2, windowHeight / 2 - 40, 42, Color{ 255, 50, 60, 255 });
    FontManager::DrawTextCentered("Press [R] to Respawn  |  [L] Return to Lobby", windowWidth / 2, windowHeight / 2 + 18, 16, Color{ 220, 225, 240, 230 });
}

void HUD::DrawVictory(int windowWidth, int windowHeight) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 10, 24, 18, 220 });
    FontManager::DrawTextTitleCentered("ROYAL ARENA CONQUERED", windowWidth / 2, windowHeight / 2 - 50, 42, Color{ 255, 225, 80, 255 });
    FontManager::DrawTextCentered("Grand Champion Spoils & Victor's Crest Awarded!", windowWidth / 2, windowHeight / 2 + 6, 16, Color{ 140, 240, 200, 255 });
    FontManager::DrawTextCentered("Press [R] to Replay  |  [L] Return to Realm Lobby", windowWidth / 2, windowHeight / 2 + 36, 14, Color{ 210, 240, 225, 220 });
}
