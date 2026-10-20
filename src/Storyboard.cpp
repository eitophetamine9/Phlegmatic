#include "Storyboard.hpp"
#include <cmath>

StoryboardSystem::StoryboardSystem() {
    Init();
}

void StoryboardSystem::Init() {
    acts.clear();

    acts.push_back({
        "Prologue: The Forgotten Arena",
        "You awaken in the ruined subterranean colosseum of the Phlegmatic Order. Shadows stir in the dim torchlight.",
        "Trial 1: Practice WASD / Arrow movement and test Light Attack (L-Click) & Heavy Attack (R-Click)."
    });

    acts.push_back({
        "Act I: The Blade's Warning",
        "Slasher sentinels emerge from the dark, telegraphing their lethal curved strikes across the stone floor.",
        "Trial 2: Observe the red cone telegraphs. Time your [Q] key right before impact to execute a PERFECT PARRY!"
    });

    acts.push_back({
        "Act II: Titans of the Pit",
        "The ground trembles as Brute crushers charge their devastating ground slams and Chargers prepare linear lunges.",
        "Trial 3: Stagger the Brute by parrying or sidestepping the expanding red circle, then unleash heavy counter-strikes!"
    });

    currentActIndex = 0;
    showOverlay = false;
}

void StoryboardSystem::Update(float dt) {
    if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_F1)) {
        ToggleOverlay();
    }
    if (showOverlay) {
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_E)) {
            NextAct();
        }
        if (IsKeyPressed(KEY_LEFT)) {
            PrevAct();
        }
    }
}

void StoryboardSystem::NextAct() {
    if (currentActIndex + 1 < (int)acts.size()) {
        currentActIndex++;
    } else {
        currentActIndex = 0;
    }
}

void StoryboardSystem::PrevAct() {
    if (currentActIndex - 1 >= 0) {
        currentActIndex--;
    } else {
        currentActIndex = (int)acts.size() - 1;
    }
}

void StoryboardSystem::DrawHUD(int windowWidth, int windowHeight) const {
    const auto& current = acts[currentActIndex];

    // Top Storyboard Banner (Glassmorphic dark pill)
    const int bannerWidth = 720;
    const int bannerHeight = 52;
    int bannerX = (windowWidth - bannerWidth) / 2;
    int bannerY = 14;

    // Dark glass background
    DrawRectangleRounded(Rectangle{ (float)bannerX, (float)bannerY, (float)bannerWidth, (float)bannerHeight }, 0.25f, 8, Color{ 16, 20, 28, 210 });
    DrawRectangleRoundedLines(Rectangle{ (float)bannerX, (float)bannerY, (float)bannerWidth, (float)bannerHeight }, 0.25f, 8, Color{ 55, 70, 95, 230 });

    // Act title
    DrawText(current.actTitle.c_str(), bannerX + 16, bannerY + 8, 15, Color{ 255, 210, 80, 255 });

    // Objective text
    DrawText(current.combatObjective.c_str(), bannerX + 16, bannerY + 28, 13, Color{ 210, 225, 245, 230 });

    // TAB overlay hint on right
    DrawRectangleRounded(Rectangle{ (float)(bannerX + bannerWidth - 110), (float)(bannerY + 12), 94, 28 }, 0.3f, 4, Color{ 35, 45, 65, 220 });
    DrawText("[TAB] Story", bannerX + bannerWidth - 100, bannerY + 19, 12, Color{ 140, 200, 255, 255 });
}

void StoryboardSystem::DrawOverlay(int windowWidth, int windowHeight) const {
    if (!showOverlay) return;

    // Dim background overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 10, 16, 225 });

    // Main Storyboard Modal Window
    const int modalWidth = 860;
    const int modalHeight = 520;
    int modalX = (windowWidth - modalWidth) / 2;
    int modalY = (windowHeight - modalHeight) / 2;

    DrawRectangleRounded(Rectangle{ (float)modalX, (float)modalY, (float)modalWidth, (float)modalHeight }, 0.10f, 8, Color{ 18, 22, 32, 250 });
    DrawRectangleRoundedLines(Rectangle{ (float)modalX, (float)modalY, (float)modalWidth, (float)modalHeight }, 0.10f, 8, Color{ 70, 90, 130, 255 });

    // Header
    DrawText("STORYBOARD & COMBAT ARCHITECTURE", modalX + 30, modalY + 24, 22, Color{ 255, 220, 90, 255 });
    DrawText("Use [LEFT / RIGHT] or [E] to change chapters  |  Press [TAB] to close", modalX + 30, modalY + 54, 13, Color{ 160, 180, 210, 200 });

    DrawLine(modalX + 30, modalY + 76, modalX + modalWidth - 30, modalY + 76, Color{ 40, 52, 75, 255 });

    // Chapter Navigation Buttons / Tabs
    for (size_t i = 0; i < acts.size(); i++) {
        int tabX = modalX + 30 + (int)i * 260;
        int tabY = modalY + 90;
        bool isSel = ((int)i == currentActIndex);
        Color tabBg = isSel ? Color{ 45, 65, 105, 255 } : Color{ 25, 30, 42, 220 };
        Color tabBorder = isSel ? Color{ 100, 200, 255, 255 } : Color{ 50, 60, 80, 200 };

        DrawRectangleRounded(Rectangle{ (float)tabX, (float)tabY, 245, 36 }, 0.2f, 4, tabBg);
        DrawRectangleRoundedLines(Rectangle{ (float)tabX, (float)tabY, 245, 36 }, 0.2f, 4, tabBorder);
        DrawText(acts[i].actTitle.c_str(), tabX + 12, tabY + 10, 13, isSel ? Color{ 255, 255, 255, 255 } : Color{ 170, 185, 205, 220 });
    }

    // Active Chapter Narrative Details
    const auto& cur = acts[currentActIndex];
    int contentY = modalY + 145;

    DrawText("NARRATIVE PREMISE:", modalX + 30, contentY, 15, Color{ 255, 180, 60, 255 });
    DrawText(cur.narrative.c_str(), modalX + 30, contentY + 24, 14, Color{ 220, 230, 245, 240 });

    DrawText("CURRENT OBJECTIVE & COMBAT TRIAL:", modalX + 30, contentY + 68, 15, Color{ 100, 225, 255, 255 });
    DrawText(cur.combatObjective.c_str(), modalX + 30, contentY + 92, 14, Color{ 240, 245, 255, 255 });

    // Controls & Combat Breakdown Panel
    int panelY = contentY + 140;
    DrawRectangleRounded(Rectangle{ (float)(modalX + 30), (float)panelY, (float)(modalWidth - 60), 170 }, 0.08f, 6, Color{ 24, 30, 44, 255 });
    DrawRectangleRoundedLines(Rectangle{ (float)(modalX + 30), (float)panelY, (float)(modalWidth - 60), 170 }, 0.08f, 6, Color{ 50, 65, 95, 255 });

    DrawText("ACTIVE COMBAT MECHANICS:", modalX + 48, panelY + 16, 14, Color{ 255, 215, 80, 255 });

    DrawText("- WASD or Arrow Keys : 8-directional fluid top-down movement", modalX + 48, panelY + 40, 13, Color{ 200, 215, 235, 240 });
    DrawText("- Left Mouse Click     : Light Attack (Rapid slash arc, low stamina, interrupts basic foes)", modalX + 48, panelY + 62, 13, Color{ 120, 230, 255, 240 });
    DrawText("- Right Mouse Click    : Heavy Attack (Charged cleave, devastating damage & high knockback)", modalX + 48, panelY + 84, 13, Color{ 255, 185, 80, 240 });
    DrawText("- Q Key [Parry]        : Raise parry shield at right time! Deflects attack, stuns foe, opens counter-buff!", modalX + 48, panelY + 106, 13, Color{ 255, 235, 110, 255 });
    DrawText("- Enemy Telegraphs     : Foes show warning sectors/circles/lines filling with red before striking!", modalX + 48, panelY + 128, 13, Color{ 255, 110, 110, 240 });
}
