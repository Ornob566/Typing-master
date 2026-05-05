#pragma once
#include <string>
#include "Constants.h"

typedef enum { STATE_MENU, STATE_GAME, STATE_FINAL } GameState;

struct LevelStats {
    double wpm       = 0;
    double accuracy  = 0;
    double timeTaken = 0;
};

class GameModel {
public:
    GameState   currentState;
    int         currentLevel;
    bool        isLevelStarted;
    bool        isLevelFinished;

    std::string targetText;
    std::string userInput;

    double timeLimits[TOTAL_LEVELS];
    LevelStats stats[TOTAL_LEVELS];

    double startTime;
    double countdownStart;
    double totalSessionTime;

    GameModel();

    void LoadLevel(int levelIdx);
    void ResetForLevel(double now);
    void CalculateResults(double now);
    bool IsTimeUp(double now) const;
    int  CountdownRemaining(double now) const;
    float TimerRatio(double now) const;
};
