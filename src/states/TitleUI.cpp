#include "TitleUI.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/FontManager.hpp"
#include <cmath>

int TitleUI::Draw(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool isArenaUnlocked) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 12, 14, 20, 255 });

    float time = (float)GetTime();

    Vector2 center = { (float)windowWidth * 0.5f, (float)windowHeight * 0.38f };
    float pulse = 1.0f + 0.04f * sinf(time * 2.5f);
    DrawCircleLines((int)center.x, (int)center.y, 280.0f * pulse, Color{ 35, 45, 65, 120 });
    DrawCircleLines((int)center.x, (int)center.y, 160.0f * pulse, Color{ 50, 70, 105, 140 });

    const char* title = "PHLEGMATIC";
    int titleSize = 64;
    int titleW = FontManager::MeasureTextTitle(title, titleSize);
    FontManager::DrawTextTitle(title, (int)(center.x - titleW * 0.5f + 2), (int)(center.y - 120 + 2), titleSize, Color{ 0, 0, 0, 200 });
    FontManager::DrawTextTitle(title, (int)(center.x - titleW * 0.5f), (int)(center.y - 120), titleSize, Color{ 255, 220, 100, 255 });

    const char* subtitle = "A Dark Fantasy Action RPG of Composure & Precision";
    int subSize = 18;
    int subW = FontManager::MeasureText(subtitle, subSize);
    FontManager::DrawText(subtitle, (int)(center.x - subW * 0.5f), (int)(center.y - 50), subSize, Color{ 180, 200, 230, 230 });

    // Buttons Container
    const float btnWidth = 260.0f;
    const float btnHeight = 48.0f;
    const float btnGap = 14.0f;
    float btnX = center.x - btnWidth * 0.5f;

    // 1. PLAY BUTTON
    float playY = center.y + 12.0f;
    Rectangle playRec = { btnX, playY, btnWidth, btnHeight };
    bool playHovered = CheckCollisionPointRec(mousePos, playRec);

    Color playBg = playHovered ? Color{ 45, 75, 130, 255 } : Color{ 25, 35, 55, 230 };
    Color playBorder = playHovered ? Color{ 100, 220, 255, 255 } : Color{ 60, 85, 125, 255 };
    DrawRectangleRounded(playRec, 0.25f, 6, playBg);
    DrawRectangleRoundedLines(playRec, 0.25f, 6, playBorder);

    const char* playText = "PLAY CAMPAIGN";
    int playSize = 19;
    int playW = FontManager::MeasureText(playText, playSize);
    FontManager::DrawText(playText, (int)(center.x - playW * 0.5f), (int)(playY + 14), playSize, playHovered ? Color{ 255, 255, 255, 255 } : Color{ 210, 230, 255, 240 });

    // 2. TEST ARENA BUTTON (Unlocked after winning or hard losing to Zil)
    float arenaBtnY = playY + btnHeight + btnGap;
    Rectangle arenaRec = { btnX, arenaBtnY, btnWidth, btnHeight };
    bool arenaHovered = CheckCollisionPointRec(mousePos, arenaRec);

    if (isArenaUnlocked) {
        Color arenaBg = arenaHovered ? Color{ 75, 45, 105, 255 } : Color{ 36, 24, 52, 230 };
        Color arenaBorder = arenaHovered ? Color{ 255, 215, 80, 255 } : Color{ 175, 110, 230, 240 };
        DrawRectangleRounded(arenaRec, 0.25f, 6, arenaBg);
        DrawRectangleRoundedLines(arenaRec, 0.25f, 6, arenaBorder);

        const char* arenaText = "TEST ARENA - 5 STAGES";
        int arenaSize = 16;
        int arenaW = FontManager::MeasureText(arenaText, arenaSize);
        FontManager::DrawText(arenaText, (int)(center.x - arenaW * 0.5f), (int)(arenaBtnY + 15), arenaSize, arenaHovered ? Color{ 255, 255, 255, 255 } : Color{ 235, 205, 255, 240 });
    } else {
        DrawRectangleRounded(arenaRec, 0.25f, 6, Color{ 18, 20, 28, 160 });
        DrawRectangleRoundedLines(arenaRec, 0.25f, 6, Color{ 45, 52, 68, 160 });

        const char* arenaText = "TEST ARENA [LOCKED]";
        int arenaSize = 15;
        int arenaW = FontManager::MeasureText(arenaText, arenaSize);
        FontManager::DrawText(arenaText, (int)(center.x - arenaW * 0.5f), (int)(arenaBtnY + 16), arenaSize, Color{ 110, 120, 140, 180 });

        if (arenaHovered) {
            const char* lockMsg = "Duel Zil's Fragment in Chapter 1 to unlock the 5-Stage Combat Arena";
            int lmW = FontManager::MeasureText(lockMsg, 12);
            FontManager::DrawText(lockMsg, (int)(center.x - lmW * 0.5f), (int)(windowHeight - 72), 12, Color{ 255, 180, 70, 240 });
        }
    }

    // 3. CONTROLS BUTTON
    float ctrlBtnY = arenaBtnY + btnHeight + btnGap;
    Rectangle ctrlRec = { btnX, ctrlBtnY, btnWidth, btnHeight };
    bool ctrlHovered = CheckCollisionPointRec(mousePos, ctrlRec);

    Color ctrlBg = ctrlHovered ? Color{ 40, 55, 80, 255 } : Color{ 20, 28, 42, 230 };
    Color ctrlBorder = ctrlHovered ? Color{ 255, 215, 80, 255 } : Color{ 50, 70, 95, 255 };
    DrawRectangleRounded(ctrlRec, 0.25f, 6, ctrlBg);
    DrawRectangleRoundedLines(ctrlRec, 0.25f, 6, ctrlBorder);

    const char* ctrlText = "CONTROLS";
    int ctrlSize = 18;
    int ctrlW = FontManager::MeasureText(ctrlText, ctrlSize);
    FontManager::DrawText(ctrlText, (int)(center.x - ctrlW * 0.5f), (int)(ctrlBtnY + 15), ctrlSize, ctrlHovered ? Color{ 255, 230, 100, 255 } : Color{ 180, 205, 235, 230 });

    // Quick Hint at bottom
    const char* hint = isArenaUnlocked
        ? "Select [PLAY CAMPAIGN] for Story Chapter 1  |  [TEST ARENA] for the 5-Stage Arena"
        : "Press [PLAY CAMPAIGN] to begin Chapter 1: The Ruined Castle";
    int hintW = FontManager::MeasureText(hint, 13);
    FontManager::DrawText(hint, (int)(center.x - hintW * 0.5f), (int)(windowHeight - 45), 13, Color{ 140, 165, 195, 200 });

    if (isMousePressed) {
        if (playHovered) {
            SoundFX::PlayButtonClick();
            return 1; // Play
        }
        if (isArenaUnlocked && arenaHovered) {
            SoundFX::PlayEquipWeapon();
            return 3; // Test Arena
        }
        if (ctrlHovered) {
            SoundFX::PlayButtonClick();
            return 2; // Controls
        }
    }
    return 0;
}
