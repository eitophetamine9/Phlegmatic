#pragma once
#include "raylib.h"

class TitleUI {
public:
    // Returns 0 = None, 1 = Play Campaign, 2 = Controls, 3 = Test Arena
    static int Draw(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool isArenaUnlocked = false);
};
