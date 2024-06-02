
#pragma once
#include "DEFINE.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.hpp"
#include "Tower.hpp"
#include "PelletTower.hpp"
#include "Bullet.hpp"
#include "Arena.hpp"
#include "TowerButton.hpp"
#include <vector>
#include "Button.hpp"
#include "JsonUtils.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include "SocketServer.hpp" // Include the socket server header
#include "CommandQueue.hpp"

class Game {
public:
    Game(int port);
    void Run();
    void placeTower(TowerType towerType, sf::Vector2f position);
    void placeTowerAtGrid(TowerType towerType, int gridX, int gridY);
    void addCommand(std::function<void()> command);
    
    enum class GameState {
        Default,
        PlacingTower,
        TowerSelected,
        GameOver,
        Paused,
    };
    std::vector<TowerButton> buttons;
    
    void startGame();
    void setGameState(GameState newState);
    void handleMouseHover(sf::Vector2i hoverPosition);
    sf::Vector2f getGridCellPosition(int gridX, int gridY) const;
    void sellTowerAtGrid(int gridX, int gridY);
    void upgradeTowerAtGrid(int gridX, int gridY);
    void resetGame();
    bool getGameOver() const;
private:
    
    sf::RectangleShape gameOverOverlay;
    sf::Text gameOverText;
    bool gameOver;
    
    std::unique_ptr<Arena> arena;
    SocketServer socketServer;
    bool gameStarted;
    sf::Clock pausedClock;
    sf::RectangleShape pauseOverlay;
    sf::Text pausedText;
    Button PauseButton;
    sf::CircleShape dot;
    TowerButton pelletTowerButton;
    TowerButton SwarmTowerButton;
    TowerButton DartTowerButton;
    TowerButton SquirtTowerButton;
    TowerButton BashTowerButton;
    TowerButton FrostTowerButton;
    Button StartButton;
    Button ResetButton;
    sf::CircleShape towerRangeCircle;
    
    void updateDebts();
    TowerType currentlyPlacing;
    bool showPreview = true;
    std::unique_ptr<Tower> previewTower;
    GameState currentState;
    sf::RenderWindow window;
//    std::vector<std::unique_ptr<Tower>> towers;
//    std::vector<Enemy> enemies;
    
    mutable std::mutex gameMutex; // Mutex to protect game state
    CommandQueue commandQueue;    // Command queue to serialize commands
    
    sf::Font gameFont;
    
    sf::Text timeText, levelText, livesText, goldText, scoreText;
    
    sf::RectangleShape infoBar;
    sf::RectangleShape towerInfoBar;
    sf::Text _towerCost, _towerDamage, _towerRange, _towerSpeed, _towerInfoText;
    Button SellTowerButton;
    
    sf::RectangleShape upgradeTowerInfoBar;
    sf::Text _upgradeTowerCost, _upgradeTowerDamage, _upgradeTowerRange, _upgradeTowerSpeed, _upgradeTowerInfoText;
    Button UpgradeTowerButton;
    
    void updateUpgradeDisplayInfo(Tower& tower);
    
    int time = 1;
    
    void updateTowerMenuInfo( Tower& tower );
    void setupText(sf::Text& text, const std::string& value, float x, float y, sf::Color color);
    void updateStatsDisplay();  // Updates the text objects with the current stats
    void updateTowerDisplayInfo(Tower& tower );
    void displayTowerMenu(sf::RenderWindow& window);
    sf::Vector2i getGridPosition(const sf::Vector2f& position) const;
    bool showTowerMenu;
    void displayInfoBar(sf::RenderWindow& window);
    void displayTowerInfo(sf::RenderWindow& window);
    void setCurrentlyPlacing(TowerType type);
    void processEvents();
    void update(float dt);
    void render();
    void handleMouseClick(sf::Vector2i clickPosition);
    bool isFreeSpace(const sf::Vector2f& gridPosition) const;
    bool collision(const sf::Shape& shape1, const sf::Shape& shape2) const;
    
};
