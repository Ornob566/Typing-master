#include "raylib.h"
#include <string.h>
#include <stdio.h>

#define MAX_LEN 1000
#define MAX_LINES 10
#define COUNTDOWN 3
#define LEVELS 3

typedef enum { MENU, GAME, FINAL } GameState;

// Load text lines from file
int LoadTextLinesFromFile(const char *filename, char lines[MAX_LINES][MAX_LEN]) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    int count = 0;
    while (fgets(lines[count], MAX_LEN, file) && count < MAX_LINES) {
        size_t len = strlen(lines[count]);
        if (len > 0 && lines[count][len - 1] == '\n')
            lines[count][len - 1] = '\0';
        count++;
    }
    fclose(file);
    return count;
}

int main() {
    const int screenWidth = 1300;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Typing Master(Clone Project)");
    InitAudioDevice();

    Texture2D bg = LoadTexture("bg.jpg");
    Rectangle src = {0, 0, (float)bg.width, (float)bg.height};
    Rectangle dest = {0, 0, (float)screenWidth, (float)screenHeight};

   // Load Sounds (for events)
    Sound keySound = LoadSound("key.wav");
    Sound enterSound = LoadSound("enter.wav");
    Sound backspaceSound = LoadSound("backspace.wav");
    Sound finishSound = LoadSound("finish.wav");

    //  Background Music Setup
    Music backgroundMusic = LoadMusicStream("sound.mp3"); //  ADDED: Load music stream
    PlayMusicStream(backgroundMusic);                       //  ADDED: Start playback
    SetMusicVolume(backgroundMusic, 1.0f);                   // ADDED: Set volume (e.g., to 40%)

    char levels[LEVELS][MAX_LINES][MAX_LEN];
    int linesCount[LEVELS];
    linesCount[0] = LoadTextLinesFromFile("level1.txt", levels[0]);
    linesCount[1] = LoadTextLinesFromFile("level2.txt", levels[1]);
    linesCount[2] = LoadTextLinesFromFile("level3.txt", levels[2]);

    double levelTimeLimits[LEVELS] = {45, 75, 120};

    int level = 0;
    GameState state = MENU;
    char userInput[MAX_LEN] = "";
    int index = 0;
    double startTime = 0, timeTaken = 0;
    bool started = false, finished = false;
    double countdownStart = 0;
    double totalTime = 0;

    //arrays to store per-level results
    double levelTimes[LEVELS] = {0};
    double levelAccuracy[LEVELS] = {0};
    double levelWPM[LEVELS] = {0};

    // Cursor Blinking Variables
    double blinkTimer = 0.0;
    bool cursorVisible = true;
    const double blinkInterval = 0.5;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMusicStream(backgroundMusic);

        // Update Cursor Blink Logic
        if (GetTime() - blinkTimer >= blinkInterval) {
            cursorVisible = !cursorVisible;
            blinkTimer = GetTime();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexturePro(bg, src, dest, (Vector2){0, 0}, 0.0f, WHITE);

        switch (state) {
        case MENU:
            DrawText("Typing Master(Clone Project)", 420, 200, 40, YELLOW);
            DrawText("Press ENTER to Start", 500, 400, 30, LIGHTGRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                state = GAME;
                countdownStart = GetTime();
                PlaySound(enterSound);
            }
            break;

        case GAME:
            if (!started) {
                int remaining = COUNTDOWN - (int)(GetTime() - countdownStart);
                if (remaining > 0) {
                    DrawText(TextFormat("Starting in %d...", remaining), 580, 350, 40, RED);
                } else {
                    started = true;
                    startTime = GetTime();
                    PlaySound(enterSound);
                }
            } 
            else if (!finished) {
                timeTaken = GetTime() - startTime;

                // Normal typing
                int key = GetCharPressed();
                while (key > 0) {
                    if ((key >= 32 && key <= 125) && index < MAX_LEN - 1) {
                        userInput[index++] = (char)key;
                        userInput[index] = '\0';
                        PlaySound(keySound);
                    }
                    key = GetCharPressed();
                }

                // Handle ENTER (newline)
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                    if (index < MAX_LEN - 1) {
                        userInput[index++] = '\n';
                        userInput[index] = '\0';
                        PlaySound(enterSound);
                    }
                }

                // Handle BACKSPACE
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (index > 0) {
                        index--;
                        userInput[index] = '\0';
                        PlaySound(backspaceSound);
                    }
                }

                // TAB to finish manually
                if (IsKeyPressed(KEY_TAB)) {
                    finished = true;
                    PlaySound(finishSound);
                }

                if (timeTaken >= levelTimeLimits[level]) {
                    finished = true;
                    PlaySound(finishSound);
                }

                // Target text combine
                char targetText[MAX_LEN] = "";
                for (int i = 0; i < linesCount[level]; i++) {
                    strcat(targetText, levels[level][i]);
                    if (i < linesCount[level] - 1)
                        strcat(targetText, "\n");
                }

                DrawText(TextFormat("Level: %d | Time: %.1f / %.0f", level + 1, timeTaken, levelTimeLimits[level]), 40, 30, 25, WHITE);
                DrawText("Target Text:", 60, 100, 25, WHITE);
                DrawText(targetText, 60, 140, 26, LIGHTGRAY);
                DrawText("Your Input:", 60, 300, 25, WHITE);

                // Draw typed text with colors
                int x = 60, y = 340;
                int lineHeight = 35;
                for (int i = 0; i < index; i++) {
                    char c[2] = {userInput[i], '\0'};
                    Color color = (i < strlen(targetText) && userInput[i] == targetText[i]) ? GREEN : RED;
                    if (userInput[i] == '\n') {
                        y += lineHeight;
                        x = 60;
                    } else {
                        DrawText(c, x, y, 26, color);
                        x += MeasureText(c, 26);
                    }
                }

                // Draw the Blinking Cursor
                if (cursorVisible) {
                    DrawRectangle(x, y, 3, 30, YELLOW); 
                }

                DrawText("[ENTER] New Line | [BACKSPACE] Delete | [TAB] Finish", 60, 660, 20, LIGHTGRAY);
            } 
            else {
                // Calculate level results
                double levelTime = GetTime() - startTime;
                totalTime += levelTime;

                char targetText[MAX_LEN] = "";
                for (int i = 0; i < linesCount[level]; i++) strcat(targetText, levels[level][i]);

                int correctChars = 0;
                for (int i = 0; i < index && targetText[i] != '\0'; i++)
                    if (userInput[i] == targetText[i]) correctChars++;

                // Prevent division by zero checks
                double accuracy = 0.0;
                if (strlen(targetText) > 0) {
                     accuracy = ((double)correctChars / (double)strlen(targetText)) * 100.0;
                }
                
                double wpm = 0.0;
                if (levelTime > 0) {
                    wpm = ((double)index / 5.0) / (levelTime / 60.0);
                }

                // Save results
                levelTimes[level] = levelTime;
                levelAccuracy[level] = accuracy;
                levelWPM[level] = wpm;

                DrawText("LEVEL COMPLETED!", 480, 250, 40, YELLOW);
                DrawText(TextFormat("Time Taken: %.1f sec", levelTime), 500, 320, 25, WHITE);
                DrawText(TextFormat("WPM: %.1f", wpm), 500, 360, 25, WHITE);
                DrawText(TextFormat("Accuracy: %.1f%%", accuracy), 500, 400, 25, WHITE);
                DrawText("Press ENTER for Next Level", 500, 470, 25, LIGHTGRAY);

                if (IsKeyPressed(KEY_ENTER)) {
                    level++;
                    if (level < LEVELS) {
                        started = false;
                        finished = false;
                        index = 0;
                        userInput[0] = '\0';
                        countdownStart = GetTime();
                        PlaySound(enterSound);
                    } else {
                        state = FINAL;
                    }
                }
            }
            break;

        case FINAL:
            DrawText(" FINAL RESULT! ", 500, 150, 40, YELLOW);
            int y = 250;
            for (int i = 0; i < LEVELS; i++) {
                DrawText(TextFormat("LEVEL %d -> Time: %.1f s | WPM: %.1f | Accuracy: %.1f%%",
                    i + 1, levelTimes[i], levelWPM[i], levelAccuracy[i]), 400, y, 25, WHITE);
                y += 50;
            }
            DrawText(TextFormat("TOTAL TIME: %.1f s", totalTime), 520, y + 30, 30, LIGHTGRAY);
            DrawText("Press ESC to Exit", 540, y + 80, 25, LIGHTGRAY);
            break;
        }

        EndDrawing();
    }
    
    UnloadTexture(bg);
    UnloadSound(keySound);
    UnloadSound(enterSound);
    UnloadSound(backspaceSound);
    UnloadSound(finishSound);
    UnloadMusicStream(backgroundMusic); 
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}