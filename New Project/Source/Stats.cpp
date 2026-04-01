#include "GameManager.hpp"
#include <fstream>

// --- MENU STATE ---
class MenuState : public IState {
public:
    void Enter(GameManager& gm) override {}
    void Update(GameManager& gm) override {
        if (IsKeyPressed(KEY_ENTER)) {
            // Transition to GameState (Logic for loading levels goes here)
        }
    }
    void Draw(GameManager& gm) override {
        gm.ui.DrawCard({400, 200, 500, 300}, "TYPING MASTER ++", "SOLID Edition");
    }
};

// --- PLAYING STATE ---
class PlayingState : public IState {
    void Enter(GameManager& gm) override {
        gm.userInput = "";
        gm.levelStartTime = GetTime();
        // Load file logic...
    }
    void Update(GameManager& gm) override {
        // Input Handling logic...
    }
    void Draw(GameManager& gm) override {
        gm.ui.DrawTypingArea(gm.targetText, gm.userInput, true);
    }
};