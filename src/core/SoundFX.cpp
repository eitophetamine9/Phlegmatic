#include "SoundFX.hpp"
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool SoundFX::initialized = false;
Sound SoundFX::sndLightAttack = { 0 };
Sound SoundFX::sndHeavyAttack = { 0 };
Sound SoundFX::sndParrySuccess = { 0 };
Sound SoundFX::sndParryMiss = { 0 };
Sound SoundFX::sndHitImpact = { 0 };
Sound SoundFX::sndTelegraph = { 0 };
Sound SoundFX::sndEnemyDeath = { 0 };
Sound SoundFX::sndDash = { 0 };
Sound SoundFX::sndProjectileShoot = { 0 };
Sound SoundFX::sndProjectileReflect = { 0 };
Sound SoundFX::sndEquipWeapon = { 0 };
Sound SoundFX::sndButtonClick = { 0 };
Sound SoundFX::sndCutsceneChime = { 0 };
Sound SoundFX::sndLoreDiscovery = { 0 };
Sound SoundFX::sndCrusaderClash = { 0 };
Sound SoundFX::sndTruthSeekerResonance = { 0 };
Sound SoundFX::sndGateOpen = { 0 };

Sound SoundFX::GenerateTone(float startFreq, float endFreq, float duration, float volume, int waveType) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float phase = 0.0f;
    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;
        float currentFreq = startFreq + (endFreq - startFreq) * t;
        float dt = currentFreq / (float)sampleRate;
        phase += dt;
        if (phase > 1.0f) phase -= 1.0f;

        float envelope = (1.0f - t);
        if (waveType == 2) envelope = envelope * envelope;

        float sampleVal = 0.0f;
        if (waveType == 0) { // Sine wave
            sampleVal = sinf((float)(phase * 2.0 * M_PI));
        } else if (waveType == 1) { // Triangle / Saw
            sampleVal = (phase < 0.5f) ? (4.0f * phase - 1.0f) : (3.0f - 4.0f * phase);
        } else if (waveType == 2) { // Metallic chime with overtone
            sampleVal = 0.6f * sinf((float)(phase * 2.0 * M_PI)) + 0.4f * sinf((float)(phase * 5.4 * M_PI));
        } else if (waveType == 3) { // White noise / swoosh
            sampleVal = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        }

        samples[i] = (short)(sampleVal * envelope * volume * 32767.0f);
    }

    Wave wave = { 0 };
    wave.frameCount = totalSamples;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 16;
    wave.channels = 1;
    wave.data = samples;

    Sound snd = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return snd;
}

void SoundFX::Init() {
    if (initialized) return;
    InitAudioDevice();

    sndLightAttack = GenerateTone(350.0f, 180.0f, 0.12f, 0.35f, 3);
    sndHeavyAttack = GenerateTone(180.0f, 70.0f, 0.22f, 0.50f, 3);
    sndParrySuccess = GenerateTone(880.0f, 1320.0f, 0.35f, 0.65f, 2);
    sndParryMiss = GenerateTone(220.0f, 140.0f, 0.15f, 0.30f, 0);
    sndHitImpact = GenerateTone(240.0f, 80.0f, 0.18f, 0.55f, 1);
    sndTelegraph = GenerateTone(600.0f, 750.0f, 0.09f, 0.28f, 0);
    sndEnemyDeath = GenerateTone(150.0f, 40.0f, 0.40f, 0.60f, 1);
    sndDash = GenerateTone(450.0f, 200.0f, 0.14f, 0.40f, 3);
    sndProjectileShoot = GenerateTone(500.0f, 900.0f, 0.11f, 0.35f, 1);
    sndProjectileReflect = GenerateTone(1200.0f, 1800.0f, 0.25f, 0.70f, 2);
    sndEquipWeapon = GenerateTone(300.0f, 600.0f, 0.18f, 0.50f, 2);
    sndButtonClick = GenerateTone(520.0f, 780.0f, 0.08f, 0.35f, 0);
    sndCutsceneChime = GenerateTone(520.0f, 780.0f, 0.45f, 0.45f, 2);
    sndLoreDiscovery = GenerateTone(440.0f, 880.0f, 0.50f, 0.55f, 2);
    sndCrusaderClash = GenerateTone(160.0f, 60.0f, 0.28f, 0.65f, 3);
    sndTruthSeekerResonance = GenerateTone(260.0f, 680.0f, 0.65f, 0.60f, 2);
    sndGateOpen = GenerateTone(95.0f, 40.0f, 0.70f, 0.55f, 1);

    initialized = true;
}

void SoundFX::Close() {
    if (!initialized) return;
    UnloadSound(sndLightAttack);
    UnloadSound(sndHeavyAttack);
    UnloadSound(sndParrySuccess);
    UnloadSound(sndParryMiss);
    UnloadSound(sndHitImpact);
    UnloadSound(sndTelegraph);
    UnloadSound(sndEnemyDeath);
    UnloadSound(sndDash);
    UnloadSound(sndProjectileShoot);
    UnloadSound(sndProjectileReflect);
    UnloadSound(sndEquipWeapon);
    UnloadSound(sndButtonClick);
    UnloadSound(sndCutsceneChime);
    UnloadSound(sndLoreDiscovery);
    UnloadSound(sndCrusaderClash);
    UnloadSound(sndTruthSeekerResonance);
    UnloadSound(sndGateOpen);

    CloseAudioDevice();
    initialized = false;
}

void SoundFX::PlayLightAttack() { if (initialized) PlaySound(sndLightAttack); }
void SoundFX::PlayHeavyAttack() { if (initialized) PlaySound(sndHeavyAttack); }
void SoundFX::PlayParrySuccess() { if (initialized) PlaySound(sndParrySuccess); }
void SoundFX::PlayParryMiss() { if (initialized) PlaySound(sndParryMiss); }
void SoundFX::PlayHitImpact() { if (initialized) PlaySound(sndHitImpact); }
void SoundFX::PlayTelegraphWarning() { if (initialized) PlaySound(sndTelegraph); }
void SoundFX::PlayEnemyDeath() { if (initialized) PlaySound(sndEnemyDeath); }
void SoundFX::PlayDash() { if (initialized) PlaySound(sndDash); }
void SoundFX::PlayProjectileShoot() { if (initialized) PlaySound(sndProjectileShoot); }
void SoundFX::PlayProjectileReflect() { if (initialized) PlaySound(sndProjectileReflect); }
void SoundFX::PlayEquipWeapon() { if (initialized) PlaySound(sndEquipWeapon); }
void SoundFX::PlayButtonClick() { if (initialized) PlaySound(sndButtonClick); }
void SoundFX::PlayCutsceneChime() { if (initialized) PlaySound(sndCutsceneChime); }
void SoundFX::PlayLoreDiscovery() { if (initialized) PlaySound(sndLoreDiscovery); }
void SoundFX::PlayCrusaderClash() { if (initialized) PlaySound(sndCrusaderClash); }
void SoundFX::PlayTruthSeekerResonance() { if (initialized) PlaySound(sndTruthSeekerResonance); }
void SoundFX::PlayGateOpen() { if (initialized) PlaySound(sndGateOpen); }
