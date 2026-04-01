#ifndef STATS_MANAGER_HPP
#define STATS_MANAGER_HPP
#include <string>
#include "Common.hpp"

class StatsManager {
public:
    LevelStats Calculate(const std::string& target, const std::string& input, double timeTaken) {
        LevelStats s;
        s.timeTaken = timeTaken;
        int correct = 0;
        size_t minLen = std::min(target.length(), input.length());
        for(size_t i = 0; i < minLen; i++) if(target[i] == input[i]) correct++;
        
        s.accuracy = (target.length() > 0) ? ((double)correct / target.length()) * 100 : 0;
        s.wpm = (timeTaken > 0) ? (input.length() / 5.0) / (timeTaken / 60.0) : 0;
        return s;
    }
};
#endif