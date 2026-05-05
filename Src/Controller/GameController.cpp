#include "GameController.h"
#include "Constants.h"

GameController::GameController() {
    InitWindow(screenWidth, screenHeight, "Typing Master Pro ++ (MVC)");
    InitAudioDevice();
    SetTargetFPS(60);
    view.LoadAssets();
}

GameController::~GameController() {
    view.UnloadAssets();
    CloseAudioDevice();
    CloseWindow();
}

void GameController::Run() {
    while (!WindowShouldClose()) {
        Update();
        view.Draw(model, cursorVisible);
    }
}

void GameController::Update() {
    double now = GetTime();

    if (now - blinkTimer >= 0.5) {
        cursorVisible = !cursorVisible;
        blinkTimer    = now;
    }

    switch (model.currentState) {
        case STATE_MENU:
            if (IsKeyPressed(KEY_ENTER)) {
                model.currentState = STATE_GAME;
                model.currentLevel = 0;
                model.ResetForLevel(now);
            }
            break;
        case STATE_GAME:
            UpdateGame(now);
            break;
        case STATE_FINAL:
            break;
    }
}

void GameController::UpdateGame(double now) {
    if (!model.isLevelStarted) {
        if (now - model.countdownStart >= COUNTDOWN_TIME) {
            model.isLevelStarted = true;
            model.startTime      = now;
        }
    } else if (!model.isLevelFinished) {
        HandleTypingInput();
        if (model.IsTimeUp(now)) {
            model.isLevelFinished = true;
            model.CalculateResults(now);
        }
    } else {
        if (IsKeyPressed(KEY_ENTER)) {
            model.totalSessionTime += model.stats[model.currentLevel].timeTaken;
            model.currentLevel++;
            if (model.currentLevel < TOTAL_LEVELS)
                model.ResetForLevel(now);
            else
                model.currentState = STATE_FINAL;
        }
    }
}

void GameController::HandleTypingInput() {
    int key = GetCharPressed();
    while (key > 0) {
        if ((int)model.userInput.length() < MAX_LEN)
            model.userInput += (char)key;
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !model.userInput.empty())
        model.userInput.pop_back();

    if (IsKeyPressed(KEY_ENTER))
        model.userInput += '\n';

    if (IsKeyPressed(KEY_TAB)) {
        model.isLevelFinished = true;
        model.CalculateResults(GetTime());
    }
}
