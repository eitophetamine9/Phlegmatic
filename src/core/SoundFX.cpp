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
    float phase2 = 0.0f;
    float phase3 = 0.0f;
    float filterState = 0.0f;

    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;
        float currentFreq = startFreq + (endFreq - startFreq) * t;
        float dt = currentFreq / (float)sampleRate;
        phase += dt;
        if (phase > 1.0f) phase -= 1.0f;

        float sampleVal = 0.0f;
        float envelope = 1.0f - t;

        if (waveType == 0) {
            // Medieval Cathedral Abbey Bell / Ancient Relic Chime:
            // Authentic bronze bell harmonics (fundamental, minor third tierce, fifth, octave)
            envelope = (1.0f - t) * (1.0f - t);
            phase2 += (currentFreq * 1.503f) / (float)sampleRate;
            if (phase2 > 1.0f) phase2 -= 1.0f;
            phase3 += (currentFreq * 2.76f) / (float)sampleRate;
            if (phase3 > 1.0f) phase3 -= 1.0f;

            sampleVal = 0.50f * sinf((float)(phase * 2.0 * M_PI))
                      + 0.32f * sinf((float)(phase2 * 2.0 * M_PI))
                      + 0.18f * sinf((float)(phase3 * 2.0 * M_PI));
        } else if (waveType == 1) {
            // Steel Blade Air Whoosh / Scabbard Draw:
            // Filtered aerodynamic air displacement with subtle steel overtone
            float rawNoise = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
            filterState += (rawNoise - filterState) * 0.25f;
            float steelEdge = sinf((float)(phase * 2.0 * M_PI));
            envelope = sinf(t * (float)M_PI); // smooth rise and fall
            sampleVal = 0.65f * filterState + 0.35f * steelEdge;
        } else if (waveType == 2) {
            // Steel Broadsword & Anvil Blade Clash:
            // Sharp metallic impact transient with ringing steel overtone resonance
            float metalNoise = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * expf(-t * 28.0f);
            phase2 += (currentFreq * 2.42f) / (float)sampleRate;
            if (phase2 > 1.0f) phase2 -= 1.0f;
            phase3 += (currentFreq * 4.78f) / (float)sampleRate;
            if (phase3 > 1.0f) phase3 -= 1.0f;

            envelope = expf(-t * 8.5f);
            sampleVal = 0.35f * metalNoise
                      + 0.40f * sinf((float)(phase * 2.0 * M_PI))
                      + 0.25f * sinf((float)(phase2 * 2.0 * M_PI))
                      + 0.15f * sinf((float)(phase3 * 2.0 * M_PI));
        } else if (waveType == 3) {
            // Heavy Armor Impact / Chainmail & Flesh Thud:
            // Deep low frequency punch with short metallic rattle
            envelope = expf(-t * 14.0f);
            float punch = sinf((float)(phase * 2.0 * M_PI));
            float rattle = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * expf(-t * 22.0f);
            sampleVal = 0.75f * punch + 0.25f * rattle;
        } else if (waveType == 4) {
            // Wooden Crossbow / Bowstring Snap:
            // Fast attack wood transient + brief string tension twang
            envelope = expf(-t * 18.0f);
            float twang = sinf((float)(phase * 2.0 * M_PI));
            float snap = (t < 0.08f) ? (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) : 0.0f;
            sampleVal = 0.60f * snap + 0.40f * twang;
        } else if (waveType == 5) {
            // Ominous Medieval War Horn / Battle Drone:
            // Rich brassy reed harmonics with subtle vibrato (replaces sci-fi beep!)
            float vibrato = 1.0f + 0.04f * sinf((float)(t * 16.0 * M_PI));
            float hornPhase = phase * vibrato;
            envelope = (t < 0.2f) ? (t / 0.2f) : (1.0f - (t - 0.2f) / 0.8f);
            sampleVal = 0.50f * sinf((float)(hornPhase * 2.0 * M_PI))
                      + 0.30f * sinf((float)(hornPhase * 4.0 * M_PI))
                      + 0.20f * sinf((float)(hornPhase * 6.0 * M_PI));
        } else if (waveType == 6) {
            // Portcullis Iron Chains & Stone Grinding:
            float grind = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
            filterState += (grind - filterState) * 0.15f;
            float lowRumble = sinf((float)(phase * 2.0 * M_PI));
            envelope = (1.0f - t);
            sampleVal = 0.60f * lowRumble + 0.40f * filterState;
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

    // 1. Blade slashes (steel blade air displacement)
    sndLightAttack = GenerateTone(220.0f, 150.0f, 0.15f, 0.38f, 1);
    sndHeavyAttack = GenerateTone(120.0f, 75.0f, 0.26f, 0.50f, 1);

    // 2. Iron sword & anvil clash (authentic medieval clash!)
    sndParrySuccess = GenerateTone(520.0f, 440.0f, 0.35f, 0.65f, 2);
    sndCrusaderClash = GenerateTone(420.0f, 340.0f, 0.40f, 0.70f, 2);
    sndProjectileReflect = GenerateTone(640.0f, 560.0f, 0.24f, 0.60f, 2);

    // 3. Armor hits, thuds, collapses
    sndHitImpact = GenerateTone(110.0f, 55.0f, 0.18f, 0.55f, 3);
    sndEnemyDeath = GenerateTone(85.0f, 40.0f, 0.45f, 0.60f, 3);
    sndParryMiss = GenerateTone(150.0f, 110.0f, 0.12f, 0.25f, 1);

    // 4. Evasion (gravel & cloak)
    sndDash = GenerateTone(190.0f, 110.0f, 0.14f, 0.35f, 1);

    // 5. Bowstring / Crossbow release (authentic wooden snap, NOT laser zap!)
    sndProjectileShoot = GenerateTone(320.0f, 140.0f, 0.13f, 0.42f, 4);

    // 6. Medieval war horn warning (replaces retro sci-fi beep!)
    sndTelegraph = GenerateTone(110.0f, 105.0f, 0.28f, 0.38f, 5);

    // 7. Equipment & scabbard
    sndEquipWeapon = GenerateTone(280.0f, 210.0f, 0.20f, 0.45f, 1);
    sndButtonClick = GenerateTone(240.0f, 180.0f, 0.08f, 0.30f, 4);

    // 8. Ancient abbey & cathedral chimes (rich medieval church bell harmonics)
    sndCutsceneChime = GenerateTone(330.0f, 330.0f, 0.75f, 0.50f, 0);
    sndLoreDiscovery = GenerateTone(440.0f, 440.0f, 0.70f, 0.55f, 0);
    sndTruthSeekerResonance = GenerateTone(277.0f, 277.0f, 0.90f, 0.65f, 0);

    // 9. Heavy iron portcullis chains & stone rumble
    sndGateOpen = GenerateTone(65.0f, 35.0f, 0.85f, 0.60f, 6);

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
