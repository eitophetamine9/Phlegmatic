#include "UI.hpp"
#include "../core/SoundFX.hpp"
#include <cmath>

bool GameUI::DrawTitleScreen(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed) {
    // Atmospheric dark backdrop
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 12, 14, 20, 255 });

    float time = (float)GetTime();

    // Ambient background rings
    Vector2 center = { (float)windowWidth * 0.5f, (float)windowHeight * 0.42f };
    float pulse = 1.0f + 0.04f * sinf(time * 2.5f);
    DrawCircleLines((int)center.x, (int)center.y, 280.0f * pulse, Color{ 35, 45, 65, 120 });
    DrawCircleLines((int)center.x, (int)center.y, 160.0f * pulse, Color{ 50, 70, 105, 140 });

    // Title text
    const char* title = "PHLEGMATIC";
    int titleSize = 64;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (int)(center.x - titleW * 0.5f + 2), (int)(center.y - 120 + 2), titleSize, Color{ 0, 0, 0, 200 });
    DrawText(title, (int)(center.x - titleW * 0.5f), (int)(center.y - 120), titleSize, Color{ 255, 220, 100, 255 });

    // Subtitle
    const char* subtitle = "A Top-Down Action RPG of Composure & Precision";
    int subSize = 18;
    int subW = MeasureText(subtitle, subSize);
    DrawText(subtitle, (int)(center.x - subW * 0.5f), (int)(center.y - 50), subSize, Color{ 180, 200, 230, 230 });

    // Interactive PLAY Button
    const float btnWidth = 260.0f;
    const float btnHeight = 60.0f;
    float btnX = center.x - btnWidth * 0.5f;
    float btnY = center.y + 20.0f;

    Rectangle btnRec = { btnX, btnY, btnWidth, btnHeight };
    bool isHovered = CheckCollisionPointRec(mousePos, btnRec);

    Color btnBg = isHovered ? Color{ 45, 75, 130, 255 } : Color{ 25, 35, 55, 230 };
    Color btnBorder = isHovered ? Color{ 100, 220, 255, 255 } : Color{ 60, 85, 125, 255 };

    DrawRectangleRounded(btnRec, 0.25f, 6, btnBg);
    DrawRectangleRoundedLines(btnRec, 0.25f, 6, btnBorder);

    const char* playText = "PLAY";
    int playSize = 26;
    int playW = MeasureText(playText, playSize);
    DrawText(playText, (int)(center.x - playW * 0.5f), (int)(btnY + 16), playSize, isHovered ? Color{ 255, 255, 255, 255 } : Color{ 210, 230, 255, 240 });

    // Controls Overview Panel
    const float ctrlW = 620.0f;
    const float ctrlH = 90.0f;
    float ctrlX = center.x - ctrlW * 0.5f;
    float ctrlY = (float)windowHeight - ctrlH - 30.0f;

    DrawRectangleRounded(Rectangle{ ctrlX, ctrlY, ctrlW, ctrlH }, 0.15f, 6, Color{ 18, 22, 32, 220 });
    DrawRectangleRoundedLines(Rectangle{ ctrlX, ctrlY, ctrlW, ctrlH }, 0.15f, 6, Color{ 45, 58, 80, 220 });

    DrawText("CONTROLS:", (int)ctrlX + 20, (int)ctrlY + 12, 13, Color{ 255, 215, 80, 255 });
    DrawText("WASD / Arrows: Move  |  Mouse: Aim  |  L-Click: Light Attack  |  R-Click: Heavy Attack", (int)ctrlX + 20, (int)ctrlY + 36, 13, Color{ 210, 225, 245, 230 });
    DrawText("Q: Dash (Evasion / i-frames)  |  F: Timely Parry (Reflect & Stagger)  |  R: Reset", (int)ctrlX + 20, (int)ctrlY + 58, 13, Color{ 120, 230, 255, 240 });

    if (isHovered && isMousePressed) {
        SoundFX::PlayButtonClick();
        return true;
    }
    return false;
}

void GameUI::DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves) {
    // Player HUD Panel (Bottom Left)
    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 110.0f;
    const float hudWidth = 280.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    // Health Bar
    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 185, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(185 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 185, 12, Color{ 80, 40, 50, 220 });

    // Stamina Bar
    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 185, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(185 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 185, 12, Color{ 90, 75, 30, 220 });

    // Action Cooldown & Stance Indicators
    if (hasCounterBuff) {
        DrawText("COUNTER BUFF: +50% DAMAGE!", (int)hudX + 14, (int)hudY + 58, 12, Color{ 255, 230, 80, 255 });
    } else if (isParrying) {
        DrawText("PARRY WINDOW ACTIVE!", (int)hudX + 14, (int)hudY + 58, 12, Color{ 80, 240, 255, 255 });
    } else {
        std::string dashStatus = (dashCooldown <= 0.0f) ? "[Q] Dash Ready" : "[Q] Dash CD";
        std::string parryStatus = (parryCooldown <= 0.0f) ? "[F] Parry Ready" : "[F] Parry CD";
        DrawText((dashStatus + "  |  " + parryStatus).c_str(), (int)hudX + 14, (int)hudY + 58, 11, Color{ 150, 180, 210, 220 });
    }

    // Top Wave Status Banner (No story text, clean combat indicator)
    const float bannerW = 280.0f;
    const float bannerH = 40.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 16, 20, 28, 210 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 55, 70, 95, 230 });

    std::string waveStr = "COMBAT ARENA - WAVE " + std::to_string(currentWave) + " / " + std::to_string(totalWaves);
    int waveW = MeasureText(waveStr.c_str(), 14);
    DrawText(waveStr.c_str(), (int)(bannerX + (bannerW - waveW) * 0.5f), (int)(bannerY + 13), 14, Color{ 255, 220, 100, 255 });
}

void GameUI::DrawGameOver(int windowWidth, int windowHeight) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 20, 5, 8, 210 });
    int titleW = MeasureText("FALLEN IN BATTLE", 38);
    DrawText("FALLEN IN BATTLE", (windowWidth - titleW) / 2, windowHeight / 2 - 40, 38, Color{ 255, 50, 60, 255 });
    int subW = MeasureText("Press [R] to Respawn", 18);
    DrawText("Press [R] to Respawn", (windowWidth - subW) / 2, windowHeight / 2 + 18, 18, Color{ 220, 225, 240, 230 });
}

void GameUI::DrawVictory(int windowWidth, int windowHeight) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 10, 24, 18, 210 });
    int titleW = MeasureText("ARENA CLEARED", 38);
    DrawText("ARENA CLEARED", (windowWidth - titleW) / 2, windowHeight / 2 - 40, 38, Color{ 255, 225, 80, 255 });
    int subW = MeasureText("Press [R] to Replay", 18);
    DrawText("Press [R] to Replay", (windowWidth - subW) / 2, windowHeight / 2 + 18, 18, Color{ 210, 240, 225, 230 });
}
