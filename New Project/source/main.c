#include "common.h"
#include "game.h"

int main() {
    // 1. Window/Platform Setup (Remains in main or a bootstrapper)
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Typing Master Pro (SOLID C)");
    InitAudioDevice();
    SetTargetFPS(60);

    // 2. Initialize Game Manager (SRP: Orchestration)
    GameManager* gameApp = GameManager_Create();
    if (!gameApp) {
        CloseWindow();
        return 1;
    }

    // 3. Main game loop
    while (!WindowShouldClose()) {
        GameManager_RunFrame(gameApp);
    }

    // 4. Resource release
    GameManager_Destroy(gameApp);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}