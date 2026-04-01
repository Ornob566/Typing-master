#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <memory>
#include <vector>
#include "IState.hpp"
#include "UIRenderer.hpp"
#include "StatsManager.hpp"

class GameManager {
public:
    std::unique_ptr<IState> currentState;
    UIRenderer ui;
    StatsManager statsProcessor;
    
    // Shared Data
    std::string targetText;
    std::string userInput;
    LevelStats allStats[TOTAL_LEVELS];
    int currentLevel = 0;
    double levelStartTime = 0;

    void ChangeState(std::unique_ptr<IState> newState) {
        currentState = std::move(newState);
        currentState->Enter(*this);
    }

    void Run();
};
#endif