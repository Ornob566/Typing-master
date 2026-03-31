#include "timer.h"
#include <stdlib.h>

TimerManager* TimerManager_Create() {
    TimerManager* timer = (TimerManager*)malloc(sizeof(TimerManager));
    if (!timer) return NULL;
    timer->startTime = 0;
    timer->countdownStart = 0;
    timer->blinkTimer = 0;
    timer->cursorVisible = true;
    return timer;
}

void TimerManager_Destroy(TimerManager* timer) {
    if (timer) free(timer);
}

void TimerManager_Update(TimerManager* timer) {
    if (GetTime() - timer->blinkTimer >= 0.5) {
        timer->cursorVisible = !timer->cursorVisible;
        timer->blinkTimer = GetTime();
    }
}

void TimerManager_StartCountdown(TimerManager* timer) {
    timer->countdownStart = GetTime();
}

int TimerManager_GetCountdownRemaining(TimerManager* timer) {
    return COUNTDOWN - (int)(GetTime() - timer->countdownStart);
}

void TimerManager_StartLevelTimer(TimerManager* timer) {
    timer->startTime = GetTime();
}

double TimerManager_GetElapsed(TimerManager* timer) {
    if (timer->startTime == 0) return 0;
    return GetTime() - timer->startTime;
}

bool TimerManager_IsCursorVisible(TimerManager* timer) {
    return timer->cursorVisible;
}