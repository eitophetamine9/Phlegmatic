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
Sound SoundFX::sndCoinPickup = { 0 };
Sound SoundFX::sndTranceActivate = { 0 };

static inline float SoftClip(float x) {
    return x / (1.0f + fabsf(x));
}

// Procedural sound generators producing authentic, visceral medieval audio
static Sound CreateSoundFromBuffer(short* samples, int totalSamples, int sampleRate = 44100) {
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

// 1. Blade Slash: Aerodynamic cold-steel air cut with swept resonant filtering & edge shimmer
static Sound GenerateBladeSlash(float duration = 0.16f, float volume = 0.55f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float bp1 = 0.0f, bp2 = 0.0f;
    float phase = 0.0f;
    float phaseEdge = 0.0f;

    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;

        // Sweeping aerodynamic turbulence: center freq ascends from 450Hz to 2400Hz then falls to 320Hz
        float centerFreq = 400.0f + 2000.0f * sinf(t * (float)M_PI);
        float q = 0.22f; // Bandwidth factor

        // White noise excitation
        float noise = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;

        // Two-pole bandpass filter
        bp1 += (noise - bp1) * (centerFreq / (float)sampleRate) * 2.2f;
        bp2 += (bp1 - bp2) * (centerFreq / (float)sampleRate) * 2.2f;
        float filteredAir = (bp1 - bp2) * 2.6f;

        // Metallic blade edge overtone (~1100Hz whistle during peak arc)
        float edgeFreq = 950.0f + 650.0f * sinf(t * (float)M_PI);
        phaseEdge += edgeFreq / (float)sampleRate;
        if (phaseEdge > 1.0f) phaseEdge -= 1.0f;
        float edgeTone = sinf((float)(phaseEdge * 2.0 * M_PI)) * expf(-t * 8.0f);

        // Smooth swing envelope with rapid attack and clean tail
        float envelope = sinf(t * (float)M_PI);
        envelope = envelope * envelope; // quadratic smoothing

        float sampleVal = SoftClip(filteredAir * 0.75f + edgeTone * 0.25f);
        samples[i] = (short)(sampleVal * envelope * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

// 2. Heavy Blunt Smash: Deep physical shockwave, crushing impact transient, and sub-bass resonance
static Sound GenerateBluntSmash(float duration = 0.32f, float volume = 0.70f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float phaseBass = 0.0f;
    float phaseRumble = 0.0f;
    float filterCrunch = 0.0f;

    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;

        // Pitch drops from 160Hz punch down to 42Hz deep earth tremor
        float bassFreq = 42.0f + 118.0f * expf(-t * 16.0f);
        phaseBass += bassFreq / (float)sampleRate;
        if (phaseBass > 1.0f) phaseBass -= 1.0f;

        phaseRumble += 38.0f / (float)sampleRate;
        if (phaseRumble > 1.0f) phaseRumble -= 1.0f;

        // Sharp crushing stone/metal contact transient (first 18ms)
        float contactNoise = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        filterCrunch += (contactNoise - filterCrunch) * 0.35f;
        float crunch = filterCrunch * expf(-t * 28.0f);

        // Sub-bass body
        float bass = sinf((float)(phaseBass * 2.0 * M_PI));
        float rumble = sinf((float)(phaseRumble * 2.0 * M_PI)) * 0.35f;

        // Exponential decay envelope
        float env = expf(-t * 7.5f);

        float raw = (crunch * 0.55f + bass * 0.75f + rumble * 0.30f);
        float sampleVal = SoftClip(raw * 1.4f); // Saturated punch
        samples[i] = (short)(sampleVal * env * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

// 3. Steel Parry Clash: Authentic tempered high-carbon blade clash with sustained inharmonic ring
static Sound GenerateParryClash(float duration = 0.40f, float volume = 0.75f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    // Authentic inharmonic blade resonance modes (fundamental & non-integer steel modes)
    float p1 = 0.0f, p2 = 0.0f, p3 = 0.0f, p4 = 0.0f, p5 = 0.0f;
    const float f1 = 780.0f;
    const float f2 = 1240.0f;
    const float f3 = 1890.0f;
    const float f4 = 2950.0f;
    const float f5 = 4400.0f;

    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;

        p1 += f1 / (float)sampleRate; if (p1 > 1.0f) p1 -= 1.0f;
        p2 += f2 / (float)sampleRate; if (p2 > 1.0f) p2 -= 1.0f;
        p3 += f3 / (float)sampleRate; if (p3 > 1.0f) p3 -= 1.0f;
        p4 += f4 / (float)sampleRate; if (p4 > 1.0f) p4 -= 1.0f;
        p5 += f5 / (float)sampleRate; if (p5 > 1.0f) p5 -= 1.0f;

        // Initial violent metal bite (< 6ms)
        float sparkBurst = (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * expf(-t * 80.0f);

        // Ringing bell-like blade harmonics
        float ring = 0.35f * sinf((float)(p1 * 2.0 * M_PI)) * expf(-t * 6.5f)
                   + 0.28f * sinf((float)(p2 * 2.0 * M_PI)) * expf(-t * 8.0f)
                   + 0.22f * sinf((float)(p3 * 2.0 * M_PI)) * expf(-t * 11.0f)
                   + 0.15f * sinf((float)(p4 * 2.0 * M_PI)) * expf(-t * 15.0f)
                   + 0.10f * sinf((float)(p5 * 2.0 * M_PI)) * expf(-t * 22.0f);

        float sampleVal = SoftClip(sparkBurst * 0.70f + ring * 1.10f);
        samples[i] = (short)(sampleVal * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

// 4. Flesh & Armor Hit Impact: Solid physical thwack with low chest thump and armor rattle
static Sound GenerateHitImpact(float duration = 0.18f, float volume = 0.65f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float phaseThump = 0.0f;
    float filterRattle = 0.0f;

    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;

        // 95Hz down to 48Hz chest-thump
        float freq = 48.0f + 47.0f * expf(-t * 24.0f);
        phaseThump += freq / (float)sampleRate;
        if (phaseThump > 1.0f) phaseThump -= 1.0f;

        // Snappy initial slap noise
        float rawNoise = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        filterRattle += (rawNoise - filterRattle) * 0.28f;
        float slap = filterRattle * expf(-t * 36.0f);

        float thump = sinf((float)(phaseThump * 2.0 * M_PI)) * expf(-t * 14.0f);

        float sampleVal = SoftClip(slap * 0.60f + thump * 0.85f);
        samples[i] = (short)(sampleVal * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

// 5. Dash / Evasion: Soft fluttering fabric swoosh & gravel shuffle
static Sound GenerateDashSwoosh(float duration = 0.16f, float volume = 0.40f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float filterState = 0.0f;
    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;

        float noise = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        filterState += (noise - filterState) * 0.16f; // low-pass fabric rustle

        float env = sinf(t * (float)M_PI);
        env = env * env;

        float sampleVal = SoftClip(filterState * 1.2f);
        samples[i] = (short)(sampleVal * env * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

// 6. Medieval Cathedral Abbey Bell / Ancient Relic Chime
static Sound GenerateAbbeyChime(float rootFreq = 330.0f, float duration = 0.85f, float volume = 0.55f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float p1 = 0.0f, p2 = 0.0f, p3 = 0.0f;
    float f1 = rootFreq;
    float f2 = rootFreq * 1.503f; // Tierce (minor third)
    float f3 = rootFreq * 2.76f;  // Fifth octave

    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;

        p1 += f1 / (float)sampleRate; if (p1 > 1.0f) p1 -= 1.0f;
        p2 += f2 / (float)sampleRate; if (p2 > 1.0f) p2 -= 1.0f;
        p3 += f3 / (float)sampleRate; if (p3 > 1.0f) p3 -= 1.0f;

        float bell = 0.50f * sinf((float)(p1 * 2.0 * M_PI)) * expf(-t * 2.5f)
                   + 0.32f * sinf((float)(p2 * 2.0 * M_PI)) * expf(-t * 3.8f)
                   + 0.18f * sinf((float)(p3 * 2.0 * M_PI)) * expf(-t * 5.5f);

        samples[i] = (short)(SoftClip(bell) * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

// 7. War Horn Warning: Resonant brass reed drone with natural swelling attack and vibrato
static Sound GenerateWarHorn(float rootFreq = 110.0f, float duration = 0.35f, float volume = 0.45f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float phase = 0.0f;
    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;

        float vibrato = 1.0f + 0.035f * sinf((float)(t * 14.0 * M_PI));
        phase += (rootFreq * vibrato) / (float)sampleRate;
        if (phase > 1.0f) phase -= 1.0f;

        float env = (t < 0.22f) ? (t / 0.22f) : (1.0f - (t - 0.22f) / 0.78f);
        float horn = 0.52f * sinf((float)(phase * 2.0 * M_PI))
                   + 0.30f * sinf((float)(phase * 4.0 * M_PI))
                   + 0.18f * sinf((float)(phase * 6.0 * M_PI));

        samples[i] = (short)(SoftClip(horn * 1.15f) * env * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

// 8. Portcullis Gate: Deep stone dragging & iron chains
static Sound GenerateGateRumble(float duration = 0.90f, float volume = 0.60f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float phase = 0.0f;
    float filter = 0.0f;
    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;

        phase += 48.0f / (float)sampleRate;
        if (phase > 1.0f) phase -= 1.0f;

        float n = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        filter += (n - filter) * 0.14f;

        float stone = sinf((float)(phase * 2.0 * M_PI)) * 0.60f + filter * 0.40f;
        float env = (1.0f - t);

        samples[i] = (short)(SoftClip(stone) * env * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

static Sound GenerateSilverCoinClink(float duration = 0.20f, float volume = 0.55f) {
    const int sampleRate = 44100;
    int totalSamples = (int)(sampleRate * duration);
    if (totalSamples <= 0) totalSamples = 1;

    short* samples = (short*)malloc(totalSamples * sizeof(short));
    if (!samples) return Sound{ 0 };

    float phase1 = 0.0f;
    float phase2 = 0.0f;
    float f1 = 2093.0f; // C7
    float f2 = 3136.0f; // G7

    for (int i = 0; i < totalSamples; i++) {
        float t = (float)i / (float)totalSamples;
        phase1 += f1 / (float)sampleRate;
        if (phase1 > 1.0f) phase1 -= 1.0f;
        phase2 += f2 / (float)sampleRate;
        if (phase2 > 1.0f) phase2 -= 1.0f;

        float sig = sinf((float)(phase1 * 2.0 * M_PI)) * 0.65f + sinf((float)(phase2 * 2.0 * M_PI)) * 0.35f;
        float env = expf(-14.0f * t);

        samples[i] = (short)(SoftClip(sig * env) * volume * 32767.0f);
    }

    return CreateSoundFromBuffer(samples, totalSamples, sampleRate);
}

void SoundFX::Init() {
    if (initialized) return;
    InitAudioDevice();

    // 1. Blade slashes (crisp cold-steel whoosh)
    sndLightAttack = GenerateBladeSlash(0.14f, 0.52f);
    sndHeavyAttack = GenerateBluntSmash(0.28f, 0.72f); // Deep seismic impact and crunch

    // 2. Tempered sword clashes & deflections
    sndParrySuccess = GenerateParryClash(0.38f, 0.78f);
    sndCrusaderClash = GenerateParryClash(0.48f, 0.82f);
    sndProjectileReflect = GenerateParryClash(0.30f, 0.70f);

    // 3. Fleshy/armored hit impacts and thuds
    sndHitImpact = GenerateHitImpact(0.17f, 0.68f);
    sndEnemyDeath = GenerateBluntSmash(0.40f, 0.70f);
    sndParryMiss = GenerateBladeSlash(0.12f, 0.35f);

    // 4. Movement swooshes (cloth/leather)
    sndDash = GenerateDashSwoosh(0.14f, 0.42f);

    // 5. Arrow / Projectile whoosh
    sndProjectileShoot = GenerateBladeSlash(0.11f, 0.45f);

    // 6. Medieval war horn warning
    sndTelegraph = GenerateWarHorn(110.0f, 0.32f, 0.48f);

    // 7. Scabbard draw & UI
    sndEquipWeapon = GenerateBladeSlash(0.22f, 0.48f);
    sndButtonClick = GenerateDashSwoosh(0.06f, 0.32f);

    // 8. Chimes & holy artifacts
    sndCutsceneChime = GenerateAbbeyChime(330.0f, 0.75f, 0.55f);
    sndLoreDiscovery = GenerateAbbeyChime(440.0f, 0.70f, 0.60f);
    sndTruthSeekerResonance = GenerateAbbeyChime(261.6f, 0.95f, 0.70f);

    // 9. Gate open & currency pickup & trance
    sndGateOpen = GenerateGateRumble(0.95f, 0.65f);
    sndCoinPickup = GenerateSilverCoinClink(0.18f, 0.55f);
    sndTranceActivate = GenerateAbbeyChime(523.25f, 1.20f, 0.70f);

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
    UnloadSound(sndCoinPickup);
    UnloadSound(sndTranceActivate);

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
void SoundFX::PlayHeal() { if (initialized) PlaySound(sndCutsceneChime); }
void SoundFX::PlayCoinPickup() { if (initialized) PlaySound(sndCoinPickup); }
void SoundFX::PlayTranceActivate() { if (initialized) PlaySound(sndTranceActivate); }
