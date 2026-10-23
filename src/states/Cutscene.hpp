#pragma once
#include "raylib.h"
#include <string>
#include <vector>

struct CutsceneSegment {
    std::string actTitle;
    std::string bodyText;
    Color accentColor;
};

class Cutscene {
public:
    Cutscene();

    void InitPrologue();
    void Reset();
    bool Update(float dt); // Returns true when cutscene completes or is skipped
    void Draw(int windowWidth, int windowHeight) const;

    bool IsFinished() const { return finished; }
    void Skip() { finished = true; }
    int GetCurrentSegment() const { return currentSegmentIndex; }
    int GetTotalSegments() const { return (int)segments.size(); }

private:
    std::vector<CutsceneSegment> segments;
    int currentSegmentIndex;
    int visibleChars;
    float charTimer;
    float fadeAlpha;
    float ambientTimer;
    bool finished;

    void DrawWrappedText(const std::string& text, int posX, int posY, int maxLineWidth, int fontSize, int lineSpacing, Color color, int maxChars) const;
};
