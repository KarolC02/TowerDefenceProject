#include "Pathfinder.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

Pathfinder::Pathfinder(int gridWidth, int gridHeight, bool* grid) : gridWidth(gridWidth), gridHeight(gridHeight), grid(grid) {}

std::vector<std::pair<int, int>> Pathfinder::findPathBFS(const std::pair<int, int>& start, const std::pair<int, int>& end) {
    std::vector<std::pair<int, int>> path;
    std::queue<std::pair<int, int>> queue;
    std::unordered_map<int, std::pair<int, int>> cameFrom;
    queue.push(start);
    cameFrom[toIndex(start.first, start.second)] = {-1, -1};

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();
        if (current == end) {
            std::pair<int, int> step = end;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[toIndex(step.first, step.second)];
                if (step.first == -1) break;
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : std::vector<std::pair<int, int>>{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}) {
            std::pair<int, int> next = {current.first + dir.first, current.second + dir.second};
            if (next.first >= 0 && next.first < gridWidth && next.second >= 0 && next.second < gridHeight && !grid[toIndex(next.first, next.second)]
                && cameFrom.find(toIndex(next.first, next.second)) == cameFrom.end()) {
                queue.push(next);
                cameFrom[toIndex(next.first, next.second)] = current;
            }
        }
    }
    return {};
}

void Pathfinder::markPathCellsRed(const std::vector<std::pair<int, int>>& path, std::vector<sf::RectangleShape>& pathCells) {
    pathCells.clear();
    for (const auto& cell : path) {
        sf::RectangleShape rect(sf::Vector2f(GRID_CELL_SIZE, GRID_CELL_SIZE));
        rect.setPosition(cell.first * GRID_CELL_SIZE + LEFT_OFFSET, cell.second * GRID_CELL_SIZE + TOP_OFFSET);
        rect.setFillColor(sf::Color::Red);  // Set path cells to red
        pathCells.push_back(rect);
    }
}

int Pathfinder::toIndex(int x, int y) const {
    return y * gridWidth + x;
}
