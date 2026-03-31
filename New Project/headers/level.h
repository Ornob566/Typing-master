#ifndef LEVEL_H
#define LEVEL_H

#include "common.h"

// SOLID: SRP - Solely responsible for loading and holding raw level data.
typedef struct LevelData {
    char lines[MAX_LINES][MAX_LEN];
    int linesCount;
    double timeLimit;
} LevelData;

typedef struct LevelLoader {
    LevelData loadedLevels[LEVELS];
} LevelLoader;

LevelLoader* LevelLoader_Create();
void LevelLoader_Destroy(LevelLoader* loader);

// Returns NULL if level index is invalid
const LevelData* LevelLoader_GetLevel(LevelLoader* loader, int levelIndex);

#endif