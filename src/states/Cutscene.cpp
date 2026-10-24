#include "Cutscene.hpp"
#include "../core/SoundFX.hpp"
#include <cmath>
#include <sstream>

Cutscene::Cutscene() {
    InitPrologue();
    Reset();
}

void Cutscene::InitPrologue() {
    segments.clear();

    segments.push_back({
        "ACT I - THE AGE OF CHAOS",
        "For over 500 years, the Land of Tydeshire was filled with chaos and never ending war. Order was not an option, there was no unity.\n\nFactions came along and so then became kingdoms. Each king wants to rule over the entire Land of Tydeshire, most failed miserably however one stood at the top.",
        Color{ 255, 120, 80, 255 }
    });

    segments.push_back({
        "ACT II - CHARLZ THE LIEGE OF KINGS",
        "After 500 years of chaos, one king stood at the top, King Charlz.\n\nHis overwhelming authority and remarkable strength unified the Land of Tydeshire crowning him Charlz the Liege of Kings. He then proceeded to divide the land into 5 kingdoms and appoint a king to each kingdom.",
        Color{ 255, 215, 80, 255 }
    });

    segments.push_back({
        "ACT III - THE FIVE KINGDOMS",
        "The 5 Kingdoms are Zyphden, Velnoth, Krynn, Drak'Mar, and Solis. Each Kingdom had a specific role in keeping peace and order throughout the land.\n\nBut this did not last long, peace only lasted for about 50 years until Charlz mysteriously vanished leaving no trace behind.",
        Color{ 100, 220, 255, 255 }
    });

    segments.push_back({
        "ACT IV - THE 2ND GREAT WAR",
        "The Kings believed he was dead and they began plotting to overthrow each other to take over the land. Thus began the 2nd Great War.\n\nThe truth behind the disappearance of Charlz was never uncovered, but rumors say that he was betrayed by his closest allies. The truth was shattered and scattered across the land, waiting to be pieced together.",
        Color{ 200, 140, 255, 255 }
    });
}

void Cutscene::Reset() {
    currentSegmentIndex = 0;
    visibleChars = 0;
    charTimer = 0.0f;
    fadeAlpha = 0.0f;
    ambientTimer = 0.0f;
    finished = false;
}

bool Cutscene::Update(float dt) {
    if (finished) return true;

    ambientTimer += dt;
    fadeAlpha = fminf(1.0f, fadeAlpha + dt * 3.0f);

    if (currentSegmentIndex >= (int)segments.size()) {
        finished = true;
        return true;
    }

    const auto& seg = segments[currentSegmentIndex];
    int totalLen = (int)seg.bodyText.length();

    // Typewriter character reveal
    if (visibleChars < totalLen) {
        charTimer += dt;
        while (charTimer >= 0.020f && visibleChars < totalLen) {
            visibleChars++;
            charTimer -= 0.020f;
        }
    }

    Vector2 mousePos = GetMousePosition();
    Rectangle skipRec = { 1600.0f - 180.0f, 24.0f, 150.0f, 36.0f };
    bool isSkipHovered = CheckCollisionPointRec(mousePos, skipRec);

    // Skip handling
    if (IsKeyPressed(KEY_ESCAPE) || (isSkipHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
        SoundFX::PlayButtonClick();
        finished = true;
        return true;
    }

    // Advance handling
    bool advancePressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) ||
        (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isSkipHovered);

    if (advancePressed) {
        if (visibleChars < totalLen) {
            // First press reveals remainder of paragraph immediately
            visibleChars = totalLen;
        } else {
            // Second press advances to next segment
            currentSegmentIndex++;
            visibleChars = 0;
            charTimer = 0.0f;
            fadeAlpha = 0.2f;
            SoundFX::PlayCutsceneChime();

            if (currentSegmentIndex >= (int)segments.size()) {
                finished = true;
                return true;
            }
        }
    }

    return false;
}

void Cutscene::DrawWrappedText(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color, int maxChars) const {
    std::istringstream stream(text);
    std::string paragraph;
    int currentY = posY;
    int charsDrawn = 0;

    while (std::getline(stream, paragraph)) {
        if (paragraph.empty()) {
            currentY += fontSize + lineSpacing;
            continue;
        }

        std::istringstream wordStream(paragraph);
        std::string word;
        std::string currentLine = "";

        while (wordStream >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            int testWidth = MeasureText(testLine.c_str(), fontSize);

            if (testWidth > maxLineWidth && !currentLine.empty()) {
                // Print current line
                if (maxChars < 0 || charsDrawn < maxChars) {
                    int printLen = (maxChars < 0) ? (int)currentLine.length() : (int)fminf((float)currentLine.length(), (float)(maxChars - charsDrawn));
                    if (printLen > 0) {
                        std::string sub = currentLine.substr(0, printLen);
                        DrawText(sub.c_str(), posX, currentY, fontSize, color);
                        charsDrawn += printLen;
                    }
                }
                currentY += fontSize + lineSpacing;
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }

        if (!currentLine.empty()) {
            if (maxChars < 0 || charsDrawn < maxChars) {
                int printLen = (maxChars < 0) ? (int)currentLine.length() : (int)fminf((float)currentLine.length(), (float)(maxChars - charsDrawn));
                if (printLen > 0) {
                    std::string sub = currentLine.substr(0, printLen);
                    DrawText(sub.c_str(), posX, currentY, fontSize, color);
                    charsDrawn += printLen;
                }
            }
            currentY += fontSize + lineSpacing;
        }
    }
}

void Cutscene::Draw(int windowWidth, int windowHeight) const {
    if (finished || currentSegmentIndex >= (int)segments.size()) return;

    // 1. Deep cinematic darkness
    DrawRectangle(0, 0, windowWidth, windowHeight, Color{ 8, 10, 15, 255 });

    // 2. Ambient drifting particles / embers
    for (int i = 0; i < 24; i++) {
        float px = fmodf((float)(i * 97) + ambientTimer * (15.0f + (float)(i % 5) * 6.0f), (float)windowWidth);
        float py = fmodf((float)(i * 73) - ambientTimer * (10.0f + (float)(i % 4) * 8.0f), (float)windowHeight);
        if (py < 0.0f) py += (float)windowHeight;

        float alphaMod = 0.5f + 0.5f * sinf(ambientTimer * 2.0f + (float)i);
        Color emberColor = (i % 3 == 0) ? Color{ 255, 180, 70, (unsigned char)(90 * alphaMod) } : Color{ 120, 160, 240, (unsigned char)(60 * alphaMod) };
        DrawCircleV({ px, py }, 2.0f + (float)(i % 3), emberColor);
    }

    const auto& seg = segments[currentSegmentIndex];

    // 3. Central illuminated manuscript card
    const float cardW = 960.0f;
    const float cardH = 540.0f;
    float cardX = ((float)windowWidth - cardW) * 0.5f;
    float cardY = ((float)windowHeight - cardH) * 0.5f;

    // Card background & layered border
    DrawRectangleRounded(Rectangle{ cardX, cardY, cardW, cardH }, 0.08f, 6, Color{ 14, 17, 26, 240 });
    DrawRectangleRoundedLines(Rectangle{ cardX, cardY, cardW, cardH }, 0.08f, 6, Color{ 60, 75, 105, 230 });
    DrawRectangleRoundedLines(Rectangle{ cardX + 6, cardY + 6, cardW - 12, cardH - 12 }, 0.07f, 6, Color{ 35, 45, 65, 180 });

    // Corner filigree accents
    Color cornerColor = Color{ 255, 215, 80, 220 };
    DrawLineEx({ cardX + 16, cardY + 16 }, { cardX + 46, cardY + 16 }, 2.0f, cornerColor);
    DrawLineEx({ cardX + 16, cardY + 16 }, { cardX + 16, cardY + 46 }, 2.0f, cornerColor);
    DrawLineEx({ cardX + cardW - 16, cardY + 16 }, { cardX + cardW - 46, cardY + 16 }, 2.0f, cornerColor);
    DrawLineEx({ cardX + cardW - 16, cardY + 16 }, { cardX + cardW - 16, cardY + 46 }, 2.0f, cornerColor);

    DrawLineEx({ cardX + 16, cardY + cardH - 16 }, { cardX + 46, cardY + cardH - 16 }, 2.0f, cornerColor);
    DrawLineEx({ cardX + 16, cardY + cardH - 16 }, { cardX + 16, cardY + cardH - 46 }, 2.0f, cornerColor);
    DrawLineEx({ cardX + cardW - 16, cardY + cardH - 16 }, { cardX + cardW - 46, cardY + cardH - 16 }, 2.0f, cornerColor);
    DrawLineEx({ cardX + cardW - 16, cardY + cardH - 16 }, { cardX + cardW - 16, cardY + cardH - 46 }, 2.0f, cornerColor);

    // Header: CHRONICLES OF TYDESHIRE
    const char* subHeader = "— CHRONICLES OF TYDESHIRE —";
    int shW = MeasureText(subHeader, 13);
    DrawText(subHeader, (int)(cardX + (cardW - shW) * 0.5f), (int)cardY + 36, 13, Color{ 200, 175, 120, 200 });

    // Act Title
    int titleW = MeasureText(seg.actTitle.c_str(), 24);
    DrawText(seg.actTitle.c_str(), (int)(cardX + (cardW - titleW) * 0.5f), (int)cardY + 62, 24, seg.accentColor);

    // Decorative divider line
    float divHalfW = 180.0f;
    float divY = cardY + 104.0f;
    DrawLineEx({ cardX + (cardW * 0.5f) - divHalfW, divY }, { cardX + (cardW * 0.5f) + divHalfW, divY }, 1.5f, Color{ 140, 110, 60, 200 });
    DrawCircle((int)(cardX + cardW * 0.5f), (int)divY, 3.5f, Color{ 255, 215, 80, 255 });

    // Body Text (Wrapped & Typewritten)
    float textMargin = 64.0f;
    int maxTextW = (int)(cardW - textMargin * 2.0f);
    DrawWrappedText(seg.bodyText, (int)(cardX + textMargin), (int)(cardY + 135.0f), maxTextW, 19, 14, Color{ 230, 235, 245, (unsigned char)(255 * fadeAlpha) }, visibleChars);

    // Bottom Navigation Bar
    // Segment dots: ● ○ ○ ○
    float dotsStartX = cardX + (cardW - (float)segments.size() * 24.0f) * 0.5f;
    float dotsY = cardY + cardH - 58.0f;

    for (int i = 0; i < (int)segments.size(); i++) {
        float dotX = dotsStartX + (float)i * 24.0f;
        if (i == currentSegmentIndex) {
            DrawCircle((int)dotX, (int)dotsY, 5.0f, seg.accentColor);
            DrawCircleLines((int)dotX, (int)dotsY, 8.0f, Color{ 255, 255, 255, 180 });
        } else if (i < currentSegmentIndex) {
            DrawCircle((int)dotX, (int)dotsY, 3.5f, Color{ 180, 195, 220, 180 });
        } else {
            DrawCircleLines((int)dotX, (int)dotsY, 3.5f, Color{ 70, 85, 115, 180 });
        }
    }

    // Interactive prompt
    const char* advancePrompt = (visibleChars < (int)seg.bodyText.length()) ? "[SPACE / CLICK] Reveal Text" : "[SPACE / CLICK] Continue";
    int apW = MeasureText(advancePrompt, 13);
    DrawText(advancePrompt, (int)(cardX + (cardW - apW) * 0.5f), (int)cardY + cardH - 36, 13, Color{ 180, 210, 240, 220 });

    // Top Right Skip Button
    Vector2 mousePos = GetMousePosition();
    Rectangle skipRec = { (float)windowWidth - 190.0f, 24.0f, 160.0f, 36.0f };
    bool isSkipHov = CheckCollisionPointRec(mousePos, skipRec);

    DrawRectangleRounded(skipRec, 0.25f, 4, isSkipHov ? Color{ 55, 70, 95, 240 } : Color{ 22, 28, 42, 210 });
    DrawRectangleRoundedLines(skipRec, 0.25f, 4, isSkipHov ? Color{ 120, 220, 255, 255 } : Color{ 60, 80, 115, 200 });

    const char* skipText = "SKIP PROLOGUE [ESC]";
    int stW = MeasureText(skipText, 11);
    DrawText(skipText, (int)(skipRec.x + (skipRec.width - stW) * 0.5f), (int)(skipRec.y + 12), 11, isSkipHov ? Color{ 255, 255, 255, 255 } : Color{ 190, 215, 240, 220 });
}
