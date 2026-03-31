#ifndef STATE_H
#define STATE_H

#include "common.h"

// SOLID: OCP/LSP/DIP - This structure simulates an interface/abstract base class.
// concrete state implementations (defined in state.c) fill these function pointers.
typedef struct StateHandler {
    void (*Enter)(GameManager* game);
    void (*Update)(GameManager* game); // SOLID: ISP - handle input/logic
    void (*Draw)(GameManager* game);   // SOLID: ISP - handle rendering
    void (*Exit)(GameManager* game);
} StateHandler;

// Factory functions to get concrete instances of interfaces
const StateHandler* State_GetMenuHandler();
const StateHandler* State_GetGameHandler();
const StateHandler* State_GetFinalHandler();

#endif