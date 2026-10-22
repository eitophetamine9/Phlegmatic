#include "UI.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/Weapon.hpp"
#include <cmath>

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

void GameUI::DrawLobbyHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, bool isNearPedestal, const std::string& nearWeaponName) {
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
    const float hudWidth = 280.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 185, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(185 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 185, 12, Color{ 80, 40, 50, 220 });

    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 185, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(185 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 185, 12, Color{ 90, 75, 30, 220 });

    std::string dashStatus = (dashCooldown <= 0.0f) ? "[Q] Dash Ready" : "[Q] Dash CD";
    std::string parryStatus = (parryCooldown <= 0.0f) ? "[F] Parry Ready" : "[F] Parry CD";
    DrawText((dashStatus + "  |  " + parryStatus).c_str(), (int)hudX + 14, (int)hudY + 58, 11, Color{ 150, 180, 210, 220 });
}

void GameUI::DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves) {
    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 110.0f;
    const float hudWidth = 280.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 185, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(185 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 185, 12, Color{ 80, 40, 50, 220 });

    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 185, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(185 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 185, 12, Color{ 90, 75, 30, 220 });

    if (hasCounterBuff) {
        DrawText("COUNTER BUFF: +50% DAMAGE!", (int)hudX + 14, (int)hudY + 58, 12, Color{ 255, 230, 80, 255 });
    } else if (isParrying) {
        DrawText("PARRY WINDOW ACTIVE!", (int)hudX + 14, (int)hudY + 58, 12, Color{ 80, 240, 255, 255 });
    } else {
        std::string dashStatus = (dashCooldown <= 0.0f) ? "[Q] Dash Ready" : "[Q] Dash CD";
        std::string parryStatus = (parryCooldown <= 0.0f) ? "[F] Parry Ready" : "[F] Parry CD";
        DrawText((dashStatus + "  |  " + parryStatus).c_str(), (int)hudX + 14, (int)hudY + 58, 11, Color{ 150, 180, 210, 220 });
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
