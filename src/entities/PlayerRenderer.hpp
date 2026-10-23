#pragma once
#include "raylib.h"

class Player;

class PlayerRenderer {
public:
    static void Draw(const Player& player);
};
