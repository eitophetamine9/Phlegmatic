#pragma once
#include "raylib.h"
#include <string>
#include <vector>

enum class GameFont {
    BODY,   // MedievalSharp - Dialogues, codex lore, HUD labels, quest tracker
    TITLE   // Cinzel - Headers, banners, title cards, realm transitions
};

enum class FontScaleMode {
    STANDARD = 100,    // 1.00x Base
    LARGE = 125,       // 1.25x (+25%)
    EXTRA_LARGE = 150  // 1.50x (+50%)
};

class FontManager {
public:
    static void Init();
    static void Unload();
    static bool IsLoaded();

    // Scale mode controls
    static void SetScaleMode(FontScaleMode mode);
    static FontScaleMode GetScaleMode();
    static float GetScaleFactor();
    static int GetScaledSize(int baseSize);

    static Font GetFont(GameFont fontType = GameFont::BODY);
    static Font GetBodyFont();
    static Font GetTitleFont();

    // High-level drawing helpers with automatic UTF-8 sanitization
    static void DrawGameText(const std::string& text, int posX, int posY, int fontSize, Color color, GameFont fontType = GameFont::BODY);
    static void DrawGameText(const char* text, int posX, int posY, int fontSize, Color color, GameFont fontType = GameFont::BODY);
    static void DrawTextTitle(const std::string& text, int posX, int posY, int fontSize, Color color);
    static void DrawTextEx(GameFont fontType, const std::string& text, Vector2 position, float fontSize, float spacing, Color tint);

    // Drop-shadow outline rendering helpers for high contrast against background clutter
    static void DrawGameTextShadow(const std::string& text, int posX, int posY, int fontSize, Color color, Color shadowColor = Color{ 10, 12, 18, 220 }, int offset = 2, GameFont fontType = GameFont::BODY);
    static void DrawGameTextShadow(const char* text, int posX, int posY, int fontSize, Color color, Color shadowColor = Color{ 10, 12, 18, 220 }, int offset = 2, GameFont fontType = GameFont::BODY);
    static void DrawTextTitleShadow(const std::string& text, int posX, int posY, int fontSize, Color color, Color shadowColor = Color{ 10, 12, 18, 220 }, int offset = 2);
    static void DrawTextCenteredShadow(const std::string& text, int centerX, int posY, int fontSize, Color color, Color shadowColor = Color{ 10, 12, 18, 220 }, int offset = 2, GameFont fontType = GameFont::BODY);
    static void DrawTextTitleCenteredShadow(const std::string& text, int centerX, int posY, int fontSize, Color color, Color shadowColor = Color{ 10, 12, 18, 220 }, int offset = 2);
    static void DrawTextWrappedShadow(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color, Color shadowColor = Color{ 10, 12, 18, 220 }, int offset = 2, GameFont fontType = GameFont::BODY, int maxChars = -1);

    // Centered drawing helpers
    static void DrawTextCentered(const std::string& text, int centerX, int posY, int fontSize, Color color, GameFont fontType = GameFont::BODY);
    static void DrawTextTitleCentered(const std::string& text, int centerX, int posY, int fontSize, Color color);

    // Text measurement helpers
    static int MeasureGameText(const std::string& text, int fontSize, GameFont fontType = GameFont::BODY);
    static int MeasureGameText(const char* text, int fontSize, GameFont fontType = GameFont::BODY);
    static int MeasureTextTitle(const std::string& text, int fontSize);
    static Vector2 MeasureTextEx(GameFont fontType, const std::string& text, float fontSize, float spacing = 1.0f);

    // Backward-compatibility aliases
    static void DrawText(const std::string& text, int posX, int posY, int fontSize, Color color, GameFont fontType = GameFont::BODY) {
        DrawGameText(text, posX, posY, fontSize, color, fontType);
    }
    static void DrawTextShadow(const std::string& text, int posX, int posY, int fontSize, Color color, Color shadowColor = Color{ 10, 12, 18, 220 }, int offset = 2, GameFont fontType = GameFont::BODY) {
        DrawGameTextShadow(text, posX, posY, fontSize, color, shadowColor, offset, fontType);
    }
    static int MeasureText(const std::string& text, int fontSize, GameFont fontType = GameFont::BODY) {
        return MeasureGameText(text, fontSize, fontType);
    }

    // Word-wrapped text drawing
    static void DrawTextWrapped(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color, GameFont fontType = GameFont::BODY, int maxChars = -1);

    // UTF-8 string sanitizer ensuring no unrendered '?' replacement glyphs
    static std::string SanitizeText(const std::string& input);

private:
    static std::vector<int> GenerateExtendedCodepoints();
    static Font bodyFont;
    static Font titleFont;
    static bool isLoaded;
    static FontScaleMode currentScaleMode;
};

// Global transparent redirects to ensure consistent MedievalSharp / Cinzel typography across the entire game
#undef DrawText
#undef MeasureText
#define DrawText(...) FontManager::DrawGameText(__VA_ARGS__)
#define MeasureText(...) FontManager::MeasureGameText(__VA_ARGS__)

