#ifndef INPUT_H
#define INPUT_H

#include "common.h"

// SOLID: SRP - Handles keyboard input and text buffer manipulation.
typedef struct InputHandler {
    char buffer[MAX_LEN];
    int index;
} InputHandler;

InputHandler* InputHandler_Create();
void InputHandler_Destroy(InputHandler* input);

// SOLID: ISP - Specific interface for updating input state
void InputHandler_Update(InputHandler* input); 
void InputHandler_Reset(InputHandler* input);

const char* InputHandler_GetBuffer(InputHandler* input);
int InputHandler_GetIndex(InputHandler* input);

#endif