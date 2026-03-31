#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "state.h" // Needed for StateHandler definition

// SOLID: SRP - Coordinates high-level flow, owns all managers, manages state transitions.
typedef struct GameManager {
    UIRenderer* ui;
    InputHandler* input;
    LevelLoader* levels;
    TimerManager* timer;
    StatsManager* stats;
    
    // SOLID: DIP - Depends on the abstraction (StateHandler interface), not concrete implementations.
    const StateHandler* currentState;

    // High-level logical state variables
    int currentLevelIndex;
    char targetTextBuffer[MAX_LEN];
    bool isLevelStarted;
    bool isLevelFinished;
} GameManager;

GameManager* GameManager_Create();
void GameManager_Destroy(GameManager* game);

// The main game loop calls this
void GameManager_RunFrame(GameManager* game);

// SOLID: SRP - function specifically for changing state
void GameManager_ChangeState(GameManager* game, const StateHandler* newState);

#endif