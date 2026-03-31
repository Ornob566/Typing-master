#ifndef TIMER_H
#define TIMER_H

#include "common.h"

// SOLID: SRP - Handles time tracking logic, blinks, and countdowns.
typedef struct TimerManager {
    double startTime;
    double countdownStart;
    double blinkTimer;
    bool cursorVisible;
} TimerManager;

TimerManager* TimerManager_Create();
void TimerManager_Destroy(TimerManager* timer);

void TimerManager_Update(TimerManager* timer); // Handles blink logic
void TimerManager_StartCountdown(TimerManager* timer);
int TimerManager_GetCountdownRemaining(TimerManager* timer);
void TimerManager_StartLevelTimer(TimerManager* timer);
double TimerManager_GetElapsed(TimerManager* timer);
bool TimerManager_IsCursorVisible(TimerManager* timer);

#endif