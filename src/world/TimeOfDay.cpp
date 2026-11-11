#include "TimeOfDay.hpp"
#include <cmath>
#include <cstdio>
#include <algorithm>

static const char* kPointLightFragShader = R"(
#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

void main() {
    vec2 d = (fragTexCoord - vec2(0.5)) * 2.0;
    float dist = length(d);
    float edgeFade = 1.0 - smoothstep(0.0, 1.0, dist);
    float invSq = 1.0 / (1.0 + 4.8 * dist * dist);
    float atten = clamp(invSq * edgeFade * edgeFade, 0.0, 1.0) * fragColor.a;
    vec3 warmCore = mix(fragColor.rgb, vec3(1.0, 0.97, 0.90), clamp((1.0 - dist * 2.1) * 0.28, 0.0, 0.28));
    finalColor = vec4(warmCore * atten * 1.15, 1.0);
}
)";

static const char* kCompositeFragShader = R"(
#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform float uDarkness;
out vec4 finalColor;

void main() {
    vec3 light = texture(texture0, fragTexCoord).rgb;
    // Soft shoulder rolloff so overlapping fire/torch pools glow naturally without harsh clipping
    vec3 mapped = light / (vec3(1.0) + max(light - vec3(0.85), vec3(0.0)) * 0.55);
    // Purkinje blue-shift on dark night ambient regions while keeping lit areas rich & vibrant
    float lum = dot(mapped, vec3(0.299, 0.587, 0.114));
    float coolMask = clamp((0.36 - lum) * 2.4, 0.0, 1.0) * uDarkness;
    vec3 purkinjeTint = vec3(0.91, 0.97, 1.13);
    vec3 result = mix(mapped, mapped * purkinjeTint, coolMask * 0.45);
    finalColor = vec4(clamp(result, 0.05, 1.12), 1.0);
}
)";

static Color LerpColorCosine(Color c1, Color c2, float linearT) {
    float t = fmaxf(0.0f, fminf(1.0f, linearT));
    float smoothT = 0.5f - 0.5f * cosf(PI * t);
    return Color{
        (unsigned char)(c1.r + (c2.r - c1.r) * smoothT),
        (unsigned char)(c1.g + (c2.g - c1.g) * smoothT),
        (unsigned char)(c1.b + (c2.b - c1.b) * smoothT),
        255
    };
}

TimeOfDaySystem::TimeOfDaySystem()
    : worldHours(8.0f), timeSpeed(24.0f / 360.0f) { // 360 seconds for full 24h cycle
}

TimeOfDaySystem::~TimeOfDaySystem() {
    if (IsWindowReady()) {
        UnloadResources();
    }
}

void TimeOfDaySystem::InitResources(int width, int height) {
    if (width <= 0 || height <= 0) return;

    if (!shadersInitialized) {
        shadersInitialized = true;
        pointLightShader = LoadShaderFromMemory(nullptr, kPointLightFragShader);
        compositeShader = LoadShaderFromMemory(nullptr, kCompositeFragShader);
        if (IsShaderValid(compositeShader)) {
            compositeDarknessLoc = GetShaderLocation(compositeShader, "uDarkness");
        }
    }

    if (!texturesInitialized) {
        texturesInitialized = true;
        Image whiteImg = GenImageColor(2, 2, WHITE);
        whiteQuadTex = LoadTextureFromImage(whiteImg);
        UnloadImage(whiteImg);
        SetTextureFilter(whiteQuadTex, TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(whiteQuadTex, TEXTURE_WRAP_CLAMP);

        Image gradImg = GenImageGradientRadial(128, 128, 0.0f, WHITE, BLANK);
        radialFalloffTex = LoadTextureFromImage(gradImg);
        UnloadImage(gradImg);
        SetTextureFilter(radialFalloffTex, TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(radialFalloffTex, TEXTURE_WRAP_CLAMP);
    }

    if (!lightMapInitialized || lightMap.texture.width != width || lightMap.texture.height != height) {
        if (lightMapInitialized) {
            UnloadRenderTexture(lightMap);
        }
        lightMap = LoadRenderTexture(width, height);
        SetTextureFilter(lightMap.texture, TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(lightMap.texture, TEXTURE_WRAP_CLAMP);
        lightMapInitialized = true;
    }
}

void TimeOfDaySystem::UnloadResources() {
    if (lightMapInitialized) {
        UnloadRenderTexture(lightMap);
        lightMap = { 0 };
        lightMapInitialized = false;
    }
    if (shadersInitialized) {
        if (IsShaderValid(pointLightShader)) UnloadShader(pointLightShader);
        if (IsShaderValid(compositeShader)) UnloadShader(compositeShader);
        pointLightShader = { 0 };
        compositeShader = { 0 };
        compositeDarknessLoc = -1;
        shadersInitialized = false;
    }
    if (texturesInitialized) {
        if (whiteQuadTex.id != 0) UnloadTexture(whiteQuadTex);
        if (radialFalloffTex.id != 0) UnloadTexture(radialFalloffTex);
        whiteQuadTex = { 0 };
        radialFalloffTex = { 0 };
        texturesInitialized = false;
    }
}

void TimeOfDaySystem::Update(float dt) {
    worldHours += timeSpeed * dt;
    if (worldHours >= 24.0f) {
        worldHours = fmodf(worldHours, 24.0f);
    }
}

float TimeOfDaySystem::GetHour() const {
    return floorf(worldHours);
}

float TimeOfDaySystem::GetMinute() const {
    float mins = (worldHours - floorf(worldHours)) * 60.0f;
    return floorf(mins);
}

std::string TimeOfDaySystem::GetFormattedTime() const {
    int h = (int)GetHour();
    int m = (int)GetMinute();
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", h, m);
    return std::string(buf);
}

TimePhase TimeOfDaySystem::GetPhase() const {
    if (worldHours >= 5.0f && worldHours < 8.5f) return TimePhase::DAWN;
    if (worldHours >= 8.5f && worldHours < 17.5f) return TimePhase::DAY;
    if (worldHours >= 17.5f && worldHours < 20.5f) return TimePhase::DUSK;
    return TimePhase::NIGHT;
}

const char* TimeOfDaySystem::GetPhaseName() const {
    switch (GetPhase()) {
        case TimePhase::DAWN: return "DAWN";
        case TimePhase::DAY: return "DAY";
        case TimePhase::DUSK: return "DUSK";
        case TimePhase::NIGHT: return "NIGHT";
    }
    return "UNKNOWN";
}

Color TimeOfDaySystem::GetAmbientColor() const {
    const Color cDay      = { 255, 255, 255, 255 };
    const Color cGoldHour = { 255, 224, 178, 255 };
    const Color cSunset   = { 226, 146, 128, 255 };
    const Color cTwilight = { 122, 104, 152, 255 };
    const Color cNight    = { 44,  58,  102, 255 };
    const Color cPreDawn  = { 148, 166, 206, 255 };
    const Color cSunrise  = { 244, 192, 148, 255 };

    // 08:30 - 17:30 : Clear daylight (pure multiplicative white = no tint)
    if (worldHours >= 8.5f && worldHours < 17.5f) {
        return cDay;
    }
    // 17:30 - 18:45 : Golden Hour
    if (worldHours >= 17.5f && worldHours < 18.75f) {
        return LerpColorCosine(cDay, cGoldHour, (worldHours - 17.5f) / 1.25f);
    }
    // 18:45 - 19:45 : Warm Amber-Rose Sunset
    if (worldHours >= 18.75f && worldHours < 19.75f) {
        return LerpColorCosine(cGoldHour, cSunset, (worldHours - 18.75f) / 1.0f);
    }
    // 19:45 - 20:30 : Dusk Twilight -> Moonlit Indigo
    if (worldHours >= 19.75f && worldHours < 20.5f) {
        Color mid = LerpColorCosine(cSunset, cTwilight, (worldHours - 19.75f) / 0.375f);
        if (worldHours >= 20.125f) {
            return LerpColorCosine(cTwilight, cNight, (worldHours - 20.125f) / 0.375f);
        }
        return mid;
    }
    // 20:30 - 05:00 : Crisp Moonlit Night
    if (worldHours >= 20.5f || worldHours < 5.0f) {
        return cNight;
    }
    // 05:00 - 06:15 : Pre-dawn mist
    if (worldHours >= 5.0f && worldHours < 6.25f) {
        return LerpColorCosine(cNight, cPreDawn, (worldHours - 5.0f) / 1.25f);
    }
    // 06:15 - 07:30 : Warm Peach Sunrise
    if (worldHours >= 6.25f && worldHours < 7.5f) {
        return LerpColorCosine(cPreDawn, cSunrise, (worldHours - 6.25f) / 1.25f);
    }
    // 07:30 - 08:30 : Morning Clarity into Daylight
    return LerpColorCosine(cSunrise, cDay, (worldHours - 7.5f) / 1.0f);
}

float TimeOfDaySystem::GetDarknessLevel() const {
    Color amb = GetAmbientColor();
    float lum = (0.299f * (float)amb.r + 0.587f * (float)amb.g + 0.114f * (float)amb.b) / 255.0f;
    return fmaxf(0.0f, fminf(1.0f, 1.0f - lum));
}

void TimeOfDaySystem::ClearLights() {
    activeLights.clear();
}

void TimeOfDaySystem::RegisterLight(Vector2 pos, float radius, Color color, float intensity) {
    activeLights.push_back({ pos, radius, color, intensity });
}

void TimeOfDaySystem::DrawLighting(
    Camera2D camera,
    Vector2 playerPos,
    const Rectangle& mapBounds,
    const std::vector<Vector2>& torches,
    bool isInterior,
    bool isSubterranean,
    bool isFrostSubterranean
) {
    (void)mapBounds;
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    if (sw <= 0 || sh <= 0) return;

    Color ambColor = GetAmbientColor();
    if (isInterior) {
        // Warm candlelit cozy interior shadow lifted by hearths, furnaces, and braziers
        ambColor = Color{ 208, 178, 150, 255 };
    } else if (isFrostSubterranean) {
        // Deep glacial blue cavern ambiance illuminated by ice crystals & campfire
        ambColor = Color{ 58, 82, 124, 255 };
    } else if (isSubterranean) {
        // Deep subterranean mine ambiance illuminated by mining braziers & elemental altar
        ambColor = Color{ 64, 60, 82, 255 };
    }

    if (ambColor.r >= 253 && ambColor.g >= 253 && ambColor.b >= 253) {
        return;
    }

    InitResources(sw, sh);
    if (!lightMapInitialized) return;

    float lum = (0.299f * (float)ambColor.r + 0.587f * (float)ambColor.g + 0.114f * (float)ambColor.b) / 255.0f;
    float darkness = fmaxf(0.0f, fminf(1.0f, 1.0f - lum));
    float lightWeight = (isInterior || isSubterranean || isFrostSubterranean) ? 0.92f : fminf(1.0f, 0.28f + darkness * 0.92f);
    float time = (float)GetTime();

    bool usePointShader = IsShaderValid(pointLightShader) && whiteQuadTex.id != 0;

    auto DrawPointQuad = [&](Vector2 pos, float radius, Color col, float intensity) {
        float clampedInt = fmaxf(0.0f, fminf(1.0f, intensity));
        if (clampedInt <= 0.005f || radius <= 1.0f) return;

        Rectangle dst = { pos.x - radius, pos.y - radius, radius * 2.0f, radius * 2.0f };
        if (usePointShader) {
            Rectangle src = { 0.0f, 0.0f, (float)whiteQuadTex.width, (float)whiteQuadTex.height };
            Color tint = { col.r, col.g, col.b, (unsigned char)(clampedInt * 255.0f) };
            DrawTexturePro(whiteQuadTex, src, dst, { 0.0f, 0.0f }, 0.0f, tint);
        } else if (radialFalloffTex.id != 0) {
            Rectangle src = { 0.0f, 0.0f, (float)radialFalloffTex.width, (float)radialFalloffTex.height };
            DrawTexturePro(radialFalloffTex, src, dst, { 0.0f, 0.0f }, 0.0f, ColorAlpha(col, clampedInt * 0.85f));
        }
    };

    // 1. Offscreen Multiplicative Lightmap Pass
    BeginTextureMode(lightMap);
    ClearBackground(ambColor);
    BeginMode2D(camera);
    BeginBlendMode(BLEND_ADDITIVE);
    if (usePointShader) {
        BeginShaderMode(pointLightShader);
    }

    // Player warm hip lantern
    if (playerPos.x > -9000.0f) {
        float pFlicker = 1.0f + 0.025f * sinf(time * 7.2f) + 0.012f * sinf(time * 16.4f);
        DrawPointQuad(playerPos, 245.0f * pFlicker, Color{ 255, 224, 158, 255 }, 0.88f * lightWeight);
    }

    // Map wall/brazier torches
    Color torchCol = isFrostSubterranean ? Color{ 120, 215, 255, 255 } : Color{ 255, 168, 72, 255 };
    for (const auto& torch : torches) {
        float tFlicker = 1.0f + 0.045f * sinf(time * 8.5f + torch.x * 0.13f) + 0.020f * sinf(time * 19.0f + torch.y * 0.09f);
        DrawPointQuad(torch, 210.0f * tFlicker, torchCol, 0.86f * lightWeight);
    }

    // Dynamic realm & spell point lights (campfires, altars, portals, crystals, projectiles)
    for (const auto& light : activeLights) {
        float lFlicker = 1.0f + 0.035f * sinf(time * 7.5f + light.position.x * 0.07f + light.position.y * 0.11f);
        DrawPointQuad(light.position, light.radius * lFlicker, light.color, light.intensity * lightWeight);
    }

    if (usePointShader) {
        EndShaderMode();
    }
    EndBlendMode();
    EndMode2D();
    EndTextureMode();

    // 2. Multiplicative Composite Pass onto Main Scene Buffer
    Rectangle srcFull = { 0.0f, 0.0f, (float)lightMap.texture.width, -(float)lightMap.texture.height };
    Rectangle dstFull = { 0.0f, 0.0f, (float)sw, (float)sh };

    BeginBlendMode(BLEND_MULTIPLIED);
    if (IsShaderValid(compositeShader)) {
        if (compositeDarknessLoc >= 0) {
            SetShaderValue(compositeShader, compositeDarknessLoc, &darkness, SHADER_UNIFORM_FLOAT);
        }
        BeginShaderMode(compositeShader);
        DrawTexturePro(lightMap.texture, srcFull, dstFull, { 0.0f, 0.0f }, 0.0f, WHITE);
        EndShaderMode();
    } else {
        DrawTexturePro(lightMap.texture, srcFull, dstFull, { 0.0f, 0.0f }, 0.0f, WHITE);
    }
    EndBlendMode();

    // 3. Delicate Atmospheric Light Bloom in Mist/Air (subtle warm scatter, no hard edges)
    if (radialFalloffTex.id != 0 && lightWeight > 0.25f) {
        BeginMode2D(camera);
        BeginBlendMode(BLEND_ADDITIVE);
        Rectangle rSrc = { 0.0f, 0.0f, (float)radialFalloffTex.width, (float)radialFalloffTex.height };

        for (const auto& torch : torches) {
            float bloomR = 90.0f + 4.0f * sinf(time * 8.5f + torch.x * 0.13f);
            Rectangle rDst = { torch.x - bloomR, torch.y - bloomR, bloomR * 2.0f, bloomR * 2.0f };
            DrawTexturePro(radialFalloffTex, rSrc, rDst, { 0.0f, 0.0f }, 0.0f, ColorAlpha(torchCol, 0.09f * lightWeight));
        }
        for (const auto& light : activeLights) {
            float bloomR = light.radius * 0.42f;
            Rectangle rDst = { light.position.x - bloomR, light.position.y - bloomR, bloomR * 2.0f, bloomR * 2.0f };
            DrawTexturePro(radialFalloffTex, rSrc, rDst, { 0.0f, 0.0f }, 0.0f, ColorAlpha(light.color, 0.08f * light.intensity * lightWeight));
        }

        EndBlendMode();
        EndMode2D();
    }
}

