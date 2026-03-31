#include "state.h"
#include "game.h"
#include "ui.h"
#include "input.h"
#include "level.h"
#include "timer.h"
#include "stats.h"
#include <string.h>

// Helper to build target string (encapsulated)
static void BuildTargetString(const LevelData* data, char *outStr) {
    outStr[0] = '\0';
    if (!data) return;
    for (int i = 0; i < data->linesCount; i++) {
        strcat(outStr, data->lines[i]);
        if (i < data->linesCount - 1) strcat(outStr, "\n");
    }
}

// ============================================================================
// CONCRETE STATE: MENU
// ============================================================================
static void Menu_Enter(GameManager* game) {
    InputHandler_Reset(game->input);
    game->currentLevelIndex = 0;
}

static void Menu_Update(GameManager* game) {
    if (IsKeyPressed(KEY_ENTER)) {
        GameManager_ChangeState(game, State_GetGameHandler());
    }
}

static void Menu_Draw(GameManager* game) {
    UIRenderer_DrawMenu(game->ui);
}

// Create the interface struct for Menu
static const StateHandler menuHandler = {
    Menu_Enter, Menu_Update, Menu_Draw, NULL // Exit is NULL (optional)
};

const StateHandler* State_GetMenuHandler() { return &menuHandler; }


// ============================================================================
// CONCRETE STATE: GAME (Sub-states handled logically within)
// ============================================================================
static void Game_Enter(GameManager* game) {
    game->isLevelStarted = false;
    game->isLevelFinished = false;
    InputHandler_Reset(game->input);
    TimerManager_StartCountdown(game->timer);
    
    // Setup target text
    const LevelData* lvl = LevelLoader_GetLevel(game->levels, game->currentLevelIndex);
    BuildTargetString(lvl, game->targetTextBuffer);
}

static void Game_Update(GameManager* game) {
    if (!game->isLevelStarted) {
        // Countdown phase
        if (TimerManager_GetCountdownRemaining(game->timer) <= 0) {
            game->isLevelStarted = true;
            TimerManager_StartLevelTimer(game->timer);
        }
    } 
    else if (!game->isLevelFinished) {
        // Typing phase
        double timeElapsed = TimerManager_GetElapsed(game->timer);
        const LevelData* lvl = LevelLoader_GetLevel(game->levels, game->currentLevelIndex);
        
        InputHandler_Update(game->input);
        TimerManager_Update(game->timer); // Blink logic

        // Level finishing conditions
        if (IsKeyPressed(KEY_TAB) || timeElapsed >= lvl->timeLimit) {
            game->isLevelFinished = true;
            
            // Calculate results immediately upon finish
            StatsManager_CalculateLevelResult(
                game->stats, 
                game->currentLevelIndex, 
                game->targetTextBuffer, 
                InputHandler_GetBuffer(game->input), 
                InputHandler_GetIndex(game->input), 
                timeElapsed
            );
        }
    } 
    else {
        // Result phase
        if (IsKeyPressed(KEY_ENTER)) {
            // Add time to total
            double levelTime = TimerManager_GetElapsed(game->timer);
            StatsManager_AddTimeToTotal(game->stats, levelTime);

            game->currentLevelIndex++;
            if (game->currentLevelIndex < LEVELS) {
                // Re-enter Game state for next level (reset variables)
                Game_Enter(game); 
            } else {
                GameManager_ChangeState(game, State_GetFinalHandler());
            }
        }
    }
}

static void Game_Draw(GameManager* game) {
    if (!game->isLevelStarted) {
        UIRenderer_DrawCountdown(game->ui, TimerManager_GetCountdownRemaining(game->timer));
    } 
    else if (!game->isLevelFinished) {
        const LevelData* lvl = LevelLoader_GetLevel(game->levels, game->currentLevelIndex);
        UIRenderer_DrawGameSub(
            game->ui,
            game->currentLevelIndex,
            TimerManager_GetElapsed(game->timer),
            lvl->timeLimit,
            game->targetTextBuffer,
            InputHandler_GetBuffer(game->input),
            InputHandler_GetIndex(game->input),
            TimerManager_IsCursorVisible(game->timer)
        );
    } 
    else {
        const LevelResult* res = StatsManager_GetResult(game->stats, game->currentLevelIndex);
        UIRenderer_DrawLevelResult(game->ui, res);
    }
}

static const StateHandler gameHandler = {
    Game_Enter, Game_Update, Game_Draw, NULL
};

const StateHandler* State_GetGameHandler() { return &gameHandler; }


// ============================================================================
// CONCRETE STATE: FINAL
// ============================================================================
static void Final_Draw(GameManager* game) {
    // Collect data needed for drawing from stats manager
    LevelResult results[LEVELS];
    for(int i=0; i<LEVELS; i++) {
        results[i] = *StatsManager_GetResult(game->stats, i);
    }
    double total = StatsManager_GetTotalTime(game->stats);

    UIRenderer_DrawFinalStats(game->ui, results, total);
}

static const StateHandler finalHandler = {
    NULL, NULL, Final_Draw, NULL // Only Draw is needed here
};

const StateHandler* State_GetFinalHandler() { return &finalHandler; }