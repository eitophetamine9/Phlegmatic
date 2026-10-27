#include "UI.hpp"
#include "TitleUI.hpp"
#include "HUD.hpp"
#include "ModalUI.hpp"

int GameUI::DrawTitleScreen(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool isArenaUnlocked, bool& showSlotModal, int& deletingSlotIndex) {
    return TitleUI::Draw(windowWidth, windowHeight, mousePos, isMousePressed, isArenaUnlocked, showSlotModal, deletingSlotIndex);
}

bool GameUI::DrawControlsModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed) {
    return ModalUI::DrawControls(windowWidth, windowHeight, mousePos, isMousePressed);
}

bool GameUI::DrawInventoryModal(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex) {
    return ModalUI::DrawInventory(windowWidth, windowHeight, mousePos, isMousePressed, player, activeTab, selectedFragmentIndex);
}

void GameUI::DrawLobbyHUD(int windowWidth, int windowHeight, const Player& player, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore) {
    HUD::DrawLobby(windowWidth, windowHeight, player, isNearPedestal, nearWeaponName, hasUnreadLore);
}

void GameUI::DrawCombatHUD(int windowWidth, int windowHeight, const Player& player, int currentWave, int totalWaves, bool hasUnreadLore) {
    HUD::DrawCombat(windowWidth, windowHeight, player, currentWave, totalWaves, hasUnreadLore);
}

void GameUI::DrawCampaignHUD(int windowWidth, int windowHeight, const Player& player, const std::string& locationName, bool hasUnreadLore) {
    HUD::DrawCampaignHUD(windowWidth, windowHeight, player, locationName, hasUnreadLore);
}

void GameUI::DrawLobbyHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isNearPedestal, const std::string& nearWeaponName, bool hasUnreadLore) {
    HUD::DrawLobby(windowWidth, windowHeight, hp, maxHp, stamina, maxStamina, weapon, dashCooldown, parryCooldown, heavyCooldown, isNearPedestal, nearWeaponName, hasUnreadLore);
}

void GameUI::DrawCombatHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, int currentWave, int totalWaves, bool hasUnreadLore) {
    HUD::DrawCombat(windowWidth, windowHeight, hp, maxHp, stamina, maxStamina, weapon, dashCooldown, parryCooldown, heavyCooldown, isParrying, hasCounterBuff, currentWave, totalWaves, hasUnreadLore);
}

void GameUI::DrawCampaignHUD(int windowWidth, int windowHeight, float hp, float maxHp, float stamina, float maxStamina, WeaponType weapon, float dashCooldown, float parryCooldown, float heavyCooldown, bool isParrying, bool hasCounterBuff, const std::string& locationName, bool hasUnreadLore) {
    HUD::DrawCampaignHUD(windowWidth, windowHeight, hp, maxHp, stamina, maxStamina, weapon, dashCooldown, parryCooldown, heavyCooldown, isParrying, hasCounterBuff, locationName, hasUnreadLore);
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

bool GameUI::DrawNPCDialogueBox(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const std::string& speaker, const std::vector<std::string>& pages, int& currentPage, Color accentColor) {
    return HUD::DrawNPCDialogueBox(windowWidth, windowHeight, mousePos, isMousePressed, speaker, pages, currentPage, accentColor);
}

int GameUI::DrawBorderlandsGateway(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player) {
    return ModalUI::DrawBorderlandsGateway(windowWidth, windowHeight, mousePos, isMousePressed, player);
}

bool GameUI::DrawZyphdenTransition(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, float dt) {
    return ModalUI::DrawZyphdenTransition(windowWidth, windowHeight, mousePos, isMousePressed, dt);
}

