#pragma once
#include "raylib.h"
#include <string>

class Player;

class ModalUI {
public:
    static bool DrawControls(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed);
    static bool DrawInventory(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, Player& player, int& activeTab, int& selectedFragmentIndex);
    static int DrawZilDefeat(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool canRetry);
    static int DrawBorderlandsGateway(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, const Player& player);
};
