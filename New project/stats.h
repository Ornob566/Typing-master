#ifndef STATS_H
#define STATS_H

#include "common.h"

typedef struct LevelResult {
    double wpm;
    double accuracy;
    double time;
} LevelResult;

// SOLID: SRP - Calculates and stores gameplay statistics.
typedef struct StatsManager {
    LevelResult results[LEVELS];
    double totalTime;
} StatsManager;

StatsManager* StatsManager_Create();
void StatsManager_Destroy(StatsManager* stats);

void StatsManager_CalculateLevelResult(StatsManager* stats, int levelIndex, const char* target, const char* input, int inputLen, double timeElapsed);
void StatsManager_AddTimeToTotal(StatsManager* stats, double time);

const LevelResult* StatsManager_GetResult(StatsManager* stats, int levelIndex);
double StatsManager_GetTotalTime(StatsManager* stats);

#endif