#ifndef UI_H
#define UI_H

#include "common.h"
#include "stats.h" // Needed for struct LevelResult definitions in Final Stats

// SOLID: SRP - solely responsible for Drawing Raylib elements. Depends on primitives, not managers.
typedef struct UIRenderer {
    Texture2D background;
} UIRenderer;

UIRenderer* UIRenderer_Create();
void UIRenderer_Destroy(UIRenderer* ui);

// SOLID: ISP/DIP - Functions accept specific data needed for drawing, not whole manager objects
void UIRenderer_Begin(UIRenderer* ui);
void UIRenderer_End();

void UIRenderer_DrawMenu(UIRenderer* ui);

void UIRenderer_DrawCountdown(UIRenderer* ui, int remaining);

void UIRenderer_DrawGameSub(UIRenderer* ui, int currentLevel, double timeElapsed, double timeLimit, 
                            const char* targetText, const char* userInput, int inputIndex, bool cursorVisible);

void UIRenderer_DrawLevelResult(UIRenderer* ui, const LevelResult* result);

void UIRenderer_DrawFinalStats(UIRenderer* ui, const LevelResult results[LEVELS], double totalTime);

#endif