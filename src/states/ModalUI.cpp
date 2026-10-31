#include "ModalUI.hpp"

#include "../core/SoundFX.hpp"

#include "../combat/Weapon.hpp"

#include "../combat/Armor.hpp"

#include "../combat/Totem.hpp"

#include "../rendering/AssetManager.hpp"

#include "../rendering/FontManager.hpp"

#include "../entities/Player.hpp"

#include <cmath>

#include <sstream>



static void DrawJournalWrappedText(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color) {

    FontManager::DrawTextWrapped(text, posX, posY, maxLineWidth, fontSize, lineSpacing, color, GameFont::BODY);

}



bool ModalUI::DrawControls(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed) {
    static int s_settingsTab = 0; // 0: Controls & Combat, 1: Audio Mixer
    static bool s_isDraggingMaster = false;
    static bool s_isDraggingSFX = false;
    static bool s_isDraggingAmbience = false;

    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        s_isDraggingMaster = false;
        s_isDraggingSFX = false;
        s_isDraggingAmbience = false;
    }

    // Dim background
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 10, 16, 225 });

    const float modalW = 780.0f;
    const float modalH = 580.0f;
    float modalX = ((float)windowWidth - modalW) * 0.5f;
    float modalY = ((float)windowHeight - modalH) * 0.5f;

    DrawRectangleRounded(Rectangle{ modalX, modalY, modalW, modalH }, 0.08f, 8, Color{ 16, 20, 30, 252 });
    DrawRectangleRoundedLines(Rectangle{ modalX, modalY, modalW, modalH }, 0.08f, 8, Color{ 60, 80, 115, 255 });

    // Header Tabs: [CONTROLS & COMBAT] | [AUDIO MIXER]
    Rectangle tabCtrlRec = { modalX + 32.0f, modalY + 20.0f, 210.0f, 28.0f };
    Rectangle tabAudioRec = { modalX + 252.0f, modalY + 20.0f, 190.0f, 28.0f };
    bool isCtrlHov = CheckCollisionPointRec(mousePos, tabCtrlRec);
    bool isAudioHov = CheckCollisionPointRec(mousePos, tabAudioRec);

    if (isCtrlHov && isMousePressed) { s_settingsTab = 0; SoundFX::PlayButtonClick(); }
    if (isAudioHov && isMousePressed) { s_settingsTab = 1; SoundFX::PlayButtonClick(); }

    DrawRectangleRounded(tabCtrlRec, 0.25f, 4, (s_settingsTab == 0) ? Color{ 40, 56, 85, 255 } : (isCtrlHov ? Color{ 26, 36, 54, 220 } : Color{ 18, 24, 36, 180 }));
    DrawRectangleRoundedLines(tabCtrlRec, 0.25f, 4, (s_settingsTab == 0) ? Color{ 255, 215, 80, 240 } : Color{ 60, 80, 110, 200 });
    DrawText("CONTROLS & COMBAT", (int)tabCtrlRec.x + 18, (int)tabCtrlRec.y + 8, 12, (s_settingsTab == 0) ? Color{ 255, 225, 95, 255 } : Color{ 180, 200, 225, 220 });

    DrawRectangleRounded(tabAudioRec, 0.25f, 4, (s_settingsTab == 1) ? Color{ 40, 56, 85, 255 } : (isAudioHov ? Color{ 26, 36, 54, 220 } : Color{ 18, 24, 36, 180 }));
    DrawRectangleRoundedLines(tabAudioRec, 0.25f, 4, (s_settingsTab == 1) ? Color{ 255, 215, 80, 240 } : Color{ 60, 80, 110, 200 });
    DrawText("AUDIO MIXER", (int)tabAudioRec.x + 36, (int)tabAudioRec.y + 8, 12, (s_settingsTab == 1) ? Color{ 255, 225, 95, 255 } : Color{ 180, 200, 225, 220 });

    DrawLine((int)modalX + 32, (int)modalY + 60, (int)(modalX + modalW - 32), (int)modalY + 60, Color{ 40, 52, 75, 255 });

    float startY = modalY + 76.0f;

    if (s_settingsTab == 0) {
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
            { "Q Key",         "DASH",     "High-velocity directional dash with i-frames invulnerability. Evade attacks!", Color{ 120, 240, 180, 255 } },
            { "F Key",         "PARRY",    "Active 0.28s guard. Timely parries stagger foes & grant +50% counter dmg buff!", Color{ 255, 230, 80, 255 } },
            { "E Key",         "Interact", "Equip weapon at pedestals or enter Arena Portal.", Color{ 200, 150, 255, 255 } },
            { "C / TAB Key",   "Controls", "Toggle this Controls Menu modal at any time.", Color{ 160, 180, 200, 255 } },
            { "R / L / ESC",   "Menu Keys","[R] Respawn/Reset  |  [L] Return to Lobby  |  [ESC] Title Screen.", Color{ 140, 160, 180, 255 } }
        };

        float rowHeight = 38.0f;
        for (int i = 0; i < 9; i++) {
            float rowY = startY + (float)i * rowHeight;

            if (i % 2 == 1) {
                DrawRectangle((int)modalX + 30, (int)rowY, (int)(modalW - 60), (int)rowHeight - 4, Color{ 22, 28, 42, 140 });
            }

            float badgeW = 120.0f;
            DrawRectangleRounded(Rectangle{ modalX + 36, rowY + 3, badgeW, 24 }, 0.25f, 4, Color{ 30, 38, 54, 255 });
            DrawRectangleRoundedLines(Rectangle{ modalX + 36, rowY + 3, badgeW, 24 }, 0.25f, 4, items[i].badgeColor);
            int kw = MeasureText(items[i].key, 11);
            DrawText(items[i].key, (int)(modalX + 36 + (badgeW - kw) * 0.5f), (int)rowY + 9, 11, items[i].badgeColor);

            DrawText(items[i].action, (int)modalX + 175, (int)rowY + 9, 12, Color{ 255, 255, 255, 255 });
            DrawText(items[i].details, (int)modalX + 265, (int)rowY + 9, 11, Color{ 185, 205, 230, 220 });
        }
    } else {
        // AUDIO MIXER PANEL
        DrawText("PROCEDURAL ACOUSTIC MIXER & GAIN BALANCING", (int)modalX + 36, (int)startY + 6, 16, Color{ 255, 215, 80, 255 });
        DrawText("Balance master output gain, weapon clash effects, and environmental acoustic loops.", (int)modalX + 36, (int)startY + 28, 11, Color{ 175, 195, 225, 220 });

        struct VolumeControl {
            const char* title;
            const char* desc;
            float val;
            Color barCol;
        };

        VolumeControl vcs[3] = {
            { "MASTER GAIN", "Overall engine audio level across all synthesized buffers and UI.", SoundFX::GetMasterVolume(), Color{ 100, 220, 255, 255 } },
            { "SOUND EFFECTS (SFX)", "Melee blade slashes, shield deflections, parry rings, and blunt impacts.", SoundFX::GetSFXVolume(), Color{ 255, 180, 60, 255 } },
            { "ENVIRONMENTAL AMBIENCE", "Procedural whistling winds, hearth crackle, and subterranean echoes.", SoundFX::GetAmbienceVolume(), Color{ 120, 240, 170, 255 } }
        };

        float sliderW = modalW - 200.0f;
        float sliderH = 14.0f;

        for (int i = 0; i < 3; i++) {
            float groupY = startY + 68.0f + (float)i * 88.0f;

            // Title & Value text
            DrawText(vcs[i].title, (int)modalX + 40, (int)groupY, 13, Color{ 255, 255, 255, 255 });
            int pct = (int)(vcs[i].val * 100.0f + 0.5f);
            std::string pctStr = std::to_string(pct) + "%";
            DrawText(pctStr.c_str(), (int)(modalX + 40.0f + sliderW + 24.0f), (int)groupY + 20, 14, vcs[i].barCol);

            DrawText(vcs[i].desc, (int)modalX + 40, (int)groupY + 18, 10, Color{ 160, 180, 205, 200 });

            // Slider track
            Rectangle trackRec = { modalX + 40.0f, groupY + 36.0f, sliderW, sliderH };
            bool isTrackHov = CheckCollisionPointRec(mousePos, Rectangle{ trackRec.x, trackRec.y - 6.0f, trackRec.width, trackRec.height + 12.0f });

            if (isTrackHov && isMousePressed) {
                if (i == 0) s_isDraggingMaster = true;
                if (i == 1) s_isDraggingSFX = true;
                if (i == 2) s_isDraggingAmbience = true;
            }

            if ((i == 0 && s_isDraggingMaster) || (i == 1 && s_isDraggingSFX) || (i == 2 && s_isDraggingAmbience)) {
                float ratio = (mousePos.x - trackRec.x) / trackRec.width;
                if (ratio < 0.0f) ratio = 0.0f;
                if (ratio > 1.0f) ratio = 1.0f;
                if (i == 0) SoundFX::SetMasterVolume(ratio);
                else if (i == 1) SoundFX::SetSFXVolume(ratio);
                else if (i == 2) SoundFX::SetAmbienceVolume(ratio);
            }

            // Track background
            DrawRectangleRounded(trackRec, 0.5f, 4, Color{ 24, 32, 48, 255 });
            DrawRectangleRoundedLines(trackRec, 0.5f, 4, Color{ 55, 75, 110, 220 });

            // Fill bar
            float fillW = sliderW * vcs[i].val;
            if (fillW > 4.0f) {
                Rectangle fillRec = { trackRec.x, trackRec.y, fillW, sliderH };
                DrawRectangleRounded(fillRec, 0.5f, 4, vcs[i].barCol);
            }

            // Handle Knob
            Vector2 knobPos = { trackRec.x + fillW, trackRec.y + sliderH * 0.5f };
            DrawCircleV(knobPos, 9.0f, Color{ 255, 255, 255, 255 });
            DrawCircleLines((int)knobPos.x, (int)knobPos.y, 9.0f, Color{ 20, 26, 36, 255 });
        }

        // Test audio button
        Rectangle testBtnRec = { modalX + 40.0f, startY + 342.0f, 220.0f, 32.0f };
        bool isTestHov = CheckCollisionPointRec(mousePos, testBtnRec);
        DrawRectangleRounded(testBtnRec, 0.25f, 4, isTestHov ? Color{ 45, 68, 105, 255 } : Color{ 28, 38, 58, 240 });
        DrawRectangleRoundedLines(testBtnRec, 0.25f, 4, isTestHov ? Color{ 255, 215, 80, 255 } : Color{ 80, 120, 180, 220 });
        DrawText("TEST COMBAT SOUND", (int)testBtnRec.x + 24, (int)testBtnRec.y + 10, 11, isTestHov ? Color{ 255, 255, 255, 255 } : Color{ 200, 225, 255, 220 });

        if (isTestHov && isMousePressed) {
            SoundFX::PlayCrusaderClash();
        }
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



    if ((isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE)) {

        SoundFX::PlayButtonClick();

        return true; // Close requested

    }

    return false;

}




// ==========================================
bool ModalUI::DrawInventory(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex) {

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



    // Tab 3: Realm Map [M]

    Rectangle tab3Rec = { modalX + 496, tabY, 190.0f, 34.0f };

    bool isTab3Hov = CheckCollisionPointRec(mousePos, tab3Rec);

    if ((isTab3Hov && isMousePressed) || IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_M)) {

        activeTab = 2;

        SoundFX::PlayButtonClick();

        return true; // Signals closing inventory and opening map

    }



    DrawRectangleRounded(tab3Rec, 0.2f, 4, isTab3Hov ? Color{ 24, 30, 46, 220 } : Color{ 18, 22, 34, 180 });

    DrawText("3. REALM MAP [M]", (int)tab3Rec.x + 16, (int)tab3Rec.y + 10, 13, isTab3Hov ? Color{ 240, 245, 255, 255 } : Color{ 160, 175, 195, 200 });



    // TAB CONTENT AREA

    float contentY = modalY + 125.0f;



    if (activeTab == 0) {
        // =========================================================================
        // TAB 0: EQUIPPED SILHOUETTE HUD & INTERACTIVE ARSENAL DRAWER (v0.0.1e)
        // =========================================================================
        static int s_selectedSlot = 1; // 0 = HELMET, 1 = CHESTPLATE, 2 = LEGGINGS, 3 = TOTEM, 4 = WEAPONS
        static bool s_apparelHidden = false;

        float leftX = modalX + 24.0f;
        float leftW = 460.0f;
        float leftH = 500.0f;

        // Container backdrop for Left Column
        DrawRectangleRounded(Rectangle{ leftX, contentY - 8.0f, leftW, leftH }, 0.04f, 6, Color{ 14, 18, 26, 230 });
        DrawRectangleRoundedLines(Rectangle{ leftX, contentY - 8.0f, leftW, leftH }, 0.04f, 6, Color{ 45, 60, 85, 200 });

        // Header: EQUIPPED
        DrawText("EQUIPPED", (int)leftX + 16, (int)contentY + 4, 16, Color{ 255, 235, 175, 255 });

        // Apparel Hidden Toggle Pill
        Rectangle apparelPill = { leftX + leftW - 144.0f, contentY + 2.0f, 130.0f, 22.0f };
        bool isApparelHov = CheckCollisionPointRec(mousePos, apparelPill);
        if (isApparelHov && isMousePressed) {
            s_apparelHidden = !s_apparelHidden;
            SoundFX::PlayEquipWeapon();
        }
        DrawRectangleRounded(apparelPill, 0.3f, 4, s_apparelHidden ? Color{ 55, 75, 110, 240 } : (isApparelHov ? Color{ 35, 45, 65, 220 } : Color{ 22, 28, 42, 200 }));
        DrawRectangleRoundedLines(apparelPill, 0.3f, 4, s_apparelHidden ? Color{ 120, 200, 255, 255 } : Color{ 60, 80, 110, 200 });
        DrawText(s_apparelHidden ? "APPAREL HIDDEN" : "APPAREL VISIBLE", (int)apparelPill.x + 12, (int)apparelPill.y + 5, 10, s_apparelHidden ? Color{ 160, 225, 255, 255 } : Color{ 160, 175, 195, 220 });

        // Horizontal divider under EQUIPPED
        DrawLine((int)leftX + 16, (int)contentY + 30, (int)(leftX + leftW - 16), (int)contentY + 30, Color{ 50, 68, 96, 200 });

        // Center silhouette doll
        Vector2 dollCenter = { leftX + leftW * 0.5f, contentY + 115.0f };
        HelmetType drawHelm = s_apparelHidden ? HelmetType::NONE : player.GetHelmet();
        ChestplateType drawChest = s_apparelHidden ? ChestplateType::NONE : player.GetChestplate();
        LeggingsType drawLegs = s_apparelHidden ? LeggingsType::NONE : player.GetLeggings();
        AssetManager::DrawCharacterDoll(dollCenter, 0.92f, player.GetWeapon(), drawHelm, drawChest, drawLegs, player.GetTotem(), (float)GetTime());

        // TOP SLOT: [HELMET]
        Rectangle helmRec = { dollCenter.x - 70.0f, contentY + 38.0f, 140.0f, 42.0f };
        bool isHelmHov = CheckCollisionPointRec(mousePos, helmRec);
        if (isHelmHov && isMousePressed) {
            s_selectedSlot = 0;
            SoundFX::PlayEquipWeapon();
        }
        HelmetStats hStats = GetHelmetStats(player.GetHelmet());
        DrawRectangleRounded(helmRec, 0.15f, 4, (s_selectedSlot == 0) ? Color{ 32, 48, 75, 240 } : (isHelmHov ? Color{ 26, 36, 56, 220 } : Color{ 18, 24, 38, 200 }));
        DrawRectangleRoundedLines(helmRec, 0.15f, 4, (s_selectedSlot == 0) ? Color{ 255, 215, 80, 255 } : (isHelmHov ? Color{ 100, 160, 240, 220 } : Color{ 48, 64, 92, 200 }));
        DrawText("HEAD / HELMET", (int)helmRec.x + 8, (int)helmRec.y + 5, 9, Color{ 140, 160, 185, 220 });
        DrawText(hStats.name.c_str(), (int)helmRec.x + 8, (int)helmRec.y + 18, 11, (s_selectedSlot == 0) ? Color{ 255, 225, 100, 255 } : Color{ 220, 235, 255, 255 });
        std::string hStatMini = "Def +" + std::to_string((int)hStats.defense) + " | Will +" + std::to_string((int)hStats.willBonus);
        DrawText(hStatMini.c_str(), (int)helmRec.x + 8, (int)helmRec.y + 30, 9, Color{ 100, 240, 180, 230 });

        // MID-LEFT SLOT: [CHEST]
        Rectangle chestRec = { leftX + 14.0f, dollCenter.y - 25.0f, 126.0f, 46.0f };
        bool isChestHov = CheckCollisionPointRec(mousePos, chestRec);
        if (isChestHov && isMousePressed) {
            s_selectedSlot = 1;
            SoundFX::PlayEquipWeapon();
        }
        ChestplateStats cStats = GetChestplateStats(player.GetChestplate());
        DrawRectangleRounded(chestRec, 0.15f, 4, (s_selectedSlot == 1) ? Color{ 32, 48, 75, 240 } : (isChestHov ? Color{ 26, 36, 56, 220 } : Color{ 18, 24, 38, 200 }));
        DrawRectangleRoundedLines(chestRec, 0.15f, 4, (s_selectedSlot == 1) ? Color{ 255, 215, 80, 255 } : (isChestHov ? Color{ 100, 160, 240, 220 } : Color{ 48, 64, 92, 200 }));
        DrawText("CHEST / CUIRASS", (int)chestRec.x + 8, (int)chestRec.y + 5, 9, Color{ 140, 160, 185, 220 });
        DrawText(cStats.name.c_str(), (int)chestRec.x + 8, (int)chestRec.y + 19, 11, (s_selectedSlot == 1) ? Color{ 255, 225, 100, 255 } : Color{ 220, 235, 255, 255 });
        std::string cStatMini = "Def +" + std::to_string((int)cStats.defense) + (cStats.maxHpBonus > 0.0f ? (" | HP +" + std::to_string((int)cStats.maxHpBonus)) : "");
        DrawText(cStatMini.c_str(), (int)chestRec.x + 8, (int)chestRec.y + 32, 9, Color{ 100, 240, 180, 230 });

        // MID-RIGHT SLOT: [LEGS]
        Rectangle legsRec = { leftX + leftW - 140.0f, dollCenter.y - 25.0f, 126.0f, 46.0f };
        bool isLegsHov = CheckCollisionPointRec(mousePos, legsRec);
        if (isLegsHov && isMousePressed) {
            s_selectedSlot = 2;
            SoundFX::PlayEquipWeapon();
        }
        LeggingsStats lStats = GetLeggingsStats(player.GetLeggings());
        DrawRectangleRounded(legsRec, 0.15f, 4, (s_selectedSlot == 2) ? Color{ 32, 48, 75, 240 } : (isLegsHov ? Color{ 26, 36, 56, 220 } : Color{ 18, 24, 38, 200 }));
        DrawRectangleRoundedLines(legsRec, 0.15f, 4, (s_selectedSlot == 2) ? Color{ 255, 215, 80, 255 } : (isLegsHov ? Color{ 100, 160, 240, 220 } : Color{ 48, 64, 92, 200 }));
        DrawText("LEGS / GREAVES", (int)legsRec.x + 8, (int)legsRec.y + 5, 9, Color{ 140, 160, 185, 220 });
        DrawText(lStats.name.c_str(), (int)legsRec.x + 8, (int)legsRec.y + 19, 11, (s_selectedSlot == 2) ? Color{ 255, 225, 100, 255 } : Color{ 220, 235, 255, 255 });
        std::string lStatMini = "Def +" + std::to_string((int)lStats.defense) + " | Will +" + std::to_string((int)lStats.willBonus);
        DrawText(lStatMini.c_str(), (int)legsRec.x + 8, (int)legsRec.y + 32, 9, Color{ 100, 240, 180, 230 });

        // TOTEMS SECTION (Replacing Accessories - only 1 equipped)
        float totemSecY = contentY + 200.0f;
        DrawLine((int)leftX + 16, (int)totemSecY, (int)(leftX + leftW - 16), (int)totemSecY, Color{ 50, 68, 96, 200 });
        DrawText("TOTEMS", (int)leftX + 16, (int)totemSecY + 6, 12, Color{ 255, 215, 90, 255 });
        DrawText("(Only 1 equipped at a time)", (int)leftX + 80, (int)totemSecY + 7, 10, Color{ 140, 160, 185, 200 });

        Rectangle totemSlotRec = { leftX + 14.0f, totemSecY + 24.0f, leftW - 28.0f, 44.0f };
        bool isTotemSlotHov = CheckCollisionPointRec(mousePos, totemSlotRec);
        if (isTotemSlotHov && isMousePressed) {
            s_selectedSlot = 3;
            SoundFX::PlayEquipWeapon();
        }
        TotemStats tStats = GetTotemStats(player.GetTotem());
        bool hasTotem = (player.GetTotem() != TotemType::NONE);
        DrawRectangleRounded(totemSlotRec, 0.12f, 4, (s_selectedSlot == 3) ? Color{ 32, 48, 75, 240 } : (isTotemSlotHov ? Color{ 26, 36, 56, 220 } : Color{ 18, 24, 38, 200 }));
        DrawRectangleRoundedLines(totemSlotRec, 0.12f, 4, hasTotem ? tStats.glowColor : ((s_selectedSlot == 3) ? Color{ 255, 215, 80, 255 } : Color{ 48, 64, 92, 200 }));
        DrawText(hasTotem ? tStats.name.c_str() : "No Totem Attuned (Click to Select)", (int)totemSlotRec.x + 12, (int)totemSlotRec.y + 8, 12, hasTotem ? tStats.glowColor : Color{ 140, 155, 175, 220 });
        std::string totSummary = hasTotem ? ("[+] " + tStats.buffText + " | [-] " + tStats.nerfText) : "Attune a primal relic to empower your combat humors.";
        DrawText(totSummary.c_str(), (int)totemSlotRec.x + 12, (int)totemSlotRec.y + 26, 10, hasTotem ? Color{ 160, 230, 200, 230 } : Color{ 110, 125, 145, 200 });

        // WEAPONS SECTION
        float wepSecY = totemSecY + 74.0f;
        DrawLine((int)leftX + 16, (int)wepSecY, (int)(leftX + leftW - 16), (int)wepSecY, Color{ 50, 68, 96, 200 });
        DrawText("WEAPONS", (int)leftX + 16, (int)wepSecY + 6, 12, Color{ 255, 215, 90, 255 });

        struct WepBtnInfo { WeaponType type; const char* label; };
        WepBtnInfo wepBtns[5] = {
            { WeaponType::SWORD, "Sword" },
            { WeaponType::DAGGERS, "Daggers" },
            { WeaponType::MACE, "Mace" },
            { WeaponType::HALBERD, "Halberd" },
            { WeaponType::TRUTH_SEEKER, "Seeker" }
        };
        float btnW = (leftW - 32.0f - 16.0f) / 5.0f;
        for (int i = 0; i < 5; ++i) {
            Rectangle wRec = { leftX + 16.0f + (float)i * (btnW + 4.0f), wepSecY + 24.0f, btnW, 36.0f };
            bool isWUnlocked = player.GetInventory().HasWeapon(wepBtns[i].type);
            bool isWEquipped = (player.GetWeapon() == wepBtns[i].type);
            bool isWHov = CheckCollisionPointRec(mousePos, wRec);
            if (isWHov && isMousePressed) {
                if (isWUnlocked) {
                    player.SetWeapon(wepBtns[i].type);
                }
                s_selectedSlot = 4;
            }
            Color wBg = isWEquipped ? Color{ 45, 65, 100, 255 } : (isWHov && isWUnlocked ? Color{ 30, 42, 65, 220 } : Color{ 18, 22, 34, 200 });
            Color wBorder = isWEquipped ? Color{ 255, 215, 80, 255 } : (isWUnlocked ? (isWHov ? Color{ 100, 160, 240, 220 } : Color{ 50, 68, 96, 200 }) : Color{ 32, 38, 50, 160 });
            DrawRectangleRounded(wRec, 0.15f, 4, wBg);
            DrawRectangleRoundedLines(wRec, 0.15f, 4, wBorder);
            DrawText(wepBtns[i].label, (int)wRec.x + 8, (int)wRec.y + 11, 11, isWEquipped ? Color{ 255, 225, 100, 255 } : (isWUnlocked ? Color{ 220, 235, 255, 255 } : Color{ 90, 100, 115, 180 }));
        }

        // FULL ARMOR SET BONUS BANNER
        float bannerY = wepSecY + 70.0f;
        Rectangle bannerRec = { leftX + 14.0f, bannerY, leftW - 28.0f, 96.0f };
        ArmorSet curSet = player.GetActiveArmorSet();
        ArmorSetInfo setInfo = GetArmorSetInfo(curSet);
        bool hasFullSet = (curSet != ArmorSet::NONE);
        DrawRectangleRounded(bannerRec, 0.1f, 4, hasFullSet ? Color{ 28, 38, 58, 240 } : Color{ 18, 22, 34, 200 });
        DrawRectangleRoundedLines(bannerRec, 0.1f, 4, hasFullSet ? Color{ 255, 215, 80, 240 } : Color{ 48, 64, 90, 200 });

        if (hasFullSet) {
            std::string setTitle = "FULL SET BONUS: " + setInfo.name + " (3/3 ACTIVE)";
            DrawText(setTitle.c_str(), (int)bannerRec.x + 12, (int)bannerRec.y + 8, 12, Color{ 255, 225, 100, 255 });
            std::string passStr = "[Passive]: " + setInfo.passiveName + " - " + setInfo.passiveDescription;
            DrawText(passStr.c_str(), (int)bannerRec.x + 12, (int)bannerRec.y + 26, 10, Color{ 120, 245, 180, 240 });

            float cd = player.GetArmorSkillCooldown();
            std::string skillStr = "[Special Skill [V]]: " + setInfo.skillName + " (" + std::to_string((int)setInfo.skillCooldown) + "s CD)";
            if (cd > 0.0f) {
                skillStr += " [COOLDOWN: " + std::to_string((int)ceilf(cd)) + "s]";
            } else {
                skillStr += " [READY]";
            }
            DrawText(skillStr.c_str(), (int)bannerRec.x + 12, (int)bannerRec.y + 46, 10, (cd <= 0.0f) ? Color{ 255, 210, 80, 255 } : Color{ 240, 120, 80, 240 });
            DrawText(setInfo.skillDescription.c_str(), (int)bannerRec.x + 12, (int)bannerRec.y + 64, 9, Color{ 180, 205, 235, 220 });
        } else {
            DrawText("ARMOR SET STATUS: MISMATCHED PIECES", (int)bannerRec.x + 12, (int)bannerRec.y + 10, 12, Color{ 180, 195, 215, 220 });
            DrawText("Equip 3 matching set pieces (Helmet + Chest + Leggings) to awaken", (int)bannerRec.x + 12, (int)bannerRec.y + 32, 10, Color{ 140, 160, 185, 200 });
            DrawText("powerful passive set bonuses and the activated special skill via [V].", (int)bannerRec.x + 12, (int)bannerRec.y + 50, 10, Color{ 140, 160, 185, 200 });
            DrawText("Sets: Commoner's, Crusader, Vanguard, Shadowstrider, Scrap Iron, Basalt.", (int)bannerRec.x + 12, (int)bannerRec.y + 70, 9, Color{ 255, 205, 100, 220 });
        }

        // =========================================================================
        // RIGHT COLUMN: SELECTION & ARSENAL DRAWER (SLIDE-OUT INSPECTOR)
        // =========================================================================
        float rightX = modalX + 504.0f;
        float rightW = 548.0f;
        float rightH = 500.0f;

        DrawRectangleRounded(Rectangle{ rightX, contentY - 8.0f, rightW, rightH }, 0.04f, 6, Color{ 14, 18, 26, 230 });
        DrawRectangleRoundedLines(Rectangle{ rightX, contentY - 8.0f, rightW, rightH }, 0.04f, 6, Color{ 45, 60, 85, 200 });

        // Category tabs on right drawer
        const char* slotCatNames[5] = { "1. HELMET", "2. CHESTPLATE", "3. LEGGINGS", "4. TOTEM", "5. WEAPONS" };
        float tabW = (rightW - 32.0f - 16.0f) / 5.0f;
        for (int i = 0; i < 5; ++i) {
            Rectangle catRec = { rightX + 16.0f + (float)i * (tabW + 4.0f), contentY + 2.0f, tabW, 26.0f };
            bool isCatHov = CheckCollisionPointRec(mousePos, catRec);
            if (isCatHov && isMousePressed) {
                s_selectedSlot = i;
                SoundFX::PlayEquipWeapon();
            }
            bool isCatActive = (s_selectedSlot == i);
            DrawRectangleRounded(catRec, 0.2f, 4, isCatActive ? Color{ 40, 60, 95, 240 } : (isCatHov ? Color{ 28, 38, 58, 220 } : Color{ 18, 24, 36, 180 }));
            DrawRectangleRoundedLines(catRec, 0.2f, 4, isCatActive ? Color{ 255, 215, 80, 255 } : Color{ 48, 65, 92, 180 });
            DrawText(slotCatNames[i], (int)catRec.x + 6, (int)catRec.y + 7, 10, isCatActive ? Color{ 255, 225, 100, 255 } : Color{ 160, 180, 205, 200 });
        }

        DrawLine((int)rightX + 16, (int)contentY + 34, (int)(rightX + rightW - 16), (int)contentY + 34, Color{ 50, 68, 96, 200 });
        DrawText("CLICK ANY UNLOCKED PIECE TO EQUIP IMMEDIATELY", (int)rightX + 18, (int)contentY + 40, 10, Color{ 140, 165, 195, 220 });

        float drawerListY = contentY + 58.0f;

        if (s_selectedSlot == 0) {
            // HELMETS LIST
            struct HelmEntry { HelmetType type; const char* lockMsg; };
            HelmEntry helms[7] = {
                { HelmetType::NONE, "Unarmored default" },
                { HelmetType::COMMONERS_HOOD, "Ruined Castle - Awakening Pedestal" },
                { HelmetType::CRUSADER_HELM, "Zyphden - Defeat Knight Lenon in spar" },
                { HelmetType::VANGUARD_GREATHELM, "Lobby / Arsenal testing supplies" },
                { HelmetType::SHADOW_COWL, "Lobby / Arsenal testing supplies" },
                { HelmetType::SCRAP_SALLET, "Blacksmith Varek - Armor Forge" },
                { HelmetType::BASALT_GREATHELM, "Blacksmith Varek - Armor Forge" }
            };
            for (int i = 0; i < 7; ++i) {
                float iy = drawerListY + (float)i * 61.0f;
                Rectangle iRec = { rightX + 16.0f, iy, rightW - 32.0f, 57.0f };
                bool isUnlocked = player.GetInventory().HasHelmet(helms[i].type);
                bool isEquipped = (player.GetHelmet() == helms[i].type);
                bool isHov = CheckCollisionPointRec(mousePos, iRec);

                HelmetStats st = GetHelmetStats(helms[i].type);
                DrawRectangleRounded(iRec, 0.12f, 4, isEquipped ? Color{ 34, 50, 78, 255 } : (isHov && isUnlocked ? Color{ 26, 38, 58, 230 } : Color{ 16, 22, 34, 200 }));
                DrawRectangleRoundedLines(iRec, 0.12f, 4, isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? (isHov ? Color{ 100, 160, 240, 220 } : Color{ 48, 65, 95, 200 }) : Color{ 35, 42, 55, 160 }));

                DrawText(st.name.c_str(), (int)iRec.x + 12, (int)iy + 6, 12, isUnlocked ? Color{ 235, 245, 255, 255 } : Color{ 120, 130, 145, 200 });
                if (isEquipped) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 78.0f, iy + 6.0f, 68.0f, 18.0f }, 0.25f, 4, Color{ 255, 215, 80, 230 });
                    DrawText("EQUIPPED", (int)(iRec.x + iRec.width - 70.0f), (int)iy + 9, 9, Color{ 14, 18, 26, 255 });
                } else if (isUnlocked) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 64.0f, iy + 6.0f, 54.0f, 18.0f }, 0.25f, 4, isHov ? Color{ 60, 95, 145, 255 } : Color{ 30, 44, 66, 220 });
                    DrawText("EQUIP", (int)(iRec.x + iRec.width - 54.0f), (int)iy + 9, 9, isHov ? Color{ 255, 255, 255, 255 } : Color{ 175, 200, 230, 220 });
                }

                DrawText(st.description.c_str(), (int)iRec.x + 12, (int)iy + 23, 9, Color{ 170, 190, 215, 210 });
                if (isUnlocked) {
                    std::string sLine = "Defense: +" + std::to_string((int)st.defense) + " | Will: +" + std::to_string((int)st.willBonus);
                    if (st.maxHpBonus > 0.0f) sLine += " | HP: +" + std::to_string((int)st.maxHpBonus);
                    DrawText(sLine.c_str(), (int)iRec.x + 12, (int)iy + 40, 9, Color{ 100, 245, 180, 240 });
                } else {
                    std::string lMsg = "Locked // " + std::string(helms[i].lockMsg);
                    DrawText(lMsg.c_str(), (int)iRec.x + 12, (int)iy + 40, 9, Color{ 230, 95, 95, 220 });
                }

                if (isUnlocked && !isEquipped && isHov && isMousePressed) {
                    player.SetHelmet(helms[i].type);
                }
            }
        } else if (s_selectedSlot == 1) {
            // CHESTPLATES LIST
            struct ChestEntry { ChestplateType type; const char* lockMsg; };
            ChestEntry chests[7] = {
                { ChestplateType::NONE, "Unarmored default" },
                { ChestplateType::COMMONERS_TUNIC, "Ruined Castle - Awakening Pedestal" },
                { ChestplateType::CRUSADER_CUIRASS, "Zyphden - Defeat Knight Lenon in spar" },
                { ChestplateType::VANGUARD_CHEST, "Lobby / Arsenal testing supplies" },
                { ChestplateType::SHADOW_GARB, "Lobby / Arsenal testing supplies" },
                { ChestplateType::SCRAP_CUIRASS, "Blacksmith Varek - Armor Forge" },
                { ChestplateType::BASALT_CUIRASS, "Blacksmith Varek - Armor Forge" }
            };
            for (int i = 0; i < 7; ++i) {
                float iy = drawerListY + (float)i * 61.0f;
                Rectangle iRec = { rightX + 16.0f, iy, rightW - 32.0f, 57.0f };
                bool isUnlocked = player.GetInventory().HasChestplate(chests[i].type);
                bool isEquipped = (player.GetChestplate() == chests[i].type);
                bool isHov = CheckCollisionPointRec(mousePos, iRec);

                ChestplateStats st = GetChestplateStats(chests[i].type);
                DrawRectangleRounded(iRec, 0.12f, 4, isEquipped ? Color{ 34, 50, 78, 255 } : (isHov && isUnlocked ? Color{ 26, 38, 58, 230 } : Color{ 16, 22, 34, 200 }));
                DrawRectangleRoundedLines(iRec, 0.12f, 4, isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? (isHov ? Color{ 100, 160, 240, 220 } : Color{ 48, 65, 95, 200 }) : Color{ 35, 42, 55, 160 }));

                DrawText(st.name.c_str(), (int)iRec.x + 12, (int)iy + 6, 12, isUnlocked ? Color{ 235, 245, 255, 255 } : Color{ 120, 130, 145, 200 });
                if (isEquipped) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 78.0f, iy + 6.0f, 68.0f, 18.0f }, 0.25f, 4, Color{ 255, 215, 80, 230 });
                    DrawText("EQUIPPED", (int)(iRec.x + iRec.width - 70.0f), (int)iy + 9, 9, Color{ 14, 18, 26, 255 });
                } else if (isUnlocked) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 64.0f, iy + 6.0f, 54.0f, 18.0f }, 0.25f, 4, isHov ? Color{ 60, 95, 145, 255 } : Color{ 30, 44, 66, 220 });
                    DrawText("EQUIP", (int)(iRec.x + iRec.width - 54.0f), (int)iy + 9, 9, isHov ? Color{ 255, 255, 255, 255 } : Color{ 175, 200, 230, 220 });
                }

                DrawText(st.description.c_str(), (int)iRec.x + 12, (int)iy + 23, 9, Color{ 170, 190, 215, 210 });
                if (isUnlocked) {
                    std::string sLine = "Defense: +" + std::to_string((int)st.defense) + " | HP: +" + std::to_string((int)st.maxHpBonus) + " | Will: +" + std::to_string((int)st.willBonus);
                    DrawText(sLine.c_str(), (int)iRec.x + 12, (int)iy + 40, 9, Color{ 100, 245, 180, 240 });
                } else {
                    std::string lMsg = "Locked // " + std::string(chests[i].lockMsg);
                    DrawText(lMsg.c_str(), (int)iRec.x + 12, (int)iy + 40, 9, Color{ 230, 95, 95, 220 });
                }

                if (isUnlocked && !isEquipped && isHov && isMousePressed) {
                    player.SetChestplate(chests[i].type);
                }
            }
        } else if (s_selectedSlot == 2) {
            // LEGGINGS LIST
            struct LegEntry { LeggingsType type; const char* lockMsg; };
            LegEntry legs[7] = {
                { LeggingsType::NONE, "Unarmored default" },
                { LeggingsType::COMMONERS_BREECHES, "Ruined Castle - Awakening Pedestal" },
                { LeggingsType::CRUSADER_GREAVES, "Zyphden - Defeat Knight Lenon in spar" },
                { LeggingsType::VANGUARD_GREAVES, "Lobby / Arsenal testing supplies" },
                { LeggingsType::SHADOW_LEGGINGS, "Lobby / Arsenal testing supplies" },
                { LeggingsType::SCRAP_GREAVES, "Blacksmith Varek - Armor Forge" },
                { LeggingsType::BASALT_GREAVES, "Blacksmith Varek - Armor Forge" }
            };
            for (int i = 0; i < 7; ++i) {
                float iy = drawerListY + (float)i * 61.0f;
                Rectangle iRec = { rightX + 16.0f, iy, rightW - 32.0f, 57.0f };
                bool isUnlocked = player.GetInventory().HasLeggings(legs[i].type);
                bool isEquipped = (player.GetLeggings() == legs[i].type);
                bool isHov = CheckCollisionPointRec(mousePos, iRec);

                LeggingsStats st = GetLeggingsStats(legs[i].type);
                DrawRectangleRounded(iRec, 0.12f, 4, isEquipped ? Color{ 34, 50, 78, 255 } : (isHov && isUnlocked ? Color{ 26, 38, 58, 230 } : Color{ 16, 22, 34, 200 }));
                DrawRectangleRoundedLines(iRec, 0.12f, 4, isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? (isHov ? Color{ 100, 160, 240, 220 } : Color{ 48, 65, 95, 200 }) : Color{ 35, 42, 55, 160 }));

                DrawText(st.name.c_str(), (int)iRec.x + 12, (int)iy + 6, 12, isUnlocked ? Color{ 235, 245, 255, 255 } : Color{ 120, 130, 145, 200 });
                if (isEquipped) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 78.0f, iy + 6.0f, 68.0f, 18.0f }, 0.25f, 4, Color{ 255, 215, 80, 230 });
                    DrawText("EQUIPPED", (int)(iRec.x + iRec.width - 70.0f), (int)iy + 9, 9, Color{ 14, 18, 26, 255 });
                } else if (isUnlocked) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 64.0f, iy + 6.0f, 54.0f, 18.0f }, 0.25f, 4, isHov ? Color{ 60, 95, 145, 255 } : Color{ 30, 44, 66, 220 });
                    DrawText("EQUIP", (int)(iRec.x + iRec.width - 54.0f), (int)iy + 9, 9, isHov ? Color{ 255, 255, 255, 255 } : Color{ 175, 200, 230, 220 });
                }

                DrawText(st.description.c_str(), (int)iRec.x + 12, (int)iy + 23, 9, Color{ 170, 190, 215, 210 });
                if (isUnlocked) {
                    std::string sLine = "Defense: +" + std::to_string((int)st.defense) + " | Will: +" + std::to_string((int)st.willBonus);
                    DrawText(sLine.c_str(), (int)iRec.x + 12, (int)iy + 40, 9, Color{ 100, 245, 180, 240 });
                } else {
                    std::string lMsg = "Locked // " + std::string(legs[i].lockMsg);
                    DrawText(lMsg.c_str(), (int)iRec.x + 12, (int)iy + 40, 9, Color{ 230, 95, 95, 220 });
                }

                if (isUnlocked && !isEquipped && isHov && isMousePressed) {
                    player.SetLeggings(legs[i].type);
                }
            }
        } else if (s_selectedSlot == 3) {
            // TOTEMS LIST
            struct TotemEntry { TotemType type; const char* lockMsg; };
            TotemEntry totems[6] = {
                { TotemType::NONE, "No totem attuned" },
                { TotemType::TOTEM_SANITY, "Solsden Sanctuary — Campaign progression" },
                { TotemType::TOTEM_INSANITY, "Sandbox Arena / Testing supplies" },
                { TotemType::TOTEM_MIST, "Sandbox Arena / Testing supplies" },
                { TotemType::CRUSADER_TOTEM, "Zyphden Castle — Royal Knight armory" },
                { TotemType::ELEMENTAL_CORE_TOTEM, "Quarry Primordial Altar — 5% Secret Boss drop" }
            };
            for (int i = 0; i < 6; ++i) {
                float iy = drawerListY + (float)i * 72.0f;
                Rectangle iRec = { rightX + 16.0f, iy, rightW - 32.0f, 68.0f };
                bool isUnlocked = player.GetInventory().HasTotem(totems[i].type);
                bool isEquipped = (player.GetTotem() == totems[i].type);
                bool isHov = CheckCollisionPointRec(mousePos, iRec);

                TotemStats st = GetTotemStats(totems[i].type);
                DrawRectangleRounded(iRec, 0.12f, 4, isEquipped ? Color{ 34, 50, 78, 255 } : (isHov && isUnlocked ? Color{ 26, 38, 58, 230 } : Color{ 16, 22, 34, 200 }));
                DrawRectangleRoundedLines(iRec, 0.12f, 4, isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? (isHov ? Color{ 100, 160, 240, 220 } : Color{ 48, 65, 95, 200 }) : Color{ 35, 42, 55, 160 }));

                DrawText(st.name.c_str(), (int)iRec.x + 12, (int)iy + 6, 12, isUnlocked ? st.glowColor : Color{ 120, 130, 145, 200 });
                if (isEquipped) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 78.0f, iy + 6.0f, 68.0f, 18.0f }, 0.25f, 4, Color{ 255, 215, 80, 230 });
                    DrawText("ATTUNED", (int)(iRec.x + iRec.width - 70.0f), (int)iy + 9, 9, Color{ 14, 18, 26, 255 });
                } else if (isUnlocked) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 64.0f, iy + 6.0f, 54.0f, 18.0f }, 0.25f, 4, isHov ? Color{ 60, 95, 145, 255 } : Color{ 30, 44, 66, 220 });
                    DrawText("ATTUNE", (int)(iRec.x + iRec.width - 54.0f), (int)iy + 9, 9, isHov ? Color{ 255, 255, 255, 255 } : Color{ 175, 200, 230, 220 });
                }

                DrawText(st.description.c_str(), (int)iRec.x + 12, (int)iy + 23, 9, Color{ 170, 190, 215, 210 });
                if (isUnlocked) {
                    std::string bStr = "[+] " + st.buffText;
                    DrawText(bStr.c_str(), (int)iRec.x + 12, (int)iy + 38, 9, Color{ 100, 245, 180, 240 });
                    std::string nStr = "[-] " + st.nerfText;
                    DrawText(nStr.c_str(), (int)iRec.x + 12, (int)iy + 51, 9, (st.nerfText.find("None") != std::string::npos) ? Color{ 160, 180, 200, 200 } : Color{ 255, 95, 95, 240 });
                } else {
                    std::string lMsg = "Locked // " + std::string(totems[i].lockMsg);
                    DrawText(lMsg.c_str(), (int)iRec.x + 12, (int)iy + 45, 9, Color{ 230, 95, 95, 220 });
                }

                if (isUnlocked && isHov && isMousePressed) {
                    player.SetTotem(isEquipped ? TotemType::NONE : totems[i].type);
                }
            }
        } else if (s_selectedSlot == 4) {
            // WEAPONS LIST
            struct WepListEntry { WeaponType type; const char* lockMsg; };
            WepListEntry weps[5] = {
                { WeaponType::SWORD, "Ruined Castle - Awakening Pedestal" },
                { WeaponType::DAGGERS, "Ruined Castle - Upper Gallery Pedestal" },
                { WeaponType::MACE, "Ruined Castle - North Crypt Pedestal" },
                { WeaponType::HALBERD, "Zyphden - Royal Guard Armory Pedestal" },
                { WeaponType::TRUTH_SEEKER, "Royal Arsenal - High Sanctum Pedestal" }
            };
            for (int i = 0; i < 5; ++i) {
                float iy = drawerListY + (float)i * 84.0f;
                Rectangle iRec = { rightX + 16.0f, iy, rightW - 32.0f, 78.0f };
                bool isUnlocked = player.GetInventory().HasWeapon(weps[i].type);
                bool isEquipped = (player.GetWeapon() == weps[i].type);
                bool isHov = CheckCollisionPointRec(mousePos, iRec);

                WeaponStats st = GetWeaponStats(weps[i].type);
                int upgLvl = player.GetInventory().GetWeaponUpgrade(weps[i].type);
                DrawRectangleRounded(iRec, 0.12f, 4, isEquipped ? Color{ 34, 50, 78, 255 } : (isHov && isUnlocked ? Color{ 26, 38, 58, 230 } : Color{ 16, 22, 34, 200 }));
                DrawRectangleRoundedLines(iRec, 0.12f, 4, isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? (isHov ? Color{ 100, 160, 240, 220 } : Color{ 48, 65, 95, 200 }) : Color{ 35, 42, 55, 160 }));

                std::string wTitle = st.name + (upgLvl > 0 ? (" +" + std::to_string(upgLvl)) : "");
                DrawText(wTitle.c_str(), (int)iRec.x + 12, (int)iy + 6, 12, isUnlocked ? Color{ 255, 225, 100, 255 } : Color{ 120, 130, 145, 200 });
                if (isEquipped) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 78.0f, iy + 6.0f, 68.0f, 18.0f }, 0.25f, 4, Color{ 255, 215, 80, 230 });
                    DrawText("EQUIPPED", (int)(iRec.x + iRec.width - 70.0f), (int)iy + 9, 9, Color{ 14, 18, 26, 255 });
                } else if (isUnlocked) {
                    DrawRectangleRounded(Rectangle{ iRec.x + iRec.width - 64.0f, iy + 6.0f, 54.0f, 18.0f }, 0.25f, 4, isHov ? Color{ 60, 95, 145, 255 } : Color{ 30, 44, 66, 220 });
                    DrawText("EQUIP", (int)(iRec.x + iRec.width - 54.0f), (int)iy + 9, 9, isHov ? Color{ 255, 255, 255, 255 } : Color{ 175, 200, 230, 220 });
                }

                DrawText(st.description.c_str(), (int)iRec.x + 12, (int)iy + 23, 9, Color{ 170, 190, 215, 210 });
                if (isUnlocked) {
                    std::string sLine = "Light Dmg: " + std::to_string((int)st.lightDamage) + " | Heavy Dmg: " + std::to_string((int)st.heavyDamage) + " | Range: " + std::to_string((int)st.lightRange);
                    DrawText(sLine.c_str(), (int)iRec.x + 12, (int)iy + 40, 9, Color{ 100, 245, 180, 240 });
                    std::string hLine = "[Poise Break]: " + std::to_string((int)(st.poiseMultiplier * 100.0f)) + "% | Pierce: " + std::to_string((int)(st.defensePierceBonus * 100.0f)) + "%";
                    DrawText(hLine.c_str(), (int)iRec.x + 12, (int)iy + 56, 9, Color{ 255, 200, 100, 230 });
                } else {
                    std::string lMsg = "Locked // " + std::string(weps[i].lockMsg);
                    DrawText(lMsg.c_str(), (int)iRec.x + 12, (int)iy + 48, 9, Color{ 230, 95, 95, 220 });
                }

                if (isUnlocked && !isEquipped && isHov && isMousePressed) {
                    player.SetWeapon(weps[i].type);
                }
            }
        }
    } else {


        // =========================================================================

        // TAB 1: THE ILLUMINATED GRIMOIRE & CHRONICLES OF TYDESHIRE

        // =========================================================================

        const auto& frags = player.GetInventory().GetFragments();

        int totalFrags = (int)frags.size();

        int discoveredFrags = player.GetInventory().GetDiscoveredFragmentCount();



        // Animated Page-Flip State

        static float s_flipProgress = 1.0f;

        static int s_flipDir = 0; // +1 forward (left to right), -1 backward

        static int s_targetFragIndex = 0;



        float frameDt = GetFrameTime();

        if (frameDt > 0.05f) frameDt = 0.016f;



        if (s_flipProgress < 1.0f) {

            s_flipProgress += frameDt * 4.8f;

            if (s_flipProgress >= 0.5f && selectedFragmentIndex != s_targetFragIndex) {

                selectedFragmentIndex = s_targetFragIndex;

                if (selectedFragmentIndex >= 0 && selectedFragmentIndex < totalFrags) {

                    player.GetInventory().MarkFragmentRead(frags[selectedFragmentIndex].id);

                }

            }

            if (s_flipProgress >= 1.0f) {

                s_flipProgress = 1.0f;

            }

        }



        // Hotkeys for page flipping: [A] / [LEFT] = Prev, [D] / [RIGHT] = Next

        if (s_flipProgress >= 1.0f) {

            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {

                if (selectedFragmentIndex > 0) {

                    s_targetFragIndex = selectedFragmentIndex - 1;

                    s_flipDir = -1;

                    s_flipProgress = 0.0f;

                    SoundFX::PlayButtonClick();

                }

            } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {

                if (selectedFragmentIndex < totalFrags - 1) {

                    s_targetFragIndex = selectedFragmentIndex + 1;

                    s_flipDir = 1;

                    s_flipProgress = 0.0f;

                    SoundFX::PlayButtonClick();

                }

            }

        }



        // Clamp selected index

        if (selectedFragmentIndex < 0) selectedFragmentIndex = 0;

        if (selectedFragmentIndex >= totalFrags) selectedFragmentIndex = totalFrags - 1;



        // Book Geometry

        float bookX = modalX + 26.0f;

        float bookY = contentY + 2.0f;

        float bookW = modalW - 52.0f; // 1028 px

        float bookH = 485.0f;



        // 1. Dark Antique Mahogany Leather Cover Backing

        Rectangle coverRec = { bookX, bookY, bookW, bookH };

        DrawRectangleRounded(coverRec, 0.04f, 8, Color{ 34, 20, 14, 255 });

        DrawRectangleRoundedLines(coverRec, 0.04f, 8, Color{ 175, 130, 55, 230 });

        DrawRectangleRoundedLines(Rectangle{ bookX + 4.0f, bookY + 4.0f, bookW - 8.0f, bookH - 8.0f }, 0.035f, 8, Color{ 75, 48, 25, 180 });



        // Antique Brass Corner Brackets on Leather Cover

        Color brassCol = Color{ 215, 175, 75, 230 };

        float cLen = 26.0f;

        DrawLineEx({ bookX + 8.0f, bookY + 8.0f }, { bookX + 8.0f + cLen, bookY + 8.0f }, 2.0f, brassCol);

        DrawLineEx({ bookX + 8.0f, bookY + 8.0f }, { bookX + 8.0f, bookY + 8.0f + cLen }, 2.0f, brassCol);

        DrawLineEx({ bookX + bookW - 8.0f, bookY + 8.0f }, { bookX + bookW - 8.0f - cLen, bookY + 8.0f }, 2.0f, brassCol);

        DrawLineEx({ bookX + bookW - 8.0f, bookY + 8.0f }, { bookX + bookW - 8.0f, bookY + 8.0f + cLen }, 2.0f, brassCol);

        DrawLineEx({ bookX + 8.0f, bookY + bookH - 8.0f }, { bookX + 8.0f + cLen, bookY + bookH - 8.0f }, 2.0f, brassCol);

        DrawLineEx({ bookX + 8.0f, bookY + bookH - 8.0f }, { bookX + 8.0f, bookY + bookH - 8.0f - cLen }, 2.0f, brassCol);

        DrawLineEx({ bookX + bookW - 8.0f, bookY + bookH - 8.0f }, { bookX + bookW - 8.0f - cLen, bookY + bookH - 8.0f }, 2.0f, brassCol);

        DrawLineEx({ bookX + bookW - 8.0f, bookY + bookH - 8.0f }, { bookX + bookW - 8.0f, bookY + bookH - 8.0f - cLen }, 2.0f, brassCol);



        // 2. Open Parchment Pages Dimensions

        float spineWidth = 24.0f;

        float spineX = bookX + bookW * 0.5f;

        float pageMargin = 12.0f;

        float pageW = (bookW - spineWidth - pageMargin * 2.0f) * 0.5f; // ~485px

        float pageH = bookH - 24.0f; // 461px

        float leftPageX = bookX + pageMargin;

        float rightPageX = spineX + spineWidth * 0.5f;

        float pageY = bookY + 12.0f;



        // Deckle paper edge layers beneath pages

        for (int layer = 3; layer >= 1; --layer) {

            float off = (float)layer * 1.5f;

            Color deckleCol = Color{ 210, 198, 172, (unsigned char)(210 - layer * 35) };

            DrawRectangleRounded(Rectangle{ leftPageX - off, pageY + off, pageW, pageH }, 0.02f, 4, deckleCol);

            DrawRectangleRounded(Rectangle{ rightPageX + off, pageY + off, pageW, pageH }, 0.02f, 4, deckleCol);

        }



        // Left & Right Parchment Sheets

        Color parchmentBase = Color{ 246, 238, 218, 255 };

        Color parchmentBorder = Color{ 190, 170, 135, 220 };

        Rectangle leftPageRec = { leftPageX, pageY, pageW, pageH };

        Rectangle rightPageRec = { rightPageX, pageY, pageW, pageH };



        DrawRectangleRounded(leftPageRec, 0.02f, 4, parchmentBase);

        DrawRectangleRoundedLines(leftPageRec, 0.02f, 4, parchmentBorder);



        DrawRectangleRounded(rightPageRec, 0.02f, 4, parchmentBase);

        DrawRectangleRoundedLines(rightPageRec, 0.02f, 4, parchmentBorder);



        // Spine Crease & Shadow

        DrawRectangleGradientH((int)(spineX - 34.0f), (int)pageY, 34, (int)pageH, Color{ 0, 0, 0, 0 }, Color{ 70, 45, 25, 95 });

        DrawRectangleGradientH((int)(spineX), (int)pageY, 34, (int)pageH, Color{ 70, 45, 25, 95 }, Color{ 0, 0, 0, 0 });

        DrawRectangle((int)(spineX - 5.0f), (int)pageY - 2, 10, (int)pageH + 4, Color{ 20, 12, 8, 255 });

        DrawRectangleLines((int)(spineX - 5.0f), (int)pageY - 2, 10, (int)pageH + 4, Color{ 115, 80, 40, 200 });



        // Crimson Silk Bookmark hanging from upper spine

        DrawRectangle((int)spineX - 4, (int)bookY - 2, 8, 44, Color{ 185, 35, 35, 255 });

        DrawTriangle({ spineX - 4.0f, bookY + 42.0f }, { spineX + 4.0f, bookY + 42.0f }, { spineX, bookY + 50.0f }, Color{ 230, 185, 60, 255 });



        const auto& curFrag = frags[selectedFragmentIndex];



        // =========================================================================

        // LEFT PAGE CONTENT: CHAPTER INDEX & LOCATION CLUES

        // =========================================================================

        float lPad = leftPageX + 22.0f;

        float lW = pageW - 44.0f;



        // Left Header

        FontManager::DrawText("CHRONICLES OF TYDESHIRE", (int)lPad, (int)pageY + 16, 14, Color{ 55, 35, 20, 255 }, GameFont::TITLE);

        std::string progressStr = "Restored: " + std::to_string(discoveredFrags) + " / " + std::to_string(totalFrags) + " Fragments";

        FontManager::DrawText(progressStr.c_str(), (int)lPad, (int)pageY + 34, 11, Color{ 135, 90, 35, 255 }, GameFont::BODY);

        DrawLine((int)lPad, (int)pageY + 50, (int)(lPad + lW), (int)pageY + 50, Color{ 180, 150, 105, 200 });



        // Bookmark Page Selector (10 clickable tabs across 2 rows)

        FontManager::DrawText("PAGE BOOKMARKS (CLICK TO TURN PAGE):", (int)lPad, (int)pageY + 58, 10, Color{ 110, 80, 45, 240 }, GameFont::TITLE);

        const char* romanNumerals[10] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X" };

        float tabW = (lW - 16.0f) / 5.0f;

        float tabH = 26.0f;



        for (int i = 0; i < 10 && i < totalFrags; i++) {

            int row = i / 5;

            int col = i % 5;

            float tx = lPad + (float)col * (tabW + 4.0f);

            float ty = pageY + 76.0f + (float)row * (tabH + 4.0f);

            Rectangle tabRec = { tx, ty, tabW, tabH };

            bool isTabHov = CheckCollisionPointRec(mousePos, tabRec);

            bool isTabSel = (selectedFragmentIndex == i);



            Color tBg = isTabSel ? Color{ 215, 175, 90, 255 } : (isTabHov ? Color{ 232, 218, 186, 255 } : Color{ 238, 228, 205, 230 });

            Color tBorder = isTabSel ? Color{ 120, 75, 25, 255 } : (frags[i].discovered ? Color{ 160, 130, 85, 220 } : Color{ 190, 175, 150, 180 });



            DrawRectangleRounded(tabRec, 0.2f, 4, tBg);

            DrawRectangleRoundedLines(tabRec, 0.2f, 4, tBorder);



            std::string label = std::string(romanNumerals[i]);

            int lbw = FontManager::MeasureText(label, 11, GameFont::TITLE);

            Color textCol = isTabSel ? Color{ 35, 20, 10, 255 } : (frags[i].discovered ? Color{ 70, 45, 25, 240 } : Color{ 145, 130, 110, 200 });

            FontManager::DrawText(label, (int)(tx + (tabW - lbw) * 0.5f), (int)ty + 6, 11, textCol, GameFont::TITLE);



            // Red unread notification dot

            if (frags[i].discovered && !frags[i].isRead) {

                DrawCircle((int)tx + (int)tabW - 5, (int)ty + 5, 3.5f, Color{ 210, 45, 45, 255 });

            }



            if (isTabHov && isMousePressed && s_flipProgress >= 1.0f && selectedFragmentIndex != i) {

                s_targetFragIndex = i;

                s_flipDir = (i > selectedFragmentIndex) ? 1 : -1;

                s_flipProgress = 0.0f;

                SoundFX::PlayButtonClick();

            }

        }



        // Active Chapter Summary

        float sumY = pageY + 144.0f;

        DrawLine((int)lPad, (int)sumY, (int)(lPad + lW), (int)sumY, Color{ 180, 150, 105, 180 });



        std::string chapHeader = "SELECTED: " + curFrag.title;

        FontManager::DrawText(chapHeader.c_str(), (int)lPad, (int)sumY + 8, 11, Color{ 45, 30, 18, 255 }, GameFont::TITLE);

        std::string realmStr = "Origin Realm: " + curFrag.realm;

        FontManager::DrawText(realmStr.c_str(), (int)lPad, (int)sumY + 24, 10, Color{ 85, 60, 35, 220 }, GameFont::BODY);



        // DEDICATED LOCATION CLUE & COMPASS PARCHMENT PLAQUE

        float clueBoxY = sumY + 44.0f;

        float clueBoxH = 200.0f;

        Rectangle clueRec = { lPad, clueBoxY, lW, clueBoxH };



        DrawRectangleRounded(clueRec, 0.08f, 6, Color{ 238, 228, 204, 255 });

        DrawRectangleRoundedLines(clueRec, 0.08f, 6, Color{ 165, 130, 80, 220 });

        DrawRectangleRoundedLines(Rectangle{ lPad + 3.0f, clueBoxY + 3.0f, lW - 6.0f, clueBoxH - 6.0f }, 0.07f, 6, Color{ 215, 195, 160, 180 });



        // Compass Rose Header

        FontManager::DrawText("LOCATION CLUE & RECOVERY COMPASS", (int)lPad + 14, (int)clueBoxY + 12, 11, Color{ 160, 38, 28, 255 }, GameFont::TITLE);

        DrawLine((int)lPad + 14, (int)clueBoxY + 28, (int)(lPad + lW - 14), (int)clueBoxY + 28, Color{ 180, 140, 95, 180 });



        // Recovery Clue Body

        std::string clueBody = curFrag.clue.empty() ? "Seek forgotten altars and guarded chests throughout Tydeshire." : curFrag.clue;

        FontManager::DrawTextWrapped(clueBody, (int)lPad + 14, (int)clueBoxY + 38, (int)lW - 28, 12, 5, Color{ 48, 34, 22, 255 }, GameFont::BODY);



        // Discovery Status Badge inside plaque

        float badgeY = clueBoxY + clueBoxH - 36.0f;

        if (curFrag.discovered) {

            DrawRectangleRounded(Rectangle{ lPad + 14, badgeY, lW - 28, 24.0f }, 0.25f, 4, Color{ 215, 240, 215, 255 });

            DrawRectangleRoundedLines(Rectangle{ lPad + 14, badgeY, lW - 28, 24.0f }, 0.25f, 4, Color{ 40, 140, 60, 220 });

            FontManager::DrawText("RECOVERED & CHRONICLED", (int)lPad + 24, (int)badgeY + 6, 10, Color{ 20, 110, 45, 255 }, GameFont::TITLE);

        } else {

            DrawRectangleRounded(Rectangle{ lPad + 14, badgeY, lW - 28, 24.0f }, 0.25f, 4, Color{ 245, 220, 215, 255 });

            DrawRectangleRoundedLines(Rectangle{ lPad + 14, badgeY, lW - 28, 24.0f }, 0.25f, 4, Color{ 180, 50, 40, 220 });

            FontManager::DrawText("SHROUDED - SEARCH THIS LOCATION", (int)lPad + 24, (int)badgeY + 6, 10, Color{ 160, 35, 25, 255 }, GameFont::TITLE);

        }



        // Bottom Left Page Navigation: Previous Page Button

        float navBtnY = pageY + pageH - 42.0f;

        float navBtnW = 160.0f;

        float navBtnH = 30.0f;

        Rectangle prevBtnRec = { lPad, navBtnY, navBtnW, navBtnH };

        bool canPrev = (selectedFragmentIndex > 0);

        bool isPrevHov = canPrev && CheckCollisionPointRec(mousePos, prevBtnRec);



        if (canPrev) {

            DrawRectangleRounded(prevBtnRec, 0.25f, 4, isPrevHov ? Color{ 210, 185, 140, 255 } : Color{ 228, 214, 185, 255 });

            DrawRectangleRoundedLines(prevBtnRec, 0.25f, 4, isPrevHov ? Color{ 110, 70, 25, 255 } : Color{ 155, 120, 75, 220 });

            FontManager::DrawText("< PREV PAGE [A]", (int)lPad + 18, (int)navBtnY + 8, 11, isPrevHov ? Color{ 20, 12, 6, 255 } : Color{ 65, 42, 22, 240 }, GameFont::TITLE);



            if (isPrevHov && isMousePressed && s_flipProgress >= 1.0f) {

                s_targetFragIndex = selectedFragmentIndex - 1;

                s_flipDir = -1;

                s_flipProgress = 0.0f;

                SoundFX::PlayButtonClick();

            }

        }



        // =========================================================================

        // RIGHT PAGE CONTENT: ILLUMINATED CHRONICLE SPREAD

        // =========================================================================

        float rPad = rightPageX + 22.0f;

        float rW = pageW - 44.0f;



        // Right Header & Page Number

        std::string pageNumStr = "Page " + std::to_string(selectedFragmentIndex + 1) + " of " + std::to_string(totalFrags);

        int pnw = FontManager::MeasureText(pageNumStr, 11, GameFont::BODY);

        FontManager::DrawText(pageNumStr.c_str(), (int)(rPad + rW - pnw), (int)pageY + 16, 11, Color{ 120, 90, 50, 220 }, GameFont::BODY);



        std::string rOrigin = "HISTORICAL RECORD // " + curFrag.realm;

        FontManager::DrawText(rOrigin.c_str(), (int)rPad, (int)pageY + 16, 10, Color{ 40, 70, 95, 240 }, GameFont::TITLE);

        DrawLine((int)rPad, (int)pageY + 32, (int)(rPad + rW), (int)pageY + 32, Color{ 180, 150, 105, 180 });



        // Chronicle Title in Burnished Title Font

        FontManager::DrawText(curFrag.title.c_str(), (int)rPad, (int)pageY + 42, 16, Color{ 68, 36, 18, 255 }, GameFont::TITLE);

        DrawLine((int)rPad, (int)pageY + 68, (int)(rPad + rW), (int)pageY + 68, Color{ 160, 125, 65, 200 });



        // Illuminated Body Content

        if (curFrag.discovered) {

            // Illuminated Initial Capital Letter Box

            char firstChar = curFrag.fullText.empty() ? 'T' : curFrag.fullText[0];

            std::string dropLetter(1, firstChar);

            float dropBoxX = rPad;

            float dropBoxY = pageY + 78.0f;

            float dropBoxSize = 38.0f;



            DrawRectangleRounded(Rectangle{ dropBoxX, dropBoxY, dropBoxSize, dropBoxSize }, 0.2f, 4, Color{ 225, 195, 130, 255 });

            DrawRectangleRoundedLines(Rectangle{ dropBoxX, dropBoxY, dropBoxSize, dropBoxSize }, 0.2f, 4, Color{ 120, 75, 25, 255 });

            int dlw = FontManager::MeasureText(dropLetter, 22, GameFont::TITLE);

            FontManager::DrawText(dropLetter, (int)(dropBoxX + (dropBoxSize - dlw) * 0.5f), (int)dropBoxY + 8, 22, Color{ 65, 25, 12, 255 }, GameFont::TITLE);



            // Narrative Text Body

            std::string remainingText = (curFrag.fullText.size() > 1) ? curFrag.fullText.substr(1) : "";

            FontManager::DrawTextWrapped(remainingText, (int)rPad + (int)dropBoxSize + 10, (int)dropBoxY + 4, (int)rW - (int)dropBoxSize - 10, 13, 6, Color{ 42, 30, 20, 255 }, GameFont::BODY);



            // Bottom Historical Epilogue Quote

            float quoteY = pageY + pageH - 74.0f;

            DrawLine((int)rPad, (int)quoteY, (int)(rPad + rW), (int)quoteY, Color{ 180, 150, 105, 160 });

            const char* bookQuote = "\"The truth of Tydeshire endures though crowns turn to rust and kings fall to madness.\"";

            FontManager::DrawText(bookQuote, (int)rPad, (int)quoteY + 10, 10, Color{ 130, 95, 45, 220 }, GameFont::BODY);

        } else {

            // Shrouded Page Graphic

            float shroudBoxY = pageY + 95.0f;

            float shroudBoxH = 260.0f;

            Rectangle shroudRec = { rPad, shroudBoxY, rW, shroudBoxH };



            DrawRectangleRounded(shroudRec, 0.05f, 6, Color{ 232, 220, 195, 255 });

            DrawRectangleRoundedLines(shroudRec, 0.05f, 6, Color{ 170, 140, 105, 200 });



            // Arcane Runes Row

            const char* runes = "* * *  R U N I C   S E A L  * * *";

            int rnw = FontManager::MeasureText(runes, 11, GameFont::TITLE);

            FontManager::DrawText(runes, (int)(rPad + (rW - rnw) * 0.5f), (int)shroudBoxY + 28, 11, Color{ 150, 110, 60, 220 }, GameFont::TITLE);



            const char* shTitle = "THIS CHRONICLE FRAGMENT REMAINS SHROUDED";

            int stw = FontManager::MeasureText(shTitle, 13, GameFont::TITLE);

            FontManager::DrawText(shTitle, (int)(rPad + (rW - stw) * 0.5f), (int)shroudBoxY + 68, 13, Color{ 165, 40, 30, 255 }, GameFont::TITLE);



            std::string shDesc =

                "The parchment detailing this epoch was torn asunder when King Charlz succumbed to the outer corruption.\n\n"

                "Examine the Recovery Compass on the left page to track down its hidden resting place across the realm.";

            FontManager::DrawTextWrapped(shDesc, (int)rPad + 20, (int)shroudBoxY + 108, (int)rW - 40, 12, 6, Color{ 60, 45, 30, 240 }, GameFont::BODY);

        }



        // Bottom Right Page Navigation: Next Page Button

        Rectangle nextBtnRec = { rPad + rW - navBtnW, navBtnY, navBtnW, navBtnH };

        bool canNext = (selectedFragmentIndex < totalFrags - 1);

        bool isNextHov = canNext && CheckCollisionPointRec(mousePos, nextBtnRec);



        if (canNext) {

            DrawRectangleRounded(nextBtnRec, 0.25f, 4, isNextHov ? Color{ 210, 185, 140, 255 } : Color{ 228, 214, 185, 255 });

            DrawRectangleRoundedLines(nextBtnRec, 0.25f, 4, isNextHov ? Color{ 110, 70, 25, 255 } : Color{ 155, 120, 75, 220 });

            FontManager::DrawText("NEXT PAGE [D] >", (int)(rPad + rW - navBtnW + 18), (int)navBtnY + 8, 11, isNextHov ? Color{ 20, 12, 6, 255 } : Color{ 65, 42, 22, 240 }, GameFont::TITLE);



            if (isNextHov && isMousePressed && s_flipProgress >= 1.0f) {

                s_targetFragIndex = selectedFragmentIndex + 1;

                s_flipDir = 1;

                s_flipProgress = 0.0f;

                SoundFX::PlayButtonClick();

            }

        }



        // =========================================================================

        // ANIMATED 3D PAGE-FLIP OVERLAY EFFECT

        // =========================================================================

        if (s_flipProgress < 1.0f) {

            float flipAngle = s_flipProgress * PI;

            float cosVal = cosf(flipAngle);

            float flipW = pageW * fabsf(cosVal);

            float flipAlpha = sinf(flipAngle);



            // Sweeping shadow cast across the stationary page

            if (s_flipDir > 0) {

                // Turning forward (right page flips over to left)

                float shadowX = spineX + (1.0f - s_flipProgress) * (pageW * 0.7f);

                DrawRectangleGradientH((int)spineX, (int)pageY, (int)(shadowX - spineX), (int)pageH, ColorAlpha(BLACK, 0.28f * flipAlpha), Color{ 0, 0, 0, 0 });

            } else {

                // Turning backward (left page flips over to right)

                float shadowX = spineX - (1.0f - s_flipProgress) * (pageW * 0.7f);

                DrawRectangleGradientH((int)shadowX, (int)pageY, (int)(spineX - shadowX), (int)pageH, Color{ 0, 0, 0, 0 }, ColorAlpha(BLACK, 0.28f * flipAlpha));

            }



            // The curling flipping page rectangle

            Rectangle curlingPage;

            if (cosVal >= 0.0f) {

                // Page is still on the source side of the spine

                if (s_flipDir > 0) {

                    curlingPage = { spineX, pageY, flipW, pageH };

                } else {

                    curlingPage = { spineX - flipW, pageY, flipW, pageH };

                }

            } else {

                // Page has crossed over the spine to the destination side

                if (s_flipDir > 0) {

                    curlingPage = { spineX - flipW, pageY, flipW, pageH };

                } else {

                    curlingPage = { spineX, pageY, flipW, pageH };

                }

            }



            // Render curling parchment sheet with gradient curl shading

            Color curlPaper = Color{ 242, 232, 208, 255 };

            DrawRectangleRounded(curlingPage, 0.02f, 4, curlPaper);

            DrawRectangleRoundedLines(curlingPage, 0.02f, 4, Color{ 170, 145, 110, 230 });



            // Crease highlight on page turn

            float curlEdgeX = (s_flipDir > 0) ? (curlingPage.x + curlingPage.width) : curlingPage.x;

            DrawLineEx({ curlEdgeX, pageY }, { curlEdgeX, pageY + pageH }, 2.0f, ColorAlpha(Color{ 255, 245, 220, 255 }, 0.6f * flipAlpha));

        }

    }



    if (closeClicked || IsKeyPressed(KEY_ESCAPE)) {

        SoundFX::PlayButtonClick();

        return true;

    }



    return false;

}





int ModalUI::DrawZilDefeat(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool canRetry) {

    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 4, 6, 225 });



    float modalW = 740.0f;

    float modalH = 430.0f;

    float modalX = ((float)windowWidth - modalW) * 0.5f;

    float modalY = ((float)windowHeight - modalH) * 0.5f;



    DrawRectangleRounded(Rectangle{ modalX, modalY, modalW, modalH }, 0.05f, 8, Color{ 18, 12, 16, 252 });

    DrawRectangleRoundedLines(Rectangle{ modalX, modalY, modalW, modalH }, 0.05f, 8, Color{ 220, 110, 45, 240 });

    DrawRectangleRoundedLines(Rectangle{ modalX + 4, modalY + 4, modalW - 8, modalH - 8 }, 0.045f, 8, Color{ 60, 35, 40, 180 });



    Color cornerCol = Color{ 255, 160, 50, 240 };

    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 38, modalY + 12 }, 2.0f, cornerCol);

    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 12, modalY + 38 }, 2.0f, cornerCol);

    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 38, modalY + 12 }, 2.0f, cornerCol);

    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 12, modalY + 38 }, 2.0f, cornerCol);

    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 38, modalY + modalH - 12 }, 2.0f, cornerCol);

    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 12, modalY + modalH - 38 }, 2.0f, cornerCol);

    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 38, modalY + modalH - 12 }, 2.0f, cornerCol);

    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 12, modalY + modalH - 38 }, 2.0f, cornerCol);



    int headerW = MeasureText("TRIAL OF THE FALLEN CRUSADER", 20);

    DrawText("TRIAL OF THE FALLEN CRUSADER", (int)(modalX + (modalW - headerW) * 0.5f), (int)modalY + 28, 20, Color{ 255, 90, 70, 255 });

    DrawLine((int)modalX + 50, (int)modalY + 62, (int)(modalX + modalW - 50), (int)modalY + 62, Color{ 140, 70, 30, 200 });



    int choiceResult = 0;



    if (canRetry) {

        int subW = MeasureText("THE SHADOW OF ZIL REMAINS UNBROKEN", 15);

        DrawText("THE SHADOW OF ZIL REMAINS UNBROKEN", (int)(modalX + (modalW - subW) * 0.5f), (int)modalY + 84, 15, Color{ 255, 205, 80, 255 });



        std::string quoteText =

            "Your blade struck with fierce purpose, wanderer, but the sacred cadence of Truth Seeker proved unyielding.\n\n"

            "The knight's remnant lowers his weapon, observing your labored breath with quiet respect. Will you rise once more to prove your mastery... or accept his mercy and press northward?";

        DrawJournalWrappedText(quoteText, (int)modalX + 50, (int)modalY + 120, (int)modalW - 100, 14, 8, Color{ 230, 235, 245, 240 });



        const char* noteText = "Martial Accord: You may invoke one honorable rematch to master Truth Seeker's cadence.";

        int noteW = MeasureText(noteText, 11);

        DrawText(noteText, (int)(modalX + (modalW - noteW) * 0.5f), (int)modalY + 280, 11, Color{ 255, 170, 60, 220 });



        Rectangle btnRetry = { modalX + 50.0f, modalY + 330.0f, 300.0f, 54.0f };

        bool hoverRetry = CheckCollisionPointRec(mousePos, btnRetry);

        DrawRectangleRounded(btnRetry, 0.2f, 6, hoverRetry ? Color{ 90, 35, 20, 255 } : Color{ 45, 22, 16, 240 });

        DrawRectangleRoundedLines(btnRetry, 0.2f, 6, hoverRetry ? Color{ 255, 190, 70, 255 } : Color{ 220, 120, 40, 230 });

        int btn1W = MeasureText("INVOKE REMATCH [1 ATTEMPT]", 14);

        DrawText("INVOKE REMATCH [1 ATTEMPT]", (int)(btnRetry.x + (btnRetry.width - btn1W) * 0.5f), (int)btnRetry.y + 19, 14, hoverRetry ? Color{ 255, 255, 255, 255 } : Color{ 255, 215, 100, 240 });



        Rectangle btnProceed = { modalX + 390.0f, modalY + 330.0f, 300.0f, 54.0f };

        bool hoverProceed = CheckCollisionPointRec(mousePos, btnProceed);

        DrawRectangleRounded(btnProceed, 0.2f, 6, hoverProceed ? Color{ 35, 55, 85, 255 } : Color{ 22, 28, 42, 240 });

        DrawRectangleRoundedLines(btnProceed, 0.2f, 6, hoverProceed ? Color{ 140, 210, 255, 255 } : Color{ 90, 130, 180, 230 });

        int btn2W = MeasureText("ACCEPT MERCY & PROCEED", 14);

        DrawText("ACCEPT MERCY & PROCEED", (int)(btnProceed.x + (btnProceed.width - btn2W) * 0.5f), (int)btnProceed.y + 19, 14, hoverProceed ? Color{ 255, 255, 255, 255 } : Color{ 180, 215, 255, 240 });



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

        int subW = MeasureText("HONOR ACCORD OF THE SUN CRUSADER", 15);

        DrawText("HONOR ACCORD OF THE SUN CRUSADER", (int)(modalX + (modalW - subW) * 0.5f), (int)modalY + 84, 15, Color{ 140, 210, 255, 255 });



        std::string quoteText =

            "The rematch is ended, yet your resolve remains unbroken in the eyes of the crusader.\n\n"

            "Take Zil's crusade seal northward, wanderer. Cross the northern threshold and present his crest to Captain Lenon in Zyphden.";

        DrawJournalWrappedText(quoteText, (int)modalX + 50, (int)modalY + 120, (int)modalW - 100, 14, 8, Color{ 230, 235, 245, 240 });



        Rectangle btnProceed = { modalX + 170.0f, modalY + 330.0f, 400.0f, 54.0f };

        bool hoverProceed = CheckCollisionPointRec(mousePos, btnProceed);

        DrawRectangleRounded(btnProceed, 0.2f, 6, hoverProceed ? Color{ 35, 60, 95, 255 } : Color{ 24, 32, 50, 240 });

        DrawRectangleRoundedLines(btnProceed, 0.2f, 6, hoverProceed ? Color{ 140, 220, 255, 255 } : Color{ 100, 150, 210, 230 });

        int btnW = MeasureText("ENTER THE REALM OF ZYPHDEN", 14);

        DrawText("ENTER THE REALM OF ZYPHDEN", (int)(btnProceed.x + (btnProceed.width - btnW) * 0.5f), (int)btnProceed.y + 19, 14, hoverProceed ? Color{ 255, 255, 255, 255 } : Color{ 190, 225, 255, 240 });



        if (isMousePressed && hoverProceed) {

            SoundFX::PlayButtonClick();

            choiceResult = 2;

        }

    }



    return choiceResult;

}



int ModalUI::DrawBorderlandsGateway(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player) {

    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 11, 18, 240 });



    float cardW = 1060.0f;

    float cardH = 650.0f;

    float cardX = ((float)windowWidth - cardW) * 0.5f;

    float cardY = ((float)windowHeight - cardH) * 0.5f;



    DrawRectangleRounded(Rectangle{ cardX, cardY, cardW, cardH }, 0.04f, 8, Color{ 14, 18, 28, 252 });

    DrawRectangleRoundedLines(Rectangle{ cardX, cardY, cardW, cardH }, 0.04f, 8, Color{ 215, 175, 75, 240 });

    DrawRectangleRoundedLines(Rectangle{ cardX + 4, cardY + 4, cardW - 8, cardH - 8 }, 0.035f, 8, Color{ 48, 65, 96, 180 });



    Color cornerCol = Color{ 255, 215, 80, 240 };

    DrawLineEx({ cardX + 12, cardY + 12 }, { cardX + 38, cardY + 12 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + 12, cardY + 12 }, { cardX + 12, cardY + 38 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + cardW - 12, cardY + 12 }, { cardX + cardW - 38, cardY + 12 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + cardW - 12, cardY + 12 }, { cardX + cardW - 12, cardY + 38 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + 12, cardY + cardH - 12 }, { cardX + 38, cardY + cardH - 12 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + 12, cardY + cardH - 12 }, { cardX + 12, cardY + cardH - 38 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + cardW - 12, cardY + cardH - 12 }, { cardX + cardW - 38, cardY + cardH - 12 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + cardW - 12, cardY + cardH - 12 }, { cardX + cardW - 12, cardY + cardH - 38 }, 2.0f, cornerCol);



    int headerW = MeasureText("CHAPTER 1 COMPLETE // STORY IN DEVELOPMENT", 22);

    DrawText("CHAPTER 1 COMPLETE // STORY IN DEVELOPMENT", (int)(cardX + (cardW - headerW) * 0.5f), (int)cardY + 28, 22, Color{ 255, 215, 80, 255 });

    

    const char* subQuote = "“The northern road into Zyphden and the remaining kingdoms of Tydeshire is currently in development.”";

    int subW = MeasureText(subQuote, 13);

    DrawText(subQuote, (int)(cardX + (cardW - subW) * 0.5f), (int)cardY + 58, 13, Color{ 160, 215, 245, 230 });

    DrawLine((int)cardX + 40, (int)cardY + 84, (int)(cardX + cardW - 40), (int)cardY + 84, Color{ 140, 110, 50, 200 });



    DrawText("OVERLAND ATLAS // THE FIVE KINGDOMS OF TYDESHIRE", (int)cardX + 44, (int)cardY + 98, 12, Color{ 255, 195, 75, 255 });



    struct KingdomCard {

        std::string name;

        std::string status;

        std::string desc;

        Color color;

        bool unlocked;

    };



    KingdomCard realms[5] = {

        { "ZYPHDEN", "IN DEV", "Windborne Spires & Ancient Altars", Color{ 100, 230, 185, 255 }, true },

        { "AZSHDEN", "LOCKED", "Sunken Bastion & Coastal Keeps", Color{ 80, 160, 240, 180 }, false },

        { "FLEURSHDEN", "LOCKED", "Blooming Thorns & Royal Knights", Color{ 235, 120, 165, 180 }, false },

        { "NOIRDEN", "LOCKED", "Shrouded Obsidian Peaks", Color{ 180, 115, 245, 180 }, false },

        { "SOLSDEN", "LOCKED", "Radiant Sun-Plains & High Sanctuary", Color{ 255, 205, 75, 180 }, false }

    };



    float rcardW = (cardW - 88.0f - 40.0f) / 5.0f;

    float rcardH = 150.0f;

    float rcardY = cardY + 124.0f;



    for (int k = 0; k < 5; k++) {

        float rcardX = cardX + 44.0f + (float)k * (rcardW + 10.0f);

        Rectangle rrec = { rcardX, rcardY, rcardW, rcardH };



        Color fillCol = realms[k].unlocked ? Color{ 22, 38, 48, 250 } : Color{ 16, 20, 30, 240 };

        DrawRectangleRounded(rrec, 0.12f, 4, fillCol);

        DrawRectangleRoundedLines(rrec, 0.12f, 4, realms[k].unlocked ? realms[k].color : Color{ 50, 65, 90, 200 });



        DrawText(realms[k].name.c_str(), (int)rcardX + 12, (int)rcardY + 16, 14, realms[k].color);



        Color statusBg = realms[k].unlocked ? Color{ 30, 80, 60, 240 } : Color{ 35, 40, 55, 220 };

        DrawRectangleRounded(Rectangle{ rcardX + 12, rcardY + 38, 85, 20 }, 0.3f, 4, statusBg);

        DrawText(realms[k].status.c_str(), (int)rcardX + 18, (int)rcardY + 42, 10, realms[k].unlocked ? Color{ 140, 255, 200, 255 } : Color{ 160, 175, 200, 220 });



        DrawJournalWrappedText(realms[k].desc, (int)rcardX + 12, (int)rcardY + 68, (int)rcardW - 24, 11, 4, Color{ 190, 205, 225, 230 });

    }



    float sec2Y = cardY + 300.0f;

    DrawLine((int)cardX + 40, (int)sec2Y, (int)(cardX + cardW - 40), (int)sec2Y, Color{ 50, 68, 96, 200 });



    float col1X = cardX + 44.0f;

    float col1W = (cardW - 88.0f - 24.0f) * 0.5f;

    float col2X = col1X + col1W + 24.0f;

    float col2W = col1W;



    Rectangle armoryBox = { col1X, sec2Y + 20.0f, col1W, 200.0f };

    DrawRectangleRounded(armoryBox, 0.08f, 6, Color{ 18, 24, 36, 240 });

    DrawRectangleRoundedLines(armoryBox, 0.08f, 6, Color{ 80, 110, 160, 200 });



    DrawText("CHOSEN WEAPON & LEGENDARY ARSENAL", (int)col1X + 20, (int)sec2Y + 36, 12, Color{ 255, 205, 80, 255 });

    WeaponStats wStats = GetWeaponStats(player.GetWeapon(), player.GetInventory().GetWeaponUpgrade(player.GetWeapon()));

    DrawText(wStats.name.c_str(), (int)col1X + 20, (int)sec2Y + 58, 18, (player.GetWeapon() == WeaponType::TRUTH_SEEKER) ? Color{ 255, 225, 100, 255 } : Color{ 220, 235, 255, 255 });



    std::string wStatsStr = "Damage: " + std::to_string((int)wStats.lightDamage) + " Light / " + std::to_string((int)wStats.heavyDamage) + " Heavy  |  Speed: " + std::to_string((int)(1.0f / wStats.lightCooldown)) + " atk/s";

    DrawText(wStatsStr.c_str(), (int)col1X + 20, (int)sec2Y + 84, 12, Color{ 160, 190, 220, 230 });



    if (player.GetWeapon() == WeaponType::TRUTH_SEEKER) {

        DrawText("SPECIAL: Dawnfire Rune Blade [2-Hit Light Combo + Area Shockwave]", (int)col1X + 20, (int)sec2Y + 106, 11, Color{ 255, 175, 45, 255 });

        DrawText("Earned by defeating Zil's Fragment at the High Altar.", (int)col1X + 20, (int)sec2Y + 126, 11, Color{ 180, 210, 240, 220 });

    } else {

        DrawText("SPECIAL: Basic steel armament. Spared by Zil's Fragment.", (int)col1X + 20, (int)sec2Y + 106, 11, Color{ 160, 200, 240, 220 });

        DrawText("Carried into the borderlands to begin the journey.", (int)col1X + 20, (int)sec2Y + 126, 11, Color{ 150, 175, 200, 200 });

    }



    Rectangle chronoBox = { col2X, sec2Y + 20.0f, col2W, 200.0f };

    DrawRectangleRounded(chronoBox, 0.08f, 6, Color{ 18, 24, 36, 240 });

    DrawRectangleRoundedLines(chronoBox, 0.08f, 6, Color{ 80, 110, 160, 200 });



    int discoveredCount = 0;

    for (const auto& frag : player.GetInventory().GetFragments()) {

        if (frag.discovered) discoveredCount++;

    }



    DrawText("CHRONICLES OF TYDESHIRE // LORE PROGRESS", (int)col2X + 20, (int)sec2Y + 36, 12, Color{ 255, 205, 80, 255 });

    std::string fragScoreStr = "Discovered Fragments: " + std::to_string(discoveredCount) + " / 4";

    DrawText(fragScoreStr.c_str(), (int)col2X + 20, (int)sec2Y + 58, 18, Color{ 140, 220, 255, 255 });



    DrawText("Fragment I: The 500-Year Chaos - Uncovered", (int)col2X + 20, (int)sec2Y + 86, 11, Color{ 200, 225, 245, 240 });

    DrawText("Fragment II: The Five Realms of Charlz - Uncovered", (int)col2X + 20, (int)sec2Y + 104, 11, Color{ 200, 225, 245, 240 });

    

    bool hasFrag3 = player.GetInventory().GetFragments().size() > 2 && player.GetInventory().GetFragments()[2].discovered;

    DrawText(hasFrag3 ? "Fragment III: Zil's Holy Oath - Uncovered" : "Fragment III: Zil's Holy Oath - Shrouded [Defeat Zil's Fragment]", (int)col2X + 20, (int)sec2Y + 122, 11, hasFrag3 ? Color{ 255, 215, 80, 240 } : Color{ 130, 145, 170, 200 });



    DrawText("Tip: Press [I] anywhere in the world to open your Lore Journal.", (int)col2X + 20, (int)sec2Y + 154, 11, Color{ 255, 180, 70, 230 });



    int choiceResult = 0;



    Rectangle btnArena = { cardX + 160.0f, cardY + 550.0f, 340.0f, 54.0f };

    bool hoverArena = CheckCollisionPointRec(mousePos, btnArena);

    DrawRectangleRounded(btnArena, 0.2f, 6, hoverArena ? Color{ 40, 75, 115, 255 } : Color{ 24, 44, 72, 240 });

    DrawRectangleRoundedLines(btnArena, 0.2f, 6, hoverArena ? Color{ 140, 230, 255, 255 } : Color{ 80, 155, 220, 230 });

    int btn1W = MeasureText("[1] ENTER TEST ARENA", 15);

    DrawText("[1] ENTER TEST ARENA", (int)(btnArena.x + (btnArena.width - btn1W) * 0.5f), (int)btnArena.y + 19, 15, hoverArena ? Color{ 255, 255, 255, 255 } : Color{ 200, 240, 255, 240 });



    Rectangle btnHub = { cardX + 560.0f, cardY + 550.0f, 340.0f, 54.0f };

    bool hoverHub = CheckCollisionPointRec(mousePos, btnHub);

    DrawRectangleRounded(btnHub, 0.2f, 6, hoverHub ? Color{ 75, 45, 25, 255 } : Color{ 42, 26, 18, 240 });

    DrawRectangleRoundedLines(btnHub, 0.2f, 6, hoverHub ? Color{ 255, 190, 70, 255 } : Color{ 205, 125, 50, 230 });

    int btn2W = MeasureText("[2] BACK TO HUB", 15);

    DrawText("[2] BACK TO HUB", (int)(btnHub.x + (btnHub.width - btn2W) * 0.5f), (int)btnHub.y + 19, 15, hoverHub ? Color{ 255, 255, 255, 255 } : Color{ 255, 220, 110, 240 });



    if (isMousePressed) {

        if (hoverArena) {

            SoundFX::PlayButtonClick();

            choiceResult = 1;

        } else if (hoverHub) {

            SoundFX::PlayButtonClick();

            choiceResult = 2;

        }

    }



    return choiceResult;

}



bool ModalUI::DrawZyphdenTransition(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, float dt) {

    static float s_animTimer = 0.0f;

    s_animTimer += dt;



    static Texture2D s_customAsset = { 0 };

    static bool s_hasCheckedAsset = false;

    static bool s_customAssetLoaded = false;



    if (!s_hasCheckedAsset) {

        s_hasCheckedAsset = true;

        const char* assetCandidates[] = {

            "assets/zyphden_transition.png",

            "assets/transition_zyphden.png",

            "resources/zyphden_transition.png"

        };

        for (const char* path : assetCandidates) {

            if (FileExists(path)) {

                s_customAsset = LoadTexture(path);

                if (s_customAsset.id > 0) {

                    s_customAssetLoaded = true;

                    break;

                }

            }

        }

    }



    // Deep vignette & dark fantasy stone backdrop

    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 10, 15, 255 });



    // Subtle atmospheric dust particles

    for (int i = 0; i < 24; ++i) {

        float px = fmodf((float)i * 73.0f + s_animTimer * (15.0f + (float)(i % 5) * 8.0f), (float)windowWidth);

        float py = fmodf((float)i * 47.0f + sinf(s_animTimer * 0.8f + (float)i) * 30.0f, (float)windowHeight);

        float pAlpha = (sinf(s_animTimer * 1.5f + (float)i) * 0.5f + 0.5f) * 0.4f;

        DrawCircle((int)px, (int)py, 1.5f, ColorAlpha(Color{ 130, 190, 255, 255 }, pAlpha));

    }



    // Outer framing card

    float cardW = fminf((float)windowWidth - 80.0f, 1140.0f);

    float cardH = fminf((float)windowHeight - 80.0f, 760.0f);

    float cardX = ((float)windowWidth - cardW) * 0.5f;

    float cardY = ((float)windowHeight - cardH) * 0.5f;



    // Outer stone frame

    DrawRectangleRounded(Rectangle{ cardX, cardY, cardW, cardH }, 0.03f, 8, Color{ 15, 18, 27, 252 });

    DrawRectangleRoundedLines(Rectangle{ cardX, cardY, cardW, cardH }, 0.03f, 8, Color{ 70, 100, 145, 200 });

    DrawRectangleRoundedLines(Rectangle{ cardX + 4.0f, cardY + 4.0f, cardW - 8.0f, cardH - 8.0f }, 0.025f, 8, Color{ 30, 42, 62, 160 });



    // Ornate corner brackets

    Color goldBracket = Color{ 230, 185, 75, 230 };

    float bLen = 32.0f;

    // Top-Left

    DrawLineEx({ cardX + 12.0f, cardY + 12.0f }, { cardX + 12.0f + bLen, cardY + 12.0f }, 2.0f, goldBracket);

    DrawLineEx({ cardX + 12.0f, cardY + 12.0f }, { cardX + 12.0f, cardY + 12.0f + bLen }, 2.0f, goldBracket);

    // Top-Right

    DrawLineEx({ cardX + cardW - 12.0f, cardY + 12.0f }, { cardX + cardW - 12.0f - bLen, cardY + 12.0f }, 2.0f, goldBracket);

    DrawLineEx({ cardX + cardW - 12.0f, cardY + 12.0f }, { cardX + cardW - 12.0f, cardY + 12.0f + bLen }, 2.0f, goldBracket);

    // Bottom-Left

    DrawLineEx({ cardX + 12.0f, cardY + cardH - 12.0f }, { cardX + 12.0f + bLen, cardY + cardH - 12.0f }, 2.0f, goldBracket);

    DrawLineEx({ cardX + 12.0f, cardY + cardH - 12.0f }, { cardX + 12.0f, cardY + cardH - 12.0f - bLen }, 2.0f, goldBracket);

    // Bottom-Right

    DrawLineEx({ cardX + cardW - 12.0f, cardY + cardH - 12.0f }, { cardX + cardW - 12.0f - bLen, cardY + cardH - 12.0f }, 2.0f, goldBracket);

    DrawLineEx({ cardX + cardW - 12.0f, cardY + cardH - 12.0f }, { cardX + cardW - 12.0f, cardY + cardH - 12.0f - bLen }, 2.0f, goldBracket);



    // Top Header

    float headerY = cardY + 24.0f;

    const char* actTag = "CHAPTER 1  //  ACT III";

    int actW = FontManager::MeasureText(actTag, 13, GameFont::TITLE);

    FontManager::DrawText(actTag, (int)(cardX + (cardW - actW) * 0.5f), (int)headerY, 13, Color{ 255, 205, 80, 240 }, GameFont::TITLE);



    const char* realmTitle = "THE FORTIFIED REALM OF ZYPHDEN";

    int titleW = FontManager::MeasureText(realmTitle, 26, GameFont::TITLE);

    FontManager::DrawText(realmTitle, (int)(cardX + (cardW - titleW) * 0.5f), (int)headerY + 22, 26, Color{ 240, 246, 255, 255 }, GameFont::TITLE);



    const char* realmSubtitle = "Guardian of the Gale Winds  •  Seat of King Kreptu's Court";

    int subW = FontManager::MeasureText(realmSubtitle, 13);

    FontManager::DrawText(realmSubtitle, (int)(cardX + (cardW - subW) * 0.5f), (int)headerY + 52, 13, Color{ 130, 175, 225, 210 });



    DrawLine((int)cardX + 40, (int)headerY + 74, (int)(cardX + cardW - 40), (int)headerY + 74, Color{ 48, 68, 98, 180 });



    // Central Artwork Presentation Box (Aspect ~16:9)

    float artW = cardW - 80.0f;

    float artH = 390.0f;

    float artX = cardX + 40.0f;

    float artY = headerY + 86.0f;



    Rectangle artRec = { artX, artY, artW, artH };



    if (s_customAssetLoaded && s_customAsset.id > 0) {

        // Draw user's custom transition texture

        DrawRectangleRec(artRec, Color{ 6, 8, 12, 255 });

        float scale = fminf(artW / (float)s_customAsset.width, artH / (float)s_customAsset.height);

        float destW = (float)s_customAsset.width * scale;

        float destH = (float)s_customAsset.height * scale;

        float destX = artX + (artW - destW) * 0.5f;

        float destY = artY + (artH - destH) * 0.5f;

        DrawTexturePro(s_customAsset,

                       Rectangle{ 0.0f, 0.0f, (float)s_customAsset.width, (float)s_customAsset.height },

                       Rectangle{ destX, destY, destW, destH },

                       Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);

        DrawRectangleLinesEx(artRec, 2.0f, Color{ 100, 160, 240, 200 });

    } else {

        // High-Quality Atmospheric Transition Screen Placeholder

        DrawRectangleRounded(artRec, 0.02f, 6, Color{ 10, 14, 22, 250 });

        DrawRectangleRoundedLines(artRec, 0.02f, 6, Color{ 55, 85, 128, 200 });



        // Inner dashed guide line simulation

        DrawRectangleLinesEx(Rectangle{ artX + 12.0f, artY + 12.0f, artW - 24.0f, artH - 24.0f }, 1.0f, Color{ 35, 52, 78, 140 });



        // Crosshairs & center indicator

        float cx = artX + artW * 0.5f;

        float cy = artY + artH * 0.5f;

        DrawLine((int)cx - 40, (int)cy, (int)cx + 40, (int)cy, Color{ 60, 95, 145, 160 });

        DrawLine((int)cx, (int)cy - 40, (int)cx, (int)cy + 40, Color{ 60, 95, 145, 160 });

        DrawCircleLines((int)cx, (int)cy, 28.0f, Color{ 80, 130, 190, 180 });



        // Placeholder Badge and Instructions

        const char* phTitle = "[ TRANSITION SCREEN ARTWORK ASSET PLACEHOLDER ]";

        int phW = FontManager::MeasureText(phTitle, 16);

        FontManager::DrawText(phTitle, (int)(cx - phW * 0.5f), (int)cy - 65, 16, Color{ 120, 200, 255, 240 });



        const char* phDrop = "Drop custom transition illustration into: assets/zyphden_transition.png";

        int dropW = FontManager::MeasureText(phDrop, 13);

        FontManager::DrawText(phDrop, (int)(cx - dropW * 0.5f), (int)cy + 55, 13, Color{ 220, 230, 245, 230 });



        const char* phSpecs = "Supported format: 1920x1080 (16:9) or 1280x720 PNG / JPG (Auto-scaled)";

        int specsW = FontManager::MeasureText(phSpecs, 11);

        FontManager::DrawText(phSpecs, (int)(cx - specsW * 0.5f), (int)cy + 75, 11, Color{ 130, 165, 205, 190 });



        const char* phNote = "The engine will automatically detect and render your illustration here upon load.";

        int noteW = FontManager::MeasureText(phNote, 11);

        FontManager::DrawText(phNote, (int)(cx - noteW * 0.5f), (int)cy + 95, 11, Color{ 255, 215, 80, 200 });

    }



    // Flavor Narrative Lore Box

    float loreY = artY + artH + 16.0f;

    std::string realmLore =

        "Behind towering wind-carved limestone ramparts and iron portcullises lies the sovereign city of Zyphden. "

        "Within its cobbled wards and hearthside taverns, the Royal Vanguard maintains an unyielding vigil against the madness festering across Tydeshire.";

    DrawJournalWrappedText(realmLore, (int)cardX + 45, (int)loreY, (int)cardW - 90, 13, 6, Color{ 210, 222, 238, 230 });



    // Bottom Action / Confirmation Button

    float btnW = 380.0f;

    float btnH = 50.0f;

    float btnX = cardX + (cardW - btnW) * 0.5f;

    float btnY = cardY + cardH - 68.0f;



    Rectangle btnRec = { btnX, btnY, btnW, btnH };

    bool isHover = CheckCollisionPointRec(mousePos, btnRec);



    float pulse = (sinf(s_animTimer * 4.0f) * 0.5f + 0.5f);

    Color btnBg = isHover ? Color{ 40, 75, 120, 255 } : Color{ 24, 40, 68, 230 };

    Color btnBorder = isHover ? Color{ 160, 225, 255, 255 } : ColorAlpha(Color{ 100, 165, 240, 230 }, 0.7f + 0.3f * pulse);



    DrawRectangleRounded(btnRec, 0.2f, 6, btnBg);

    DrawRectangleRoundedLines(btnRec, 0.2f, 6, btnBorder);



    const char* btnLabel = "ENTER CITY GATES [SPACE / ENTER / CLICK]";

    int btnLabelW = FontManager::MeasureText(btnLabel, 14, GameFont::TITLE);

    FontManager::DrawText(btnLabel, (int)(btnX + (btnW - btnLabelW) * 0.5f), (int)btnY + 17, 14, isHover ? Color{ 255, 255, 255, 255 } : Color{ 220, 240, 255, 240 }, GameFont::TITLE);



    bool confirmed = (isHover && isMousePressed) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_E);

    if (confirmed) {

        SoundFX::PlayButtonClick();

        return true;

    }



    return false;

}



bool ModalUI::DrawQuarryTransition(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, float dt) {

    static float s_quarryAnimTimer = 0.0f;

    s_quarryAnimTimer += dt;



    static bool s_quarryCustomLoaded = false;

    static Texture2D s_quarryCustomAsset = { 0 };

    static bool s_quarryAssetChecked = false;



    if (!s_quarryAssetChecked) {

        s_quarryAssetChecked = true;

        const char* potentialPaths[] = {

            "assets/quarry_transition.png",

            "assets/quarry_transition.jpg",

            "resources/quarry_transition.png"

        };

        for (const char* p : potentialPaths) {

            if (FileExists(p)) {

                s_quarryCustomAsset = LoadTexture(p);

                if (s_quarryCustomAsset.id > 0) {

                    SetTextureFilter(s_quarryCustomAsset, TEXTURE_FILTER_BILINEAR);

                    s_quarryCustomLoaded = true;

                    break;

                }

            }

        }

    }



    // 1. Dark Vignette Atmospheric Backdrop

    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 10, 8, 14, 245 });



    // 2. Large Central Ornate Card

    float cardW = fminf((float)windowWidth - 80.0f, 1140.0f);

    float cardH = fminf((float)windowHeight - 80.0f, 760.0f);

    float cardX = ((float)windowWidth - cardW) * 0.5f;

    float cardY = ((float)windowHeight - cardH) * 0.5f;



    Rectangle cardRec = { cardX, cardY, cardW, cardH };



    // Outer backing card & filigree borders

    DrawRectangleRounded(cardRec, 0.03f, 8, Color{ 16, 14, 22, 252 });

    DrawRectangleRoundedLines(cardRec, 0.03f, 8, Color{ 225, 175, 55, 240 });

    DrawRectangleRoundedLines(Rectangle{ cardX + 4, cardY + 4, cardW - 8, cardH - 8 }, 0.025f, 8, Color{ 65, 50, 85, 180 });



    // Amber & bronze corner filigree accents

    Color cornerCol = Color{ 255, 195, 60, 240 };

    DrawLineEx({ cardX + 16, cardY + 16 }, { cardX + 46, cardY + 16 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + 16, cardY + 16 }, { cardX + 16, cardY + 46 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + cardW - 16, cardY + 16 }, { cardX + cardW - 46, cardY + 16 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + cardW - 16, cardY + 16 }, { cardX + cardW - 16, cardY + 46 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + 16, cardY + cardH - 16 }, { cardX + 46, cardY + cardH - 16 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + 16, cardY + cardH - 16 }, { cardX + 16, cardY + cardH - 46 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + cardW - 16, cardY + cardH - 16 }, { cardX + cardW - 46, cardY + cardH - 16 }, 2.0f, cornerCol);

    DrawLineEx({ cardX + cardW - 16, cardY + cardH - 16 }, { cardX + cardW - 16, cardY + cardH - 46 }, 2.0f, cornerCol);



    // Header Title

    float headerY = cardY + 28.0f;

    const char* realmTitle = "THE EASTERN QUARRY & BASALT DEPTHS";

    int titleW = FontManager::MeasureText(realmTitle, 26, GameFont::TITLE);

    FontManager::DrawText(realmTitle, (int)(cardX + (cardW - titleW) * 0.5f), (int)headerY, 26, Color{ 255, 215, 80, 255 }, GameFont::TITLE);



    const char* realmSubtitle = "Subterranean Mines of Ancient Titans and Fabled Ore Veins";

    int subW = FontManager::MeasureText(realmSubtitle, 13);

    FontManager::DrawText(realmSubtitle, (int)(cardX + (cardW - subW) * 0.5f), (int)headerY + 48, 13, Color{ 210, 185, 130, 210 });



    DrawLine((int)cardX + 40, (int)headerY + 70, (int)(cardX + cardW - 40), (int)headerY + 70, Color{ 85, 68, 48, 180 });



    // Central Artwork Presentation Box (Aspect ~16:9)

    float artW = cardW - 80.0f;

    float artH = 390.0f;

    float artX = cardX + 40.0f;

    float artY = headerY + 82.0f;



    Rectangle artRec = { artX, artY, artW, artH };



    if (s_quarryCustomLoaded && s_quarryCustomAsset.id > 0) {

        DrawRectangleRec(artRec, Color{ 8, 6, 12, 255 });

        float scale = fminf(artW / (float)s_quarryCustomAsset.width, artH / (float)s_quarryCustomAsset.height);

        float destW = (float)s_quarryCustomAsset.width * scale;

        float destH = (float)s_quarryCustomAsset.height * scale;

        float destX = artX + (artW - destW) * 0.5f;

        float destY = artY + (artH - destH) * 0.5f;

        DrawTexturePro(s_quarryCustomAsset,

                       Rectangle{ 0.0f, 0.0f, (float)s_quarryCustomAsset.width, (float)s_quarryCustomAsset.height },

                       Rectangle{ destX, destY, destW, destH },

                       Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);

        DrawRectangleLinesEx(artRec, 2.0f, Color{ 225, 175, 55, 200 });

    } else {

        // High-Quality Atmospheric Quarry Transition Screen Placeholder

        DrawRectangleRounded(artRec, 0.02f, 6, Color{ 14, 12, 20, 250 });

        DrawRectangleRoundedLines(artRec, 0.02f, 6, Color{ 95, 75, 50, 200 });

        DrawRectangleLinesEx(Rectangle{ artX + 12.0f, artY + 12.0f, artW - 24.0f, artH - 24.0f }, 1.0f, Color{ 65, 50, 35, 140 });



        float cx = artX + artW * 0.5f;

        float cy = artY + artH * 0.5f;

        DrawLine((int)cx - 40, (int)cy, (int)cx + 40, (int)cy, Color{ 160, 120, 50, 160 });

        DrawLine((int)cx, (int)cy - 40, (int)cx, (int)cy + 40, Color{ 160, 120, 50, 160 });

        DrawCircleLines((int)cx, (int)cy, 28.0f, Color{ 200, 150, 60, 180 });



        const char* phTitle = "[ QUARRY DEPTHS TRANSITION ARTWORK PLACEHOLDER ]";

        int phW = FontManager::MeasureText(phTitle, 16);

        FontManager::DrawText(phTitle, (int)(cx - phW * 0.5f), (int)cy - 65, 16, Color{ 255, 215, 80, 240 });



        const char* phDrop = "Drop custom transition illustration into: assets/quarry_transition.png";

        int dropW = FontManager::MeasureText(phDrop, 13);

        FontManager::DrawText(phDrop, (int)(cx - dropW * 0.5f), (int)cy + 55, 13, Color{ 240, 225, 205, 230 });



        const char* phSpecs = "Supported format: 1920x1080 (16:9) or 1280x720 PNG / JPG (Auto-scaled)";

        int specsW = FontManager::MeasureText(phSpecs, 11);

        FontManager::DrawText(phSpecs, (int)(cx - specsW * 0.5f), (int)cy + 75, 11, Color{ 185, 160, 120, 190 });



        const char* phNote = "The engine will automatically detect and render your illustration here upon load.";

        int noteW = FontManager::MeasureText(phNote, 11);

        FontManager::DrawText(phNote, (int)(cx - noteW * 0.5f), (int)cy + 95, 11, Color{ 255, 190, 60, 200 });

    }



    // Flavor Narrative Lore Box

    float loreY = artY + artH + 16.0f;

    std::string realmLore =

        "Beneath the jagged limestone ridges east of Zyphden lie the ancestral Quarry Depths. Ancient miners unearthered primordial ore crystals here before dark tremors woke the stone itself. Deep within the deepest mine shaft, an ancient elemental altar stands dormant, vibrating in resonance with forgotten lore fragments.";

    DrawJournalWrappedText(realmLore, (int)cardX + 45, (int)loreY, (int)cardW - 90, 13, 6, Color{ 235, 222, 210, 230 });



    // Bottom Action / Confirmation Button

    float btnW = 420.0f;

    float btnH = 50.0f;

    float btnX = cardX + (cardW - btnW) * 0.5f;

    float btnY = cardY + cardH - 68.0f;



    Rectangle btnRec = { btnX, btnY, btnW, btnH };

    bool isHover = CheckCollisionPointRec(mousePos, btnRec);



    float pulse = (sinf(s_quarryAnimTimer * 4.0f) * 0.5f + 0.5f);

    Color btnBg = isHover ? Color{ 85, 55, 20, 255 } : Color{ 50, 32, 12, 230 };

    Color btnBorder = isHover ? Color{ 255, 225, 95, 255 } : ColorAlpha(Color{ 225, 175, 55, 230 }, 0.7f + 0.3f * pulse);



    DrawRectangleRounded(btnRec, 0.2f, 6, btnBg);

    DrawRectangleRoundedLines(btnRec, 0.2f, 6, btnBorder);



    const char* btnLabel = "DESCEND INTO QUARRY MINES [SPACE / ENTER / CLICK]";

    int btnLabelW = FontManager::MeasureText(btnLabel, 13, GameFont::TITLE);

    FontManager::DrawText(btnLabel, (int)(btnX + (btnW - btnLabelW) * 0.5f), (int)btnY + 17, 13, isHover ? Color{ 255, 255, 255, 255 } : Color{ 255, 235, 160, 240 }, GameFont::TITLE);



    bool confirmed = (isHover && isMousePressed) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_E);

    if (confirmed) {

        SoundFX::PlayButtonClick();

        return true;

    }



    return false;

}



bool ModalUI::DrawMapModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player, GameState currentState) {
    // 1. Dark Frosted Vignette Backdrop
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 4, 10, 14, 235 });

    const float modalW = 1140.0f;
    const float modalH = 650.0f;
    const float modalX = ((float)windowWidth - modalW) * 0.5f;
    const float modalY = ((float)windowHeight - modalH) * 0.5f;
    Rectangle modalRec = { modalX, modalY, modalW, modalH };

    // Outer frosted teal glass backing
    DrawRectangleRounded(modalRec, 0.03f, 8, Color{ 7, 18, 24, 252 });
    DrawRectangleRoundedLines(modalRec, 0.03f, 8, Color{ 45, 180, 200, 230 });
    DrawRectangleRoundedLines(Rectangle{ modalX + 4.0f, modalY + 4.0f, modalW - 8.0f, modalH - 8.0f }, 0.025f, 8, Color{ 20, 75, 90, 180 });

    // Corner Filigree & Brackets (Image 2 style)
    Color cornerCol = Color{ 80, 225, 245, 240 };
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 38, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 12, modalY + 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 38, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 12, modalY + 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 38, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 12, modalY + modalH - 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 38, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 12, modalY + modalH - 38 }, 2.0f, cornerCol);

    // Subtle Cartesian / Isometric Grid overlay lines (Image 2)
    for (int gx = 0; gx < (int)modalW; gx += 40) {
        DrawLine((int)modalX + gx, (int)modalY + 6, (int)modalX + gx, (int)modalY + (int)modalH - 6, Color{ 20, 60, 75, 35 });
    }
    for (int gy = 0; gy < (int)modalH; gy += 40) {
        DrawLine((int)modalX + 6, (int)modalY + gy, (int)modalX + (int)modalW - 6, (int)modalY + gy, Color{ 20, 60, 75, 35 });
    }

    // 2. HEADER SECTION (Image 2: Decorative wing lines, Game Stage title, 3 stars)
    float headerX = modalX + 48.0f;
    float headerY = modalY + 28.0f;

    // Wing lines
    DrawLineEx({ headerX, headerY + 14.0f }, { headerX + 45.0f, headerY + 14.0f }, 1.5f, Color{ 140, 215, 235, 180 });
    DrawCircle((int)headerX + 45, (int)headerY + 14, 3.0f, Color{ 140, 215, 235, 220 });
    DrawText("Game Stage", (int)headerX + 60, (int)headerY, 26, Color{ 255, 255, 255, 255 });
    int stageW = MeasureText("Game Stage", 26);
    DrawCircle((int)headerX + 60 + stageW + 15, (int)headerY + 14, 3.0f, Color{ 140, 215, 235, 220 });
    DrawLineEx({ headerX + 60 + stageW + 15.0f, headerY + 14.0f }, { headerX + 60 + stageW + 60.0f, headerY + 14.0f }, 1.5f, Color{ 140, 215, 235, 180 });

    // Three Glowing Stars (Image 2)
    float starX = headerX + 90.0f;
    float starY = headerY + 36.0f;
    DrawText("***", (int)starX, (int)starY, 22, Color{ 255, 220, 80, 255 });

    // Subtitle
    DrawText("Explore Game Stages of Season 4 before your adventure through the world of Gaia begins.", (int)headerX, (int)starY + 26, 10, Color{ 140, 185, 205, 200 });

    // Top Right: Compass & Close Button
    float closeW = 95.0f;
    float closeH = 28.0f;
    Rectangle closeRec = { modalX + modalW - closeW - 32.0f, modalY + 24.0f, closeW, closeH };
    bool isCloseHov = CheckCollisionPointRec(mousePos, closeRec);
    DrawRectangleRounded(closeRec, 0.25f, 4, isCloseHov ? Color{ 32, 70, 85, 255 } : Color{ 14, 32, 42, 220 });
    DrawRectangleRoundedLines(closeRec, 0.25f, 4, isCloseHov ? Color{ 80, 225, 245, 255 } : Color{ 40, 100, 120, 200 });
    DrawText("CLOSE [M]", (int)closeRec.x + 16, (int)closeRec.y + 8, 11, isCloseHov ? Color{ 255, 255, 255, 255 } : Color{ 180, 225, 240, 220 });
    bool shouldClose = (isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M);

    // 3. TACTICAL DIAMOND WAYPOINTS (<1> - <5>) & LEYLINE MAP
    struct MapStage { int id; const char* name; Vector2 pos; const char* threat; const char* minLvl; const char* lore; };
    MapStage stages[5] = {
        { 1, "STAGE #1: RUINED CASTLE", { modalX + 310.0f, modalY + 510.0f }, "Normal", "Min Level: 1", "At the beginning, run the foundations for playing via the newest patches. Dangerous foes are available and relevant leylines will lead all." },
        { 2, "STAGE #2: CROSSROADS", { modalX + 590.0f, modalY + 420.0f }, "Moderate", "Min Level: 5", "A sprawling highway crossing southern pines. Bandits ambush supply lines while ancient stone shrines whisper of Solsden." },
        { 3, "STAGE #3: ZYPHDEN KEEP", { modalX + 720.0f, modalY + 250.0f }, "High", "Min Level: 15", "The fortified royal citadel. King Kreptu watches from the throne dais as Chancellor Malakor and forge masters guard the flame." },
        { 4, "STAGE #4: QUARRY MINES", { modalX + 860.0f, modalY + 380.0f }, "Severe", "Min Level: 20", "Subterranean basalt shafts overrun by rogue ore golems. Mining picks strike primordial veins of ancient crystal." },
        { 5, "STAGE #5: NORTH MARCHE", { modalX + 560.0f, modalY + 575.0f }, "Lethal", "Min Level: 30", "Alpine snowy mountain passes guarding the sealed gate toward the northern mist and the Liege of Kings." }
    };

    static int s_selectedStage = 1;

    // Determine which stage is active based on currentState
    int activeStageId = 1;
    if (currentState == GameState::RUINED_CASTLE) activeStageId = 1;
    else if (currentState == GameState::CROSSROADS) activeStageId = 2;
    else if ((currentState == GameState::ZYPHDEN_REALM || currentState == GameState::ZYPHDEN_TRANSITION)) activeStageId = 3;

    // Draw connecting leylines between diamond waypoints (Image 2)
    Color leylineCol = Color{ 40, 140, 160, 180 };
    DrawLineEx(stages[0].pos, stages[1].pos, 2.5f, leylineCol);
    DrawLineEx(stages[1].pos, stages[2].pos, 2.5f, leylineCol);
    DrawLineEx(stages[2].pos, stages[3].pos, 2.5f, leylineCol);
    DrawLineEx(stages[1].pos, stages[4].pos, 2.0f, Color{ 35, 110, 130, 150 });

    // Draw Diamond Waypoints (<1> - <5>)
    float animTime = (float)GetTime();
    for (int i = 0; i < 5; i++) {
        Vector2 pt = stages[i].pos;
        bool isActive = (stages[i].id == activeStageId);
        bool isSel = (stages[i].id == s_selectedStage);

        // Check hover/click
        Rectangle clickBox = { pt.x - 22.0f, pt.y - 22.0f, 44.0f, 44.0f };
        bool isNodeHov = CheckCollisionPointRec(mousePos, clickBox);
        if (isNodeHov && isMousePressed) {
            s_selectedStage = stages[i].id;
            SoundFX::PlayButtonClick();
        }

        // Active Player Beacon Pulse Rings (Image 2 cyan diamond <1>)
        if (isActive) {
            float pulse = sinf(animTime * 3.5f) * 6.0f;
            DrawPolyLines(pt, 4, 30.0f + pulse, 45.0f, Color{ 0, 229, 255, 120 });
            DrawPolyLines(pt, 4, 24.0f, 45.0f, Color{ 80, 240, 255, 200 });
            DrawPoly(pt, 4, 20.0f, 45.0f, Color{ 0, 170, 210, 240 });
        } else {
            DrawPoly(pt, 4, 18.0f, 45.0f, (isNodeHov || isSel) ? Color{ 25, 55, 70, 240 } : Color{ 14, 32, 42, 220 });
            DrawPolyLines(pt, 4, 18.0f, 45.0f, (isNodeHov || isSel) ? Color{ 255, 225, 90, 255 } : Color{ 70, 160, 180, 200 });
        }

        // Diamond arrows / brackets (Image 2 style: < 1 >)
        DrawLineEx({ pt.x - 24.0f, pt.y }, { pt.x - 18.0f, pt.y - 6.0f }, 1.5f, Color{ 160, 225, 240, 220 });
        DrawLineEx({ pt.x - 24.0f, pt.y }, { pt.x - 18.0f, pt.y + 6.0f }, 1.5f, Color{ 160, 225, 240, 220 });
        DrawLineEx({ pt.x + 24.0f, pt.y }, { pt.x + 18.0f, pt.y - 6.0f }, 1.5f, Color{ 160, 225, 240, 220 });
        DrawLineEx({ pt.x + 24.0f, pt.y }, { pt.x + 18.0f, pt.y + 6.0f }, 1.5f, Color{ 160, 225, 240, 220 });

        std::string numStr = std::to_string(stages[i].id);
        int nw = MeasureText(numStr.c_str(), 14);
        DrawText(numStr.c_str(), (int)(pt.x - nw * 0.5f), (int)(pt.y - 7.0f), 14, isActive ? Color{ 255, 255, 255, 255 } : Color{ 220, 240, 255, 240 });
    }

    // 4. FLOATING STAGE INSPECTION CARD (Image 2: Left panel STAGE #1 with details)
    float cardX = modalX + 50.0f;
    float cardY = modalY + 140.0f;
    float cardW = 340.0f;
    float cardH = 340.0f;
    Rectangle cardRec = { cardX, cardY, cardW, cardH };

    DrawRectangleRounded(cardRec, 0.05f, 6, Color{ 10, 24, 32, 245 });
    DrawRectangleRoundedLines(cardRec, 0.05f, 6, Color{ 45, 125, 145, 200 });

    MapStage curStage = stages[s_selectedStage - 1];

    // Card Header: STAGE #X
    DrawText(curStage.name, (int)cardX + 22, (int)cardY + 22, 16, Color{ 255, 255, 255, 255 });

    // Two capability / reward icon badges (Image 2)
    float badge1X = cardX + cardW - 86.0f;
    float badge2X = cardX + cardW - 46.0f;
    float badgeY = cardY + 18.0f;
    Rectangle b1 = { badge1X, badgeY, 32.0f, 32.0f };
    Rectangle b2 = { badge2X, badgeY, 32.0f, 32.0f };
    DrawRectangleRounded(b1, 0.2f, 4, Color{ 20, 50, 75, 255 });
    DrawRectangleRoundedLines(b1, 0.2f, 4, Color{ 80, 200, 255, 220 });
    DrawCircleLines((int)b1.x + 16, (int)b1.y + 16, 8.0f, Color{ 80, 200, 255, 255 });
    DrawRectangleRounded(b2, 0.2f, 4, Color{ 50, 35, 20, 255 });
    DrawRectangleRoundedLines(b2, 0.2f, 4, Color{ 255, 180, 60, 220 });
    DrawPolyLines({ b2.x + 16.0f, b2.y + 16.0f }, 4, 8.0f, 45.0f, Color{ 255, 180, 60, 255 });

    // Stats block
    DrawText(curStage.minLvl, (int)cardX + 22, (int)cardY + 54, 11, Color{ 160, 200, 225, 200 });
    std::string thStr = "Threat: " + std::string(curStage.threat);
    DrawText(thStr.c_str(), (int)cardX + 22, (int)cardY + 72, 11, Color{ 160, 200, 225, 200 });
    DrawText("Fragments: 4/10 Uncovered", (int)cardX + 22, (int)cardY + 90, 11, Color{ 160, 200, 225, 200 });

    // Lore Description
    DrawLine((int)cardX + 22, (int)cardY + 116, (int)(cardX + cardW - 22), (int)cardY + 116, Color{ 30, 75, 95, 180 });
    DrawText(curStage.lore, (int)cardX + 22, (int)cardY + 128, 10, Color{ 180, 215, 235, 220 });

    // [ DISCOVER MORE ] Button (Image 2)
    float discW = 180.0f;
    float discH = 34.0f;
    Rectangle discRec = { cardX + (cardW - discW) * 0.5f, cardY + cardH - 52.0f, discW, discH };
    bool isDiscHov = CheckCollisionPointRec(mousePos, discRec);
    DrawRectangleRounded(discRec, 0.2f, 4, isDiscHov ? Color{ 35, 75, 95, 255 } : Color{ 18, 42, 54, 220 });
    DrawRectangleRoundedLines(discRec, 0.2f, 4, isDiscHov ? Color{ 80, 225, 245, 255 } : Color{ 45, 110, 130, 200 });
    DrawText("DISCOVER MORE", (int)discRec.x + 32, (int)discRec.y + 11, 11, isDiscHov ? Color{ 255, 255, 255, 255 } : Color{ 190, 225, 240, 220 });

    // Cycle stage arrows on card edges: < and >
    Rectangle leftArrow = { cardX - 28.0f, cardY + cardH * 0.5f - 18.0f, 24.0f, 36.0f };
    Rectangle rightArrow = { cardX + cardW + 4.0f, cardY + cardH * 0.5f - 18.0f, 24.0f, 36.0f };
    if (CheckCollisionPointRec(mousePos, leftArrow) && isMousePressed) {
        s_selectedStage = (s_selectedStage <= 1) ? 5 : (s_selectedStage - 1);
        SoundFX::PlayButtonClick();
    }
    if (CheckCollisionPointRec(mousePos, rightArrow) && isMousePressed) {
        s_selectedStage = (s_selectedStage >= 5) ? 1 : (s_selectedStage + 1);
        SoundFX::PlayButtonClick();
    }
    DrawText("<", (int)leftArrow.x + 8, (int)leftArrow.y + 8, 20, Color{ 80, 220, 245, 220 });
    DrawText(">", (int)rightArrow.x + 8, (int)rightArrow.y + 8, 20, Color{ 80, 220, 245, 220 });

    // 5. CALL TO ACTION BUTTON (Image 2: Hexagonal angled banner [ PLAY NOW ] on bottom right)
    float playW = 200.0f;
    float playH = 44.0f;
    Rectangle playRec = { modalX + modalW - playW - 48.0f, modalY + modalH - playH - 36.0f, playW, playH };
    bool isPlayHov = CheckCollisionPointRec(mousePos, playRec);
    if ((isPlayHov && isMousePressed) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        SoundFX::PlayButtonClick();
        return true; // Closes map modal and resumes journey!
    }

    // Angled / Hexagonal button shape (Image 2)
    Vector2 p1 = { playRec.x + 14.0f, playRec.y };
    Vector2 p2 = { playRec.x + playW - 14.0f, playRec.y };
    Vector2 p3 = { playRec.x + playW, playRec.y + playH * 0.5f };
    Vector2 p4 = { playRec.x + playW - 14.0f, playRec.y + playH };
    Vector2 p5 = { playRec.x + 14.0f, playRec.y + playH };
    Vector2 p6 = { playRec.x, playRec.y + playH * 0.5f };

    DrawTriangle(p1, p2, p3, isPlayHov ? Color{ 180, 110, 60, 255 } : Color{ 130, 75, 40, 230 });
    DrawTriangle(p1, p3, p6, isPlayHov ? Color{ 180, 110, 60, 255 } : Color{ 130, 75, 40, 230 });
    DrawTriangle(p6, p3, p4, isPlayHov ? Color{ 180, 110, 60, 255 } : Color{ 130, 75, 40, 230 });
    DrawTriangle(p6, p4, p5, isPlayHov ? Color{ 180, 110, 60, 255 } : Color{ 130, 75, 40, 230 });

    DrawLineEx(p1, p2, 2.0f, Color{ 255, 190, 120, 255 });
    DrawLineEx(p2, p3, 2.0f, Color{ 255, 190, 120, 255 });
    DrawLineEx(p3, p4, 2.0f, Color{ 255, 190, 120, 255 });
    DrawLineEx(p4, p5, 2.0f, Color{ 255, 190, 120, 255 });
    DrawLineEx(p5, p6, 2.0f, Color{ 255, 190, 120, 255 });
    DrawLineEx(p6, p1, 2.0f, Color{ 255, 190, 120, 255 });

    int ptw = MeasureText("RESUME JOURNEY", 14);
    DrawText("RESUME JOURNEY", (int)(playRec.x + (playW - ptw) * 0.5f), (int)(playRec.y + 15.0f), 14, Color{ 255, 255, 255, 255 });

    if (shouldClose) {
        SoundFX::PlayButtonClick();
        return true;
    }

    return false;
}

