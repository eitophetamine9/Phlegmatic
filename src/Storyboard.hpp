#pragma once
#include "raylib.h"
#include <string>
#include <vector>

struct StoryAct {
    std::string actTitle;
    std::string narrative;
    std::string combatObjective;
    bool isCompleted = false;
};

class StoryboardSystem {
public:
    StoryboardSystem();

    void Init();
    void Update(float dt);
    void DrawHUD(int windowWidth, int windowHeight) const;
    void DrawOverlay(int windowWidth, int windowHeight) const;

    void ToggleOverlay() { showOverlay = !showOverlay; }
    bool IsOverlayOpen() const { return showOverlay; }

    void NextAct();
    void PrevAct();

    int GetCurrentActIndex() const { return currentActIndex; }
    const StoryAct& GetCurrentAct() const { return acts[currentActIndex]; }

private:
    std::vector<StoryAct> acts;
    int currentActIndex = 0;
    bool showOverlay = false;
};
