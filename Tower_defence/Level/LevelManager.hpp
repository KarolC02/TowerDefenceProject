#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "Level.hpp"
#include <vector>
#include <string>

class LevelManager {
public:
    static std::vector<Level> loadLevels(const std::string& filename);
};

#endif // LEVELMANAGER_HPP
