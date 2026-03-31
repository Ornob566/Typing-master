#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Internal helper function (encapsulated)
static int LoadTextLinesFromFile(const char *filename, char lines[MAX_LINES][MAX_LEN]) {
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

LevelLoader* LevelLoader_Create() {
    LevelLoader* loader = (LevelLoader*)malloc(sizeof(LevelLoader));
    if (!loader) return NULL;

    // Hardcoded config matching original code
    loader->loadedLevels[0].timeLimit = 45;
    loader->loadedLevels[1].timeLimit = 75;
    loader->loadedLevels[2].timeLimit = 120;

    loader->loadedLevels[0].linesCount = LoadTextLinesFromFile("level1.txt", loader->loadedLevels[0].lines);
    loader->loadedLevels[1].linesCount = LoadTextLinesFromFile("level2.txt", loader->loadedLevels[1].lines);
    loader->loadedLevels[2].linesCount = LoadTextLinesFromFile("level3.txt", loader->loadedLevels[2].lines);

    return loader;
}

void LevelLoader_Destroy(LevelLoader* loader) {
    if (loader) free(loader);
}

const LevelData* LevelLoader_GetLevel(LevelLoader* loader, int levelIndex) {
    if (!loader || levelIndex < 0 || levelIndex >= LEVELS) return NULL;
    return &loader->loadedLevels[levelIndex];
}