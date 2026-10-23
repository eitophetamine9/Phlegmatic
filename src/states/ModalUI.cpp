#include "ModalUI.hpp"
#include "../core/SoundFX.hpp"
#include "../combat/Weapon.hpp"
#include "../entities/Player.hpp"
#include <cmath>
#include <sstream>

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

bool ModalUI::DrawControls(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed) {
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

    if ((isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE)) {
        SoundFX::PlayButtonClick();
        return true; // Close requested
    }
    return false;
}

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

    // TAB CONTENT AREA
    float contentY = modalY + 125.0f;

    if (activeTab == 0) {
        // TAB 0: EQUIPMENT & ARSENAL
        float col1X = modalX + 36.0f;
        float col1W = 510.0f;

        DrawText("CURRENTLY WIELDED WEAPON", (int)col1X, (int)contentY, 13, Color{ 255, 220, 90, 255 });

        WeaponType curWeapon = player.GetWeapon();
        WeaponStats curStats = GetWeaponStats(curWeapon);

        Rectangle curWRec = { col1X, contentY + 24.0f, col1W, 115.0f };
        DrawRectangleRounded(curWRec, 0.1f, 6, Color{ 22, 30, 46, 240 });
        DrawRectangleRoundedLines(curWRec, 0.1f, 6, (curWeapon == WeaponType::TRUTH_SEEKER) ? Color{ 255, 215, 80, 255 } : Color{ 80, 120, 175, 240 });

        DrawText(curStats.name.c_str(), (int)col1X + 20, (int)contentY + 40, 18, Color{ 255, 255, 255, 255 });
        DrawText(curStats.description.c_str(), (int)col1X + 20, (int)contentY + 66, 11, Color{ 180, 205, 235, 230 });

        std::string statsLine = "Light Damage: " + std::to_string((int)curStats.lightDamage) + "  |  Heavy Damage: " + std::to_string((int)curStats.heavyDamage) + "  |  Arc: " + std::to_string((int)curStats.lightArc) + " deg";
        DrawText(statsLine.c_str(), (int)col1X + 20, (int)contentY + 92, 11, Color{ 120, 225, 255, 240 });

        // Unlocked Weapons Section
        float arsenalY = contentY + 160.0f;
        DrawText("AVAILABLE WEAPONS IN ARSENAL", (int)col1X, (int)arsenalY, 13, Color{ 255, 220, 90, 255 });

        struct WeaponEntry {
            WeaponType type;
            const char* label;
        };
        WeaponEntry availableWeapons[] = {
            { WeaponType::SWORD, "Crusader's Broadsword" },
            { WeaponType::DAGGERS, "Dual Daggers" },
            { WeaponType::MACE, "War Mace" },
            { WeaponType::TRUTH_SEEKER, "Truth Seeker (Legendary Holy Blade)" }
        };

        for (int i = 0; i < 4; i++) {
            float itemY = arsenalY + 24.0f + (float)i * 68.0f;
            Rectangle itemRec = { col1X, itemY, col1W, 58.0f };
            bool isUnlocked = player.GetInventory().HasWeapon(availableWeapons[i].type);
            bool isEquipped = (player.GetWeapon() == availableWeapons[i].type);
            bool isHov = CheckCollisionPointRec(mousePos, itemRec);

            Color bgCol = isEquipped ? Color{ 34, 48, 72, 255 } : (isHov && isUnlocked ? Color{ 26, 36, 54, 240 } : Color{ 18, 22, 34, 200 });
            Color borderCol = isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? Color{ 55, 75, 105, 220 } : Color{ 40, 45, 60, 160 });

            DrawRectangleRounded(itemRec, 0.15f, 4, bgCol);
            DrawRectangleRoundedLines(itemRec, 0.15f, 4, borderCol);

            WeaponStats st = GetWeaponStats(availableWeapons[i].type);
            DrawText(availableWeapons[i].label, (int)col1X + 16, (int)itemY + 12, 14, isUnlocked ? Color{ 255, 255, 255, 255 } : Color{ 110, 120, 140, 180 });

            std::string subDesc = isUnlocked
                ? (isEquipped ? "[EQUIPPED ACTIVE BLADE]" : "Click or press [E] in lobby to equip")
                : "Locked // Defeat Zil the Corrupted Crusader in Chapter 1";
            DrawText(subDesc.c_str(), (int)col1X + 16, (int)itemY + 34, 10, isEquipped ? Color{ 255, 220, 100, 255 } : (isUnlocked ? Color{ 160, 190, 220, 220 } : Color{ 210, 90, 90, 220 }));

            if (isUnlocked && !isEquipped && isHov && isMousePressed) {
                player.SetWeapon(availableWeapons[i].type);
                SoundFX::PlayEquipWeapon();
            }
        }

        // Right Column: Combat Attributes & Passives
        float col2X = modalX + 575.0f;
        float col2W = modalW - 610.0f;

        DrawText("MARTIAL ATTRIBUTES & RATINGS", (int)col2X, (int)contentY, 13, Color{ 255, 220, 90, 255 });

        Rectangle statsBox = { col2X, contentY + 24.0f, col2W, 195.0f };
        DrawRectangleRounded(statsBox, 0.08f, 6, Color{ 18, 23, 34, 220 });
        DrawRectangleRoundedLines(statsBox, 0.08f, 6, Color{ 55, 75, 105, 230 });

        DrawText("Base Health:", (int)col2X + 20, (int)contentY + 40, 11, Color{ 140, 210, 255, 240 });
        std::string hpStr = std::to_string((int)player.GetMaxHealth()) + " HP";
        DrawText(hpStr.c_str(), (int)col2X + 175, (int)contentY + 40, 11, Color{ 255, 255, 255, 255 });

        DrawText("Stamina Pool:", (int)col2X + 20, (int)contentY + 62, 11, Color{ 140, 210, 255, 240 });
        std::string stamStr = std::to_string((int)player.GetMaxStamina()) + " SP";
        DrawText(stamStr.c_str(), (int)col2X + 175, (int)contentY + 62, 11, Color{ 255, 255, 255, 255 });

        DrawText("Stamina Regen:", (int)col2X + 20, (int)contentY + 84, 11, Color{ 140, 210, 255, 240 });
        DrawText("35 / sec (Regenerates during neutral)", (int)col2X + 175, (int)contentY + 84, 11, Color{ 255, 255, 255, 255 });

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
            Color border = isSel ? Color{ 255, 215, 80, 240 } : (frags[i].discovered ? (isHov ? Color{ 100, 160, 230, 220 } : Color{ 50, 68, 96, 200 }) : Color{ 35, 42, 58, 160 });

            DrawRectangleRounded(fragRec, 0.12f, 4, bg);
            DrawRectangleRoundedLines(fragRec, 0.12f, 4, border);

            if (frags[i].discovered) {
                DrawText(frags[i].realm.c_str(), (int)col1X + 16, (int)fragY + 12, 10, Color{ 140, 210, 255, 220 });
                DrawText(frags[i].title.c_str(), (int)col1X + 16, (int)fragY + 28, 14, Color{ 255, 255, 255, 255 });
                DrawText(frags[i].preview.c_str(), (int)col1X + 16, (int)fragY + 50, 11, Color{ 175, 195, 220, 200 });

                if (!frags[i].isRead) {
                    DrawRectangleRounded(Rectangle{ col1X + col1W - 74, fragY + 10, 60, 18 }, 0.3f, 4, Color{ 255, 180, 40, 240 });
                    DrawText("NEW", (int)(col1X + col1W - 56), (int)fragY + 13, 10, Color{ 10, 12, 18, 255 });
                }
            } else {
                DrawText("??? SHROUDED RECORD", (int)col1X + 16, (int)fragY + 14, 10, Color{ 120, 130, 150, 200 });
                DrawText("Undiscovered Fragment", (int)col1X + 16, (int)fragY + 32, 14, Color{ 150, 160, 180, 180 });
                DrawText("Explore Tydeshire and defeat guardians to reveal", (int)col1X + 16, (int)fragY + 54, 11, Color{ 110, 120, 140, 180 });
            }

            if (isHov && isMousePressed) {
                selectedFragmentIndex = i;
                player.GetInventory().MarkFragmentRead(frags[i].id);
                SoundFX::PlayButtonClick();
            }
        }

        // Right Column: Chronicle Text Reader
        float col2X = modalX + 415.0f;
        float col2W = modalW - 450.0f;
        float col2H = 465.0f;

        DrawRectangleRounded(Rectangle{ col2X, contentY + 24.0f, col2W, col2H }, 0.04f, 6, Color{ 16, 20, 30, 240 });
        DrawRectangleRoundedLines(Rectangle{ col2X, contentY + 24.0f, col2W, col2H }, 0.04f, 6, Color{ 205, 165, 75, 220 });
        DrawRectangleRoundedLines(Rectangle{ col2X + 4, contentY + 28.0f, col2W - 8, col2H - 8 }, 0.04f, 6, Color{ 50, 65, 90, 180 });

        if (selectedFragmentIndex >= 0 && selectedFragmentIndex < totalFrags) {
            const auto& curFrag = frags[selectedFragmentIndex];
            if (curFrag.discovered) {
                std::string originStr = "HISTORICAL RECORD // " + curFrag.realm;
                DrawText(originStr.c_str(), (int)col2X + 28, (int)contentY + 48, 11, Color{ 120, 220, 255, 240 });

                DrawText(curFrag.title.c_str(), (int)col2X + 28, (int)contentY + 68, 20, Color{ 255, 215, 80, 255 });
                DrawLine((int)col2X + 28, (int)contentY + 100, (int)(col2X + col2W - 28), (int)contentY + 100, Color{ 160, 125, 55, 220 });

                DrawJournalWrappedText(curFrag.fullText, (int)col2X + 28, (int)contentY + 122, (int)col2W - 56, 14, 8, Color{ 230, 238, 250, 245 });

                DrawLine((int)col2X + 28, (int)(contentY + 24.0f + col2H - 46), (int)(col2X + col2W - 28), (int)(contentY + 24.0f + col2H - 46), Color{ 50, 65, 90, 180 });
                const char* footQuote = "“The true history of Tydeshire has been scattered. Seek the ruins for meaning.”";
                DrawText(footQuote, (int)col2X + 28, (int)(contentY + 24.0f + col2H - 32), 11, Color{ 200, 170, 90, 220 });
            } else {
                int uhW = MeasureText("THIS CHRONICLE FRAGMENT REMAINS SHROUDED", 16);
                DrawText("THIS CHRONICLE FRAGMENT REMAINS SHROUDED", (int)(col2X + (col2W - uhW) * 0.5f), (int)contentY + 184, 16, Color{ 215, 90, 80, 255 });

                const char* unDesc1 = "The parchment detailing this epoch was torn asunder during the Lord Liege's madness.";
                int ud1W = MeasureText(unDesc1, 12);
                DrawText(unDesc1, (int)(col2X + (col2W - ud1W) * 0.5f), (int)contentY + 224, 12, Color{ 170, 190, 215, 220 });

                const char* unDesc2 = "Defeat corrupted crusaders and search forgotten shrines across the Land of Tydeshire to recover it.";
                int ud2W = MeasureText(unDesc2, 12);
                DrawText(unDesc2, (int)(col2X + (col2W - ud2W) * 0.5f), (int)contentY + 248, 12, Color{ 150, 170, 195, 200 });
            }
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

    int headerW = MeasureText("DEFEAT // ZIL'S JUDGMENT", 20);
    DrawText("DEFEAT // ZIL'S JUDGMENT", (int)(modalX + (modalW - headerW) * 0.5f), (int)modalY + 28, 20, Color{ 255, 90, 70, 255 });
    DrawLine((int)modalX + 50, (int)modalY + 62, (int)(modalX + modalW - 50), (int)modalY + 62, Color{ 140, 70, 30, 200 });

    int choiceResult = 0;

    if (canRetry) {
        int subW = MeasureText("THE FALLEN CRUSADER STANDS VICTORIOUS", 15);
        DrawText("THE FALLEN CRUSADER STANDS VICTORIOUS", (int)(modalX + (modalW - subW) * 0.5f), (int)modalY + 84, 15, Color{ 255, 205, 80, 255 });

        std::string quoteText =
            "“You fought with fierce intent, wanderer, but Truth Seeker remains unyielding in these hands.\n\n"
            "Will you rise once more and test your blade... or yield and carry onward into Zyphden?”";
        DrawJournalWrappedText(quoteText, (int)modalX + 50, (int)modalY + 120, (int)modalW - 100, 14, 8, Color{ 230, 235, 245, 240 });

        const char* noteText = "Honor bound: You are granted a singular retry duel to claim the legendary blade.";
        int noteW = MeasureText(noteText, 11);
        DrawText(noteText, (int)(modalX + (modalW - noteW) * 0.5f), (int)modalY + 280, 11, Color{ 255, 170, 60, 220 });

        Rectangle btnRetry = { modalX + 50.0f, modalY + 330.0f, 300.0f, 54.0f };
        bool hoverRetry = CheckCollisionPointRec(mousePos, btnRetry);
        DrawRectangleRounded(btnRetry, 0.2f, 6, hoverRetry ? Color{ 90, 35, 20, 255 } : Color{ 45, 22, 16, 240 });
        DrawRectangleRoundedLines(btnRetry, 0.2f, 6, hoverRetry ? Color{ 255, 190, 70, 255 } : Color{ 220, 120, 40, 230 });
        int btn1W = MeasureText("RETRY DUEL [1 REMAINING]", 14);
        DrawText("RETRY DUEL [1 REMAINING]", (int)(btnRetry.x + (btnRetry.width - btn1W) * 0.5f), (int)btnRetry.y + 19, 14, hoverRetry ? Color{ 255, 255, 255, 255 } : Color{ 255, 215, 100, 240 });

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
        int subW = MeasureText("SPARED BY THE CORRUPTED CRUSADER", 15);
        DrawText("SPARED BY THE CORRUPTED CRUSADER", (int)(modalX + (modalW - subW) * 0.5f), (int)modalY + 84, 15, Color{ 140, 210, 255, 255 });

        std::string quoteText =
            "“You have exhausted your trials, yet your spirit refuses to be extinguished.\n\n"
            "I shall not strike down one who dares to question the Lord Liege. Keep your broadsword and walk through the northern gate into Zyphden.\n\n"
            "Seek the scattered chronicles... and pierce the truth of King Charlz.”";
        DrawJournalWrappedText(quoteText, (int)modalX + 50, (int)modalY + 120, (int)modalW - 100, 14, 8, Color{ 230, 235, 245, 240 });

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

    int headerW = MeasureText("CHAPTER 1 COMPLETE // THE GATES TO ZYPHDEN UNLOCKED", 22);
    DrawText("CHAPTER 1 COMPLETE // THE GATES TO ZYPHDEN UNLOCKED", (int)(cardX + (cardW - headerW) * 0.5f), (int)cardY + 28, 22, Color{ 255, 215, 80, 255 });
    
    const char* subQuote = "“The ancient sanctuary lies behind you. The vast overland wilderness of Tydeshire awaits.”";
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
        { "ZYPHDEN", "UNLOCKED", "Windborne Spires & Ancient Altars", Color{ 100, 230, 185, 255 }, true },
        { "AZSHDEN", "LOCKED", "Sunken Bastion & Coastal Keeps", Color{ 80, 160, 240, 180 }, false },
        { "FLEURSHDEN", "LOCKED", "Blooming Thorns & Royal Knights", Color{ 235, 120, 165, 180 }, false },
        { "NOIRDEN", "LOCKED", "Shrouded Obsidian Peaks", Color{ 180, 115, 245, 180 }, false },
        { "CITADEL", "LOCKED", "Throne of Charlz the Lord Liege", Color{ 255, 75, 85, 180 }, false }
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
    WeaponStats wStats = GetWeaponStats(player.GetWeapon());
    DrawText(wStats.name.c_str(), (int)col1X + 20, (int)sec2Y + 58, 18, (player.GetWeapon() == WeaponType::TRUTH_SEEKER) ? Color{ 255, 225, 100, 255 } : Color{ 220, 235, 255, 255 });

    std::string wStatsStr = "Damage: " + std::to_string((int)wStats.lightDamage) + " Light / " + std::to_string((int)wStats.heavyDamage) + " Heavy  |  Speed: " + std::to_string((int)(1.0f / wStats.lightCooldown)) + " atk/s";
    DrawText(wStatsStr.c_str(), (int)col1X + 20, (int)sec2Y + 84, 12, Color{ 160, 190, 220, 230 });

    if (player.GetWeapon() == WeaponType::TRUTH_SEEKER) {
        DrawText("SPECIAL: Dawnfire Rune Blade (+60% Counter Bonus on Parry)", (int)col1X + 20, (int)sec2Y + 106, 11, Color{ 255, 175, 45, 255 });
        DrawText("Earned by defeating Zil the Corrupted Crusader at the High Altar.", (int)col1X + 20, (int)sec2Y + 126, 11, Color{ 180, 210, 240, 220 });
    } else {
        DrawText("SPECIAL: Basic steel armament. Spared by Zil the Corrupted Crusader.", (int)col1X + 20, (int)sec2Y + 106, 11, Color{ 160, 200, 240, 220 });
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
    DrawText(hasFrag3 ? "Fragment III: The King's Madness - Uncovered" : "Fragment III: The King's Madness - Shrouded (Defeat Zil)", (int)col2X + 20, (int)sec2Y + 122, 11, hasFrag3 ? Color{ 255, 215, 80, 240 } : Color{ 130, 145, 170, 200 });

    DrawText("Tip: Press [I] anywhere in the world to open your Lore Journal.", (int)col2X + 20, (int)sec2Y + 154, 11, Color{ 255, 180, 70, 230 });

    int choiceResult = 0;

    Rectangle btnCastle = { cardX + 160.0f, cardY + 550.0f, 340.0f, 54.0f };
    bool hoverCastle = CheckCollisionPointRec(mousePos, btnCastle);
    DrawRectangleRounded(btnCastle, 0.2f, 6, hoverCastle ? Color{ 40, 60, 95, 255 } : Color{ 24, 34, 52, 240 });
    DrawRectangleRoundedLines(btnCastle, 0.2f, 6, hoverCastle ? Color{ 140, 220, 255, 255 } : Color{ 80, 125, 180, 230 });
    int btn1W = MeasureText("RE-ENTER RUINED CASTLE", 14);
    DrawText("RE-ENTER RUINED CASTLE", (int)(btnCastle.x + (btnCastle.width - btn1W) * 0.5f), (int)btnCastle.y + 19, 14, hoverCastle ? Color{ 255, 255, 255, 255 } : Color{ 200, 230, 255, 240 });

    Rectangle btnLobby = { cardX + 560.0f, cardY + 550.0f, 340.0f, 54.0f };
    bool hoverLobby = CheckCollisionPointRec(mousePos, btnLobby);
    DrawRectangleRounded(btnLobby, 0.2f, 6, hoverLobby ? Color{ 75, 45, 25, 255 } : Color{ 42, 26, 18, 240 });
    DrawRectangleRoundedLines(btnLobby, 0.2f, 6, hoverLobby ? Color{ 255, 190, 70, 255 } : Color{ 205, 125, 50, 230 });
    int btn2W = MeasureText("RETURN TO SANCTUARY LOBBY", 14);
    DrawText("RETURN TO SANCTUARY LOBBY", (int)(btnLobby.x + (btnLobby.width - btn2W) * 0.5f), (int)btnLobby.y + 19, 14, hoverLobby ? Color{ 255, 255, 255, 255 } : Color{ 255, 220, 110, 240 });

    if (isMousePressed) {
        if (hoverCastle) {
            SoundFX::PlayButtonClick();
            choiceResult = 1;
        } else if (hoverLobby) {
            SoundFX::PlayButtonClick();
            choiceResult = 2;
        }
    }

    return choiceResult;
}
