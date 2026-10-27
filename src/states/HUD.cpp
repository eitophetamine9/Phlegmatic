#include "HUD.hpp"
#include "../combat/Weapon.hpp"
#include "../entities/Player.hpp"
#include "../rendering/FontManager.hpp"
#include <cmath>
#include <cstdio>

void HUD::DrawActionSlot(const ActionSlotInfo& slot) {
    const Rectangle& r = slot.bounds;
    bool onCooldown = slot.currentCooldown > 0.001f;
    bool outOfStock = (slot.stockCount == 0);

    // Background tile
    Color bgColor = outOfStock ? Color{ 24, 14, 18, 220 } : Color{ 14, 18, 26, 230 };
    DrawRectangleRounded(r, 0.22f, 4, bgColor);

    // Cooldown wipe overlay
    if (onCooldown && slot.maxCooldown > 0.001f) {
        float ratio = fminf(1.0f, fmaxf(0.0f, slot.currentCooldown / slot.maxCooldown));
        float sweepH = r.height * ratio;
        float sweepY = r.y + (r.height - sweepH);
        DrawRectangleRounded(Rectangle{ r.x, sweepY, r.width, sweepH }, 0.22f, 4, Color{ 8, 10, 16, 210 });
        DrawLineEx({ r.x + 3.0f, sweepY }, { r.x + r.width - 3.0f, sweepY }, 1.5f, Color{ 200, 220, 255, 180 });
    }

    // Border
    Color borderColor;
    if (slot.isActive) {
        borderColor = slot.themeColor;
        // Outer glowing pulse ring
        DrawRectangleRoundedLines(Rectangle{ r.x - 2.0f, r.y - 2.0f, r.width + 4.0f, r.height + 4.0f }, 0.25f, 4, Color{ slot.themeColor.r, slot.themeColor.g, slot.themeColor.b, 180 });
    } else if (onCooldown) {
        borderColor = Color{ 48, 58, 76, 200 };
    } else if (outOfStock) {
        borderColor = Color{ 120, 40, 50, 190 };
    } else {
        borderColor = Color{ slot.themeColor.r, slot.themeColor.g, slot.themeColor.b, 200 };
    }
    DrawRectangleRoundedLines(r, 0.22f, 4, borderColor);

    // Keybind badge at top-left
    DrawText(slot.keybind, (int)r.x + 4, (int)r.y + 4, 10, Color{ 210, 225, 245, 220 });

    // Stock badge at top-right (if consumable)
    if (slot.stockCount >= 0) {
        std::string stockStr = "x" + std::to_string(slot.stockCount);
        int sw = MeasureText(stockStr.c_str(), 10);
        float badgeW = (float)sw + 8.0f;
        float badgeH = 13.0f;
        float badgeX = r.x + r.width - badgeW - 3.0f;
        float badgeY = r.y + 3.0f;
        Color badgeBg = (slot.stockCount > 0) ? Color{ 20, 60, 35, 220 } : Color{ 60, 20, 25, 220 };
        Color badgeTxt = (slot.stockCount > 0) ? Color{ 120, 255, 170, 255 } : Color{ 255, 120, 120, 255 };
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 3, badgeBg);
        DrawText(stockStr.c_str(), (int)badgeX + 4, (int)badgeY + 1, 10, badgeTxt);
    }

    // Centered content: countdown if on cooldown, else action name
    if (onCooldown) {
        char cdBuf[16];
        if (slot.currentCooldown >= 10.0f) {
            snprintf(cdBuf, sizeof(cdBuf), "%.0fs", slot.currentCooldown);
        } else {
            snprintf(cdBuf, sizeof(cdBuf), "%.1fs", slot.currentCooldown);
        }
        int cw = MeasureText(cdBuf, 13);
        int cx = (int)(r.x + (r.width - cw) * 0.5f);
        int cy = (int)(r.y + (r.height - 13) * 0.5f);
        DrawText(cdBuf, cx + 1, cy + 1, 13, Color{ 0, 0, 0, 220 });
        DrawText(cdBuf, cx, cy, 13, Color{ 255, 240, 150, 255 });
    } else {
        int nw = MeasureText(slot.actionName, 10);
        int nx = (int)(r.x + (r.width - nw) * 0.5f);
        int ny = (int)(r.y + (r.height - 10) * 0.5f + 3.0f);
        Color txtCol = outOfStock ? Color{ 140, 100, 105, 180 } : (slot.isActive ? Color{ 255, 255, 255, 255 } : slot.themeColor);
        DrawText(slot.actionName, nx, ny, 10, txtCol);
    }
}

void HUD::DrawActionBar(int windowWidth, int windowHeight, const Player& player) {
    const int numSlots = 5;
    const float slotW = 50.0f;
    const float slotH = 50.0f;
    const float slotSpacing = 8.0f;
    const float totalW = numSlots * slotW + (numSlots - 1) * slotSpacing;
    const float barX = ((float)windowWidth - totalW) * 0.5f;
    const float barY = (float)windowHeight - slotH - 18.0f;

    // Backing glass plate
    Rectangle plate = { barX - 10.0f, barY - 16.0f, totalW + 20.0f, slotH + 24.0f };
    DrawRectangleRounded(plate, 0.22f, 6, Color{ 12, 16, 24, 215 });
    DrawRectangleRoundedLines(plate, 0.22f, 6, Color{ 48, 62, 85, 200 });

    // Status / Mode Banner centered right above the slots
    if (player.HasCounterBuff()) {
        const char* banner = "COUNTER BUFF ACTIVE: +50% DAMAGE";
        int bw = MeasureText(banner, 10);
        DrawText(banner, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 10, Color{ 255, 225, 80, 255 });
    } else if (player.IsParrying()) {
        const char* banner = "PARRY WINDOW ACTIVE";
        int bw = MeasureText(banner, 10);
        DrawText(banner, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 10, Color{ 80, 240, 255, 255 });
    } else if (player.IsStaggered()) {
        char stagBuf[64];
        snprintf(stagBuf, sizeof(stagBuf), "STAGGERED! (%.1fs)", player.GetStaggerTimer());
        int bw = MeasureText(stagBuf, 10);
        DrawText(stagBuf, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 10, Color{ 255, 180, 60, 255 });
    } else if (player.HasStaggerImmunity()) {
        const char* banner = "POSTURE GUARD (IMMUNITY)";
        int bw = MeasureText(banner, 10);
        DrawText(banner, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 10, Color{ 140, 220, 255, 220 });
    } else {
        const char* title = "ACTION ABILITIES";
        int bw = MeasureText(title, 9);
        DrawText(title, (int)(barX + (totalW - bw) * 0.5f), (int)plate.y + 3, 9, Color{ 140, 170, 205, 180 });
    }

    // Slot 1: Dash [Q]
    ActionSlotInfo dashSlot;
    dashSlot.bounds = { barX + 0 * (slotW + slotSpacing), barY, slotW, slotH };
    dashSlot.actionName = "DASH";
    dashSlot.keybind = "[Q]";
    dashSlot.currentCooldown = player.GetDashCooldown();
    dashSlot.maxCooldown = player.GetMaxDashCooldown();
    dashSlot.themeColor = Color{ 70, 160, 255, 255 };
    dashSlot.isActive = player.IsDashing();
    dashSlot.stockCount = -1;
    DrawActionSlot(dashSlot);

    // Slot 2: Light Attack [LMB]
    ActionSlotInfo lightSlot;
    lightSlot.bounds = { barX + 1 * (slotW + slotSpacing), barY, slotW, slotH };
    lightSlot.actionName = "LIGHT";
    lightSlot.keybind = "[LMB]";
    lightSlot.currentCooldown = player.GetLightCooldown();
    lightSlot.maxCooldown = player.GetMaxLightCooldown();
    lightSlot.themeColor = Color{ 90, 230, 255, 255 };
    lightSlot.isActive = player.IsAttacking() && player.GetCurrentAttack().type == AttackType::LIGHT;
    lightSlot.stockCount = -1;
    DrawActionSlot(lightSlot);

    // Slot 3: Heavy Attack [RMB]
    ActionSlotInfo heavySlot;
    heavySlot.bounds = { barX + 2 * (slotW + slotSpacing), barY, slotW, slotH };
    heavySlot.actionName = "HEAVY";
    heavySlot.keybind = "[RMB]";
    heavySlot.currentCooldown = player.GetHeavyCooldown();
    heavySlot.maxCooldown = player.GetMaxHeavyCooldown();
    heavySlot.themeColor = Color{ 255, 175, 50, 255 };
    heavySlot.isActive = player.IsHeavyWindingUp() || (player.IsAttacking() && player.GetCurrentAttack().type == AttackType::HEAVY);
    heavySlot.stockCount = -1;
    DrawActionSlot(heavySlot);

    // Slot 4: Parry [F]
    ActionSlotInfo parrySlot;
    parrySlot.bounds = { barX + 3 * (slotW + slotSpacing), barY, slotW, slotH };
    parrySlot.actionName = "PARRY";
    parrySlot.keybind = "[F]";
    parrySlot.currentCooldown = player.GetParryCooldown();
    parrySlot.maxCooldown = player.GetMaxParryCooldown();
    parrySlot.themeColor = player.HasCounterBuff() ? Color{ 255, 225, 80, 255 } : Color{ 80, 240, 255, 255 };
    parrySlot.isActive = player.IsParrying() || player.HasCounterBuff();
    parrySlot.stockCount = -1;
    DrawActionSlot(parrySlot);

    // Slot 5: Health Potion [1]
    ActionSlotInfo potSlot;
    potSlot.bounds = { barX + 4 * (slotW + slotSpacing), barY, slotW, slotH };
    potSlot.actionName = "POTION";
    potSlot.keybind = "[1]";
    potSlot.currentCooldown = player.GetPotionCooldown();
    potSlot.maxCooldown = 7.0f;
    potSlot.themeColor = Color{ 70, 230, 130, 255 };
    potSlot.isActive = false;
    potSlot.stockCount = player.GetInventory().GetHealthPotionCount();
    DrawActionSlot(potSlot);
}

static void DrawPlayerCard(int windowWidth, int windowHeight, const Player& player) {
    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 114.0f;
    const float hudWidth = 320.0f;
    const float hudHeight = 90.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 215 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    // HEALTH BAR
    float hpPercent = fmaxf(0.0f, fminf(1.0f, player.GetHealth() / player.GetMaxHealth()));
    DrawText("HP", (int)hudX + 14, (int)hudY + 12, 11, Color{ 235, 80, 90, 255 });
    DrawRectangle((int)hudX + 68, (int)hudY + 12, 170, 12, Color{ 32, 20, 24, 220 });
    DrawRectangle((int)hudX + 68, (int)hudY + 12, (int)(170 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 68, (int)hudY + 12, 170, 12, Color{ 85, 40, 50, 220 });
    char hpBuf[32];
    snprintf(hpBuf, sizeof(hpBuf), "%d/%d", (int)player.GetHealth(), (int)player.GetMaxHealth());
    DrawText(hpBuf, (int)hudX + 246, (int)hudY + 12, 10, Color{ 230, 205, 210, 225 });

    // STAMINA BAR
    float stamPercent = fmaxf(0.0f, fminf(1.0f, player.GetStamina() / player.GetMaxStamina()));
    DrawText("STAM", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 68, (int)hudY + 32, 170, 12, Color{ 35, 30, 16, 220 });
    DrawRectangle((int)hudX + 68, (int)hudY + 32, (int)(170 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 68, (int)hudY + 32, 170, 12, Color{ 90, 75, 32, 220 });
    char stamBuf[32];
    snprintf(stamBuf, sizeof(stamBuf), "%d/%d", (int)player.GetStamina(), (int)player.GetMaxStamina());
    DrawText(stamBuf, (int)hudX + 246, (int)hudY + 32, 10, Color{ 245, 230, 190, 225 });

    // ATTRIBUTES ROW: DEFENSE & WILL
    char defBuf[32];
    snprintf(defBuf, sizeof(defBuf), "DEF: %d", (int)player.GetDefense());
    DrawText(defBuf, (int)hudX + 14, (int)hudY + 58, 11, Color{ 140, 210, 255, 240 });

    char willBuf[32];
    snprintf(willBuf, sizeof(willBuf), "WILL: %d", (int)player.GetWill());
    DrawText(willBuf, (int)hudX + 88, (int)hudY + 58, 11, Color{ 255, 205, 110, 240 });

    // STATUS BADGE PILL
    const float badgeX = hudX + 172.0f;
    const float badgeY = hudY + 54.0f;
    const float badgeW = 136.0f;
    const float badgeH = 20.0f;

    if (player.IsStaggered()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 70, 35, 15, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 255, 180, 50, 240 });
        DrawText("STAGGERED!", (int)badgeX + 24, (int)badgeY + 5, 10, Color{ 255, 210, 90, 255 });
    } else if (player.HasStaggerImmunity()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 20, 50, 70, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 100, 210, 255, 240 });
        DrawText("POISE GUARD", (int)badgeX + 22, (int)badgeY + 5, 10, Color{ 180, 235, 255, 255 });
    } else if (player.HasInvulnerability()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 20, 60, 60, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 80, 250, 210, 240 });
        DrawText("I-FRAMES", (int)badgeX + 36, (int)badgeY + 5, 10, Color{ 140, 255, 230, 255 });
    } else if (player.HasCounterBuff()) {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 65, 50, 15, 230 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 255, 220, 60, 255 });
        DrawText("+50% COUNTER", (int)badgeX + 18, (int)badgeY + 5, 10, Color{ 255, 235, 100, 255 });
    } else {
        DrawRectangleRounded(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 18, 28, 38, 210 });
        DrawRectangleRoundedLines(Rectangle{ badgeX, badgeY, badgeW, badgeH }, 0.35f, 4, Color{ 60, 85, 115, 190 });
        DrawText("COMBAT READY", (int)badgeX + 20, (int)badgeY + 5, 10, Color{ 160, 200, 235, 220 });
    }
}

void HUD::DrawLobby(int windowWidth, int windowHeight, const Player& player, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore) {
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

    // Journal & Lore button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[I] Journal", windowWidth - 263, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 170, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

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
    WeaponStats stats = GetWeaponStats(player.GetWeapon());
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
    DrawPlayerCard(windowWidth, windowHeight, player);

    // Modular Visual Cooldown Action Bar (Bottom Center)
    DrawActionBar(windowWidth, windowHeight, player);
}

void HUD::DrawCombat(int windowWidth, int windowHeight, const Player& player, int currentWave, int totalWaves, bool hasUnreadLore) {
    // Player HUD Panel (Bottom Left)
    DrawPlayerCard(windowWidth, windowHeight, player);

    // Modular Visual Cooldown Action Bar (Bottom Center)
    DrawActionBar(windowWidth, windowHeight, player);

    // Active Weapon Badge (Bottom Right)
    WeaponStats stats = GetWeaponStats(player.GetWeapon());
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

    // Journal & Lore button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[I] Journal", windowWidth - 263, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 170, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 128, 25, 12, Color{ 120, 220, 255, 240 });
}

void HUD::DrawCampaignHUD(int windowWidth, int windowHeight, const Player& player, const std::string& locationName, bool hasUnreadLore) {
    // Player HUD Panel (Bottom Left)
    DrawPlayerCard(windowWidth, windowHeight, player);

    // Modular Visual Cooldown Action Bar (Bottom Center)
    DrawActionBar(windowWidth, windowHeight, player);

    // Active Weapon Badge (Bottom Right)
    WeaponStats stats = GetWeaponStats(player.GetWeapon());
    const float wBadgeW = 200.0f;
    const float wBadgeH = 46.0f;
    float wBadgeX = (float)windowWidth - wBadgeW - 24.0f;
    float wBadgeY = (float)windowHeight - wBadgeH - 24.0f;

    DrawRectangleRounded(Rectangle{ wBadgeX, wBadgeY, wBadgeW, wBadgeH }, 0.25f, 6, Color{ 16, 20, 30, 210 });
    DrawRectangleRoundedLines(Rectangle{ wBadgeX, wBadgeY, wBadgeW, wBadgeH }, 0.25f, 6, Color{ 55, 75, 105, 220 });
    DrawText("WEAPON:", (int)wBadgeX + 12, (int)wBadgeY + 8, 10, Color{ 255, 215, 80, 255 });
    DrawText(stats.name.c_str(), (int)wBadgeX + 12, (int)wBadgeY + 22, 14, Color{ 255, 255, 255, 255 });

    // Top Location Header Banner (NO wave counter in campaign!)
    DrawLocationHeader(windowWidth, locationName);

    // Journal & Lore button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[I] Journal", windowWidth - 263, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 170, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 128, 25, 12, Color{ 120, 220, 255, 240 });
}

// Legacy float-based overloads
void HUD::DrawLobby(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore) {
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

    // Journal & Lore button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[I] Journal", windowWidth - 263, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 170, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

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
    const float hudWidth = 310.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(215 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 80, 40, 50, 220 });

    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(215 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 90, 75, 30, 220 });

    char dashBuf[32];
    char parryBuf[32];
    char heavyBuf[32];
    char statusBuf[128];

    if (dashCooldown <= 0.0f) snprintf(dashBuf, sizeof(dashBuf), "[Q] Dash");
    else snprintf(dashBuf, sizeof(dashBuf), "[Q] %.1fs", dashCooldown);

    if (parryCooldown <= 0.0f) snprintf(parryBuf, sizeof(parryBuf), "[F] Parry");
    else snprintf(parryBuf, sizeof(parryBuf), "[F] %.1fs", parryCooldown);

    if (heavyCooldown <= 0.0f) snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] Heavy");
    else snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] %.1fs", heavyCooldown);

    snprintf(statusBuf, sizeof(statusBuf), "%s  |  %s  |  %s", dashBuf, parryBuf, heavyBuf);
    DrawText(statusBuf, (int)hudX + 14, (int)hudY + 58, 10, Color{ 170, 205, 235, 240 });
}

void HUD::DrawCombat(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves, bool hasUnreadLore) {
    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 110.0f;
    const float hudWidth = 310.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(215 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 80, 40, 50, 220 });

    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(215 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 90, 75, 30, 220 });

    if (hasCounterBuff) {
        DrawText("COUNTER BUFF: +50% DAMAGE!", (int)hudX + 14, (int)hudY + 58, 11, Color{ 255, 230, 80, 255 });
    } else if (isParrying) {
        DrawText("PARRY WINDOW ACTIVE!", (int)hudX + 14, (int)hudY + 58, 11, Color{ 80, 240, 255, 255 });
    } else {
        char dashBuf[32];
        char parryBuf[32];
        char heavyBuf[32];
        char statusBuf[128];

        if (dashCooldown <= 0.0f) snprintf(dashBuf, sizeof(dashBuf), "[Q] Dash");
        else snprintf(dashBuf, sizeof(dashBuf), "[Q] %.1fs", dashCooldown);

        if (parryCooldown <= 0.0f) snprintf(parryBuf, sizeof(parryBuf), "[F] Parry");
        else snprintf(parryBuf, sizeof(parryBuf), "[F] %.1fs", parryCooldown);

        if (heavyCooldown <= 0.0f) snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] Heavy");
        else snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] %.1fs", heavyCooldown);

        snprintf(statusBuf, sizeof(statusBuf), "%s  |  %s  |  %s", dashBuf, parryBuf, heavyBuf);
        DrawText(statusBuf, (int)hudX + 14, (int)hudY + 58, 10, Color{ 170, 205, 235, 240 });
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

    // Journal & Lore button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[I] Journal", windowWidth - 263, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 170, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 128, 25, 12, Color{ 120, 220, 255, 240 });
}

void HUD::DrawLocationHeader(int windowWidth, const std::string& locationName) {
    const float bannerW = 340.0f;
    const float bannerH = 42.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = 16.0f;

    // Dark glass backing card
    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 14, 18, 28, 225 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.25f, 6, Color{ 205, 165, 75, 240 });
    DrawRectangleRoundedLines(Rectangle{ bannerX + 3, bannerY + 3, bannerW - 6, bannerH - 6 }, 0.20f, 6, Color{ 48, 62, 90, 160 });

    // Golden corner accents
    Color cornerCol = Color{ 255, 215, 80, 240 };
    DrawLineEx({ bannerX + 8, bannerY + 8 }, { bannerX + 22, bannerY + 8 }, 1.5f, cornerCol);
    DrawLineEx({ bannerX + 8, bannerY + 8 }, { bannerX + 8, bannerY + 22 }, 1.5f, cornerCol);
    DrawLineEx({ bannerX + bannerW - 8, bannerY + 8 }, { bannerX + bannerW - 22, bannerY + 8 }, 1.5f, cornerCol);
    DrawLineEx({ bannerX + bannerW - 8, bannerY + 8 }, { bannerX + bannerW - 8, bannerY + 22 }, 1.5f, cornerCol);

    // Map Name centered in Gold
    int nameW = MeasureText(locationName.c_str(), 16);
    DrawText(locationName.c_str(), (int)(bannerX + (bannerW - nameW) * 0.5f), (int)bannerY + 13, 16, Color{ 255, 225, 95, 255 });
}

void HUD::DrawCampaignHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, const std::string& locationName, bool hasUnreadLore) {
    const float hudX = 24.0f;
    const float hudY = (float)windowHeight - 110.0f;
    const float hudWidth = 310.0f;
    const float hudHeight = 86.0f;

    DrawRectangleRounded(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 14, 18, 26, 210 });
    DrawRectangleRoundedLines(Rectangle{ hudX, hudY, hudWidth, hudHeight }, 0.2f, 6, Color{ 48, 62, 85, 220 });

    float hpPercent = fmaxf(0.0f, hp / maxHp);
    DrawText("HEALTH", (int)hudX + 14, (int)hudY + 12, 11, Color{ 230, 80, 90, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 30, 20, 24, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 12, (int)(215 * hpPercent), 12, Color{ 235, 55, 75, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 12, 215, 12, Color{ 80, 40, 50, 220 });

    float stamPercent = fmaxf(0.0f, stamina / maxStamina);
    DrawText("STAMINA", (int)hudX + 14, (int)hudY + 32, 11, Color{ 255, 210, 80, 255 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 35, 30, 15, 220 });
    DrawRectangle((int)hudX + 70, (int)hudY + 32, (int)(215 * stamPercent), 12, Color{ 255, 195, 45, 255 });
    DrawRectangleLines((int)hudX + 70, (int)hudY + 32, 215, 12, Color{ 90, 75, 30, 220 });

    if (hasCounterBuff) {
        DrawText("COUNTER BUFF: +50% DAMAGE!", (int)hudX + 14, (int)hudY + 58, 11, Color{ 255, 230, 80, 255 });
    } else if (isParrying) {
        DrawText("PARRY WINDOW ACTIVE!", (int)hudX + 14, (int)hudY + 58, 11, Color{ 80, 240, 255, 255 });
    } else {
        char dashBuf[32];
        char parryBuf[32];
        char heavyBuf[32];
        char statusBuf[128];

        if (dashCooldown <= 0.0f) snprintf(dashBuf, sizeof(dashBuf), "[Q] Dash");
        else snprintf(dashBuf, sizeof(dashBuf), "[Q] %.1fs", dashCooldown);

        if (parryCooldown <= 0.0f) snprintf(parryBuf, sizeof(parryBuf), "[F] Parry");
        else snprintf(parryBuf, sizeof(parryBuf), "[F] %.1fs", parryCooldown);

        if (heavyCooldown <= 0.0f) snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] Heavy");
        else snprintf(heavyBuf, sizeof(heavyBuf), "[R-Atk] %.1fs", heavyCooldown);

        snprintf(statusBuf, sizeof(statusBuf), "%s  |  %s  |  %s", dashBuf, parryBuf, heavyBuf);
        DrawText(statusBuf, (int)hudX + 14, (int)hudY + 58, 10, Color{ 170, 205, 235, 240 });
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

    // Top Location Header Banner (NO wave counter in campaign!)
    DrawLocationHeader(windowWidth, locationName);

    // Journal & Lore button in top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 275), 16.0f, 120.0f, 32.0f }, 0.3f, 4, hasUnreadLore ? Color{ 255, 215, 80, 240 } : Color{ 60, 85, 120, 220 });
    DrawText("[I] Journal", windowWidth - 263, 25, 12, hasUnreadLore ? Color{ 255, 225, 100, 255 } : Color{ 140, 210, 255, 240 });
    if (hasUnreadLore) {
        DrawCircle(windowWidth - 170, 23, 4.0f, Color{ 255, 200, 50, 255 });
    }

    // Controls hint top right
    DrawRectangleRounded(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 25, 35, 52, 220 });
    DrawRectangleRoundedLines(Rectangle{ (float)(windowWidth - 140), 16.0f, 116.0f, 32.0f }, 0.3f, 4, Color{ 60, 85, 120, 220 });
    DrawText("[C] Controls", windowWidth - 128, 25, 12, Color{ 120, 220, 255, 240 });
}

void HUD::DrawBossHealthBar(int windowWidth, const std::string& bossName, float hp, float maxHp) {
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

void HUD::DrawBossDialogueBanner(int windowWidth, int windowHeight, const std::string& speaker, const std::string& text, Color accentColor) {
    float bannerW = 820.0f;
    float bannerH = 74.0f;
    float bannerX = ((float)windowWidth - bannerW) * 0.5f;
    float bannerY = (float)windowHeight - 165.0f;

    DrawRectangleRounded(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.2f, 6, Color{ 12, 15, 22, 235 });
    DrawRectangleRoundedLines(Rectangle{ bannerX, bannerY, bannerW, bannerH }, 0.2f, 6, accentColor);

    // Speaker Name
    DrawText(speaker.c_str(), (int)bannerX + 22, (int)bannerY + 12, 11, accentColor);

    // Text Quote
    DrawText(text.c_str(), (int)bannerX + 22, (int)bannerY + 32, 14, Color{ 240, 244, 255, 250 });
}

bool HUD::DrawNPCDialogueBox(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const std::string& speaker, const std::vector<std::string>& pages, int& currentPage, Color accentColor) {
    if (pages.empty()) return false;

    // Bounds check current page
    if (currentPage < 0) currentPage = 0;
    if (currentPage >= (int)pages.size()) currentPage = (int)pages.size() - 1;

    // ESC key closes dialogue immediately
    if (IsKeyPressed(KEY_ESCAPE)) {
        return false;
    }

    // Modal dimensions
    const float boxW = 840.0f;
    const float boxH = 190.0f;
    const float boxX = ((float)windowWidth - boxW) * 0.5f;
    const float boxY = (float)windowHeight - boxH - 28.0f;

    // Dark backdrop card with accent border
    DrawRectangleRounded(Rectangle{ boxX, boxY, boxW, boxH }, 0.14f, 6, Color{ 14, 18, 28, 245 });
    DrawRectangleRoundedLines(Rectangle{ boxX, boxY, boxW, boxH }, 0.14f, 6, accentColor);
    DrawRectangleRoundedLines(Rectangle{ boxX + 3.0f, boxY + 3.0f, boxW - 6.0f, boxH - 6.0f }, 0.12f, 6, Color{ 45, 60, 85, 180 });

    // Speaker Name Header (Golden/Accent)
    FontManager::DrawText(speaker.c_str(), (int)boxX + 28, (int)boxY + 16, 17, accentColor, GameFont::TITLE);

    // Page indicator top-right
    char pageBuf[32];
    snprintf(pageBuf, sizeof(pageBuf), "Page %d / %d", currentPage + 1, (int)pages.size());
    FontManager::DrawText(pageBuf, (int)(boxX + boxW - 120.0f), (int)boxY + 18, 12, Color{ 170, 190, 220, 230 });

    // Word-wrapped text rendering with authentic medieval typography
    const std::string& text = pages[currentPage];
    const float maxLineWidth = boxW - 56.0f;
    FontManager::DrawTextWrapped(text, (int)boxX + 28, (int)boxY + 46, (int)maxLineWidth, 15, 6, Color{ 235, 240, 252, 250 }, GameFont::BODY);

    // Navigation Buttons at Bottom
    const float btnY = boxY + boxH - 40.0f;
    const float btnH = 26.0f;

    // 1. [ < PREV (A) ] Button
    bool hasPrev = (currentPage > 0);
    Rectangle prevBtn = { boxX + 28.0f, btnY, 110.0f, btnH };
    bool prevHover = hasPrev && CheckCollisionPointRec(mousePos, prevBtn);
    Color prevBg = !hasPrev ? Color{ 25, 30, 42, 120 } : (prevHover ? Color{ 55, 75, 115, 240 } : Color{ 32, 44, 65, 220 });
    Color prevBorder = !hasPrev ? Color{ 50, 60, 80, 100 } : (prevHover ? Color{ 140, 195, 255, 255 } : Color{ 70, 95, 140, 200 });
    Color prevText = !hasPrev ? Color{ 100, 110, 130, 120 } : (prevHover ? Color{ 255, 255, 255, 255 } : Color{ 190, 215, 245, 230 });

    DrawRectangleRounded(prevBtn, 0.25f, 4, prevBg);
    DrawRectangleRoundedLines(prevBtn, 0.25f, 4, prevBorder);
    int prevW = FontManager::MeasureText("< PREV [A]", 11);
    FontManager::DrawText("< PREV [A]", (int)(prevBtn.x + (prevBtn.width - prevW) * 0.5f), (int)prevBtn.y + 7, 11, prevText);

    if (hasPrev && ((prevHover && isMousePressed) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))) {
        currentPage--;
    }

    // 2. [ NEXT > (D) ] or [ FINISH (SPACE) ] Button
    bool isLastPage = (currentPage >= (int)pages.size() - 1);
    Rectangle nextBtn = { boxX + 150.0f, btnY, 130.0f, btnH };
    bool nextHover = CheckCollisionPointRec(mousePos, nextBtn);
    Color nextBg = nextHover ? Color{ 70, 90, 135, 240 } : Color{ 38, 52, 80, 220 };
    Color nextBorder = nextHover ? Color{ 255, 220, 110, 255 } : Color{ 180, 150, 70, 220 };
    Color nextText = nextHover ? Color{ 255, 240, 150, 255 } : Color{ 240, 225, 160, 240 };

    DrawRectangleRounded(nextBtn, 0.25f, 4, nextBg);
    DrawRectangleRoundedLines(nextBtn, 0.25f, 4, nextBorder);
    const char* nextLabel = isLastPage ? "FINISH [SPACE]" : "NEXT > [D]";
    int nextW = FontManager::MeasureText(nextLabel, 11);
    FontManager::DrawText(nextLabel, (int)(nextBtn.x + (nextBtn.width - nextW) * 0.5f), (int)nextBtn.y + 7, 11, nextText);

    if ((nextHover && isMousePressed) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_SPACE)) {
        if (!isLastPage) {
            currentPage++;
        } else {
            return false; // Finished reading dialogue!
        }
    }

    // 3. [ CLOSE (ESC) ] Button
    Rectangle closeBtn = { boxX + boxW - 138.0f, btnY, 110.0f, btnH };
    bool closeHover = CheckCollisionPointRec(mousePos, closeBtn);
    Color closeBg = closeHover ? Color{ 130, 40, 50, 240 } : Color{ 58, 28, 35, 220 };
    Color closeBorder = closeHover ? Color{ 255, 110, 120, 255 } : Color{ 150, 60, 75, 200 };
    Color closeText = closeHover ? Color{ 255, 230, 235, 255 } : Color{ 230, 175, 185, 230 };

    DrawRectangleRounded(closeBtn, 0.25f, 4, closeBg);
    DrawRectangleRoundedLines(closeBtn, 0.25f, 4, closeBorder);
    int closeW = FontManager::MeasureText("CLOSE [ESC]", 11);
    FontManager::DrawText("CLOSE [ESC]", (int)(closeBtn.x + (closeBtn.width - closeW) * 0.5f), (int)closeBtn.y + 7, 11, closeText);

    if (closeHover && isMousePressed) {
        return false;
    }

    return true; // Keep open
}

void HUD::DrawGameOver(int windowWidth, int windowHeight) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 20, 5, 8, 210 });
    FontManager::DrawTextTitleCentered("FALLEN IN BATTLE", windowWidth / 2, windowHeight / 2 - 40, 42, Color{ 255, 50, 60, 255 });
    FontManager::DrawTextCentered("Press [R] to Respawn  |  [L] Return to Lobby", windowWidth / 2, windowHeight / 2 + 18, 16, Color{ 220, 225, 240, 230 });
}

void HUD::DrawVictory(int windowWidth, int windowHeight) {
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 10, 24, 18, 210 });
    FontManager::DrawTextTitleCentered("ARENA CLEARED", windowWidth / 2, windowHeight / 2 - 40, 42, Color{ 255, 225, 80, 255 });
    FontManager::DrawTextCentered("Press [R] to Replay  |  [L] Return to Lobby", windowWidth / 2, windowHeight / 2 + 18, 16, Color{ 210, 240, 225, 230 });
}
