#include "Timer.hpp"
#include "raylib.h"

bool Timer::UpdateCursorBlink(double& blinkTracker) {
    if (GetTime() - blinkTracker >= 0.5) {
        blinkTracker = GetTime();
        return true; // Toggle visibility
    }
    return false;
}

int Timer::GetCountdownRemaining(double start, int total) {
    int remaining = total - (int)(GetTime() - start);
    return (remaining < 0) ? 0 : remaining;
}