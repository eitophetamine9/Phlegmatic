#pragma once
#include "raylib.h"

class SoundFX {
public:
    static void Init();
    static void Close();

    static void PlayLightAttack();
    static void PlayHeavyAttack();
    static void PlayParrySuccess();
    static void PlayParryMiss();
    static void PlayHitImpact();
    static void PlayTelegraphWarning();
    static void PlayEnemyDeath();

private:
    static bool initialized;
    static Sound sndLightAttack;
    static Sound sndHeavyAttack;
    static Sound sndParrySuccess;
    static Sound sndParryMiss;
    static Sound sndHitImpact;
    static Sound sndTelegraph;
    static Sound sndEnemyDeath;

    static Sound GenerateTone(float startFreq, float endFreq, float duration, float volume, int waveType);
};
