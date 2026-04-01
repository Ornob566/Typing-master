#ifndef COMMON_HPP
#define COMMON_HPP
#include "raylib.h"
#include <string>

const int MAX_LEN = 1000;
const int TOTAL_LEVELS = 3;

struct LevelStats {
    double wpm = 0;
    double accuracy = 0;
    double timeTaken = 0;
};

// Color Palette
const Color COLOR_BG = { 18, 18, 22, 255 };
const Color COLOR_CARD = { 30, 30, 35, 255 };
const Color COLOR_ACCENT = { 0, 170, 255, 255 };
#endif