#include "GameManager.hpp"
#include "IState.hpp"

void GameManager::Init() {
    InitWindow(1300, 720, "Typing Master SOLID Refactor");
    InitAudioDevice();
    SetTargetFPS(60);
    // Initial state set in main.cpp
}

void GameManager::Update() {
    if (currentState) {
        currentState->Update(*this);
    }
}

void GameManager::Draw() {
    BeginDrawing();
    ClearBackground(COLOR_BG);
    if (currentState) {
        currentState->Draw(*this);
    }
    EndDrawing();
}

void GameManager::Cleanup() {
    CloseAudioDevice();
    CloseWindow();
}