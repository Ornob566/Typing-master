#include "InputHandler.hpp"
#include "raylib.h"

void InputHandler::ProcessTyping(std::string& buffer) {
    int key = GetCharPressed();
    while (key > 0) {
        if (buffer.length() < 1000) buffer += (char)key;
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !buffer.empty()) buffer.pop_back();
    if (IsKeyPressed(KEY_ENTER)) buffer += '\n';
}