#include "UI.hpp"
#include "TitleUI.hpp"
#include "HUD.hpp"
#include "ModalUI.hpp"

int GameUI::DrawTitleScreen(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool isArenaUnlocked) {
    return TitleUI::Draw(windowWidth, windowHeight, mousePos, isMousePressed, isArenaUnlocked);
}

bool GameUI::DrawControlsModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed) {
    return ModalUI::DrawControls(windowWidth, windowHeight, mousePos, isMousePressed);
}

bool GameUI::DrawInventoryModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex) {
    return ModalUI::DrawInventory(windowWidth, windowHeight, mousePos, isMousePressed, player, activeTab, selectedFragmentIndex);
}

void GameUI::DrawLobbyHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore) {
    HUD::DrawLobby(windowWidth, windowHeight, hp, maxHp, stamina, maxStamina, weapon, dashCooldown, parryCooldown, heavyCooldown, isNearPedestal, nearWeaponName, hasUnreadLore);
}

void GameUI::DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves, bool hasUnreadLore) {
    HUD::DrawCombat(windowWidth, windowHeight, hp, maxHp, stamina, maxStamina, weapon, dashCooldown, parryCooldown, heavyCooldown, isParrying, hasCounterBuff, currentWave, totalWaves, hasUnreadLore);
}

void GameUI::DrawBossHealthBar(int windowWidth, const std::string& bossName, float hp, float maxHp) {
    HUD::DrawBossHealthBar(windowWidth, bossName, hp, maxHp);
}

void GameUI::DrawGameOver(int windowWidth, int windowHeight) {
    HUD::DrawGameOver(windowWidth, windowHeight);
}

void GameUI::DrawVictory(int windowWidth, int windowHeight) {
    HUD::DrawVictory(windowWidth, windowHeight);
}

int GameUI::DrawZilDefeatModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool canRetry) {
    return ModalUI::DrawZilDefeat(windowWidth, windowHeight, mousePos, isMousePressed, canRetry);
}

void GameUI::DrawBossDialogueBanner(int windowWidth, int windowHeight, const std::string& speaker, const std::string& text, Color accentColor) {
    HUD::DrawBossDialogueBanner(windowWidth, windowHeight, speaker, text, accentColor);
}

int GameUI::DrawBorderlandsGateway(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player) {
    return ModalUI::DrawBorderlandsGateway(windowWidth, windowHeight, mousePos, isMousePressed, player);
}
