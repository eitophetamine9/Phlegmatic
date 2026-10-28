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
        { "Q Key",         "DASH",     "High-velocity directional dash with i-frames invulnerability. Evade attacks!", Color{ 120, 240, 180, 255 } },
        { "F Key",         "PARRY",    "Active 0.28s guard. Timely parries stagger foes & grant +50% counter dmg buff!", Color{ 255, 230, 80, 255 } },
        { "E Key",         "Interact", "Equip weapon at pedestals or enter Arena Portal.", Color{ 200, 150, 255, 255 } },
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
        // TAB 0: EQUIPMENT, CHARACTER PREVIEW & MARTIAL STATS
        static int s_equipCategory = 0; // 0 = WEAPONS, 1 = ARMORS, 2 = TOTEMS

        // COLUMN 1: CHARACTER DOLL & QUICK-SLOT BADGES
        float col1X = modalX + 34.0f;
        float col1W = 260.0f;

        DrawText("ROYAL CHAMPION", (int)col1X, (int)contentY, 13, Color{ 255, 220, 90, 255 });

        // Doll Stage Box
        Rectangle dollBox = { col1X, contentY + 22.0f, col1W, 260.0f };
        DrawRectangleRounded(dollBox, 0.08f, 6, Color{ 18, 23, 34, 230 });
        DrawRectangleRoundedLines(dollBox, 0.08f, 6, Color{ 60, 80, 115, 240 });

        // Draw animated paper doll
        Vector2 dollCenter = { col1X + col1W * 0.5f, contentY + 140.0f };
        AssetManager::DrawCharacterDoll(dollCenter, 1.35f, player.GetWeapon(), player.GetArmor(), player.GetTotem(), (float)GetTime());

        // Subtitle below doll: Current Armor & Totem
        ArmorStats curArmStats = GetArmorStats(player.GetArmor());
        TotemStats curTotStats = GetTotemStats(player.GetTotem());
        int anw = MeasureText(curArmStats.name.c_str(), 11);
        DrawText(curArmStats.name.c_str(), (int)(col1X + (col1W - anw) * 0.5f), (int)(dollBox.y + dollBox.height - 24.0f), 11, Color{ 200, 220, 245, 230 });

        // Quick Slot Badges below doll box
        float slotStartY = contentY + 292.0f;
        float slotH = 58.0f;

        // Weapon Quick Slot
        Rectangle slotWRec = { col1X, slotStartY, col1W, slotH };
        bool isWSlotHov = CheckCollisionPointRec(mousePos, slotWRec);
        if (isWSlotHov && isMousePressed) {
            s_equipCategory = 0;
            SoundFX::PlayButtonClick();
        }
        DrawRectangleRounded(slotWRec, 0.12f, 4, (s_equipCategory == 0) ? Color{ 32, 44, 66, 255 } : (isWSlotHov ? Color{ 24, 32, 48, 220 } : Color{ 18, 24, 36, 200 }));
        DrawRectangleRoundedLines(slotWRec, 0.12f, 4, (s_equipCategory == 0) ? Color{ 255, 215, 80, 240 } : Color{ 50, 70, 100, 200 });
        DrawText("[WEAPON SLOT]", (int)col1X + 12, (int)slotStartY + 8, 10, Color{ 140, 210, 255, 220 });
        WeaponStats wStats = GetWeaponStats(player.GetWeapon(), player.GetInventory().GetWeaponUpgrade(player.GetWeapon()));
        DrawText(wStats.name.c_str(), (int)col1X + 12, (int)slotStartY + 24, 13, Color{ 255, 255, 255, 255 });
        DrawText("Click to view arsenal", (int)col1X + 12, (int)slotStartY + 41, 9, Color{ 160, 180, 200, 180 });

        // Armor Quick Slot
        Rectangle slotARec = { col1X, slotStartY + 64.0f, col1W, slotH };
        bool isASlotHov = CheckCollisionPointRec(mousePos, slotARec);
        if (isASlotHov && isMousePressed) {
            s_equipCategory = 1;
            SoundFX::PlayButtonClick();
        }
        DrawRectangleRounded(slotARec, 0.12f, 4, (s_equipCategory == 1) ? Color{ 32, 44, 66, 255 } : (isASlotHov ? Color{ 24, 32, 48, 220 } : Color{ 18, 24, 36, 200 }));
        DrawRectangleRoundedLines(slotARec, 0.12f, 4, (s_equipCategory == 1) ? Color{ 255, 215, 80, 240 } : Color{ 50, 70, 100, 200 });
        DrawText("[ARMOR SLOT]", (int)col1X + 12, (int)slotStartY + 72, 10, Color{ 140, 210, 255, 220 });
        DrawText(curArmStats.name.c_str(), (int)col1X + 12, (int)slotStartY + 88, 13, Color{ 255, 255, 255, 255 });
        DrawText("Click to view armors", (int)col1X + 12, (int)slotStartY + 105, 9, Color{ 160, 180, 200, 180 });

        // Totem Quick Slot
        Rectangle slotTRec = { col1X, slotStartY + 128.0f, col1W, slotH };
        bool isTSlotHov = CheckCollisionPointRec(mousePos, slotTRec);
        if (isTSlotHov && isMousePressed) {
            s_equipCategory = 2;
            SoundFX::PlayButtonClick();
        }
        DrawRectangleRounded(slotTRec, 0.12f, 4, (s_equipCategory == 2) ? Color{ 32, 44, 66, 255 } : (isTSlotHov ? Color{ 24, 32, 48, 220 } : Color{ 18, 24, 36, 200 }));
        DrawRectangleRoundedLines(slotTRec, 0.12f, 4, (s_equipCategory == 2) ? Color{ 255, 215, 80, 240 } : Color{ 50, 70, 100, 200 });
        DrawText("[TOTEM RELIC]", (int)col1X + 12, (int)slotStartY + 136, 10, Color{ 140, 210, 255, 220 });
        std::string tName = (player.GetTotem() == TotemType::NONE) ? "None Attuned" : curTotStats.name;
        DrawText(tName.c_str(), (int)col1X + 12, (int)slotStartY + 152, 13, (player.GetTotem() == TotemType::NONE) ? Color{ 160, 175, 195, 200 } : curTotStats.glowColor);
        DrawText("Click to view totems", (int)col1X + 12, (int)slotStartY + 169, 9, Color{ 160, 180, 200, 180 });

        // COLUMN 2: EQUIPMENT SELECTION & ARSENAL
        float col2X = modalX + 310.0f;
        float col2W = 410.0f;

        // Category Tabs: WEAPONS, ARMORS, TOTEMS
        float catW = (col2W - 16.0f) / 3.0f;
        const char* catNames[3] = { "WEAPONS (4)", "ARMORS (4)", "TOTEMS (5)" };
        for (int c = 0; c < 3; c++) {
            Rectangle catRec = { col2X + (float)c * (catW + 8.0f), contentY, catW, 28.0f };
            bool isCatHov = CheckCollisionPointRec(mousePos, catRec);
            bool isCatActive = (s_equipCategory == c);
            if (isCatHov && isMousePressed) {
                s_equipCategory = c;
                SoundFX::PlayButtonClick();
            }
            DrawRectangleRounded(catRec, 0.2f, 4, isCatActive ? Color{ 36, 48, 72, 255 } : (isCatHov ? Color{ 26, 34, 50, 220 } : Color{ 18, 22, 34, 180 }));
            DrawRectangleRoundedLines(catRec, 0.2f, 4, isCatActive ? Color{ 255, 215, 80, 240 } : Color{ 55, 75, 105, 200 });
            int tw = MeasureText(catNames[c], 11);
            DrawText(catNames[c], (int)(catRec.x + (catW - tw) * 0.5f), (int)catRec.y + 8, 11, isCatActive ? Color{ 255, 225, 100, 255 } : (isCatHov ? Color{ 240, 245, 255, 255 } : Color{ 160, 175, 195, 200 }));
        }

        float listY = contentY + 36.0f;

        // Subcategory 0: WEAPONS
        if (s_equipCategory == 0) {
            struct WeaponItem {
                WeaponType type;
                const char* label;
                const char* lockMsg;
            };
            WeaponItem weapons[4] = {
                { WeaponType::SWORD, "Crusader's Broadsword", "" },
                { WeaponType::DAGGERS, "Dual Daggers", "Craft at Blacksmith Varek's Forge using Ore Scraps" },
                { WeaponType::MACE, "War Mace", "Craft at Blacksmith Varek's Forge using Ore Scraps" },
                { WeaponType::TRUTH_SEEKER, "Truth Seeker (Partial)", "Beat Corrupted Zil in Ruined Castle" }
            };

            for (int i = 0; i < 4; i++) {
                float itemY = listY + (float)i * 110.0f;
                Rectangle itemRec = { col2X, itemY, col2W, 102.0f };
                bool isUnlocked = player.GetInventory().HasWeapon(weapons[i].type);
                bool isEquipped = (player.GetWeapon() == weapons[i].type);
                bool isHov = CheckCollisionPointRec(mousePos, itemRec);

                Color bgCol = isEquipped ? Color{ 32, 46, 70, 255 } : (isHov && isUnlocked ? Color{ 24, 34, 52, 240 } : Color{ 16, 20, 32, 200 });
                Color borderCol = isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? (isHov ? Color{ 100, 160, 230, 220 } : Color{ 50, 70, 100, 200 }) : Color{ 35, 42, 56, 160 });

                DrawRectangleRounded(itemRec, 0.1f, 4, bgCol);
                DrawRectangleRoundedLines(itemRec, 0.1f, 4, borderCol);

                int upgLvl = player.GetInventory().GetWeaponUpgrade(weapons[i].type);
                WeaponStats st = GetWeaponStats(weapons[i].type, upgLvl);
                DrawText(st.name.c_str(), (int)col2X + 14, (int)itemY + 10, 13, isUnlocked ? Color{ 255, 255, 255, 255 } : Color{ 120, 130, 145, 200 });

                if (isEquipped) {
                    DrawRectangleRounded(Rectangle{ col2X + col2W - 88.0f, itemY + 8.0f, 76.0f, 20.0f }, 0.25f, 4, Color{ 255, 215, 80, 230 });
                    DrawText("EQUIPPED", (int)(col2X + col2W - 78.0f), (int)itemY + 12, 10, Color{ 14, 18, 26, 255 });
                } else if (isUnlocked) {
                    DrawRectangleRounded(Rectangle{ col2X + col2W - 74.0f, itemY + 8.0f, 62.0f, 20.0f }, 0.25f, 4, isHov ? Color{ 60, 90, 130, 255 } : Color{ 30, 42, 62, 230 });
                    DrawText("EQUIP", (int)(col2X + col2W - 60.0f), (int)itemY + 12, 10, isHov ? Color{ 255, 255, 255, 255 } : Color{ 170, 195, 225, 220 });
                }

                DrawText(st.description.c_str(), (int)col2X + 14, (int)itemY + 30, 10, Color{ 175, 195, 220, 210 });

                if (isUnlocked) {
                    std::string dmgLine = "L-Atk: " + std::to_string((int)st.lightDamage) + " dmg  |  H-Atk: " + std::to_string((int)st.heavyDamage) + " dmg  |  Arc: " + std::to_string((int)st.lightArc) + " deg";
                    DrawText(dmgLine.c_str(), (int)col2X + 14, (int)itemY + 76, 10, Color{ 120, 225, 255, 230 });
                } else {
                    std::string lockStr = "Locked // " + std::string(weapons[i].lockMsg);
                    DrawText(lockStr.c_str(), (int)col2X + 14, (int)itemY + 76, 10, Color{ 220, 90, 90, 220 });
                }

                if (isUnlocked && !isEquipped && isHov && isMousePressed) {
                    player.SetWeapon(weapons[i].type);
                    SoundFX::PlayEquipWeapon();
                }
            }
        }
        // Subcategory 1: ARMORS
        else if (s_equipCategory == 1) {
            struct ArmorItem {
                ArmorType type;
                const char* lockMsg;
            };
            ArmorItem armors[4] = {
                { ArmorType::COMMONERS_SCRAP, "Ruined Castle - Beside broadsword pedestal" },
                { ArmorType::CRUSADER_PLATE, "Zyphden - Defeat Knight Lenon in spar" },
                { ArmorType::VANGUARD_MAIL, "Lobby / Arsenal testing supplies" },
                { ArmorType::SHADOW_LEATHER, "Lobby / Arsenal testing supplies" }
            };

            for (int i = 0; i < 4; i++) {
                float itemY = listY + (float)i * 110.0f;
                Rectangle itemRec = { col2X, itemY, col2W, 102.0f };
                bool isUnlocked = player.GetInventory().HasArmor(armors[i].type);
                bool isEquipped = (player.GetArmor() == armors[i].type);
                bool isHov = CheckCollisionPointRec(mousePos, itemRec);

                Color bgCol = isEquipped ? Color{ 32, 46, 70, 255 } : (isHov && isUnlocked ? Color{ 24, 34, 52, 240 } : Color{ 16, 20, 32, 200 });
                Color borderCol = isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? (isHov ? Color{ 100, 160, 230, 220 } : Color{ 50, 70, 100, 200 }) : Color{ 35, 42, 56, 160 });

                DrawRectangleRounded(itemRec, 0.1f, 4, bgCol);
                DrawRectangleRoundedLines(itemRec, 0.1f, 4, borderCol);

                ArmorStats st = GetArmorStats(armors[i].type);
                DrawText(st.name.c_str(), (int)col2X + 14, (int)itemY + 10, 13, isUnlocked ? Color{ 255, 255, 255, 255 } : Color{ 120, 130, 145, 200 });

                if (isEquipped) {
                    DrawRectangleRounded(Rectangle{ col2X + col2W - 88.0f, itemY + 8.0f, 76.0f, 20.0f }, 0.25f, 4, Color{ 255, 215, 80, 230 });
                    DrawText("EQUIPPED", (int)(col2X + col2W - 78.0f), (int)itemY + 12, 10, Color{ 14, 18, 26, 255 });
                } else if (isUnlocked) {
                    DrawRectangleRounded(Rectangle{ col2X + col2W - 74.0f, itemY + 8.0f, 62.0f, 20.0f }, 0.25f, 4, isHov ? Color{ 60, 90, 130, 255 } : Color{ 30, 42, 62, 230 });
                    DrawText("EQUIP", (int)(col2X + col2W - 60.0f), (int)itemY + 12, 10, isHov ? Color{ 255, 255, 255, 255 } : Color{ 170, 195, 225, 220 });
                }

                DrawText(st.description.c_str(), (int)col2X + 14, (int)itemY + 28, 10, Color{ 175, 195, 220, 210 });

                if (isUnlocked) {
                    std::string statLine = "Defense: +" + std::to_string((int)st.defense);
                    if (st.maxHpBonus > 0.0f) statLine += " | HP: +" + std::to_string((int)st.maxHpBonus);
                    if (st.speedModifier != 1.0f) {
                        int pct = (int)((st.speedModifier - 1.0f) * 100.0f);
                        statLine += " | Spd: " + (pct > 0 ? ("+" + std::to_string(pct)) : std::to_string(pct)) + "%";
                    }
                    if (st.staminaRegenModifier != 1.0f) {
                        int pct = (int)((st.staminaRegenModifier - 1.0f) * 100.0f);
                        statLine += " | Stam: " + (pct > 0 ? ("+" + std::to_string(pct)) : std::to_string(pct)) + "%";
                    }
                    DrawText(statLine.c_str(), (int)col2X + 14, (int)itemY + 76, 10, Color{ 100, 240, 200, 230 });
                } else {
                    std::string lockStr = "Locked // " + std::string(armors[i].lockMsg);
                    DrawText(lockStr.c_str(), (int)col2X + 14, (int)itemY + 76, 10, Color{ 220, 90, 90, 220 });
                }

                if (isUnlocked && !isEquipped && isHov && isMousePressed) {
                    player.SetArmor(armors[i].type);
                    SoundFX::PlayEquipWeapon();
                }
            }
        }
        // Subcategory 2: TOTEMS
        else if (s_equipCategory == 2) {
            struct TotemItem {
                TotemType type;
                const char* lockMsg;
            };
            TotemItem totems[5] = {
                { TotemType::TOTEM_SANITY, "Crossroads - Hidden ruined altar chest" },
                { TotemType::TOTEM_INSANITY, "Sanctum - Testing chamber offering" },
                { TotemType::TOTEM_MIST, "Mist Realm - Shrouded archive" },
                { TotemType::CRUSADER_TOTEM, "Zyphden - Crusader chapel shrine" },
                { TotemType::ELEMENTAL_CORE_TOTEM, "Quarry Depths - Defeat Elemental Golem" }
            };

            for (int i = 0; i < 5; i++) {
                float itemY = listY + (float)i * 92.0f;
                Rectangle itemRec = { col2X, itemY, col2W, 86.0f };
                bool isUnlocked = player.GetInventory().HasTotem(totems[i].type);
                bool isEquipped = (player.GetTotem() == totems[i].type);
                bool isHov = CheckCollisionPointRec(mousePos, itemRec);

                Color bgCol = isEquipped ? Color{ 32, 46, 70, 255 } : (isHov && isUnlocked ? Color{ 24, 34, 52, 240 } : Color{ 16, 20, 32, 200 });
                Color borderCol = isEquipped ? Color{ 255, 215, 80, 255 } : (isUnlocked ? (isHov ? Color{ 100, 160, 230, 220 } : Color{ 50, 70, 100, 200 }) : Color{ 35, 42, 56, 160 });

                DrawRectangleRounded(itemRec, 0.1f, 4, bgCol);
                DrawRectangleRoundedLines(itemRec, 0.1f, 4, borderCol);

                TotemStats st = GetTotemStats(totems[i].type);
                DrawText(st.name.c_str(), (int)col2X + 14, (int)itemY + 8, 13, isUnlocked ? st.glowColor : Color{ 120, 130, 145, 200 });

                if (isEquipped) {
                    DrawRectangleRounded(Rectangle{ col2X + col2W - 88.0f, itemY + 6.0f, 76.0f, 20.0f }, 0.25f, 4, Color{ 255, 215, 80, 230 });
                    DrawText("ATTUNED", (int)(col2X + col2W - 78.0f), (int)itemY + 10, 10, Color{ 14, 18, 26, 255 });
                } else if (isUnlocked) {
                    DrawRectangleRounded(Rectangle{ col2X + col2W - 74.0f, itemY + 6.0f, 62.0f, 20.0f }, 0.25f, 4, isHov ? Color{ 60, 90, 130, 255 } : Color{ 30, 42, 62, 230 });
                    DrawText("ATTUNE", (int)(col2X + col2W - 62.0f), (int)itemY + 10, 10, isHov ? Color{ 255, 255, 255, 255 } : Color{ 170, 195, 225, 220 });
                }

                DrawText(st.description.c_str(), (int)col2X + 14, (int)itemY + 26, 10, Color{ 175, 195, 220, 210 });

                if (isUnlocked) {
                    std::string buffStr = "[+] " + st.buffText;
                    DrawText(buffStr.c_str(), (int)col2X + 14, (int)itemY + 46, 10, Color{ 100, 245, 180, 240 });
                    std::string nerfStr = "[-] " + st.nerfText;
                    DrawText(nerfStr.c_str(), (int)col2X + 14, (int)itemY + 64, 10, (st.nerfText.find("None") != std::string::npos) ? Color{ 160, 180, 200, 200 } : Color{ 255, 95, 95, 240 });
                } else {
                    std::string lockStr = "Locked // " + std::string(totems[i].lockMsg);
                    DrawText(lockStr.c_str(), (int)col2X + 14, (int)itemY + 54, 10, Color{ 220, 90, 90, 220 });
                }

                if (isUnlocked && isHov && isMousePressed) {
                    if (isEquipped) {
                        player.SetTotem(TotemType::NONE);
                    } else {
                        player.SetTotem(totems[i].type);
                    }
                    SoundFX::PlayEquipWeapon();
                }
            }
        }

        // COLUMN 3: MARTIAL ATTRIBUTES, ACTIVE TOTEM, QUEST ITEMS
        float col3X = modalX + 736.0f;
        float col3W = modalW - 770.0f;

        DrawText("MARTIAL ATTRIBUTES", (int)col3X, (int)contentY, 13, Color{ 255, 220, 90, 255 });

        Rectangle statsBox = { col3X, contentY + 22.0f, col3W, 182.0f };
        DrawRectangleRounded(statsBox, 0.08f, 6, Color{ 18, 23, 34, 220 });
        DrawRectangleRoundedLines(statsBox, 0.08f, 6, Color{ 55, 75, 105, 230 });

        // Health
        DrawText("Maximum Vitality:", (int)col3X + 16, (int)contentY + 34, 11, Color{ 140, 210, 255, 240 });
        std::string hpStr = std::to_string((int)player.GetMaxHealth()) + " HP";
        if (curArmStats.maxHpBonus > 0.0f) {
            hpStr += " (+" + std::to_string((int)curArmStats.maxHpBonus) + " Armor)";
        }
        DrawText(hpStr.c_str(), (int)col3X + 140, (int)contentY + 34, 11, Color{ 255, 255, 255, 255 });

        // Defense
        float def = player.GetDefense();
        float mitPct = (def / (def + 100.0f)) * 100.0f;
        DrawText("Armor Defense:", (int)col3X + 16, (int)contentY + 54, 11, Color{ 140, 210, 255, 240 });
        std::string defStr = std::to_string((int)def) + " (" + std::to_string((int)mitPct) + "% Reduction)";
        DrawText(defStr.c_str(), (int)col3X + 140, (int)contentY + 54, 11, Color{ 100, 240, 200, 255 });

        // Stamina & Regen
        DrawText("Stamina & Regen:", (int)col3X + 16, (int)contentY + 74, 11, Color{ 140, 210, 255, 240 });
        std::string stamStr = std::to_string((int)player.GetMaxStamina()) + " SP / " + std::to_string((int)player.GetStaminaRegenRate()) + "s";
        DrawText(stamStr.c_str(), (int)col3X + 140, (int)contentY + 74, 11, Color{ 255, 255, 255, 255 });

        // Movement Speed
        DrawText("Movement Speed:", (int)col3X + 16, (int)contentY + 94, 11, Color{ 140, 210, 255, 240 });
        std::string spdStr = std::to_string((int)player.GetSpeed()) + " units/s";
        DrawText(spdStr.c_str(), (int)col3X + 140, (int)contentY + 94, 11, Color{ 240, 245, 255, 255 });

        // Attack Multiplier
        DrawText("Damage Output:", (int)col3X + 16, (int)contentY + 114, 11, Color{ 140, 210, 255, 240 });
        int dmgPct = (int)(curTotStats.damageMultiplier * 100.0f);
        std::string dmgStr = std::to_string(dmgPct) + "% Attack Power";
        DrawText(dmgStr.c_str(), (int)col3X + 140, (int)contentY + 114, 11, (dmgPct > 100) ? Color{ 100, 245, 180, 255 } : ((dmgPct < 100) ? Color{ 255, 100, 100, 255 } : Color{ 255, 255, 255, 255 }));

        // Parry & Dash windows
        DrawText("Parry / Dodge:", (int)col3X + 16, (int)contentY + 134, 11, Color{ 140, 210, 255, 240 });
        DrawText("0.28s Parry  |  0.18s Dash", (int)col3X + 140, (int)contentY + 134, 11, Color{ 240, 245, 255, 255 });

        // Counter buff
        DrawText("Parry Riposte:", (int)col3X + 16, (int)contentY + 154, 11, Color{ 140, 210, 255, 240 });
        DrawText("+50% Damage Riposte Buff", (int)col3X + 140, (int)contentY + 154, 11, Color{ 255, 220, 80, 255 });

        // Active Totem Resonance Box
        float totemBoxY = contentY + 214.0f;
        Rectangle totemBox = { col3X, totemBoxY, col3W, 110.0f };
        bool hasTotem = (player.GetTotem() != TotemType::NONE);
        DrawRectangleRounded(totemBox, 0.08f, 6, Color{ 18, 23, 34, 220 });
        DrawRectangleRoundedLines(totemBox, 0.08f, 6, hasTotem ? curTotStats.glowColor : Color{ 55, 75, 105, 230 });

        DrawText("ACTIVE TOTEM RESONANCE", (int)col3X + 16, (int)totemBoxY + 12, 12, Color{ 255, 220, 90, 255 });
        if (hasTotem) {
            DrawText(curTotStats.name.c_str(), (int)col3X + 16, (int)totemBoxY + 32, 13, curTotStats.glowColor);
            std::string tb = "[+] " + curTotStats.buffText;
            DrawText(tb.c_str(), (int)col3X + 16, (int)totemBoxY + 54, 10, Color{ 100, 245, 180, 240 });
            std::string tn = "[-] " + curTotStats.nerfText;
            DrawText(tn.c_str(), (int)col3X + 16, (int)totemBoxY + 72, 10, (curTotStats.nerfText.find("None") != std::string::npos) ? Color{ 160, 180, 200, 200 } : Color{ 255, 95, 95, 240 });
        } else {
            DrawText("No totem attuned.", (int)col3X + 16, (int)totemBoxY + 36, 12, Color{ 160, 175, 195, 200 });
            DrawText("Attune a sacred relic to empower your", (int)col3X + 16, (int)totemBoxY + 58, 10, Color{ 140, 155, 175, 180 });
            DrawText("strikes with primordial gifts & vows.", (int)col3X + 16, (int)totemBoxY + 74, 10, Color{ 140, 155, 175, 180 });
        }

        // Relics & Quest Items Box
        float relicsY = contentY + 334.0f;
        Rectangle relicsBox = { col3X, relicsY, col3W, 140.0f };
        DrawRectangleRounded(relicsBox, 0.08f, 6, Color{ 18, 23, 34, 220 });
        DrawRectangleRoundedLines(relicsBox, 0.08f, 6, Color{ 55, 75, 105, 230 });

        DrawText("RELICS & QUEST ITEMS", (int)col3X + 16, (int)relicsY + 12, 12, Color{ 255, 220, 90, 255 });
        DrawLine((int)col3X + 16, (int)relicsY + 28, (int)(col3X + col3W - 16), (int)relicsY + 28, Color{ 40, 52, 75, 255 });

        const auto& qItems = player.GetInventory().GetQuestItems();
        if (qItems.empty()) {
            DrawText("No quest artifacts collected yet.", (int)col3X + 16, (int)relicsY + 44, 11, Color{ 150, 165, 185, 200 });
            DrawText("Explore Tydeshire, defeat corrupted lieges,", (int)col3X + 16, (int)relicsY + 66, 10, Color{ 130, 145, 165, 180 });
            DrawText("and speak with royal allies to gather relics.", (int)col3X + 16, (int)relicsY + 82, 10, Color{ 130, 145, 165, 180 });
        } else {
            for (size_t i = 0; i < qItems.size() && i < 2; i++) {
                float qy = relicsY + 38.0f + (float)i * 48.0f;
                DrawRectangleRounded(Rectangle{ col3X + 12, qy, col3W - 24, 42.0f }, 0.15f, 4, Color{ 24, 30, 44, 180 });
                DrawText(qItems[i].name.c_str(), (int)col3X + 20, (int)qy + 6, 11, qItems[i].rarityColor);
                DrawText(qItems[i].description.c_str(), (int)col3X + 20, (int)qy + 22, 9, Color{ 180, 200, 220, 220 });
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
    // 1. Dark Vignette Backdrop
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 10, 14, 230 });

    const float modalW = 1140.0f;
    const float modalH = 640.0f;
    const float modalX = ((float)windowWidth - modalW) * 0.5f;
    const float modalY = ((float)windowHeight - modalH) * 0.5f;
    Rectangle modalRec = { modalX, modalY, modalW, modalH };

    // Outer backing card & filigree borders
    DrawRectangleRounded(modalRec, 0.03f, 8, Color{ 14, 16, 24, 252 });
    DrawRectangleRoundedLines(modalRec, 0.03f, 8, Color{ 215, 175, 65, 245 });
    DrawRectangleRoundedLines(Rectangle{ modalX + 4.0f, modalY + 4.0f, modalW - 8.0f, modalH - 8.0f }, 0.025f, 8, Color{ 55, 70, 95, 180 });

    // Corner Filigree Ornaments
    Color cornerCol = Color{ 255, 215, 80, 240 };
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 38, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + 12 }, { modalX + 12, modalY + 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 38, modalY + 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + 12 }, { modalX + modalW - 12, modalY + 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 38, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + 12, modalY + modalH - 12 }, { modalX + 12, modalY + modalH - 38 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 38, modalY + modalH - 12 }, 2.0f, cornerCol);
    DrawLineEx({ modalX + modalW - 12, modalY + modalH - 12 }, { modalX + modalW - 12, modalY + modalH - 38 }, 2.0f, cornerCol);

    // 2. Header Section
    FontManager::DrawText("REALM MAP OF TYDESHIRE & ZYPHDEN", (int)modalX + 32, (int)modalY + 16, 20, Color{ 255, 225, 95, 255 }, GameFont::TITLE);
    FontManager::DrawText("Imperial Cartography Guild Survey | Northern Marchlands & Frontier Territories", (int)modalX + 32, (int)modalY + 40, 11, Color{ 175, 195, 225, 220 }, GameFont::BODY);

    // Interactive Legend & Filter Pills (0 = ALL, 1 = QUESTS, 2 = SERVICES, 3 = DANGER)
    static int s_activeFilter = 0;
    const char* filterLabels[4] = { "ALL [0]", "QUESTS [1]", "SERVICES [2]", "DANGER [3]" };
    Color filterColors[4] = { Color{ 220, 235, 255, 255 }, Color{ 255, 215, 80, 255 }, Color{ 80, 210, 255, 255 }, Color{ 255, 100, 90, 255 } };

    float filterStartX = modalX + 540.0f;
    float filterY = modalY + 20.0f;
    for (int i = 0; i < 4; i++) {
        float fW = 95.0f;
        float fH = 26.0f;
        float fX = filterStartX + (float)i * 102.0f;
        Rectangle fRec = { fX, filterY, fW, fH };
        bool isFHov = CheckCollisionPointRec(mousePos, fRec);
        if ((isFHov && isMousePressed) || (IsKeyPressed(KEY_ZERO + i))) {
            s_activeFilter = i;
            SoundFX::PlayButtonClick();
        }

        bool isActive = (s_activeFilter == i);
        DrawRectangleRounded(fRec, 0.3f, 4, isActive ? Color{ 36, 48, 70, 240 } : (isFHov ? Color{ 25, 32, 48, 220 } : Color{ 18, 22, 34, 180 }));
        DrawRectangleRoundedLines(fRec, 0.3f, 4, isActive ? filterColors[i] : (isFHov ? Color{ 140, 180, 220, 200 } : Color{ 55, 70, 95, 160 }));
        FontManager::DrawText(filterLabels[i], (int)fX + 12, (int)filterY + 6, 10, isActive ? filterColors[i] : Color{ 170, 185, 205, 220 }, GameFont::BODY);
    }

    // Close Button [ESC / M]
    const float closeW = 120.0f;
    const float closeH = 30.0f;
    float closeX = modalX + modalW - closeW - 32.0f;
    float closeY = modalY + 18.0f;
    Rectangle closeRec = { closeX, closeY, closeW, closeH };
    bool isCloseHov = CheckCollisionPointRec(mousePos, closeRec);

    DrawRectangleRounded(closeRec, 0.25f, 4, isCloseHov ? Color{ 55, 75, 110, 255 } : Color{ 28, 36, 52, 230 });
    DrawRectangleRoundedLines(closeRec, 0.25f, 4, isCloseHov ? Color{ 120, 220, 255, 255 } : Color{ 70, 90, 125, 230 });
    FontManager::DrawText("CLOSE [ESC/M]", (int)closeX + 14, (int)closeY + 8, 11, isCloseHov ? Color{ 255, 255, 255, 255 } : Color{ 190, 210, 235, 220 }, GameFont::BODY);

    bool shouldClose = (isCloseHov && isMousePressed) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M);

    // 3. Map Viewport & Pan / Zoom Engine
    const float viewX = modalX + 24.0f;
    const float viewY = modalY + 68.0f;
    const float viewW = modalW - 48.0f;
    const float viewH = modalH - 126.0f;
    Rectangle viewRec = { viewX, viewY, viewW, viewH };

    static Vector2 s_mapPan = { 0.0f, 0.0f };
    static float s_mapZoom = 1.0f;
    static bool s_isDragging = false;
    static Vector2 s_dragStart = { 0.0f, 0.0f };
    static Vector2 s_panStart = { 0.0f, 0.0f };

    bool inView = CheckCollisionPointRec(mousePos, viewRec);

    // Mouse wheel zooming
    if (inView) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            s_mapZoom = fmaxf(0.75f, fminf(2.2f, s_mapZoom + wheel * 0.12f));
        }
    }

    // Mouse dragging
    if (inView && isMousePressed) {
        s_isDragging = true;
        s_dragStart = mousePos;
        s_panStart = s_mapPan;
    }

    if (s_isDragging) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            s_mapPan.x = s_panStart.x + (mousePos.x - s_dragStart.x);
            s_mapPan.y = s_panStart.y + (mousePos.y - s_dragStart.y);
        } else {
            s_isDragging = false;
        }
    }

    // Clamp panning
    s_mapPan.x = fmaxf(-450.0f * s_mapZoom, fminf(450.0f * s_mapZoom, s_mapPan.x));
    s_mapPan.y = fmaxf(-320.0f * s_mapZoom, fminf(320.0f * s_mapZoom, s_mapPan.y));

    // Recenter keyboard shortcut
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_R)) {
        s_mapPan = { 0.0f, 0.0f };
        s_mapZoom = 1.0f;
    }

    // 4. Scissor Mode for Cartographic Map Canvas
    BeginScissorMode((int)viewX, (int)viewY, (int)viewW, (int)viewH);

    // Dark fantasy aged parchment paper canvas
    Vector2 mapCenter = { viewX + viewW * 0.5f + s_mapPan.x, viewY + viewH * 0.5f + s_mapPan.y };
    float paperW = 1120.0f * s_mapZoom;
    float paperH = 660.0f * s_mapZoom;
    Rectangle paperRec = { mapCenter.x - paperW * 0.5f, mapCenter.y - paperH * 0.5f, paperW, paperH };

    // Aged parchment base fill
    DrawRectangleRec(paperRec, Color{ 24, 21, 20, 255 });
    DrawRectangleLinesEx(paperRec, 2.0f, Color{ 75, 62, 48, 200 });
    DrawRectangleLinesEx(Rectangle{ paperRec.x + 4.0f, paperRec.y + 4.0f, paperRec.width - 8.0f, paperRec.height - 8.0f }, 1.0f, Color{ 55, 44, 34, 160 });

    // Latitude & Longitude Coordinate Lines
    Color gridCol = Color{ 48, 40, 34, 120 };
    for (int gx = -400; gx <= 400; gx += 200) {
        float lineX = mapCenter.x + (float)gx * s_mapZoom;
        DrawLineEx({ lineX, paperRec.y }, { lineX, paperRec.y + paperRec.height }, 1.0f, gridCol);
    }
    for (int gy = -200; gy <= 200; gy += 150) {
        float lineY = mapCenter.y + (float)gy * s_mapZoom;
        DrawLineEx({ paperRec.x, lineY }, { paperRec.x + paperRec.width, lineY }, 1.0f, gridCol);
    }

    // A. Whispering Sea / Western Coastline
    float coastX = mapCenter.x - 380.0f * s_mapZoom;
    DrawRectangleRec(Rectangle{ paperRec.x, paperRec.y, coastX - paperRec.x, paperRec.height }, Color{ 18, 24, 32, 230 });
    DrawLineEx({ coastX, paperRec.y }, { coastX, paperRec.y + paperRec.height }, 2.0f, Color{ 45, 65, 85, 200 });
    // Coastline waves
    for (float wy = paperRec.y + 40.0f; wy < paperRec.y + paperRec.height - 40.0f; wy += 60.0f * s_mapZoom) {
        DrawText("~ ~ ~", (int)(coastX - 60.0f * s_mapZoom), (int)wy, (int)(11 * s_mapZoom), Color{ 50, 75, 105, 140 });
    }
    DrawText("THE WHISPERING SEA", (int)(paperRec.x + 20.0f), (int)(mapCenter.y - 120.0f * s_mapZoom), (int)(12 * s_mapZoom), Color{ 65, 95, 130, 160 });

    // B. The River Tydes
    Vector2 riverPoints[7] = {
        { mapCenter.x + 40.0f * s_mapZoom, mapCenter.y - 300.0f * s_mapZoom },
        { mapCenter.x - 10.0f * s_mapZoom, mapCenter.y - 180.0f * s_mapZoom },
        { mapCenter.x - 45.0f * s_mapZoom, mapCenter.y - 80.0f * s_mapZoom },
        { mapCenter.x + 20.0f * s_mapZoom, mapCenter.y + 10.0f * s_mapZoom },
        { mapCenter.x - 20.0f * s_mapZoom, mapCenter.y + 110.0f * s_mapZoom },
        { mapCenter.x - 60.0f * s_mapZoom, mapCenter.y + 200.0f * s_mapZoom },
        { coastX, mapCenter.y + 260.0f * s_mapZoom }
    };
    for (int r = 0; r < 6; r++) {
        DrawLineEx(riverPoints[r], riverPoints[r + 1], 4.5f * s_mapZoom, Color{ 36, 56, 80, 240 });
        DrawLineEx(riverPoints[r], riverPoints[r + 1], 1.5f * s_mapZoom, Color{ 75, 130, 185, 220 });
    }

    // C. The Royal Highway (Cobblestone Highway)
    Vector2 roadPoints[6] = {
        { mapCenter.x - 40.0f * s_mapZoom, mapCenter.y + 170.0f * s_mapZoom }, // Ruined Castle (South)
        { mapCenter.x - 10.0f * s_mapZoom, mapCenter.y + 65.0f * s_mapZoom },  // Crossroads
        { mapCenter.x - 15.0f * s_mapZoom, mapCenter.y - 45.0f * s_mapZoom },  // Zyphden Gates
        { mapCenter.x + 20.0f * s_mapZoom, mapCenter.y - 80.0f * s_mapZoom },  // Courtyard
        { mapCenter.x + 5.0f * s_mapZoom, mapCenter.y - 145.0f * s_mapZoom },  // Royal Keep
        { mapCenter.x + 15.0f * s_mapZoom, mapCenter.y - 220.0f * s_mapZoom }  // North Gate
    };
    for (int r = 0; r < 5; r++) {
        DrawLineEx(roadPoints[r], roadPoints[r + 1], 3.5f * s_mapZoom, Color{ 75, 62, 45, 230 });
        DrawLineEx(roadPoints[r], roadPoints[r + 1], 1.2f * s_mapZoom, Color{ 150, 125, 85, 200 });
    }
    // Road spur eastward to Quarry
    Vector2 quarrySpurStart = roadPoints[3];
    Vector2 quarrySpurEnd = { mapCenter.x + 230.0f * s_mapZoom, mapCenter.y - 40.0f * s_mapZoom };
    DrawLineEx(quarrySpurStart, quarrySpurEnd, 3.0f * s_mapZoom, Color{ 75, 62, 45, 230 });
    DrawLineEx(quarrySpurStart, quarrySpurEnd, 1.2f * s_mapZoom, Color{ 150, 125, 85, 200 });

    // D. Northern Dragonspire Crags (Mountain Peaks)
    for (float mx = mapCenter.x - 260.0f * s_mapZoom; mx <= mapCenter.x + 360.0f * s_mapZoom; mx += 65.0f * s_mapZoom) {
        float peakY = mapCenter.y - 280.0f * s_mapZoom;
        float peakH = 34.0f * s_mapZoom;
        DrawTriangle(
            Vector2{ mx, peakY - peakH },
            Vector2{ mx - 32.0f * s_mapZoom, peakY },
            Vector2{ mx + 32.0f * s_mapZoom, peakY },
            Color{ 45, 38, 32, 240 }
        );
        DrawLineEx({ mx, peakY - peakH }, { mx + 32.0f * s_mapZoom, peakY }, 1.5f * s_mapZoom, Color{ 100, 85, 70, 220 });
    }
    DrawText("DRAGONSPIRE CRAGS", (int)(mapCenter.x - 80.0f * s_mapZoom), (int)(mapCenter.y - 295.0f * s_mapZoom), (int)(11 * s_mapZoom), Color{ 120, 100, 80, 200 });

    // E. Eastern Quarry Plateau (Basalt Crags)
    Rectangle quarryPlateau = { mapCenter.x + 160.0f * s_mapZoom, mapCenter.y - 80.0f * s_mapZoom, 150.0f * s_mapZoom, 90.0f * s_mapZoom };
    DrawRectangleRounded(quarryPlateau, 0.2f, 4, Color{ 36, 30, 26, 230 });
    DrawRectangleRoundedLines(quarryPlateau, 0.2f, 4, Color{ 110, 85, 55, 200 });
    // Amber mineral vein sparkles
    DrawCircle((int)(quarryPlateau.x + 30.0f * s_mapZoom), (int)(quarryPlateau.y + 25.0f * s_mapZoom), 2.5f * s_mapZoom, Color{ 255, 195, 60, 220 });
    DrawCircle((int)(quarryPlateau.x + 85.0f * s_mapZoom), (int)(quarryPlateau.y + 55.0f * s_mapZoom), 3.0f * s_mapZoom, Color{ 255, 195, 60, 220 });
    DrawCircle((int)(quarryPlateau.x + 120.0f * s_mapZoom), (int)(quarryPlateau.y + 35.0f * s_mapZoom), 2.5f * s_mapZoom, Color{ 255, 195, 60, 220 });

    // F. City Walls of Zyphden
    Rectangle cityWalls = { mapCenter.x - 170.0f * s_mapZoom, mapCenter.y - 170.0f * s_mapZoom, 320.0f * s_mapZoom, 155.0f * s_mapZoom };
    DrawRectangleRounded(cityWalls, 0.12f, 6, Color{ 32, 28, 34, 210 });
    DrawRectangleRoundedLines(cityWalls, 0.12f, 6, Color{ 90, 80, 105, 220 });
    // Corner Turrets
    DrawCircle((int)cityWalls.x, (int)cityWalls.y, 7.0f * s_mapZoom, Color{ 60, 52, 70, 240 });
    DrawCircle((int)(cityWalls.x + cityWalls.width), (int)cityWalls.y, 7.0f * s_mapZoom, Color{ 60, 52, 70, 240 });
    DrawCircle((int)cityWalls.x, (int)(cityWalls.y + cityWalls.height), 7.0f * s_mapZoom, Color{ 60, 52, 70, 240 });
    DrawCircle((int)(cityWalls.x + cityWalls.width), (int)(cityWalls.y + cityWalls.height), 7.0f * s_mapZoom, Color{ 60, 52, 70, 240 });

    // G. The Whispering Westwoods (Tree Clusters)
    for (float tx = mapCenter.x - 300.0f * s_mapZoom; tx <= mapCenter.x - 180.0f * s_mapZoom; tx += 40.0f * s_mapZoom) {
        for (float ty = mapCenter.y + 40.0f * s_mapZoom; ty <= mapCenter.y + 150.0f * s_mapZoom; ty += 45.0f * s_mapZoom) {
            DrawCircle((int)tx, (int)ty, 10.0f * s_mapZoom, Color{ 28, 42, 32, 220 });
            DrawCircle((int)tx, (int)ty, 7.0f * s_mapZoom, Color{ 36, 56, 40, 240 });
        }
    }
    DrawText("THE WESTWOODS", (int)(mapCenter.x - 290.0f * s_mapZoom), (int)(mapCenter.y + 165.0f * s_mapZoom), (int)(11 * s_mapZoom), Color{ 80, 120, 90, 200 });

    // H. Ornate Compass Rose (Top Right of parchment)
    Vector2 compassCenter = { paperRec.x + paperRec.width - 80.0f * s_mapZoom, paperRec.y + 80.0f * s_mapZoom };
    DrawCircleLines((int)compassCenter.x, (int)compassCenter.y, 28.0f * s_mapZoom, Color{ 140, 115, 65, 200 });
    DrawCircleLines((int)compassCenter.x, (int)compassCenter.y, 16.0f * s_mapZoom, Color{ 90, 75, 45, 180 });
    // North pointer (Gold)
    DrawTriangle(
        Vector2{ compassCenter.x, compassCenter.y - 34.0f * s_mapZoom },
        Vector2{ compassCenter.x - 6.0f * s_mapZoom, compassCenter.y },
        Vector2{ compassCenter.x + 6.0f * s_mapZoom, compassCenter.y },
        Color{ 255, 215, 80, 255 }
    );
    // South pointer
    DrawTriangle(
        Vector2{ compassCenter.x, compassCenter.y + 34.0f * s_mapZoom },
        Vector2{ compassCenter.x + 6.0f * s_mapZoom, compassCenter.y },
        Vector2{ compassCenter.x - 6.0f * s_mapZoom, compassCenter.y },
        Color{ 100, 85, 60, 255 }
    );
    // East / West pointers
    DrawLineEx({ compassCenter.x - 30.0f * s_mapZoom, compassCenter.y }, { compassCenter.x + 30.0f * s_mapZoom, compassCenter.y }, 2.0f * s_mapZoom, Color{ 160, 130, 70, 240 });
    DrawText("N", (int)(compassCenter.x - 4.0f * s_mapZoom), (int)(compassCenter.y - 48.0f * s_mapZoom), (int)(12 * s_mapZoom), Color{ 255, 220, 95, 255 });
    DrawText("S", (int)(compassCenter.x - 4.0f * s_mapZoom), (int)(compassCenter.y + 38.0f * s_mapZoom), (int)(10 * s_mapZoom), Color{ 180, 150, 95, 220 });
    DrawText("E", (int)(compassCenter.x + 34.0f * s_mapZoom), (int)(compassCenter.y - 5.0f * s_mapZoom), (int)(10 * s_mapZoom), Color{ 180, 150, 95, 220 });
    DrawText("W", (int)(compassCenter.x - 45.0f * s_mapZoom), (int)(compassCenter.y - 5.0f * s_mapZoom), (int)(10 * s_mapZoom), Color{ 180, 150, 95, 220 });

    // I. Imperial Guild Seal (Bottom Left)
    Vector2 sealCenter = { paperRec.x + 80.0f * s_mapZoom, paperRec.y + paperRec.height - 70.0f * s_mapZoom };
    DrawCircle((int)sealCenter.x, (int)sealCenter.y, 24.0f * s_mapZoom, Color{ 85, 30, 30, 220 });
    DrawCircleLines((int)sealCenter.x, (int)sealCenter.y, 24.0f * s_mapZoom, Color{ 180, 70, 70, 240 });
    DrawPolyLines(sealCenter, 6, 16.0f * s_mapZoom, 30.0f, Color{ 220, 150, 100, 200 });
    DrawText("SIGIL", (int)(sealCenter.x - 12.0f * s_mapZoom), (int)(sealCenter.y - 5.0f * s_mapZoom), (int)(9 * s_mapZoom), Color{ 255, 200, 160, 220 });

    // 5. Points of Interest (POIs) Definition & Rendering
    struct RealmPOI {
        Vector2 offset; // Offset from mapCenter
        const char* name;
        const char* subTag;
        const char* resident;
        const char* desc;
        const char* status;
        int category; // 1 = QUEST, 2 = SERVICE, 3 = DANGER
        Color color;
    };

    RealmPOI pois[8] = {
        {
            { -40.0f, 170.0f },
            "Ruined Castle of Zil",
            "[REMEMBRANCE SHRINE]",
            "Corrupted Zil Remnant",
            "The desecrated sun courtyard where Champion Zil was defeated. Ancient solar altar hums in silence.",
            "Sanctified & Cleared",
            3,
            Color{ 255, 100, 90, 255 }
        },
        {
            { -10.0f, 65.0f },
            "Crossroads Sanctum",
            "[HIGHWAY WAYPOINT]",
            "Wayfarer's Ancient Stele",
            "The historic valley junction between southern ruins, western forests, and northern kingdom gates.",
            "Safe Highway Passage",
            2,
            Color{ 80, 210, 255, 255 }
        },
        {
            { -95.0f, -40.0f },
            "Hearthside Tavern",
            "[REST & PROVISIONS]",
            "Tavernkeep Eldon",
            "A warm gathering hall with crackling hearth, hearty braised stews, and frontier rumor whispers.",
            "Safe Tavern Sanctuary",
            2,
            Color{ 80, 210, 255, 255 }
        },
        {
            { -140.0f, -95.0f },
            "Varek's Master Forge",
            "[WEAPON CRAFTING]",
            "Blacksmith Varek",
            "Forging workshop. Craft Dual Daggers & War Maces with Quarry Ore Scraps; temper steel with Shingles.",
            "Crafting & Upgrades Active",
            2,
            Color{ 255, 185, 60, 255 }
        },
        {
            { 5.0f, -145.0f },
            "The Royal Keep & Throne",
            "[SOVEREIGN SEAT]",
            "King Kreptu & Royal Marshals",
            "Throne dais of King Kreptu. Grants audience to worthy duelists and sanctions exploration beyond borders.",
            "Main Quest: Royal Audience",
            1,
            Color{ 255, 215, 80, 255 }
        },
        {
            { 85.0f, -60.0f },
            "Courtyard Sparring Ring",
            "[MARTIAL DUEL]",
            "Knight-Captain Lenon",
            "The royal practice grounds where the Vanguard trains in poise-breaking counters and evasive maneuvers.",
            "Martial Sparring Ground",
            1,
            Color{ 255, 215, 80, 255 }
        },
        {
            { 230.0f, -40.0f },
            "Eastern Ore Quarry",
            "[RESOURCE HARVESTING]",
            "Ore Golems (7s Respawn)",
            "Gale-carved rocky plateau rich in metallic veins. Ore Golems patrol and drop vital crafting scraps.",
            "Scrap Harvesting Ground (Active)",
            3,
            Color{ 255, 120, 60, 255 }
        },
        {
            { 15.0f, -220.0f },
            "Northern Frontier Gate",
            "[NEXT REALM BORDER]",
            "Royal Frontier Sentinels",
            "Massive portcullis leading north to neighboring kingdoms. King Kreptu has granted open travel sanction.",
            "Open Exploration Sanctioned",
            1,
            Color{ 255, 225, 100, 255 }
        }
    };

    int hoveredPOIIndex = -1;

    for (int i = 0; i < 8; i++) {
        Vector2 poiScreenPos = { mapCenter.x + pois[i].offset.x * s_mapZoom, mapCenter.y + pois[i].offset.y * s_mapZoom };

        bool filterMatch = (s_activeFilter == 0 || pois[i].category == s_activeFilter);
        float alpha = filterMatch ? 1.0f : 0.25f;

        // Hover test
        float hitRadius = 18.0f * s_mapZoom;
        if (inView && filterMatch && CheckCollisionPointCircle(mousePos, poiScreenPos, hitRadius)) {
            hoveredPOIIndex = i;
        }

        bool isHov = (hoveredPOIIndex == i);

        // Outer base pin circle
        Color baseCol = ColorAlpha(pois[i].color, alpha);
        Color rimCol = isHov ? Color{ 255, 255, 255, (unsigned char)(255 * alpha) } : ColorAlpha(Color{ 18, 22, 34, 240 }, alpha);

        DrawCircle((int)poiScreenPos.x, (int)poiScreenPos.y, (12.0f + (isHov ? 3.0f : 0.0f)) * s_mapZoom, rimCol);
        DrawCircle((int)poiScreenPos.x, (int)poiScreenPos.y, 8.5f * s_mapZoom, baseCol);
        DrawCircleLines((int)poiScreenPos.x, (int)poiScreenPos.y, (12.0f + (isHov ? 3.0f : 0.0f)) * s_mapZoom, baseCol);

        // Category Icon inside pin
        if (pois[i].category == 1) {
            // Crown / Shield icon (Quest)
            DrawPolyLines(poiScreenPos, 3, 5.0f * s_mapZoom, 180.0f, ColorAlpha(WHITE, alpha));
        } else if (pois[i].category == 2) {
            // Star / Diamond icon (Service)
            DrawPoly(poiScreenPos, 4, 4.5f * s_mapZoom, 45.0f, ColorAlpha(WHITE, alpha));
        } else {
            // Crossed swords / Skull icon (Danger)
            DrawLineEx({ poiScreenPos.x - 4 * s_mapZoom, poiScreenPos.y - 4 * s_mapZoom }, { poiScreenPos.x + 4 * s_mapZoom, poiScreenPos.y + 4 * s_mapZoom }, 1.5f * s_mapZoom, ColorAlpha(WHITE, alpha));
            DrawLineEx({ poiScreenPos.x - 4 * s_mapZoom, poiScreenPos.y + 4 * s_mapZoom }, { poiScreenPos.x + 4 * s_mapZoom, poiScreenPos.y - 4 * s_mapZoom }, 1.5f * s_mapZoom, ColorAlpha(WHITE, alpha));
        }

        // POI Label below pin
        int lblW = MeasureText(pois[i].name, (int)(10 * s_mapZoom));
        Color txtCol = isHov ? Color{ 255, 255, 255, (unsigned char)(255 * alpha) } : ColorAlpha(Color{ 215, 225, 240, 220 }, alpha);
        FontManager::DrawGameTextShadow(pois[i].name, (int)(poiScreenPos.x - lblW * 0.5f), (int)(poiScreenPos.y + 14.0f * s_mapZoom), (int)(10 * s_mapZoom), txtCol);
    }

    // 6. Live Player Beacon
    Vector2 playerOffset = { 0.0f, 0.0f };
    std::string locName = "Zyphden Outskirts";

    if (currentState == GameState::ZYPHDEN_REALM) {
        Vector2 pPos = player.GetPosition();
        if (pPos.x > 1550.0f) {
            // In Eastern Quarry
            float dx = (pPos.x - 1800.0f) * 0.12f;
            float dy = (pPos.y - 700.0f) * 0.12f;
            playerOffset = { 230.0f + dx, -40.0f + dy };
            locName = "Eastern Ore Quarry Plateau";
        } else if (pPos.y < 500.0f && pPos.x > 850.0f) {
            playerOffset = { 5.0f, -145.0f };
            locName = "The Royal Keep & Throne Dais";
        } else if (pPos.x < 700.0f && pPos.y > 850.0f) {
            playerOffset = { -95.0f, -40.0f };
            locName = "Hearthside Tavern District";
        } else if (pPos.x < 700.0f && pPos.y < 750.0f) {
            playerOffset = { -140.0f, -95.0f };
            locName = "Blacksmith Varek's Master Forge";
        } else if (pPos.x > 1100.0f && pPos.y > 600.0f && pPos.y < 900.0f) {
            playerOffset = { 85.0f, -60.0f };
            locName = "Courtyard Sparring Grounds";
        } else {
            // General Zyphden Courtyard
            float nx = (pPos.x - 1000.0f) * 0.10f;
            float ny = (pPos.y - 800.0f) * 0.10f;
            playerOffset = { nx, -70.0f + ny };
            locName = "Kingdom of Zyphden - Grand Plaza";
        }
    } else if (currentState == GameState::QUARRY_DEPTHS) {
        playerOffset = { 240.0f, -40.0f };
        locName = "Quarry Mines & Primordial Altar";
    } else if (currentState == GameState::CROSSROADS) {
        playerOffset = { -10.0f, 65.0f };
        locName = "Crossroads Highway & Sanctum";
    } else if (currentState == GameState::RUINED_CASTLE) {
        playerOffset = { -40.0f, 170.0f };
        locName = "Ruined Castle of Zil (South)";
    } else if (currentState == GameState::LOBBY) {
        playerOffset = { -10.0f, 65.0f };
        locName = "Armory Sanctuary (Nexus)";
    }

    Vector2 playerScreenPos = { mapCenter.x + playerOffset.x * s_mapZoom, mapCenter.y + playerOffset.y * s_mapZoom };

    // Pulsing Radar Rings
    float pTime = (float)GetTime();
    float waveProgress = fmodf(pTime * 1.6f, 1.0f);
    float waveRadius = (8.0f + waveProgress * 24.0f) * s_mapZoom;
    unsigned char waveAlpha = (unsigned char)(220 * (1.0f - waveProgress));
    DrawCircleLines((int)playerScreenPos.x, (int)playerScreenPos.y, waveRadius, Color{ 0, 229, 255, waveAlpha });
    DrawCircleLines((int)playerScreenPos.x, (int)playerScreenPos.y, waveRadius * 0.6f, Color{ 255, 215, 80, (unsigned char)(waveAlpha * 0.8f) });

    // Central Diamond Beacon
    DrawPoly(playerScreenPos, 4, 8.0f * s_mapZoom, 45.0f, Color{ 20, 40, 65, 255 });
    DrawPolyLines(playerScreenPos, 4, 8.0f * s_mapZoom, 45.0f, Color{ 0, 229, 255, 255 });
    DrawPoly(playerScreenPos, 4, 5.0f * s_mapZoom, 45.0f, Color{ 255, 220, 90, 255 });
    DrawCircle((int)playerScreenPos.x, (int)playerScreenPos.y, 2.5f * s_mapZoom, WHITE);

    // "YOU ARE HERE" Banner
    int youW = MeasureText("YOU ARE HERE", (int)(10 * s_mapZoom));
    FontManager::DrawGameTextShadow("YOU ARE HERE", (int)(playerScreenPos.x - youW * 0.5f), (int)(playerScreenPos.y - 18.0f * s_mapZoom), (int)(10 * s_mapZoom), Color{ 140, 240, 255, 255 });

    // 7. Interactive Hover Tooltip Card
    if (hoveredPOIIndex >= 0 && hoveredPOIIndex < 8) {
        const RealmPOI& hp = pois[hoveredPOIIndex];
        Vector2 poiScreenPos = { mapCenter.x + hp.offset.x * s_mapZoom, mapCenter.y + hp.offset.y * s_mapZoom };

        float ttW = 320.0f;
        float ttH = 148.0f;
        float ttX = poiScreenPos.x + 18.0f;
        float ttY = poiScreenPos.y - ttH * 0.5f;

        // Clamp tooltip within viewport
        if (ttX + ttW > viewX + viewW - 12.0f) ttX = poiScreenPos.x - ttW - 18.0f;
        if (ttY < viewY + 12.0f) ttY = viewY + 12.0f;
        if (ttY + ttH > viewY + viewH - 12.0f) ttY = viewY + viewH - ttH - 12.0f;

        Rectangle ttRec = { ttX, ttY, ttW, ttH };
        DrawRectangleRounded(ttRec, 0.12f, 6, Color{ 14, 18, 28, 248 });
        DrawRectangleRoundedLines(ttRec, 0.12f, 6, hp.color);
        DrawRectangleLinesEx(Rectangle{ ttX + 3.0f, ttY + 3.0f, ttW - 6.0f, ttH - 6.0f }, 1.0f, Color{ 55, 75, 105, 160 });

        // Category Tag
        DrawText(hp.subTag, (int)ttX + 16, (int)ttY + 12, 10, hp.color);

        // POI Title
        FontManager::DrawText(hp.name, (int)ttX + 16, (int)ttY + 26, 14, Color{ 255, 255, 255, 255 }, GameFont::TITLE);

        // Resident NPC Line
        std::string resStr = "NPC: " + std::string(hp.resident);
        DrawText(resStr.c_str(), (int)ttX + 16, (int)ttY + 48, 11, Color{ 140, 215, 255, 240 });

        // Lore Description
        DrawJournalWrappedText(hp.desc, (int)ttX + 16, (int)ttY + 66, (int)ttW - 32, 10, 4, Color{ 205, 220, 240, 225 });

        // Status Line
        DrawLine((int)ttX + 14, (int)ttY + ttH - 26, (int)(ttX + ttW - 14), (int)ttY + ttH - 26, Color{ 55, 70, 95, 160 });
        std::string statStr = "STATUS: " + std::string(hp.status);
        DrawText(statStr.c_str(), (int)ttX + 16, (int)ttY + ttH - 20, 10, Color{ 120, 255, 170, 230 });
    }

    EndScissorMode();

    // 8. Footer Bar Section
    float footerY = modalY + modalH - 50.0f;

    // A. Current Location Badge (Bottom Left)
    std::string locText = "LOCATION: " + locName;
    DrawRectangleRounded(Rectangle{ modalX + 24.0f, footerY + 6.0f, 340.0f, 32.0f }, 0.25f, 4, Color{ 20, 26, 38, 230 });
    DrawRectangleRoundedLines(Rectangle{ modalX + 24.0f, footerY + 6.0f, 340.0f, 32.0f }, 0.25f, 4, Color{ 70, 95, 135, 220 });
    DrawCircle((int)(modalX + 40.0f), (int)(footerY + 22.0f), 4.0f, Color{ 0, 229, 255, 255 });
    FontManager::DrawGameTextShadow(locText.c_str(), (int)(modalX + 52.0f), (int)(footerY + 15.0f), 11, Color{ 230, 245, 255, 255 });

    // B. Navigation Hint (Center)
    const char* navHint = "[Drag] Pan Map  |  [Wheel] Zoom  |  [SPACE / R] Recenter";
    FontManager::DrawGameTextShadow(navHint, (int)(modalX + 400.0f), (int)(footerY + 16.0f), 11, Color{ 160, 185, 215, 210 });

    // C. Action Buttons (Bottom Right)
    // 1. Recenter Button
    float rBtnX = modalX + modalW - 390.0f;
    Rectangle rBtnRec = { rBtnX, footerY + 6.0f, 105.0f, 32.0f };
    bool isRHov = CheckCollisionPointRec(mousePos, rBtnRec);
    if ((isRHov && isMousePressed)) {
        s_mapPan = { 0.0f, 0.0f };
        s_mapZoom = 1.0f;
        SoundFX::PlayButtonClick();
    }
    DrawRectangleRounded(rBtnRec, 0.25f, 4, isRHov ? Color{ 40, 55, 80, 240 } : Color{ 24, 30, 44, 210 });
    DrawRectangleRoundedLines(rBtnRec, 0.25f, 4, isRHov ? Color{ 140, 210, 255, 255 } : Color{ 65, 85, 120, 200 });
    FontManager::DrawText("RECENTER", (int)rBtnX + 16, (int)footerY + 15, 11, isRHov ? Color{ 255, 255, 255, 255 } : Color{ 180, 205, 235, 220 }, GameFont::BODY);

    // 2. Zoom Out (-)
    float zmBtnX = rBtnX + 115.0f;
    Rectangle zmRec = { zmBtnX, footerY + 6.0f, 42.0f, 32.0f };
    bool isZmHov = CheckCollisionPointRec(mousePos, zmRec);
    if (isZmHov && isMousePressed) {
        s_mapZoom = fmaxf(0.75f, s_mapZoom - 0.2f);
        SoundFX::PlayButtonClick();
    }
    DrawRectangleRounded(zmRec, 0.25f, 4, isZmHov ? Color{ 40, 55, 80, 240 } : Color{ 24, 30, 44, 210 });
    DrawRectangleRoundedLines(zmRec, 0.25f, 4, isZmHov ? Color{ 140, 210, 255, 255 } : Color{ 65, 85, 120, 200 });
    FontManager::DrawText("-", (int)zmBtnX + 17, (int)footerY + 14, 14, WHITE, GameFont::TITLE);

    // 3. Zoom In (+)
    float zpBtnX = zmBtnX + 50.0f;
    Rectangle zpRec = { zpBtnX, footerY + 6.0f, 42.0f, 32.0f };
    bool isZpHov = CheckCollisionPointRec(mousePos, zpRec);
    if (isZpHov && isMousePressed) {
        s_mapZoom = fminf(2.2f, s_mapZoom + 0.2f);
        SoundFX::PlayButtonClick();
    }
    DrawRectangleRounded(zpRec, 0.25f, 4, isZpHov ? Color{ 40, 55, 80, 240 } : Color{ 24, 30, 44, 210 });
    DrawRectangleRoundedLines(zpRec, 0.25f, 4, isZpHov ? Color{ 140, 210, 255, 255 } : Color{ 65, 85, 120, 200 });
    FontManager::DrawText("+", (int)zpBtnX + 15, (int)footerY + 14, 14, WHITE, GameFont::TITLE);

    // 4. Return to Game Button
    float retBtnX = zpBtnX + 58.0f;
    Rectangle retRec = { retBtnX, footerY + 6.0f, 100.0f, 32.0f };
    bool isRetHov = CheckCollisionPointRec(mousePos, retRec);
    if (isRetHov && isMousePressed) {
        shouldClose = true;
        SoundFX::PlayButtonClick();
    }
    DrawRectangleRounded(retRec, 0.25f, 4, isRetHov ? Color{ 55, 75, 110, 255 } : Color{ 28, 36, 52, 230 });
    DrawRectangleRoundedLines(retRec, 0.25f, 4, isRetHov ? Color{ 120, 220, 255, 255 } : Color{ 70, 90, 125, 230 });
    FontManager::DrawText("RETURN", (int)retBtnX + 22, (int)footerY + 15, 11, isRetHov ? Color{ 255, 255, 255, 255 } : Color{ 190, 210, 235, 220 }, GameFont::BODY);

    if (shouldClose) {
        SoundFX::PlayButtonClick();
        return true;
    }

    return false;
}

