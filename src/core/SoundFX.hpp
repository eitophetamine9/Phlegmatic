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
    static void PlayDash();
    static void PlayProjectileShoot();
    static void PlayProjectileReflect();
    static void PlayEquipWeapon();
    static void PlayButtonClick();
    static void PlayCutsceneChime();
    static void PlayLoreDiscovery();
    static void PlayCrusaderClash();
    static void PlayTruthSeekerResonance();
    static void PlayGateOpen();
    static void PlayHeal();
    static void PlayCoinPickup();

private:
    static bool initialized;
    static Sound sndLightAttack;
    static Sound sndHeavyAttack;
    static Sound sndParrySuccess;
    static Sound sndParryMiss;
    static Sound sndHitImpact;
    static Sound sndTelegraph;
    static Sound sndEnemyDeath;
    static Sound sndDash;
    static Sound sndProjectileShoot;
    static Sound sndProjectileReflect;
    static Sound sndEquipWeapon;
    static Sound sndButtonClick;
    static Sound sndCutsceneChime;
    static Sound sndLoreDiscovery;
    static Sound sndCrusaderClash;
    static Sound sndTruthSeekerResonance;
    static Sound sndGateOpen;
    static Sound sndCoinPickup;

    static Sound GenerateTone(float startFreq, float endFreq, float duration, float volume, int waveType);
};
