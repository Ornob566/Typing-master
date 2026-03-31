#include "ui.h"
#include <stdlib.h>
#include <string.h>

UIRenderer* UIRenderer_Create() {
    UIRenderer* ui = (UIRenderer*)malloc(sizeof(UIRenderer));
    if (!ui) return NULL;
    ui->background = LoadTexture("bg.jpg"); 
    return ui;
}

void UIRenderer_Destroy(UIRenderer* ui) {
    if (ui) {
        UnloadTexture(ui->background);
        free(ui);
    }
}

void UIRenderer_Begin(UIRenderer* ui) {
    BeginDrawing();
    ClearBackground(COLOR_BG);
    if (ui->background.id > 0) {
        DrawTextureEx(ui->background, (Vector2){0,0}, 0, (float)SCREEN_WIDTH/ui->background.width, WHITE);
    }
}

void UIRenderer_End() {
    EndDrawing();
}

void UIRenderer_DrawMenu(UIRenderer* ui) {
    DrawRectangleRounded((Rectangle){ 400, 200, 500, 300 }, 0.1, 10, COLOR_CARD);
    DrawText("TYPING MASTER", 465, 260, 40, COLOR_ACCENT);
    DrawText("Test your typing speed skills", 515, 310, 18, GRAY);
    
    // Original blinking logic kept locally for UI
    if (((int)(GetTime() * 2) % 2 == 0)) 
        DrawText("PRESS [ENTER] TO START", 525, 400, 20, WHITE);
}

void UIRenderer_DrawCountdown(UIRenderer* ui, int remaining) {
    DrawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 60, COLOR_CARD);
    DrawText(TextFormat("%d", remaining), SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 - 30, 60, COLOR_ACCENT);
}

void UIRenderer_DrawGameSub(UIRenderer* ui, int currentLevel, double timeElapsed, double timeLimit, 
                            const char* targetText, const char* userInput, int inputIndex, bool cursorVisible) {
    // Top status bar
    DrawRectangle(0, 0, SCREEN_WIDTH, 60, COLOR_CARD);
    DrawText(TextFormat("LEVEL: %d", currentLevel + 1), 50, 20, 22, COLOR_ACCENT);
    
    // Time bar
    float timeLeftRatio = (float)(1.0 - (timeElapsed / timeLimit));
    if (timeLeftRatio < 0) timeLeftRatio = 0;
    DrawRectangle(350, 25, 600, 12, DARKGRAY);
    DrawRectangle(350, 25, (int)(600 * timeLeftRatio), 12, COLOR_ACCENT);

    // Main typing card
    DrawRectangleRounded((Rectangle){ 50, 100, 1200, 550 }, 0.03, 10, COLOR_CARD);
    
    // Target text
    DrawText("TARGET:", 80, 130, 16, COLOR_ACCENT);
    DrawText(targetText, 80, 165, 24, GRAY);

    // user input(including colour)
    DrawText("YOUR INPUT:", 80, 350, 16, GREEN);
    int drawX = 80, drawY = 390;
    int targetLen = strlen(targetText);

    for (int i = 0; i < inputIndex; i++) {
        char c[2] = { userInput[i], '\0' };
        Color cCol = (i < targetLen && userInput[i] == targetText[i]) ? WHITE : RED;
        
        if (userInput[i] == '\n') { drawY += 35; drawX = 80; }
        else {
            DrawText(c, drawX, drawY, 26, cCol);
            drawX += MeasureText(c, 26);
        }
    }
    if (cursorVisible) DrawRectangle(drawX, drawY + 5, 12, 2, COLOR_ACCENT);
}

void UIRenderer_DrawLevelResult(UIRenderer* ui, const LevelResult* result) {
    if (!result) return;
    DrawRectangleRounded((Rectangle){ 450, 180, 400, 350 }, 0.1, 10, COLOR_CARD);
    DrawText("LEVEL DONE!", 540, 220, 30, COLOR_ACCENT);
    DrawText(TextFormat("WPM: %.1f", result->wpm), 550, 290, 24, WHITE);
    DrawText(TextFormat("Accuracy: %.1f%%", result->accuracy), 550, 330, 24, WHITE);
    DrawText("Press [ENTER] to Continue", 535, 450, 18, GRAY);
}

void UIRenderer_DrawFinalStats(UIRenderer* ui, const LevelResult results[LEVELS], double totalTime) {
    DrawRectangleRounded((Rectangle){ 350, 120, 600, 480 }, 0.05, 10, COLOR_CARD);
    DrawText("FINAL STATISTICS", 480, 160, 35, COLOR_ACCENT);
    
    for (int i = 0; i < LEVELS; i++) {
        DrawText(TextFormat("Level %d: %2.0f WPM  |  %2.0f%% Accuracy", i+1, results[i].wpm, results[i].accuracy), 430, 250 + (i*50), 22, WHITE);
    }
    
    DrawRectangle(430, 420, 440, 2, DARKGRAY);
    DrawText(TextFormat("TOTAL TIME: %.1f seconds", totalTime), 490, 450, 24, GREEN);
    DrawText("Press [ESC] to Exit", 560, 540, 16, GRAY);
}