#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include <string>
#include "DEFINE.hpp"
#include <vector>
#include JSON_LIB_PATH
#include "TowerUpgrade.h"
#include <SFML/Graphics.hpp>

struct TowerParameters {
    sf::Color color;
    float attackSpeed;
    int range;
    int damage;
    int cost;
    std::vector<TowerUpgrade> upgrades;
};

// Function to get a float value from a JSON file given a key
float getJsonValue(const std::string& filename, const std::string& key);

// Function to print the content of a JSON file
void printJsonFile(const std::string& filename);

// Function to load tower parameters from a JSON file
TowerParameters loadTowerParameters(const std::string& towerType);

#endif // JSONUTILS_HPP
