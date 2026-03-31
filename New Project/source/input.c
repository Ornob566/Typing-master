#include "input.h"
#include <stdlib.h>
#include <string.h>

InputHandler* InputHandler_Create() {
    InputHandler* input = (InputHandler*)malloc(sizeof(InputHandler));
    if (!input) return NULL;
    InputHandler_Reset(input);
    return input;
}

void InputHandler_Destroy(InputHandler* input) {
    if (input) free(input);
}

void InputHandler_Reset(InputHandler* input) {
    memset(input->buffer, 0, MAX_LEN);
    input->index = 0;
}

void InputHandler_Update(InputHandler* input) {
    int key = GetCharPressed();
    while (key > 0) {
        if (input->index < MAX_LEN - 1) {
            input->buffer[input->index++] = (char)key;
            input->buffer[input->index] = '\0';
        }
        key = GetCharPressed();
    }
    
    if (IsKeyPressed(KEY_BACKSPACE) && input->index > 0) {
        input->buffer[--input->index] = '\0';
    }
    
    if (IsKeyPressed(KEY_ENTER) && input->index < MAX_LEN - 1) {
        input->buffer[input->index++] = '\n';
        input->buffer[input->index] = '\0';
    }
}

const char* InputHandler_GetBuffer(InputHandler* input) {
    return input->buffer;
}

int InputHandler_GetIndex(InputHandler* input) {
    return input->index;
}