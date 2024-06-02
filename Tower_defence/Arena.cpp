#include "Arena.hpp"
#include <algorithm> // For std::remove_if
#include "PelletTower.hpp"
#include "BashTower.hpp"
#include <iostream>
#include <cmath>  // For std::hypot
#include <queue>
#include <unordered_map>
#include <functional> // for std::greater
#include "ExplosiveBullet.hpp"
#include "FrostBullet.hpp"
#include "ResourcePath.hpp"
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <random>



Arena::Arena(float width, float height, sf::Vector2f position)
    : alreadySpawnedThisLevel(0), currentLevelIndex(0), readyForNextLevel(true), justAddedTower(false), score(0), level(1) {
          
    
        lives = getJsonValue(CONFIG_PATH, "START_LIVES");
        gold = getJsonValue(CONFIG_PATH, "START_MONEY");

    
    
          
    levels = LevelManager::loadLevels("/Users/karol/desktop/Tower_defence/Tower_defence/Resources/levels.json");
    std::fill(&grid[0][0], &grid[0][0] + sizeof(grid), false);
        
    for (int i = 0; i < GRID_WIDTH * 2; ++i) {
        for (int j = 0; j < GRID_HEIGHT * 2; ++j) {
            if ((i == 0 && (j < 9 || j > 14)) ||
                (i == GRID_WIDTH * 2 - 1 && (j < 9 || j > 14)) ||
                j == 0 || j == 2 * GRID_HEIGHT - 1) {
                grid[i][j] = true;
            }
        }
    }
        
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(133, 187, 101));
    background.setPosition(position);
    
    topFrame1.setSize(sf::Vector2f(GRID_CELL_SIZE, 9 * GRID_CELL_SIZE));
    topFrame1.setPosition(position);
    topFrame1.setFillColor(BACKGROUND_FRAME_COLOR);
    
    topFrame2.setSize(sf::Vector2f(width - TOWER_SIDE_LENGTH,  GRID_CELL_SIZE));
    topFrame2.setPosition(position + sf::Vector2f(GRID_CELL_SIZE, 0));
    topFrame2.setFillColor(BACKGROUND_FRAME_COLOR);
    
    topFrame3.setSize(sf::Vector2f(GRID_CELL_SIZE, 9 * GRID_CELL_SIZE));
    topFrame3.setPosition(position + sf::Vector2f(width - GRID_CELL_SIZE, 0));
    topFrame3.setFillColor(BACKGROUND_FRAME_COLOR);
    
    bottomFrame1.setSize(sf::Vector2f(GRID_CELL_SIZE, 9 * GRID_CELL_SIZE));
    bottomFrame1.setPosition(position + sf::Vector2f(0, 15 * GRID_CELL_SIZE));
    bottomFrame1.setFillColor(BACKGROUND_FRAME_COLOR);
    
    bottomFrame2.setSize(sf::Vector2f(width - TOWER_SIDE_LENGTH, GRID_CELL_SIZE));
    bottomFrame2.setPosition(position + sf::Vector2f(GRID_CELL_SIZE, 23 * GRID_CELL_SIZE));
    bottomFrame2.setFillColor(BACKGROUND_FRAME_COLOR);
    
    bottomFrame3.setSize(sf::Vector2f(GRID_CELL_SIZE, 9 * GRID_CELL_SIZE));
    bottomFrame3.setPosition(position + sf::Vector2f(width - GRID_CELL_SIZE, 15 * GRID_CELL_SIZE));
    bottomFrame3.setFillColor(BACKGROUND_FRAME_COLOR);
}

void Arena::loadNextLevel() {
    pausedTime = 0;
    currentLevel = levels.at(currentLevelIndex);
    currentLevelIndex++;
    readyForNextLevel = false;
    alreadySpawnedThisLevel = 0;
}

void Arena::update(float dt, const sf::RenderWindow& window) {
    
    manageLevels();
    spawnCurrentLevelEnemies();
    updateTowers(dt, window);
    checkBulletEnemyCollisions();
    updateEnemies(dt);
    justAddedTower = false;
}

void Arena::updateTowers(float dt, const sf::RenderWindow &window) {
    for (auto& tower : towers) {
        tower->update(dt, enemies, window);
    }
}

void Arena::updateEnemies(float dt) {
    for (auto& enemy : enemies) {
        sf::Vector2i currentPosition = enemy->getGridPosition();
        if (!enemy->isImmune() && (!enemy->gotPath || justAddedTower)) {
            std::pair<int, int> start = {currentPosition.x, currentPosition.y};
            std::pair<int, int> end = {endX, endY};
            enemy->setPath(findPathAStar(start, end));
            enemy->gotPath = true;
        }
        enemy->update(dt);
    }
    removeDeadOrOutOfBoundsEnemies();
}

void Arena::spawnCurrentLevelEnemies() {
    float spawnCooldown = getJsonValue(CONFIG_PATH, "SPAWN_COOLDOWN");
    if (currentLevel.numberOfEnemies > alreadySpawnedThisLevel) {
        if (currentLevel.group) {
            while (currentLevel.numberOfEnemies > alreadySpawnedThisLevel) {
                spawnEnemy(currentLevel.health, currentLevel.value);  // Pass the value parameter
                alreadySpawnedThisLevel++;
            }
        } else {
            if (enemySpawnTimer.getElapsedTime().asSeconds() > spawnCooldown) {
                spawnEnemy(currentLevel.health, currentLevel.value);  // Pass the value parameter
                alreadySpawnedThisLevel++;
                enemySpawnTimer.restart();
            }
        }
    }
}


void Arena::manageLevels() {
    if (readyForNextLevel) {
        currentLevelTimer.restart();
        enemySpawnTimer.restart();
        loadNextLevel();
        // std::cout << "Level number: " << currentLevel.levelNumber << std::endl;
        // std::cout << "Enemy Health: " << currentLevel.health << std::endl;
        // std::cout << "NO enemies: " << currentLevel.numberOfEnemies << std::endl;
        // std::cout << "Group?: " << currentLevel.group << std::endl;
        // std::cout << "Duration in Sec: " << currentLevel.maxDuration << std::endl;
        // std::cout << "Spawn: " << currentLevel.spawn << std::endl;
    }
    if (currentLevelTimer.getElapsedTime().asSeconds() - pausedTime > currentLevel.maxDuration){
        readyForNextLevel = true;
    }
}

void Arena::draw(sf::RenderWindow& window) const {
    drawBackground(window);
    drawTowers(window);
    drawEnemies(window);
    drawBullets(window);
}

void Arena::drawBullets(sf::RenderWindow &window) const{
    for (const auto& tower : towers) {
        tower->drawBullets(window);
    }
}

void Arena::drawTowers(sf::RenderWindow &window) const {
    for (const auto& tower : towers) {
        if (tower) {
            tower->draw(window);
        } else {
            std::cerr << "Error: Attempted to draw a null tower!" << std::endl;
        }
    }
}

void Arena::drawEnemies(sf::RenderWindow &window) const {
    for (const auto& enemy : enemies) {
        enemy->draw(window);
    }
}

int Arena::random(int min, int max) {
    return min + rand() % ((max - min) + 1);
}

void Arena::drawBackground(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(topFrame1);
    window.draw(topFrame2);
    window.draw(topFrame3);
    window.draw(bottomFrame1);
    window.draw(bottomFrame2);
    window.draw(bottomFrame3);
}

void Arena::drawGrid(sf::RenderWindow &window) const {
    for (int x = 0; x <= GRID_WIDTH * 2 - 2; x++) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x * GRID_CELL_SIZE + LEFT_OFFSET + GRID_CELL_SIZE, TOP_OFFSET + GRID_CELL_SIZE)),
            sf::Vertex(sf::Vector2f(x * GRID_CELL_SIZE + LEFT_OFFSET + GRID_CELL_SIZE, TOP_OFFSET + GRID_HEIGHT * 2 * GRID_CELL_SIZE - GRID_CELL_SIZE))
        };
        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        window.draw(line, 2, sf::Lines);
    }

    for (int y = 0; y <= GRID_HEIGHT * 2 - 2; y++) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(LEFT_OFFSET + GRID_CELL_SIZE, y * GRID_CELL_SIZE + TOP_OFFSET + GRID_CELL_SIZE)),
            sf::Vertex(sf::Vector2f(LEFT_OFFSET + GRID_WIDTH * 2 * GRID_CELL_SIZE - GRID_CELL_SIZE, y * GRID_CELL_SIZE + TOP_OFFSET + GRID_CELL_SIZE))
        };
        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        window.draw(line, 2, sf::Lines);
    }
}

std::vector<std::unique_ptr<Tower>>& Arena::getTowers() {
    return towers;
}

std::vector<std::shared_ptr<Enemy>>& Arena::getEnemies() {
    return enemies;
}

bool Arena::addTower(std::unique_ptr<Tower> tower) {
    sf::Vector2f originalPos = tower->getPosition();
    sf::Vector2f snappedPos = snapToGrid(originalPos);
    int gridX = getGridX(snappedPos.x);
    int gridY = getGridY(snappedPos.y);
    int towerCost = tower->getCost();
    
    std::cout << "Attempting to add tower at original position: (" << originalPos.x << ", " << originalPos.y << "), snapped to grid position: (" << gridX << ", " << gridY << ")\n";

    if (isSpaceFree(snappedPos) && isWithinBounds(snappedPos) && gold >= towerCost ) {
        tower->setPosition(snappedPos);
        towers.push_back(std::move(tower));
        justAddedTower = true;

        updateGridForNewTower(gridX, gridY);
        std::cout << "Successfully added tower at snapped position: (" << snappedPos.x << ", " << snappedPos.y << ")\n";
        gold -= towerCost;
        return true; // Successfully added tower
    }

    std::cout << "Failed to add tower: Space is not free or not within bounds\n";
    return false; // Failed to add tower
}


int Arena::getGridX(int x) {
    return (x - LEFT_OFFSET) / GRID_CELL_SIZE - 1;
}

int Arena::getGridY(int y) {
    return (y - TOP_OFFSET) / GRID_CELL_SIZE - 1;
}

sf::Vector2f Arena::snapToGrid(const sf::Vector2f& position) const {
    sf::Vector2f snappedPos;
    snappedPos.x = std::floor((position.x - LEFT_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + LEFT_OFFSET;
    snappedPos.y = std::floor((position.y - TOP_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + TOP_OFFSET;
    return snappedPos;
}

void Arena::removeDeadOrOutOfBoundsEnemies() {
    std::vector<int> indicesToRemove;
    std::vector<std::unique_ptr<Enemy>> enemiesToSpawn;

    // First pass: Identify enemies to remove and collect spawn data
    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy& enemy = *enemies[i];
        bool isOutOfBounds = enemy.getShape().getPosition().x < 0 ||
                             enemy.getShape().getPosition().x > LEFT_OFFSET + ARENA_WIDTH + enemy.getShape().getGlobalBounds().width ||
                             enemy.getShape().getPosition().y < 0 ||
                             enemy.getShape().getPosition().y > TOP_OFFSET + ARENA_HEIGHT + enemy.getShape().getGlobalBounds().height;

        if (isOutOfBounds) {
            lives -= 1;  // Increment lives debt if enemy is out of bounds
            indicesToRemove.push_back(i);  // Mark for removal
        } else if (enemy.getIsDead()) {
            payCheck += enemy.getValue();  // Add enemy value to paycheck if dead

            if (enemy.spawn) {
                enemiesToSpawn.push_back(std::make_unique<Enemy>(enemy.getPosition() + sf::Vector2f(5, 5), enemy.getMaxHealth() / 2, enemy.getValue(), enemy.flying, false, enemy.slowImmune, enemy.fast));
                enemiesToSpawn.push_back(std::make_unique<Enemy>(enemy.getPosition(), enemy.getMaxHealth() / 2, enemy.getValue(), enemy.flying, false, enemy.slowImmune, enemy.fast));
            }

            score += enemy.getValue();  // Increment score by enemy value
            indicesToRemove.push_back(i);  // Mark for removal
        }
    }

    // Clear targets of bullets if their target is being removed
    for (auto& tower : towers) {
        for (auto& bullet : tower->getBullets()) {
            for (int index : indicesToRemove) {
                if (bullet->hasTarget(enemies[index])) {
                    bullet->clearTarget();
                }
            }
        }
    }

    // Second pass: Remove identified enemies
    for (auto it = indicesToRemove.rbegin(); it != indicesToRemove.rend(); ++it) {
        enemies.erase(enemies.begin() + *it);
    }

    // Add new enemies using emplace_back
    for (auto& newEnemy : enemiesToSpawn) {
        enemies.push_back(std::move(newEnemy));
    }
}


bool Arena::isWithinBounds(sf::Vector2f snappedPos) {
    return (snappedPos.x > LEFT_OFFSET && snappedPos.x < ARENA_WIDTH + LEFT_OFFSET
            && snappedPos.y > TOP_OFFSET && snappedPos.y < TOP_OFFSET + ARENA_HEIGHT);
}

bool Arena::isSpaceFree(const sf::Vector2f& position) {
    sf::Vector2f snappedPos = snapToGrid(position);
    for (const auto& enemy : enemies) {
        if (enemy->flying) {
            continue;
        }
        sf::Vector2f enemyPos = enemy->getPosition();
        float distance = std::hypot(snappedPos.x - enemyPos.x, snappedPos.y - enemyPos.y);
        if (distance < 1.5 * GRID_CELL_SIZE) {
            return false;  // Enemy is too close
        }
    }
    int gridX = getGridX(snappedPos.x);
    int gridY = getGridY(snappedPos.y);
    if (gridX == 0 || gridX == endX - 1) {
        return false;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int cellX = gridX + i;
            int cellY = gridY + j;
            if (cellX < 0 || cellX >= GRID_WIDTH * 2 || cellY < 0 || cellY >= GRID_HEIGHT * 2 || grid[cellX][cellY]) {
                return false;
            }
        }
    }

    // Temporarily place tower on grid
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            grid[gridX + i][gridY + j] = true;
        }
    }

    // Check if path is still available with the temporary tower
    bool pathExists = !findPathBFS({startX, startY}, {endX, endY}).empty();

    // Remove temporary tower from grid
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            grid[gridX + i][gridY + j] = false;
        }
    }

    if (!pathExists) {
        printf("The tower would block the path!\n");
        return false;
    }

    return true; // All checks passed, space is free
}

std::vector<std::pair<int, int>> Arena::findPathAStar(const std::pair<int, int>& start, const std::pair<int, int>& end) {
    std::vector<std::pair<int, int>> directions = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, // Preferable Diagonals: Down-Right, Up-Right, Down-Left, Up-Left
        {1, 0}, {0, 1}, {-1, 0}, {0, -1} // Right, Down, Left, Up
    };

    auto heuristic = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return std::hypot(a.first - b.first, a.second - b.second); // Euclidean distance
    };

    std::priority_queue<
        std::pair<float, std::pair<int, int>>,
        std::vector<std::pair<float, std::pair<int, int>>>,
        std::greater<std::pair<float, std::pair<int, int>>>
    > queue;

    std::unordered_map<int, std::pair<int, int>> cameFrom;
    std::unordered_map<int, float> costSoFar;
    std::vector<std::pair<int, int>> path;
    int cols = GRID_WIDTH * 2;

    auto toIndex = [&](int x, int y) { return y * cols + x; };

    queue.push({0, start});
    cameFrom[toIndex(start.first, start.second)] = {-1, -1};
    costSoFar[toIndex(start.first, start.second)] = 0;

    while (!queue.empty()) {
        auto current = queue.top().second;
        queue.pop();

        if (current == end) {
            std::pair<int, int> step = end;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[toIndex(step.first, step.second)];
                if (step == std::pair<int, int>{-1, -1}) break; // If reach an undefined step, stop
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : directions) {
            std::pair<int, int> next = {current.first + dir.first, current.second + dir.second};

            if (next.first >= 0 && next.first < GRID_WIDTH * 2 && next.second >= 0 && next.second < GRID_HEIGHT * 2) {
                if (std::abs(dir.first) == 1 && std::abs(dir.second) == 1) {
                    if (grid[current.first + dir.first][current.second] || grid[current.first][current.second + dir.second]) {
                        continue; // Skip if diagonal movement is blocked by a tower
                    }
                }

                float newCost = costSoFar[toIndex(current.first, current.second)] + heuristic(current, next);
                if (!grid[next.first][next.second] && (costSoFar.find(toIndex(next.first, next.second)) == costSoFar.end() || newCost < costSoFar[toIndex(next.first, next.second)])) {
                    costSoFar[toIndex(next.first, next.second)] = newCost;
                    float priority = newCost + heuristic(next, end);
                    queue.push({priority, next});
                    cameFrom[toIndex(next.first, next.second)] = current;
                }
            }
        }
    }

    return {};
}


void Arena::updateGridForNewTower(int gridX, int gridY) {
    // std::cout << "Updating grid for new tower at grid position (" << gridX << ", " << gridY << "):\n";
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int cellX = gridX + i;
            int cellY = gridY + j;
            if (cellX >= 0 && cellX < GRID_WIDTH * 2 && cellY >= 0 && cellY < GRID_HEIGHT * 2) {
                grid[cellX][cellY] = true; // Mark the cell as occupied by the tower
                // std::cout << "Marked cell (" << cellX << ", " << cellY << ") as occupied.\n";
            } else {
                // std::cout << "Cell (" << cellX << ", " << cellY << ") is out of bounds and not marked.\n";
            }
        }
    }
}

bool Arena::deleteTower(const sf::Vector2f& position) {
    sf::Vector2f snappedPos = snapToGrid(position);
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        if ((*it)->getPosition() == snappedPos) {
            int towerX = (*it)->getShape().getPosition().x;
            int towerY = (*it)->getShape().getPosition().y;
            int gridX = getGridX(towerX);
            int gridY = getGridY(towerY);

            freeGridForDeletedTower(gridX, gridY);
            
            (*it)->clearBullets(); // Clear bullets before deleting the tower
            
            gold += (*it)->getCost() * 0.6;
            towers.erase(it);

            justAddedTower = true;  // You might rename this flag to something like `towerLayoutChanged`.
            
            return true; // Tower successfully deleted.
        }
    }
    return false; // No tower found at the given position.
}

bool Arena::upgradeTower(const sf::Vector2f& position) {
    sf::Vector2f snappedPos = snapToGrid(position);
    for (auto& tower : towers) {
        if (tower->getPosition() == snappedPos) {
            if (tower->canUpgrade()) {
                int upgradeCost = tower->getNextUpgradeCost();
                if (gold >= upgradeCost) {
                    tower->upgrade();
                     gold -= upgradeCost;
                    return true; // Tower successfully updated.
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    return false; // No tower found at the given position.
}

void Arena::freeGridForDeletedTower(int gridX, int gridY) {
    // std::cout << "Freeing grid cells previously occupied by a tower at (" << gridX << ", " << gridY << "):\n";
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int cellX = gridX + i;
            int cellY = gridY + j;
            if (cellX >= 0 && cellX < GRID_WIDTH * 2 && cellY >= 0 && cellY < GRID_HEIGHT * 2) {
                grid[cellX][cellY] = false; // Mark the cell as free
                // std::cout << "Freed cell (" << cellX << ", " << cellY << ").\n";
            }
        }
    }
}

void Arena::checkBulletEnemyCollisions() {
    for (auto& tower : towers) {
        auto& bullets = tower->getBullets();
        for (auto bulletIter = bullets.begin(); bulletIter != bullets.end();) {
            bool bulletDeleted = false;
            auto& bullet = *bulletIter;
            FrostBullet* frostBullet = dynamic_cast<FrostBullet*>(bullet.get());
            ExplosiveBullet* explosiveBullet = dynamic_cast<ExplosiveBullet*>(bullet.get());

            for (auto& enemy : enemies) {
                if (bullet->getShape().getGlobalBounds().intersects(enemy->getShape().getGlobalBounds())) {
                    if (explosiveBullet) {
                        for (auto& otherEnemy : enemies) {
                            sf::Vector2f bulletPos = bullet->getPosition();
                            sf::Vector2f enemyPos = otherEnemy->getShape().getPosition();
                            float distance = std::hypot(bulletPos.x - enemyPos.x, bulletPos.y - enemyPos.y);

                            if (distance <= explosiveBullet->getExplosionRadius()) {
                                otherEnemy->damage(explosiveBullet->getDamage());
                            }
                        }
                        bulletDeleted = true;
                    } else if (frostBullet) {
                        for (auto& otherEnemy : enemies) {
                            sf::Vector2f bulletPos = bullet->getPosition();
                            sf::Vector2f enemyPos = otherEnemy->getShape().getPosition();
                            float distance = std::hypot(bulletPos.x - enemyPos.x, bulletPos.y - enemyPos.y);

                            if (distance <= frostBullet->getAreaEffectRadius()) {
                                otherEnemy->damage(frostBullet->getDamage());
                                otherEnemy->applySlow(frostBullet->getSlowFactor(), frostBullet->getSlowDuration());
                            }
                        }
                        bulletDeleted = true;
                    } else {
                        enemy->damage(bullet->getDamage());
                        bulletDeleted = true;
                    }

                    if (bulletDeleted) break;
                }
            }
            if (bulletDeleted) {
                bulletIter = bullets.erase(bulletIter);
            } else {
                ++bulletIter;
            }
        }
    }
}

std::vector<std::pair<int, int>> Arena::findPathBFS(const std::pair<int, int>& start, const std::pair<int, int>& end) {
    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {1, -1}, {1, 1}, {0, 1}, // Right, Down, Left, Up
        {0, -1}, {-1, 0}, {-1, 1}, {-1, -1} // Diagonals: Down-Right, Up-Right, Down-Left, Up-Left
    };
    std::queue<std::pair<int, int>> queue;
    std::unordered_map<int, std::pair<int, int>> cameFrom;
    std::vector<std::pair<int, int>> path;
    int cols = GRID_WIDTH * 2;

    auto toIndex = [&](int x, int y) { return y * cols + x; };
    queue.push(start);
    cameFrom[toIndex(start.first, start.second)] = {-1, -1}; // Mark start position came from nowhere

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current == end) {
            std::pair<int, int> step = end;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[toIndex(step.first, step.second)];
                if (step == std::pair<int, int>{-1, -1}) break; // If reach an undefined step, stop
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : directions) {
            std::pair<int, int> next = {current.first + dir.first, current.second + dir.second};

            if (next.first >= 0 && next.first < GRID_WIDTH * 2 && next.second >= 0 && next.second < GRID_HEIGHT * 2) {
                // Check for diagonal movement constraints
                if (std::abs(dir.first) == 1 && std::abs(dir.second) == 1) {
                    // Diagonal movement
                    if (grid[current.first + dir.first][current.second] || grid[current.first][current.second + dir.second]) {
                        continue; // Skip if diagonal movement is blocked by a tower
                    }
                }

                if (!grid[next.first][next.second] && cameFrom.find(toIndex(next.first, next.second)) == cameFrom.end()) {
                    queue.push(next);
                    cameFrom[toIndex(next.first, next.second)] = current;
                }
            }
        }
    }
    
    return {};
}


void Arena::printGrid() const {
    // std::cout << "Current Grid State:\n";
    for (int y = 0; y < GRID_HEIGHT * 2; y++) {
        for (int x = 0; x < GRID_WIDTH * 2; x++) {
            if (grid[x][y]) {
                // std::cout << "X ";
            } else {
                // std::cout << ". ";
            }
        }
        // std::cout << "\n";
    }
    // std::cout << std::endl;
}

void Arena::printPath(const std::vector<std::pair<int, int>>& path) {
    if (path.empty()) {
        // std::cout << "No path found." << std::endl;
        return;
    }

    // std::cout << "Path: ";
    for (const auto& step : path) {
        // std::cout << "(" << step.first << ", " << step.second << ") -> ";
    }
    // std::cout << "End" << std::endl;
}

void Arena::markPathCellsRed(const std::vector<std::pair<int, int>>& path) {
    pathCells.clear();
    for (const auto& cell : path) {
        sf::RectangleShape rect(sf::Vector2f(GRID_CELL_SIZE, GRID_CELL_SIZE));
        rect.setPosition(cell.first * GRID_CELL_SIZE + LEFT_OFFSET, cell.second * GRID_CELL_SIZE + TOP_OFFSET);
        rect.setFillColor(sf::Color::Red);
        pathCells.push_back(rect);
    }
}

void Arena::spawnEnemy(int health, int value) {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_real_distribution<> distr(TOP_OFFSET + 9.5 * GRID_CELL_SIZE, TOP_OFFSET + 14.5 * GRID_CELL_SIZE);

    float y = distr(eng);
    enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(0, y), health, value, currentLevel.flying, currentLevel.spawn, currentLevel.immune, currentLevel.fast));
}

int Arena::getLives() const {
    return lives;
}

void Arena::setLives(int lives) {
    this->lives = lives;
}

int Arena::getGold() const {
    return gold;
}

void Arena::setGold(int gold) {
    this->gold = gold;
}

int Arena::getScore() const {
    return score;
}

void Arena::setScore(int score) {
    this->score = score;
}

int Arena::getLevel() const {
    return currentLevelIndex;
}

void Arena::setLevel(int score) {
    this->level = level;
}


