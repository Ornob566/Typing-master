#include "game.h"
#include "ui.h"
#include "input.h"
#include "level.h"
#include "timer.h"
#include "stats.h"
#include <stdlib.h>

GameManager* GameManager_Create() {
    GameManager* game = (GameManager*)malloc(sizeof(GameManager));
    if (!game) return NULL;

    // Initialize Sub-Managers (Constructor Injection)
    game->ui = UIRenderer_Create();
    game->input = InputHandler_Create();
    game->levels = LevelLoader_Create();
    game->timer = TimerManager_Create();
    game->stats = StatsManager_Create();

    game->currentLevelIndex = 0;
    
    // Set Initial State
    game->currentState = NULL;
    GameManager_ChangeState(game, State_GetMenuHandler());

    return game;
}

void GameManager_Destroy(GameManager* game) {
    if (!game) return;
    
    // Call Exit on current state if exists
    if (game->currentState && game->currentState->Exit) {
        game->currentState->Exit(game);
    }

    // Destroy Sub-Managers
    StatsManager_Destroy(game->stats);
    TimerManager_Destroy(game->timer);
    LevelLoader_Destroy(game->levels);
    InputHandler_Destroy(game->input);
    UIRenderer_Destroy(game->ui);

    free(game);
}

void GameManager_ChangeState(GameManager* game, const StateHandler* newState) {
    if (!game || !newState) return;

    // 1. Exit old state
    if (game->currentState && game->currentState->Exit) {
        game->currentState->Exit(game);
    }

    // 2. Switch
    game->currentState = newState;

    // 3. Enter new state
    if (game->currentState->Enter) {
        game->currentState->Enter(game);
    }
}

void GameManager_RunFrame(GameManager* game) {
    if (!game || !game->currentState) return;

    // SOLID: ISP - Update/Logic handled via polymorphic call
    if (game->currentState->Update) {
        game->currentState->Update(game);
    }

    // Rendering flow controlled by GameManager
    UIRenderer_Begin(game->ui);
    
    // SOLID: ISP/Polymorphism - specific state drawing handled via pointer
    if (game->currentState->Draw) {
        game->currentState->Draw(game);
    }
    
    UIRenderer_End();
}