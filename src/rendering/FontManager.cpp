#include "FontManager.hpp"
#undef DrawText
#undef MeasureText
#include <iostream>
#include <sstream>
#include <algorithm>

Font FontManager::bodyFont = { 0 };
Font FontManager::titleFont = { 0 };
bool FontManager::isLoaded = false;

std::vector<int> FontManager::GenerateExtendedCodepoints() {
    std::vector<int> codepoints;
    codepoints.reserve(512);

    // 1. Basic Latin (ASCII 32 - 126)
    for (int i = 32; i <= 126; ++i) {
        codepoints.push_back(i);
    }

    // 2. Latin-1 Supplement (160 - 255)
    for (int i = 160; i <= 255; ++i) {
        codepoints.push_back(i);
    }

    // 3. Latin Extended-A (256 - 383)
    for (int i = 256; i <= 383; ++i) {
        codepoints.push_back(i);
    }

    // 4. General Punctuation (0x2010 - 0x2027)
    // Includes dashes (–, —), quotation marks (‘, ’, “, ”, „, ‟), daggers (†, ‡), bullet (•), ellipsis (…)
    for (int i = 0x2010; i <= 0x2027; ++i) {
        codepoints.push_back(i);
    }

    // 5. Additional Punctuation & Quotes (0x2030 - 0x203A)
    for (int i = 0x2030; i <= 0x203A; ++i) {
        codepoints.push_back(i);
    }

    // 6. Currency & Commercial Symbols
    codepoints.push_back(0x20AC); // Euro €
    codepoints.push_back(0x2122); // Trademark ™

    // 7. Arrow Symbols (←, ↑, →, ↓, ↔, ↕)
    for (int i = 0x2190; i <= 0x2195; ++i) {
        codepoints.push_back(i);
    }

    // 8. Geometric Shapes (■, □, ▲, △, ▶, ▷, ▼, ▽, ◀, ◁, ◆, ◇, ●, ○)
    for (int i = 0x25A0; i <= 0x25CF; ++i) {
        codepoints.push_back(i);
    }

    return codepoints;
}

void FontManager::Init() {
    if (isLoaded) return;

    std::vector<int> codepoints = GenerateExtendedCodepoints();
    int cpCount = (int)codepoints.size();
    const int* cpData = codepoints.data();

    // 1. Load Body Font (MedievalSharp)
    const char* bodyPath = "assets/fonts/MedievalSharp.ttf";
    if (FileExists(bodyPath)) {
        bodyFont = LoadFontEx(bodyPath, 36, cpData, cpCount);
        if (bodyFont.texture.id != 0) {
            SetTextureFilter(bodyFont.texture, TEXTURE_FILTER_BILINEAR);
        } else {
            bodyFont = GetFontDefault();
        }
    } else {
        bodyFont = GetFontDefault();
    }

    // 2. Load Title Font (Cinzel)
    const char* titlePath = "assets/fonts/Cinzel.ttf";
    if (FileExists(titlePath)) {
        titleFont = LoadFontEx(titlePath, 44, cpData, cpCount);
        if (titleFont.texture.id != 0) {
            SetTextureFilter(titleFont.texture, TEXTURE_FILTER_BILINEAR);
        } else {
            titleFont = bodyFont.texture.id != 0 ? bodyFont : GetFontDefault();
        }
    } else {
        titleFont = bodyFont.texture.id != 0 ? bodyFont : GetFontDefault();
    }

    isLoaded = true;
}

void FontManager::Unload() {
    if (!isLoaded) return;

    Font def = GetFontDefault();
    if (bodyFont.texture.id != 0 && bodyFont.texture.id != def.texture.id) {
        UnloadFont(bodyFont);
    }
    if (titleFont.texture.id != 0 && titleFont.texture.id != def.texture.id && titleFont.texture.id != bodyFont.texture.id) {
        UnloadFont(titleFont);
    }

    bodyFont = { 0 };
    titleFont = { 0 };
    isLoaded = false;
}

bool FontManager::IsLoaded() {
    return isLoaded;
}

Font FontManager::GetFont(GameFont fontType) {
    if (!isLoaded) return GetFontDefault();
    return (fontType == GameFont::TITLE) ? titleFont : bodyFont;
}

Font FontManager::GetBodyFont() {
    return GetFont(GameFont::BODY);
}

Font FontManager::GetTitleFont() {
    return GetFont(GameFont::TITLE);
}

std::string FontManager::SanitizeText(const std::string& input) {
    std::string output;
    output.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char c = (unsigned char)input[i];

        // Replace CP-1252 legacy single-byte smart quotes or dashes if encountered
        if (c == 0x91 || c == 0x92) {
            output += "'";
        } else if (c == 0x93 || c == 0x94) {
            output += "\"";
        } else if (c == 0x96 || c == 0x97) {
            output += "-";
        } else if (c == 0x85) {
            output += "...";
        } else {
            output += (char)c;
        }
    }

    return output;
}

void FontManager::DrawGameText(const std::string& text, int posX, int posY, int fontSize, Color color, GameFont fontType) {
    if (text.empty()) return;
    Font font = GetFont(fontType);
    std::string sanitized = SanitizeText(text);
    float spacing = (fontSize >= 20) ? 1.5f : 1.0f;
    Vector2 pos = { (float)posX, (float)posY };
    ::DrawTextEx(font, sanitized.c_str(), pos, (float)fontSize, spacing, color);
}

void FontManager::DrawGameText(const char* text, int posX, int posY, int fontSize, Color color, GameFont fontType) {
    if (!text || text[0] == '\0') return;
    DrawGameText(std::string(text), posX, posY, fontSize, color, fontType);
}

void FontManager::DrawTextTitle(const std::string& text, int posX, int posY, int fontSize, Color color) {
    DrawGameText(text, posX, posY, fontSize, color, GameFont::TITLE);
}

void FontManager::DrawTextEx(GameFont fontType, const std::string& text, Vector2 position, float fontSize, float spacing, Color tint) {
    if (text.empty()) return;
    Font font = GetFont(fontType);
    std::string sanitized = SanitizeText(text);
    ::DrawTextEx(font, sanitized.c_str(), position, fontSize, spacing, tint);
}

void FontManager::DrawTextCentered(const std::string& text, int centerX, int posY, int fontSize, Color color, GameFont fontType) {
    int width = MeasureGameText(text, fontSize, fontType);
    DrawGameText(text, centerX - width / 2, posY, fontSize, color, fontType);
}

void FontManager::DrawTextTitleCentered(const std::string& text, int centerX, int posY, int fontSize, Color color) {
    DrawTextCentered(text, centerX, posY, fontSize, color, GameFont::TITLE);
}

int FontManager::MeasureGameText(const std::string& text, int fontSize, GameFont fontType) {
    if (text.empty()) return 0;
    Font font = GetFont(fontType);
    std::string sanitized = SanitizeText(text);
    float spacing = (fontSize >= 20) ? 1.5f : 1.0f;
    Vector2 size = ::MeasureTextEx(font, sanitized.c_str(), (float)fontSize, spacing);
    return (int)size.x;
}

int FontManager::MeasureGameText(const char* text, int fontSize, GameFont fontType) {
    if (!text || text[0] == '\0') return 0;
    return MeasureGameText(std::string(text), fontSize, fontType);
}

int FontManager::MeasureTextTitle(const std::string& text, int fontSize) {
    return MeasureGameText(text, fontSize, GameFont::TITLE);
}

Vector2 FontManager::MeasureTextEx(GameFont fontType, const std::string& text, float fontSize, float spacing) {
    Font font = GetFont(fontType);
    std::string sanitized = SanitizeText(text);
    return ::MeasureTextEx(font, sanitized.c_str(), fontSize, spacing);
}

void FontManager::DrawTextWrapped(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color, GameFont fontType, int maxChars) {
    Font font = GetFont(fontType);
    std::string sanitized = SanitizeText(text);
    float spacing = (fontSize >= 20) ? 1.5f : 1.0f;

    std::istringstream stream(sanitized);
    std::string paragraph;
    int currentY = posY;
    int charsRendered = 0;

    while (std::getline(stream, paragraph)) {
        if (paragraph.empty()) {
            currentY += fontSize + lineSpacing;
            continue;
        }

        std::istringstream wordStream(paragraph);
        std::string word;
        std::string currentLine;

        while (wordStream >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            Vector2 sz = ::MeasureTextEx(font, testLine.c_str(), (float)fontSize, spacing);

            if ((int)sz.x > maxLineWidth && !currentLine.empty()) {
                // Render current line
                if (maxChars < 0 || charsRendered < maxChars) {
                    std::string lineToDraw = currentLine;
                    if (maxChars >= 0 && (charsRendered + (int)currentLine.length() > maxChars)) {
                        lineToDraw = currentLine.substr(0, maxChars - charsRendered);
                    }
                    Vector2 linePos = { (float)posX, (float)currentY };
                    ::DrawTextEx(font, lineToDraw.c_str(), linePos, (float)fontSize, spacing, color);
                }
                charsRendered += (int)currentLine.length() + 1;
                currentLine = word;
                currentY += fontSize + lineSpacing;
            } else {
                currentLine = testLine;
            }
        }

        if (!currentLine.empty()) {
            if (maxChars < 0 || charsRendered < maxChars) {
                std::string lineToDraw = currentLine;
                if (maxChars >= 0 && (charsRendered + (int)currentLine.length() > maxChars)) {
                    lineToDraw = currentLine.substr(0, maxChars - charsRendered);
                }
                Vector2 linePos = { (float)posX, (float)currentY };
                ::DrawTextEx(font, lineToDraw.c_str(), linePos, (float)fontSize, spacing, color);
            }
            charsRendered += (int)currentLine.length() + 1;
            currentY += fontSize + lineSpacing;
        }
    }
}
