#pragma once
#include "raylib.h"
#include "GameModel.h"

class GameView {
private:
    const int screenWidth  = 1300;
    const int screenHeight = 720;
    Texture2D background;
    bool      bgLoaded = false;

public:
    void LoadAssets();
    void UnloadAssets();
    void Draw(const GameModel& model, bool cursorVisible);

private:
    void DrawMenu(const GameModel& model);
    void DrawGame(const GameModel& model, bool cursorVisible);
    void DrawGameHUD(const GameModel& model, double now);
    void DrawTextArea(const GameModel& model, bool cursorVisible);
    void DrawLevelResult(const GameModel& model);
    void DrawFinalStats(const GameModel& model);
};
