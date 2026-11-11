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
#include <iterator>
#include "../core/GameConstants.hpp"



static void DrawJournalWrappedText(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color) {
    FontManager::DrawTextWrapped(text, posX, posY, maxLineWidth, fontSize, lineSpacing, color, GameFont::BODY);
}

static std::string ToRomanNumeral(int value) {
    if (value <= 0) return std::to_string(value);
    static const struct {
        int val;
        const char* sym;
    } romanTable[] = {
        { 1000, "M" }, { 900, "CM" }, { 500, "D" }, { 400, "CD" },
        { 100, "C" },  { 90, "XC" },  { 50, "L" },  { 40, "XL" },
        { 10, "X" },   { 9, "IX" },   { 5, "V" },   { 4, "IV" },
        { 1, "I" }
    };
    std::string result;
    for (const auto& item : romanTable) {
        while (value >= item.val) {
            result += item.sym;
            value -= item.val;
        }
    }
    return result;
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
            { "E Key",         "Interact/Exec", "Talk to NPCs, trigger Visceral Execution on staggered foes.", Color{ 200, 150, 255, 255 } },
            { "TAB / I Key",   "Inventory", "Open Equipment Matrix, 3-Slot Weapon Hotbar, Chronicles & Bestiary.", Color{ 255, 215, 80, 255 } },
            { "B Key",         "Bestiary",  "Open the Humoral Bestiary & Monster Weakness Codex directly.", Color{ 140, 210, 255, 255 } },
            { "1 - 4 Keys",     "Quick Belt", "Drink Health Potion [1], Stoneskin [2], Glacial Thaw [3], Berserk [4].", Color{ 140, 240, 180, 255 } },
            { "C Key",         "Controls", "Toggle this Controls Menu modal at any time.", Color{ 160, 180, 200, 255 } },
            { "R / L / ESC",   "Menu Keys","[R] Respawn/Reset  |  [L] Return to Lobby  |  [ESC] Title Screen.", Color{ 140, 160, 180, 255 } }
        };

        float rowHeight = 38.0f;
        for (int i = 0; i < (int)std::size(items); i++) {
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

        for (int i = 0; i < (int)std::size(vcs); i++) {
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




static void DrawWeaponSlotIcon(Vector2 center, WeaponType weapon, float scale, bool isUnlocked, bool isEquipped) {
    Color baseCol = isUnlocked ? (isEquipped ? Color{ 255, 235, 140, 255 } : Color{ 220, 235, 255, 255 }) : Color{ 90, 105, 120, 180 };
    Color glowCol = isUnlocked ? (isEquipped ? Color{ 255, 205, 50, 200 } : Color{ 80, 160, 240, 160 }) : Color{ 50, 60, 75, 120 };

    switch (weapon) {
        case WeaponType::SWORD:
            AssetManager::DrawSword({ center.x - 9.0f * scale, center.y + 9.0f * scale }, -45.0f, 24.0f * scale, baseCol, glowCol);
            break;
        case WeaponType::DAGGERS:
            AssetManager::DrawDaggers({ center.x - 5.0f * scale, center.y }, { center.x + 5.0f * scale, center.y }, -60.0f, -120.0f, 
                isUnlocked ? Color{ 180, 255, 220, 255 } : baseCol,
                isUnlocked ? Color{ 60, 240, 180, 180 } : glowCol);
            break;
        case WeaponType::MACE:
            AssetManager::DrawMace({ center.x - 8.0f * scale, center.y + 8.0f * scale }, -45.0f, 22.0f * scale,
                isUnlocked ? Color{ 235, 180, 120, 255 } : baseCol,
                isUnlocked ? Color{ 255, 140, 40, 180 } : glowCol);
            break;
        case WeaponType::HALBERD:
            AssetManager::DrawHalberd({ center.x - 10.0f * scale, center.y + 10.0f * scale }, -45.0f, 26.0f * scale,
                isUnlocked ? Color{ 230, 240, 255, 255 } : baseCol,
                isUnlocked ? Color{ 140, 200, 255, 180 } : glowCol);
            break;
        case WeaponType::TRUTH_SEEKER:
            AssetManager::DrawSword({ center.x - 10.0f * scale, center.y + 10.0f * scale }, -45.0f, 26.0f * scale,
                isUnlocked ? Color{ 255, 235, 140, 255 } : baseCol,
                isUnlocked ? Color{ 255, 200, 50, 220 } : glowCol);
            break;
        case WeaponType::ZEPHYR_DAGGERS:
            AssetManager::DrawDaggers({ center.x - 5.0f * scale, center.y }, { center.x + 5.0f * scale, center.y }, -60.0f, -120.0f,
                isUnlocked ? Color{ 110, 245, 220, 255 } : baseCol,
                isUnlocked ? Color{ 40, 210, 180, 200 } : glowCol);
            break;
        case WeaponType::WINDRUNNER_RECURVE: {
            Vector2 upperTip = { center.x - 8.0f * scale, center.y - 12.0f * scale };
            Vector2 lowerTip = { center.x + 8.0f * scale, center.y + 12.0f * scale };
            Color bowCol = isUnlocked ? Color{ 110, 245, 220, 255 } : baseCol;
            Color stringCol = isUnlocked ? Color{ 255, 255, 255, 200 } : Color{ 90, 105, 120, 140 };
            DrawSplineSegmentBezierQuadratic(upperTip, { center.x - 4.0f * scale, center.y }, lowerTip, 2.0f * scale, bowCol);
            DrawLineEx(upperTip, lowerTip, 1.0f * scale, stringCol);
            DrawLineEx({ center.x - 7.0f * scale, center.y + 7.0f * scale }, { center.x + 7.0f * scale, center.y - 7.0f * scale }, 1.2f * scale, isUnlocked ? Color{ 255, 215, 80, 230 } : baseCol);
            break;
        }
        case WeaponType::SUN_CLAYMORE:
            AssetManager::DrawClaymore({ center.x - 10.0f * scale, center.y + 10.0f * scale }, -45.0f, 28.0f * scale,
                isUnlocked ? Color{ 255, 220, 110, 255 } : baseCol,
                isUnlocked ? Color{ 255, 140, 30, 200 } : glowCol);
            break;
        case WeaponType::STORM_GLAIVE:
            AssetManager::DrawGlaive({ center.x - 10.0f * scale, center.y + 10.0f * scale }, -45.0f, 28.0f * scale,
                isUnlocked ? Color{ 140, 255, 235, 255 } : baseCol,
                isUnlocked ? Color{ 60, 210, 190, 200 } : glowCol);
            break;
        case WeaponType::GLACIAL_CLEAVER:
            AssetManager::DrawCleaver({ center.x - 10.0f * scale, center.y + 10.0f * scale }, -45.0f, 28.0f * scale,
                isUnlocked ? Color{ 160, 225, 255, 255 } : baseCol,
                isUnlocked ? Color{ 80, 190, 240, 200 } : glowCol);
            break;
        default:
            break;
    }
}

// ==========================================
static void DrawHelmetSlotIcon(Vector2 center, HelmetType helmet, float scale, bool isUnlocked, bool isEquipped) {
    Color baseCol = isUnlocked ? (isEquipped ? Color{ 255, 235, 140, 255 } : Color{ 220, 235, 255, 255 }) : Color{ 90, 105, 120, 180 };
    Color glowCol = isUnlocked ? (isEquipped ? Color{ 255, 205, 50, 200 } : Color{ 80, 160, 240, 160 }) : Color{ 50, 60, 75, 120 };

    switch (helmet) {
        case HelmetType::COMMONERS_HOOD: {
            Color hoodCol = isUnlocked ? Color{ 140, 120, 95, 255 } : baseCol;
            DrawTriangle({ center.x, center.y - 12.0f * scale }, { center.x - 10.0f * scale, center.y + 6.0f * scale }, { center.x + 10.0f * scale, center.y + 6.0f * scale }, hoodCol);
            DrawCircleV({ center.x, center.y + 1.0f * scale }, 4.5f * scale, Color{ 14, 18, 26, 255 });
            break;
        }
        case HelmetType::CRUSADER_HELM: {
            Color helmCol = isUnlocked ? Color{ 240, 200, 75, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 9.0f * scale, center.y - 11.0f * scale, 18.0f * scale, 22.0f * scale }, 0.25f, 4, helmCol);
            DrawLineEx({ center.x - 7.0f * scale, center.y - 2.0f * scale }, { center.x + 7.0f * scale, center.y - 2.0f * scale }, 2.0f * scale, Color{ 20, 24, 32, 255 });
            DrawLineEx({ center.x, center.y - 6.0f * scale }, { center.x, center.y + 7.0f * scale }, 2.0f * scale, Color{ 20, 24, 32, 255 });
            break;
        }
        case HelmetType::VANGUARD_GREATHELM: {
            Color helmCol = isUnlocked ? Color{ 140, 215, 255, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 10.0f * scale, center.y - 11.0f * scale, 20.0f * scale, 22.0f * scale }, 0.2f, 4, helmCol);
            DrawLineEx({ center.x - 8.0f * scale, center.y - 3.0f * scale }, { center.x + 8.0f * scale, center.y - 3.0f * scale }, 2.5f * scale, Color{ 16, 22, 34, 255 });
            DrawLineEx({ center.x, center.y - 13.0f * scale }, { center.x, center.y - 6.0f * scale }, 2.0f * scale, Color{ 255, 215, 80, 240 });
            break;
        }
        case HelmetType::SHADOW_COWL: {
            Color cowlCol = isUnlocked ? Color{ 48, 56, 76, 255 } : baseCol;
            DrawTriangle({ center.x, center.y - 13.0f * scale }, { center.x - 10.0f * scale, center.y + 7.0f * scale }, { center.x + 10.0f * scale, center.y + 7.0f * scale }, cowlCol);
            DrawCircle((int)(center.x - 3.5f * scale), (int)(center.y + 1.0f * scale), 1.8f * scale, isUnlocked ? Color{ 100, 245, 220, 255 } : glowCol);
            DrawCircle((int)(center.x + 3.5f * scale), (int)(center.y + 1.0f * scale), 1.8f * scale, isUnlocked ? Color{ 100, 245, 220, 255 } : glowCol);
            break;
        }
        case HelmetType::SCRAP_SALLET: {
            Color salletCol = isUnlocked ? Color{ 220, 175, 80, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 9.0f * scale, center.y - 9.0f * scale, 18.0f * scale, 19.0f * scale }, 0.3f, 4, salletCol);
            DrawLineEx({ center.x - 7.0f * scale, center.y }, { center.x + 7.0f * scale, center.y }, 2.0f * scale, Color{ 25, 30, 40, 255 });
            break;
        }
        case HelmetType::BASALT_GREATHELM: {
            Color helmCol = isUnlocked ? Color{ 62, 52, 48, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 10.0f * scale, center.y - 10.0f * scale, 20.0f * scale, 21.0f * scale }, 0.15f, 4, helmCol);
            DrawLineEx({ center.x - 7.0f * scale, center.y - 1.0f * scale }, { center.x + 7.0f * scale, center.y - 1.0f * scale }, 2.5f * scale, isUnlocked ? Color{ 255, 140, 35, 255 } : glowCol);
            break;
        }
        case HelmetType::FROSTWARDEN_HOOD: {
            Color cowlCol = isUnlocked ? Color{ 45, 68, 88, 255 } : baseCol;
            DrawTriangle({ center.x, center.y - 12.0f * scale }, { center.x - 10.0f * scale, center.y + 6.0f * scale }, { center.x + 10.0f * scale, center.y + 6.0f * scale }, cowlCol);
            DrawCircleV({ center.x, center.y + 1.0f * scale }, 4.5f * scale, Color{ 180, 230, 250, 255 });
            DrawLineEx({ center.x - 10.0f * scale, center.y + 6.0f * scale }, { center.x + 10.0f * scale, center.y + 6.0f * scale }, 2.5f * scale, isUnlocked ? Color{ 235, 248, 255, 255 } : glowCol);
            break;
        }
        case HelmetType::NONE:
        default:
            DrawCircleLines((int)center.x, (int)center.y, 8.0f * scale, Color{ 70, 85, 105, 160 });
            break;
    }
}

static void DrawChestplateSlotIcon(Vector2 center, ChestplateType chest, float scale, bool isUnlocked, bool isEquipped) {
    Color baseCol = isUnlocked ? (isEquipped ? Color{ 255, 235, 140, 255 } : Color{ 220, 235, 255, 255 }) : Color{ 90, 105, 120, 180 };
    Color glowCol = isUnlocked ? (isEquipped ? Color{ 255, 205, 50, 200 } : Color{ 80, 160, 240, 160 }) : Color{ 50, 60, 75, 120 };

    switch (chest) {
        case ChestplateType::COMMONERS_TUNIC: {
            Color tunicCol = isUnlocked ? Color{ 135, 115, 90, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 9.0f * scale, center.y - 10.0f * scale, 18.0f * scale, 21.0f * scale }, 0.15f, 4, tunicCol);
            break;
        }
        case ChestplateType::CRUSADER_CUIRASS: {
            Color cuirassCol = isUnlocked ? Color{ 80, 110, 150, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 11.0f * scale, center.y - 11.0f * scale, 22.0f * scale, 23.0f * scale }, 0.25f, 4, cuirassCol);
            DrawLineEx({ center.x - 6.0f * scale, center.y }, { center.x + 6.0f * scale, center.y }, 2.2f * scale, isUnlocked ? Color{ 255, 215, 80, 255 } : glowCol);
            DrawLineEx({ center.x, center.y - 6.0f * scale }, { center.x, center.y + 6.0f * scale }, 2.2f * scale, isUnlocked ? Color{ 255, 215, 80, 255 } : glowCol);
            break;
        }
        case ChestplateType::VANGUARD_CHEST: {
            Color cuirassCol = isUnlocked ? Color{ 95, 145, 205, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 12.0f * scale, center.y - 11.0f * scale, 24.0f * scale, 23.0f * scale }, 0.18f, 4, cuirassCol);
            DrawLineEx({ center.x - 9.0f * scale, center.y - 3.0f * scale }, { center.x + 9.0f * scale, center.y - 3.0f * scale }, 1.5f * scale, Color{ 255, 215, 80, 220 });
            DrawLineEx({ center.x - 9.0f * scale, center.y + 4.0f * scale }, { center.x + 9.0f * scale, center.y + 4.0f * scale }, 1.5f * scale, Color{ 255, 215, 80, 220 });
            break;
        }
        case ChestplateType::SHADOW_GARB: {
            Color garbCol = isUnlocked ? Color{ 40, 50, 70, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 9.0f * scale, center.y - 10.0f * scale, 18.0f * scale, 21.0f * scale }, 0.2f, 4, garbCol);
            DrawLineEx({ center.x - 7.0f * scale, center.y - 7.0f * scale }, { center.x + 7.0f * scale, center.y + 7.0f * scale }, 1.5f * scale, isUnlocked ? Color{ 100, 245, 220, 220 } : glowCol);
            break;
        }
        case ChestplateType::SCRAP_CUIRASS: {
            Color scrapCol = isUnlocked ? Color{ 175, 135, 75, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 10.0f * scale, center.y - 10.0f * scale, 20.0f * scale, 21.0f * scale }, 0.15f, 4, scrapCol);
            DrawLineEx({ center.x - 8.0f * scale, center.y }, { center.x + 8.0f * scale, center.y }, 1.5f * scale, Color{ 35, 30, 25, 220 });
            break;
        }
        case ChestplateType::BASALT_CUIRASS: {
            Color basaltCol = isUnlocked ? Color{ 58, 48, 46, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 11.0f * scale, center.y - 10.0f * scale, 22.0f * scale, 22.0f * scale }, 0.15f, 4, basaltCol);
            DrawCircleV(center, 4.0f * scale, isUnlocked ? Color{ 255, 140, 40, 255 } : glowCol);
            break;
        }
        case ChestplateType::FROSTWARDEN_HAUBERK: {
            Color cuirassCol = isUnlocked ? Color{ 36, 54, 72, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 11.0f * scale, center.y - 10.0f * scale, 22.0f * scale, 22.0f * scale }, 0.15f, 4, cuirassCol);
            DrawCircleV(center, 4.0f * scale, isUnlocked ? Color{ 140, 225, 255, 255 } : glowCol);
            DrawLineEx({ center.x - 9.0f * scale, center.y - 8.0f * scale }, { center.x + 9.0f * scale, center.y - 8.0f * scale }, 2.5f * scale, isUnlocked ? Color{ 235, 248, 255, 255 } : glowCol);
            break;
        }
        case ChestplateType::NONE:
        default:
            DrawCircleLines((int)center.x, (int)center.y, 8.0f * scale, Color{ 70, 85, 105, 160 });
            break;
    }
}

static void DrawLeggingsSlotIcon(Vector2 center, LeggingsType legs, float scale, bool isUnlocked, bool isEquipped) {
    Color baseCol = isUnlocked ? (isEquipped ? Color{ 255, 235, 140, 255 } : Color{ 220, 235, 255, 255 }) : Color{ 90, 105, 120, 180 };
    Color glowCol = isUnlocked ? (isEquipped ? Color{ 255, 205, 50, 200 } : Color{ 80, 160, 240, 160 }) : Color{ 50, 60, 75, 120 };

    switch (legs) {
        case LeggingsType::COMMONERS_BREECHES: {
            Color legCol = isUnlocked ? Color{ 125, 110, 90, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 8.0f * scale, center.y - 10.0f * scale, 6.0f * scale, 20.0f * scale }, 0.2f, 4, legCol);
            DrawRectangleRounded(Rectangle{ center.x + 2.0f * scale, center.y - 10.0f * scale, 6.0f * scale, 20.0f * scale }, 0.2f, 4, legCol);
            break;
        }
        case LeggingsType::CRUSADER_GREAVES: {
            Color plateCol = isUnlocked ? Color{ 80, 110, 150, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 8.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, plateCol);
            DrawRectangleRounded(Rectangle{ center.x + 1.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, plateCol);
            DrawLineEx({ center.x - 8.0f * scale, center.y + 4.0f * scale }, { center.x - 1.0f * scale, center.y + 4.0f * scale }, 1.5f * scale, isUnlocked ? Color{ 255, 215, 80, 220 } : glowCol);
            DrawLineEx({ center.x + 1.0f * scale, center.y + 4.0f * scale }, { center.x + 8.0f * scale, center.y + 4.0f * scale }, 1.5f * scale, isUnlocked ? Color{ 255, 215, 80, 220 } : glowCol);
            break;
        }
        case LeggingsType::VANGUARD_GREAVES: {
            Color plateCol = isUnlocked ? Color{ 95, 145, 205, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 8.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, plateCol);
            DrawRectangleRounded(Rectangle{ center.x + 1.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, plateCol);
            DrawLineEx({ center.x - 8.0f * scale, center.y - 2.0f * scale }, { center.x - 1.0f * scale, center.y - 2.0f * scale }, 1.5f * scale, Color{ 255, 215, 80, 220 });
            DrawLineEx({ center.x + 1.0f * scale, center.y - 2.0f * scale }, { center.x + 8.0f * scale, center.y - 2.0f * scale }, 1.5f * scale, Color{ 255, 215, 80, 220 });
            break;
        }
        case LeggingsType::SHADOW_LEGGINGS: {
            Color bootCol = isUnlocked ? Color{ 40, 50, 70, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 7.0f * scale, center.y - 9.0f * scale, 6.0f * scale, 19.0f * scale }, 0.2f, 4, bootCol);
            DrawRectangleRounded(Rectangle{ center.x + 1.0f * scale, center.y - 9.0f * scale, 6.0f * scale, 19.0f * scale }, 0.2f, 4, bootCol);
            DrawLineEx({ center.x - 7.0f * scale, center.y + 3.0f * scale }, { center.x - 1.0f * scale, center.y + 3.0f * scale }, 1.5f * scale, isUnlocked ? Color{ 100, 245, 220, 200 } : glowCol);
            DrawLineEx({ center.x + 1.0f * scale, center.y + 3.0f * scale }, { center.x + 7.0f * scale, center.y + 3.0f * scale }, 1.5f * scale, isUnlocked ? Color{ 100, 245, 220, 200 } : glowCol);
            break;
        }
        case LeggingsType::SCRAP_GREAVES: {
            Color scrapCol = isUnlocked ? Color{ 175, 135, 75, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 8.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, scrapCol);
            DrawRectangleRounded(Rectangle{ center.x + 1.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, scrapCol);
            break;
        }
        case LeggingsType::BASALT_GREAVES: {
            Color stoneCol = isUnlocked ? Color{ 58, 48, 46, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 8.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, stoneCol);
            DrawRectangleRounded(Rectangle{ center.x + 1.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, stoneCol);
            DrawCircle((int)(center.x - 4.5f * scale), (int)(center.y + 5.0f * scale), 2.0f * scale, isUnlocked ? Color{ 255, 140, 40, 255 } : glowCol);
            DrawCircle((int)(center.x + 4.5f * scale), (int)(center.y + 5.0f * scale), 2.0f * scale, isUnlocked ? Color{ 255, 140, 40, 255 } : glowCol);
            break;
        }
        case LeggingsType::FROSTWARDEN_GREAVES: {
            Color greaveCol = isUnlocked ? Color{ 48, 62, 80, 255 } : baseCol;
            DrawRectangleRounded(Rectangle{ center.x - 8.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, greaveCol);
            DrawRectangleRounded(Rectangle{ center.x + 1.0f * scale, center.y - 10.0f * scale, 7.0f * scale, 20.0f * scale }, 0.2f, 4, greaveCol);
            DrawLineEx({ center.x - 8.0f * scale, center.y + 5.0f * scale }, { center.x - 1.0f * scale, center.y + 5.0f * scale }, 2.0f * scale, isUnlocked ? Color{ 160, 230, 255, 255 } : glowCol);
            DrawLineEx({ center.x + 1.0f * scale, center.y + 5.0f * scale }, { center.x + 8.0f * scale, center.y + 5.0f * scale }, 2.0f * scale, isUnlocked ? Color{ 160, 230, 255, 255 } : glowCol);
            break;
        }
        case LeggingsType::NONE:
        default:
            DrawCircleLines((int)center.x, (int)center.y, 8.0f * scale, Color{ 70, 85, 105, 160 });
            break;
    }
}

static void DrawTotemSlotIcon(Vector2 center, TotemType totem, float scale, bool isUnlocked, bool isEquipped) {
    Color baseCol = isUnlocked ? (isEquipped ? Color{ 255, 235, 140, 255 } : Color{ 220, 235, 255, 255 }) : Color{ 90, 105, 120, 180 };
    Color glowCol = isUnlocked ? (isEquipped ? Color{ 255, 205, 50, 200 } : Color{ 80, 160, 240, 160 }) : Color{ 50, 60, 75, 120 };

    switch (totem) {
        case TotemType::TOTEM_SANITY: {
            DrawPoly(center, 4, 12.0f * scale, 45.0f, isUnlocked ? Color{ 90, 215, 255, 255 } : baseCol);
            DrawCircleV(center, 4.0f * scale, isUnlocked ? Color{ 255, 255, 255, 255 } : glowCol);
            break;
        }
        case TotemType::TOTEM_INSANITY: {
            DrawCircleV(center, 9.0f * scale, isUnlocked ? Color{ 235, 60, 75, 255 } : baseCol);
            DrawLineEx({ center.x - 7.0f * scale, center.y - 6.0f * scale }, { center.x - 12.0f * scale, center.y - 12.0f * scale }, 2.0f * scale, isUnlocked ? Color{ 255, 90, 105, 255 } : baseCol);
            DrawLineEx({ center.x + 7.0f * scale, center.y - 6.0f * scale }, { center.x + 12.0f * scale, center.y - 12.0f * scale }, 2.0f * scale, isUnlocked ? Color{ 255, 90, 105, 255 } : baseCol);
            DrawCircle((int)(center.x - 3.0f * scale), (int)center.y, 1.8f * scale, Color{ 20, 20, 20, 255 });
            DrawCircle((int)(center.x + 3.0f * scale), (int)center.y, 1.8f * scale, Color{ 20, 20, 20, 255 });
            break;
        }
        case TotemType::TOTEM_MIST: {
            DrawCircleV(center, 8.0f * scale, isUnlocked ? Color{ 80, 235, 215, 220 } : baseCol);
            DrawEllipseLines((int)(center.x - 8.0f * scale), (int)(center.y - 5.0f * scale), 6.0f * scale, 3.5f * scale, isUnlocked ? Color{ 140, 255, 240, 255 } : glowCol);
            DrawEllipseLines((int)(center.x + 8.0f * scale), (int)(center.y - 5.0f * scale), 6.0f * scale, 3.5f * scale, isUnlocked ? Color{ 140, 255, 240, 255 } : glowCol);
            break;
        }
        case TotemType::CRUSADER_TOTEM: {
            DrawCircleLines((int)center.x, (int)center.y, 11.0f * scale, isUnlocked ? Color{ 255, 220, 95, 240 } : glowCol);
            DrawLineEx({ center.x - 7.0f * scale, center.y }, { center.x + 7.0f * scale, center.y }, 2.5f * scale, isUnlocked ? Color{ 255, 215, 80, 255 } : baseCol);
            DrawLineEx({ center.x, center.y - 8.0f * scale }, { center.x, center.y + 8.0f * scale }, 2.5f * scale, isUnlocked ? Color{ 255, 215, 80, 255 } : baseCol);
            break;
        }
        case TotemType::ELEMENTAL_CORE_TOTEM: {
            DrawCircleV(center, 9.0f * scale, Color{ 45, 45, 52, 255 });
            DrawCircleV({ center.x - 3.0f * scale, center.y - 2.0f * scale }, 4.0f * scale, isUnlocked ? Color{ 255, 140, 45, 255 } : baseCol);
            DrawCircleV({ center.x + 3.0f * scale, center.y - 2.0f * scale }, 4.0f * scale, isUnlocked ? Color{ 80, 220, 255, 255 } : glowCol);
            DrawCircleV({ center.x, center.y + 3.5f * scale }, 3.5f * scale, isUnlocked ? Color{ 120, 245, 160, 255 } : baseCol);
            break;
        }
        case TotemType::TOTEM_PHLEGMATIC_CORE: {
            DrawPoly(center, 6, 11.5f * scale, 30.0f, isUnlocked ? Color{ 110, 215, 255, 255 } : baseCol);
            DrawPolyLines(center, 6, 11.5f * scale, 30.0f, isUnlocked ? Color{ 200, 245, 255, 255 } : glowCol);
            DrawCircleV(center, 4.0f * scale, isUnlocked ? Color{ 240, 250, 255, 255 } : glowCol);
            break;
        }
        case TotemType::NONE:
        default:
            DrawCircleLines((int)center.x, (int)center.y, 8.0f * scale, Color{ 70, 85, 105, 160 });
            break;
    }
}

static void DrawTotemBuffDebuffText(const std::string& buffText, const std::string& nerfText, int x, int y, int fontSize) {
    std::string bStr = "[+] " + buffText;
    DrawText(bStr.c_str(), x, y, fontSize, Color{ 100, 245, 190, 240 });
    int bw = FontManager::MeasureGameText(bStr, fontSize, GameFont::BODY);
    std::string sep = " | ";
    DrawText(sep.c_str(), x + bw, y, fontSize, Color{ 160, 175, 195, 180 });
    int sw = FontManager::MeasureGameText(sep, fontSize, GameFont::BODY);
    std::string nStr = "[-] " + nerfText;
    DrawText(nStr.c_str(), x + bw + sw, y, fontSize, Color{ 255, 90, 90, 255 });
}

static void DrawEquipmentBubble(Rectangle rec, bool isSelected, bool isHovered, bool isEquipped, bool isLocked, int tierPips = 0, bool showEye = false, bool eyeHidden = false) {
    // Dark metallic slate background with subtle lighting
    Color bgCol = isLocked ? Color{ 12, 14, 20, 240 } :
                  (isEquipped ? Color{ 26, 38, 62, 252 } :
                  (isSelected ? Color{ 22, 34, 54, 250 } :
                  (isHovered ? Color{ 20, 30, 48, 235 } : Color{ 14, 18, 28, 220 })));

    Color borderCol = isLocked ? Color{ 40, 48, 64, 180 } :
                      (isEquipped ? Color{ 255, 215, 80, 255 } :
                      (isSelected ? Color{ 130, 210, 255, 255 } :
                      (isHovered ? Color{ 90, 165, 240, 230 } : Color{ 52, 70, 100, 200 })));

    DrawRectangleRounded(rec, 0.16f, 4, bgCol);
    DrawRectangleRoundedLines(rec, 0.16f, 4, borderCol);

    // Inner inset border line (faithful to Assassin's Creed Valhalla style reference)
    if (rec.width > 28.0f && rec.height > 28.0f) {
        Rectangle innerRec = { rec.x + 3.0f, rec.y + 3.0f, rec.width - 6.0f, rec.height - 6.0f };
        Color innerCol = isEquipped ? Color{ 190, 160, 65, 120 } :
                         (isSelected ? Color{ 80, 140, 205, 120 } : Color{ 36, 48, 70, 100 });
        DrawRectangleRoundedLines(innerRec, 0.12f, 4, innerCol);
    }

    // Ornate corner brackets with diamond corner vertex terminals
    Color bracketCol = isLocked ? Color{ 52, 62, 80, 140 } :
                       (isEquipped ? Color{ 255, 225, 105, 250 } :
                       (isSelected ? Color{ 150, 225, 255, 240 } :
                       (isHovered ? Color{ 120, 190, 255, 220 } : Color{ 75, 105, 150, 180 })));

    float arm = std::min(8.0f, rec.width * 0.22f);
    float inset = 3.0f;

    // Top-Left corner
    DrawLineEx({ rec.x + inset, rec.y + inset }, { rec.x + inset + arm, rec.y + inset }, 1.8f, bracketCol);
    DrawLineEx({ rec.x + inset, rec.y + inset }, { rec.x + inset, rec.y + inset + arm }, 1.8f, bracketCol);
    DrawCircle((int)(rec.x + inset), (int)(rec.y + inset), 1.6f, bracketCol);

    // Top-Right corner
    DrawLineEx({ rec.x + rec.width - inset, rec.y + inset }, { rec.x + rec.width - inset - arm, rec.y + inset }, 1.8f, bracketCol);
    DrawLineEx({ rec.x + rec.width - inset, rec.y + inset }, { rec.x + rec.width - inset, rec.y + inset + arm }, 1.8f, bracketCol);
    DrawCircle((int)(rec.x + rec.width - inset), (int)(rec.y + inset), 1.6f, bracketCol);

    // Bottom-Left corner
    DrawLineEx({ rec.x + inset, rec.y + rec.height - inset }, { rec.x + inset + arm, rec.y + rec.height - inset }, 1.8f, bracketCol);
    DrawLineEx({ rec.x + inset, rec.y + rec.height - inset }, { rec.x + inset, rec.y + rec.height - inset - arm }, 1.8f, bracketCol);
    DrawCircle((int)(rec.x + inset), (int)(rec.y + rec.height - inset), 1.6f, bracketCol);

    // Bottom-Right corner
    DrawLineEx({ rec.x + rec.width - inset, rec.y + rec.height - inset }, { rec.x + rec.width - inset - arm, rec.y + rec.height - inset }, 1.8f, bracketCol);
    DrawLineEx({ rec.x + rec.width - inset, rec.y + rec.height - inset }, { rec.x + rec.width - inset, rec.y + rec.height - inset - arm }, 1.8f, bracketCol);
    DrawCircle((int)(rec.x + rec.width - inset), (int)(rec.y + rec.height - inset), 1.6f, bracketCol);

    // Golden Equipped indicator (top-left diamond and bright dot)
    if (isEquipped) {
        Vector2 eqCenter = { rec.x + 8.0f, rec.y + 8.0f };
        DrawPoly(eqCenter, 4, 3.8f, 45.0f, Color{ 255, 215, 80, 255 });
        DrawCircle((int)eqCenter.x, (int)eqCenter.y, 1.5f, Color{ 255, 255, 220, 255 });
    }

    // Tier diamond pips along bottom edge
    if (tierPips > 0 && !isLocked) {
        float pipSpacing = 6.5f;
        float totalPipsW = (float)(tierPips - 1) * pipSpacing;
        float pipsStartX = rec.x + rec.width * 0.5f - totalPipsW * 0.5f;
        float pipsY = rec.y + rec.height - 7.0f;
        for (int p = 0; p < tierPips; ++p) {
            Vector2 pPos = { pipsStartX + (float)p * pipSpacing, pipsY };
            DrawPoly(pPos, 4, 2.4f, 45.0f, isEquipped ? Color{ 255, 220, 95, 240 } : Color{ 140, 205, 255, 220 });
        }
    }

    // Eye icon for apparel visibility
    if (showEye) {
        float eyeX = rec.x + 9.0f;
        float eyeY = rec.y + rec.height - 8.0f;
        Color eyeCol = eyeHidden ? Color{ 120, 140, 165, 180 } : Color{ 160, 225, 255, 230 };
        DrawCircleLines((int)eyeX, (int)eyeY, 3.2f, eyeCol);
        if (!eyeHidden) {
            DrawCircle((int)eyeX, (int)eyeY, 1.4f, eyeCol);
        } else {
            DrawLineEx({ eyeX - 3.5f, eyeY + 3.5f }, { eyeX + 3.5f, eyeY - 3.5f }, 1.2f, eyeCol);
        }
    }

    // Padlock symbol if locked
    if (isLocked) {
        float lockX = rec.x + rec.width - 9.0f;
        float lockY = rec.y + rec.height - 8.0f;
        DrawRectangle((int)lockX - 3, (int)lockY - 2, 6, 5, Color{ 185, 75, 75, 230 });
        DrawCircleLines((int)lockX, (int)lockY - 3, 2.4f, Color{ 210, 95, 95, 230 });
    }
}

static void DrawEquipmentSlotBubble(Rectangle rec, const char* categoryTag, bool isSelected, bool isHovered, bool isEquipped, int tierPips = 0) {
    DrawEquipmentBubble(rec, isSelected, isHovered, isEquipped, false, tierPips);
    if (categoryTag) {
        DrawText(categoryTag, (int)rec.x + 8, (int)rec.y + 5, 10, Color{ 140, 165, 195, 200 });
    }
}
bool ModalUI::DrawInventory(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex) {

    // Dimmed cinematic backdrop

    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 6, 9, 14, 215 });



    float modalW = std::min(1560.0f, (float)windowWidth - 48.0f);
    float modalH = std::min(860.0f, (float)windowHeight - 36.0f);

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
    DrawText("Manage royal armaments, inspect combat stats, and read uncovered lore fragments.", (int)modalX + 36, (int)modalY + 48, 13, Color{ 175, 195, 220, 220 });

    // Close Button [ESC / I]
    const float closeW = 120.0f;
    const float closeH = 32.0f;
    float closeX = modalX + modalW - closeW - 32.0f;
    float closeY = modalY + 22.0f;
    Rectangle closeRec = { closeX, closeY, closeW, closeH };
    bool isCloseHov = CheckCollisionPointRec(mousePos, closeRec);

    DrawRectangleRounded(closeRec, 0.25f, 4, isCloseHov ? Color{ 55, 75, 110, 255 } : Color{ 28, 36, 52, 230 });
    DrawRectangleRoundedLines(closeRec, 0.25f, 4, isCloseHov ? Color{ 120, 220, 255, 255 } : Color{ 70, 90, 125, 230 });
    DrawText("CLOSE [ESC/TAB]", (int)closeX + 10, (int)closeY + 9, 12, isCloseHov ? Color{ 255, 255, 255, 255 } : Color{ 190, 210, 235, 220 });

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
        DrawText("1. EQUIPMENT & ARSENAL", (int)tab1Rec.x + 16, (int)tab1Rec.y + 9, 14, Color{ 255, 225, 100, 255 });
    } else {
        DrawRectangleRounded(tab1Rec, 0.2f, 4, isTab1Hov ? Color{ 24, 30, 46, 220 } : Color{ 18, 22, 34, 180 });
        DrawText("1. EQUIPMENT & ARSENAL", (int)tab1Rec.x + 16, (int)tab1Rec.y + 9, 14, isTab1Hov ? Color{ 240, 245, 255, 255 } : Color{ 160, 175, 195, 200 });
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
        DrawText(tab2Label.c_str(), (int)tab2Rec.x + 16, (int)tab2Rec.y + 9, 14, Color{ 255, 225, 100, 255 });
    } else {
        DrawRectangleRounded(tab2Rec, 0.2f, 4, isTab2Hov ? Color{ 24, 30, 46, 220 } : Color{ 18, 22, 34, 180 });
        DrawText(tab2Label.c_str(), (int)tab2Rec.x + 16, (int)tab2Rec.y + 9, 14, hasUnread ? Color{ 255, 215, 80, 255 } : (isTab2Hov ? Color{ 240, 245, 255, 255 } : Color{ 160, 175, 195, 200 }));
    }

    // Tab 3: Humoral Bestiary
    Rectangle tab3Rec = { modalX + 496, tabY, 210.0f, 34.0f };
    bool isTab3Hov = CheckCollisionPointRec(mousePos, tab3Rec);
    if ((isTab3Hov && isMousePressed) || IsKeyPressed(KEY_THREE)) {
        activeTab = 2;
        SoundFX::PlayButtonClick();
    }
    if (activeTab == 2) {
        DrawRectangleRounded(tab3Rec, 0.2f, 4, Color{ 32, 42, 64, 255 });
        DrawRectangleRoundedLines(tab3Rec, 0.2f, 4, Color{ 255, 215, 80, 240 });
        DrawText("3. HUMORAL BESTIARY", (int)tab3Rec.x + 16, (int)tab3Rec.y + 9, 14, Color{ 255, 225, 100, 255 });
    } else {
        DrawRectangleRounded(tab3Rec, 0.2f, 4, isTab3Hov ? Color{ 24, 30, 46, 220 } : Color{ 18, 22, 34, 180 });
        DrawText("3. HUMORAL BESTIARY", (int)tab3Rec.x + 16, (int)tab3Rec.y + 9, 14, isTab3Hov ? Color{ 240, 245, 255, 255 } : Color{ 160, 175, 195, 200 });
    }

    // Tab 4: Realm Map [M]
    Rectangle tab4Rec = { modalX + 716, tabY, 190.0f, 34.0f };
    bool isTab4Hov = CheckCollisionPointRec(mousePos, tab4Rec);
    if ((isTab4Hov && isMousePressed) || IsKeyPressed(KEY_FOUR)) {
        activeTab = 3;
        SoundFX::PlayButtonClick();
        return true; // Signals closing inventory and opening map
    }
    DrawRectangleRounded(tab4Rec, 0.2f, 4, isTab4Hov ? Color{ 24, 30, 46, 220 } : Color{ 18, 22, 34, 180 });
    DrawText("4. REALM MAP [M]", (int)tab4Rec.x + 16, (int)tab4Rec.y + 9, 14, isTab4Hov ? Color{ 240, 245, 255, 255 } : Color{ 160, 175, 195, 200 });

    // TAB CONTENT AREA
    float contentY = modalY + 125.0f;

    if (activeTab == 0) {
        // =========================================================================
        // TAB 0: OVERHAULED EQUIPMENT MATRIX & ARSENAL SHOWCASE
        // Faithful to reference: Loadouts, square equipment slots, consumables bar,
        // large hero showcase doll with live combat stats & high contrast legible text.
        // =========================================================================
        static int s_selectedSlot = 1; // 0 = HELMET, 1 = CHESTPLATE, 2 = LEGGINGS, 3 = TOTEM, 4 = WEAPONS
        static bool s_apparelHidden = false;
        static int s_activeLoadout = 1; // 1 = Loadout I, 2 = Loadout II, 3 = Loadout III

        float leftX = modalX + 28.0f;
        float leftW = (modalW - 72.0f) * 0.49f;
        float leftH = modalH - 146.0f;

        // Container backdrop for Left Column (Equipment Matrix)
        DrawRectangleRounded(Rectangle{ leftX, contentY - 8.0f, leftW, leftH }, 0.03f, 6, Color{ 13, 17, 26, 240 });
        DrawRectangleRoundedLines(Rectangle{ leftX, contentY - 8.0f, leftW, leftH }, 0.03f, 6, Color{ 48, 65, 92, 220 });

        // -------------------------------------------------------------
        // TOP HEADER: LOADOUTS & APPAREL TOGGLE
        // -------------------------------------------------------------
        float headerY = contentY + 2.0f;
        DrawText("EQUIPMENT MATRIX", (int)leftX + 16, (int)headerY + 2, 16, Color{ 255, 230, 140, 255 });

        // Loadout diamond badges: [ I ] [ II ] [ III ]
        float loadoutX = leftX + 175.0f;
        DrawText("LOADOUTS:", (int)loadoutX, (int)headerY + 3, 12, Color{ 160, 180, 210, 200 });
        const char* loadoutLabels[3] = { "I", "II", "III" };
        for (int lo = 0; lo < 3; ++lo) {
            float loBtnX = loadoutX + 74.0f + (float)lo * 30.0f;
            Rectangle loRec = { loBtnX, headerY - 1.0f, 26.0f, 22.0f };
            bool isLoActive = (player.GetInventory().GetActiveLoadoutIndex() == lo);
            bool isLoHov = CheckCollisionPointRec(mousePos, loRec);
            if (isLoHov && isMousePressed) {
                player.GetInventory().SetActiveLoadoutIndex(lo);
                const auto& preset = player.GetInventory().GetLoadout(lo);
                if (preset.isConfigured) {
                    if (player.GetInventory().HasWeapon(preset.weapon)) player.SetWeapon(preset.weapon);
                    if (player.GetInventory().HasHelmet(preset.helmet)) player.SetHelmet(preset.helmet);
                    if (player.GetInventory().HasChestplate(preset.chestplate)) player.SetChestplate(preset.chestplate);
                    if (player.GetInventory().HasLeggings(preset.leggings)) player.SetLeggings(preset.leggings);
                    if (player.GetInventory().HasTotem(preset.totem)) player.SetTotem(preset.totem);
                    SoundFX::PlayEquipWeapon();
                } else {
                    player.GetInventory().SaveCurrentToLoadout(lo, player.GetWeapon(), player.GetHelmet(), player.GetChestplate(), player.GetLeggings(), player.GetTotem());
                    SoundFX::PlayButtonClick();
                }
            }
            DrawRectangleRounded(loRec, 0.25f, 4, isLoActive ? Color{ 45, 70, 115, 255 } : (isLoHov ? Color{ 30, 42, 64, 220 } : Color{ 18, 24, 36, 200 }));
            DrawRectangleRoundedLines(loRec, 0.25f, 4, isLoActive ? Color{ 255, 215, 80, 255 } : Color{ 55, 75, 105, 200 });
            DrawText(loadoutLabels[lo], (int)loBtnX + 9, (int)headerY + 3, 12, isLoActive ? Color{ 255, 225, 100, 255 } : Color{ 175, 195, 220, 200 });
        }

        // Snapshot Save Button: [SAVE]
        Rectangle saveRec = { loadoutX + 74.0f + 3.0f * 30.0f + 4.0f, headerY - 1.0f, 46.0f, 22.0f };
        bool isSaveHov = CheckCollisionPointRec(mousePos, saveRec);
        if (isSaveHov && isMousePressed) {
            int curLo = player.GetInventory().GetActiveLoadoutIndex();
            player.GetInventory().SaveCurrentToLoadout(curLo, player.GetWeapon(), player.GetHelmet(), player.GetChestplate(), player.GetLeggings(), player.GetTotem());
            SoundFX::PlayEquipWeapon();
        }
        DrawRectangleRounded(saveRec, 0.25f, 4, isSaveHov ? Color{ 40, 75, 60, 220 } : Color{ 20, 45, 36, 200 });
        DrawRectangleRoundedLines(saveRec, 0.25f, 4, isSaveHov ? Color{ 120, 245, 180, 255 } : Color{ 60, 140, 100, 200 });
        DrawText("SAVE", (int)saveRec.x + 8, (int)headerY + 4, 10, isSaveHov ? Color{ 160, 255, 210, 255 } : Color{ 120, 220, 170, 220 });

        // Apparel Hidden Toggle Pill
        Rectangle apparelPill = { leftX + leftW - 146.0f, headerY - 1.0f, 134.0f, 24.0f };
        bool isApparelHov = CheckCollisionPointRec(mousePos, apparelPill);
        if (isApparelHov && isMousePressed) {
            s_apparelHidden = !s_apparelHidden;
            SoundFX::PlayEquipWeapon();
        }
        DrawRectangleRounded(apparelPill, 0.25f, 4, s_apparelHidden ? Color{ 55, 75, 110, 240 } : (isApparelHov ? Color{ 35, 45, 65, 220 } : Color{ 22, 28, 42, 200 }));
        DrawRectangleRoundedLines(apparelPill, 0.25f, 4, s_apparelHidden ? Color{ 120, 200, 255, 255 } : Color{ 60, 80, 110, 200 });
        DrawText(s_apparelHidden ? "APPAREL HIDDEN" : "APPAREL VISIBLE", (int)apparelPill.x + 8, (int)apparelPill.y + 5, 11, s_apparelHidden ? Color{ 160, 225, 255, 255 } : Color{ 160, 175, 195, 220 });

        DrawLine((int)leftX + 16, (int)contentY + 28, (int)(leftX + leftW - 16), (int)contentY + 28, Color{ 48, 65, 92, 180 });

        // -------------------------------------------------------------
        // SECTION 1: ARMOR PIECES (3 Large Framed Square Slots)
        // -------------------------------------------------------------
        float secArmorY = contentY + 34.0f;
        DrawText("ARMOR PIECES", (int)leftX + 16, (int)secArmorY, 14, Color{ 255, 215, 90, 240 });

        float slotW = (leftW - 32.0f - 16.0f) / 3.0f;
        float slotH = (modalH > 780.0f) ? 76.0f : 66.0f;

        // Slot 0: Helmet Equipment Bubble
        Rectangle helmSlotRec = { leftX + 16.0f, secArmorY + 20.0f, slotW, slotH };
        bool isHelmSlotHov = CheckCollisionPointRec(mousePos, helmSlotRec);
        if (isHelmSlotHov && isMousePressed) {
            s_selectedSlot = 0;
            SoundFX::PlayEquipWeapon();
        }
        HelmetStats hStats = GetHelmetStats(player.GetHelmet());
        bool isHelmSel = (s_selectedSlot == 0);
        bool hasHelmEquipped = (player.GetHelmet() != HelmetType::NONE);
        DrawRectangleRounded(helmSlotRec, 0.14f, 4, isHelmSel ? Color{ 28, 40, 64, 250 } : (isHelmSlotHov ? Color{ 22, 30, 48, 220 } : Color{ 14, 18, 28, 200 }));
        DrawRectangleRoundedLines(helmSlotRec, 0.14f, 4, hasHelmEquipped ? Color{ 255, 215, 80, 240 } : (isHelmSel ? Color{ 130, 210, 255, 255 } : Color{ 48, 65, 92, 200 }));

        float hBubbleSize = slotH - 12.0f;
        Rectangle hIconBox = { helmSlotRec.x + 6.0f, helmSlotRec.y + 6.0f, hBubbleSize, hBubbleSize };
        DrawEquipmentBubble(hIconBox, isHelmSel, isHelmSlotHov, hasHelmEquipped, false, 2, true, s_apparelHidden);
        DrawHelmetSlotIcon({ hIconBox.x + hIconBox.width * 0.5f, hIconBox.y + hIconBox.height * 0.5f }, player.GetHelmet(), 1.05f, true, hasHelmEquipped);

        float hTextX = hIconBox.x + hIconBox.width + 10.0f;
        DrawText("HEAD / HELMET", (int)hTextX, (int)helmSlotRec.y + 7, 10, Color{ 145, 170, 205, 200 });
        DrawText(hStats.name.c_str(), (int)hTextX, (int)helmSlotRec.y + 22, 13, isHelmSel ? Color{ 255, 225, 100, 255 } : Color{ 230, 240, 255, 255 });
        std::string hSub = "Def +" + std::to_string((int)hStats.defense) + " | Will +" + std::to_string((int)hStats.willBonus);
        DrawText(hSub.c_str(), (int)hTextX, (int)helmSlotRec.y + 44, 11, Color{ 105, 240, 185, 230 });

        // Slot 1: Chestplate Equipment Bubble
        Rectangle chestSlotRec = { leftX + 16.0f + slotW + 8.0f, secArmorY + 20.0f, slotW, slotH };
        bool isChestSlotHov = CheckCollisionPointRec(mousePos, chestSlotRec);
        if (isChestSlotHov && isMousePressed) {
            s_selectedSlot = 1;
            SoundFX::PlayEquipWeapon();
        }
        ChestplateStats cStats = GetChestplateStats(player.GetChestplate());
        bool isChestSel = (s_selectedSlot == 1);
        bool hasChestEquipped = (player.GetChestplate() != ChestplateType::NONE);
        DrawRectangleRounded(chestSlotRec, 0.14f, 4, isChestSel ? Color{ 28, 40, 64, 250 } : (isChestSlotHov ? Color{ 22, 30, 48, 220 } : Color{ 14, 18, 28, 200 }));
        DrawRectangleRoundedLines(chestSlotRec, 0.14f, 4, hasChestEquipped ? Color{ 255, 215, 80, 240 } : (isChestSel ? Color{ 130, 210, 255, 255 } : Color{ 48, 65, 92, 200 }));

        float cBubbleSize = slotH - 12.0f;
        Rectangle cIconBox = { chestSlotRec.x + 6.0f, chestSlotRec.y + 6.0f, cBubbleSize, cBubbleSize };
        DrawEquipmentBubble(cIconBox, isChestSel, isChestSlotHov, hasChestEquipped, false, 3, true, s_apparelHidden);
        DrawChestplateSlotIcon({ cIconBox.x + cIconBox.width * 0.5f, cIconBox.y + cIconBox.height * 0.5f }, player.GetChestplate(), 1.05f, true, hasChestEquipped);

        float cTextX = cIconBox.x + cIconBox.width + 10.0f;
        DrawText("CHEST / CUIRASS", (int)cTextX, (int)chestSlotRec.y + 7, 10, Color{ 145, 170, 205, 200 });
        DrawText(cStats.name.c_str(), (int)cTextX, (int)chestSlotRec.y + 22, 13, isChestSel ? Color{ 255, 225, 100, 255 } : Color{ 230, 240, 255, 255 });
        std::string cSub = "Def +" + std::to_string((int)cStats.defense) + (cStats.maxHpBonus > 0.0f ? (" | HP +" + std::to_string((int)cStats.maxHpBonus)) : "");
        DrawText(cSub.c_str(), (int)cTextX, (int)chestSlotRec.y + 44, 11, Color{ 105, 240, 185, 230 });

        // Slot 2: Leggings Equipment Bubble
        Rectangle legsSlotRec = { leftX + 16.0f + (slotW + 8.0f) * 2.0f, secArmorY + 20.0f, slotW, slotH };
        bool isLegsSlotHov = CheckCollisionPointRec(mousePos, legsSlotRec);
        if (isLegsSlotHov && isMousePressed) {
            s_selectedSlot = 2;
            SoundFX::PlayEquipWeapon();
        }
        LeggingsStats lStats = GetLeggingsStats(player.GetLeggings());
        bool isLegsSel = (s_selectedSlot == 2);
        bool hasLegsEquipped = (player.GetLeggings() != LeggingsType::NONE);
        DrawRectangleRounded(legsSlotRec, 0.14f, 4, isLegsSel ? Color{ 28, 40, 64, 250 } : (isLegsSlotHov ? Color{ 22, 30, 48, 220 } : Color{ 14, 18, 28, 200 }));
        DrawRectangleRoundedLines(legsSlotRec, 0.14f, 4, hasLegsEquipped ? Color{ 255, 215, 80, 240 } : (isLegsSel ? Color{ 130, 210, 255, 255 } : Color{ 48, 65, 92, 200 }));

        float lBubbleSize = slotH - 12.0f;
        Rectangle lIconBox = { legsSlotRec.x + 6.0f, legsSlotRec.y + 6.0f, lBubbleSize, lBubbleSize };
        DrawEquipmentBubble(lIconBox, isLegsSel, isLegsSlotHov, hasLegsEquipped, false, 2, true, s_apparelHidden);
        DrawLeggingsSlotIcon({ lIconBox.x + lIconBox.width * 0.5f, lIconBox.y + lIconBox.height * 0.5f }, player.GetLeggings(), 1.05f, true, hasLegsEquipped);

        float lTextX = lIconBox.x + lIconBox.width + 10.0f;
        DrawText("LEGS / GREAVES", (int)lTextX, (int)legsSlotRec.y + 7, 10, Color{ 145, 170, 205, 200 });
        DrawText(lStats.name.c_str(), (int)lTextX, (int)legsSlotRec.y + 22, 13, isLegsSel ? Color{ 255, 225, 100, 255 } : Color{ 230, 240, 255, 255 });
        std::string lSub = "Def +" + std::to_string((int)lStats.defense) + " | Will +" + std::to_string((int)lStats.willBonus);
        DrawText(lSub.c_str(), (int)lTextX, (int)legsSlotRec.y + 44, 11, Color{ 105, 240, 185, 230 });

        // -------------------------------------------------------------// SECTION 2: WEAPONS & ARSENAL (Vector Icons for all weapons)
        // -------------------------------------------------------------
        float secWepY = secArmorY + slotH + 26.0f;
        DrawLine((int)leftX + 16, (int)secWepY, (int)(leftX + leftW - 16), (int)secWepY, Color{ 45, 60, 85, 180 });
        DrawText("WEAPONS (PRIMARY ARSENAL)", (int)leftX + 16, (int)secWepY + 6, 14, Color{ 255, 215, 90, 240 });

        struct WepBtnInfo { WeaponType type; };
        WepBtnInfo wepBtns[10] = {
            { WeaponType::SWORD },
            { WeaponType::DAGGERS },
            { WeaponType::MACE },
            { WeaponType::HALBERD },
            { WeaponType::TRUTH_SEEKER },
            { WeaponType::ZEPHYR_DAGGERS },
            { WeaponType::WINDRUNNER_RECURVE },
            { WeaponType::SUN_CLAYMORE },
            { WeaponType::STORM_GLAIVE },
            { WeaponType::GLACIAL_CLEAVER }
        };
        float wepBtnW = (leftW - 32.0f - (float)(std::size(wepBtns) - 1) * 4.0f) / (float)std::size(wepBtns);
        float wepBtnH = 42.0f;
        int hoveredWepIdx = -1;
        Rectangle hoveredWepRec = { 0, 0, 0, 0 };

        for (int i = 0; i < (int)std::size(wepBtns); ++i) {
            Rectangle wRec = { leftX + 16.0f + (float)i * (wepBtnW + 4.0f), secWepY + 24.0f, wepBtnW, wepBtnH };
            bool isWUnlocked = player.GetInventory().HasWeapon(wepBtns[i].type);
            bool isWEquipped = (player.GetWeapon() == wepBtns[i].type);
            bool isWHov = CheckCollisionPointRec(mousePos, wRec);
            if (isWHov) {
                hoveredWepIdx = i;
                hoveredWepRec = wRec;
            }
            if (isWHov && isMousePressed) {
                if (isWUnlocked) {
                    player.SetWeapon(wepBtns[i].type);
                    SoundFX::PlayEquipWeapon();
                }
                s_selectedSlot = 4;
            }
            int wTierPips = (i < 2) ? 1 : ((i < 4) ? 2 : 3);
            DrawEquipmentBubble(wRec, (isWEquipped && s_selectedSlot == 4), isWHov, isWEquipped, !isWUnlocked, wTierPips);

            // Vector Icon centered in slot - clean, visual, zero squished text!
            Vector2 iconCenter = { wRec.x + wRec.width * 0.5f, wRec.y + wRec.height * 0.5f };
            DrawWeaponSlotIcon(iconCenter, wepBtns[i].type, (wRec.width < 50.0f) ? 0.85f : 0.98f, isWUnlocked, isWEquipped);

            // Upgrade level badge in bottom-right corner (+1, +2, etc.)
            int curUpg = player.GetInventory().GetWeaponUpgrade(wepBtns[i].type);
            if (isWUnlocked && curUpg > 0) {
                std::string badge = "+" + std::to_string(curUpg);
                DrawText(badge.c_str(), (int)(wRec.x + wRec.width - 16), (int)(wRec.y + wRec.height - 12), 10, Color{ 255, 220, 100, 255 });
            }
        }

        // Floating tooltip for hovered weapon slot
        if (hoveredWepIdx >= 0) {
            WeaponType hw = wepBtns[hoveredWepIdx].type;
            bool hwUnlocked = player.GetInventory().HasWeapon(hw);
            bool hwEquipped = (player.GetWeapon() == hw);
            int hwUpg = player.GetInventory().GetWeaponUpgrade(hw);
            WeaponStats hwStats = GetWeaponStats(hw, hwUpg);

            std::string tipTitle = hwStats.name + (hwUpg > 0 ? (" +" + std::to_string(hwUpg)) : "");
            std::string tipStats;
            if (hwUnlocked) {
                tipStats = "Light: " + std::to_string((int)hwStats.lightDamage) + " | Heavy: " + std::to_string((int)hwStats.heavyDamage) + " | Pierce: " + std::to_string((int)(hwStats.defensePierceBonus * 100.0f)) + "%" + (hwEquipped ? " [EQUIPPED]" : " [CLICK TO EQUIP]");
            } else {
                tipStats = "[LOCKED] Inspect in Arsenal Drawer to view unlock condition";
            }

            int tw1 = FontManager::MeasureGameText(tipTitle, 13, GameFont::BODY);
            int tw2 = FontManager::MeasureGameText(tipStats, 11, GameFont::BODY);
            float tipW = (float)std::max(tw1, tw2) + 24.0f;
            float tipH = 46.0f;
            float tipX = hoveredWepRec.x + hoveredWepRec.width * 0.5f - tipW * 0.5f;
            if (tipX < leftX + 16.0f) tipX = leftX + 16.0f;
            if (tipX + tipW > leftX + leftW - 16.0f) tipX = leftX + leftW - 16.0f - tipW;
            float tipY = hoveredWepRec.y - tipH - 8.0f;

            DrawRectangleRounded(Rectangle{ tipX, tipY, tipW, tipH }, 0.18f, 4, Color{ 14, 18, 28, 250 });
            DrawRectangleRoundedLines(Rectangle{ tipX, tipY, tipW, tipH }, 0.18f, 4, hwEquipped ? Color{ 255, 215, 80, 240 } : Color{ 70, 95, 135, 220 });
            DrawText(tipTitle.c_str(), (int)tipX + 12, (int)tipY + 6, 13, hwEquipped ? Color{ 255, 225, 100, 255 } : Color{ 235, 245, 255, 255 });
            DrawText(tipStats.c_str(), (int)tipX + 12, (int)tipY + 24, 11, hwUnlocked ? Color{ 120, 240, 190, 240 } : Color{ 235, 110, 110, 220 });
        }

        // -------------------------------------------------------------
        // SECTION 3: TOTEM RELIC (TALISMAN & QUICK-ATTUNE GRID)
        // -------------------------------------------------------------
        float secTotemY = secWepY + wepBtnH + 24.0f;
        DrawLine((int)leftX + 16, (int)secTotemY, (int)(leftX + leftW - 16), (int)secTotemY, Color{ 45, 60, 85, 180 });
        DrawText("TOTEM RELIC (ATTUNED PRIMAL CORE)", (int)leftX + 16, (int)secTotemY + 6, 14, Color{ 255, 215, 90, 240 });

        // Row 1: Attuned Totem Status Bubble Card
        Rectangle totemSlotRec = { leftX + 16.0f, secTotemY + 24.0f, leftW - 32.0f, 48.0f };
        bool isTotemSlotHov = CheckCollisionPointRec(mousePos, totemSlotRec);
        if (isTotemSlotHov && isMousePressed) {
            s_selectedSlot = 3;
            SoundFX::PlayEquipWeapon();
        }
        TotemStats tStats = GetTotemStats(player.GetTotem());
        bool hasTotem = (player.GetTotem() != TotemType::NONE);
        bool isTotemSel = (s_selectedSlot == 3);
        DrawRectangleRounded(totemSlotRec, 0.12f, 4, isTotemSel ? Color{ 28, 42, 68, 250 } : (isTotemSlotHov ? Color{ 22, 32, 50, 220 } : Color{ 15, 20, 30, 200 }));
        DrawRectangleRoundedLines(totemSlotRec, 0.12f, 4, hasTotem ? tStats.glowColor : (isTotemSel ? Color{ 255, 215, 80, 255 } : Color{ 48, 65, 92, 200 }));

        Rectangle tIconBox = { totemSlotRec.x + 4.0f, totemSlotRec.y + 4.0f, 40.0f, 40.0f };
        DrawEquipmentBubble(tIconBox, isTotemSel, isTotemSlotHov, hasTotem, false, hasTotem ? 3 : 0);
        DrawTotemSlotIcon({ tIconBox.x + tIconBox.width * 0.5f, tIconBox.y + tIconBox.height * 0.5f }, player.GetTotem(), 0.95f, true, hasTotem);

        float tTextX = tIconBox.x + tIconBox.width + 10.0f;
        DrawText(hasTotem ? tStats.name.c_str() : "No Totem Attuned (Quick-Attune Below or Open Drawer)", (int)tTextX, (int)totemSlotRec.y + 6, 13, hasTotem ? tStats.glowColor : Color{ 145, 160, 180, 220 });
        if (hasTotem) {
            DrawTotemBuffDebuffText(tStats.buffText, tStats.nerfText, (int)tTextX, (int)totemSlotRec.y + 26, 11);
        } else {
            DrawText("Attune a discovered primal relic to awaken elemental resonances.", (int)tTextX, (int)totemSlotRec.y + 26, 11, Color{ 130, 145, 165, 200 });
        }

        // Row 2: 1x6 Quick-Attune Grid for all 6 Totems!
        float totGridY = totemSlotRec.y + totemSlotRec.height + 6.0f;
        struct TotemBtnInfo { TotemType type; int tierPips; };
        TotemBtnInfo totemBtns[6] = {
            { TotemType::TOTEM_SANITY, 2 },
            { TotemType::TOTEM_INSANITY, 2 },
            { TotemType::TOTEM_MIST, 3 },
            { TotemType::CRUSADER_TOTEM, 3 },
            { TotemType::ELEMENTAL_CORE_TOTEM, 4 },
            { TotemType::TOTEM_PHLEGMATIC_CORE, 4 }
        };
        float tBtnW = (leftW - 32.0f - 20.0f) / 6.0f;
        float tBtnH = 38.0f;
        int hoveredTotemIdx = -1;
        Rectangle hoveredTotemRec = { 0, 0, 0, 0 };

        for (int ti = 0; ti < 6; ++ti) {
            Rectangle tbRec = { leftX + 16.0f + (float)ti * (tBtnW + 4.0f), totGridY, tBtnW, tBtnH };
            bool isTUnlocked = player.GetInventory().HasTotem(totemBtns[ti].type);
            bool isTEquipped = (player.GetTotem() == totemBtns[ti].type);
            bool isTHov = CheckCollisionPointRec(mousePos, tbRec);
            if (isTHov) {
                hoveredTotemIdx = ti;
                hoveredTotemRec = tbRec;
            }
            if (isTHov && isMousePressed) {
                if (isTUnlocked) {
                    player.SetTotem(isTEquipped ? TotemType::NONE : totemBtns[ti].type);
                    SoundFX::PlayEquipWeapon();
                }
                s_selectedSlot = 3;
            }

            DrawEquipmentBubble(tbRec, (isTEquipped && s_selectedSlot == 3), isTHov, isTEquipped, !isTUnlocked, totemBtns[ti].tierPips);
            DrawTotemSlotIcon({ tbRec.x + tbRec.width * 0.5f, tbRec.y + tbRec.height * 0.5f }, totemBtns[ti].type, 0.85f, isTUnlocked, isTEquipped);
        }

        // Floating tooltip for hovered totem quick-attune slot
        if (hoveredTotemIdx >= 0) {
            TotemType ht = totemBtns[hoveredTotemIdx].type;
            bool htUnlocked = player.GetInventory().HasTotem(ht);
            bool htEquipped = (player.GetTotem() == ht);
            TotemStats htStats = GetTotemStats(ht);

            std::string tipTitle = htStats.name + (htEquipped ? " [ATTUNED]" : (htUnlocked ? " [CLICK TO ATTUNE]" : " [LOCKED]"));
            int tw1 = FontManager::MeasureGameText(tipTitle, 13, GameFont::BODY);
            int tw2 = FontManager::MeasureGameText(htStats.buffText + " | " + htStats.nerfText, 11, GameFont::BODY);
            float tipW = (float)std::max(tw1, tw2) + 28.0f;
            float tipH = 48.0f;
            float tipX = hoveredTotemRec.x + hoveredTotemRec.width * 0.5f - tipW * 0.5f;
            if (tipX < leftX + 16.0f) tipX = leftX + 16.0f;
            if (tipX + tipW > leftX + leftW - 16.0f) tipX = leftX + leftW - 16.0f - tipW;
            float tipY = hoveredTotemRec.y - tipH - 8.0f;

            DrawRectangleRounded(Rectangle{ tipX, tipY, tipW, tipH }, 0.16f, 4, Color{ 14, 18, 28, 250 });
            DrawRectangleRoundedLines(Rectangle{ tipX, tipY, tipW, tipH }, 0.16f, 4, htEquipped ? htStats.glowColor : Color{ 70, 95, 135, 220 });
            DrawText(tipTitle.c_str(), (int)tipX + 12, (int)tipY + 6, 13, htEquipped ? htStats.glowColor : Color{ 235, 245, 255, 255 });
            if (htUnlocked) {
                DrawTotemBuffDebuffText(htStats.buffText, htStats.nerfText, (int)tipX + 12, (int)tipY + 26, 11);
            } else {
                DrawText("Locked // Defeat realm boss or find crypt pedestal to attune", (int)tipX + 12, (int)tipY + 26, 11, Color{ 235, 100, 100, 220 });
            }
        }

        // -------------------------------------------------------------
        // SECTION 4: CONSUMABLES & ESSENCES (Horizontal Framed Slots)
        // -------------------------------------------------------------
        float secMatY = totGridY + tBtnH + 20.0f;
        DrawLine((int)leftX + 16, (int)secMatY, (int)(leftX + leftW - 16), (int)secMatY, Color{ 45, 60, 85, 180 });
        DrawText("CONSUMABLES & ESSENCES", (int)leftX + 16, (int)secMatY + 6, 14, Color{ 255, 215, 90, 240 });

        struct MatSlot { const char* label; int count; Color col; const char* tag; };
        MatSlot matSlots[5] = {
            { "POTIONS", player.GetInventory().GetHealthPotionCount(), Color{ 235, 75, 75, 255 }, "[F] Heal" },
            { "SHINGLES", player.GetInventory().GetShingles(), Color{ 215, 225, 245, 255 }, "Currency" },
            { "ORE SCRAP", player.GetInventory().GetOreScraps(), Color{ 235, 165, 65, 255 }, "Forge Ore" },
            { "MIST ESS.", player.GetInventory().GetEssenceOfMist(), Color{ 90, 225, 210, 255 }, "Summoning" },
            { "SHADOW", player.GetInventory().GetShadowEssence(), Color{ 180, 105, 255, 255 }, "Core Relic" }
        };
        float matW = (leftW - 32.0f - 16.0f) / 5.0f;
        float matH = 50.0f;
        for (int m = 0; m < (int)std::size(matSlots); ++m) {
            Rectangle mRec = { leftX + 16.0f + (float)m * (matW + 4.0f), secMatY + 24.0f, matW, matH };
            DrawRectangleRounded(mRec, 0.12f, 4, Color{ 18, 23, 34, 220 });
            DrawRectangleRoundedLines(mRec, 0.12f, 4, Color{ 45, 60, 85, 180 });
            DrawText(matSlots[m].label, (int)mRec.x + 6, (int)mRec.y + 4, 11, Color{ 145, 165, 190, 210 });
            std::string countStr = "x" + std::to_string(matSlots[m].count);
            DrawText(countStr.c_str(), (int)mRec.x + 6, (int)mRec.y + 19, 14, matSlots[m].col);
            DrawText(matSlots[m].tag, (int)mRec.x + 6, (int)mRec.y + 35, 10, Color{ 120, 135, 155, 190 });
        }

        // -------------------------------------------------------------
        // SECTION 5: FULL ARMOR SET BONUS BANNER
        // -------------------------------------------------------------
        float bannerY = secMatY + matH + 18.0f;
        float bannerMaxH = leftH - (bannerY - contentY) - 8.0f;
        if (bannerMaxH < 100.0f) bannerMaxH = 100.0f;
        Rectangle bannerRec = { leftX + 14.0f, bannerY, leftW - 28.0f, bannerMaxH };
        ArmorSet curSet = player.GetActiveArmorSet();
        ArmorSetInfo setInfo = GetArmorSetInfo(curSet);
        bool hasFullSet = (curSet != ArmorSet::NONE);
        DrawRectangleRounded(bannerRec, 0.08f, 4, hasFullSet ? Color{ 26, 36, 56, 240 } : Color{ 16, 20, 30, 200 });
        DrawRectangleRoundedLines(bannerRec, 0.08f, 4, hasFullSet ? Color{ 255, 215, 80, 240 } : Color{ 48, 64, 90, 200 });

        if (hasFullSet) {
            std::string setTitle = "FULL SET BONUS: " + setInfo.name + " (3/3 ACTIVE)";
            DrawText(setTitle.c_str(), (int)bannerRec.x + 12, (int)bannerRec.y + 8, 14, Color{ 255, 225, 100, 255 });
            std::string passStr = "[Passive]: " + setInfo.passiveName + " - " + setInfo.passiveDescription;
            int passH = FontManager::MeasureTextWrappedHeight(passStr, (int)bannerRec.width - 24, 12, 4);
            FontManager::DrawTextWrapped(passStr, (int)bannerRec.x + 12, (int)bannerRec.y + 26, (int)bannerRec.width - 24, 12, 4, Color{ 120, 245, 180, 240 });

            float cd = player.GetArmorSkillCooldown();
            std::string skillTitle = "[Special Skill [V]]: " + setInfo.skillName + " (" + std::to_string((int)setInfo.skillCooldown) + "s CD)";
            if (cd > 0.0f) {
                skillTitle += " [COOLDOWN: " + std::to_string((int)ceilf(cd)) + "s]";
            } else {
                skillTitle += " [READY]";
            }
            float skillY = bannerRec.y + 26.0f + (float)passH + 6.0f;
            DrawText(skillTitle.c_str(), (int)bannerRec.x + 12, (int)skillY, 12, (cd <= 0.0f) ? Color{ 255, 210, 80, 255 } : Color{ 240, 120, 80, 240 });
            FontManager::DrawTextWrapped(setInfo.skillDescription, (int)bannerRec.x + 12, (int)skillY + 16, (int)bannerRec.width - 24, 11, 3, Color{ 180, 205, 235, 220 });
        } else {
            DrawText("ARMOR SET STATUS: MISMATCHED PIECES", (int)bannerRec.x + 12, (int)bannerRec.y + 8, 14, Color{ 190, 205, 225, 230 });
            FontManager::DrawTextWrapped("Equip 3 matching set pieces (Helmet + Chest + Leggings) to awaken powerful humoral bonuses and the activated special skill via [V].",
                (int)bannerRec.x + 12, (int)bannerRec.y + 28, (int)bannerRec.width - 24, 12, 4, Color{ 150, 170, 195, 210 });
            DrawText("Sets: Commoner's, Crusader, Vanguard, Shadowstrider, Scrap Iron, Basalt.", (int)bannerRec.x + 12, (int)bannerRec.y + bannerMaxH - 20.0f, 11, Color{ 255, 205, 100, 230 });
        }

        // =========================================================================
        // RIGHT COLUMN: HERO SHOWCASE DOLL & COMBAT ATTRIBUTES + ARSENAL DRAWER
        // =========================================================================
        float rightX = leftX + leftW + 16.0f;
        float rightW = modalW - 48.0f - leftW - 16.0f;
        float rightH = leftH;

        DrawRectangleRounded(Rectangle{ rightX, contentY - 8.0f, rightW, rightH }, 0.03f, 6, Color{ 13, 17, 26, 240 });
        DrawRectangleRoundedLines(Rectangle{ rightX, contentY - 8.0f, rightW, rightH }, 0.03f, 6, Color{ 48, 65, 92, 220 });

        // -------------------------------------------------------------
        // TOP HALF OF RIGHT COLUMN: HERO SHOWCASE & LIVE COMBAT STATS
        // -------------------------------------------------------------
        float heroBoxH = (modalH > 780.0f) ? 220.0f : 175.0f;
        Rectangle heroBoxRec = { rightX + 12.0f, contentY - 2.0f, rightW - 24.0f, heroBoxH };
        DrawRectangleRounded(heroBoxRec, 0.04f, 4, Color{ 18, 24, 38, 200 });
        DrawRectangleRoundedLines(heroBoxRec, 0.04f, 4, Color{ 40, 56, 80, 180 });

        // Hero Doll Pedestal & Character Rendering
        float dollScale = (modalH > 780.0f) ? 1.75f : 1.35f;
        Vector2 dollCenter = { heroBoxRec.x + 110.0f, heroBoxRec.y + heroBoxH * 0.52f };
        
        // Runic pedestal circle
        Vector2 pedestalCenter = { dollCenter.x, dollCenter.y + 48.0f * dollScale };
        DrawCircleGradient(pedestalCenter, 65.0f * dollScale, Color{ 40, 65, 105, 160 }, Color{ 12, 16, 24, 0 });
        DrawEllipseLines((int)dollCenter.x, (int)(dollCenter.y + 44.0f * dollScale), 55.0f * dollScale, 18.0f * dollScale, Color{ 100, 165, 235, 180 });
        DrawEllipseLines((int)dollCenter.x, (int)(dollCenter.y + 44.0f * dollScale), 40.0f * dollScale, 12.0f * dollScale, Color{ 255, 215, 80, 160 });

        HelmetType drawHelm = s_apparelHidden ? HelmetType::NONE : player.GetHelmet();
        ChestplateType drawChest = s_apparelHidden ? ChestplateType::NONE : player.GetChestplate();
        LeggingsType drawLegs = s_apparelHidden ? LeggingsType::NONE : player.GetLeggings();
        AssetManager::DrawCharacterDoll(dollCenter, dollScale, player.GetWeapon(), drawHelm, drawChest, drawLegs, player.GetTotem(), (float)GetTime());

        // Live Combat Attributes Panel (Right side of hero box)
        float statPanelX = heroBoxRec.x + 220.0f;
        float statPanelW = heroBoxRec.width - 232.0f;
        DrawText("COMBAT ATTRIBUTES", (int)statPanelX, (int)heroBoxRec.y + 8, 14, Color{ 255, 225, 110, 255 });
        DrawLine((int)statPanelX, (int)heroBoxRec.y + 26, (int)(statPanelX + statPanelW), (int)heroBoxRec.y + 26, Color{ 50, 70, 98, 180 });

        struct StatRow { const char* name; std::string val; Color col; };
        StatRow statRows[6] = {
            { "MAX HEALTH", std::to_string((int)player.GetHealth()) + " / " + std::to_string((int)player.GetMaxHealth()), Color{ 120, 245, 140, 255 } },
            { "STAMINA", std::to_string((int)player.GetStamina()) + " / " + std::to_string((int)player.GetMaxStamina()), Color{ 255, 205, 80, 255 } },
            { "DEFENSE", std::to_string((int)player.GetDefense()), Color{ 120, 195, 255, 255 } },
            { "WILLPOWER", std::to_string((int)player.GetWill()), Color{ 210, 145, 255, 255 } },
            { "SPEED", std::to_string((int)player.GetSpeed()), Color{ 100, 240, 220, 255 } },
            { "STAM REGEN", std::to_string((int)player.GetStaminaRegenRate()) + " /s", Color{ 240, 220, 140, 255 } }
        };

        float statRowH = (heroBoxH - 40.0f) / 6.0f;
        for (int sr = 0; sr < (int)std::size(statRows); ++sr) {
            float sry = heroBoxRec.y + 30.0f + (float)sr * statRowH;
            DrawText(statRows[sr].name, (int)statPanelX + 4, (int)sry + 2, 12, Color{ 160, 180, 205, 220 });
            int valW = FontManager::MeasureGameText(statRows[sr].val, 12, GameFont::BODY);
            DrawText(statRows[sr].val.c_str(), (int)(statPanelX + statPanelW - valW - 8.0f), (int)sry + 2, 12, statRows[sr].col);
            if (sr < 5) {
                DrawLine((int)statPanelX + 4, (int)sry + (int)statRowH - 1, (int)(statPanelX + statPanelW - 4), (int)sry + (int)statRowH - 1, Color{ 32, 44, 62, 120 });
            }
        }

        // -------------------------------------------------------------
        // BOTTOM HALF OF RIGHT COLUMN: SELECTION & ARSENAL DRAWER
        // -------------------------------------------------------------
        float drawerY = heroBoxRec.y + heroBoxH + 12.0f;
        DrawLine((int)rightX + 16, (int)drawerY - 4.0f, (int)(rightX + rightW - 16), (int)drawerY - 4.0f, Color{ 48, 65, 92, 180 });

        // Category tabs on drawer
        const char* slotCatNames[5] = { "1. HELMET", "2. CHEST", "3. LEGGINGS", "4. TOTEM", "5. WEAPONS" };
        float tabW = (rightW - 32.0f - 16.0f) / 5.0f;
        for (int i = 0; i < (int)std::size(slotCatNames); ++i) {
            Rectangle catRec = { rightX + 16.0f + (float)i * (tabW + 4.0f), drawerY, tabW, 28.0f };
            bool isCatHov = CheckCollisionPointRec(mousePos, catRec);
            if (isCatHov && isMousePressed) {
                s_selectedSlot = i;
                SoundFX::PlayEquipWeapon();
            }
            bool isCatActive = (s_selectedSlot == i);
            DrawRectangleRounded(catRec, 0.2f, 4, isCatActive ? Color{ 40, 60, 95, 240 } : (isCatHov ? Color{ 28, 38, 58, 220 } : Color{ 18, 24, 36, 180 }));
            DrawRectangleRoundedLines(catRec, 0.2f, 4, isCatActive ? Color{ 255, 215, 80, 255 } : Color{ 48, 65, 92, 180 });
            DrawText(slotCatNames[i], (int)catRec.x + 6, (int)catRec.y + 7, 12, isCatActive ? Color{ 255, 225, 100, 255 } : Color{ 160, 180, 205, 200 });
        }

        float drawerListY = drawerY + 36.0f;
        float drawerCardW = rightW - 32.0f;
        float drawerH = contentY + rightH - drawerListY - 8.0f;
        float gridW = 295.0f;
        float gridH = drawerH;
        float panelX = rightX + 16.0f + gridW + 14.0f;
        float panelW = drawerCardW - gridW - 14.0f;
        float panelH = drawerH;

        static int s_inspectedHelm = 2;   // Default: Crusader Sallet
        static int s_inspectedChest = 2;  // Default: Crusader Cuirass
        static int s_inspectedLegs = 2;   // Default: Crusader Greaves
        static int s_inspectedTotem = 0;  // Default: Totem of Sanity
        static int s_inspectedWep = 0;    // Default: Sword

        float bSize = 78.0f;
        float bSpacingX = 18.0f;
        float bSpacingY = 14.0f;
        float gridStartX = rightX + 16.0f + 14.0f;
        float gridStartY = drawerListY + 34.0f;

        if (s_selectedSlot == 0) {
            // =========================================================
            // 1. HELMET ARMORY GRID & INSPECTION
            // =========================================================
            struct HelmEntry { HelmetType type; const char* lockMsg; int tierPips; };
            HelmEntry helms[8] = {
                { HelmetType::NONE, "Unarmored default", 0 },
                { HelmetType::COMMONERS_HOOD, "Ruined Castle - Awakening Pedestal", 1 },
                { HelmetType::CRUSADER_HELM, "Zyphden - Defeat Knight Lenon in courtyard spar", 2 },
                { HelmetType::VANGUARD_GREATHELM, "Blacksmith Varek - Armor Forge", 2 },
                { HelmetType::SHADOW_COWL, "Varek's Forge - Craft with Mist Essence", 3 },
                { HelmetType::SCRAP_SALLET, "Blacksmith Varek - Armor Forge", 2 },
                { HelmetType::BASALT_GREATHELM, "Blacksmith Varek - Armor Forge", 3 },
                { HelmetType::FROSTWARDEN_HOOD, "Blacksmith Varek - Highland Troll Crafting", 3 }
            };

            int unlockedCount = 0;
            for (int i = 0; i < (int)std::size(helms); ++i) {
                if (player.GetInventory().HasHelmet(helms[i].type)) unlockedCount++;
            }

            // Left Grid Container
            DrawRectangleRounded(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 15, 20, 30, 240 });
            DrawRectangleRoundedLines(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 48, 65, 92, 180 });
            std::string gridTitle = "HELMETS & CRESTS (" + std::to_string(unlockedCount) + "/" + std::to_string(std::size(helms)) + " UNLOCKED)";
            DrawText(gridTitle.c_str(), (int)rightX + 28, (int)drawerListY + 8, 12, Color{ 255, 215, 90, 240 });
            DrawLine((int)rightX + 28, (int)drawerListY + 24, (int)(rightX + 16.0f + gridW - 12.0f), (int)drawerListY + 24, Color{ 42, 58, 82, 160 });

            if (s_inspectedHelm < 0 || s_inspectedHelm >= (int)std::size(helms)) s_inspectedHelm = 2;

            for (int i = 0; i < (int)std::size(helms); ++i) {
                int col = i % 3;
                int row = i / 3;
                Rectangle bRec = { gridStartX + (float)col * (bSize + bSpacingX), gridStartY + (float)row * (bSize + bSpacingY), bSize, bSize };
                bool isUnlocked = player.GetInventory().HasHelmet(helms[i].type);
                bool isEquipped = (player.GetHelmet() == helms[i].type);
                bool isSelected = (s_inspectedHelm == i);
                bool isHov = CheckCollisionPointRec(mousePos, bRec);

                if (isHov) s_inspectedHelm = i;
                if (isHov && isMousePressed) {
                    if (isUnlocked && !isEquipped) {
                        player.SetHelmet(helms[i].type);
                        SoundFX::PlayEquipWeapon();
                    }
                }

                DrawEquipmentBubble(bRec, isSelected, isHov, isEquipped, !isUnlocked, helms[i].tierPips);
                Vector2 iconCenter = { bRec.x + bSize * 0.5f, bRec.y + bSize * 0.5f };
                DrawHelmetSlotIcon(iconCenter, helms[i].type, 1.25f, isUnlocked, isEquipped);
            }

            // Right Inspection Panel
            HelmetStats st = GetHelmetStats(helms[s_inspectedHelm].type);
            ArmorSetInfo setInfo = GetArmorSetInfo(st.setAffinity);
            bool isInspUnlocked = player.GetInventory().HasHelmet(helms[s_inspectedHelm].type);
            bool isInspEquipped = (player.GetHelmet() == helms[s_inspectedHelm].type);

            DrawRectangleRounded(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, Color{ 16, 22, 34, 245 });
            DrawRectangleRoundedLines(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, isInspEquipped ? Color{ 255, 215, 80, 220 } : Color{ 52, 72, 105, 200 });

            // Preview bubble
            Rectangle prevRec = { panelX + 14.0f, drawerListY + 12.0f, 54.0f, 54.0f };
            DrawEquipmentBubble(prevRec, true, false, isInspEquipped, !isInspUnlocked, helms[s_inspectedHelm].tierPips);
            DrawHelmetSlotIcon({ prevRec.x + prevRec.width * 0.5f, prevRec.y + prevRec.height * 0.5f }, helms[s_inspectedHelm].type, 1.35f, isInspUnlocked, isInspEquipped);

            // Header titles
            float titleX = panelX + 78.0f;
            DrawText(st.name.c_str(), (int)titleX, (int)drawerListY + 12, 15, isInspEquipped ? Color{ 255, 225, 100, 255 } : Color{ 235, 245, 255, 255 });
            DrawText("Royal Knight Headpiece", (int)titleX, (int)drawerListY + 31, 11, Color{ 160, 180, 205, 200 });
            std::string affText = (st.setAffinity != ArmorSet::NONE) ? ("Set: " + setInfo.name) : "Standalone Helm";
            DrawText(affText.c_str(), (int)titleX, (int)drawerListY + 47, 11, Color{ 120, 240, 190, 240 });

            // Action button
            Rectangle btnRec = { panelX + panelW - 98.0f, drawerListY + 12.0f, 86.0f, 28.0f };
            bool isBtnHov = CheckCollisionPointRec(mousePos, btnRec);
            if (isInspEquipped) {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 255, 215, 80, 230 });
                DrawText("EQUIPPED", (int)btnRec.x + 14, (int)btnRec.y + 7, 11, Color{ 14, 18, 26, 255 });
            } else if (isInspUnlocked) {
                DrawRectangleRounded(btnRec, 0.25f, 4, isBtnHov ? Color{ 60, 100, 160, 255 } : Color{ 36, 56, 88, 230 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, isBtnHov ? Color{ 140, 215, 255, 255 } : Color{ 70, 110, 175, 220 });
                DrawText("EQUIP", (int)btnRec.x + 24, (int)btnRec.y + 7, 11, isBtnHov ? Color{ 255, 255, 255, 255 } : Color{ 200, 225, 255, 240 });
                if (isBtnHov && isMousePressed) {
                    player.SetHelmet(helms[s_inspectedHelm].type);
                    SoundFX::PlayEquipWeapon();
                }
            } else {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 26, 30, 40, 200 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, Color{ 60, 40, 45, 180 });
                DrawText("LOCKED", (int)btnRec.x + 20, (int)btnRec.y + 7, 11, Color{ 235, 95, 95, 220 });
            }

            // Divider
            DrawLine((int)panelX + 14, (int)drawerListY + 74, (int)(panelX + panelW - 14), (int)drawerListY + 74, Color{ 45, 62, 90, 180 });

            // Stats
            DrawText("COMBAT ATTRIBUTES & BONUSES", (int)panelX + 14, (int)drawerListY + 82, 11, Color{ 255, 215, 90, 240 });
            HelmetStats curSt = GetHelmetStats(player.GetHelmet());
            int defDelta = (int)(st.defense - curSt.defense);
            int willDelta = (int)(st.willBonus - curSt.willBonus);
            int hpDelta = (int)(st.maxHpBonus - curSt.maxHpBonus);

            std::string sLine1 = "Def: +" + std::to_string((int)st.defense);
            if (!isInspEquipped && defDelta != 0) sLine1 += " (" + std::string(defDelta > 0 ? "+" : "") + std::to_string(defDelta) + ")";
            sLine1 += " | Will: +" + std::to_string((int)st.willBonus);
            if (!isInspEquipped && willDelta != 0) sLine1 += " (" + std::string(willDelta > 0 ? "+" : "") + std::to_string(willDelta) + ")";
            if (st.maxHpBonus > 0.0f || curSt.maxHpBonus > 0.0f) {
                sLine1 += " | HP: +" + std::to_string((int)st.maxHpBonus);
                if (!isInspEquipped && hpDelta != 0) sLine1 += " (" + std::string(hpDelta > 0 ? "+" : "") + std::to_string(hpDelta) + ")";
            }
            DrawText(sLine1.c_str(), (int)panelX + 14, (int)drawerListY + 100, 12, Color{ 105, 240, 185, 240 });

            float curY = drawerListY + 118.0f;
            if (st.setAffinity != ArmorSet::NONE) {
                std::string synLine = "[Synergy]: " + setInfo.passiveName + " (" + (player.GetActiveArmorSet() == st.setAffinity ? "3/3 Active" : "1/3 Inactive") + ")";
                DrawText(synLine.c_str(), (int)panelX + 14, (int)curY, 11, (player.GetActiveArmorSet() == st.setAffinity) ? Color{ 255, 220, 95, 240 } : Color{ 150, 170, 195, 200 });
                curY += 20.0f;
            } else {
                curY += 4.0f;
            }

            // Divider
            DrawLine((int)panelX + 14, (int)curY, (int)(panelX + panelW - 14), (int)curY, Color{ 45, 62, 90, 180 });
            curY += 8.0f;

            // Lore
            DrawText("ARMAMENT LORE & HISTORY", (int)panelX + 14, (int)curY, 11, Color{ 255, 215, 90, 240 });
            curY += 18.0f;
            FontManager::DrawTextWrapped(st.description, (int)panelX + 14, (int)curY, (int)panelW - 28, 11, 3, Color{ 185, 205, 230, 220 });

            // Source at bottom
            float sourceY = drawerListY + panelH - 32.0f;
            DrawLine((int)panelX + 14, (int)sourceY - 6.0f, (int)(panelX + panelW - 14), (int)sourceY - 6.0f, Color{ 45, 62, 90, 160 });
            if (isInspUnlocked) {
                DrawText("STATUS // Acquired and ready for battle", (int)panelX + 14, (int)sourceY, 11, Color{ 110, 240, 180, 240 });
            } else {
                std::string lMsg = "LOCKED // " + std::string(helms[s_inspectedHelm].lockMsg);
                DrawText(lMsg.c_str(), (int)panelX + 14, (int)sourceY, 11, Color{ 255, 95, 95, 240 });
            }
        } else if (s_selectedSlot == 1) {
            // =========================================================
            // 2. CHESTPLATE ARMORY GRID & INSPECTION
            // =========================================================
            struct ChestEntry { ChestplateType type; const char* lockMsg; int tierPips; };
            ChestEntry chests[8] = {
                { ChestplateType::NONE, "Unarmored default", 0 },
                { ChestplateType::COMMONERS_TUNIC, "Ruined Castle - Awakening Pedestal", 1 },
                { ChestplateType::CRUSADER_CUIRASS, "Zyphden - Defeat Knight Lenon in courtyard spar", 2 },
                { ChestplateType::VANGUARD_CHEST, "Blacksmith Varek - Armor Forge", 2 },
                { ChestplateType::SHADOW_GARB, "Varek's Forge - Craft with Mist Essence", 3 },
                { ChestplateType::SCRAP_CUIRASS, "Blacksmith Varek - Armor Forge", 2 },
                { ChestplateType::BASALT_CUIRASS, "Blacksmith Varek - Armor Forge", 3 },
                { ChestplateType::FROSTWARDEN_HAUBERK, "Blacksmith Varek - Highland Troll Crafting", 3 }
            };

            int unlockedCount = 0;
            for (int i = 0; i < (int)std::size(chests); ++i) {
                if (player.GetInventory().HasChestplate(chests[i].type)) unlockedCount++;
            }

            // Left Grid Container
            DrawRectangleRounded(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 15, 20, 30, 240 });
            DrawRectangleRoundedLines(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 48, 65, 92, 180 });
            std::string gridTitle = "CHESTPLATES & CUIRASSES (" + std::to_string(unlockedCount) + "/" + std::to_string(std::size(chests)) + " UNLOCKED)";
            DrawText(gridTitle.c_str(), (int)rightX + 28, (int)drawerListY + 8, 12, Color{ 255, 215, 90, 240 });
            DrawLine((int)rightX + 28, (int)drawerListY + 24, (int)(rightX + 16.0f + gridW - 12.0f), (int)drawerListY + 24, Color{ 42, 58, 82, 160 });

            if (s_inspectedChest < 0 || s_inspectedChest >= (int)std::size(chests)) s_inspectedChest = 2;

            for (int i = 0; i < (int)std::size(chests); ++i) {
                int col = i % 3;
                int row = i / 3;
                Rectangle bRec = { gridStartX + (float)col * (bSize + bSpacingX), gridStartY + (float)row * (bSize + bSpacingY), bSize, bSize };
                bool isUnlocked = player.GetInventory().HasChestplate(chests[i].type);
                bool isEquipped = (player.GetChestplate() == chests[i].type);
                bool isSelected = (s_inspectedChest == i);
                bool isHov = CheckCollisionPointRec(mousePos, bRec);

                if (isHov) s_inspectedChest = i;
                if (isHov && isMousePressed) {
                    if (isUnlocked && !isEquipped) {
                        player.SetChestplate(chests[i].type);
                        SoundFX::PlayEquipWeapon();
                    }
                }

                DrawEquipmentBubble(bRec, isSelected, isHov, isEquipped, !isUnlocked, chests[i].tierPips);
                Vector2 iconCenter = { bRec.x + bSize * 0.5f, bRec.y + bSize * 0.5f };
                DrawChestplateSlotIcon(iconCenter, chests[i].type, 1.25f, isUnlocked, isEquipped);
            }

            // Right Inspection Panel
            ChestplateStats st = GetChestplateStats(chests[s_inspectedChest].type);
            ArmorSetInfo setInfo = GetArmorSetInfo(st.setAffinity);
            bool isInspUnlocked = player.GetInventory().HasChestplate(chests[s_inspectedChest].type);
            bool isInspEquipped = (player.GetChestplate() == chests[s_inspectedChest].type);

            DrawRectangleRounded(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, Color{ 16, 22, 34, 245 });
            DrawRectangleRoundedLines(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, isInspEquipped ? Color{ 255, 215, 80, 220 } : Color{ 52, 72, 105, 200 });

            // Preview bubble
            Rectangle prevRec = { panelX + 14.0f, drawerListY + 12.0f, 54.0f, 54.0f };
            DrawEquipmentBubble(prevRec, true, false, isInspEquipped, !isInspUnlocked, chests[s_inspectedChest].tierPips);
            DrawChestplateSlotIcon({ prevRec.x + prevRec.width * 0.5f, prevRec.y + prevRec.height * 0.5f }, chests[s_inspectedChest].type, 1.35f, isInspUnlocked, isInspEquipped);

            // Header titles
            float titleX = panelX + 78.0f;
            DrawText(st.name.c_str(), (int)titleX, (int)drawerListY + 12, 15, isInspEquipped ? Color{ 255, 225, 100, 255 } : Color{ 235, 245, 255, 255 });
            DrawText("Tempered Body Cuirass", (int)titleX, (int)drawerListY + 31, 11, Color{ 160, 180, 205, 200 });
            std::string affText = (st.setAffinity != ArmorSet::NONE) ? ("Set: " + setInfo.name) : "Standalone Cuirass";
            DrawText(affText.c_str(), (int)titleX, (int)drawerListY + 47, 11, Color{ 120, 240, 190, 240 });

            // Action button
            Rectangle btnRec = { panelX + panelW - 98.0f, drawerListY + 12.0f, 86.0f, 28.0f };
            bool isBtnHov = CheckCollisionPointRec(mousePos, btnRec);
            if (isInspEquipped) {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 255, 215, 80, 230 });
                DrawText("EQUIPPED", (int)btnRec.x + 14, (int)btnRec.y + 7, 11, Color{ 14, 18, 26, 255 });
            } else if (isInspUnlocked) {
                DrawRectangleRounded(btnRec, 0.25f, 4, isBtnHov ? Color{ 60, 100, 160, 255 } : Color{ 36, 56, 88, 230 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, isBtnHov ? Color{ 140, 215, 255, 255 } : Color{ 70, 110, 175, 220 });
                DrawText("EQUIP", (int)btnRec.x + 24, (int)btnRec.y + 7, 11, isBtnHov ? Color{ 255, 255, 255, 255 } : Color{ 200, 225, 255, 240 });
                if (isBtnHov && isMousePressed) {
                    player.SetChestplate(chests[s_inspectedChest].type);
                    SoundFX::PlayEquipWeapon();
                }
            } else {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 26, 30, 40, 200 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, Color{ 60, 40, 45, 180 });
                DrawText("LOCKED", (int)btnRec.x + 20, (int)btnRec.y + 7, 11, Color{ 235, 95, 95, 220 });
            }

            // Divider
            DrawLine((int)panelX + 14, (int)drawerListY + 74, (int)(panelX + panelW - 14), (int)drawerListY + 74, Color{ 45, 62, 90, 180 });

            // Stats
            DrawText("COMBAT ATTRIBUTES & BONUSES", (int)panelX + 14, (int)drawerListY + 82, 11, Color{ 255, 215, 90, 240 });
            ChestplateStats curSt = GetChestplateStats(player.GetChestplate());
            int defDelta = (int)(st.defense - curSt.defense);
            int willDelta = (int)(st.willBonus - curSt.willBonus);
            int hpDelta = (int)(st.maxHpBonus - curSt.maxHpBonus);

            std::string sLine1 = "Def: +" + std::to_string((int)st.defense);
            if (!isInspEquipped && defDelta != 0) sLine1 += " (" + std::string(defDelta > 0 ? "+" : "") + std::to_string(defDelta) + ")";
            sLine1 += " | Will: +" + std::to_string((int)st.willBonus);
            if (!isInspEquipped && willDelta != 0) sLine1 += " (" + std::string(willDelta > 0 ? "+" : "") + std::to_string(willDelta) + ")";
            if (st.maxHpBonus > 0.0f || curSt.maxHpBonus > 0.0f) {
                sLine1 += " | HP: +" + std::to_string((int)st.maxHpBonus);
                if (!isInspEquipped && hpDelta != 0) sLine1 += " (" + std::string(hpDelta > 0 ? "+" : "") + std::to_string(hpDelta) + ")";
            }
            DrawText(sLine1.c_str(), (int)panelX + 14, (int)drawerListY + 100, 12, Color{ 105, 240, 185, 240 });

            float curY = drawerListY + 118.0f;
            if (st.setAffinity != ArmorSet::NONE) {
                std::string synLine = "[Synergy]: " + setInfo.passiveName + " (" + (player.GetActiveArmorSet() == st.setAffinity ? "3/3 Active" : "1/3 Inactive") + ")";
                DrawText(synLine.c_str(), (int)panelX + 14, (int)curY, 11, (player.GetActiveArmorSet() == st.setAffinity) ? Color{ 255, 220, 95, 240 } : Color{ 150, 170, 195, 200 });
                curY += 20.0f;
            } else {
                curY += 4.0f;
            }

            // Divider
            DrawLine((int)panelX + 14, (int)curY, (int)(panelX + panelW - 14), (int)curY, Color{ 45, 62, 90, 180 });
            curY += 8.0f;

            // Lore
            DrawText("ARMAMENT LORE & HISTORY", (int)panelX + 14, (int)curY, 11, Color{ 255, 215, 90, 240 });
            curY += 18.0f;
            FontManager::DrawTextWrapped(st.description, (int)panelX + 14, (int)curY, (int)panelW - 28, 11, 3, Color{ 185, 205, 230, 220 });

            // Source at bottom
            float sourceY = drawerListY + panelH - 32.0f;
            DrawLine((int)panelX + 14, (int)sourceY - 6.0f, (int)(panelX + panelW - 14), (int)sourceY - 6.0f, Color{ 45, 62, 90, 160 });
            if (isInspUnlocked) {
                DrawText("STATUS // Acquired and ready for battle", (int)panelX + 14, (int)sourceY, 11, Color{ 110, 240, 180, 240 });
            } else {
                std::string lMsg = "LOCKED // " + std::string(chests[s_inspectedChest].lockMsg);
                DrawText(lMsg.c_str(), (int)panelX + 14, (int)sourceY, 11, Color{ 255, 95, 95, 240 });
            }
        } else if (s_selectedSlot == 2) {
            // =========================================================
            // 3. LEGGINGS ARMORY GRID & INSPECTION
            // =========================================================
            struct LegEntry { LeggingsType type; const char* lockMsg; int tierPips; };
            LegEntry legs[8] = {
                { LeggingsType::NONE, "Unarmored default", 0 },
                { LeggingsType::COMMONERS_BREECHES, "Ruined Castle - Awakening Pedestal", 1 },
                { LeggingsType::CRUSADER_GREAVES, "Zyphden - Defeat Knight Lenon in courtyard spar", 2 },
                { LeggingsType::VANGUARD_GREAVES, "Blacksmith Varek - Armor Forge", 2 },
                { LeggingsType::SHADOW_LEGGINGS, "Varek's Forge - Craft with Mist Essence", 3 },
                { LeggingsType::SCRAP_GREAVES, "Blacksmith Varek - Armor Forge", 2 },
                { LeggingsType::BASALT_GREAVES, "Blacksmith Varek - Armor Forge", 3 },
                { LeggingsType::FROSTWARDEN_GREAVES, "Blacksmith Varek - Highland Troll Crafting", 3 }
            };

            int unlockedCount = 0;
            for (int i = 0; i < (int)std::size(legs); ++i) {
                if (player.GetInventory().HasLeggings(legs[i].type)) unlockedCount++;
            }

            // Left Grid Container
            DrawRectangleRounded(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 15, 20, 30, 240 });
            DrawRectangleRoundedLines(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 48, 65, 92, 180 });
            std::string gridTitle = "LEGGINGS & GREAVES (" + std::to_string(unlockedCount) + "/" + std::to_string(std::size(legs)) + " UNLOCKED)";
            DrawText(gridTitle.c_str(), (int)rightX + 28, (int)drawerListY + 8, 12, Color{ 255, 215, 90, 240 });
            DrawLine((int)rightX + 28, (int)drawerListY + 24, (int)(rightX + 16.0f + gridW - 12.0f), (int)drawerListY + 24, Color{ 42, 58, 82, 160 });

            if (s_inspectedLegs < 0 || s_inspectedLegs >= (int)std::size(legs)) s_inspectedLegs = 2;

            for (int i = 0; i < (int)std::size(legs); ++i) {
                int col = i % 3;
                int row = i / 3;
                Rectangle bRec = { gridStartX + (float)col * (bSize + bSpacingX), gridStartY + (float)row * (bSize + bSpacingY), bSize, bSize };
                bool isUnlocked = player.GetInventory().HasLeggings(legs[i].type);
                bool isEquipped = (player.GetLeggings() == legs[i].type);
                bool isSelected = (s_inspectedLegs == i);
                bool isHov = CheckCollisionPointRec(mousePos, bRec);

                if (isHov) s_inspectedLegs = i;
                if (isHov && isMousePressed) {
                    if (isUnlocked && !isEquipped) {
                        player.SetLeggings(legs[i].type);
                        SoundFX::PlayEquipWeapon();
                    }
                }

                DrawEquipmentBubble(bRec, isSelected, isHov, isEquipped, !isUnlocked, legs[i].tierPips);
                Vector2 iconCenter = { bRec.x + bSize * 0.5f, bRec.y + bSize * 0.5f };
                DrawLeggingsSlotIcon(iconCenter, legs[i].type, 1.25f, isUnlocked, isEquipped);
            }

            // Right Inspection Panel
            LeggingsStats st = GetLeggingsStats(legs[s_inspectedLegs].type);
            ArmorSetInfo setInfo = GetArmorSetInfo(st.setAffinity);
            bool isInspUnlocked = player.GetInventory().HasLeggings(legs[s_inspectedLegs].type);
            bool isInspEquipped = (player.GetLeggings() == legs[s_inspectedLegs].type);

            DrawRectangleRounded(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, Color{ 16, 22, 34, 245 });
            DrawRectangleRoundedLines(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, isInspEquipped ? Color{ 255, 215, 80, 220 } : Color{ 52, 72, 105, 200 });

            // Preview bubble
            Rectangle prevRec = { panelX + 14.0f, drawerListY + 12.0f, 54.0f, 54.0f };
            DrawEquipmentBubble(prevRec, true, false, isInspEquipped, !isInspUnlocked, legs[s_inspectedLegs].tierPips);
            DrawLeggingsSlotIcon({ prevRec.x + prevRec.width * 0.5f, prevRec.y + prevRec.height * 0.5f }, legs[s_inspectedLegs].type, 1.35f, isInspUnlocked, isInspEquipped);

            // Header titles
            float titleX = panelX + 78.0f;
            DrawText(st.name.c_str(), (int)titleX, (int)drawerListY + 12, 15, isInspEquipped ? Color{ 255, 225, 100, 255 } : Color{ 235, 245, 255, 255 });
            DrawText("Armored Greaves & Boots", (int)titleX, (int)drawerListY + 31, 11, Color{ 160, 180, 205, 200 });
            std::string affText = (st.setAffinity != ArmorSet::NONE) ? ("Set: " + setInfo.name) : "Standalone Greaves";
            DrawText(affText.c_str(), (int)titleX, (int)drawerListY + 47, 11, Color{ 120, 240, 190, 240 });

            // Action button
            Rectangle btnRec = { panelX + panelW - 98.0f, drawerListY + 12.0f, 86.0f, 28.0f };
            bool isBtnHov = CheckCollisionPointRec(mousePos, btnRec);
            if (isInspEquipped) {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 255, 215, 80, 230 });
                DrawText("EQUIPPED", (int)btnRec.x + 14, (int)btnRec.y + 7, 11, Color{ 14, 18, 26, 255 });
            } else if (isInspUnlocked) {
                DrawRectangleRounded(btnRec, 0.25f, 4, isBtnHov ? Color{ 60, 100, 160, 255 } : Color{ 36, 56, 88, 230 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, isBtnHov ? Color{ 140, 215, 255, 255 } : Color{ 70, 110, 175, 220 });
                DrawText("EQUIP", (int)btnRec.x + 24, (int)btnRec.y + 7, 11, isBtnHov ? Color{ 255, 255, 255, 255 } : Color{ 200, 225, 255, 240 });
                if (isBtnHov && isMousePressed) {
                    player.SetLeggings(legs[s_inspectedLegs].type);
                    SoundFX::PlayEquipWeapon();
                }
            } else {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 26, 30, 40, 200 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, Color{ 60, 40, 45, 180 });
                DrawText("LOCKED", (int)btnRec.x + 20, (int)btnRec.y + 7, 11, Color{ 235, 95, 95, 220 });
            }

            // Divider
            DrawLine((int)panelX + 14, (int)drawerListY + 74, (int)(panelX + panelW - 14), (int)drawerListY + 74, Color{ 45, 62, 90, 180 });

            // Stats
            DrawText("COMBAT ATTRIBUTES & BONUSES", (int)panelX + 14, (int)drawerListY + 82, 11, Color{ 255, 215, 90, 240 });
            LeggingsStats curSt = GetLeggingsStats(player.GetLeggings());
            int defDelta = (int)(st.defense - curSt.defense);
            int willDelta = (int)(st.willBonus - curSt.willBonus);
            int hpDelta = (int)(st.maxHpBonus - curSt.maxHpBonus);

            std::string sLine1 = "Def: +" + std::to_string((int)st.defense);
            if (!isInspEquipped && defDelta != 0) sLine1 += " (" + std::string(defDelta > 0 ? "+" : "") + std::to_string(defDelta) + ")";
            sLine1 += " | Will: +" + std::to_string((int)st.willBonus);
            if (!isInspEquipped && willDelta != 0) sLine1 += " (" + std::string(willDelta > 0 ? "+" : "") + std::to_string(willDelta) + ")";
            if (st.maxHpBonus > 0.0f || curSt.maxHpBonus > 0.0f) {
                sLine1 += " | HP: +" + std::to_string((int)st.maxHpBonus);
                if (!isInspEquipped && hpDelta != 0) sLine1 += " (" + std::string(hpDelta > 0 ? "+" : "") + std::to_string(hpDelta) + ")";
            }
            DrawText(sLine1.c_str(), (int)panelX + 14, (int)drawerListY + 100, 12, Color{ 105, 240, 185, 240 });

            float curY = drawerListY + 118.0f;
            if (st.setAffinity != ArmorSet::NONE) {
                std::string synLine = "[Synergy]: " + setInfo.passiveName + " (" + (player.GetActiveArmorSet() == st.setAffinity ? "3/3 Active" : "1/3 Inactive") + ")";
                DrawText(synLine.c_str(), (int)panelX + 14, (int)curY, 11, (player.GetActiveArmorSet() == st.setAffinity) ? Color{ 255, 220, 95, 240 } : Color{ 150, 170, 195, 200 });
                curY += 20.0f;
            } else {
                curY += 4.0f;
            }

            // Divider
            DrawLine((int)panelX + 14, (int)curY, (int)(panelX + panelW - 14), (int)curY, Color{ 45, 62, 90, 180 });
            curY += 8.0f;

            // Lore
            DrawText("ARMAMENT LORE & HISTORY", (int)panelX + 14, (int)curY, 11, Color{ 255, 215, 90, 240 });
            curY += 18.0f;
            FontManager::DrawTextWrapped(st.description, (int)panelX + 14, (int)curY, (int)panelW - 28, 11, 3, Color{ 185, 205, 230, 220 });

            // Source at bottom
            float sourceY = drawerListY + panelH - 32.0f;
            DrawLine((int)panelX + 14, (int)sourceY - 6.0f, (int)(panelX + panelW - 14), (int)sourceY - 6.0f, Color{ 45, 62, 90, 160 });
            if (isInspUnlocked) {
                DrawText("STATUS // Acquired and ready for battle", (int)panelX + 14, (int)sourceY, 11, Color{ 110, 240, 180, 240 });
            } else {
                std::string lMsg = "LOCKED // " + std::string(legs[s_inspectedLegs].lockMsg);
                DrawText(lMsg.c_str(), (int)panelX + 14, (int)sourceY, 11, Color{ 255, 95, 95, 240 });
            }
        } else if (s_selectedSlot == 3) {
            // =========================================================
            // 4. TOTEM RELIC CATALOG GRID & INSPECTION
            // =========================================================
            struct TotemListEntry { TotemType type; const char* lockMsg; int tierPips; };
            TotemListEntry totems[GameLimits::ATTUNABLE_TOTEM_COUNT] = {
                { TotemType::TOTEM_SANITY, "Ruined Castle - Awakening Pedestal", 2 },
                { TotemType::TOTEM_INSANITY, "Northern Crags - Berserker Troll Boss (25% drop)", 4 },
                { TotemType::TOTEM_MIST, "Restless Mist Forest - Gale Archer Boss (15% drop)", 3 },
                { TotemType::CRUSADER_TOTEM, "Zyphden - Spar with Knight Lenon (25% drop)", 3 },
                { TotemType::ELEMENTAL_CORE_TOTEM, "Quarry Depths - Elemental Golem Boss (25% drop)", 4 },
                { TotemType::TOTEM_PHLEGMATIC_CORE, "Northern Crags - Altar of Cold Solitude / Torvald Bounty", 4 }
            };

            int unlockedCount = 0;
            for (int i = 0; i < GameLimits::ATTUNABLE_TOTEM_COUNT; ++i) {
                if (player.GetInventory().HasTotem(totems[i].type)) unlockedCount++;
            }

            // Left Grid Container
            DrawRectangleRounded(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 15, 20, 30, 240 });
            DrawRectangleRoundedLines(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 48, 65, 92, 180 });
            std::string gridTitle = "ATTUNED PRIMAL CORES (" + std::to_string(unlockedCount) + "/" + std::to_string(GameLimits::ATTUNABLE_TOTEM_COUNT) + " UNLOCKED)";
            DrawText(gridTitle.c_str(), (int)rightX + 28, (int)drawerListY + 8, 12, Color{ 255, 215, 90, 240 });
            DrawLine((int)rightX + 28, (int)drawerListY + 24, (int)(rightX + 16.0f + gridW - 12.0f), (int)drawerListY + 24, Color{ 42, 58, 82, 160 });

            if (s_inspectedTotem < 0 || s_inspectedTotem >= GameLimits::ATTUNABLE_TOTEM_COUNT) s_inspectedTotem = 0;

            for (int i = 0; i < GameLimits::ATTUNABLE_TOTEM_COUNT; ++i) {
                int col = i % 3;
                int row = i / 3;
                Rectangle bRec = { gridStartX + (float)col * (bSize + bSpacingX), gridStartY + (float)row * (bSize + bSpacingY), bSize, bSize };
                bool isUnlocked = player.GetInventory().HasTotem(totems[i].type);
                bool isEquipped = (player.GetTotem() == totems[i].type);
                bool isSelected = (s_inspectedTotem == i);
                bool isHov = CheckCollisionPointRec(mousePos, bRec);

                if (isHov) s_inspectedTotem = i;
                if (isHov && isMousePressed) {
                    if (isUnlocked) {
                        player.SetTotem(isEquipped ? TotemType::NONE : totems[i].type);
                        SoundFX::PlayEquipWeapon();
                    }
                }

                DrawEquipmentBubble(bRec, isSelected, isHov, isEquipped, !isUnlocked, totems[i].tierPips);
                Vector2 iconCenter = { bRec.x + bSize * 0.5f, bRec.y + bSize * 0.5f };
                DrawTotemSlotIcon(iconCenter, totems[i].type, 1.35f, isUnlocked, isEquipped);
            }

            // Right Inspection Panel
            TotemStats st = GetTotemStats(totems[s_inspectedTotem].type);
            bool isInspUnlocked = player.GetInventory().HasTotem(totems[s_inspectedTotem].type);
            bool isInspEquipped = (player.GetTotem() == totems[s_inspectedTotem].type);

            DrawRectangleRounded(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, Color{ 16, 22, 34, 245 });
            DrawRectangleRoundedLines(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, isInspEquipped ? st.glowColor : Color{ 52, 72, 105, 200 });

            // Preview bubble
            Rectangle prevRec = { panelX + 14.0f, drawerListY + 12.0f, 54.0f, 54.0f };
            DrawEquipmentBubble(prevRec, true, false, isInspEquipped, !isInspUnlocked, totems[s_inspectedTotem].tierPips);
            DrawTotemSlotIcon({ prevRec.x + prevRec.width * 0.5f, prevRec.y + prevRec.height * 0.5f }, totems[s_inspectedTotem].type, 1.4f, isInspUnlocked, isInspEquipped);

            // Header titles
            float titleX = panelX + 78.0f;
            DrawText(st.name.c_str(), (int)titleX, (int)drawerListY + 12, 15, isInspEquipped ? st.glowColor : Color{ 235, 245, 255, 255 });
            DrawText("Primal Humoral Relic", (int)titleX, (int)drawerListY + 31, 11, Color{ 160, 180, 205, 200 });
            DrawText("Attunement: Core Soul Resonance", (int)titleX, (int)drawerListY + 47, 11, Color{ 120, 240, 190, 240 });

            // Action button
            Rectangle btnRec = { panelX + panelW - 98.0f, drawerListY + 12.0f, 86.0f, 28.0f };
            bool isBtnHov = CheckCollisionPointRec(mousePos, btnRec);
            if (isInspEquipped) {
                DrawRectangleRounded(btnRec, 0.25f, 4, st.glowColor);
                DrawText("ATTUNED", (int)btnRec.x + 16, (int)btnRec.y + 7, 11, Color{ 14, 18, 26, 255 });
            } else if (isInspUnlocked) {
                DrawRectangleRounded(btnRec, 0.25f, 4, isBtnHov ? Color{ 60, 100, 160, 255 } : Color{ 36, 56, 88, 230 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, isBtnHov ? Color{ 140, 215, 255, 255 } : Color{ 70, 110, 175, 220 });
                DrawText("ATTUNE", (int)btnRec.x + 20, (int)btnRec.y + 7, 11, isBtnHov ? Color{ 255, 255, 255, 255 } : Color{ 200, 225, 255, 240 });
                if (isBtnHov && isMousePressed) {
                    player.SetTotem(totems[s_inspectedTotem].type);
                    SoundFX::PlayEquipWeapon();
                }
            } else {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 26, 30, 40, 200 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, Color{ 60, 40, 45, 180 });
                DrawText("LOCKED", (int)btnRec.x + 20, (int)btnRec.y + 7, 11, Color{ 235, 95, 95, 220 });
            }

            // Divider
            DrawLine((int)panelX + 14, (int)drawerListY + 74, (int)(panelX + panelW - 14), (int)drawerListY + 74, Color{ 45, 62, 90, 180 });

            // Totem Buff and RED Debuff Text
            DrawText("RELIC RESONANCES & HARMONIES", (int)panelX + 14, (int)drawerListY + 82, 11, Color{ 255, 215, 90, 240 });
            DrawTotemBuffDebuffText(st.buffText, st.nerfText, (int)panelX + 14, (int)drawerListY + 100, 12);

            // Special notes for totems
            if (totems[s_inspectedTotem].type == TotemType::CRUSADER_TOTEM) {
                DrawText("[Passive]: Will of Atlas", (int)panelX + 14, (int)drawerListY + 120, 11, Color{ 255, 215, 80, 255 });
                FontManager::DrawTextWrapped("When Health falls below 20%, automatically awakens 4 seconds of complete invulnerability (60s cooldown).",
                    (int)panelX + 14, (int)drawerListY + 135, (int)panelW - 28, 11, 3, Color{ 215, 230, 255, 220 });
            } else if (totems[s_inspectedTotem].type == TotemType::ELEMENTAL_CORE_TOTEM) {
                DrawText("[Companion]: Mini Elemental Golem (380 HP, 42 DEF)", (int)panelX + 14, (int)drawerListY + 120, 11, Color{ 255, 150, 50, 255 });
                FontManager::DrawTextWrapped("Wields 3 new combat attack patterns: Heavy AoE Slam, Forward Bash Knockback, and Elemental Spike Throw.",
                    (int)panelX + 14, (int)drawerListY + 135, (int)panelW - 28, 11, 3, Color{ 215, 230, 255, 220 });
            } else if (totems[s_inspectedTotem].type == TotemType::TOTEM_MIST) {
                DrawText("[Companion]: Mist Fairy (+20% Crit, +35% Crit Dmg)", (int)panelX + 14, (int)drawerListY + 120, 11, Color{ 110, 245, 225, 255 });
                FontManager::DrawTextWrapped("A loyal vapor sprite circles the bearer, guiding critical strikes and periodically generating restorative mist healing orbs (+30 HP).",
                    (int)panelX + 14, (int)drawerListY + 135, (int)panelW - 28, 11, 3, Color{ 215, 230, 255, 220 });
            } else if (totems[s_inspectedTotem].type == TotemType::TOTEM_INSANITY) {
                DrawText("[Trait]: Reckless Blood Frenzy", (int)panelX + 14, (int)drawerListY + 120, 11, Color{ 255, 75, 75, 255 });
                FontManager::DrawTextWrapped("Bestows +65% overall damage, +35% speed, and +25% attack speed, but cuts defense by 50% and causes bearer to suffer x1.5 incoming damage. When Health drops below 40%, overall damage surges to +135%!",
                    (int)panelX + 14, (int)drawerListY + 135, (int)panelW - 28, 11, 3, Color{ 255, 185, 185, 220 });
            } else if (totems[s_inspectedTotem].type == TotemType::TOTEM_PHLEGMATIC_CORE) {
                DrawText("[Signature]: Phlegmatic Apathy & Cold Solitude", (int)panelX + 14, (int)drawerListY + 120, 11, Color{ 130, 225, 255, 255 });
                FontManager::DrawTextWrapped("Grants -80% knockback received, +30 Max HP, +25 Will, and converts 25% of sustained damage into temporary Gray Health that can be reclaimed by landing counter-attacks. Chills foes on hit.",
                    (int)panelX + 14, (int)drawerListY + 135, (int)panelW - 28, 11, 3, Color{ 200, 240, 255, 220 });
            }

            // Divider
            DrawLine((int)panelX + 14, (int)drawerListY + 175, (int)(panelX + panelW - 14), (int)drawerListY + 175, Color{ 45, 62, 90, 180 });

            // Lore
            DrawText("RELIC LORE & CODEX", (int)panelX + 14, (int)drawerListY + 183, 11, Color{ 255, 215, 90, 240 });
            FontManager::DrawTextWrapped(st.description, (int)panelX + 14, (int)drawerListY + 201, (int)panelW - 28, 11, 4, Color{ 185, 205, 230, 220 });

            // Source at bottom
            float sourceY = drawerListY + panelH - 32.0f;
            DrawLine((int)panelX + 14, (int)sourceY - 6.0f, (int)(panelX + panelW - 14), (int)sourceY - 6.0f, Color{ 45, 62, 90, 160 });
            if (isInspUnlocked) {
                DrawText("STATUS // Attuned and resonating with primal power", (int)panelX + 14, (int)sourceY, 11, Color{ 110, 240, 180, 240 });
            } else {
                std::string lMsg = "LOCKED // " + std::string(totems[s_inspectedTotem].lockMsg);
                DrawText(lMsg.c_str(), (int)panelX + 14, (int)sourceY, 11, Color{ 255, 95, 95, 240 });
            }
        } else if (s_selectedSlot == 4) {
            // =========================================================
            // 5. WEAPON ARSENAL GRID & INSPECTION
            // =========================================================
            struct WepListEntry { WeaponType type; const char* lockMsg; int tierPips; };
            WepListEntry weps[10] = {
                { WeaponType::SWORD, "Ruined Castle - Awakening Pedestal", 1 },
                { WeaponType::DAGGERS, "Ruined Castle - Upper Gallery Pedestal", 1 },
                { WeaponType::MACE, "Ruined Castle - North Crypt Pedestal", 2 },
                { WeaponType::HALBERD, "Zyphden - Royal Guard Armory Pedestal", 2 },
                { WeaponType::TRUTH_SEEKER, "Royal Arsenal - High Sanctum Pedestal", 3 },
                { WeaponType::ZEPHYR_DAGGERS, "Varek's Forge - Crafted from Shadow & Mist Essences", 3 },
                { WeaponType::WINDRUNNER_RECURVE, "Varek's Forge - Crafted from Gale Archer Trophies", 3 },
                { WeaponType::SUN_CLAYMORE, "Varek's Forge - Forged from Tempered Ore & Solar Slag", 3 },
                { WeaponType::STORM_GLAIVE, "Varek's Forge - Forged from Gale Mist & Tempered Steel", 3 },
                { WeaponType::GLACIAL_CLEAVER, "Varek's Forge - Forged from Berserker Troll Horn", 3 }
            };

            int unlockedCount = 0;
            for (int i = 0; i < (int)std::size(weps); ++i) {
                if (player.GetInventory().HasWeapon(weps[i].type)) unlockedCount++;
            }

            // Left Grid Container
            DrawRectangleRounded(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 15, 20, 30, 240 });
            DrawRectangleRoundedLines(Rectangle{ rightX + 16.0f, drawerListY, gridW, gridH }, 0.04f, 4, Color{ 48, 65, 92, 180 });
            std::string gridTitle = "ROYAL WEAPON ARSENAL (" + std::to_string(unlockedCount) + "/" + std::to_string(std::size(weps)) + " UNLOCKED)";
            DrawText(gridTitle.c_str(), (int)rightX + 28, (int)drawerListY + 8, 12, Color{ 255, 215, 90, 240 });
            DrawLine((int)rightX + 28, (int)drawerListY + 24, (int)(rightX + 16.0f + gridW - 12.0f), (int)drawerListY + 24, Color{ 42, 58, 82, 160 });

            if (s_inspectedWep < 0 || s_inspectedWep >= (int)std::size(weps)) s_inspectedWep = 0;

            for (int i = 0; i < (int)std::size(weps); ++i) {
                int col = i % 3;
                int row = i / 3;
                Rectangle bRec = { gridStartX + (float)col * (bSize + bSpacingX), gridStartY + (float)row * (bSize + bSpacingY), bSize, bSize };
                bool isUnlocked = player.GetInventory().HasWeapon(weps[i].type);
                bool isEquipped = (player.GetWeapon() == weps[i].type);
                bool isSelected = (s_inspectedWep == i);
                bool isHov = CheckCollisionPointRec(mousePos, bRec);
                int upgLvl = player.GetInventory().GetWeaponUpgrade(weps[i].type);

                if (isHov) s_inspectedWep = i;
                if (isHov && isMousePressed) {
                    if (isUnlocked && !isEquipped) {
                        player.SetWeapon(weps[i].type);
                        SoundFX::PlayEquipWeapon();
                    }
                }

                DrawEquipmentBubble(bRec, isSelected, isHov, isEquipped, !isUnlocked, weps[i].tierPips);
                Vector2 iconCenter = { bRec.x + bSize * 0.5f, bRec.y + bSize * 0.5f };
                DrawWeaponSlotIcon(iconCenter, weps[i].type, 1.2f, isUnlocked, isEquipped);

                if (isUnlocked && upgLvl > 0) {
                    std::string badge = "+" + std::to_string(upgLvl);
                    DrawText(badge.c_str(), (int)(bRec.x + bRec.width - 18), (int)(bRec.y + bRec.height - 13), 10, Color{ 255, 220, 100, 255 });
                }
            }

            // Right Inspection Panel
            int curUpg = player.GetInventory().GetWeaponUpgrade(weps[s_inspectedWep].type);
            WeaponStats st = GetWeaponStats(weps[s_inspectedWep].type, curUpg);
            bool isInspUnlocked = player.GetInventory().HasWeapon(weps[s_inspectedWep].type);
            bool isInspEquipped = (player.GetWeapon() == weps[s_inspectedWep].type);

            DrawRectangleRounded(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, Color{ 16, 22, 34, 245 });
            DrawRectangleRoundedLines(Rectangle{ panelX, drawerListY, panelW, panelH }, 0.04f, 4, isInspEquipped ? Color{ 255, 215, 80, 220 } : Color{ 52, 72, 105, 200 });

            // Preview bubble
            Rectangle prevRec = { panelX + 14.0f, drawerListY + 12.0f, 54.0f, 54.0f };
            DrawEquipmentBubble(prevRec, true, false, isInspEquipped, !isInspUnlocked, weps[s_inspectedWep].tierPips);
            DrawWeaponSlotIcon({ prevRec.x + prevRec.width * 0.5f, prevRec.y + prevRec.height * 0.5f }, weps[s_inspectedWep].type, 1.35f, isInspUnlocked, isInspEquipped);

            // Header titles
            float titleX = panelX + 78.0f;
            std::string wTitle = st.name + (curUpg > 0 ? (" +" + std::to_string(curUpg)) : "");
            DrawText(wTitle.c_str(), (int)titleX, (int)drawerListY + 12, 15, isInspEquipped ? Color{ 255, 225, 100, 255 } : Color{ 235, 245, 255, 255 });
            DrawText("Royal Armory Weapon", (int)titleX, (int)drawerListY + 31, 11, Color{ 160, 180, 205, 200 });
            std::string upgText = (curUpg > 0) ? ("Tempered + " + std::to_string(curUpg) + " at Varek's Forge") : "Standard Tier Armament";
            DrawText(upgText.c_str(), (int)titleX, (int)drawerListY + 47, 11, Color{ 120, 240, 190, 240 });

            // Action button
            Rectangle btnRec = { panelX + panelW - 98.0f, drawerListY + 12.0f, 86.0f, 28.0f };
            bool isBtnHov = CheckCollisionPointRec(mousePos, btnRec);
            if (isInspEquipped) {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 255, 215, 80, 230 });
                DrawText("EQUIPPED", (int)btnRec.x + 14, (int)btnRec.y + 7, 11, Color{ 14, 18, 26, 255 });
            } else if (isInspUnlocked) {
                DrawRectangleRounded(btnRec, 0.25f, 4, isBtnHov ? Color{ 60, 100, 160, 255 } : Color{ 36, 56, 88, 230 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, isBtnHov ? Color{ 140, 215, 255, 255 } : Color{ 70, 110, 175, 220 });
                DrawText("EQUIP", (int)btnRec.x + 24, (int)btnRec.y + 7, 11, isBtnHov ? Color{ 255, 255, 255, 255 } : Color{ 200, 225, 255, 240 });
                if (isBtnHov && isMousePressed) {
                    player.SetWeapon(weps[s_inspectedWep].type);
                    SoundFX::PlayEquipWeapon();
                }
            } else {
                DrawRectangleRounded(btnRec, 0.25f, 4, Color{ 26, 30, 40, 200 });
                DrawRectangleRoundedLines(btnRec, 0.25f, 4, Color{ 60, 40, 45, 180 });
                DrawText("LOCKED", (int)btnRec.x + 20, (int)btnRec.y + 7, 11, Color{ 235, 95, 95, 220 });
            }

            // Divider
            DrawLine((int)panelX + 14, (int)drawerListY + 74, (int)(panelX + panelW - 14), (int)drawerListY + 74, Color{ 45, 62, 90, 180 });

            // Stats
            DrawText("COMBAT ATTRIBUTES & METRICS", (int)panelX + 14, (int)drawerListY + 82, 11, Color{ 255, 215, 90, 240 });
            int equippedUpg = player.GetInventory().GetWeaponUpgrade(player.GetWeapon());
            WeaponStats curSt = GetWeaponStats(player.GetWeapon(), equippedUpg);
            int lightDelta = (int)(st.lightDamage - curSt.lightDamage);
            int heavyDelta = (int)(st.heavyDamage - curSt.heavyDamage);
            int reachDelta = (int)(st.lightRange - curSt.lightRange);
            int pierceDelta = (int)((st.defensePierceBonus - curSt.defensePierceBonus) * 100.0f);

            std::string sLine1 = "Light: " + std::to_string((int)st.lightDamage);
            if (!isInspEquipped && lightDelta != 0) sLine1 += " (" + std::string(lightDelta > 0 ? "+" : "") + std::to_string(lightDelta) + ")";
            sLine1 += " | Heavy: " + std::to_string((int)st.heavyDamage);
            if (!isInspEquipped && heavyDelta != 0) sLine1 += " (" + std::string(heavyDelta > 0 ? "+" : "") + std::to_string(heavyDelta) + ")";
            DrawText(sLine1.c_str(), (int)panelX + 14, (int)drawerListY + 100, 12, Color{ 105, 240, 185, 240 });

            std::string sLine2 = "Reach: " + std::to_string((int)st.lightRange) + "px";
            if (!isInspEquipped && reachDelta != 0) sLine2 += " (" + std::string(reachDelta > 0 ? "+" : "") + std::to_string(reachDelta) + ")";
            sLine2 += " | Pierce: " + std::to_string((int)(st.defensePierceBonus * 100.0f)) + "%";
            if (!isInspEquipped && pierceDelta != 0) sLine2 += " (" + std::string(pierceDelta > 0 ? "+" : "") + std::to_string(pierceDelta) + "%)";
            DrawText(sLine2.c_str(), (int)panelX + 14, (int)drawerListY + 118, 12, Color{ 140, 210, 255, 240 });

            // Special weapon archetype traits
            if (weps[s_inspectedWep].type == WeaponType::WINDRUNNER_RECURVE) {
                DrawText("[Archetype]: Charged shots break enemy guard & stun. Rapid pierce velocity.", (int)panelX + 14, (int)drawerListY + 138, 11, Color{ 255, 220, 95, 240 });
            } else if (weps[s_inspectedWep].type == WeaponType::ZEPHYR_DAGGERS) {
                DrawText("[Archetype]: High crit multiplier & rapid flurry strikes with evasive momentum.", (int)panelX + 14, (int)drawerListY + 138, 11, Color{ 110, 245, 225, 240 });
            } else if (weps[s_inspectedWep].type == WeaponType::SUN_CLAYMORE) {
                DrawText("[Archetype]: Devastating heavy cleaves with unbreakable hyper-armor posture.", (int)panelX + 14, (int)drawerListY + 138, 11, Color{ 255, 160, 60, 240 });
            } else {
                DrawText("[Archetype]: Balanced physical weapon with high deflection composure.", (int)panelX + 14, (int)drawerListY + 138, 11, Color{ 160, 185, 215, 220 });
            }

            // Divider
            DrawLine((int)panelX + 14, (int)drawerListY + 158, (int)(panelX + panelW - 14), (int)drawerListY + 158, Color{ 45, 62, 90, 180 });

            // Lore
            DrawText("WEAPON HISTORY & CODEX", (int)panelX + 14, (int)drawerListY + 166, 11, Color{ 255, 215, 90, 240 });
            FontManager::DrawTextWrapped(st.description, (int)panelX + 14, (int)drawerListY + 186, (int)panelW - 28, 11, 4, Color{ 185, 205, 230, 220 });

            // Source at bottom
            float sourceY = drawerListY + panelH - 32.0f;
            DrawLine((int)panelX + 14, (int)sourceY - 6.0f, (int)(panelX + panelW - 14), (int)sourceY - 6.0f, Color{ 45, 62, 90, 160 });
            if (isInspUnlocked) {
                DrawText("STATUS // Unlocked and ready for battle", (int)panelX + 14, (int)sourceY, 11, Color{ 110, 240, 180, 240 });
            } else {
                std::string lMsg = "LOCKED // " + std::string(weps[s_inspectedWep].lockMsg);
                DrawText(lMsg.c_str(), (int)panelX + 14, (int)sourceY, 11, Color{ 255, 95, 95, 240 });
            }
        }
    } else if (activeTab == 2) {
        // =========================================================================
        // TAB 2: THE HUMORAL BESTIARY & TACTICAL CODEX (v0.0.1e)
        // Displays all 9 enemy archetypes, animated 3D silhouettes, humoral affinities,
        // tactical weaknesses, and 3-tier unlockable lore seals based on kill counts!
        // =========================================================================
        static int s_selectedBestiary = 0;
        struct BestiaryEntry {
            const char* name;
            const char* latinTitle;
            const char* humorName;
            Color humorColor;
            int killIdx;
            const char* combatStyle;
            const char* weakness;
            const char* drops;
            const char* lore1; // 1 kill
            const char* lore5; // 5 kills
            const char* lore15;// 15 kills
        };

        static const BestiaryEntry s_entries[GameLimits::BESTIARY_COUNT] = {
            {
                "Corrupted Slasher", "Sanguis Corruptus", "SANGUINE (Blood)", Color{ 240, 80, 70, 255 }, 0,
                "Swarms in agile packs. Erratic curved-blade lunges.",
                "Easily deflected with shield parries into lethal ripostes.",
                "Silver Shingles (1-5), Elixir Flasks (15%)",
                "Maddened survivors who drank the tainted fountain waters of Old Tydeshiire.",
                "Their flesh has ossified into crude weapon fragments; they feel no fatigue.",
                "Vanguard records confirm they retain fragments of human grief in their dying gasps."
            },
            {
                "Corrupted Archer", "Sagittarius Profanus", "SANGUINE (Blood)", Color{ 240, 80, 70, 255 }, 1,
                "Maintains long distance. Piercing crimson arrows.",
                "Stationary during reload; dashing disrupts aiming lines.",
                "Silver Shingles (1-5), Elixir Flasks (15%)",
                "Former royal garrison marksmen blinded by sanguine miasma.",
                "Their bows are strung with sinew pulled from fallen comrades.",
                "Target their quiver cords to inflict immediate critical poise damage."
            },
            {
                "Void Pyromancer", "Ignis Abyssi", "CHOLERIC (Fire)", Color{ 255, 120, 40, 255 }, 2,
                "Casts tracking void orbs and eruptive flame fissures.",
                "Light defense; staggering with daggers or halberd interrupts casting.",
                "Silver Shingles (10-25), Elixir Flasks (35%)",
                "Heretical scholars seeking warmth in the darkness after the Citadel fell.",
                "Their occult flame feeds upon the victim's internal composure.",
                "Ancient grimoires warn that extinguishing their embers summons abyssal echoes."
            },
            {
                "Iron Brute", "Malleus Furens", "CHOLERIC (Fire)", Color{ 255, 120, 40, 255 }, 3,
                "Relentless greatclub cleaves with unstoppable momentum.",
                "War Mace skull crusher inflicts -20% armor fracture, shattering hyper-armor.",
                "Silver Shingles (10-25), Elixir Flasks (35%)",
                "Vanguard infantry welded into their armor plates by heat and dark magic.",
                "Their iron plating deflects slashing blades; blunt force is mandatory.",
                "Each fallen brute yields tempered slag prized by master blacksmiths."
            },
            {
                "The Grand Inquisitor", "Inquisitor Magnus", "CHOLERIC (Fire Boss)", Color{ 255, 60, 50, 255 }, 4,
                "Radial nova starbursts and crushing overhead judgment smites.",
                "Parrying his ground slam exposes his guard for 3 full seconds.",
                "High Shingles (100-150), 2x Guaranteed Elixir Flasks",
                "High prelate of the broken church, burning heresy where no truth remains.",
                "Wields the censer of cleansing flame, turning holy hymns into deadly shockwaves.",
                "His defeat shattered the cathedral wards, opening the highway to the Crossroads."
            },
            {
                "Ore Golem & Mini Golems", "Geomantis Primordialis", "MELANCHOLIC (Earth)", Color{ 230, 180, 60, 255 }, 5,
                "Impenetrable stone phalanx with long-range 7-second artillery snipes.",
                "Blunt War Mace deals devastating crushing damage and breaks stone carapaces.",
                "Ore Scraps (Exclusive Crafting Material), Silver Shingles",
                "Animated quarry slag and volcanic basalt bound by ancient geomancy.",
                "Mini golems detach from the parent core to flank and suppress intruders.",
                "Their dense obsidian hearts retain primordial heat and grant unbreakable will."
            },
            {
                "Elemental Golem Titan", "Colossus Elementorum", "MELANCHOLIC (Earth Boss)", Color{ 245, 160, 40, 255 }, 6,
                "Sweeping basalt spires, minion summons, and earthquake quakes.",
                "Shattering legs with heavy weapon combos interrupts his spire eruption.",
                "Elemental Core Totem (25% Drop Rate), High Shingle Stashes",
                "Ancient guardian slumbering in the basalt depths since King Kreptu's father.",
                "Channels the elemental humors of the continent through his magma core.",
                "Binds the friendly elemental companion to whoever proves worthy of his totem."
            },
            {
                "Gale Bandit", "Praedo Zephyri", "PHLEGMATIC (Wind)", Color{ 90, 235, 215, 255 }, 7,
                "Acrobatic dual-dagger lunges, back-dash evasions, and throwing daggers.",
                "Extended reach weapons (Royal Halberd) out-range their knives; parry breaks poise.",
                "Essence of Mist (35% Drop Rate), Silver Shingles (Strictly NO Ore Scraps)",
                "Outcasts haunting the misty glades west of Zyphden, raiding supply convoys.",
                "They harvest Essence of Mist to brew agility tinctures and temper shadow garments.",
                "Their loyalty belongs entirely to the Whispering Altar and the Gale Archer."
            },
            {
                "The Gale Archer", "Custos Nebulae", "PHLEGMATIC (Wind Boss)", Color{ 110, 255, 230, 255 }, 8,
                "Triple windshot spread, Gale Piercer piercing beam, and Storm Rain barrages.",
                "Dash-dodging through wind beams allows closing in for rapid melee flurries.",
                "Totem of Mist (15% Drop Rate), Guaranteed Shadow Essence (1-2x), Shingles",
                "Guardian spirit of the Whispering Altar, testing mastery of wind and shadow.",
                "Her bow was carved from the roots of the world pine before the mist arrived.",
                "Her sacred Shadow Essence can forge the Shadowstrider set and Zephyr armaments."
            },
            {
                "Frost Troll", "Trollus Glacialis", "MELANCHOLIC (Frost)", Color{ 140, 205, 255, 255 }, 9,
                "Towering glacial brutes swinging spiked war clubs with frostbite cleaves.",
                "Parrying the heavy swing leaves their posture completely shattered for 2.5 seconds.",
                "Silver Shingles (15-30), Ore Scraps (30%), Elixir Flasks (25%)",
                "Ancient highland guardians dwelling in the snow-dusted crags north of Zyphden.",
                "Their flesh has absorbed decades of mountain permafrost, rendering them resilient to slashing strikes.",
                "Highland legends speak of their eternal feud against the vanguard knights of old."
            },
            {
                "Berserker Troll Warlord", "Trollus Furens", "SANGUINE (Blood Boss)", Color{ 255, 45, 60, 255 }, 10,
                "Devastating overhead ground smashes, shockwave tremors, and frenzied rage sweeps.",
                "Dodging behind his ground smite creates a massive opening for high-damage combo punish.",
                "Totem of Insanity (25% Drop Rate), High Shingle Stashes, Ore Scraps",
                "Chieftain of the northern highland troll clan who drank from the corrupted subterranean blood veins.",
                "When death nears, his blood surges with ferocious fury, shattering enemy armor and poise.",
                "Guards the ancient Blood Altar in the highest crags, waiting for a champion to challenge his throne."
            }
        };

        // Layout: Left column (list of cards), Right column (illuminated codex page)
        float bLeftX = modalX + 28.0f;
        float bLeftW = 420.0f;
        float bLeftH = modalH - 146.0f;

        float bRightX = bLeftX + bLeftW + 18.0f;
        float bRightW = modalW - bLeftW - 74.0f;
        float bRightH = bLeftH;

        // Draw Left List Card Container
        Rectangle listRec = { bLeftX, contentY, bLeftW, bLeftH };
        DrawRectangleRounded(listRec, 0.04f, 6, Color{ 12, 16, 24, 240 });
        DrawRectangleRoundedLines(listRec, 0.04f, 6, Color{ 45, 65, 95, 200 });

        float itemH = (bLeftH - 24.0f) / (float)std::size(s_entries);
        for (int i = 0; i < (int)std::size(s_entries); ++i) {
            float itemY = contentY + 12.0f + (float)i * itemH;
            Rectangle itemRec = { bLeftX + 12.0f, itemY, bLeftW - 24.0f, itemH - 4.0f };
            bool isHov = CheckCollisionPointRec(mousePos, itemRec);
            bool isSel = (s_selectedBestiary == i);

            if (isHov && isMousePressed) {
                s_selectedBestiary = i;
                SoundFX::PlayButtonClick();
            }

            int kills = player.GetInventory().GetEnemyKills(s_entries[i].killIdx);
            DrawRectangleRounded(itemRec, 0.15f, 4, isSel ? Color{ 36, 52, 78, 255 } : (isHov ? Color{ 22, 30, 46, 220 } : Color{ 16, 20, 30, 180 }));
            DrawRectangleRoundedLines(itemRec, 0.15f, 4, isSel ? Color{ 255, 215, 80, 240 } : (isHov ? s_entries[i].humorColor : Color{ 40, 55, 75, 160 }));

            // Name
            FontManager::DrawText(s_entries[i].name, (int)itemRec.x + 12, (int)itemRec.y + 6, 12, isSel ? Color{ 255, 235, 130, 255 } : Color{ 225, 235, 250, 240 }, GameFont::TITLE);
            // Humor badge
            FontManager::DrawGameTextShadow(s_entries[i].humorName, (int)itemRec.x + 12, (int)itemRec.y + 22, 9, s_entries[i].humorColor);
            // Seal Tier badge & Kills pill
            std::string tierTag = (kills >= 15) ? "[SEAL III]" : ((kills >= 5) ? "[SEAL II]" : ((kills >= 1) ? "[SEAL I]" : "[-]"));
            Color tierTagCol = (kills >= 15) ? Color{ 255, 215, 80, 255 } : ((kills >= 5) ? Color{ 100, 220, 255, 240 } : ((kills >= 1) ? Color{ 120, 255, 180, 220 } : Color{ 120, 130, 145, 160 }));
            FontManager::DrawGameTextShadow(tierTag, (int)(itemRec.x + itemRec.width - 66), (int)itemRec.y + 5, 9, tierTagCol);
            std::string killStr = "KILLS: " + std::to_string(kills);
            FontManager::DrawGameTextShadow(killStr, (int)(itemRec.x + itemRec.width - 66), (int)itemRec.y + 20, 9, (kills > 0) ? Color{ 120, 255, 180, 220 } : Color{ 140, 150, 165, 180 });
        }

        // Draw Right Codex Card Container (Illuminated Grimoire Parchment)
        Rectangle codexRec = { bRightX, contentY, bRightW, bRightH };
        DrawRectangleRounded(codexRec, 0.04f, 6, Color{ 24, 20, 18, 255 });
        DrawRectangleRoundedLines(codexRec, 0.04f, 6, Color{ 175, 130, 55, 240 });
        DrawRectangleRoundedLines(Rectangle{ bRightX + 4.0f, contentY + 4.0f, bRightW - 8.0f, bRightH - 8.0f }, 0.035f, 6, Color{ 75, 48, 25, 180 });

        const auto& cur = s_entries[s_selectedBestiary];
        int curKills = player.GetInventory().GetEnemyKills(cur.killIdx);

        // Header Title
        FontManager::DrawTextTitleCenteredShadow(cur.name, (int)(bRightX + bRightW * 0.5f), (int)contentY + 16, 20, Color{ 255, 235, 140, 255 });
        std::string sub = std::string(cur.latinTitle) + "  —  " + cur.humorName;
        FontManager::DrawTextCenteredShadow(sub, (int)(bRightX + bRightW * 0.5f), (int)contentY + 44, 12, cur.humorColor);

        // Ornamental divider
        DrawLineEx({ bRightX + 40.0f, contentY + 65.0f }, { bRightX + bRightW - 40.0f, contentY + 65.0f }, 1.5f, Color{ 175, 130, 55, 180 });

        // Tactical Combat Dossier
        float secY = contentY + 76.0f;
        FontManager::DrawText("TACTICAL COMBAT DOSSIER", (int)bRightX + 32, (int)secY, 13, Color{ 230, 195, 100, 255 }, GameFont::TITLE);
        FontManager::DrawGameTextShadow("Combat Cadence: ", (int)bRightX + 32, (int)secY + 22, 11, Color{ 180, 200, 220, 220 });
        FontManager::DrawGameTextShadow(cur.combatStyle, (int)bRightX + 135, (int)secY + 22, 11, Color{ 245, 245, 245, 255 });

        FontManager::DrawGameTextShadow("Known Weakness: ", (int)bRightX + 32, (int)secY + 40, 11, Color{ 180, 200, 220, 220 });
        FontManager::DrawGameTextShadow(cur.weakness, (int)bRightX + 135, (int)secY + 40, 11, Color{ 110, 245, 220, 255 });

        FontManager::DrawGameTextShadow("Yield / Drops: ", (int)bRightX + 32, (int)secY + 58, 11, Color{ 180, 200, 220, 220 });
        FontManager::DrawGameTextShadow(cur.drops, (int)bRightX + 135, (int)secY + 58, 11, Color{ 255, 225, 120, 255 });

        // Divider
        DrawLineEx({ bRightX + 32.0f, secY + 80.0f }, { bRightX + bRightW - 32.0f, secY + 80.0f }, 1.0f, Color{ 85, 60, 40, 180 });

        // Progressive Lore Seals & Active Hunting Perks
        float loreY = secY + 90.0f;
        FontManager::DrawText("CODEX EXCERPTS & HUNTING MASTERY PERKS", (int)bRightX + 32, (int)loreY, 13, Color{ 230, 195, 100, 255 }, GameFont::TITLE);

        // Seal 1: 1 Kill
        bool u1 = (curKills >= 1);
        FontManager::DrawGameTextShadow(u1 ? "[SEAL I - INITIATE (1 KILL)]" : "[SEAL I - LOCKED (REQUIRES 1 KILL)]", (int)bRightX + 32, (int)loreY + 20, 11, u1 ? Color{ 120, 255, 180, 240 } : Color{ 150, 150, 160, 180 });
        FontManager::DrawGameTextShadow(u1 ? cur.lore1 : "Defeat this enemy once to unlock initial reconnaissance notes.", (int)bRightX + 48, (int)loreY + 36, 10, u1 ? Color{ 230, 220, 200, 240 } : Color{ 120, 120, 130, 160 });

        // Seal 2: 5 Kills
        bool u5 = (curKills >= 5);
        FontManager::DrawGameTextShadow(u5 ? "[SEAL II - ADEPT (5 KILLS)]" : "[SEAL II - LOCKED (REQUIRES 5 KILLS)]", (int)bRightX + 32, (int)loreY + 54, 11, u5 ? Color{ 100, 220, 255, 240 } : Color{ 150, 150, 160, 180 });
        FontManager::DrawGameTextShadow(u5 ? cur.lore5 : "Achieve 5 victories to decipher deeper humoral vulnerabilities.", (int)bRightX + 48, (int)loreY + 70, 10, u5 ? Color{ 230, 220, 200, 240 } : Color{ 120, 120, 130, 160 });

        // Seal 3: 15 Kills
        bool u15 = (curKills >= 15);
        FontManager::DrawGameTextShadow(u15 ? "[SEAL III - MASTER (15 KILLS)]" : "[SEAL III - LOCKED (REQUIRES 15 KILLS)]", (int)bRightX + 32, (int)loreY + 88, 11, u15 ? Color{ 255, 215, 80, 240 } : Color{ 150, 150, 160, 180 });
        FontManager::DrawGameTextShadow(u15 ? cur.lore15 : "Achieve 15 victories to unveil ancient grimoire archives.", (int)bRightX + 48, (int)loreY + 104, 10, u15 ? Color{ 230, 220, 200, 240 } : Color{ 120, 120, 130, 160 });

        // Active Combat Perk Card
        float perkY = loreY + 124.0f;
        Rectangle perkRec = { bRightX + 32.0f, perkY, bRightW - 64.0f, 54.0f };
        DrawRectangleRounded(perkRec, 0.15f, 4, Color{ 16, 22, 34, 230 });
        DrawRectangleRoundedLines(perkRec, 0.15f, 4, u5 ? Color{ 120, 215, 255, 220 } : Color{ 55, 70, 95, 180 });

        const char* perkTitle = "COMBAT MASTERY PERK";
        const char* perkDesc = "Accumulate kills to unlock family-wide hunting traits.";
        Color perkBadgeCol = Color{ 150, 165, 185, 200 };

        if (cur.killIdx == 9 || cur.killIdx == 10) {
            perkTitle = "HUNTING PERK: TROLL SLAYER";
            perkDesc = u15 ? "ACTIVE [TIER III]: +25% Poise Dmg, Executions Heal 25 HP, +15% Base Dmg, +25 Frost Resist." :
                       (u5 ? "ACTIVE [TIER II]: +15% Damage vs Trolls, +25 Frostbite Resistance." :
                             "LOCKED: 5 kills grants +15% Dmg & Frost Resist; 15 kills grants +25% Poise & Heal on Execute.");
            perkBadgeCol = u15 ? Color{ 255, 215, 80, 255 } : (u5 ? Color{ 120, 220, 255, 255 } : Color{ 150, 165, 185, 180 });
        } else if (cur.killIdx == 5 || cur.killIdx == 6) {
            perkTitle = "HUNTING PERK: GOLEM BREAKER";
            perkDesc = u15 ? "ACTIVE [TIER III]: 25% Armor Pierce, Heavies Break Melancholic Shells, +20% Poise Dmg, +1 Scrap Mining." :
                       (u5 ? "ACTIVE [TIER II]: +20% Poise Dmg vs Golems, +1 Bonus Scrap from mining nodes." :
                             "LOCKED: 5 kills grants +20% Poise Dmg & +1 Scrap Mining; 15 kills grants 25% Armor Pierce.");
            perkBadgeCol = u15 ? Color{ 255, 215, 80, 255 } : (u5 ? Color{ 120, 220, 255, 255 } : Color{ 150, 165, 185, 180 });
        } else if (cur.killIdx == 7 || cur.killIdx == 8) {
            perkTitle = "HUNTING PERK: GALE HUNTER";
            perkDesc = u15 ? "ACTIVE [TIER III]: +20% Damage vs Gale Entities, Parries Reflect Poise Dmg, +15% Stamina Regen." :
                       (u5 ? "ACTIVE [TIER II]: +15% Stamina Regen Rate, +10% Movement Speed in Mists." :
                             "LOCKED: 5 kills grants +15% Stamina Regen; 15 kills grants +20% Dmg & Poise Reflect.");
            perkBadgeCol = u15 ? Color{ 255, 215, 80, 255 } : (u5 ? Color{ 120, 220, 255, 255 } : Color{ 150, 165, 185, 180 });
        } else {
            perkTitle = "HUNTING PERK: CULTIST PURIFIER";
            perkDesc = u15 ? "ACTIVE [TIER III]: +15% Damage vs Cultists, Kills Restore 15 Stamina and Extend Adrenaline Buff." :
                       (u5 ? "ACTIVE [TIER II]: +10% Melee Damage vs Cultists, +10 Max Composure." :
                             "LOCKED: 5 kills grants +10% Dmg & Composure; 15 kills grants Stamina and Adrenaline on kill.");
            perkBadgeCol = u15 ? Color{ 255, 215, 80, 255 } : (u5 ? Color{ 120, 220, 255, 255 } : Color{ 150, 165, 185, 180 });
        }

        FontManager::DrawText(perkTitle, (int)perkRec.x + 12, (int)perkRec.y + 7, 11, perkBadgeCol, GameFont::TITLE);
        FontManager::DrawTextWrapped(perkDesc, (int)perkRec.x + 12, (int)perkRec.y + 22, (int)perkRec.width - 24, 10, 2, Color{ 220, 235, 250, 230 });

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
        if (totalFrags <= 0) {
            return false;
        }
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



        // Bookmark Page Selector (dynamic clickable tabs across dynamic rows)
        FontManager::DrawText("PAGE BOOKMARKS (CLICK TO TURN PAGE):", (int)lPad, (int)pageY + 58, 10, Color{ 110, 80, 45, 240 }, GameFont::TITLE);

        int cols = 5;
        if (totalFrags > 10 && totalFrags <= 21) {
            cols = 7;
        } else if (totalFrags > 21) {
            cols = 8;
        }
        int rows = (totalFrags > 0) ? ((totalFrags + cols - 1) / cols) : 1;
        float tabSpacing = 4.0f;
        float tabW = (lW - (float)(cols - 1) * tabSpacing) / (float)cols;
        float tabH = (rows <= 2) ? 24.0f : (rows == 3 ? 20.0f : 18.0f);
        float bookmarkStartY = pageY + 74.0f;

        for (int i = 0; i < totalFrags; i++) {
            int row = i / cols;
            int col = i % cols;
            float tx = lPad + (float)col * (tabW + tabSpacing);
            float ty = bookmarkStartY + (float)row * (tabH + tabSpacing);
            Rectangle tabRec = { tx, ty, tabW, tabH };
            bool isTabHov = CheckCollisionPointRec(mousePos, tabRec);
            bool isTabSel = (selectedFragmentIndex == i);

            Color tBg = isTabSel ? Color{ 215, 175, 90, 255 } : (isTabHov ? Color{ 232, 218, 186, 255 } : Color{ 238, 228, 205, 230 });
            Color tBorder = isTabSel ? Color{ 120, 75, 25, 255 } : (frags[i].discovered ? Color{ 160, 130, 85, 220 } : Color{ 190, 175, 150, 180 });

            DrawRectangleRounded(tabRec, 0.2f, 4, tBg);
            DrawRectangleRoundedLines(tabRec, 0.2f, 4, tBorder);

            std::string label = ToRomanNumeral(i + 1);
            int lbw = FontManager::MeasureText(label, 11, GameFont::TITLE);
            Color textCol = isTabSel ? Color{ 35, 20, 10, 255 } : (frags[i].discovered ? Color{ 70, 45, 25, 240 } : Color{ 145, 130, 110, 200 });
            FontManager::DrawText(label, (int)(tx + (tabW - lbw) * 0.5f), (int)ty + (int)((tabH - 12.0f) * 0.5f), 11, textCol, GameFont::TITLE);

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
        float bookmarksBottom = bookmarkStartY + (float)rows * tabH + (float)(rows > 0 ? (rows - 1) : 0) * tabSpacing;
        float sumY = bookmarksBottom + 8.0f;
        DrawLine((int)lPad, (int)sumY, (int)(lPad + lW), (int)sumY, Color{ 180, 150, 105, 180 });

        std::string chapHeader = "SELECTED: " + curFrag.title;
        FontManager::DrawText(chapHeader.c_str(), (int)lPad, (int)sumY + 6, 11, Color{ 45, 30, 18, 255 }, GameFont::TITLE);
        std::string realmStr = "Origin Realm: " + curFrag.realm;
        FontManager::DrawText(realmStr.c_str(), (int)lPad, (int)sumY + 22, 10, Color{ 85, 60, 35, 220 }, GameFont::BODY);

        // DEDICATED LOCATION CLUE & COMPASS PARCHMENT PLAQUE
        float clueBoxY = sumY + 38.0f;
        float navBtnY = pageY + pageH - 42.0f;
        float clueBoxH = std::max(120.0f, (navBtnY - 8.0f) - clueBoxY);
        Rectangle clueRec = { lPad, clueBoxY, lW, clueBoxH };

        DrawRectangleRounded(clueRec, 0.08f, 6, Color{ 238, 228, 204, 255 });
        DrawRectangleRoundedLines(clueRec, 0.08f, 6, Color{ 165, 130, 80, 220 });
        DrawRectangleRoundedLines(Rectangle{ lPad + 3.0f, clueBoxY + 3.0f, lW - 6.0f, clueBoxH - 6.0f }, 0.07f, 6, Color{ 215, 195, 160, 180 });

        // Compass Rose Header
        FontManager::DrawText("LOCATION CLUE & RECOVERY COMPASS", (int)lPad + 14, (int)clueBoxY + 10, 11, Color{ 160, 38, 28, 255 }, GameFont::TITLE);
        DrawLine((int)lPad + 14, (int)clueBoxY + 24, (int)(lPad + lW - 14), (int)clueBoxY + 24, Color{ 180, 140, 95, 180 });

        // Recovery Clue Body
        std::string clueBody = curFrag.clue.empty() ? "Seek forgotten altars and guarded chests throughout Tydeshire." : curFrag.clue;
        FontManager::DrawTextWrapped(clueBody, (int)lPad + 14, (int)clueBoxY + 32, (int)lW - 28, 12, 5, Color{ 48, 34, 22, 255 }, GameFont::BODY);

        // Discovery Status Badge inside plaque
        float badgeH = 24.0f;
        float badgeY = clueBoxY + clueBoxH - badgeH - 10.0f;
        if (curFrag.discovered) {
            DrawRectangleRounded(Rectangle{ lPad + 14, badgeY, lW - 28, badgeH }, 0.25f, 4, Color{ 215, 240, 215, 255 });
            DrawRectangleRoundedLines(Rectangle{ lPad + 14, badgeY, lW - 28, badgeH }, 0.25f, 4, Color{ 40, 140, 60, 220 });
            FontManager::DrawText("RECOVERED & CHRONICLED", (int)lPad + 24, (int)badgeY + 6, 10, Color{ 20, 110, 45, 255 }, GameFont::TITLE);
        } else {
            DrawRectangleRounded(Rectangle{ lPad + 14, badgeY, lW - 28, badgeH }, 0.25f, 4, Color{ 245, 220, 215, 255 });
            DrawRectangleRoundedLines(Rectangle{ lPad + 14, badgeY, lW - 28, badgeH }, 0.25f, 4, Color{ 180, 50, 40, 220 });
            FontManager::DrawText("SHROUDED - SEARCH THIS LOCATION", (int)lPad + 24, (int)badgeY + 6, 10, Color{ 160, 35, 25, 255 }, GameFont::TITLE);
        }



        // Bottom Left Page Navigation: Previous Page Button
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



    for (int k = 0; k < (int)std::size(realms); k++) {

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

    std::string fragScoreStr = "Discovered Fragments: " + std::to_string(discoveredCount) + " / " + std::to_string(player.GetInventory().GetFragments().size());

    DrawText(fragScoreStr.c_str(), (int)col2X + 20, (int)sec2Y + 58, 18, Color{ 140, 220, 255, 255 });



    DrawText("Fragment I: The 500-Year Chaos - Uncovered", (int)col2X + 20, (int)sec2Y + 86, 11, Color{ 200, 225, 245, 240 });

    DrawText("Fragment II: The Five Realms of Charlz - Uncovered", (int)col2X + 20, (int)sec2Y + 104, 11, Color{ 200, 225, 245, 240 });

    

    bool hasFrag3 = player.GetInventory().GetFragments().size() > 2 && player.GetInventory().GetFragments()[2].discovered;

    DrawText(hasFrag3 ? "Fragment III: Zil's Holy Oath - Uncovered" : "Fragment III: Zil's Holy Oath - Shrouded [Defeat Zil's Fragment]", (int)col2X + 20, (int)sec2Y + 122, 11, hasFrag3 ? Color{ 255, 215, 80, 240 } : Color{ 130, 145, 170, 200 });



    DrawText("Tip: Press [TAB] or [I] anywhere in the world to open your Inventory & Lore Journal.", (int)col2X + 20, (int)sec2Y + 154, 11, Color{ 255, 180, 70, 230 });



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



bool ModalUI::DrawMapModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player, GameState currentState, int* outFastTravelStage) {
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
    bool shouldClose = (isCloseHov && isMousePressed);

    // 3. TACTICAL DIAMOND WAYPOINTS (<1> - <6>) & LEYLINE MAP
    struct MapStage { int id; const char* name; Vector2 pos; const char* threat; const char* minLvl; const char* lore; };
    MapStage stages[6] = {
        { 1, "STAGE #1: RUINED CASTLE", { modalX + 310.0f, modalY + 510.0f }, "Normal", "Min Level: 1", "At the beginning, run the foundations for playing via the newest patches. Dangerous foes are available and relevant leylines will lead all." },
        { 2, "STAGE #2: CROSSROADS", { modalX + 590.0f, modalY + 420.0f }, "Moderate", "Min Level: 5", "A sprawling highway crossing southern pines. Bandits ambush supply lines while ancient stone shrines whisper of Solsden." },
        { 3, "STAGE #3: ZYPHDEN KEEP", { modalX + 720.0f, modalY + 250.0f }, "High", "Min Level: 15", "The fortified royal citadel. King Kreptu watches from the throne dais as Chancellor Malakor and forge masters guard the flame." },
        { 4, "STAGE #4: QUARRY MINES", { modalX + 860.0f, modalY + 380.0f }, "Severe", "Min Level: 20", "Subterranean basalt shafts overrun by rogue ore golems. Mining picks strike primordial veins of ancient crystal." },
        { 5, "STAGE #5: RESTLESS MIST FOREST", { modalX + 440.0f, modalY + 260.0f }, "Lethal", "Min Level: 25", "Haunted western woodlands shrouded in primordial fog. Gale bandits roam the glades while the Whispering Altar calls." },
        { 6, "STAGE #6: NORTHERN CRAGS", { modalX + 720.0f, modalY + 120.0f }, "Cataclysmic", "Min Level: 30", "Frostbound highland crags north of Zyphden. Savage Frost Trolls prowl the jagged peaks under the dominion of the Berserker Troll." }
    };

    static int s_selectedStage = 1;

    // Determine which stage is active based on currentState
    int activeStageId = 1;
    if (currentState == GameState::RUINED_CASTLE) activeStageId = 1;
    else if (currentState == GameState::CROSSROADS) activeStageId = 2;
    else if ((currentState == GameState::ZYPHDEN_REALM || currentState == GameState::ZYPHDEN_TRANSITION)) activeStageId = 3;
    else if (currentState == GameState::QUARRY_DEPTHS) activeStageId = 4;
    else if (currentState == GameState::RESTLESS_MIST_FOREST) activeStageId = 5;
    else if (currentState == GameState::NORTH_ZYPHDEN) activeStageId = 6;

    // Draw connecting leylines between diamond waypoints (Image 2)
    Color leylineCol = Color{ 40, 140, 160, 180 };
    DrawLineEx(stages[0].pos, stages[1].pos, 2.5f, leylineCol);
    DrawLineEx(stages[1].pos, stages[2].pos, 2.5f, leylineCol);
    DrawLineEx(stages[2].pos, stages[3].pos, 2.5f, leylineCol);
    DrawLineEx(stages[2].pos, stages[4].pos, 2.5f, Color{ 40, 140, 160, 180 });
    DrawLineEx(stages[2].pos, stages[5].pos, 2.5f, Color{ 40, 140, 160, 180 });

    // Draw Diamond Waypoints (<1> - <5>)
    float animTime = (float)GetTime();
    for (int i = 0; i < (int)std::size(stages); i++) {
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
    std::string frText = "Fragments: " + std::to_string(player.GetInventory().GetDiscoveredFragmentCount()) + " / " + std::to_string(GameLimits::LORE_FRAGMENT_COUNT) + " Uncovered";
    DrawText(frText.c_str(), (int)cardX + 22, (int)cardY + 90, 11, Color{ 160, 200, 225, 200 });

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
        s_selectedStage = (s_selectedStage <= 1) ? 6 : (s_selectedStage - 1);
        SoundFX::PlayButtonClick();
    }
    if (CheckCollisionPointRec(mousePos, rightArrow) && isMousePressed) {
        s_selectedStage = (s_selectedStage >= 6) ? 1 : (s_selectedStage + 1);
        SoundFX::PlayButtonClick();
    }
    DrawText("<", (int)leftArrow.x + 8, (int)leftArrow.y + 8, 20, Color{ 80, 220, 245, 220 });
    DrawText(">", (int)rightArrow.x + 8, (int)rightArrow.y + 8, 20, Color{ 80, 220, 245, 220 });

    // 5. CALL TO ACTION BUTTON (Image 2: Hexagonal angled banner [ PLAY NOW ] on bottom right)
    float playW = 200.0f;
    float playH = 44.0f;
    Rectangle playRec = { modalX + modalW - playW - 48.0f, modalY + modalH - playH - 36.0f, playW, playH };
    bool isPlayHov = CheckCollisionPointRec(mousePos, playRec);
    bool canTravel = (s_selectedStage != activeStageId) && player.GetInventory().IsCampfireUnlocked(s_selectedStage);
    if ((isPlayHov && isMousePressed) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        SoundFX::PlayButtonClick();
        if (outFastTravelStage && canTravel) {
            *outFastTravelStage = s_selectedStage;
        }
        return true; // Closes map modal and resumes journey / fast travels!
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

    const char* btnLabel = canTravel ? "FAST TRAVEL" : "RESUME JOURNEY";
    int ptw = MeasureText(btnLabel, 14);
    DrawText(btnLabel, (int)(playRec.x + (playW - ptw) * 0.5f), (int)(playRec.y + 15.0f), 14, Color{ 255, 255, 255, 255 });

    if (shouldClose) {
        SoundFX::PlayButtonClick();
        return true;
    }

    return false;
}

bool ModalUI::DrawStorageStash(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player) {
    // Semi-transparent backdrop
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 0, 0, 0, 195 });

    float modalW = 900.0f;
    float modalH = 580.0f;
    float modalX = ((float)windowWidth - modalW) * 0.5f;
    float modalY = ((float)windowHeight - modalH) * 0.5f;

    // Window frame
    Rectangle modalRec = { modalX, modalY, modalW, modalH };
    DrawRectangleRounded(modalRec, 0.04f, 6, Color{ 16, 20, 28, 252 });
    DrawRectangleRoundedLines(modalRec, 0.04f, 6, Color{ 205, 160, 70, 240 });

    // Header banner
    Rectangle headerRec = { modalX, modalY, modalW, 56.0f };
    DrawRectangleRounded(headerRec, 0.04f, 6, Color{ 24, 30, 44, 255 });
    DrawLineEx({ modalX, modalY + 56.0f }, { modalX + modalW, modalY + 56.0f }, 2.0f, Color{ 180, 140, 60, 220 });

    FontManager::DrawTextTitleCentered("ANCIENT VANGUARD STORAGE STASH", windowWidth / 2, (int)modalY + 12, 18, Color{ 255, 220, 110, 255 });
    FontManager::DrawTextTitleCentered("Persistent Vault Trunk Synchronized Across All Sanctuaries and Bonfires", windowWidth / 2, (int)modalY + 36, 11, Color{ 170, 190, 215, 220 });

    // Close Button [X]
    Rectangle closeBtn = { modalX + modalW - 44.0f, modalY + 12.0f, 32.0f, 32.0f };
    bool isCloseHov = CheckCollisionPointRec(mousePos, closeBtn);
    DrawRectangleRounded(closeBtn, 0.2f, 4, isCloseHov ? Color{ 180, 40, 50, 255 } : Color{ 40, 24, 28, 220 });
    DrawRectangleRoundedLines(closeBtn, 0.2f, 4, Color{ 240, 90, 100, 230 });
    DrawText("X", (int)closeBtn.x + 11, (int)closeBtn.y + 8, 16, Color{ 255, 255, 255, 255 });

    bool shouldClose = (isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE);

    Inventory& inv = player.GetInventory();
    PlayerStash& stash = inv.GetStash();

    struct StashEntry {
        int id;
        std::string name;
        int carried;
        int stored;
        Color color;
    };

    std::vector<StashEntry> entries = {
        { 0, "Royal Shingles", inv.GetShingles(), stash.shingles, Color{ 255, 215, 80, 255 } },
        { 1, "Ore Scraps", inv.GetOreScraps(), stash.oreScraps, Color{ 180, 190, 205, 255 } },
        { 2, "Frost Shards", inv.GetFrostShards(), stash.frostShards, Color{ 130, 220, 255, 255 } },
        { 3, "Essence of Mist", inv.GetEssenceOfMist(), stash.essenceOfMist, Color{ 120, 240, 200, 255 } },
        { 4, "Shadow Essence", inv.GetShadowEssence(), stash.shadowEssence, Color{ 190, 120, 255, 255 } },
        { 5, "Health Potions", inv.GetHealthPotionCount(), stash.healthPotions, Color{ 255, 80, 95, 255 } },
        { 6, "Stoneskin Tonics", inv.GetStoneskinTonicCount(), stash.stoneskinTonics, Color{ 230, 160, 60, 255 } },
        { 7, "Glacial Draughts", inv.GetGlacialDraughtCount(), stash.glacialDraughts, Color{ 100, 200, 255, 255 } },
        { 8, "Berserker Draughts", inv.GetBerserkerDraughtCount(), stash.berserkerDraughts, Color{ 255, 60, 75, 255 } }
    };

    float paneW = (modalW - 60.0f) * 0.5f;
    float paneH = 430.0f;
    float leftX = modalX + 22.0f;
    float rightX = modalX + modalW - paneW - 22.0f;
    float paneY = modalY + 70.0f;

    // Left Pane (Carried)
    DrawRectangleRounded(Rectangle{ leftX, paneY, paneW, paneH }, 0.03f, 4, Color{ 12, 16, 24, 230 });
    DrawRectangleRoundedLines(Rectangle{ leftX, paneY, paneW, paneH }, 0.03f, 4, Color{ 60, 80, 110, 200 });
    FontManager::DrawGameTextShadow("CARRIED POUCH (INVENTORY)", (int)leftX + 16, (int)paneY + 12, 12, Color{ 220, 230, 245, 255 });

    // Right Pane (Vault Stash)
    DrawRectangleRounded(Rectangle{ rightX, paneY, paneW, paneH }, 0.03f, 4, Color{ 12, 16, 24, 230 });
    DrawRectangleRoundedLines(Rectangle{ rightX, paneY, paneW, paneH }, 0.03f, 4, Color{ 190, 150, 60, 200 });
    FontManager::DrawGameTextShadow("VAULT STASH TRUNK (STORED)", (int)rightX + 16, (int)paneY + 12, 12, Color{ 255, 220, 110, 255 });

    float rowH = 40.0f;
    float rowStartY = paneY + 36.0f;

    for (size_t i = 0; i < entries.size(); ++i) {
        float ry = rowStartY + (float)i * rowH;
        const auto& e = entries[i];

        // Left Pane Row (Deposit)
        Rectangle leftRow = { leftX + 8.0f, ry, paneW - 16.0f, rowH - 4.0f };
        DrawRectangleRounded(leftRow, 0.15f, 4, (i % 2 == 0) ? Color{ 20, 26, 38, 220 } : Color{ 16, 22, 32, 220 });

        DrawCircle((int)leftRow.x + 12, (int)(leftRow.y + leftRow.height * 0.5f), 4.5f, e.color);
        DrawText(e.name.c_str(), (int)leftRow.x + 24, (int)leftRow.y + 11, 11, Color{ 215, 225, 240, 240 });

        char cCountBuf[32];
        snprintf(cCountBuf, sizeof(cCountBuf), "x%d", e.carried);
        DrawText(cCountBuf, (int)leftRow.x + 175, (int)leftRow.y + 11, 11, e.carried > 0 ? Color{ 255, 255, 255, 255 } : Color{ 120, 130, 145, 200 });

        // Deposit 1 (or 50 for shingles)
        Rectangle btnDep1 = { leftRow.x + leftRow.width - 120.0f, leftRow.y + 5.0f, 52.0f, leftRow.height - 10.0f };
        bool hovDep1 = CheckCollisionPointRec(mousePos, btnDep1) && e.carried > 0;
        DrawRectangleRounded(btnDep1, 0.2f, 4, hovDep1 ? Color{ 50, 100, 160, 255 } : Color{ 28, 44, 70, 220 });
        DrawRectangleRoundedLines(btnDep1, 0.2f, 4, e.carried > 0 ? Color{ 90, 160, 240, 220 } : Color{ 50, 65, 85, 180 });
        const char* d1Label = (e.id == 0) ? "-> 50" : "-> 1";
        DrawText(d1Label, (int)btnDep1.x + 8, (int)btnDep1.y + 6, 10, e.carried > 0 ? Color{ 255, 255, 255, 255 } : Color{ 110, 120, 135, 180 });

        if (hovDep1 && isMousePressed) {
            int amt = (e.id == 0) ? 50 : 1;
            inv.DepositItem(e.id, amt);
            SoundFX::PlayButtonClick();
        }

        // Deposit All
        Rectangle btnDepAll = { leftRow.x + leftRow.width - 62.0f, leftRow.y + 5.0f, 54.0f, leftRow.height - 10.0f };
        bool hovDepAll = CheckCollisionPointRec(mousePos, btnDepAll) && e.carried > 0;
        DrawRectangleRounded(btnDepAll, 0.2f, 4, hovDepAll ? Color{ 60, 120, 180, 255 } : Color{ 32, 52, 80, 220 });
        DrawRectangleRoundedLines(btnDepAll, 0.2f, 4, e.carried > 0 ? Color{ 110, 185, 255, 220 } : Color{ 50, 65, 85, 180 });
        DrawText("-> ALL", (int)btnDepAll.x + 6, (int)btnDepAll.y + 6, 10, e.carried > 0 ? Color{ 255, 255, 255, 255 } : Color{ 110, 120, 135, 180 });

        if (hovDepAll && isMousePressed) {
            inv.DepositItem(e.id, e.carried);
            SoundFX::PlayButtonClick();
        }

        // Right Pane Row (Withdraw)
        Rectangle rightRow = { rightX + 8.0f, ry, paneW - 16.0f, rowH - 4.0f };
        DrawRectangleRounded(rightRow, 0.15f, 4, (i % 2 == 0) ? Color{ 22, 28, 38, 220 } : Color{ 18, 22, 32, 220 });

        DrawCircle((int)rightRow.x + 12, (int)(rightRow.y + rightRow.height * 0.5f), 4.5f, e.color);
        DrawText(e.name.c_str(), (int)rightRow.x + 24, (int)rightRow.y + 11, 11, Color{ 235, 215, 160, 240 });

        char sCountBuf[32];
        snprintf(sCountBuf, sizeof(sCountBuf), "x%d", e.stored);
        DrawText(sCountBuf, (int)rightRow.x + 175, (int)rightRow.y + 11, 11, e.stored > 0 ? Color{ 255, 225, 120, 255 } : Color{ 120, 130, 145, 200 });

        // Withdraw 1 (or 50 for shingles)
        Rectangle btnWd1 = { rightRow.x + rightRow.width - 120.0f, rightRow.y + 5.0f, 52.0f, rightRow.height - 10.0f };
        bool hovWd1 = CheckCollisionPointRec(mousePos, btnWd1) && e.stored > 0;
        DrawRectangleRounded(btnWd1, 0.2f, 4, hovWd1 ? Color{ 140, 105, 45, 255 } : Color{ 65, 48, 24, 220 });
        DrawRectangleRoundedLines(btnWd1, 0.2f, 4, e.stored > 0 ? Color{ 240, 185, 75, 220 } : Color{ 75, 60, 40, 180 });
        const char* w1Label = (e.id == 0) ? "50 <-" : "1 <-";
        DrawText(w1Label, (int)btnWd1.x + 8, (int)btnWd1.y + 6, 10, e.stored > 0 ? Color{ 255, 255, 255, 255 } : Color{ 135, 120, 100, 180 });

        if (hovWd1 && isMousePressed) {
            int amt = (e.id == 0) ? 50 : 1;
            inv.WithdrawItem(e.id, amt);
            SoundFX::PlayButtonClick();
        }

        // Withdraw All
        Rectangle btnWdAll = { rightRow.x + rightRow.width - 62.0f, rightRow.y + 5.0f, 54.0f, rightRow.height - 10.0f };
        bool hovWdAll = CheckCollisionPointRec(mousePos, btnWdAll) && e.stored > 0;
        DrawRectangleRounded(btnWdAll, 0.2f, 4, hovWdAll ? Color{ 160, 120, 50, 255 } : Color{ 75, 55, 28, 220 });
        DrawRectangleRoundedLines(btnWdAll, 0.2f, 4, e.stored > 0 ? Color{ 255, 205, 90, 220 } : Color{ 75, 60, 40, 180 });
        DrawText("ALL <-", (int)btnWdAll.x + 6, (int)btnWdAll.y + 6, 10, e.stored > 0 ? Color{ 255, 255, 255, 255 } : Color{ 135, 120, 100, 180 });

        if (hovWdAll && isMousePressed) {
            inv.WithdrawItem(e.id, e.stored);
            SoundFX::PlayButtonClick();
        }
    }

    // Bottom Action Bar
    float bottomY = modalY + modalH - 56.0f;

    // Deposit All Crafting Materials Button
    Rectangle btnDepMat = { leftX, bottomY, 240.0f, 36.0f };
    bool hovDepMat = CheckCollisionPointRec(mousePos, btnDepMat);
    DrawRectangleRounded(btnDepMat, 0.2f, 4, hovDepMat ? Color{ 40, 80, 120, 255 } : Color{ 24, 45, 75, 220 });
    DrawRectangleRoundedLines(btnDepMat, 0.2f, 4, Color{ 80, 150, 225, 220 });
    DrawText("STORE ALL CRAFTING MATERIALS", (int)btnDepMat.x + 12, (int)btnDepMat.y + 11, 10, Color{ 210, 235, 255, 255 });

    if (hovDepMat && isMousePressed) {
        inv.DepositItem(1, inv.GetOreScraps());
        inv.DepositItem(2, inv.GetFrostShards());
        inv.DepositItem(3, inv.GetEssenceOfMist());
        inv.DepositItem(4, inv.GetShadowEssence());
        SoundFX::PlayButtonClick();
    }

    // Withdraw All Elixirs Button
    Rectangle btnWdElix = { leftX + 252.0f, bottomY, 230.0f, 36.0f };
    bool hovWdElix = CheckCollisionPointRec(mousePos, btnWdElix);
    DrawRectangleRounded(btnWdElix, 0.2f, 4, hovWdElix ? Color{ 120, 85, 35, 255 } : Color{ 65, 45, 22, 220 });
    DrawRectangleRoundedLines(btnWdElix, 0.2f, 4, Color{ 220, 165, 65, 220 });
    DrawText("RETRIEVE ALL ELIXIRS & POTIONS", (int)btnWdElix.x + 10, (int)btnWdElix.y + 11, 10, Color{ 255, 230, 160, 255 });

    if (hovWdElix && isMousePressed) {
        inv.WithdrawItem(5, stash.healthPotions);
        inv.WithdrawItem(6, stash.stoneskinTonics);
        inv.WithdrawItem(7, stash.glacialDraughts);
        inv.WithdrawItem(8, stash.berserkerDraughts);
        SoundFX::PlayButtonClick();
    }

    // Close Vault Button
    Rectangle btnClose = { modalX + modalW - 170.0f, bottomY, 148.0f, 36.0f };
    bool hovClose = CheckCollisionPointRec(mousePos, btnClose);
    DrawRectangleRounded(btnClose, 0.2f, 4, hovClose ? Color{ 70, 35, 40, 255 } : Color{ 45, 25, 30, 220 });
    DrawRectangleRoundedLines(btnClose, 0.2f, 4, Color{ 200, 90, 100, 220 });
    DrawText("CLOSE VAULT [ESC]", (int)btnClose.x + 14, (int)btnClose.y + 11, 11, Color{ 255, 220, 225, 255 });

    if (hovClose && isMousePressed) {
        SoundFX::PlayButtonClick();
        shouldClose = true;
    }

    return shouldClose;
}

