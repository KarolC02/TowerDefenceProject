#include "LevelManager.hpp"
#include <fstream>
#include "DEFINE.hpp"
#include <iostream>
#include JSON_LIB_PATH

std::vector<Level> LevelManager::loadLevels(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    nlohmann::json json;
    file >> json;
    file.close();

    std::vector<Level> levels;

    for (const auto& j : json["levels"]) {
        if (j.contains("levelNumber") && j.contains("maxDuration") && j.contains("numberOfEnemies") &&
            j.contains("group") && j.contains("health") && j.contains("immune") &&
            j.contains("fast") && j.contains("flying") && j.contains("spawn") && j.contains("value")) {
            levels.emplace_back(
                j["levelNumber"].get<int>(),
                j["maxDuration"].get<float>(),
                j["numberOfEnemies"].get<int>(),
                j["group"].get<bool>(),
                j["health"].get<int>(),
                j["immune"].get<bool>(),
                j["fast"].get<bool>(),
                j["flying"].get<bool>(),
                j["spawn"].get<bool>(),
                j["value"].get<int>()
            );
        } else {
            throw std::runtime_error("Missing one or more level properties in JSON file. ");
        }
    }
    return levels;
}
