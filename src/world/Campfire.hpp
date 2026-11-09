#pragma once
#include "raylib.h"
#include "raymath.h"
#include "../entities/Player.hpp"
#include "../core/Particles.hpp"
#include "../core/SoundFX.hpp"
#include "../rendering/FontManager.hpp"
#include <string>
#include <cmath>

struct Campfire {
    int id = 1; // 1 = Ruined Castle, 2 = Crossroads, 3 = Zyphden, 4 = Quarry Depths, 5 = Restless Mist Forest
    std::string name = "Campfire";
    Vector2 position = { 0.0f, 0.0f };

    void Update(Player& player, ParticleSystem& particles) {
        float dist = Vector2Distance(player.GetPosition(), position);
        if (dist <= 90.0f) {
            if (IsKeyPressed(KEY_E)) {
                player.SetHealth(player.GetMaxHealth());
                player.ResetStamina();
                int curPotions = player.GetInventory().GetHealthPotionCount();
                if (curPotions < 5) {
                    player.GetInventory().AddHealthPotions(5 - curPotions);
                }
                player.GetInventory().UnlockCampfire(id);
                particles.SpawnSparks(position, Color{ 255, 200, 80, 255 }, 35, 180.0f);
                particles.AddScreenShake(3.0f, 0.15f);
                SoundFX::PlayHeal();
                std::string msg = "RESTED: Health & Potions Restored! " + name + " Attuned!";
                particles.SpawnFloatingText(player.GetPosition(), msg, Color{ 255, 220, 100, 255 }, 20.0f);
            }
        }
    }

    void Draw(float time, const Player& player) const {
        bool isUnlocked = player.GetInventory().IsCampfireUnlocked(id);
        float dist = Vector2Distance(player.GetPosition(), position);

        // 1. Stone ring
        DrawCircle((int)position.x, (int)position.y + 4, 22.0f, Color{ 35, 40, 48, 220 });
        DrawCircleLines((int)position.x, (int)position.y + 4, 22.0f, Color{ 85, 95, 110, 200 });

        // 2. Burning fire & embers
        float firePulse = (sinf(time * 5.0f) + 1.0f) * 0.5f;
        Color fireGlow = isUnlocked ? Color{ 255, 140, 40, 60 } : Color{ 80, 180, 255, 50 };
        DrawCircleGradient(position, 65.0f + 8.0f * firePulse, fireGlow, Color{ 0, 0, 0, 0 });

        // Criss-cross logs
        DrawLineEx({ position.x - 12.0f, position.y + 8.0f }, { position.x + 12.0f, position.y - 4.0f }, 3.5f, Color{ 62, 42, 28, 255 });
        DrawLineEx({ position.x + 12.0f, position.y + 8.0f }, { position.x - 12.0f, position.y - 4.0f }, 3.5f, Color{ 75, 48, 32, 255 });

        // Flame Core
        float flicker = sinf(time * 12.0f) * 2.5f;
        Vector2 flameTip = { position.x + flicker, position.y - 18.0f - firePulse * 4.0f };
        Color innerFlame = isUnlocked ? Color{ 255, 230, 110, 240 } : Color{ 160, 230, 255, 240 };
        Color outerFlame = isUnlocked ? Color{ 255, 90, 30, 200 } : Color{ 60, 140, 240, 200 };
        DrawTriangle({ position.x - 8.0f, position.y + 4.0f }, { position.x + 8.0f, position.y + 4.0f }, flameTip, outerFlame);
        DrawTriangle({ position.x - 4.0f, position.y + 2.0f }, { position.x + 4.0f, position.y + 2.0f }, { flameTip.x, flameTip.y + 5.0f }, innerFlame);

        // Embers
        float emberY = fmodf(time * 28.0f, 35.0f);
        DrawCircle((int)(position.x + sinf(time * 3.0f) * 8.0f), (int)(position.y - 10.0f - emberY), 1.8f, innerFlame);

        // Floating Prompt
        if (dist <= 130.0f) {
            std::string prompt = isUnlocked ? "[E] REST / [T] STORAGE STASH / [M] FAST TRAVEL"
                                            : "[E] ATTUNE & REST / [T] STORAGE STASH";
            FontManager::DrawGameTextShadow(prompt, (int)position.x - 170, (int)position.y - 45, 11, Color{ 255, 225, 120, 240 });
        }
    }
};
