#include "GameModel.h"
#include <fstream>
#include <algorithm>

GameModel::GameModel() {
    currentState     = STATE_MENU;
    currentLevel     = 0;
    isLevelStarted   = false;
    isLevelFinished  = false;
    userInput        = "";
    totalSessionTime = 0;
    startTime        = 0;
    countdownStart   = 0;

    timeLimits[0] = 45;
    timeLimits[1] = 75;
    timeLimits[2] = 120;
}

void GameModel::LoadLevel(int levelIdx) {
    std::string filename = "assets/levels/level" + std::to_string(levelIdx + 1) + ".txt";
    std::ifstream file(filename);
    std::string line;
    targetText = "";

    if (file.is_open()) {
        while (std::getline(file, line))
            targetText += line + "\n";
        if (!targetText.empty()) targetText.pop_back();
        file.close();
    } else {
        targetText = "Error: level file not found!";
    }
}

void GameModel::ResetForLevel(double now) {
    isLevelStarted  = false;
    isLevelFinished = false;
    userInput       = "";
    countdownStart  = now;
    LoadLevel(currentLevel);
}

void GameModel::CalculateResults(double now) {
    double finalTime = now - startTime;
    int correctChars = 0;
    int minLen = (int)std::min(userInput.length(), targetText.length());

    for (int i = 0; i < minLen; i++)
        if (userInput[i] == targetText[i]) correctChars++;

    stats[currentLevel].accuracy = (targetText.length() > 0)
        ? ((double)correctChars / targetText.length()) * 100.0 : 0;
    stats[currentLevel].wpm = (finalTime > 0)
        ? (userInput.length() / 5.0) / (finalTime / 60.0) : 0;
    stats[currentLevel].timeTaken = finalTime;
}

bool GameModel::IsTimeUp(double now) const {
    return (now - startTime) >= timeLimits[currentLevel];
}

int GameModel::CountdownRemaining(double now) const {
    int r = COUNTDOWN_TIME - (int)(now - countdownStart);
    return r > 0 ? r : 1;
}

float GameModel::TimerRatio(double now) const {
    float r = 1.0f - (float)((now - startTime) / timeLimits[currentLevel]);
    return r < 0.0f ? 0.0f : r;
}
