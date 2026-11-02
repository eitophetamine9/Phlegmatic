#pragma once
#include "raylib.h"

class TitleUI {
public:
    // Returns:
    // 0 = None / Stay on Title
    // 1 = Controls Modal
    // 2 = Test Arena
    // 10 + N = Load Game Slot N (11 = Slot 1, 12 = Slot 2, 13 = Slot 3)
    // 20 + N = New Game Slot N (21 = Slot 1, 22 = Slot 2, 23 = Slot 3)
    static int Draw(int windowWidth, int windowHeight, Vector2 mousePos, bool isMousePressed, bool isArenaUnlocked, bool& showSlotModal, int& deletingSlotIndex);
};
