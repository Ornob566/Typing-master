#pragma once
#include "GameModel.h"
#include "GameView.h"

class GameController {
private:
    const int screenWidth  = 1300;
    const int screenHeight = 720;

    GameModel model;
    GameView  view;

    double blinkTimer    = 0;
    bool   cursorVisible = true;

public:
    GameController();
    ~GameController();
    void Run();

private:
    void Update();
    void UpdateGame(double now);
    void HandleTypingInput();
};
