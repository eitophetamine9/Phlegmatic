#pragma once
#include "raylib.h"
#include <string>
#include <vector>

enum class GameFont {
    BODY,   // MedievalSharp - Dialogues, codex lore, HUD labels, quest tracker
    TITLE   // Cinzel - Headers, banners, title cards, realm transitions
};

class FontManager {
public:
    static void Init();
    static void Unload();
    static bool IsLoaded();

    static Font GetFont(GameFont fontType = GameFont::BODY);
    static Font GetBodyFont();
    static Font GetTitleFont();

    // High-level drawing helpers with automatic UTF-8 sanitization
    static void DrawText(const std::string& text, int posX, int posY, int fontSize, Color color, GameFont fontType = GameFont::BODY);
    static void DrawTextTitle(const std::string& text, int posX, int posY, int fontSize, Color color);
    static void DrawTextEx(GameFont fontType, const std::string& text, Vector2 position, float fontSize, float spacing, Color tint);

    // Centered drawing helpers
    static void DrawTextCentered(const std::string& text, int centerX, int posY, int fontSize, Color color, GameFont fontType = GameFont::BODY);
    static void DrawTextTitleCentered(const std::string& text, int centerX, int posY, int fontSize, Color color);

    // Text measurement helpers
    static int MeasureText(const std::string& text, int fontSize, GameFont fontType = GameFont::BODY);
    static int MeasureTextTitle(const std::string& text, int fontSize);
    static Vector2 MeasureTextEx(GameFont fontType, const std::string& text, float fontSize, float spacing = 1.0f);

    // Word-wrapped text drawing
    static void DrawTextWrapped(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color, GameFont fontType = GameFont::BODY, int maxChars = -1);

    // UTF-8 string sanitizer ensuring no unrendered '?' replacement glyphs
    static std::string SanitizeText(const std::string& input);

private:
    static std::vector<int> GenerateExtendedCodepoints();
    static Font bodyFont;
    static Font titleFont;
    static bool isLoaded;
};
