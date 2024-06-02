#ifndef ARENA_HPP
#define ARENA_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include "LevelManager.hpp"
#include "Tower.hpp"
#include "Enemy.hpp"
#include "DEFINE.hpp"
#include <mutex> // Include mutex
#include <shared_mutex>

class Arena {
public:
    Arena(float width, float height, sf::Vector2f position);
    
    void loadNextLevel();
    void update(float dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
    void drawGrid(sf::RenderWindow& window) const;
    
    std::vector<std::unique_ptr<Tower>>& getTowers();
    std::vector<std::shared_ptr<Enemy>>& getEnemies();

    bool addTower(std::unique_ptr<Tower> tower);
    bool deleteTower(const sf::Vector2f& position);
    bool upgradeTower(const sf::Vector2f& position);
    
    sf::Vector2f snapToGrid(const sf::Vector2f& position) const;
    bool isSpaceFree(const sf::Vector2f& position);
    bool isWithinBounds(sf::Vector2f snappedPos);

    int getGridX(int x);
    int getGridY(int y);

    void manageLevels();
    void spawnCurrentLevelEnemies();
    void updateTowers(float dt, const sf::RenderWindow& window);
    void updateEnemies(float dt);
    void removeDeadOrOutOfBoundsEnemies();
    void checkBulletEnemyCollisions();

    void printGrid() const;
    void printPath(const std::vector<std::pair<int, int>>& path);
    void markPathCellsRed(const std::vector<std::pair<int, int>>& path);
    
    void spawnEnemy(int health, int value);

    int random(int min, int max);

    std::vector<std::pair<int, int>> findPathAStar(const std::pair<int, int>& start, const std::pair<int, int>& end);
    std::vector<std::pair<int, int>> findPathBFS(const std::pair<int, int>& start, const std::pair<int, int>& end);
    bool grid[GRID_WIDTH * 2][GRID_HEIGHT * 2];
    
    Level currentLevel;
    sf::Clock currentLevelTimer;
    int payCheck;
    int scorePayCheck;
    int livesDebt;
    float pausedTime;
    void reset();
    
    int getLives() const;
    void setLives(int lives);
    int getGold() const;
    void setGold(int gold);
    int getScore() const;
    void setScore(int score);
    int getLevel() const;
    void setLevel(int score);
    
private:
    
    int lives;
    int gold;
    int score;
    int level;
    
    void drawBullets(sf::RenderWindow &window) const;
    
    sf::RectangleShape background;
    sf::RectangleShape topFrame1, topFrame2, topFrame3;
    sf::RectangleShape bottomFrame1, bottomFrame2, bottomFrame3;
    
    
    std::vector<std::unique_ptr<Tower>> towers;
    
    mutable std::shared_timed_mutex mtx;

    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<sf::RectangleShape> pathCells;

    std::vector<Level> levels;
    int currentLevelIndex;
    int alreadySpawnedThisLevel;
    bool readyForNextLevel;
    
    sf::Clock enemySpawnTimer;

    bool justAddedTower;

    void updateGridForNewTower(int gridX, int gridY);
    void freeGridForDeletedTower(int gridX, int gridY);
    
    void drawTowers(sf::RenderWindow &window) const;
void drawEnemies(sf::RenderWindow &window) const;

    void drawBackground(sf::RenderWindow& window) const;

};


#endif // ARENA_HPP
