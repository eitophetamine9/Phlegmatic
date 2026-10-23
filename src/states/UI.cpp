#include "UI.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/Weapon.hpp"
#include "../entities/Player.hpp"
#include "../rendering/AssetManager.hpp"
#include <cmath>
#include <sstream>

int GameUI::DrawTitleScreen(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 12, 14, 20, 255 });

    float time = (float)GetTime();

    Vector2 center = { (float)windowWidth * 0.5f, (float)windowHeight * 0.40f };
    float pulse = 1.0f + 0.04f * sinf(time * 2.5f);
    DrawCircleLines((int)center.x, (int)center.y, 280.0f * pulse, Color{ 35, 45, 65, 120 });
    DrawCircleLines((int)center.x, (int)center.y, 160.0f * pulse, Color{ 50, 70, 105, 140 });

    const char* title = "PHLEGMATIC";
    int titleSize = 64;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (int)(center.x - titleW * 0.5f + 2), (int)(center.y - 120 + 2), titleSize, Color{ 0, 0, 0, 200 });
    DrawText(title, (int)(center.x - titleW * 0.5f), (int)(center.y - 120), titleSize, Color{ 255, 220, 100, 255 });

    const char* subtitle = "A Top-Down Action RPG of Composure & Precision";
    int subSize = 18;
    int subW = MeasureText(subtitle, subSize);
    DrawText(subtitle, (int)(center.x - subW * 0.5f), (int)(center.y - 50), subSize, Color{ 180, 200, 230, 230 });

    // Buttons Container
    const float btnWidth = 240.0f;
    const float btnHeight = 52.0f;
    float btnX = center.x - btnWidth * 0.5f;

    // 1. PLAY BUTTON
    float playY = center.y + 15.0f;
    Rectangle playRec = { btnX, playY, btnWidth, btnHeight };
    bool playHovered = CheckCollisionPointRec(mousePos, playRec);

    Color playBg = playHovered ? Color{ 45, 75, 130, 255 } : Color{ 25, 35, 55, 230 };
    Color playBorder = playHovered ? Color{ 100, 220, 255, 255 } : Color{ 60, 85, 125, 255 };
    DrawRectangleRounded(playRec, 0.25f, 6, playBg);
    DrawRectangleRoundedLines(playRec, 0.25f, 6, playBorder);

    const char* playText = "PLAY";
    int playSize = 24;
    int playW = MeasureText(playText, playSize);
    DrawText(playText, (int)(center.x - playW * 0.5f), (int)(playY + 14), playSize, playHovered ? Color{ 255, 255, 255, 255 } : Color{ 210, 230, 255, 240 });

    // 2. CONTROLS BUTTON
    float ctrlBtnY = playY + btnHeight + 16.0f;
    Rectangle ctrlRec = { btnX, ctrlBtnY, btnWidth, btnHeight };
    bool ctrlHovered = CheckCollisionPointRec(mousePos, ctrlRec);

    Color ctrlBg = ctrlHovered ? Color{ 40, 55, 80, 255 } : Color{ 20, 28, 42, 230 };
    Color ctrlBorder = ctrlHovered ? Color{ 255, 215, 80, 255 } : Color{ 50, 70, 95, 255 };
    DrawRectangleRounded(ctrlRec, 0.25f, 6, ctrlBg);
    DrawRectangleRoundedLines(ctrlRec, 0.25f, 6, ctrlBorder);

    const char* ctrlText = "CONTROLS";
    int ctrlSize = 20;
    int ctrlW = MeasureText(ctrlText, ctrlSize);
    DrawText(ctrlText, (int)(center.x - ctrlW * 0.5f), (int)(ctrlBtnY + 16), ctrlSize, ctrlHovered ? Color{ 255, 230, 100, 255 } : Color{ 180, 205, 235, 230 });

    // Quick Hint at bottom
    const char* hint = "Press [PLAY] to enter the Armory Sanctuary (Lobby)";
    int hintW = MeasureText(hint, 13);
    DrawText(hint, (int)(center.x - hintW * 0.5f), (int)(windowHeight - 45), 13, Color{ 140, 165, 195, 200 });

    if (isMousePressed) {
        if (playHovered) {
            SoundFX::PlayButtonClick();
            return 1; // Play
        }
        if (ctrlHovered) {
            SoundFX::PlayButtonClick();
            return 2; // Controls
        }
    }
    return 0;
}

bool GameUI::DrawControlsModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed) {
    // Dim background
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 10, 16, 225 });

    const float modalW = 760.0f;
    const float modalH = 530.0f;
    float modalX = ((float)windowWidth - modalW) * 0.5f;
    float modalY = ((float)windowHeight - modalH) * 0.5f;

    DrawRectangleRounded(Rectangle{ modalX, modalY, modalW, modalH }, 0.08f, 8, Color{ 16, 20, 30, 252 });
    DrawRectangleRoundedLines(Rectangle{ modalX, modalY, modalW, modalH }, 0.08f, 8, Color{ 60, 80, 115, 255 });

    // Header
    DrawText("CONTROLS & COMBAT MECHANICS", (int)modalX + 32, (int)modalY + 24, 22, Color{ 255, 220, 90, 255 });
    DrawText("Master movement, evasive dashes, and timely parries to dominate the arena.", (int)modalX + 32, (int)modalY + 52, 12, Color{ 170, 190, 215, 220 });
    DrawLine((int)modalX + 32, (int)modalY + 74, (int)(modalX + modalW - 32), (int)modalY + 74, Color{ 40, 52, 75, 255 });

    // Controls Table Content
    struct ControlItem {
        const char* key;
        const char* action;
        const char* details;
        Color badgeColor;
    };

    ControlItem items[] = {
        { "WASD / Arrows", "Movement", "Fluid 8-directional movement with obstacle collision physics.", Color{ 80, 140, 220, 255 } },
        { "Mouse Cursor",  "Aiming",   "Player rotates facing direction and directs attacks towards reticle.", Color{ 80, 140, 220, 255 } },
        { "Left Click",    "Light Atk","Fast, agile combo slash arc. Low stamina, rapid recovery.", Color{ 100, 220, 255, 255 } },
        { "Right Click",   "Heavy Atk","Charged lunging cleave. Devastating damage and massive knockback.", Color{ 255, 180, 60, 255 } },
        { "Q Key",         "DASH",     "High-velocity directional dash with i-frames (invulnerability). Evade attacks!", Color{ 120, 240, 180, 255 } },
        { "F Key",         "PARRY",    "Active 0.28s guard. Timely parries stagger foes & grant +50% counter dmg buff!", Color{ 255, 230, 80, 255 } },
        { "E Key",         "Interact", "Equip weapon at pedestals (Sword, Daggers, Mace) or enter Arena Portal.", Color{ 200, 150, 255, 255 } },
        { "C / TAB Key",   "Controls", "Toggle this Controls Menu modal at any time.", Color{ 160, 180, 200, 255 } },
        { "R / L / ESC",   "Menu Keys","[R] Respawn/Reset  |  [L] Return to Lobby  |  [ESC] Title Screen.", Color{ 140, 160, 180, 255 } }
    };

    float startY = modalY + 90.0f;
    float rowHeight = 38.0f;

    for (int i = 0; i < 9; i++) {
        float rowY = startY + (float)i * rowHeight;

        // Alternate row background
        if (i % 2 == 1) {
            DrawRectangle((int)modalX + 30, (int)rowY, (int)(modalW - 60), (int)rowHeight - 4, Color{ 22, 28, 42, 140 });
        }

        // Key Badge
        float badgeW = 120.0f;
        DrawRectangleRounded(Rectangle{ modalX + 36, rowY + 3, badgeW, 24 }, 0.25f, 4, Color{ 30, 38, 54, 255 });
        DrawRectangleRoundedLines(Rectangle{ modalX + 36, rowY + 3, badgeW, 24 }, 0.25f, 4, items[i].badgeColor);
        int kw = MeasureText(items[i].key, 11);
        DrawText(items[i].key, (int)(modalX + 36 + (badgeW - kw) * 0.5f), (int)rowY + 9, 11, items[i].badgeColor);

        // Action
        DrawText(items[i].action, (int)modalX + 175, (int)rowY + 9, 12, Color{ 255, 255, 255, 255 });

        // Details
        DrawText(items[i].details, (int)modalX + 265, (int)rowY + 9, 11, Color{ 185, 205, 230, 220 });
    }

    // Interactive CLOSE Button at Bottom
    const float closeW = 160.0f;
    const float closeH = 38.0f;
    float closeX = modalX + (modalW - closeW) * 0.5f;
    float closeY = modalY + modalH - 52.0f;

    Rectangle closeRec = { closeX, closeY, closeW, closeH };
    bool isCloseHov = CheckCollisionPointRec(mousePos, closeRec);

    DrawRectangleRounded(closeRec, 0.25f, 4, isCloseHov ? Color{ 55, 75, 110, 255 } : Color{ 30, 40, 60, 240 });
    DrawRectangleRoundedLines(closeRec, 0.25f, 4, isCloseHov ? Color{ 100, 220, 255, 255 } : Color{ 60, 80, 110, 255 });

    const char* clText = "CLOSE [ESC / C]";
    int clW = MeasureText(clText, 12);
    DrawText(clText, (int)(closeX + (closeW - clW) * 0.5f), (int)(closeY + 13), 12, isCloseHov ? Color{ 255, 255, 255, 255 } : Color{ 200, 220, 245, 220 });

    if ((isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_C) || IsKeyPressed(KEY_TAB)) {
        SoundFX::PlayButtonClick();
        return true; // Close requested
    }
    return false;
}

void GameUI::DrawLobbyHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore) {
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

void GameUI::DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves, bool hasUnreadLore) {
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
    const float bannerW = 280.0f;
    const float bannerH = 40.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 16, 20, 28, 210 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 55, 70, 95, 230 });

    std::string waveStr = "COMBAT ARENA - WAVE " + std::to_string(currentWave) + " / " + std::to_string(totalWaves);
    int waveW = MeasureText(waveStr.c_str(), 14);
    DrawText(waveStr.c_str(), (int)(bannerX + (bannerW - waveW) * 0.5f), (int)(bannerY + 13), 14, Color{ 255, 220, 100, 255 });

    // Journal & Lore button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[I] Journal", windowWidth - 263, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 170, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 128, 25, 12, Color{ 120, 220, 255, 240 });
}

void GameUI::DrawBossHealthBar(int windowWidth, const std::string& bossName, float hp, float maxHp) {
    const float barW = 540.0f;
    const float barH = 18.0f;
    float barX = ((float)windowWidth - barW) * 0.5f;
    float barY = 64.0f;

    // Outer backing plate
    DrawRectangleRounded(Rectangle{ barX - 16.0f, barY - 26.0f, barW + 32.0f, barH + 38.0f }, 0.2f, 4, Color{ 16, 12, 20, 230 });
    DrawRectangleRoundedLines(Rectangle{ barX - 16.0f, barY - 26.0f, barW + 32.0f, barH + 38.0f }, 0.2f, 4, Color{ 140, 90, 40, 240 });

    // Boss Name & Crest
    int nameW = MeasureText(bossName.c_str(), 14);
    DrawText(bossName.c_str(), (int)(barX + (barW - nameW) * 0.5f), (int)barY - 20, 14, Color{ 255, 215, 80, 255 });

    // Bar background
    DrawRectangleRounded(Rectangle{ barX, barY, barW, barH }, 0.2f, 4, Color{ 35, 15, 22, 240 });

    // Bar fill
    float hpPercent = fmaxf(0.0f, fminf(1.0f, hp / maxHp));
    DrawRectangleRounded(Rectangle{ barX, barY, barW * hpPercent, barH }, 0.2f, 4, Color{ 220, 45, 65, 255 });

    // Golden frame
    DrawRectangleRoundedLines(Rectangle{ barX, barY, barW, barH }, 0.2f, 4, Color{ 255, 200, 60, 220 });

    // HP textual value
    std::string valStr = std::to_string((int)hp) + " / " + std::to_string((int)maxHp);
    int valW = MeasureText(valStr.c_str(), 10);
    DrawText(valStr.c_str(), (int)(barX + (barW - valW) * 0.5f), (int)barY + 4, 10, Color{ 255, 255, 255, 240 });
}

void GameUI::DrawGameOver(int windowWidth, int windowHeight) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 20, 5, 8, 210 });
    int titleW = MeasureText("FALLEN IN BATTLE", 38);
    DrawText("FALLEN IN BATTLE", (windowWidth - titleW) / 2, windowHeight / 2 - 40, 38, Color{ 255, 50, 60, 255 });
    int subW = MeasureText("Press [R] to Respawn  |  [L] Return to Lobby", 18);
    DrawText("Press [R] to Respawn  |  [L] Return to Lobby", (windowWidth - subW) / 2, windowHeight / 2 + 18, 18, Color{ 220, 225, 240, 230 });
}

void GameUI::DrawVictory(int windowWidth, int windowHeight) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 10, 24, 18, 210 });
    int titleW = MeasureText("ARENA CLEARED", 38);
    DrawText("ARENA CLEARED", (windowWidth - titleW) / 2, windowHeight / 2 - 40, 38, Color{ 255, 225, 80, 255 });
    int subW = MeasureText("Press [R] to Replay  |  [L] Return to Lobby", 18);
    DrawText("Press [R] to Replay  |  [L] Return to Lobby", (windowWidth - subW) / 2, windowHeight / 2 + 18, 18, Color{ 210, 240, 225, 230 });
}

static void DrawJournalWrappedText(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color) {
    std::istringstream stream(text);
    std::string paragraph;
    int currentY = posY;

    while (std::getline(stream, paragraph)) {
        if (paragraph.empty()) {
            currentY += fontSize + lineSpacing;
            continue;
        }

        std::istringstream wordStream(paragraph);
        std::string word;
        std::string currentLine = "";

        while (wordStream >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            int testWidth = MeasureText(testLine.c_str(), fontSize);

            if (testWidth > maxLineWidth && !currentLine.empty()) {
                DrawText(currentLine.c_str(), posX, currentY, fontSize, color);
                currentY += fontSize + lineSpacing;
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }

        if (!currentLine.empty()) {
            DrawText(currentLine.c_str(), posX, currentY, fontSize, color);
            currentY += fontSize + lineSpacing;
        }
    }
}

bool GameUI::DrawInventoryModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex) {
    // Dimmed cinematic backdrop
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 6, 9, 14, 215 });

    float modalW = 1080.0f;
    float modalH = 640.0f;
    float modalX = ((float)windowWidth - modalW) * 0.5f;
    float modalY = ((float)windowHeight - modalH) * 0.5f;

    // Outer backing card & filigree borders
    DrawRectangleRounded(Rectangle{ modalX, modalY, modalW, modalH }, 0.04f, 8, Color{ 14, 17, 26, 250 });
    DrawRectangleRoundedLines(Rectangle{ modalX, modalY, modalW, modalH }, 0.04f, 8, Color{ 205, 165, 75, 230 });
    DrawRectangleRoundedLines(Rectangle{ modalX + 4, modalY + 4, modalW - 8, modalH - 8 }, 0.035f, 8, Color{ 45, 58, 82, 180 });

    // Golden corner filigree
    Color cornerCol = Color{ 255, 215, 80, 240 };
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 38, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 12, modalY + 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 38, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 12, modalY + 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 38, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 12, modalY + modalH - 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 38, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 12, modalY + modalH - 38 }, 2.0f, cornerCol);

    // Modal Header
    DrawText("CHRONICLES & ARMORY OF TYDESHIRE", (int)modalX + 36, (int)modalY + 20, 22, Color{ 255, 220, 90, 255 });
    DrawText("Manage royal armaments, inspect combat stats, and read uncovered lore fragments.", (int)modalX + 36, (int)modalY + 48, 12, Color{ 175, 195, 220, 220 });

    // Close Button [ESC / I]
    const float closeW = 120.0f;
    const float closeH = 32.0f;
    float closeX = modalX + modalW - closeW - 32.0f;
    float closeY = modalY + 22.0f;
    Rectangle closeRec = { closeX, closeY, closeW, closeH };
    bool isCloseHov = CheckCollisionPointRec(mousePos, closeRec);

    DrawRectangleRounded(closeRec, 0.25f, 4, isCloseHov ? Color{ 55, 75, 110, 255 } : Color{ 28, 36, 52, 230 });
    DrawRectangleRoundedLines(closeRec, 0.25f, 4, isCloseHov ? Color{ 120, 220, 255, 255 } : Color{ 70, 90, 125, 230 });
    DrawText("CLOSE [ESC/I]", (int)closeX + 16, (int)closeY + 9, 12, isCloseHov ? Color{ 255, 255, 255, 255 } : Color{ 190, 210, 235, 220 });

    bool closeClicked = (isCloseHov && isMousePressed);

    // Tab Navigation Bar
    float tabY = modalY + 76.0f;
    DrawLine((int)modalX + 32, (int)tabY + 34, (int)(modalX + modalW - 32), (int)tabY + 34, Color{ 45, 58, 82, 255 });

    // Tab 1: Equipment & Arsenal
    Rectangle tab1Rec = { modalX + 36, tabY, 230.0f, 34.0f };
    bool isTab1Hov = CheckCollisionPointRec(mousePos, tab1Rec);
    if ((isTab1Hov && isMousePressed) || IsKeyPressed(KEY_ONE)) {
        activeTab = 0;
        SoundFX::PlayButtonClick();
    }

    if (activeTab == 0) {
        DrawRectangleRounded(tab1Rec, 0.2f, 4, Color{ 32, 42, 64, 255 });
        DrawRectangleRoundedLines(tab1Rec, 0.2f, 4, Color{ 255, 215, 80, 240 });
        DrawText("1. EQUIPMENT & ARSENAL", (int)tab1Rec.x + 16, (int)tab1Rec.y + 10, 13, Color{ 255, 225, 100, 255 });
    } else {
        DrawRectangleRounded(tab1Rec, 0.2f, 4, isTab1Hov ? Color{ 24, 30, 46, 220 } : Color{ 18, 22, 34, 180 });
        DrawText("1. EQUIPMENT & ARSENAL", (int)tab1Rec.x + 16, (int)tab1Rec.y + 10, 13, isTab1Hov ? Color{ 240, 245, 255, 255 } : Color{ 160, 175, 195, 200 });
    }

    // Tab 2: Lore Journal
    Rectangle tab2Rec = { modalX + 276, tabY, 210.0f, 34.0f };
    bool isTab2Hov = CheckCollisionPointRec(mousePos, tab2Rec);
    if ((isTab2Hov && isMousePressed) || IsKeyPressed(KEY_TWO)) {
        activeTab = 1;
        SoundFX::PlayButtonClick();
    }

    bool hasUnread = player.GetInventory().HasUnreadFragments();
    std::string tab2Label = hasUnread ? "2. LORE JOURNAL (!)" : "2. LORE JOURNAL";

    if (activeTab == 1) {
        DrawRectangleRounded(tab2Rec, 0.2f, 4, Color{ 32, 42, 64, 255 });
        DrawRectangleRoundedLines(tab2Rec, 0.2f, 4, Color{ 255, 215, 80, 240 });
        DrawText(tab2Label.c_str(), (int)tab2Rec.x + 16, (int)tab2Rec.y + 10, 13, Color{ 255, 225, 100, 255 });
    } else {
        DrawRectangleRounded(tab2Rec, 0.2f, 4, isTab2Hov ? Color{ 24, 30, 46, 220 } : Color{ 18, 22, 34, 180 });
        DrawText(tab2Label.c_str(), (int)tab2Rec.x + 16, (int)tab2Rec.y + 10, 13, hasUnread ? Color{ 255, 215, 80, 255 } : (isTab2Hov ? Color{ 240, 245, 255, 255 } : Color{ 160, 175, 195, 200 }));
    }

    // TAB CONTENT AREA
    float contentY = modalY + 125.0f;

    if (activeTab == 0) {
        // TAB 0: EQUIPMENT & ARSENAL
        // Left Column: Active Weapon & Arsenal (width = 510)
        float col1X = modalX + 36.0f;
        float col1W = 510.0f;

        // Active Weapon Card
        WeaponType curW = player.GetWeapon();
        WeaponStats curStats = GetWeaponStats(curW);

        DrawRectangleRounded(Rectangle{ col1X, contentY, col1W, 160.0f }, 0.08f, 6, Color{ 20, 25, 38, 220 });
        DrawRectangleRoundedLines(Rectangle{ col1X, contentY, col1W, 160.0f }, 0.08f, 6, (curW == WeaponType::TRUTH_SEEKER) ? Color{ 255, 215, 80, 240 } : Color{ 65, 85, 120, 230 });

        DrawText("CURRENTLY EQUIPPED:", (int)col1X + 18, (int)contentY + 14, 11, Color{ 255, 215, 80, 255 });
        DrawText(curStats.name.c_str(), (int)col1X + 18, (int)contentY + 30, 20, Color{ 255, 255, 255, 255 });

        const char* rarityText = (curW == WeaponType::TRUTH_SEEKER) ? "[LEGENDARY ROYAL RELIC]" : "[STANDARD COMBAT WEAPON]";
        Color rarityColor = (curW == WeaponType::TRUTH_SEEKER) ? Color{ 255, 215, 70, 255 } : Color{ 100, 220, 255, 240 };
        DrawText(rarityText, (int)col1X + 18 + MeasureText(curStats.name.c_str(), 20) + 12, (int)contentY + 36, 11, rarityColor);

        // Stats grid
        std::string lightStr = "Light Attack: " + std::to_string((int)curStats.lightDamage) + " Dmg | Arc: " + std::to_string((int)curStats.lightArc) + " deg | Reach: " + std::to_string((int)curStats.lightRange);
        std::string heavyStr = "Heavy Attack: " + std::to_string((int)curStats.heavyDamage) + " Dmg | CD: " + std::to_string((int)curStats.heavyCooldown) + "s | Reach: " + std::to_string((int)curStats.heavyRange);
        std::string dashStr = "Dash Recovery: " + std::to_string((int)curStats.dashCooldown) + "s cooldown | Stamina: " + std::to_string((int)curStats.dashStaminaCost) + " SP";

        DrawText(lightStr.c_str(), (int)col1X + 18, (int)contentY + 62, 11, Color{ 130, 230, 255, 240 });
        DrawText(heavyStr.c_str(), (int)col1X + 18, (int)contentY + 80, 11, Color{ 255, 185, 80, 240 });
        DrawText(dashStr.c_str(), (int)col1X + 18, (int)contentY + 98, 11, Color{ 160, 240, 180, 240 });
        DrawText(curStats.description.c_str(), (int)col1X + 18, (int)contentY + 124, 10, Color{ 185, 205, 225, 200 });

        // Arsenal Bag Section
        float arsenalY = contentY + 176.0f;
        DrawText("WEAPON ARSENAL (Click unlocked weapon to equip):", (int)col1X, (int)arsenalY, 13, Color{ 255, 220, 100, 255 });

        struct WeaponEntry {
            WeaponType type;
            const char* title;
            const char* tag;
        };
        WeaponEntry arsenalList[] = {
            { WeaponType::SWORD, "Broadsword", "Balanced reach & recovery" },
            { WeaponType::DAGGERS, "Dual Daggers", "Swift twin flurry & 360 spin" },
            { WeaponType::MACE, "War Mace", "Heavy poise crushing slam" },
            { WeaponType::TRUTH_SEEKER, "Truth Seeker", "Holy royal blade of King Charlz" }
        };

        for (int i = 0; i < 4; i++) {
            float itemY = arsenalY + 24.0f + (float)i * 68.0f;
            Rectangle itemRec = { col1X, itemY, col1W, 58.0f };
            bool isHov = CheckCollisionPointRec(mousePos, itemRec);
            bool isEquipped = (curW == arsenalList[i].type);
            bool isUnlocked = player.GetInventory().HasWeapon(arsenalList[i].type);

            Color bg = isEquipped ? Color{ 36, 46, 68, 255 } : (isHov && isUnlocked ? Color{ 28, 38, 56, 240 } : Color{ 18, 22, 32, 200 });
            Color border = isEquipped ? Color{ 255, 215, 80, 255 } : (isHov && isUnlocked ? Color{ 100, 220, 255, 255 } : Color{ 48, 62, 85, 180 });

            DrawRectangleRounded(itemRec, 0.15f, 4, bg);
            DrawRectangleRoundedLines(itemRec, 0.15f, 4, border);

            if (isUnlocked) {
                DrawText(arsenalList[i].title, (int)itemRec.x + 16, (int)itemRec.y + 12, 14, isEquipped ? Color{ 255, 225, 100, 255 } : Color{ 240, 245, 255, 255 });
                DrawText(arsenalList[i].tag, (int)itemRec.x + 16, (int)itemRec.y + 32, 11, Color{ 160, 185, 210, 200 });

                if (isEquipped) {
                    DrawText("[EQUIPPED]", (int)(itemRec.x + itemRec.width - 100), (int)itemRec.y + 20, 12, Color{ 255, 215, 80, 255 });
                } else if (isHov) {
                    DrawText("[CLICK TO EQUIP]", (int)(itemRec.x + itemRec.width - 140), (int)itemRec.y + 20, 12, Color{ 100, 240, 255, 255 });
                }

                if (isHov && isMousePressed && !isEquipped) {
                    player.SetWeapon(arsenalList[i].type);
                }
            } else {
                // Locked Weapon
                DrawText(arsenalList[i].title, (int)itemRec.x + 16, (int)itemRec.y + 12, 14, Color{ 110, 120, 135, 255 });
                DrawText("[LOCKED - Defeat Zil in the Castle Ruins to claim]", (int)itemRec.x + 16, (int)itemRec.y + 32, 11, Color{ 200, 90, 80, 220 });
            }
        }

        // Right Column: Knight Attributes & Relics (width = 470)
        float col2X = modalX + 575.0f;
        float col2W = 470.0f;

        DrawRectangleRounded(Rectangle{ col2X, contentY, col2W, 220.0f }, 0.08f, 6, Color{ 18, 23, 34, 220 });
        DrawRectangleRoundedLines(Rectangle{ col2X, contentY, col2W, 220.0f }, 0.08f, 6, Color{ 55, 75, 105, 230 });

        DrawText("KNIGHT COMBAT ATTRIBUTES", (int)col2X + 20, (int)contentY + 16, 14, Color{ 255, 220, 90, 255 });
        DrawLine((int)col2X + 20, (int)contentY + 38, (int)(col2X + col2W - 20), (int)contentY + 38, Color{ 40, 52, 75, 255 });

        // Health Bar & values
        float hpPerc = fmaxf(0.0f, player.GetHealth() / player.GetMaxHealth());
        DrawText("Health Points:", (int)col2X + 20, (int)contentY + 52, 11, Color{ 230, 90, 100, 255 });
        DrawRectangle((int)col2X + 130, (int)contentY + 52, 200, 12, Color{ 35, 20, 25, 220 });
        DrawRectangle((int)col2X + 130, (int)contentY + 52, (int)(200 * hpPerc), 12, Color{ 235, 55, 75, 255 });
        std::string hpStr = std::to_string((int)player.GetHealth()) + " / " + std::to_string((int)player.GetMaxHealth());
        DrawText(hpStr.c_str(), (int)col2X + 345, (int)contentY + 52, 11, Color{ 255, 255, 255, 240 });

        // Stamina Bar & values
        float stPerc = fmaxf(0.0f, player.GetStamina() / player.GetMaxStamina());
        DrawText("Stamina:", (int)col2X + 20, (int)contentY + 76, 11, Color{ 255, 210, 80, 255 });
        DrawRectangle((int)col2X + 130, (int)contentY + 76, 200, 12, Color{ 35, 30, 15, 220 });
        DrawRectangle((int)col2X + 130, (int)contentY + 76, (int)(200 * stPerc), 12, Color{ 255, 195, 45, 255 });
        std::string stStr = std::to_string((int)player.GetStamina()) + " / " + std::to_string((int)player.GetMaxStamina());
        DrawText(stStr.c_str(), (int)col2X + 345, (int)contentY + 76, 11, Color{ 255, 255, 255, 240 });

        // Passive mechanics
        DrawText("Dodge Invulnerability:", (int)col2X + 20, (int)contentY + 104, 11, Color{ 140, 210, 255, 240 });
        DrawText("0.18s evasive i-frames on [Q] Dash", (int)col2X + 175, (int)contentY + 104, 11, Color{ 240, 245, 255, 255 });

        DrawText("Parry Guard Window:", (int)col2X + 20, (int)contentY + 128, 11, Color{ 140, 210, 255, 240 });
        DrawText("0.28s active deflect window on [F] Key", (int)col2X + 175, (int)contentY + 128, 11, Color{ 240, 245, 255, 255 });

        DrawText("Counter-Attack Bonus:", (int)col2X + 20, (int)contentY + 152, 11, Color{ 140, 210, 255, 240 });
        DrawText("+50% damage boost following parries", (int)col2X + 175, (int)contentY + 152, 11, Color{ 255, 220, 80, 255 });

        DrawText("Movement Speed:", (int)col2X + 20, (int)contentY + 176, 11, Color{ 140, 210, 255, 240 });
        DrawText("280 units/sec (8-directional physics)", (int)col2X + 175, (int)contentY + 176, 11, Color{ 240, 245, 255, 255 });

        // Relics & Quest Items Section
        float relicsY = contentY + 236.0f;
        DrawRectangleRounded(Rectangle{ col2X, relicsY, col2W, 230.0f }, 0.08f, 6, Color{ 18, 23, 34, 220 });
        DrawRectangleRoundedLines(Rectangle{ col2X, relicsY, col2W, 230.0f }, 0.08f, 6, Color{ 55, 75, 105, 230 });

        DrawText("RELICS & KEY QUEST ITEMS", (int)col2X + 20, (int)relicsY + 16, 14, Color{ 255, 220, 90, 255 });
        DrawLine((int)col2X + 20, (int)relicsY + 38, (int)(col2X + col2W - 20), (int)relicsY + 38, Color{ 40, 52, 75, 255 });

        const auto& qItems = player.GetInventory().GetQuestItems();
        if (qItems.empty()) {
            DrawText("No quest artifacts collected yet.", (int)col2X + 20, (int)relicsY + 54, 12, Color{ 150, 165, 185, 200 });
        } else {
            for (size_t i = 0; i < qItems.size() && i < 3; i++) {
                float qy = relicsY + 50.0f + (float)i * 54.0f;
                DrawRectangleRounded(Rectangle{ col2X + 16, qy, col2W - 32, 46.0f }, 0.15f, 4, Color{ 24, 30, 44, 180 });
                DrawText(qItems[i].name.c_str(), (int)col2X + 28, (int)qy + 8, 12, qItems[i].rarityColor);
                DrawText(qItems[i].description.c_str(), (int)col2X + 28, (int)qy + 26, 10, Color{ 180, 200, 220, 220 });
            }
        }
    } else {
        // TAB 1: LORE JOURNAL
        // Left Column: List of Lore Fragments
        float col1X = modalX + 36.0f;
        float col1W = 360.0f;
        const auto& frags = player.GetInventory().GetFragments();
        int totalFrags = (int)frags.size();
        int discoveredFrags = player.GetInventory().GetDiscoveredFragmentCount();

        std::string listTitle = "LORE FRAGMENTS (" + std::to_string(discoveredFrags) + " / " + std::to_string(totalFrags) + " FOUND)";
        DrawText(listTitle.c_str(), (int)col1X, (int)contentY, 13, Color{ 255, 220, 90, 255 });

        for (int i = 0; i < totalFrags; i++) {
            float fragY = contentY + 24.0f + (float)i * 105.0f;
            Rectangle fragRec = { col1X, fragY, col1W, 96.0f };
            bool isHov = CheckCollisionPointRec(mousePos, fragRec);
            bool isSel = (selectedFragmentIndex == i);

            Color bg = isSel ? Color{ 36, 46, 68, 255 } : (isHov ? Color{ 26, 34, 52, 220 } : Color{ 18, 22, 32, 200 });
            Color border = isSel ? Color{ 255, 215, 80, 255 } : (isHov ? Color{ 100, 220, 255, 240 } : Color{ 48, 62, 85, 180 });

            DrawRectangleRounded(fragRec, 0.12f, 4, bg);
            DrawRectangleRoundedLines(fragRec, 0.12f, 4, border);

            if (frags[i].discovered) {
                // Realm tag
                std::string realmTag = "[" + frags[i].realm + "]";
                DrawText(realmTag.c_str(), (int)fragRec.x + 14, (int)fragRec.y + 10, 10, Color{ 100, 220, 255, 240 });

                // Title
                DrawText(frags[i].title.c_str(), (int)fragRec.x + 14, (int)fragRec.y + 26, 13, isSel ? Color{ 255, 225, 100, 255 } : Color{ 240, 245, 255, 255 });

                // Preview
                std::string prev = frags[i].preview;
                if (prev.length() > 52) prev = prev.substr(0, 49) + "...";
                DrawText(prev.c_str(), (int)fragRec.x + 14, (int)fragRec.y + 50, 10, Color{ 170, 190, 215, 200 });

                // Unread Badge
                if (!frags[i].isRead) {
                    DrawRectangleRounded(Rectangle{ fragRec.x + fragRec.width - 55.0f, fragRec.y + 10.0f, 42.0f, 18.0f }, 0.3f, 4, Color{ 180, 130, 20, 255 });
                    DrawText("NEW", (int)(fragRec.x + fragRec.width - 47.0f), (int)fragRec.y + 13.0f, 10, Color{ 255, 245, 200, 255 });
                }

                if (isHov && isMousePressed) {
                    selectedFragmentIndex = i;
                    player.GetInventory().MarkFragmentRead(frags[i].id);
                    SoundFX::PlayButtonClick();
                }
            } else {
                // Undiscovered Fragment
                DrawText("[UNDISCOVERED FRAGMENT]", (int)fragRec.x + 14, (int)fragRec.y + 16, 12, Color{ 120, 135, 155, 255 });
                DrawText("Search ruins and battlements across Tydeshire", (int)fragRec.x + 14, (int)fragRec.y + 40, 10, Color{ 90, 105, 125, 200 });
                DrawText("to uncover this lost chronicle.", (int)fragRec.x + 14, (int)fragRec.y + 56, 10, Color{ 90, 105, 125, 200 });

                if (isHov && isMousePressed) {
                    selectedFragmentIndex = i;
                    SoundFX::PlayButtonClick();
                }
            }
        }

        // Right Column: Reading Manuscript Panel (width = 620, height = 440)
        float col2X = modalX + 420.0f;
        float col2W = 620.0f;
        float col2H = 440.0f;

        DrawRectangleRounded(Rectangle{ col2X, contentY, col2W, col2H }, 0.05f, 6, Color{ 20, 24, 36, 245 });
        DrawRectangleRoundedLines(Rectangle{ col2X, contentY, col2W, col2H }, 0.05f, 6, Color{ 180, 140, 60, 220 });
        DrawRectangleRoundedLines(Rectangle{ col2X + 4, contentY + 4, col2W - 8, col2H - 8 }, 0.04f, 6, Color{ 50, 65, 90, 180 });

        if (selectedFragmentIndex >= 0 && selectedFragmentIndex < totalFrags) {
            const auto& curFrag = frags[selectedFragmentIndex];
            if (curFrag.discovered) {
                // Illuminated Realm Heading
                std::string originStr = "HISTORICAL RECORD // " + curFrag.realm;
                DrawText(originStr.c_str(), (int)col2X + 28, (int)contentY + 24, 11, Color{ 120, 220, 255, 240 });

                // Fragment Title in Gold
                DrawText(curFrag.title.c_str(), (int)col2X + 28, (int)contentY + 44, 20, Color{ 255, 215, 80, 255 });
                DrawLine((int)col2X + 28, (int)contentY + 76, (int)(col2X + col2W - 28), (int)contentY + 76, Color{ 160, 125, 55, 220 });

                // Narrative wrapped text
                DrawJournalWrappedText(curFrag.fullText, (int)col2X + 28, (int)contentY + 98, (int)col2W - 56, 14, 8, Color{ 230, 238, 250, 245 });

                // Footer quote
                DrawLine((int)col2X + 28, (int)(contentY + col2H - 46), (int)(col2X + col2W - 28), (int)(contentY + col2H - 46), Color{ 50, 65, 90, 180 });
                const char* footQuote = "“The true history of Tydeshire has been scattered. Seek the ruins for meaning.”";
                DrawText(footQuote, (int)col2X + 28, (int)(contentY + col2H - 32), 11, Color{ 200, 170, 90, 220 });
            } else {
                // Undiscovered State Reading Screen
                int uhW = MeasureText("THIS CHRONICLE FRAGMENT REMAINS SHROUDED", 16);
                DrawText("THIS CHRONICLE FRAGMENT REMAINS SHROUDED", (int)(col2X + (col2W - uhW) * 0.5f), (int)contentY + 160, 16, Color{ 215, 90, 80, 255 });

                const char* unDesc1 = "The parchment detailing this epoch was torn asunder during the Lord Liege's madness.";
                int ud1W = MeasureText(unDesc1, 12);
                DrawText(unDesc1, (int)(col2X + (col2W - ud1W) * 0.5f), (int)contentY + 200, 12, Color{ 170, 190, 215, 220 });

                const char* unDesc2 = "Defeat corrupted crusaders and search forgotten shrines across the Land of Tydeshire to recover it.";
                int ud2W = MeasureText(unDesc2, 12);
                DrawText(unDesc2, (int)(col2X + (col2W - ud2W) * 0.5f), (int)contentY + 224, 12, Color{ 150, 170, 195, 200 });
            }
        }
    }

    if (closeClicked || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_I)) {
        SoundFX::PlayButtonClick();
        return true;
    }

    return false;
}

int GameUI::DrawZilDefeatModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool canRetry) {
    // Dark ominous background overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 4, 6, 225 });

    float modalW = 740.0f;
    float modalH = 430.0f;
    float modalX = ((float)windowWidth - modalW) * 0.5f;
    float modalY = ((float)windowHeight - modalH) * 0.5f;

    // Outer backing card
    DrawRectangleRounded(Rectangle{ modalX, modalY, modalW, modalH }, 0.05f, 8, Color{ 18, 12, 16, 252 });
    DrawRectangleRoundedLines(Rectangle{ modalX, modalY, modalW, modalH }, 0.05f, 8, Color{ 220, 110, 45, 240 });
    DrawRectangleRoundedLines(Rectangle{ modalX + 4, modalY + 4, modalW - 8, modalH - 8 }, 0.045f, 8, Color{ 60, 35, 40, 180 });

    // Filigree corner accents
    Color cornerCol = Color{ 255, 160, 50, 240 };
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 38, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 12, modalY + 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 38, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 12, modalY + 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 38, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 12, modalY + modalH - 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 38, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 12, modalY + modalH - 38 }, 2.0f, cornerCol);

    // Header Title
    int headerW = MeasureText("DEFEAT // ZIL'S JUDGMENT", 20);
    DrawText("DEFEAT // ZIL'S JUDGMENT", (int)(modalX + (modalW - headerW) * 0.5f), (int)modalY + 28, 20, Color{ 255, 90, 70, 255 });
    DrawLine((int)modalX + 50, (int)modalY + 62, (int)(modalX + modalW - 50), (int)modalY + 62, Color{ 140, 70, 30, 200 });

    int choiceResult = 0;

    if (canRetry) {
        // Flavor Subtitle
        int subW = MeasureText("THE FALLEN CRUSADER STANDS VICTORIOUS", 15);
        DrawText("THE FALLEN CRUSADER STANDS VICTORIOUS", (int)(modalX + (modalW - subW) * 0.5f), (int)modalY + 84, 15, Color{ 255, 205, 80, 255 });

        // Narrative speech
        std::string quoteText =
            "“You fought with fierce intent, wanderer, but Truth Seeker remains unyielding in these hands.\n\n"
            "Will you rise once more and test your blade... or yield and carry onward into Zyphden?”";
        DrawJournalWrappedText(quoteText, (int)modalX + 50, (int)modalY + 120, (int)modalW - 100, 14, 8, Color{ 230, 235, 245, 240 });

        // Lore footnote
        const char* noteText = "Honor bound: You are granted a singular retry duel to claim the legendary blade.";
        int noteW = MeasureText(noteText, 11);
        DrawText(noteText, (int)(modalX + (modalW - noteW) * 0.5f), (int)modalY + 280, 11, Color{ 255, 170, 60, 220 });

        // Button 1: Retry Battle
        Rectangle btnRetry = { modalX + 50.0f, modalY + 330.0f, 300.0f, 54.0f };
        bool hoverRetry = CheckCollisionPointRec(mousePos, btnRetry);
        DrawRectangleRounded(btnRetry, 0.2f, 6, hoverRetry ? Color{ 90, 35, 20, 255 } : Color{ 45, 22, 16, 240 });
        DrawRectangleRoundedLines(btnRetry, 0.2f, 6, hoverRetry ? Color{ 255, 190, 70, 255 } : Color{ 220, 120, 40, 230 });
        int btn1W = MeasureText("RETRY DUEL [1 REMAINING]", 14);
        DrawText("RETRY DUEL [1 REMAINING]", (int)(btnRetry.x + (btnRetry.width - btn1W) * 0.5f), (int)btnRetry.y + 19, 14, hoverRetry ? Color{ 255, 255, 255, 255 } : Color{ 255, 215, 100, 240 });

        // Button 2: Accept Spare & Proceed
        Rectangle btnProceed = { modalX + 390.0f, modalY + 330.0f, 300.0f, 54.0f };
        bool hoverProceed = CheckCollisionPointRec(mousePos, btnProceed);
        DrawRectangleRounded(btnProceed, 0.2f, 6, hoverProceed ? Color{ 35, 55, 85, 255 } : Color{ 22, 28, 42, 240 });
        DrawRectangleRoundedLines(btnProceed, 0.2f, 6, hoverProceed ? Color{ 140, 210, 255, 255 } : Color{ 90, 130, 180, 230 });
        int btn2W = MeasureText("ACCEPT SPARE & PROCEED", 14);
        DrawText("ACCEPT SPARE & PROCEED", (int)(btnProceed.x + (btnProceed.width - btn2W) * 0.5f), (int)btnProceed.y + 19, 14, hoverProceed ? Color{ 255, 255, 255, 255 } : Color{ 180, 215, 255, 240 });

        if (isMousePressed) {
            if (hoverRetry) {
                SoundFX::PlayButtonClick();
                choiceResult = 1;
            } else if (hoverProceed) {
                SoundFX::PlayButtonClick();
                choiceResult = 2;
            }
        }
    } else {
        // Defeated on Retry Screen
        int subW = MeasureText("SPARED BY THE CORRUPTED CRUSADER", 15);
        DrawText("SPARED BY THE CORRUPTED CRUSADER", (int)(modalX + (modalW - subW) * 0.5f), (int)modalY + 84, 15, Color{ 140, 210, 255, 255 });

        std::string quoteText =
            "“You have exhausted your trials, yet your spirit refuses to be extinguished.\n\n"
            "I shall not strike down one who dares to question the Lord Liege. Keep your broadsword and walk through the northern gate into Zyphden.\n\n"
            "Seek the scattered chronicles... and pierce the truth of King Charlz.”";
        DrawJournalWrappedText(quoteText, (int)modalX + 50, (int)modalY + 120, (int)modalW - 100, 14, 8, Color{ 230, 235, 245, 240 });

        // Single Button: Proceed
        Rectangle btnProceed = { modalX + 170.0f, modalY + 330.0f, 400.0f, 54.0f };
        bool hoverProceed = CheckCollisionPointRec(mousePos, btnProceed);
        DrawRectangleRounded(btnProceed, 0.2f, 6, hoverProceed ? Color{ 35, 60, 95, 255 } : Color{ 24, 32, 50, 240 });
        DrawRectangleRoundedLines(btnProceed, 0.2f, 6, hoverProceed ? Color{ 140, 220, 255, 255 } : Color{ 100, 150, 210, 230 });
        int btnW = MeasureText("PROCEED INTO THE REALM OF ZYPHDEN", 14);
        DrawText("PROCEED INTO THE REALM OF ZYPHDEN", (int)(btnProceed.x + (btnProceed.width - btnW) * 0.5f), (int)btnProceed.y + 19, 14, hoverProceed ? Color{ 255, 255, 255, 255 } : Color{ 190, 225, 255, 240 });

        if (isMousePressed && hoverProceed) {
            SoundFX::PlayButtonClick();
            choiceResult = 2;
        }
    }

    return choiceResult;
}

void GameUI::DrawBossDialogueBanner(int windowWidth, int windowHeight, const std::string& speaker, const std::string& text, Color accentColor) {
    float bannerW = 820.0f;
    float bannerH = 74.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = (float)windowHeight - 165.0f;

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.2f, 6, Color{ 12, 15, 22, 235 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.2f, 6, accentColor);

    // Speaker Name
    DrawText(speaker.c_str(), (int)bannerX + 22, (int)bannerY + 12, 11, accentColor);

    // Text Quote
    DrawText(text.c_str(), (int)bannerX + 22, (int)bannerY + 32, 14, Color{ 240, 244, 255, 250 });
}

