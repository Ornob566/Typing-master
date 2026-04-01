#include "UIRenderer.hpp"
#include "raylib.h"

void UIRenderer::DrawCard(Rectangle rect, const char* title, const char* sub) {
    DrawRectangleRounded(rect, 0.1f, 10, COLOR_CARD);
    DrawText(title, rect.x + 40, rect.y + 60, 40, COLOR_ACCENT);
    DrawText(sub, rect.x + 180, rect.y + 110, 18, GRAY);
}

void UIRenderer::DrawProgressBar(int x, int y, float ratio) {
    DrawRectangle(x, y, 600, 12, DARKGRAY);
    DrawRectangle(x, y, (int)(600 * ratio), 12, COLOR_ACCENT);
}

void UIRenderer::DrawTypingArea(const std::string& target, const std::string& input, bool cursorVisible) {
    DrawRectangleRounded({ 50, 100, 1200, 550 }, 0.03f, 10, COLOR_CARD);
    DrawText("TARGET:", 80, 130, 16, COLOR_ACCENT);
    DrawText(target.c_str(), 80, 165, 24, GRAY);

    DrawText("YOUR INPUT:", 80, 350, 16, GREEN);
    int drawX = 80, drawY = 390;
    for (size_t i = 0; i < input.length(); i++) {
        char c[2] = { input[i], '\0' };
        Color cCol = (i < target.length() && input[i] == target[i]) ? WHITE : RED;
        if (input[i] == '\n') { drawY += 35; drawX = 80; }
        else {
            DrawText(c, drawX, drawY, 26, cCol);
            drawX += MeasureText(c, 26);
        }
    }
    if (cursorVisible) DrawRectangle(drawX, drawY + 5, 12, 2, COLOR_ACCENT);
}