#include "raylib.h"
#include <string.h>
#include <stdio.h>

//Constant settings
#define MAX_LEN 1000
#define MAX_LINES 10
#define COUNTDOWN 3
#define LEVELS 3

//Colour pallet
#define COLOR_BG      (Color){ 18, 18, 22, 255 } 
#define COLOR_CARD    (Color){ 30, 30, 35, 255 }  
#define COLOR_ACCENT  (Color){ 0, 170, 255, 255 } 
#define COLOR_TEXT    (Color){ 200, 200, 200, 255 } 

typedef enum { STATE_MENU, STATE_GAME, STATE_FINAL } GameState;

//In game data stracture
typedef struct {
    char levels[LEVELS][MAX_LINES][MAX_LEN];
    int linesCount[LEVELS];
    double timeLimits[LEVELS];
    double resultsWPM[LEVELS];
    double resultsAccuracy[LEVELS];
    double resultsTime[LEVELS];
} GameData;

// Function for load level from file
int LoadTextLinesFromFile(const char *filename, char lines[MAX_LINES][MAX_LEN]) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    int count = 0;
    while (fgets(lines[count], MAX_LEN, file) && count < MAX_LINES) {
        size_t len = strlen(lines[count]);
        if (len > 0 && lines[count][len - 1] == '\n') lines[count][len - 1] = '\0';
        count++;
    }
    fclose(file);
    return count;
}

// Target text combining function
void BuildTargetString(GameData *data, int level, char *outStr) {
    outStr[0] = '\0';
    for (int i = 0; i < data->linesCount[level]; i++) {
        strcat(outStr, data->levels[level][i]);
        if (i < data->linesCount[level] - 1) strcat(outStr, "\n");
    }
}

int main() {
    // window setup
    const int screenWidth = 1300;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Typing Master Pro");
    InitAudioDevice();
    SetTargetFPS(60);

    // Data initialization
    GameData game = {0};
    game.timeLimits[0] = 45; game.timeLimits[1] = 75; game.timeLimits[2] = 120;
    game.linesCount[0] = LoadTextLinesFromFile("level1.txt", game.levels[0]);
    game.linesCount[1] = LoadTextLinesFromFile("level2.txt", game.levels[1]);
    game.linesCount[2] = LoadTextLinesFromFile("level3.txt", game.levels[2]);

    // Game State Variable
    GameState currentState = STATE_MENU;
    int currentLevel = 0;
    char userInput[MAX_LEN] = "";
    char targetText[MAX_LEN] = "";
    int inputIndex = 0;
    
    double startTime = 0, countdownStart = 0, totalTime = 0;
    bool isLevelStarted = false, isLevelFinished = false;
    bool cursorVisible = true;
    double blinkTimer = 0;

    Texture2D bg = LoadTexture("bg.jpg"); 

    // Main game loop
    while (!WindowShouldClose()) {
        //Updated logic
        if (GetTime() - blinkTimer >= 0.5) {
            cursorVisible = !cursorVisible;
            blinkTimer = GetTime();
        }

        // Drawing logic
        BeginDrawing();
        ClearBackground(COLOR_BG);
        if (bg.id > 0) DrawTextureEx(bg, (Vector2){0,0}, 0, (float)screenWidth/bg.width, WHITE);

        switch (currentState) {
            case STATE_MENU:
                DrawRectangleRounded((Rectangle){ 400, 200, 500, 300 }, 0.1, 10, COLOR_CARD);
                DrawText("TYPING MASTER", 465, 260, 40, COLOR_ACCENT);
                DrawText("Test your typing speed skills", 515, 310, 18, GRAY);
                
                if (((int)(GetTime() * 2) % 2 == 0)) 
                    DrawText("PRESS [ENTER] TO START", 525, 400, 20, WHITE);

                if (IsKeyPressed(KEY_ENTER)) {
                    currentState = STATE_GAME;
                    countdownStart = GetTime();
                    BuildTargetString(&game, currentLevel, targetText);
                }
                break;

            case STATE_GAME:
                if (!isLevelStarted) {
                    // Countdown Screen
                    int remaining = COUNTDOWN - (int)(GetTime() - countdownStart);
                    DrawCircle(screenWidth/2, screenHeight/2, 60, COLOR_CARD);
                    DrawText(TextFormat("%d", remaining), screenWidth/2 - 15, screenHeight/2 - 30, 60, COLOR_ACCENT);
                    if (remaining <= 0) {
                        isLevelStarted = true;
                        startTime = GetTime();
                    }
                } 
                else if (!isLevelFinished) {
                    double timeElapsed = GetTime() - startTime;
                    
                    //Input Handeling
                    int key = GetCharPressed();
                    while (key > 0) {
                        if (inputIndex < MAX_LEN - 1) {
                            userInput[inputIndex++] = (char)key;
                            userInput[inputIndex] = '\0';
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && inputIndex > 0) userInput[--inputIndex] = '\0';
                    if (IsKeyPressed(KEY_ENTER) && inputIndex < MAX_LEN - 1) {
                        userInput[inputIndex++] = '\n'; userInput[inputIndex] = '\0';
                    }
                    
                    // condition for finishing level
                    if (IsKeyPressed(KEY_TAB) || timeElapsed >= game.timeLimits[currentLevel]) {
                        isLevelFinished = true;
                    }

                    // UI rendaring
                    // Top status bar
                    DrawRectangle(0, 0, screenWidth, 60, COLOR_CARD);
                    DrawText(TextFormat("LEVEL: %d", currentLevel + 1), 50, 20, 22, COLOR_ACCENT);
                    
                    // Time bar
                    float timeLeftRatio = (float)(1.0 - (timeElapsed / game.timeLimits[currentLevel]));
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
                    for (int i = 0; i < inputIndex; i++) {
                        char c[2] = { userInput[i], '\0' };
                        Color cCol = (i < (int)strlen(targetText) && userInput[i] == targetText[i]) ? WHITE : RED;
                        
                        if (userInput[i] == '\n') { drawY += 35; drawX = 80; }
                        else {
                            DrawText(c, drawX, drawY, 26, cCol);
                            drawX += MeasureText(c, 26);
                        }
                    }
                    if (cursorVisible) DrawRectangle(drawX, drawY + 5, 12, 2, COLOR_ACCENT);
                } 
                else {
                    //level result
                    double finalLevelTime = GetTime() - startTime;
                    int correctChars = 0;
                    int targetLen = strlen(targetText);
                    for (int i = 0; i < inputIndex && i < targetLen; i++) {
                        if (userInput[i] == targetText[i]) correctChars++;
                    }

                    game.resultsAccuracy[currentLevel] = (targetLen > 0) ? ((double)correctChars/targetLen)*100 : 0;
                    game.resultsWPM[currentLevel] = (finalLevelTime > 0) ? (inputIndex/5.0)/(finalLevelTime/60.0) : 0;
                    game.resultsTime[currentLevel] = finalLevelTime;

                    DrawRectangleRounded((Rectangle){ 450, 180, 400, 350 }, 0.1, 10, COLOR_CARD);
                    DrawText("LEVEL DONE!", 540, 220, 30, COLOR_ACCENT);
                    DrawText(TextFormat("WPM: %.1f", game.resultsWPM[currentLevel]), 550, 290, 24, WHITE);
                    DrawText(TextFormat("Accuracy: %.1f%%", game.resultsAccuracy[currentLevel]), 550, 330, 24, WHITE);
                    DrawText("Press [ENTER] to Continue", 535, 450, 18, GRAY);

                    if (IsKeyPressed(KEY_ENTER)) {
                        totalTime += finalLevelTime;
                        currentLevel++;
                        if (currentLevel < LEVELS) {
                            isLevelStarted = false; isLevelFinished = false; inputIndex = 0; 
                            userInput[0] = '\0'; countdownStart = GetTime();
                            BuildTargetString(&game, currentLevel, targetText);
                        } else currentState = STATE_FINAL;
                    }
                }
                break;

            case STATE_FINAL:
                DrawRectangleRounded((Rectangle){ 350, 120, 600, 480 }, 0.05, 10, COLOR_CARD);
                DrawText("FINAL STATISTICS", 480, 160, 35, COLOR_ACCENT);
                
                for (int i = 0; i < LEVELS; i++) {
                    DrawText(TextFormat("Level %d: %2.0f WPM  |  %2.0f%% Accuracy", i+1, game.resultsWPM[i], game.resultsAccuracy[i]), 430, 250 + (i*50), 22, WHITE);
                }
                
                DrawRectangle(430, 420, 440, 2, DARKGRAY);
                DrawText(TextFormat("TOTAL TIME: %.1f seconds", totalTime), 490, 450, 24, GREEN);
                DrawText("Press [ESC] to Exit", 560, 540, 16, GRAY);
                break;
        }

        EndDrawing();
    }

    //Resource release
    UnloadTexture(bg);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}