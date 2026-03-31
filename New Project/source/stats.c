#include "stats.h"
#include <stdlib.h>
#include <string.h>

StatsManager* StatsManager_Create() {
    StatsManager* stats = (StatsManager*)malloc(sizeof(StatsManager));
    if (!stats) return NULL;
    memset(stats, 0, sizeof(StatsManager));
    return stats;
}

void StatsManager_Destroy(StatsManager* stats) {
    if (stats) free(stats);
}

void StatsManager_CalculateLevelResult(StatsManager* stats, int levelIndex, const char* target, const char* input, int inputLen, double timeElapsed) {
    if (levelIndex < 0 || levelIndex >= LEVELS) return;

    int correctChars = 0;
    int targetLen = strlen(target);
    for (int i = 0; i < inputLen && i < targetLen; i++) {
        if (input[i] == target[i]) correctChars++;
    }

    stats->results[levelIndex].accuracy = (targetLen > 0) ? ((double)correctChars / targetLen) * 100 : 0;
    // Standard WPM: (all typed characters / 5) / time in minutes
    stats->results[levelIndex].wpm = (timeElapsed > 0) ? (inputLen / 5.0) / (timeElapsed / 60.0) : 0;
    stats->results[levelIndex].time = timeElapsed;
}

void StatsManager_AddTimeToTotal(StatsManager* stats, double time) {
    stats->totalTime += time;
}

const LevelResult* StatsManager_GetResult(StatsManager* stats, int levelIndex) {
    if (levelIndex < 0 || levelIndex >= LEVELS) return NULL;
    return &stats->results[levelIndex];
}

double StatsManager_GetTotalTime(StatsManager* stats) {
    return stats->totalTime;
}