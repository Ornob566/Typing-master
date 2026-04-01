#include "GameManager.hpp"
#include "States.cpp" // Simplified for this example

void GameManager::Run() {
    InitWindow(1300, 720, "Typing Master SOLID");
    SetTargetFPS(60);

    ChangeState(std::make_unique<MenuState>());

    while (!WindowShouldClose()) {
        currentState->Update(*this);
        
        BeginDrawing();
        ClearBackground(COLOR_BG);
        currentState->Draw(*this);
        EndDrawing();
    }
    CloseWindow();
}

int main() {
    GameManager game;
    game.Run();
    return 0;
}