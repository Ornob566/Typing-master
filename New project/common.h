#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"

// Constant settings
#define MAX_LEN 1000
#define MAX_LINES 10
#define COUNTDOWN 3
#define LEVELS 3

// Screen settings
#define SCREEN_WIDTH 1300
#define SCREEN_HEIGHT 720

// Colour pallet
#define COLOR_BG      (Color){ 18, 18, 22, 255 } 
#define COLOR_CARD    (Color){ 30, 30, 35, 255 }  
#define COLOR_ACCENT  (Color){ 0, 170, 255, 255 } 
#define COLOR_TEXT    (Color){ 200, 200, 200, 255 } 

// Opaque struct definitions for Type Safety
typedef struct GameManager GameManager;
typedef struct UIRenderer UIRenderer;
typedef struct InputHandler InputHandler;
typedef struct LevelLoader LevelLoader;
typedef struct TimerManager TimerManager;
typedef struct StatsManager StatsManager;
typedef struct StateHandler StateHandler; // The "Interface" for states

#endif