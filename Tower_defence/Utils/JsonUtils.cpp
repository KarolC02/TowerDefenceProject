#include "JsonUtils.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include "DEFINE.hpp"

// Helper function to split string by delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

float getJsonValue(const std::string& filename, const std::string& key) {
    try {
        // std::cout << "Attempting to open file: " << filename << std::endl;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << filename << std::endl;
            throw std::runtime_error("Could not open file: " + filename);
        }

        nlohmann::json json;
        try {
            file >> json;
        } catch (const std::exception& e) {
            file.close();
            std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
            throw std::runtime_error("Error parsing JSON file: " + std::string(e.what()));
        }
        file.close();

        // Navigate through nested keys
        nlohmann::json* currentJson = &json;
        for (const auto& k : split(key, '.')) {
            if (currentJson->find(k) == currentJson->end()) {
                std::cerr << "Key not found in JSON file: " << key << std::endl;
                throw std::runtime_error("Key not found in JSON file: " + key);
            }
            currentJson = &(*currentJson)[k];
        }

        float value = currentJson->get<float>();
        // std::cout << "Loaded " << key << " from JSON: " << value << std::endl;
        return value;
    } catch (const std::exception& e) {
        std::cerr << "Error loading " << key << " from JSON: " << e.what() << std::endl;
        std::cerr << "Filename: " << filename << ", Key: " << key << std::endl;
        throw;
    }
}

void printJsonFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        nlohmann::json json;
        file >> json;
        file.close();

        std::cout << "Current JSON file content: " << json.dump(4) << std::endl; // Pretty print with 4 spaces
    } catch (const std::exception& e) {
        std::cerr << "Error loading JSON file: " << e.what() << std::endl;
    }
}

TowerParameters loadTowerParameters(const std::string& towerType) {
    try {
        std::ifstream file(TOWER_PARAMETERS_PATH);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << TOWER_PARAMETERS_PATH << std::endl;
            throw std::runtime_error("Could not open file: " + std::string(TOWER_PARAMETERS_PATH));
        }

        nlohmann::json json;
        file >> json;
        file.close();

        if (json.find(towerType) == json.end()) {
            std::cerr << "Tower type not found in JSON file: " << towerType << std::endl;
            throw std::runtime_error("Tower type not found in JSON file: " + towerType);
        }

        TowerParameters params;
        auto& towerJson = json[towerType]["initial"];
        params.color = sf::Color(towerJson["color"][0], towerJson["color"][1], towerJson["color"][2]);
        params.attackSpeed = towerJson["attackSpeed"];
        params.range = towerJson["range"];
        params.damage = towerJson["damage"];
        params.cost = towerJson["cost"];

        for (const auto& upgrade : json[towerType]["upgrades"]) {
            TowerUpgrade tu(
                upgrade["name"],
                upgrade["attackSpeed"],
                upgrade["range"],
                upgrade["damage"],
                upgrade["cost"]
            );
            params.upgrades.push_back(tu);
        }

        return params;
    } catch (const std::exception& e) {
        std::cerr << "Error loading parameters for " << towerType << " from JSON: " << e.what() << std::endl;
        throw;
    }
}
