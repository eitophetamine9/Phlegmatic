#include "TitleUI.hpp"
#include "../core/SoundFX.hpp"
#include "../core/SaveManager.hpp"
#include "../rendering/FontManager.hpp"
#include <cmath>

int TitleUI::Draw(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool isArenaUnlocked, bool& showSlotModal, int& deletingSlotIndex) {
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

    // 1. PLAY CAMPAIGN BUTTON
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

    // 2. TEST ARENA BUTTON
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

        if (arenaHovered && !showSlotModal) {
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
    if (!showSlotModal) {
        const char* hint = isArenaUnlocked
            ? "Select [PLAY CAMPAIGN] to choose a Save Record  |  [TEST ARENA] for Combat Waves"
            : "Select [PLAY CAMPAIGN] to begin or continue your Pilgrimage";
        int hintW = FontManager::MeasureText(hint, 13);
        FontManager::DrawText(hint, (int)(center.x - hintW * 0.5f), (int)(windowHeight - 45), 13, Color{ 140, 165, 195, 200 });

        if (isMousePressed) {
            if (playHovered) {
                SoundFX::PlayButtonClick();
                showSlotModal = true;
                deletingSlotIndex = 0;
                return 0;
            }
            if (isArenaUnlocked && arenaHovered) {
                SoundFX::PlayEquipWeapon();
                return 2; // Test Arena
            }
            if (ctrlHovered) {
                SoundFX::PlayButtonClick();
                return 1; // Controls
            }
        }
        return 0;
    }

    // ==========================================
    // 3-SLOT SAVE FILE SELECTION MODAL
    // ==========================================
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 6, 8, 14, 235 });

    // Header
    FontManager::DrawTextTitleCentered("— SELECT PILGRIMAGE RECORD —", windowWidth / 2, 42, 28, Color{ 255, 215, 80, 255 });
    FontManager::DrawTextCentered("Choose a sacred chronicle to continue your pilgrimage or begin anew", windowWidth / 2, 78, 14, Color{ 170, 195, 225, 210 });

    const float totalCardsWidth = 1040.0f;
    const float cardW = 320.0f;
    const float cardH = 440.0f;
    const float cardGap = 40.0f;
    const float startX = ((float)windowWidth - totalCardsWidth) * 0.5f;
    const float cardY = 115.0f;

    const char* slotNames[] = { "RECORD I", "RECORD II", "RECORD III" };

    for (int slot = 1; slot <= 3; ++slot) {
        float cardX = startX + (float)(slot - 1) * (cardW + cardGap);
        Rectangle cardRec = { cardX, cardY, cardW, cardH };

        SaveSlotSummary summary;
        bool hasData = SaveManager::LoadSummary(slot, summary);

        Color cardBg = hasData ? Color{ 16, 20, 30, 245 } : Color{ 12, 14, 22, 220 };
        Color cardBorder = hasData ? Color{ 70, 105, 155, 220 } : Color{ 40, 52, 72, 160 };

        DrawRectangleRounded(cardRec, 0.04f, 6, cardBg);
        DrawRectangleRoundedLines(cardRec, 0.04f, 6, cardBorder);
        DrawRectangleRoundedLines(Rectangle{ cardX + 4.0f, cardY + 4.0f, cardW - 8.0f, cardH - 8.0f }, 0.035f, 6, Color{ 25, 35, 52, 140 });

        // Ornate Corner Accent Lines
        Color goldCorner = Color{ 255, 215, 80, 180 };
        DrawLineEx({ cardX + 10, cardY + 10 }, { cardX + 28, cardY + 10 }, 1.5f, goldCorner);
        DrawLineEx({ cardX + 10, cardY + 10 }, { cardX + 10, cardY + 28 }, 1.5f, goldCorner);
        DrawLineEx({ cardX + cardW - 10, cardY + 10 }, { cardX + cardW - 28, cardY + 10 }, 1.5f, goldCorner);
        DrawLineEx({ cardX + cardW - 10, cardY + 10 }, { cardX + cardW - 10, cardY + 28 }, 1.5f, goldCorner);

        // Header Title
        FontManager::DrawTextTitleCentered(slotNames[slot - 1], (int)(cardX + cardW * 0.5f), (int)cardY + 18, 18, Color{ 255, 215, 80, 255 });
        DrawLine((int)cardX + 24, (int)cardY + 46, (int)(cardX + cardW - 24), (int)cardY + 46, Color{ 48, 65, 92, 180 });

        if (hasData) {
            // Populated Slot Content
            // 1. Realm Name
            FontManager::DrawTextCentered(summary.realmName, (int)(cardX + cardW * 0.5f), (int)cardY + 58, 14, Color{ 120, 210, 255, 255 }, GameFont::TITLE);

            // 2. Health Bar & Stats
            float hpRatio = fmaxf(0.0f, fminf(summary.playerHealth / fmaxf(summary.playerMaxHealth, 1.0f), 1.0f));
            DrawRectangle((int)cardX + 24, (int)cardY + 88, (int)cardW - 48, 8, Color{ 32, 38, 52, 255 });
            DrawRectangle((int)cardX + 24, (int)cardY + 88, (int)((cardW - 48) * hpRatio), 8, Color{ 220, 60, 70, 255 });
            DrawRectangleLines((int)cardX + 24, (int)cardY + 88, (int)cardW - 48, 8, Color{ 60, 75, 105, 200 });

            std::string hpStr = "Vitality: " + std::to_string((int)summary.playerHealth) + " / " + std::to_string((int)summary.playerMaxHealth);
            FontManager::DrawText(hpStr, (int)cardX + 24, (int)cardY + 104, 12, Color{ 230, 205, 210, 220 });

            // 3. Equipped Blade
            std::string wName = "Unarmed";
            if (summary.equippedWeapon == 1) wName = "Crusader's Broadsword";
            else if (summary.equippedWeapon == 2) wName = "Dual Daggers";
            else if (summary.equippedWeapon == 3) wName = "War Mace";
            else if (summary.equippedWeapon == 4) wName = "Truth Seeker";
            FontManager::DrawText("Blade: " + wName, (int)cardX + 24, (int)cardY + 132, 12, Color{ 200, 220, 245, 220 });

            // 4. Shingles Currency
            FontManager::DrawText("Shingles: " + std::to_string(summary.shingles), (int)cardX + 24, (int)cardY + 158, 12, Color{ 255, 225, 110, 240 });

            // 5. Potions
            FontManager::DrawText("Elixirs: " + std::to_string(summary.potionCount) + " Flasks", (int)cardX + 24, (int)cardY + 184, 12, Color{ 120, 240, 185, 220 });

            // 6. Lore Fragments
            FontManager::DrawText("Codex: " + std::to_string(summary.discoveredFragments) + " / 10 Fragments", (int)cardX + 24, (int)cardY + 210, 12, Color{ 140, 195, 255, 220 });

            // 7. Saved Timestamp
            FontManager::DrawText("Recorded: " + summary.dateString, (int)cardX + 24, (int)cardY + 238, 11, Color{ 130, 155, 185, 200 });

            // 8. Action Buttons
            if (deletingSlotIndex == slot) {
                // Delete Confirmation Prompt
                DrawRectangleRounded(Rectangle{ cardX + 16.0f, cardY + 280.0f, cardW - 32.0f, 96.0f }, 0.15f, 4, Color{ 40, 14, 18, 250 });
                DrawRectangleRoundedLines(Rectangle{ cardX + 16.0f, cardY + 280.0f, cardW - 32.0f, 96.0f }, 0.15f, 4, Color{ 235, 70, 80, 240 });

                FontManager::DrawTextCentered("PERMANENTLY DELETE?", (int)(cardX + cardW * 0.5f), (int)cardY + 292, 12, Color{ 255, 140, 150, 255 }, GameFont::TITLE);

                // Confirm YES button
                Rectangle yesRec = { cardX + 28.0f, cardY + 328.0f, 120.0f, 34.0f };
                bool yesHov = CheckCollisionPointRec(mousePos, yesRec);
                DrawRectangleRounded(yesRec, 0.2f, 4, yesHov ? Color{ 180, 35, 45, 255 } : Color{ 110, 25, 35, 240 });
                DrawRectangleRoundedLines(yesRec, 0.2f, 4, Color{ 255, 100, 110, 255 });
                int yesW = FontManager::MeasureText("YES [DELETE]", 11);
                FontManager::DrawText("YES [DELETE]", (int)(yesRec.x + (yesRec.width - yesW) * 0.5f), (int)yesRec.y + 10, 11, WHITE);

                // Cancel NO button
                Rectangle noRec = { cardX + cardW - 148.0f, cardY + 328.0f, 120.0f, 34.0f };
                bool noHov = CheckCollisionPointRec(mousePos, noRec);
                DrawRectangleRounded(noRec, 0.2f, 4, noHov ? Color{ 55, 75, 115, 255 } : Color{ 35, 48, 72, 240 });
                DrawRectangleRoundedLines(noRec, 0.2f, 4, Color{ 140, 195, 255, 220 });
                int noW = FontManager::MeasureText("CANCEL", 11);
                FontManager::DrawText("CANCEL", (int)(noRec.x + (noRec.width - noW) * 0.5f), (int)noRec.y + 10, 11, WHITE);

                if (isMousePressed) {
                    if (yesHov) {
                        SaveManager::DeleteSave(slot);
                        deletingSlotIndex = 0;
                        SoundFX::PlayButtonClick();
                    } else if (noHov) {
                        deletingSlotIndex = 0;
                        SoundFX::PlayButtonClick();
                    }
                }
            } else {
                // Primary CONTINUE button
                Rectangle contRec = { cardX + 24.0f, cardY + 288.0f, cardW - 48.0f, 44.0f };
                bool contHov = CheckCollisionPointRec(mousePos, contRec);
                Color contBg = contHov ? Color{ 45, 80, 140, 255 } : Color{ 26, 46, 82, 240 };
                Color contBorder = contHov ? Color{ 120, 220, 255, 255 } : Color{ 70, 115, 185, 220 };
                DrawRectangleRounded(contRec, 0.22f, 4, contBg);
                DrawRectangleRoundedLines(contRec, 0.22f, 4, contBorder);

                const char* contTxt = "CONTINUE PILGRIMAGE";
                int cW = FontManager::MeasureText(contTxt, 13, GameFont::TITLE);
                FontManager::DrawText(contTxt, (int)(contRec.x + (contRec.width - cW) * 0.5f), (int)contRec.y + 14, 13, contHov ? WHITE : Color{ 220, 240, 255, 240 }, GameFont::TITLE);

                // Danger DELETE button
                Rectangle delRec = { cardX + 24.0f, cardY + 348.0f, cardW - 48.0f, 32.0f };
                bool delHov = CheckCollisionPointRec(mousePos, delRec);
                Color delBg = delHov ? Color{ 80, 25, 32, 240 } : Color{ 36, 18, 24, 210 };
                Color delBorder = delHov ? Color{ 220, 75, 85, 240 } : Color{ 110, 42, 50, 180 };
                DrawRectangleRounded(delRec, 0.2f, 4, delBg);
                DrawRectangleRoundedLines(delRec, 0.2f, 4, delBorder);

                const char* delTxt = "DELETE RECORD";
                int dW = FontManager::MeasureText(delTxt, 11);
                FontManager::DrawText(delTxt, (int)(delRec.x + (delRec.width - dW) * 0.5f), (int)delRec.y + 9, 11, delHov ? Color{ 255, 180, 190, 255 } : Color{ 180, 110, 120, 220 });

                if (isMousePressed) {
                    if (contHov) {
                        SoundFX::PlayButtonClick();
                        return 10 + slot; // Load Slot 1, 2, or 3
                    }
                    if (delHov) {
                        SoundFX::PlayButtonClick();
                        deletingSlotIndex = slot;
                    }
                }
            }
        } else {
            // Empty Slot Content
            DrawCircleLines((int)(cardX + cardW * 0.5f), (int)cardY + 140, 36.0f, Color{ 48, 62, 85, 160 });
            DrawLine((int)(cardX + cardW * 0.5f) - 20, (int)cardY + 140, (int)(cardX + cardW * 0.5f) + 20, (int)cardY + 140, Color{ 55, 75, 105, 180 });
            DrawLine((int)(cardX + cardW * 0.5f), (int)cardY + 120, (int)(cardX + cardW * 0.5f), (int)cardY + 160, Color{ 55, 75, 105, 180 });

            FontManager::DrawTextCentered("[EMPTY RECORD]", (int)(cardX + cardW * 0.5f), (int)cardY + 200, 14, Color{ 130, 150, 180, 200 }, GameFont::TITLE);
            FontManager::DrawTextWrapped("No traveler has yet bound their soul to this chronicle.", (int)cardX + 32, (int)cardY + 230, (int)cardW - 64, 12, 6, Color{ 110, 130, 160, 180 }, GameFont::BODY);

            // Primary BEGIN PILGRIMAGE Button
            Rectangle newRec = { cardX + 24.0f, cardY + 310.0f, cardW - 48.0f, 46.0f };
            bool newHov = CheckCollisionPointRec(mousePos, newRec);
            Color newBg = newHov ? Color{ 35, 75, 60, 255 } : Color{ 20, 48, 38, 230 };
            Color newBorder = newHov ? Color{ 100, 240, 180, 255 } : Color{ 55, 145, 105, 220 };
            DrawRectangleRounded(newRec, 0.22f, 4, newBg);
            DrawRectangleRoundedLines(newRec, 0.22f, 4, newBorder);

            const char* newTxt = "BEGIN PILGRIMAGE";
            int nW = FontManager::MeasureText(newTxt, 13, GameFont::TITLE);
            FontManager::DrawText(newTxt, (int)(newRec.x + (newRec.width - nW) * 0.5f), (int)newRec.y + 15, 13, newHov ? WHITE : Color{ 180, 245, 215, 240 }, GameFont::TITLE);

            if (isMousePressed && newHov) {
                SoundFX::PlayButtonClick();
                return 20 + slot; // New Game Slot 1, 2, or 3
            }
        }
    }

    // Return / Back to Title Button
    Rectangle backRec = { (float)windowWidth * 0.5f - 140.0f, (float)windowHeight - 68.0f, 280.0f, 40.0f };
    bool backHov = CheckCollisionPointRec(mousePos, backRec);
    Color backBg = backHov ? Color{ 45, 58, 82, 240 } : Color{ 22, 28, 42, 210 };
    Color backBorder = backHov ? Color{ 140, 195, 255, 255 } : Color{ 60, 80, 115, 200 };
    DrawRectangleRounded(backRec, 0.25f, 4, backBg);
    DrawRectangleRoundedLines(backRec, 0.25f, 4, backBorder);

    const char* backTxt = "< RETURN TO TITLE MENU [ESC]";
    int bW = FontManager::MeasureText(backTxt, 12);
    FontManager::DrawText(backTxt, (int)(backRec.x + (backRec.width - bW) * 0.5f), (int)backRec.y + 13, 12, backHov ? WHITE : Color{ 190, 215, 240, 220 });

    if ((isMousePressed && backHov) || IsKeyPressed(KEY_ESCAPE)) {
        SoundFX::PlayButtonClick();
        showSlotModal = false;
        deletingSlotIndex = 0;
    }

    return 0;
}
