#include "LevelLoader.hpp"
#include <fstream>

std::string LevelLoader::LoadLevelPath(int levelIdx) {
    std::string filename = "level" + std::to_string(levelIdx + 1) + ".txt";
    std::ifstream file(filename);
    std::string content = "", line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        if (!content.empty()) content.pop_back(); 
        file.close();
    }
    return content.empty() ? "Level file missing!" : content;
}