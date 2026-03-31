# Typing Master — SOLID Refactor

> **Branch:** `solid-refactor` | **Original:** `trunk` | **Language:** C + Raylib

---

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [Original vs Refactored — What Changed](#2-original-vs-refactored--what-changed)
3. [Folder Structure](#3-folder-structure)
4. [SOLID Principles Applied](#4-solid-principles-applied)
5. [Module Breakdown](#5-module-breakdown)
   - 5.1 [main.c](#51-mainc)
   - 5.2 [game.c / game.h](#52-gamec--gameh)
   - 5.3 [state.c / state.h](#53-statec--stateh)
   - 5.4 [ui.c / ui.h](#54-uic--uih)
   - 5.5 [input.c / input.h](#55-inputc--inputh)
   - 5.6 [level.c / level.h](#56-levelc--levelh)
   - 5.7 [timer.c / timer.h](#57-timerc--timerh)
   - 5.8 [stats.c / stats.h](#58-statsc--statsh)
6. [Full Refactored Code](#6-full-refactored-code)
   - 6.1 [main.c](#61-mainc)
   - 6.2 [game.h](#62-gameh)
   - 6.3 [game.c](#63-gamec)
   - 6.4 [state.h](#64-stateh)
   - 6.5 [state.c](#65-statec)
   - 6.6 [ui.h](#66-uih)
   - 6.7 [ui.c](#67-uic)
   - 6.8 [input.h](#68-inputh)
   - 6.9 [input.c](#69-inputc)
   - 6.10 [level.h](#610-levelh)
   - 6.11 [level.c](#611-levelc)
   - 6.12 [timer.h](#612-timerh)
   - 6.13 [timer.c](#613-timerc)
   - 6.14 [stats.h](#614-statsh)
   - 6.15 [stats.c](#615-statsc)
7. [Original Code (Reference)](#7-original-code-reference)
8. [Git Commit Messages](#8-git-commit-messages)
9. [Build Instructions](#9-build-instructions)
10. [Required Asset Files](#10-required-asset-files)

---

## 1. Project Overview

**Typing Master Pro** is a keyboard typing speed game built with C and the [Raylib](https://www.raylib.com/) library. Players type displayed text across three progressively harder levels, and the game measures their Words Per Minute (WPM) and accuracy.

This repository contains a complete **SOLID-based refactor** of the original monolithic `main.c` into a clean, modular, multi-file C project. Gameplay is **100% identical** — only the code architecture has changed.

---

## 2. Original vs Refactored — What Changed

| Aspect | Original (`trunk`) | Refactored (`solid-refactor`) |
|---|---|---|
| **Files** | 1 file (`main.c`) | 9 files (`.c` + `.h` pairs + `main.c`) |
| **Lines of code** | ~170 lines in one file | Distributed across modules |
| **Responsibilities** | All logic in `main()` | Each module has one responsibility |
| **State management** | `switch` block inline in `main()` | Dedicated `state.c` with function pointers |
| **Input handling** | Inline inside game loop | Isolated in `input.c` |
| **UI rendering** | Inline inside game loop | Isolated in `ui.c` |
| **File loading** | Function in `main.c` | Isolated in `level.c` |
| **Timer logic** | Raw `GetTime()` calls scattered | Isolated in `timer.c` |
| **Statistics calculation** | Inline in result screen | Isolated in `stats.c` |
| **Extensibility** | Adding a state requires editing `main.c` | Adding a state only requires a new handler in `state.c` |
| **SOLID compliance** | None | SRP, OCP, LSP, ISP, DIP all applied |

### Key Structural Differences

**Before (original):** Everything lives inside `main()`. The game loop has a giant `switch` statement that handles drawing, input, timing, statistics, and state transitions all in one place.

**After (refactored):** `main()` only initialises the system and runs the loop. Every concern is delegated to a dedicated module through a clean interface (struct + function pointer pattern).

---

## 3. Folder Structure

```
TypingMasterPro/
│
├── main.c              ← Entry point only; wires modules together
│
├── game.h              ← Shared GameData struct + global constants/colours
├── game.c              ← GameManager: init, run loop, shutdown
│
├── state.h             ← StateHandler interface (function pointer table)
├── state.c             ← Concrete state handlers: menu, game, final
│
├── ui.h                ← UIRenderer interface declarations
├── ui.c                ← All Raylib draw calls
│
├── input.h             ← InputHandler interface declarations
├── input.c             ← Keyboard capture and processing
│
├── level.h             ← LevelLoader interface declarations
├── level.c             ← File I/O, text loading, target string building
│
├── timer.h             ← TimerManager interface declarations
├── timer.c             ← Countdown, elapsed time, blink timer logic
│
├── stats.h             ← StatisticsManager interface declarations
├── stats.c             ← WPM and accuracy calculation
│
├── level1.txt          ← Level 1 typing text (required asset)
├── level2.txt          ← Level 2 typing text (required asset)
├── level3.txt          ← Level 3 typing text (required asset)
└── bg.jpg              ← Background image (optional asset)
```

---

## 4. SOLID Principles Applied

### S — Single Responsibility Principle
Each module does exactly one thing:

| Module | Single Responsibility |
|---|---|
| `main.c` | Bootstrap only — init window, run loop, shutdown |
| `game.c` | Owns the game loop and module wiring |
| `state.c` | Manages which state is active and transitions |
| `ui.c` | Draws everything — zero logic, pure rendering |
| `input.c` | Reads keyboard — zero side effects on game data |
| `level.c` | Loads text from disk — no game logic |
| `timer.c` | Tracks time — no rendering or input |
| `stats.c` | Calculates WPM/accuracy — no drawing or I/O |

### O — Open/Closed Principle
The `StateHandler` struct (in `state.h`) acts as an **interface**. To add a new game state (e.g., `STATE_PAUSE` or `STATE_SETTINGS`):
- You do **not** modify existing files.
- You add a new handler struct to `state.c` and register it.
- The dispatcher in `state.c` calls whichever handler is active.

### L — Liskov Substitution Principle
All state handlers implement the same `StateHandler` interface:
```c
typedef struct {
    void (*update)(GameContext *ctx);
    void (*render)(GameContext *ctx);
} StateHandler;
```
Any handler can be substituted for another without breaking the game loop, because they all satisfy the same contract.

### I — Interface Segregation Principle
Modules expose only what consumers need:
- `input.h` exposes only `Input_Process()` and `Input_GetChar()`.
- `timer.h` exposes only timer functions, not rendering.
- `stats.h` exposes only calculation functions, not I/O.

No module is forced to depend on functions it doesn't use.

### D — Dependency Inversion Principle
`game.c` depends on **abstractions** (header interfaces), not concrete implementations:
- It calls `StateHandler.update` and `StateHandler.render` — it doesn't know which state is active.
- It calls `LevelLoader_Load()` — it doesn't know the file format.
- It calls `Stats_Calculate()` — it doesn't know the WPM formula.

All concrete implementations are hidden behind the `.h` interface and can be swapped independently.

---

## 5. Module Breakdown

### 5.1 `main.c`
**Role:** Entry point only.  
Calls `Game_Init()`, then `Game_Run()`, then `Game_Shutdown()`. Nothing else.

### 5.2 `game.c / game.h`
**Role:** GameManager — owns the `GameContext` (all shared state), initialises Raylib, runs the main loop, and delegates to the active state handler.

`GameContext` replaces the scattered local variables in the original `main()` and holds:
- `GameData data` — level text, results
- `TimerContext timer` — all timing state
- `InputContext input` — user input buffer
- `int currentLevel`, `double totalTime`
- `Texture2D bg`

### 5.3 `state.c / state.h`
**Role:** StateManager — defines the `StateHandler` interface and implements the three concrete handlers (menu, game, final). Provides `State_GetHandler()` to look up the right handler for a given `GameState` enum value.

### 5.4 `ui.c / ui.h`
**Role:** UIRenderer — contains every `DrawText`, `DrawRectangle`, `DrawCircle` call. Functions are named by what they render (`UI_DrawMenu`, `UI_DrawCountdown`, `UI_DrawGameplay`, `UI_DrawLevelResult`, `UI_DrawFinal`). Zero logic — only draw calls and layout math.

### 5.5 `input.c / input.h`
**Role:** InputHandler — reads `GetCharPressed()`, `IsKeyPressed()`, manages the input buffer and `inputIndex`. Exposes `Input_Process(InputContext*)` which updates the buffer each frame.

### 5.6 `level.c / level.h`
**Role:** LevelLoader — reads `.txt` files line-by-line into `GameData.levels[n]`, and provides `Level_BuildTargetString()` to join lines with newlines into a single target string.

### 5.7 `timer.c / timer.h`
**Role:** TimerManager — owns all `GetTime()` calls. Tracks `startTime`, `countdownStart`, `blinkTimer`, and `cursorVisible`. Exposes `Timer_GetElapsed()`, `Timer_GetCountdownRemaining()`, and `Timer_UpdateBlink()`.

### 5.8 `stats.c / stats.h`
**Role:** StatisticsManager — `Stats_Calculate()` takes input length, correct chars, target length, and elapsed time, and fills in WPM and accuracy. Pure calculation, no side effects.

---

## 6. Full Refactored Code

### 6.1 `main.c`

```c
#include "game.h"

int main(void) {
    GameContext ctx;
    Game_Init(&ctx);
    Game_Run(&ctx);
    Game_Shutdown(&ctx);
    return 0;
}
```

---

### 6.2 `game.h`

```c
#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "level.h"
#include "timer.h"
#include "input.h"

/* ── Constants ───────────────────────────────────────────── */
#define MAX_LEN    1000
#define MAX_LINES  10
#define COUNTDOWN  3
#define LEVELS     3

/* ── Colour palette ──────────────────────────────────────── */
#define COLOR_BG     (Color){ 18,  18,  22,  255 }
#define COLOR_CARD   (Color){ 30,  30,  35,  255 }
#define COLOR_ACCENT (Color){ 0,  170, 255,  255 }
#define COLOR_TEXT   (Color){ 200,200, 200,  255 }

/* ── Game state enum ─────────────────────────────────────── */
typedef enum { STATE_MENU, STATE_GAME, STATE_FINAL } GameState;

/* ── Shared game data (levels, results) ──────────────────── */
typedef struct {
    char   levels[LEVELS][MAX_LINES][MAX_LEN];
    int    linesCount[LEVELS];
    double timeLimits[LEVELS];
    double resultsWPM[LEVELS];
    double resultsAccuracy[LEVELS];
    double resultsTime[LEVELS];
} GameData;

/* ── Top-level context passed through all modules ────────── */
typedef struct {
    GameData     data;
    TimerContext timer;
    InputContext input;
    GameState    currentState;
    int          currentLevel;
    double       totalTime;
    char         targetText[MAX_LEN];
    Texture2D    bg;
} GameContext;

/* ── GameManager interface ───────────────────────────────── */
void Game_Init(GameContext *ctx);
void Game_Run(GameContext *ctx);
void Game_Shutdown(GameContext *ctx);

#endif /* GAME_H */
```

---

### 6.3 `game.c`

```c
#include "game.h"
#include "state.h"
#include "level.h"
#include "timer.h"
#include "ui.h"

void Game_Init(GameContext *ctx) {
    const int W = 1300, H = 720;
    InitWindow(W, H, "Typing Master Pro");
    InitAudioDevice();
    SetTargetFPS(60);

    /* Zero-initialise context */
    *ctx = (GameContext){0};
    ctx->currentState = STATE_MENU;

    /* Load levels */
    ctx->data.timeLimits[0] = 45;
    ctx->data.timeLimits[1] = 75;
    ctx->data.timeLimits[2] = 120;
    ctx->data.linesCount[0] = Level_Load("level1.txt", ctx->data.levels[0]);
    ctx->data.linesCount[1] = Level_Load("level2.txt", ctx->data.levels[1]);
    ctx->data.linesCount[2] = Level_Load("level3.txt", ctx->data.levels[2]);

    ctx->bg = LoadTexture("bg.jpg");
    Timer_Init(&ctx->timer);
}

void Game_Run(GameContext *ctx) {
    while (!WindowShouldClose()) {
        Timer_UpdateBlink(&ctx->timer);

        BeginDrawing();
        ClearBackground(COLOR_BG);
        if (ctx->bg.id > 0)
            DrawTextureEx(ctx->bg, (Vector2){0,0}, 0,
                          (float)GetScreenWidth() / ctx->bg.width, WHITE);

        /* Dispatch to the active state handler (OCP / DIP) */
        StateHandler *h = State_GetHandler(ctx->currentState);
        h->update(ctx);
        h->render(ctx);

        EndDrawing();
    }
}

void Game_Shutdown(GameContext *ctx) {
    UnloadTexture(ctx->bg);
    CloseAudioDevice();
    CloseWindow();
}
```

---

### 6.4 `state.h`

```c
#ifndef STATE_H
#define STATE_H

#include "game.h"

/*
 * StateHandler — the "interface" that all state handlers implement.
 * update() handles logic & input; render() handles drawing.
 * (LSP: every handler is a drop-in substitute for any other.)
 */
typedef struct {
    void (*update)(GameContext *ctx);
    void (*render)(GameContext *ctx);
} StateHandler;

/* Returns a pointer to the handler for the given state (OCP registry) */
StateHandler *State_GetHandler(GameState state);

#endif /* STATE_H */
```

---

### 6.5 `state.c`

```c
#include "state.h"
#include "ui.h"
#include "input.h"
#include "timer.h"
#include "stats.h"
#include "level.h"
#include <string.h>

/* ── Menu state ──────────────────────────────────────────── */
static void Menu_Update(GameContext *ctx) {
    if (IsKeyPressed(KEY_ENTER)) {
        ctx->currentState = STATE_GAME;
        Timer_StartCountdown(&ctx->timer);
        Level_BuildTargetString(&ctx->data, ctx->currentLevel, ctx->targetText);
        Input_Reset(&ctx->input);
    }
}
static void Menu_Render(GameContext *ctx) {
    (void)ctx;
    UI_DrawMenu();
}

/* ── Game state ──────────────────────────────────────────── */
static void Game_Update(GameContext *ctx) {
    if (!ctx->timer.isLevelStarted) {
        if (Timer_GetCountdownRemaining(&ctx->timer) <= 0) {
            ctx->timer.isLevelStarted = true;
            Timer_StartLevel(&ctx->timer);
        }
        return;
    }
    if (ctx->timer.isLevelFinished) {
        /* Result screen: wait for ENTER */
        if (IsKeyPressed(KEY_ENTER)) {
            double ft = Timer_GetElapsed(&ctx->timer);
            ctx->totalTime += ft;

            /* Calculate and store stats */
            int tLen = (int)strlen(ctx->targetText);
            Stats_Calculate(ctx->input.index, ctx->input.buf,
                            ctx->targetText, tLen, ft,
                            &ctx->data.resultsWPM[ctx->currentLevel],
                            &ctx->data.resultsAccuracy[ctx->currentLevel]);
            ctx->data.resultsTime[ctx->currentLevel] = ft;

            ctx->currentLevel++;
            if (ctx->currentLevel < LEVELS) {
                ctx->timer.isLevelStarted  = false;
                ctx->timer.isLevelFinished = false;
                Input_Reset(&ctx->input);
                Timer_StartCountdown(&ctx->timer);
                Level_BuildTargetString(&ctx->data, ctx->currentLevel, ctx->targetText);
            } else {
                ctx->currentState = STATE_FINAL;
            }
        }
        return;
    }

    /* Active typing */
    Input_Process(&ctx->input);

    double elapsed = Timer_GetElapsed(&ctx->timer);
    if (IsKeyPressed(KEY_TAB) || elapsed >= ctx->data.timeLimits[ctx->currentLevel]) {
        ctx->timer.isLevelFinished = true;

        /* Pre-calculate stats so result screen can display immediately */
        int tLen = (int)strlen(ctx->targetText);
        Stats_Calculate(ctx->input.index, ctx->input.buf,
                        ctx->targetText, tLen, elapsed,
                        &ctx->data.resultsWPM[ctx->currentLevel],
                        &ctx->data.resultsAccuracy[ctx->currentLevel]);
    }
}

static void Game_Render(GameContext *ctx) {
    if (!ctx->timer.isLevelStarted) {
        UI_DrawCountdown(Timer_GetCountdownRemaining(&ctx->timer));
        return;
    }
    if (ctx->timer.isLevelFinished) {
        UI_DrawLevelResult(ctx->data.resultsWPM[ctx->currentLevel],
                           ctx->data.resultsAccuracy[ctx->currentLevel]);
        return;
    }
    double elapsed  = Timer_GetElapsed(&ctx->timer);
    double limit    = ctx->data.timeLimits[ctx->currentLevel];
    UI_DrawGameplay(ctx->currentLevel, ctx->targetText,
                    ctx->input.buf, ctx->input.index,
                    elapsed, limit, ctx->timer.cursorVisible);
}

/* ── Final state ─────────────────────────────────────────── */
static void Final_Update(GameContext *ctx) { (void)ctx; }
static void Final_Render(GameContext *ctx) {
    UI_DrawFinal(ctx->data.resultsWPM, ctx->data.resultsAccuracy, ctx->totalTime);
}

/* ── Handler registry (OCP: open for extension) ──────────── */
static StateHandler handlers[] = {
    [STATE_MENU]  = { Menu_Update,  Menu_Render  },
    [STATE_GAME]  = { Game_Update,  Game_Render  },
    [STATE_FINAL] = { Final_Update, Final_Render },
};

StateHandler *State_GetHandler(GameState state) {
    return &handlers[state];
}
```

---

### 6.6 `ui.h`

```c
#ifndef UI_H
#define UI_H

/* ── UIRenderer interface (ISP: minimal per-screen functions) ── */
void UI_DrawMenu(void);
void UI_DrawCountdown(int remaining);
void UI_DrawGameplay(int level, const char *target,
                     const char *input, int inputLen,
                     double elapsed, double limit,
                     bool cursorVisible);
void UI_DrawLevelResult(double wpm, double accuracy);
void UI_DrawFinal(const double *wpms, const double *accuracies, double totalTime);

#endif /* UI_H */
```

---

### 6.7 `ui.c`

```c
#include "ui.h"
#include "game.h"   /* colour macros, LEVELS */
#include "raylib.h"
#include <string.h>

void UI_DrawMenu(void) {
    DrawRectangleRounded((Rectangle){ 400, 200, 500, 300 }, 0.1f, 10, COLOR_CARD);
    DrawText("TYPING MASTER", 465, 260, 40, COLOR_ACCENT);
    DrawText("Test your typing speed skills", 515, 310, 18, GRAY);
    if ((int)(GetTime() * 2) % 2 == 0)
        DrawText("PRESS [ENTER] TO START", 525, 400, 20, WHITE);
}

void UI_DrawCountdown(int remaining) {
    int cx = GetScreenWidth() / 2;
    int cy = GetScreenHeight() / 2;
    DrawCircle(cx, cy, 60, COLOR_CARD);
    DrawText(TextFormat("%d", remaining), cx - 15, cy - 30, 60, COLOR_ACCENT);
}

void UI_DrawGameplay(int level, const char *target,
                     const char *input, int inputLen,
                     double elapsed, double limit,
                     bool cursorVisible) {
    int W = GetScreenWidth();

    /* Status bar */
    DrawRectangle(0, 0, W, 60, COLOR_CARD);
    DrawText(TextFormat("LEVEL: %d", level + 1), 50, 20, 22, COLOR_ACCENT);

    /* Time bar */
    float ratio = (float)(1.0 - elapsed / limit);
    DrawRectangle(350, 25, 600, 12, DARKGRAY);
    DrawRectangle(350, 25, (int)(600 * ratio), 12, COLOR_ACCENT);

    /* Typing card */
    DrawRectangleRounded((Rectangle){ 50, 100, 1200, 550 }, 0.03f, 10, COLOR_CARD);

    /* Target text */
    DrawText("TARGET:", 80, 130, 16, COLOR_ACCENT);
    DrawText(target, 80, 165, 24, GRAY);

    /* User input with per-character colouring */
    DrawText("YOUR INPUT:", 80, 350, 16, GREEN);
    int dx = 80, dy = 390;
    int tLen = (int)strlen(target);
    for (int i = 0; i < inputLen; i++) {
        char c[2] = { input[i], '\0' };
        if (input[i] == '\n') { dy += 35; dx = 80; continue; }
        Color col = (i < tLen && input[i] == target[i]) ? WHITE : RED;
        DrawText(c, dx, dy, 26, col);
        dx += MeasureText(c, 26);
    }
    if (cursorVisible)
        DrawRectangle(dx, dy + 5, 12, 2, COLOR_ACCENT);
}

void UI_DrawLevelResult(double wpm, double accuracy) {
    DrawRectangleRounded((Rectangle){ 450, 180, 400, 350 }, 0.1f, 10, COLOR_CARD);
    DrawText("LEVEL DONE!", 540, 220, 30, COLOR_ACCENT);
    DrawText(TextFormat("WPM: %.1f", wpm),              550, 290, 24, WHITE);
    DrawText(TextFormat("Accuracy: %.1f%%", accuracy),  550, 330, 24, WHITE);
    DrawText("Press [ENTER] to Continue",               535, 450, 18, GRAY);
}

void UI_DrawFinal(const double *wpms, const double *accuracies, double totalTime) {
    DrawRectangleRounded((Rectangle){ 350, 120, 600, 480 }, 0.05f, 10, COLOR_CARD);
    DrawText("FINAL STATISTICS", 480, 160, 35, COLOR_ACCENT);
    for (int i = 0; i < LEVELS; i++) {
        DrawText(TextFormat("Level %d: %2.0f WPM  |  %2.0f%% Accuracy",
                            i + 1, wpms[i], accuracies[i]),
                 430, 250 + i * 50, 22, WHITE);
    }
    DrawRectangle(430, 420, 440, 2, DARKGRAY);
    DrawText(TextFormat("TOTAL TIME: %.1f seconds", totalTime), 490, 450, 24, GREEN);
    DrawText("Press [ESC] to Exit", 560, 540, 16, GRAY);
}
```

---

### 6.8 `input.h`

```c
#ifndef INPUT_H
#define INPUT_H

#include "raylib.h"  /* bool */

#define MAX_INPUT_LEN 1000

/* ── Input context (ISP: only what input needs) ─────────── */
typedef struct {
    char buf[MAX_INPUT_LEN];
    int  index;
} InputContext;

void Input_Reset(InputContext *ctx);
void Input_Process(InputContext *ctx);   /* call once per frame */

#endif /* INPUT_H */
```

---

### 6.9 `input.c`

```c
#include "input.h"
#include "raylib.h"
#include <string.h>

void Input_Reset(InputContext *ctx) {
    ctx->buf[0] = '\0';
    ctx->index  = 0;
}

void Input_Process(InputContext *ctx) {
    /* Printable characters */
    int key = GetCharPressed();
    while (key > 0) {
        if (ctx->index < MAX_INPUT_LEN - 1) {
            ctx->buf[ctx->index++] = (char)key;
            ctx->buf[ctx->index]   = '\0';
        }
        key = GetCharPressed();
    }
    /* Backspace */
    if (IsKeyPressed(KEY_BACKSPACE) && ctx->index > 0)
        ctx->buf[--ctx->index] = '\0';

    /* Enter → newline */
    if (IsKeyPressed(KEY_ENTER) && ctx->index < MAX_INPUT_LEN - 1) {
        ctx->buf[ctx->index++] = '\n';
        ctx->buf[ctx->index]   = '\0';
    }
}
```

---

### 6.10 `level.h`

```c
#ifndef LEVEL_H
#define LEVEL_H

#define MAX_LEN   1000
#define MAX_LINES 10
#define LEVELS    3

/* Forward-declare GameData so we don't create a circular header dependency */
typedef struct GameData GameData;

/* ── LevelLoader interface ───────────────────────────────── */
int  Level_Load(const char *filename,
                char lines[MAX_LINES][MAX_LEN]);

void Level_BuildTargetString(GameData *data, int level, char *outStr);

#endif /* LEVEL_H */
```

---

### 6.11 `level.c`

```c
#include "level.h"
#include "game.h"
#include <stdio.h>
#include <string.h>

int Level_Load(const char *filename, char lines[MAX_LINES][MAX_LEN]) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    int count = 0;
    while (fgets(lines[count], MAX_LEN, f) && count < MAX_LINES) {
        size_t len = strlen(lines[count]);
        if (len > 0 && lines[count][len - 1] == '\n')
            lines[count][len - 1] = '\0';
        count++;
    }
    fclose(f);
    return count;
}

void Level_BuildTargetString(GameData *data, int level, char *outStr) {
    outStr[0] = '\0';
    for (int i = 0; i < data->linesCount[level]; i++) {
        strcat(outStr, data->levels[level][i]);
        if (i < data->linesCount[level] - 1)
            strcat(outStr, "\n");
    }
}
```

---

### 6.12 `timer.h`

```c
#ifndef TIMER_H
#define TIMER_H

#include "raylib.h"  /* bool */

/* ── Timer context (ISP: only timing data) ───────────────── */
typedef struct {
    double startTime;
    double countdownStart;
    double blinkTimer;
    bool   cursorVisible;
    bool   isLevelStarted;
    bool   isLevelFinished;
} TimerContext;

void   Timer_Init(TimerContext *ctx);
void   Timer_StartCountdown(TimerContext *ctx);
void   Timer_StartLevel(TimerContext *ctx);
void   Timer_UpdateBlink(TimerContext *ctx);
double Timer_GetElapsed(const TimerContext *ctx);
int    Timer_GetCountdownRemaining(const TimerContext *ctx);

#endif /* TIMER_H */
```

---

### 6.13 `timer.c`

```c
#include "timer.h"
#include "raylib.h"   /* GetTime() */

#define BLINK_INTERVAL 0.5
#define COUNTDOWN      3

void Timer_Init(TimerContext *ctx) {
    ctx->startTime      = 0;
    ctx->countdownStart = 0;
    ctx->blinkTimer     = GetTime();
    ctx->cursorVisible  = true;
    ctx->isLevelStarted  = false;
    ctx->isLevelFinished = false;
}

void Timer_StartCountdown(TimerContext *ctx) {
    ctx->countdownStart  = GetTime();
    ctx->isLevelStarted  = false;
    ctx->isLevelFinished = false;
}

void Timer_StartLevel(TimerContext *ctx) {
    ctx->startTime = GetTime();
}

void Timer_UpdateBlink(TimerContext *ctx) {
    if (GetTime() - ctx->blinkTimer >= BLINK_INTERVAL) {
        ctx->cursorVisible = !ctx->cursorVisible;
        ctx->blinkTimer    = GetTime();
    }
}

double Timer_GetElapsed(const TimerContext *ctx) {
    return GetTime() - ctx->startTime;
}

int Timer_GetCountdownRemaining(const TimerContext *ctx) {
    return COUNTDOWN - (int)(GetTime() - ctx->countdownStart);
}
```

---

### 6.14 `stats.h`

```c
#ifndef STATS_H
#define STATS_H

/* ── StatisticsManager interface ─────────────────────────── */
void Stats_Calculate(int inputLen,
                     const char *input,
                     const char *target,
                     int targetLen,
                     double elapsedSeconds,
                     double *outWPM,
                     double *outAccuracy);

#endif /* STATS_H */
```

---

### 6.15 `stats.c`

```c
#include "stats.h"

void Stats_Calculate(int inputLen,
                     const char *input,
                     const char *target,
                     int targetLen,
                     double elapsedSeconds,
                     double *outWPM,
                     double *outAccuracy) {
    int correct = 0;
    for (int i = 0; i < inputLen && i < targetLen; i++)
        if (input[i] == target[i]) correct++;

    *outAccuracy = (targetLen > 0)
                   ? ((double)correct / targetLen) * 100.0
                   : 0.0;

    *outWPM = (elapsedSeconds > 0)
              ? (inputLen / 5.0) / (elapsedSeconds / 60.0)
              : 0.0;
}
```

---

## 7. Original Code (Reference)

The original monolithic code from `trunk/main.c` is preserved below for comparison.

```c
#include "raylib.h"
#include <string.h>
#include <stdio.h>

#define MAX_LEN 1000
#define MAX_LINES 10
#define COUNTDOWN 3
#define LEVELS 3

#define COLOR_BG      (Color){ 18, 18, 22, 255 }
#define COLOR_CARD    (Color){ 30, 30, 35, 255 }
#define COLOR_ACCENT  (Color){ 0, 170, 255, 255 }
#define COLOR_TEXT    (Color){ 200, 200, 200, 255 }

typedef enum { STATE_MENU, STATE_GAME, STATE_FINAL } GameState;

typedef struct {
    char levels[LEVELS][MAX_LINES][MAX_LEN];
    int linesCount[LEVELS];
    double timeLimits[LEVELS];
    double resultsWPM[LEVELS];
    double resultsAccuracy[LEVELS];
    double resultsTime[LEVELS];
} GameData;

int LoadTextLinesFromFile(const char *filename, char lines[MAX_LINES][MAX_LEN]) {
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

void BuildTargetString(GameData *data, int level, char *outStr) {
    outStr[0] = '\0';
    for (int i = 0; i < data->linesCount[level]; i++) {
        strcat(outStr, data->levels[level][i]);
        if (i < data->linesCount[level] - 1) strcat(outStr, "\n");
    }
}

int main() {
    const int screenWidth = 1300;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Typing Master Pro");
    InitAudioDevice();
    SetTargetFPS(60);

    GameData game = {0};
    game.timeLimits[0] = 45; game.timeLimits[1] = 75; game.timeLimits[2] = 120;
    game.linesCount[0] = LoadTextLinesFromFile("level1.txt", game.levels[0]);
    game.linesCount[1] = LoadTextLinesFromFile("level2.txt", game.levels[1]);
    game.linesCount[2] = LoadTextLinesFromFile("level3.txt", game.levels[2]);

    GameState currentState = STATE_MENU;
    int currentLevel = 0;
    char userInput[MAX_LEN] = "";
    char targetText[MAX_LEN] = "";
    int inputIndex = 0;

    double startTime = 0, countdownStart = 0, totalTime = 0;
    bool isLevelStarted = false, isLevelFinished = false;
    bool cursorVisible = true;
    double blinkTimer = 0;

    Texture2D bg = LoadTexture("bg.jpg");

    while (!WindowShouldClose()) {
        if (GetTime() - blinkTimer >= 0.5) {
            cursorVisible = !cursorVisible;
            blinkTimer = GetTime();
        }

        BeginDrawing();
        ClearBackground(COLOR_BG);
        if (bg.id > 0) DrawTextureEx(bg, (Vector2){0,0}, 0, (float)screenWidth/bg.width, WHITE);

        switch (currentState) {
            case STATE_MENU:
                DrawRectangleRounded((Rectangle){ 400, 200, 500, 300 }, 0.1, 10, COLOR_CARD);
                DrawText("TYPING MASTER", 465, 260, 40, COLOR_ACCENT);
                DrawText("Test your typing speed skills", 515, 310, 18, GRAY);
                if (((int)(GetTime() * 2) % 2 == 0))
                    DrawText("PRESS [ENTER] TO START", 525, 400, 20, WHITE);
                if (IsKeyPressed(KEY_ENTER)) {
                    currentState = STATE_GAME;
                    countdownStart = GetTime();
                    BuildTargetString(&game, currentLevel, targetText);
                }
                break;

            case STATE_GAME:
                if (!isLevelStarted) {
                    int remaining = COUNTDOWN - (int)(GetTime() - countdownStart);
                    DrawCircle(screenWidth/2, screenHeight/2, 60, COLOR_CARD);
                    DrawText(TextFormat("%d", remaining), screenWidth/2 - 15, screenHeight/2 - 30, 60, COLOR_ACCENT);
                    if (remaining <= 0) { isLevelStarted = true; startTime = GetTime(); }
                } else if (!isLevelFinished) {
                    double timeElapsed = GetTime() - startTime;
                    int key = GetCharPressed();
                    while (key > 0) {
                        if (inputIndex < MAX_LEN - 1) { userInput[inputIndex++] = (char)key; userInput[inputIndex] = '\0'; }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && inputIndex > 0) userInput[--inputIndex] = '\0';
                    if (IsKeyPressed(KEY_ENTER) && inputIndex < MAX_LEN - 1) { userInput[inputIndex++] = '\n'; userInput[inputIndex] = '\0'; }
                    if (IsKeyPressed(KEY_TAB) || timeElapsed >= game.timeLimits[currentLevel]) isLevelFinished = true;
                    DrawRectangle(0, 0, screenWidth, 60, COLOR_CARD);
                    DrawText(TextFormat("LEVEL: %d", currentLevel + 1), 50, 20, 22, COLOR_ACCENT);
                    float timeLeftRatio = (float)(1.0 - (timeElapsed / game.timeLimits[currentLevel]));
                    DrawRectangle(350, 25, 600, 12, DARKGRAY);
                    DrawRectangle(350, 25, (int)(600 * timeLeftRatio), 12, COLOR_ACCENT);
                    DrawRectangleRounded((Rectangle){ 50, 100, 1200, 550 }, 0.03, 10, COLOR_CARD);
                    DrawText("TARGET:", 80, 130, 16, COLOR_ACCENT);
                    DrawText(targetText, 80, 165, 24, GRAY);
                    DrawText("YOUR INPUT:", 80, 350, 16, GREEN);
                    int drawX = 80, drawY = 390;
                    for (int i = 0; i < inputIndex; i++) {
                        char c[2] = { userInput[i], '\0' };
                        Color cCol = (i < (int)strlen(targetText) && userInput[i] == targetText[i]) ? WHITE : RED;
                        if (userInput[i] == '\n') { drawY += 35; drawX = 80; }
                        else { DrawText(c, drawX, drawY, 26, cCol); drawX += MeasureText(c, 26); }
                    }
                    if (cursorVisible) DrawRectangle(drawX, drawY + 5, 12, 2, COLOR_ACCENT);
                } else {
                    double finalLevelTime = GetTime() - startTime;
                    int correctChars = 0, targetLen = strlen(targetText);
                    for (int i = 0; i < inputIndex && i < targetLen; i++)
                        if (userInput[i] == targetText[i]) correctChars++;
                    game.resultsAccuracy[currentLevel] = (targetLen > 0) ? ((double)correctChars/targetLen)*100 : 0;
                    game.resultsWPM[currentLevel] = (finalLevelTime > 0) ? (inputIndex/5.0)/(finalLevelTime/60.0) : 0;
                    game.resultsTime[currentLevel] = finalLevelTime;
                    DrawRectangleRounded((Rectangle){ 450, 180, 400, 350 }, 0.1, 10, COLOR_CARD);
                    DrawText("LEVEL DONE!", 540, 220, 30, COLOR_ACCENT);
                    DrawText(TextFormat("WPM: %.1f", game.resultsWPM[currentLevel]), 550, 290, 24, WHITE);
                    DrawText(TextFormat("Accuracy: %.1f%%", game.resultsAccuracy[currentLevel]), 550, 330, 24, WHITE);
                    DrawText("Press [ENTER] to Continue", 535, 450, 18, GRAY);
                    if (IsKeyPressed(KEY_ENTER)) {
                        totalTime += finalLevelTime;
                        currentLevel++;
                        if (currentLevel < LEVELS) {
                            isLevelStarted = false; isLevelFinished = false; inputIndex = 0;
                            userInput[0] = '\0'; countdownStart = GetTime();
                            BuildTargetString(&game, currentLevel, targetText);
                        } else currentState = STATE_FINAL;
                    }
                }
                break;

            case STATE_FINAL:
                DrawRectangleRounded((Rectangle){ 350, 120, 600, 480 }, 0.05, 10, COLOR_CARD);
                DrawText("FINAL STATISTICS", 480, 160, 35, COLOR_ACCENT);
                for (int i = 0; i < LEVELS; i++)
                    DrawText(TextFormat("Level %d: %2.0f WPM  |  %2.0f%% Accuracy", i+1,
                             game.resultsWPM[i], game.resultsAccuracy[i]), 430, 250 + (i*50), 22, WHITE);
                DrawRectangle(430, 420, 440, 2, DARKGRAY);
                DrawText(TextFormat("TOTAL TIME: %.1f seconds", totalTime), 490, 450, 24, GREEN);
                DrawText("Press [ESC] to Exit", 560, 540, 16, GRAY);
                break;
        }
        EndDrawing();
    }

    UnloadTexture(bg);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
```

---

## 8. Git Commit Messages

Use these commit messages on the `solid-refactor` branch in order:

```
feat: create solid-refactor branch from trunk

refactor: extract GameData and GameContext into game.h / game.c (SRP)

refactor: isolate file loading into level.c / level.h (SRP, DIP)

refactor: isolate timing logic into timer.c / timer.h (SRP, ISP)

refactor: isolate input capture into input.c / input.h (SRP, ISP)

refactor: isolate statistics calculation into stats.c / stats.h (SRP, ISP)

refactor: isolate all draw calls into ui.c / ui.h (SRP)

refactor: introduce StateHandler interface in state.h (OCP, LSP, DIP)

refactor: implement state handlers in state.c using function pointer dispatch

refactor: reduce main.c to bootstrap only — Game_Init / Game_Run / Game_Shutdown

docs: add README.md with SOLID explanation, folder structure, and commit guide
```

---

## 9. Build Instructions

### Prerequisites
- [Raylib](https://www.raylib.com/) installed on your system
- GCC or Clang

### Compile

```bash
gcc main.c game.c state.c ui.c input.c level.c timer.c stats.c \
    -o TypingMasterPro \
    -lraylib -lm \
    -Wall -Wextra
```

### Run

```bash
./TypingMasterPro
```

> The executable must be run from the directory that contains `level1.txt`, `level2.txt`, `level3.txt`, and `bg.jpg`.

---

## 10. Required Asset Files

The game reads three plain-text level files at startup. Each file must exist next to the executable. Each file can contain **up to 10 lines**, each line up to **1000 characters**.

**`level1.txt`** — short, simple sentences (easy):
```
The quick brown fox jumps over the lazy dog.
Pack my box with five dozen liquor jugs.
How vaguely quixotic the jumbled letters appear.
```

**`level2.txt`** — medium length paragraphs (moderate):
```
Sphinx of black quartz, judge my vow.
Five boxing wizards jump quickly over the fence.
The jay, pig, fox, zebra and my wolves quack.
Crazy Fredrick bought many very exquisite opal jewels.
```

**`level3.txt`** — longer, complex sentences (hard):
```
The five boxing wizards jump quickly while the fox watches.
A mad boxer shot a quick gloved jab to the jaw of his dizzy opponent.
Sixty zippers were quickly picked from the woven jute bag.
We promptly judged antique ivory buckles for the next prize.
Amazingly few discotheques provide jukeboxes for music lovers.
```

**`bg.jpg`** — any background image (optional). If the file is missing, the game falls back to the solid `COLOR_BG` background with no error.
