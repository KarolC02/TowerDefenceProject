
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

class Game {
public:
    Game();
    void Run();

    enum class GameState {
        Default,
        PlacingTower,
        TowerSelected,
        GameOver,
        Paused
    };

    void setGameState(GameState newState); // To change game states
    void handleMouseHover(sf::Vector2i hoverPosition);
    std::vector<TowerButton> buttons;
private:

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
    Button ResetButton;
    sf::CircleShape towerRangeCircle;
    
    void updateDebts();
    TowerType currentlyPlacing;
    bool showPreview = true;
    std::unique_ptr<Tower> previewTower;
    GameState currentState;
    sf::RenderWindow window;
    Arena arena;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<Enemy> enemies;
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
    
    
    int time = 1, level = 1, lives = 20, gold = getJsonValue(CONFIG_PATH, "START_MONEY"), score = 0;
    void updateTowerMenuInfo( Tower& tower );
    void setupText(sf::Text& text, const std::string& value, float x, float y, sf::Color color);
    void updateStatsDisplay();  // Updates the text objects with the current stats
    void updateTowerDisplayInfo(Tower& tower );


    void displayTowerMenu(sf::RenderWindow& window);
    sf::Vector2i getGridPosition(const sf::Vector2f& position) const;
    bool showTowerMenu;
    void displayInfoBar(sf::RenderWindow& window);
    void displayTowerInfo(sf::RenderWindow& window);
    void resetGame();
    void setCurrentlyPlacing(TowerType type);
    void processEvents();
    void update(float dt);
    void render();
    void handleMouseClick(sf::Vector2i clickPosition);
    bool isFreeSpace(const sf::Vector2f& gridPosition) const;
    bool collision(const sf::Shape& shape1, const sf::Shape& shape2) const;
};
