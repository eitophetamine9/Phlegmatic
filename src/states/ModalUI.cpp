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
    const float modalH = 540.0f;
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
// TACTICAL UI WIREFRAME VECTOR ICON HELPERS (Image 1 & Image 2)
// ==========================================

static void DrawWireframeWeapon(Vector2 center, WeaponType type, Color col) {
    switch (type) {
        case WeaponType::HALBERD: {
            // Extended polearm shaft
            DrawLineEx({ center.x - 45.0f, center.y + 20.0f }, { center.x + 45.0f, center.y - 20.0f }, 2.5f, col);
            // Spear tip
            Vector2 tip = { center.x + 45.0f, center.y - 20.0f };
            DrawTriangle({ tip.x + 12.0f, tip.y - 5.0f }, { tip.x - 3.0f, tip.y - 8.0f }, { tip.x - 1.0f, tip.y + 4.0f }, col);
            // Crescent axe blade
            DrawCircleSectorLines({ tip.x - 10.0f, tip.y - 2.0f }, 16.0f, 210.0f, 330.0f, 12, col);
            DrawLineEx({ tip.x - 10.0f, tip.y - 2.0f }, { tip.x - 22.0f, tip.y - 12.0f }, 2.0f, col);
            // Rear beak hook
            DrawLineEx({ tip.x - 14.0f, tip.y + 2.0f }, { tip.x - 24.0f, tip.y + 10.0f }, 2.0f, col);
            break;
        }
        case WeaponType::MACE: {
            // Shaft
            DrawLineEx({ center.x - 35.0f, center.y + 20.0f }, { center.x + 25.0f, center.y - 15.0f }, 3.0f, col);
            // Flanged head
            Vector2 head = { center.x + 25.0f, center.y - 15.0f };
            DrawCircleLines((int)head.x, (int)head.y, 14.0f, col);
            DrawLineEx({ head.x - 10.0f, head.y }, { head.x + 10.0f, head.y }, 2.5f, col);
            DrawLineEx({ head.x, head.y - 10.0f }, { head.x, head.y + 10.0f }, 2.5f, col);
            DrawCircle((int)head.x, (int)head.y, 4.0f, col);
            break;
        }
        case WeaponType::TRUTH_SEEKER: {
            // Double crescent sacred blade
            DrawLineEx({ center.x - 35.0f, center.y + 18.0f }, { center.x + 35.0f, center.y - 18.0f }, 2.5f, col);
            Vector2 head = { center.x + 22.0f, center.y - 11.0f };
            DrawCircleSectorLines(head, 18.0f, 180.0f, 360.0f, 16, col);
            DrawCircleLines((int)head.x, (int)head.y, 7.0f, Color{ 255, 235, 120, 255 });
            break;
        }
        case WeaponType::SWORD:
        default: {
            // Martial Broadsword
            DrawLineEx({ center.x - 40.0f, center.y + 18.0f }, { center.x + 35.0f, center.y - 16.0f }, 3.0f, col);
            Vector2 guard = { center.x - 15.0f, center.y + 6.0f };
            DrawLineEx({ guard.x - 7.0f, guard.y - 14.0f }, { guard.x + 7.0f, guard.y + 14.0f }, 3.0f, col);
            Vector2 tip = { center.x + 35.0f, center.y - 16.0f };
            DrawLineEx(tip, { tip.x + 8.0f, tip.y - 4.0f }, 2.0f, col);
            break;
        }
    }
}

static void DrawWireframeSidearm(Vector2 center, Color col) {
    // Twin Daggers wireframe
    DrawLineEx({ center.x - 18.0f, center.y - 14.0f }, { center.x + 14.0f, center.y + 14.0f }, 2.0f, col);
    DrawLineEx({ center.x + 18.0f, center.y - 14.0f }, { center.x - 14.0f, center.y + 14.0f }, 2.0f, col);
    DrawCircleLines((int)center.x, (int)center.y, 5.0f, col);
}

static void DrawWireframeMelee(Vector2 center, Color col) {
    // Curved combat blade
    DrawLineEx({ center.x - 20.0f, center.y + 12.0f }, { center.x + 5.0f, center.y - 2.0f }, 2.0f, col);
    DrawLineEx({ center.x + 5.0f, center.y - 2.0f }, { center.x + 22.0f, center.y - 16.0f }, 2.5f, col);
    DrawLineEx({ center.x - 20.0f, center.y + 12.0f }, { center.x - 26.0f, center.y + 16.0f }, 3.5f, col);
}

static void DrawWireframeSpecial(Vector2 center, Color col) {
    // Relic diamond prism
    DrawPolyLines(center, 4, 18.0f, 45.0f, col);
    DrawCircleLines((int)center.x, (int)center.y, 6.0f, Color{ 255, 235, 120, 255 });
    DrawLineEx({ center.x - 22.0f, center.y }, { center.x + 22.0f, center.y }, 1.5f, col);
    DrawLineEx({ center.x, center.y - 22.0f }, { center.x, center.y + 22.0f }, 1.5f, col);
}

static void DrawWireframeGrenade(Vector2 center, Color col) {
    // Tactical grenade flask canister
    Rectangle body = { center.x - 9.0f, center.y - 10.0f, 18.0f, 22.0f };
    DrawRectangleRoundedLines(body, 0.35f, 4, col);
    DrawLineEx({ center.x - 9.0f, center.y - 3.0f }, { center.x + 9.0f, center.y - 3.0f }, 1.5f, col);
    DrawLineEx({ center.x - 9.0f, center.y + 4.0f }, { center.x + 9.0f, center.y + 4.0f }, 1.5f, col);
    DrawLineEx({ center.x, center.y - 10.0f }, { center.x, center.y - 15.0f }, 2.0f, col);
    DrawCircleLines((int)center.x - 4, (int)center.y - 15, 3.0f, col);
}

static void DrawWireframeHelmet(Vector2 center, Color col) {
    // Tactical diamond crest helmet (Image 1)
    DrawPolyLines(center, 4, 20.0f, 45.0f, col);
    DrawPolyLines(center, 4, 12.0f, 45.0f, Color{ col.r, col.g, col.b, 160 });
    DrawLineEx({ center.x - 24.0f, center.y }, { center.x + 24.0f, center.y }, 1.5f, col);
    DrawLineEx({ center.x, center.y - 24.0f }, { center.x, center.y + 24.0f }, 1.5f, col);
}

static void DrawWireframeCuirass(Vector2 center, Color col) {
    // Segmented armored tactical vest (Image 1)
    // High collar & shoulder cutouts
    DrawLineEx({ center.x - 18.0f, center.y - 24.0f }, { center.x + 18.0f, center.y - 24.0f }, 2.0f, col);
    DrawLineEx({ center.x - 18.0f, center.y - 24.0f }, { center.x - 26.0f, center.y - 8.0f }, 2.0f, col);
    DrawLineEx({ center.x + 18.0f, center.y - 24.0f }, { center.x + 26.0f, center.y - 8.0f }, 2.0f, col);
    // Armored torso ribs
    DrawLineEx({ center.x - 26.0f, center.y - 8.0f }, { center.x - 18.0f, center.y + 22.0f }, 2.0f, col);
    DrawLineEx({ center.x + 26.0f, center.y - 8.0f }, { center.x + 18.0f, center.y + 22.0f }, 2.0f, col);
    DrawLineEx({ center.x - 18.0f, center.y + 22.0f }, { center.x + 18.0f, center.y + 22.0f }, 2.0f, col);
    // Ballistic chest chevron lines
    DrawLineEx({ center.x - 16.0f, center.y - 10.0f }, { center.x, center.y - 2.0f }, 1.8f, col);
    DrawLineEx({ center.x + 16.0f, center.y - 10.0f }, { center.x, center.y - 2.0f }, 1.8f, col);
    DrawLineEx({ center.x - 14.0f, center.y + 4.0f }, { center.x, center.y + 12.0f }, 1.8f, col);
    DrawLineEx({ center.x + 14.0f, center.y + 4.0f }, { center.x, center.y + 12.0f }, 1.8f, col);
}

static void DrawWireframeBoots(Vector2 center, Color col) {
    // Tactical laced high boots (Image 1)
    // Left boot
    DrawLineEx({ center.x - 18.0f, center.y - 22.0f }, { center.x - 8.0f, center.y - 22.0f }, 1.8f, col);
    DrawLineEx({ center.x - 18.0f, center.y - 22.0f }, { center.x - 16.0f, center.y + 14.0f }, 1.8f, col);
    DrawLineEx({ center.x - 8.0f, center.y - 22.0f }, { center.x - 6.0f, center.y + 8.0f }, 1.8f, col);
    DrawLineEx({ center.x - 16.0f, center.y + 14.0f }, { center.x - 22.0f, center.y + 22.0f }, 2.0f, col);
    DrawLineEx({ center.x - 22.0f, center.y + 22.0f }, { center.x - 6.0f, center.y + 22.0f }, 2.0f, col);
    DrawLineEx({ center.x - 6.0f, center.y + 8.0f }, { center.x - 6.0f, center.y + 22.0f }, 1.8f, col);

    // Right boot
    DrawLineEx({ center.x + 8.0f, center.y - 22.0f }, { center.x + 18.0f, center.y - 22.0f }, 1.8f, col);
    DrawLineEx({ center.x + 8.0f, center.y - 22.0f }, { center.x + 6.0f, center.y + 8.0f }, 1.8f, col);
    DrawLineEx({ center.x + 18.0f, center.y - 22.0f }, { center.x + 16.0f, center.y + 14.0f }, 1.8f, col);
    DrawLineEx({ center.x + 6.0f, center.y + 8.0f }, { center.x + 6.0f, center.y + 22.0f }, 1.8f, col);
    DrawLineEx({ center.x + 6.0f, center.y + 22.0f }, { center.x + 22.0f, center.y + 22.0f }, 2.0f, col);
    DrawLineEx({ center.x + 16.0f, center.y + 14.0f }, { center.x + 22.0f, center.y + 22.0f }, 2.0f, col);
}

static void DrawWireframeLegs(Vector2 center, Color col) {
    // Tactical combat pants / trousers (Image 1)
    // Waistband & belt
    DrawLineEx({ center.x - 20.0f, center.y - 24.0f }, { center.x + 20.0f, center.y - 24.0f }, 2.2f, col);
    DrawLineEx({ center.x - 20.0f, center.y - 18.0f }, { center.x + 20.0f, center.y - 18.0f }, 1.5f, col);
    // Outer legs
    DrawLineEx({ center.x - 20.0f, center.y - 18.0f }, { center.x - 18.0f, center.y + 24.0f }, 2.0f, col);
    DrawLineEx({ center.x + 20.0f, center.y - 18.0f }, { center.x + 18.0f, center.y + 24.0f }, 2.0f, col);
    // Crotch & inner legs
    DrawLineEx({ center.x, center.y - 8.0f }, { center.x - 6.0f, center.y + 24.0f }, 1.8f, col);
    DrawLineEx({ center.x, center.y - 8.0f }, { center.x + 6.0f, center.y + 24.0f }, 1.8f, col);
    // Knee seams & cargo accents
    DrawLineEx({ center.x - 18.0f, center.y + 4.0f }, { center.x - 7.0f, center.y + 4.0f }, 1.5f, col);
    DrawLineEx({ center.x + 7.0f, center.y + 4.0f }, { center.x + 18.0f, center.y + 4.0f }, 1.5f, col);
}

static void DrawWireframePotion(Vector2 center, Color col) {
    // Alchemy potion flask bottle (Image 1)
    // Cork & neck
    DrawRectangleLines((int)center.x - 5, (int)center.y - 22, 10, 6, col);
    DrawLineEx({ center.x - 4.0f, center.y - 16.0f }, { center.x + 4.0f, center.y - 16.0f }, 2.0f, col);
    // Spherical body
    DrawCircleLines((int)center.x, (int)center.y + 2, 18.0f, col);
    // Bubbling liquid inside
    DrawCircleSector({ center.x, center.y + 2 }, 15.0f, 20.0f, 160.0f, 12, Color{ col.r, col.g, col.b, 90 });
    DrawCircle((int)center.x - 4, (int)center.y + 5, 2.5f, col);
    DrawCircle((int)center.x + 4, (int)center.y - 1, 2.0f, col);
}


bool ModalUI::DrawInventory(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex) {
    // Dimmed cinematic tactical backdrop
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 5, 8, 12, 225 });

    float modalW = 1120.0f;
    float modalH = 650.0f;
    float modalX = ((float)windowWidth - modalW) * 0.5f;
    float modalY = ((float)windowHeight - modalH) * 0.5f;

    // Outer backing card & tactical borders
    DrawRectangleRounded(Rectangle{ modalX, modalY, modalW, modalH }, 0.03f, 8, Color{ 11, 15, 22, 252 });
    DrawRectangleRoundedLines(Rectangle{ modalX, modalY, modalW, modalH }, 0.03f, 8, Color{ 40, 52, 72, 220 });
    DrawRectangleRoundedLines(Rectangle{ modalX + 4, modalY + 4, modalW - 8, modalH - 8 }, 0.025f, 8, Color{ 24, 32, 46, 180 });

    // Subtle corner brackets (Image 1 style)
    Color cornerCol = Color{ 140, 165, 195, 180 };
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 32, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 12, modalY + 32 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 32, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 12, modalY + 32 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 32, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 12, modalY + modalH - 32 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 32, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 12, modalY + modalH - 32 }, 2.0f, cornerCol);

    // ==========================================
    // TOP NAVIGATION BAR (Image 1: QUESTS | GEAR | INVENTORY | ABILITIES)
    // ==========================================
    float topNavY = modalY + 16.0f;
    const char* tabs[4] = { "QUESTS", "GEAR", "INVENTORY", "ABILITIES" };
    float tabWidths[4] = { 100.0f, 90.0f, 120.0f, 110.0f };
    float tabStartX = modalX + (modalW - 460.0f) * 0.5f;

    for (int t = 0; t < 4; t++) {
        Rectangle tRec = { tabStartX, topNavY, tabWidths[t], 28.0f };
        bool isHov = CheckCollisionPointRec(mousePos, tRec);
        if ((isHov && isMousePressed) || (IsKeyPressed(KEY_ONE + t))) {
            activeTab = t;
            SoundFX::PlayButtonClick();
        }

        bool isActive = (activeTab == t);
        int textW = MeasureText(tabs[t], 13);
        int textX = (int)(tRec.x + (tRec.width - textW) * 0.5f);

        if (isActive) {
            DrawText(tabs[t], textX, (int)topNavY + 4, 13, Color{ 255, 255, 255, 255 });
            // Active glowing underline (Orange/Coral bar under GEAR like in Image 1)
            DrawRectangle((int)tRec.x + 8, (int)topNavY + 24, (int)tRec.width - 16, 3, Color{ 255, 95, 75, 255 });
            DrawRectangleGradientH((int)tRec.x + 4, (int)topNavY + 24, (int)tRec.width - 8, 3, Color{ 255, 130, 90, 255 }, Color{ 255, 70, 50, 255 });
        } else {
            DrawText(tabs[t], textX, (int)topNavY + 4, 13, isHov ? Color{ 220, 230, 245, 255 } : Color{ 130, 145, 170, 200 });
        }

        tabStartX += tabWidths[t] + 16.0f;
    }

    // Close Button [ESC / I] on Top Right
    float closeW = 90.0f;
    float closeH = 26.0f;
    Rectangle closeRec = { modalX + modalW - closeW - 24.0f, topNavY, closeW, closeH };
    bool isCloseHov = CheckCollisionPointRec(mousePos, closeRec);
    DrawRectangleRounded(closeRec, 0.25f, 4, isCloseHov ? Color{ 48, 60, 85, 255 } : Color{ 20, 26, 38, 220 });
    DrawRectangleRoundedLines(closeRec, 0.25f, 4, isCloseHov ? Color{ 255, 95, 75, 255 } : Color{ 50, 65, 90, 200 });
    DrawText("CLOSE [ESC]", (int)closeRec.x + 12, (int)closeRec.y + 7, 10, isCloseHov ? Color{ 255, 255, 255, 255 } : Color{ 170, 185, 210, 220 });
    bool closeClicked = (isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_I);

    // ==========================================
    // TOP ATTRIBUTES STRIP (Directly under tabs, Image 1)
    // ==========================================
    float stripY = modalY + 54.0f;
    float stripH = 34.0f;
    DrawRectangle((int)modalX + 16, (int)stripY, (int)modalW - 32, (int)stripH, Color{ 14, 18, 26, 240 });
    DrawLine((int)modalX + 16, (int)stripY, (int)modalX + (int)modalW - 16, (int)stripY, Color{ 36, 46, 64, 255 });
    DrawLine((int)modalX + 16, (int)stripY + (int)stripH, (int)modalX + (int)modalW - 16, (int)stripY + (int)stripH, Color{ 36, 46, 64, 255 });

    // Currency on Left: Shingles & Ore Scraps
    DrawRectangle((int)modalX + 44, (int)stripY + 10, 12, 14, Color{ 255, 210, 60, 255 });
    DrawRectangleLines((int)modalX + 44, (int)stripY + 10, 12, 14, Color{ 255, 240, 140, 255 });
    std::string shinglesStr = std::to_string(player.GetInventory().GetShingles()) + " Shingles";
    DrawText(shinglesStr.c_str(), (int)modalX + 64, (int)stripY + 10, 13, Color{ 255, 225, 100, 255 });

    std::string oreStr = std::to_string(player.GetInventory().GetOreScraps()) + " Basalt Ore";
    DrawText(oreStr.c_str(), (int)modalX + 180, (int)stripY + 10, 12, Color{ 160, 200, 230, 220 });

    // Center Attributes: Heart (HP) | Blade (ATK) | Shield (DEF) | Lightning (SPD)
    float attrCenterX = modalX + modalW * 0.5f - 140.0f;

    // Heart Icon (HP)
    DrawCircle((int)attrCenterX, (int)stripY + 16, 5.0f, Color{ 255, 75, 75, 255 });
    DrawCircle((int)attrCenterX + 7, (int)stripY + 16, 5.0f, Color{ 255, 75, 75, 255 });
    DrawTriangle({ attrCenterX - 4.0f, stripY + 18.0f }, { attrCenterX + 11.0f, stripY + 18.0f }, { attrCenterX + 3.5f, stripY + 26.0f }, Color{ 255, 75, 75, 255 });
    std::string hpStr = std::to_string((int)player.GetHealth()) + "/" + std::to_string((int)player.GetMaxHealth());
    DrawText(hpStr.c_str(), (int)attrCenterX + 18, (int)stripY + 11, 12, Color{ 240, 245, 255, 255 });

    // Blade Icon (ATK)
    float atkX = attrCenterX + 95.0f;
    DrawLineEx({ atkX, stripY + 23.0f }, { atkX + 10.0f, stripY + 10.0f }, 2.0f, Color{ 255, 160, 60, 255 });
    DrawLineEx({ atkX + 1.0f, stripY + 19.0f }, { atkX + 7.0f, stripY + 23.0f }, 2.0f, Color{ 255, 160, 60, 255 });
    std::string atkStr = std::to_string((int)player.GetEquippedWeaponStats().lightDamage);
    DrawText(atkStr.c_str(), (int)atkX + 16, (int)stripY + 11, 12, Color{ 240, 245, 255, 255 });

    // Shield Icon (DEF)
    float defX = atkX + 65.0f;
    DrawPoly({ defX + 5.0f, stripY + 17.0f }, 3, 7.0f, 180.0f, Color{ 90, 200, 255, 255 });
    std::string defStr = std::to_string((int)player.GetDefense());
    DrawText(defStr.c_str(), (int)defX + 16, (int)stripY + 11, 12, Color{ 240, 245, 255, 255 });

    // Lightning Icon (SPD)
    float spdX = defX + 65.0f;
    DrawLineEx({ spdX + 4.0f, stripY + 10.0f }, { spdX, stripY + 17.0f }, 2.0f, Color{ 255, 230, 80, 255 });
    DrawLineEx({ spdX, stripY + 17.0f }, { spdX + 5.0f, stripY + 17.0f }, 2.0f, Color{ 255, 230, 80, 255 });
    DrawLineEx({ spdX + 5.0f, stripY + 17.0f }, { spdX + 1.0f, stripY + 24.0f }, 2.0f, Color{ 255, 230, 80, 255 });
    std::string spdStr = std::to_string((int)player.GetSpeed());
    DrawText(spdStr.c_str(), (int)spdX + 14, (int)stripY + 11, 12, Color{ 240, 245, 255, 255 });

    // Capacity on Right: Bag / Weight
    float capX = modalX + modalW - 145.0f;
    DrawCircleLines((int)capX, (int)stripY + 14, 4.0f, Color{ 170, 190, 220, 240 });
    DrawRectangleLines((int)capX - 6, (int)stripY + 18, 12, 8, Color{ 170, 190, 220, 240 });
    std::string capStr = std::to_string(((int)player.GetInventory().GetUnlockedWeapons().size() + (int)player.GetInventory().GetQuestItems().size() + player.GetInventory().GetHealthPotionCount())) + "/175";
    DrawText(capStr.c_str(), (int)capX + 14, (int)stripY + 11, 12, Color{ 200, 220, 245, 240 });

    float contentY = stripY + stripH + 16.0f;
    float contentH = modalH - (contentY - modalY) - 16.0f;

    // ==========================================
    // TAB 1: GEAR SCREEN (3-Column Tactical Layout from Image 1)
    // ==========================================
    if (activeTab == 1) {
        WeaponStats curWStats = player.GetEquippedWeaponStats();
        ArmorStats curAStats = GetArmorStats(player.GetArmor());
        TotemStats curTStats = GetTotemStats(player.GetTotem());
        std::string hoveredTooltip = "";

        // ------------------------------------------
        // COLUMN 1 (LEFT): ARMAMENTS & AUXILIARIES
        // ------------------------------------------
        float col1X = modalX + 40.0f;
        float col1W = 240.0f;

        // 1. MAIN WEAPON CARD (Equipped, highlighted red/coral border from Image 1)
        float mainCardH = 110.0f;
        Rectangle mainRec = { col1X, contentY + 14.0f, col1W, mainCardH };
        bool isMainHov = CheckCollisionPointRec(mousePos, mainRec);

        DrawText("MAIN", (int)col1X, (int)contentY, 11, Color{ 255, 95, 75, 255 });
        DrawRectangleRounded(mainRec, 0.08f, 6, isMainHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 240 });
        DrawRectangleRoundedLines(mainRec, 0.08f, 6, Color{ 255, 95, 75, 255 }); // Red/coral accent border!

        // Wireframe drawing of equipped weapon
        Vector2 mainWepCenter = { col1X + col1W * 0.5f, mainRec.y + 44.0f };
        DrawWireframeWeapon(mainWepCenter, player.GetWeapon(), Color{ 255, 225, 90, 255 });

        // Weapon label & stats inside card
        DrawText(curWStats.name.c_str(), (int)col1X + 12, (int)mainRec.y + mainCardH - 26.0f, 12, Color{ 255, 255, 255, 255 });
        std::string wSub = "DMG " + std::to_string((int)curWStats.lightDamage) + " | RCH " + std::to_string((int)curWStats.lightRange);
        DrawText(wSub.c_str(), (int)col1X + 12, (int)mainRec.y + mainCardH - 14.0f, 9, Color{ 170, 190, 215, 200 });
        DrawText("[1-4]", (int)col1X + col1W - 32, (int)mainRec.y + mainCardH - 18.0f, 9, Color{ 255, 95, 75, 200 });

        if (isMainHov) {
            hoveredTooltip = curWStats.name + "\n" + curWStats.description + "\nLight DMG: " + std::to_string((int)curWStats.lightDamage) + " | Heavy DMG: " + std::to_string((int)curWStats.heavyDamage) + "\nPress [1] Sword | [2] Mace | [3] Halberd | [4] Truth Seeker to swap.";
        }

        // Quick weapon swap on number keys or click
        if (IsKeyPressed(KEY_ONE)) player.SetWeapon(WeaponType::SWORD);
        if (IsKeyPressed(KEY_TWO)) player.SetWeapon(WeaponType::MACE);
        if (IsKeyPressed(KEY_THREE)) player.SetWeapon(WeaponType::HALBERD);
        if (IsKeyPressed(KEY_FOUR)) player.SetWeapon(WeaponType::TRUTH_SEEKER);

        // 2. ROW 2: SIDEARM & MELEE SLOTS
        float row2Y = mainRec.y + mainCardH + 18.0f;
        float slotSubW = (col1W - 12.0f) * 0.5f;
        float slotSubH = 80.0f;

        // SIDEARM
        Rectangle sideRec = { col1X, row2Y + 12.0f, slotSubW, slotSubH };
        bool isSideHov = CheckCollisionPointRec(mousePos, sideRec);
        DrawText("SIDEARM", (int)col1X, (int)row2Y, 10, Color{ 140, 160, 190, 220 });
        DrawRectangleRounded(sideRec, 0.08f, 4, isSideHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(sideRec, 0.08f, 4, isSideHov ? Color{ 120, 210, 255, 240 } : Color{ 36, 48, 68, 200 });
        DrawWireframeSidearm({ sideRec.x + slotSubW * 0.5f, sideRec.y + slotSubH * 0.5f - 2.0f }, Color{ 255, 220, 90, 240 });
        DrawText("Dual Daggers", (int)sideRec.x + 8, (int)sideRec.y + slotSubH - 14.0f, 8, Color{ 170, 185, 210, 200 });
        if (isSideHov) hoveredTooltip = "SIDEARM: Dual Daggers\nRapid dual-blade secondary. Execute forward puncture bursts with [SPACE] evasion cadence.";

        // MELEE
        Rectangle melRec = { col1X + slotSubW + 12.0f, row2Y + 12.0f, slotSubW, slotSubH };
        bool isMelHov = CheckCollisionPointRec(mousePos, melRec);
        DrawText("MELEE", (int)melRec.x, (int)row2Y, 10, Color{ 140, 160, 190, 220 });
        DrawRectangleRounded(melRec, 0.08f, 4, isMelHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(melRec, 0.08f, 4, isMelHov ? Color{ 120, 210, 255, 240 } : Color{ 36, 48, 68, 200 });
        DrawWireframeMelee({ melRec.x + slotSubW * 0.5f, melRec.y + slotSubH * 0.5f - 2.0f }, Color{ 255, 220, 90, 240 });
        DrawText("Combat Stiletto", (int)melRec.x + 8, (int)melRec.y + slotSubH - 14.0f, 8, Color{ 170, 185, 210, 200 });
        if (isMelHov) hoveredTooltip = "MELEE: Solsden Trench Stiletto\nTactical parrying blade used to deflect heavy attacks with timed precision [F].";

        // 3. ROW 3: SPECIAL (RELIC / TOTEM)
        float row3Y = sideRec.y + slotSubH + 18.0f;
        float specH = 84.0f;
        Rectangle specRec = { col1X, row3Y + 12.0f, col1W, specH };
        bool isSpecHov = CheckCollisionPointRec(mousePos, specRec);
        DrawText("SPECIAL", (int)col1X, (int)row3Y, 10, Color{ 140, 160, 190, 220 });
        DrawRectangleRounded(specRec, 0.08f, 6, isSpecHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(specRec, 0.08f, 6, isSpecHov ? Color{ 120, 210, 255, 240 } : Color{ 36, 48, 68, 200 });
        DrawWireframeSpecial({ col1X + 38.0f, specRec.y + specH * 0.5f }, Color{ 255, 220, 90, 240 });
        DrawText(curTStats.name.c_str(), (int)col1X + 70, (int)specRec.y + 14, 11, Color{ 255, 255, 255, 255 });
        DrawText(curTStats.description.c_str(), (int)col1X + 70, (int)specRec.y + 32, 8, Color{ 160, 185, 215, 200 });
        if (isSpecHov) hoveredTooltip = "SPECIAL RELIC: " + curTStats.name + "\n" + curTStats.description;

        // 4. ROW 4: GRENADES / AUXILIARY FLASKS
        float row4Y = specRec.y + specH + 16.0f;
        DrawText("GRENADES", (int)col1X, (int)row4Y, 10, Color{ 140, 160, 190, 220 });
        float grenW = 54.0f;
        float grenH = 54.0f;
        Rectangle gren1Rec = { col1X + 30.0f, row4Y + 12.0f, grenW, grenH };
        Rectangle gren2Rec = { col1X + 96.0f, row4Y + 12.0f, grenW, grenH };
        DrawRectangleRounded(gren1Rec, 0.12f, 4, Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(gren1Rec, 0.12f, 4, Color{ 36, 48, 68, 200 });
        DrawWireframeGrenade({ gren1Rec.x + grenW * 0.5f, gren1Rec.y + grenH * 0.5f }, Color{ 255, 220, 90, 220 });
        DrawRectangleRounded(gren2Rec, 0.12f, 4, Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(gren2Rec, 0.12f, 4, Color{ 36, 48, 68, 200 });
        DrawWireframeGrenade({ gren2Rec.x + grenW * 0.5f, gren2Rec.y + grenH * 0.5f }, Color{ 255, 220, 90, 220 });

        // ------------------------------------------
        // COLUMN 2 (CENTER): HERO PAPER DOLL & PROGRESSION BADGE
        // ------------------------------------------
        float centerColX = modalX + 310.0f;
        float centerColW = 440.0f;
        Vector2 dollCenter = { centerColX + centerColW * 0.5f, contentY + 225.0f };

        // Tactical Pedestal Circle beneath character (Image 1)
        DrawEllipse((int)dollCenter.x, (int)(dollCenter.y + 168.0f), 125.0f, 32.0f, Color{ 10, 14, 22, 200 });
        DrawEllipseLines((int)dollCenter.x, (int)(dollCenter.y + 168.0f), 125.0f, 32.0f, Color{ 45, 65, 95, 180 });
        DrawEllipseLines((int)dollCenter.x, (int)(dollCenter.y + 168.0f), 95.0f, 24.0f, Color{ 60, 95, 135, 120 });

        // Draw animated Hero Paper Doll
        AssetManager::DrawCharacterDoll(dollCenter, 1.95f, player.GetWeapon(), player.GetArmor(), player.GetTotem(), (float)GetTime());

        // Ambient sparks around character
        float animT = (float)GetTime();
        for (int i = 0; i < 4; i++) {
            float spkX = dollCenter.x + cosf(animT * 1.5f + (float)i * 1.57f) * 110.0f;
            float spkY = dollCenter.y + sinf(animT * 1.2f + (float)i * 1.2f) * 130.0f;
            DrawCircle((int)spkX, (int)spkY, 1.8f, Color{ 140, 210, 255, 160 });
        }

        // Bottom-Left Level & Title Badge (Image 1: 23 JUST_BUCK_IT)
        float titleX = centerColX + 24.0f;
        float titleY = contentY + 418.0f;
        DrawText("23", (int)titleX, (int)titleY, 34, Color{ 255, 255, 255, 255 });
        DrawRectangle((int)titleX + 48, (int)titleY + 6, 3, 24, Color{ 255, 95, 75, 255 });
        DrawText("JUST_BUCK_IT", (int)titleX + 58, (int)titleY + 4, 16, Color{ 255, 95, 75, 255 });
        DrawText("SOLSDEN VANGUARD | ROYAL CHAMPION", (int)titleX + 58, (int)titleY + 23, 10, Color{ 130, 150, 180, 220 });

        // ------------------------------------------
        // COLUMN 3 (RIGHT): APPAREL & POTIONS
        // ------------------------------------------
        float col3X = modalX + modalW - 300.0f;
        float col3W = 260.0f;
        float appW = (col3W - 14.0f) * 0.5f;
        float appH = 92.0f;

        // ROW 1: HEAD & CHEST
        float appRow1Y = contentY + 14.0f;

        // HEAD
        Rectangle headRec = { col3X, appRow1Y + 12.0f, appW, appH };
        bool isHeadHov = CheckCollisionPointRec(mousePos, headRec);
        DrawText("HEAD", (int)col3X, (int)appRow1Y, 10, Color{ 140, 160, 190, 220 });
        DrawRectangleRounded(headRec, 0.08f, 4, isHeadHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(headRec, 0.08f, 4, isHeadHov ? Color{ 120, 210, 255, 240 } : Color{ 36, 48, 68, 200 });
        DrawWireframeHelmet({ headRec.x + appW * 0.5f, headRec.y + appH * 0.5f - 4.0f }, Color{ 255, 220, 90, 240 });
        DrawText("Vanguard Helm", (int)headRec.x + 8, (int)headRec.y + appH - 14.0f, 8, Color{ 170, 185, 210, 200 });
        if (isHeadHov) hoveredTooltip = "HEAD: Vanguard Steel Sallet\nTempered visor granting +10 Willpower posture resistance.";

        // CHEST
        Rectangle chestRec = { col3X + appW + 14.0f, appRow1Y + 12.0f, appW, appH };
        bool isChestHov = CheckCollisionPointRec(mousePos, chestRec);
        DrawText("CHEST", (int)chestRec.x, (int)appRow1Y, 10, Color{ 140, 160, 190, 220 });
        DrawRectangleRounded(chestRec, 0.08f, 4, isChestHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(chestRec, 0.08f, 4, isChestHov ? Color{ 120, 210, 255, 240 } : Color{ 36, 48, 68, 200 });
        DrawWireframeCuirass({ chestRec.x + appW * 0.5f, chestRec.y + appH * 0.5f - 4.0f }, Color{ 255, 220, 90, 240 });
        DrawText(curAStats.name.c_str(), (int)chestRec.x + 8, (int)chestRec.y + appH - 14.0f, 8, Color{ 170, 185, 210, 200 });
        if (isChestHov) hoveredTooltip = "CHEST: " + curAStats.name + "\n" + curAStats.description + "\nDEF: " + std::to_string((int)curAStats.defense);

        // ROW 2: BOOTS & LEGS
        float appRow2Y = headRec.y + appH + 18.0f;

        // BOOTS
        Rectangle bootRec = { col3X, appRow2Y + 12.0f, appW, appH };
        bool isBootHov = CheckCollisionPointRec(mousePos, bootRec);
        DrawText("BOOTS", (int)col3X, (int)appRow2Y, 10, Color{ 140, 160, 190, 220 });
        DrawRectangleRounded(bootRec, 0.08f, 4, isBootHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(bootRec, 0.08f, 4, isBootHov ? Color{ 120, 210, 255, 240 } : Color{ 36, 48, 68, 200 });
        DrawWireframeBoots({ bootRec.x + appW * 0.5f, bootRec.y + appH * 0.5f - 4.0f }, Color{ 255, 220, 90, 240 });
        DrawText("Plated Treads", (int)bootRec.x + 8, (int)bootRec.y + appH - 14.0f, 8, Color{ 170, 185, 210, 200 });
        if (isBootHov) hoveredTooltip = "BOOTS: Plated Sabatons\nReinforced treads providing steady footing and 100% dash recovery.";

        // LEGS
        Rectangle legRec = { col3X + appW + 14.0f, appRow2Y + 12.0f, appW, appH };
        bool isLegHov = CheckCollisionPointRec(mousePos, legRec);
        DrawText("LEGS", (int)legRec.x, (int)appRow2Y, 10, Color{ 140, 160, 190, 220 });
        DrawRectangleRounded(legRec, 0.08f, 4, isLegHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(legRec, 0.08f, 4, isLegHov ? Color{ 120, 210, 255, 240 } : Color{ 36, 48, 68, 200 });
        DrawWireframeLegs({ legRec.x + appW * 0.5f, legRec.y + appH * 0.5f - 4.0f }, Color{ 255, 220, 90, 240 });
        DrawText("Greaves", (int)legRec.x + 8, (int)legRec.y + appH - 14.0f, 8, Color{ 170, 185, 210, 200 });
        if (isLegHov) hoveredTooltip = "LEGS: Reinforced Combat Greaves\nChainmail leggings reducing bleed trauma by 20%.";

        // ROW 3: POTION QUICK-SLOT (Image 1)
        float appRow3Y = bootRec.y + appH + 18.0f;
        float potH = 88.0f;
        Rectangle potRec = { col3X, appRow3Y + 12.0f, col3W, potH };
        bool isPotHov = CheckCollisionPointRec(mousePos, potRec);
        DrawText("POTION", (int)col3X, (int)appRow3Y, 10, Color{ 140, 160, 190, 220 });
        DrawRectangleRounded(potRec, 0.08f, 6, isPotHov ? Color{ 22, 28, 42, 255 } : Color{ 14, 18, 28, 230 });
        DrawRectangleRoundedLines(potRec, 0.08f, 6, isPotHov ? Color{ 120, 210, 255, 240 } : Color{ 36, 48, 68, 200 });
        DrawWireframePotion({ col3X + 38.0f, potRec.y + potH * 0.5f }, Color{ 255, 220, 90, 240 });

        int potCount = player.GetInventory().GetHealthPotionCount();
        std::string potCountStr = "Health Elixir x" + std::to_string(potCount);
        DrawText(potCountStr.c_str(), (int)col3X + 70, (int)potRec.y + 16, 12, Color{ 255, 255, 255, 255 });
        DrawText("Restores 45 HP | Cooldown: 7s", (int)col3X + 70, (int)potRec.y + 34, 9, Color{ 160, 190, 220, 200 });
        DrawText("[CLICK OR PRESS E TO DRINK]", (int)col3X + 70, (int)potRec.y + 52, 9, isPotHov ? Color{ 255, 215, 80, 255 } : Color{ 120, 145, 175, 180 });

        if (isPotHov) hoveredTooltip = "POTION: Health Elixir\nConsumes 1 flask to restore 45 Health Points. Click or press [E] in combat.";
        if ((isPotHov && isMousePressed) || IsKeyPressed(KEY_E)) {
            // ParticleSystem particles; // Potion use
            // player.UseHealthPotion(particles);
            SoundFX::PlayButtonClick();
        }

        // Floating Inspection Tooltip Box (if hovering over any slot)
        if (!hoveredTooltip.empty()) {
            float tipW = 340.0f;
            float tipH = 75.0f;
            float tipX = mousePos.x + 18.0f;
            float tipY = mousePos.y + 12.0f;
            if (tipX + tipW > modalX + modalW - 10.0f) tipX = modalX + modalW - tipW - 10.0f;
            if (tipY + tipH > modalY + modalH - 10.0f) tipY = modalY + modalH - tipH - 10.0f;

            DrawRectangleRounded(Rectangle{ tipX, tipY, tipW, tipH }, 0.15f, 4, Color{ 8, 12, 18, 245 });
            DrawRectangleRoundedLines(Rectangle{ tipX, tipY, tipW, tipH }, 0.15f, 4, Color{ 120, 200, 255, 220 });
            DrawText(hoveredTooltip.c_str(), (int)tipX + 12, (int)tipY + 10, 10, Color{ 225, 240, 255, 240 });
        }
    }
    // ==========================================
    // TAB 0: QUESTS SCREEN (Active Objectives Tracker)
    // ==========================================
    else if (activeTab == 0) {
        float qX = modalX + 50.0f;
        float qY = contentY + 20.0f;
        DrawText("CAMPAIGN CHRONICLES & ACTIVE OBJECTIVES", (int)qX, (int)qY, 16, Color{ 255, 220, 90, 255 });
        DrawText("Track your oaths, kingdom investigations, and uncovered secrets across Tydeshire.", (int)qX, (int)qY + 24, 11, Color{ 160, 180, 210, 200 });

        struct QuestEntry { const char* title; const char* stage; const char* desc; bool done; };
        QuestEntry quests[3] = {
            { "Act I: Solsden Awakening", "Ruined Castle", "Acquire vanguard armament from weapon rack and overcome the lingering remnant.", true },
            { "Act II: Highway of the Damned", "Crossroads", "Cross the southern timberline, purge the bandit ambush, and find the border shrine.", true },
            { "Act III: The Vanished Liege", "Zyphden Keep", "Seek royal audience with King Kreptu upon the throne dais in the High Keep.", false }
        };

        for (int i = 0; i < 3; i++) {
            float boxY = qY + 65.0f + (float)i * 105.0f;
            Rectangle qRec = { qX, boxY, modalW - 100.0f, 90.0f };
            DrawRectangleRounded(qRec, 0.08f, 6, Color{ 16, 22, 32, 230 });
            DrawRectangleRoundedLines(qRec, 0.08f, 6, quests[i].done ? Color{ 90, 220, 140, 200 } : Color{ 255, 210, 70, 220 });

            DrawCircle((int)qX + 30, (int)boxY + 45, 12.0f, quests[i].done ? Color{ 40, 140, 70, 255 } : Color{ 180, 130, 30, 255 });
            DrawText(quests[i].done ? "v" : "!", (int)qX + 26, (int)boxY + 38, 14, Color{ 255, 255, 255, 255 });

            DrawText(quests[i].title, (int)qX + 60, (int)boxY + 16, 14, Color{ 255, 255, 255, 255 });
            DrawText(quests[i].stage, (int)qX + modalW - 240, (int)boxY + 16, 11, Color{ 140, 200, 255, 240 });
            DrawText(quests[i].desc, (int)qX + 60, (int)boxY + 42, 11, Color{ 170, 190, 215, 200 });
            DrawText(quests[i].done ? "[COMPLETED]" : "[ACTIVE IN PROGRESS]", (int)qX + 60, (int)boxY + 64, 9, quests[i].done ? Color{ 120, 230, 150, 220 } : Color{ 255, 210, 80, 220 });
        }
    }
    // ==========================================
    // TAB 2: INVENTORY & LORE JOURNAL SCREEN
    // ==========================================
    else if (activeTab == 2) {
        float jX = modalX + 50.0f;
        float jY = contentY + 20.0f;
        DrawText("LORE JOURNAL & DISCOVERED FRAGMENTS", (int)jX, (int)jY, 16, Color{ 255, 220, 90, 255 });

        // Left List: Fragment indices
        float listW = 280.0f;
        float listH = contentH - 60.0f;
        Rectangle listRec = { jX, jY + 35.0f, listW, listH };
        DrawRectangleRounded(listRec, 0.05f, 4, Color{ 16, 22, 32, 230 });
        DrawRectangleRoundedLines(listRec, 0.05f, 4, Color{ 40, 55, 80, 200 });

        const auto& fragments = player.GetInventory().GetFragments();
        int totalFrag = player.GetInventory().GetTotalFragmentCount();

        for (int i = 0; i < totalFrag && i < 8; i++) {
            float itemY = listRec.y + 10.0f + (float)i * 44.0f;
            Rectangle itemRec = { listRec.x + 8.0f, itemY, listW - 16.0f, 38.0f };
            bool isItmHov = CheckCollisionPointRec(mousePos, itemRec);
            bool isSel = (selectedFragmentIndex == i);

            if (isItmHov && isMousePressed) {
                selectedFragmentIndex = i;
                SoundFX::PlayButtonClick();
            }

            DrawRectangleRounded(itemRec, 0.15f, 4, isSel ? Color{ 36, 52, 78, 255 } : (isItmHov ? Color{ 24, 32, 48, 220 } : Color{ 18, 24, 36, 180 }));
            DrawRectangleRoundedLines(itemRec, 0.15f, 4, isSel ? Color{ 255, 215, 80, 240 } : Color{ 38, 50, 72, 180 });
            std::string label = "Fragment #" + std::to_string(i + 1);
            DrawText(label.c_str(), (int)itemRec.x + 14, (int)itemY + 12, 12, isSel ? Color{ 255, 230, 110, 255 } : Color{ 180, 200, 225, 200 });
        }

        // Right View: Reading Parchment
        float viewX = jX + listW + 24.0f;
        float viewW = modalW - (jX - modalX) * 2.0f - listW - 24.0f;
        Rectangle viewRec = { viewX, jY + 35.0f, viewW, listH };
        DrawRectangleRounded(viewRec, 0.05f, 6, Color{ 20, 26, 38, 240 });
        DrawRectangleRoundedLines(viewRec, 0.05f, 6, Color{ 60, 80, 115, 200 });

        DrawText("HISTORICAL EXCERPT", (int)viewX + 24, (int)viewRec.y + 20, 14, Color{ 255, 220, 90, 255 });
        DrawLine((int)viewX + 24, (int)viewRec.y + 44, (int)(viewX + viewW - 24), (int)viewRec.y + 44, Color{ 45, 60, 85, 200 });

        std::string excerpt = "When the Liege of Kings vanished beyond the mist, Lord Zil stood as the vanguard of Tydeshire.\nYet an encroaching bile seeped from the northern fractures, fracturing mind and oath alike.\nSeek out the Four Humors, wanderer, for only temperance of soul may quench the rising frenzy.";
        DrawText(excerpt.c_str(), (int)viewX + 24, (int)viewRec.y + 60, 11, Color{ 185, 205, 235, 220 });
    }
    // ==========================================
    // TAB 3: ABILITIES SCREEN (Four Humors & Composure Perks)
    // ==========================================
    else if (activeTab == 3) {
        float aX = modalX + 50.0f;
        float aY = contentY + 20.0f;
        DrawText("THE FOUR MEDIEVAL HUMORS & MARTIAL DISCIPLINES", (int)aX, (int)aY, 16, Color{ 255, 220, 90, 255 });
        DrawText("Align your physical constitution with medieval elemental philosophies.", (int)aX, (int)aY + 24, 11, Color{ 160, 180, 210, 200 });

        struct HumorDesc { const char* name; const char* elem; const char* perk; Color col; };
        HumorDesc humors[4] = {
            { "PHLEGMATIC (Player)", "Water & Cold", "Stoic Poise: Heavy attack hyper-armor & immune to stagger in trance.", Color{ 90, 215, 255, 255 } },
            { "CHOLERIC", "Fire & Yellow Bile", "Frenzy: Aggressive thrusts increase critical strike multiplier.", Color{ 255, 120, 50, 255 } },
            { "MELANCHOLIC", "Earth & Black Bile", "Brittle Shells: Hardened carapace shattered by heavy strikes.", Color{ 190, 140, 240, 255 } },
            { "SANGUINE", "Air & Blood", "Piercing Bleeds: Rapid thrusts inflict bleeding damage over time.", Color{ 255, 80, 95, 255 } }
        };

        for (int i = 0; i < 4; i++) {
            float hBoxY = aY + 65.0f + (float)i * 90.0f;
            Rectangle hRec = { aX, hBoxY, modalW - 100.0f, 76.0f };
            DrawRectangleRounded(hRec, 0.08f, 6, Color{ 16, 22, 32, 230 });
            DrawRectangleRoundedLines(hRec, 0.08f, 6, humors[i].col);

            DrawCircle((int)aX + 30, (int)hBoxY + 38, 12.0f, Color{ humors[i].col.r, humors[i].col.g, humors[i].col.b, 60 });
            DrawCircleLines((int)aX + 30, (int)hBoxY + 38, 12.0f, humors[i].col);

            DrawText(humors[i].name, (int)aX + 60, (int)hBoxY + 16, 14, humors[i].col);
            DrawText(humors[i].elem, (int)aX + 270, (int)hBoxY + 18, 11, Color{ 170, 190, 215, 200 });
            DrawText(humors[i].perk, (int)aX + 60, (int)hBoxY + 44, 11, Color{ 220, 235, 250, 220 });
        }
    }

    if (closeClicked) {
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

