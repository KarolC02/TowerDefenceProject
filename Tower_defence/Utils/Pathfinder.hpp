#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "DEFINE.hpp"

class Pathfinder {
public:
    Pathfinder();
    Pathfinder(int gridWidth, int gridHeight, bool* grid);
    std::vector<std::pair<int, int>> findPathBFS(const std::pair<int, int>& start, const std::pair<int, int>& end);
    void markPathCellsRed(const std::vector<std::pair<int, int>>& path, std::vector<sf::RectangleShape>& pathCells);

private:
    bool* grid;
    int gridWidth;
    int gridHeight;

    int toIndex(int x, int y) const;
};
