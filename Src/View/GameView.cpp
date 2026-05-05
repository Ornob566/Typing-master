#include "GameView.h"
#include "Constants.h"

void GameView::LoadAssets() {
    background = LoadTexture("assets/textures/bg.jpg");
    bgLoaded   = (background.id > 0);
}

void GameView::UnloadAssets() {
    if (bgLoaded) UnloadTexture(background);
}

void GameView::Draw(const GameModel& model, bool cursorVisible) {
    BeginDrawing();
    ClearBackground(COLOR_BG);

    if (bgLoaded)
        DrawTextureEx(background, {0, 0}, 0,
                      (float)screenWidth / background.width, WHITE);

    switch (model.currentState) {
        case STATE_MENU:  DrawMenu(model);                break;
        case STATE_GAME:  DrawGame(model, cursorVisible); break;
        case STATE_FINAL: DrawFinalStats(model);          break;
    }

    EndDrawing();
}

void GameView::DrawMenu(const GameModel&) {
    DrawRectangleRounded({ 400, 200, 500, 300 }, 0.1f, 10, COLOR_CARD);
    DrawText("TYPING MASTER ++",   440, 260, 40, COLOR_ACCENT);
    DrawText("OOP / MVC Edition",  565, 310, 18, GRAY);

    if ((int)(GetTime() * 2) % 2 == 0)
        DrawText("PRESS [ENTER] TO START", 525, 400, 20, WHITE);
}

void GameView::DrawGame(const GameModel& model, bool cursorVisible) {
    double now = GetTime();

    if (!model.isLevelStarted) {
        int rem = model.CountdownRemaining(now);
        DrawCircle(screenWidth / 2, screenHeight / 2, 60, COLOR_CARD);
        DrawText(TextFormat("%d", rem),
                 screenWidth / 2 - 15, screenHeight / 2 - 30, 60, COLOR_ACCENT);
    } else if (!model.isLevelFinished) {
        DrawGameHUD(model, now);
        DrawTextArea(model, cursorVisible);
    } else {
        DrawLevelResult(model);
    }
}

void GameView::DrawGameHUD(const GameModel& model, double now) {
    DrawRectangle(0, 0, screenWidth, 60, COLOR_CARD);
    DrawText(TextFormat("LEVEL: %d", model.currentLevel + 1), 50, 20, 22, COLOR_ACCENT);

    float ratio = model.TimerRatio(now);
    DrawRectangle(350, 25, 600, 12, DARKGRAY);
    DrawRectangle(350, 25, (int)(600 * ratio), 12, COLOR_ACCENT);
}

void GameView::DrawTextArea(const GameModel& model, bool cursorVisible) {
    DrawRectangleRounded({ 50, 100, 1200, 550 }, 0.03f, 10, COLOR_CARD);

    DrawText("TARGET:",      80, 130, 16, COLOR_ACCENT);
    DrawText(model.targetText.c_str(), 80, 165, 24, GRAY);

    DrawText("YOUR INPUT:",  80, 350, 16, GREEN);
    int drawX = 80, drawY = 390;

    for (size_t i = 0; i < model.userInput.length(); i++) {
        char c[2] = { model.userInput[i], '\0' };

        if (model.userInput[i] == '\n') {
            drawY += 35; drawX = 80;
        } else {
            Color col = (i < model.targetText.length() &&
                         model.userInput[i] == model.targetText[i])
                        ? WHITE : RED;
            DrawText(c, drawX, drawY, 26, col);
            drawX += MeasureText(c, 26);
        }
    }

    if (cursorVisible)
        DrawRectangle(drawX, drawY + 5, 12, 2, COLOR_ACCENT);
}

void GameView::DrawLevelResult(const GameModel& model) {
    DrawRectangleRounded({ 450, 180, 400, 350 }, 0.1f, 10, COLOR_CARD);
    DrawText("LEVEL DONE!", 540, 220, 30, COLOR_ACCENT);
    DrawText(TextFormat("WPM:      %.1f",  model.stats[model.currentLevel].wpm),  550, 290, 24, WHITE);
    DrawText(TextFormat("Accuracy: %.1f%%", model.stats[model.currentLevel].accuracy), 550, 330, 24, WHITE);
    DrawText("Press [ENTER] to Continue", 535, 450, 18, GRAY);
}

void GameView::DrawFinalStats(const GameModel& model) {
    DrawRectangleRounded({ 350, 120, 600, 480 }, 0.05f, 10, COLOR_CARD);
    DrawText("FINAL STATISTICS", 480, 160, 35, COLOR_ACCENT);

    for (int i = 0; i < TOTAL_LEVELS; i++) {
        DrawText(TextFormat("Level %d:  %2.0f WPM  |  %2.0f%% Accuracy",
                 i + 1, model.stats[i].wpm, model.stats[i].accuracy),
                 430, 250 + (i * 50), 22, WHITE);
    }

    DrawRectangle(430, 420, 440, 2, DARKGRAY);
    DrawText(TextFormat("TOTAL TIME: %.1f seconds", model.totalSessionTime), 490, 450, 24, GREEN);
    DrawText("Press [ESC] to Exit", 560, 540, 16, GRAY);
}
