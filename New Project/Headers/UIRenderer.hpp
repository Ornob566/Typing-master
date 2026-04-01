#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP
#include "Common.hpp"

class UIRenderer {
public:
    void DrawCard(Rectangle rect, const char* title, const char* sub);
    void DrawProgressBar(int x, int y, float ratio);
    void DrawTypingArea(const std::string& target, const std::string& input, bool cursorVisible);
};
#endif