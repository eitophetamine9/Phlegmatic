#pragma once
#include "raylib.h"

class Enemy;

class EnemyRenderer {
public:
    static void DrawTelegraph(const Enemy& enemy);
    static void Draw(const Enemy& enemy);
};
