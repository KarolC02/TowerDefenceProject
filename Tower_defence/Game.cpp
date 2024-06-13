#include "BashTower.hpp"
#include "DEFINE.hpp"
#include "DartTower.hpp"
#include "FrostTower.hpp"
#include "Game.hpp"
#include "PelletTower.hpp"
#include "ResourcePath.hpp"
#include "SquirtTower.hpp"
#include "SwarmTower.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

Game::Game(int port)
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Desktop Tower Defence"),
      arena(std::make_unique<Arena>(ARENA_WIDTH, ARENA_HEIGHT, sf::Vector2f(LEFT_OFFSET, TOP_OFFSET))),
      currentState(GameState::Default),
      currentlyPlacing(TowerType::None),
      gameStarted(false),
      socketServer(port, *this),
      gameOver(false){

    if (!gameFont.loadFromFile(resourcePath() + "arial.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    towerRangeCircle.setFillColor(sf::Color(255, 255, 255, 50));
    towerRangeCircle.setOrigin(towerRangeCircle.getGlobalBounds().width / 2, towerRangeCircle.getGlobalBounds().height / 2);
    towerRangeCircle.setPosition(0, 0);

    showTowerMenu = false;
    infoBar.setSize(sf::Vector2f(WINDOW_WIDTH, INFOBAR_HEIGHT));
    infoBar.setPosition(0, 0);
    infoBar.setFillColor(INFOBAR_COLOR);

    dot.setRadius(5.0f);
    dot.setPosition(50, 50);
    dot.setOrigin(dot.getGlobalBounds().width / 2, dot.getGlobalBounds().height / 2);
    dot.setFillColor(sf::Color::Red);

    setupText(timeText, "Time: 0", WINDOW_WIDTH / 6, INFOBAR_HEIGHT / 2, sf::Color::White);
    setupText(levelText, "Level: 1", WINDOW_WIDTH / 6 * 2, INFOBAR_HEIGHT / 2, sf::Color::Blue);
    setupText(livesText, "Lives: 3", WINDOW_WIDTH / 6 * 3, INFOBAR_HEIGHT / 2, sf::Color::Green);
    setupText(goldText, "Gold: 100", WINDOW_WIDTH / 6 * 4, INFOBAR_HEIGHT / 2, sf::Color::Yellow);
    setupText(scoreText, "Score: 0", WINDOW_WIDTH / 6 * 5, INFOBAR_HEIGHT / 2, sf::Color::Red);

    // Button setup
    {
        // Reset Button Setup
        sf::Vector2f resetButtonPosition(WINDOW_WIDTH - 100, 50);  // Top-right corner, adjust as needed
        sf::Vector2f resetButtonSize(90, 30);  // Reasonable size for clicking
        std::string resetButtonText = "Reset";
        ResetButton = Button(resetButtonPosition, resetButtonSize, resetButtonText, sf::Color::Red);

        // Pause Button Setup
        sf::Vector2f pauseButtonPosition(WINDOW_WIDTH - 100, 100);  // Adjust position as needed
        sf::Vector2f pauseButtonSize(90, 30);  // Reasonable size for clicking
        std::string pauseButtonText = "Pause";
        PauseButton = Button(pauseButtonPosition, pauseButtonSize, pauseButtonText, sf::Color::Blue);

        // Start Button Setup
        sf::Vector2f startButtonPosition(WINDOW_WIDTH - 100, 150);  // Adjust position as needed
        sf::Vector2f startButtonSize(90, 30);  // Reasonable size for clicking
        std::string startButtonText = "Start";
        StartButton = Button(startButtonPosition, startButtonSize, startButtonText, sf::Color::Green);

        // Pause effect setup
        pauseOverlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
        pausedText.setFont(gameFont);
        pausedText.setString("Paused");
        pausedText.setCharacterSize(48);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setStyle(sf::Text::Bold);
        sf::FloatRect textRect = pausedText.getLocalBounds();
        pausedText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        pausedText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        
        // Initialize game over overlay
        
        

        sf::Vector2f buttonPosition(WINDOW_WIDTH - 8 * TOWER_SIDE_LENGTH, WINDOW_HEIGHT / 4);
        sf::Vector2f buttonSize(TOWER_SIDE_LENGTH, TOWER_SIDE_LENGTH);
        std::string buttonText = "P";
        pelletTowerButton = TowerButton(buttonPosition, buttonSize, buttonText, PELLET_TOWER_COLOR, TowerType::Pellet);
        buttons.push_back(pelletTowerButton);

        sf::Vector2f buttonPosition1(WINDOW_WIDTH - 7 * TOWER_SIDE_LENGTH, WINDOW_HEIGHT / 4);
        std::string buttonText1 = "SW";
        SwarmTowerButton = TowerButton(buttonPosition1, buttonSize, buttonText1, SWARM_TOWER_COLOR, TowerType::Swarm);
        buttons.push_back(SwarmTowerButton);

        sf::Vector2f buttonPosition2(WINDOW_WIDTH - 6 * TOWER_SIDE_LENGTH, WINDOW_HEIGHT / 4);
        std::string buttonText2 = "D";
        DartTowerButton = TowerButton(buttonPosition2, buttonSize, buttonText2, DART_TOWER_COLOR, TowerType::Dart);
        buttons.push_back(DartTowerButton);

        sf::Vector2f buttonPosition3(WINDOW_WIDTH - 5 * TOWER_SIDE_LENGTH, WINDOW_HEIGHT / 4);
        std::string buttonText3 = "S";
        SquirtTowerButton = TowerButton(buttonPosition3, buttonSize, buttonText3, SQUIRT_TOWER_COLOR, TowerType::Squirt);
        buttons.push_back(SquirtTowerButton);

        sf::Vector2f buttonPosition4(WINDOW_WIDTH - 4 * TOWER_SIDE_LENGTH, WINDOW_HEIGHT / 4);
        std::string buttonText4 = "B";
        BashTowerButton = TowerButton(buttonPosition4, buttonSize, buttonText4, BASH_TOWER_COLOR, TowerType::Bash);
        buttons.push_back(BashTowerButton);

        sf::Vector2f buttonPosition5(WINDOW_WIDTH - 3 * TOWER_SIDE_LENGTH, WINDOW_HEIGHT / 4);
        std::string buttonText5 = "F";
        FrostTowerButton = TowerButton(buttonPosition5, buttonSize, buttonText5, FROST_TOWER_COLOR, TowerType::Frost);
        buttons.push_back(FrostTowerButton);
    }

    setupText(_towerInfoText, "Info", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 1, sf::Color::Black);
    setupText(_towerCost, "Cost: xx", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 2, sf::Color::Yellow);
    setupText(_towerDamage, "Damage: xx", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 3, sf::Color::Red);
    setupText(_towerRange, "Range: xx", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 4, sf::Color::Blue);
    setupText(_towerSpeed, "speed: xx", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 5, sf::Color::Black);

    towerInfoBar.setSize(sf::Vector2f(6 * TOWER_SIDE_LENGTH, 6 * TOWER_SIDE_LENGTH));
    towerInfoBar.setPosition(pelletTowerButton.getPosition() + sf::Vector2f(-GRID_CELL_SIZE, -GRID_CELL_SIZE));
    towerInfoBar.setFillColor(TOWER_INFO_BAR_COLOR);

    // SetUp upgrade text
    setupText(_upgradeTowerInfoText, "Info", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 7, sf::Color::Black);
    setupText(_upgradeTowerCost, "Cost: xx", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 8, sf::Color::Yellow);
    setupText(_upgradeTowerDamage, "Damage: xx", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 9, sf::Color::Red);
    setupText(_upgradeTowerRange, "Range: xx", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 10, sf::Color::Blue);
    setupText(_upgradeTowerSpeed, "speed: xx", (DartTowerButton.getPosition().x + SquirtTowerButton.getPosition().x) / 2, DartTowerButton.getPosition().y + TOWER_SIDE_LENGTH * 11, sf::Color::Black);

    // Sell Button Setup
    sf::Vector2f sellTowerButtonPosition(towerInfoBar.getPosition() + sf::Vector2f(1.5 * TOWER_SIDE_LENGTH, 6 * TOWER_SIDE_LENGTH));
    sf::Vector2f sellTowerSize(3 * TOWER_SIDE_LENGTH, TOWER_SIDE_LENGTH);  // Reasonable size for clicking
    std::string sellTowerText = "Sell Tower";
    SellTowerButton = Button(sellTowerButtonPosition, sellTowerSize, sellTowerText, sf::Color(122, 41, 41, 240));

    upgradeTowerInfoBar.setSize(sf::Vector2f(6 * TOWER_SIDE_LENGTH, 5 * TOWER_SIDE_LENGTH));
    upgradeTowerInfoBar.setPosition(towerInfoBar.getPosition() + sf::Vector2f(0, towerInfoBar.getGlobalBounds().height + sellTowerSize.y));
    upgradeTowerInfoBar.setFillColor(TOWER_INFO_BAR_COLOR);

    // Upgrade Button SetUp
    sf::Vector2f upgradeTowerButtonPosition(sellTowerButtonPosition + sf::Vector2f(0, 6 * TOWER_SIDE_LENGTH));
    sf::Vector2f upgradeTowerButtonSize(3 * TOWER_SIDE_LENGTH, TOWER_SIDE_LENGTH);  // Reasonable size for clicking
    std::string upgradeTowerButtonText = "Upgrade";
    UpgradeTowerButton = Button(upgradeTowerButtonPosition, upgradeTowerButtonSize, upgradeTowerButtonText, sf::Color(122, 41, 41, 240));
}

void Game::Run() {
    sf::Clock clock;
    socketServer.start();
    const float timestep = 1.0f / 60.0f;
    float accumulator = 0.0f;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        accumulator += deltaTime.asSeconds();

        processEvents();

        while (accumulator >= timestep) {
            accumulator -= timestep;
            update(timestep);
        }
        handleMouseHover(sf::Mouse::getPosition(window));
        render();
        
        std::function<void()> command;
        while (commandQueue.pop(command)) {
            command();
        }
        
    }
    socketServer.stop();
}

void Game::setupText(sf::Text& text, const std::string& value, float x, float y, sf::Color color) {
    text.setFont(gameFont);
    text.setString(value);
    text.setCharacterSize(24);
    text.setFillColor(color);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(x, y);
}

void Game::updateStatsDisplay() {
    timeText.setString("Time: " + std::to_string(time));
    levelText.setString("Level: " + std::to_string(arena->getLevel()));
    livesText.setString("Lives: " + std::to_string(arena->getLives()));
    goldText.setString("Gold: " + std::to_string(arena->getGold()));
    scoreText.setString("Score: " + std::to_string(arena->getScore()));
}

void Game::updateTowerMenuInfo(Tower& tower) {
    updateTowerDisplayInfo(tower);
    updateUpgradeDisplayInfo(tower);
    towerRangeCircle.setRadius(tower.getRange());
    towerRangeCircle.setOrigin(towerRangeCircle.getGlobalBounds().width / 2, towerRangeCircle.getGlobalBounds().height / 2);
    towerRangeCircle.setPosition(tower.getPosition());
}

void Game::updateTowerDisplayInfo(Tower& tower) {
    _towerInfoText.setString(tower.getInfo());
    sf::FloatRect textRect = _towerInfoText.getLocalBounds();
    _towerInfoText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    _towerCost.setString("Cost: " + std::to_string(tower.getCost()));
    _towerRange.setString("Range: " + std::to_string(tower.getRange()));
    _towerDamage.setString("Damage: " + std::to_string(tower.getDamage()));
    _towerSpeed.setString("Speed: " + std::to_string(tower.getSpeed()));
}

void Game::updateUpgradeDisplayInfo(Tower& tower) {
    if (tower.canUpgrade()) {
        _upgradeTowerDamage.setString("Damage: " + std::to_string(tower.getNextUpgradeDamage()));
        _upgradeTowerCost.setString("Cost: " + std::to_string(tower.getNextUpgradeCost()));
        _upgradeTowerRange.setString("Range: " + std::to_string(tower.getNextUpgradeRange()));
        _upgradeTowerSpeed.setString("Speed: " + std::to_string(tower.getNextUpgradeSpeed()));

        _upgradeTowerInfoText.setString(tower.getNextUpgradeName());
        sf::FloatRect upgradeTextRect = _upgradeTowerInfoText.getLocalBounds();
        _upgradeTowerInfoText.setOrigin(upgradeTextRect.left + upgradeTextRect.width / 2.0f, upgradeTextRect.top + upgradeTextRect.height / 2.0f);
    } else {
        // Display that no further upgrades are available
        _upgradeTowerCost.setString("Cost: N/A");
        _upgradeTowerDamage.setString("Damage: N/A");
        _upgradeTowerRange.setString("Range: N/A");
        _upgradeTowerSpeed.setString("Speed: N/A");
        _upgradeTowerInfoText.setString("Max Level Reached");
        sf::FloatRect upgradeTextRect = _upgradeTowerInfoText.getLocalBounds();
        _upgradeTowerInfoText.setOrigin(upgradeTextRect.left + upgradeTextRect.width / 2.0f, upgradeTextRect.top + upgradeTextRect.height / 2.0f);
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R) {
                    printJsonFile(CONFIG_PATH);
                }
                break; // Add break to prevent fall-through

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (PauseButton.isMouseOver(window)) {
                    if (currentState == GameState::Paused) { // Resume the game
                        currentState = GameState::Default;
                        arena->pausedTime += pausedClock.getElapsedTime().asSeconds();
                        pausedClock.restart();

                    } else {
                        currentState = GameState::Paused; // Pause the game
                        pausedClock.restart();
                    }
                } else if (StartButton.isMouseOver(window) && !gameStarted) {
                    // std::cout << "Start Button Clicked\n";
                    gameStarted = true;  // Start the game
                } else if (pelletTowerButton.isMouseOver(window)) {
                    // std::cout << "Pellet Tower Button Clicked\n";
                    currentState = GameState::PlacingTower;
                    previewTower = std::make_unique<PelletTower>(sf::Vector2f(0, 0));  // Position will be updated on hover
                    currentlyPlacing = TowerType::Pellet;
                    showPreview = true;
                    showTowerMenu = false;
                    updateTowerDisplayInfo(*previewTower);
                } else if (SwarmTowerButton.isMouseOver(window)) {
                    // std::cout << "Swarm Tower Button Clicked\n";
                    currentState = GameState::PlacingTower;
                    previewTower = std::make_unique<SwarmTower>(sf::Vector2f(0, 0));  // Adjust the class name as necessary
                    currentlyPlacing = TowerType::Swarm;
                    showPreview = true;
                    showTowerMenu = false;
                    updateTowerDisplayInfo(*previewTower);
                } else if (DartTowerButton.isMouseOver(window)) {
                    // std::cout << "Dart Tower Button Clicked\n";
                    currentState = GameState::PlacingTower;
                    previewTower = std::make_unique<DartTower>(sf::Vector2f(0, 0));  // Adjust the class name as necessary
                    currentlyPlacing = TowerType::Dart;
                    showPreview = true;
                    showTowerMenu = false;
                    updateTowerDisplayInfo(*previewTower);
                } else if (SquirtTowerButton.isMouseOver(window)) {
                    // std::cout << "Squirt Tower Button Clicked\n";
                    currentState = GameState::PlacingTower;
                    previewTower = std::make_unique<SquirtTower>(sf::Vector2f(0, 0));  // Adjust the class name as necessary
                    currentlyPlacing = TowerType::Squirt;
                    showPreview = true;
                    showTowerMenu = false;
                    updateTowerDisplayInfo(*previewTower);
                } else if (BashTowerButton.isMouseOver(window)) {
                    // std::cout << "Bash Tower Button Clicked\n";
                    currentState = GameState::PlacingTower;
                    previewTower = std::make_unique<BashTower>(sf::Vector2f(0, 0));  // Adjust the class name as necessary
                    currentlyPlacing = TowerType::Bash;
                    showPreview = true;
                    showTowerMenu = false;
                    updateTowerDisplayInfo(*previewTower);
                } else if (FrostTowerButton.isMouseOver(window)) {
                    // std::cout << "Frost Tower Button Clicked\n";
                    currentState = GameState::PlacingTower;
                    previewTower = std::make_unique<FrostTower>(sf::Vector2f(0, 0));  // Adjust the class name as necessary
                    currentlyPlacing = TowerType::Frost;
                    showPreview = true;
                    showTowerMenu = false;
                    updateTowerDisplayInfo(*previewTower);

                } else {
                    handleMouseClick(sf::Mouse::getPosition(window));
                }
                break;
        }
    }
}

void Game::update(float dt) {
    if( !gameOver ){
        if (currentState != GameState::Paused && gameStarted) {
            arena->update(dt, window);
            time = arena->currentLevel.maxDuration - (int)arena->currentLevelTimer.getElapsedTime().asSeconds() + arena->pausedTime;
        }
        if( arena->getLives() <= 0 ){
            gameOver = true;
        }
    }else{
        
    }
}

void Game::render() {
    window.clear(sf::Color(211, 211, 211));
    updateStatsDisplay();
    displayInfoBar(window);
    displayTowerInfo(window);
    arena->draw(window);
    if (showTowerMenu) {
        SellTowerButton.draw(window);
        UpgradeTowerButton.draw(window);
        displayTowerMenu(window);
    }
    for (auto& button : buttons) {
        button.draw(window);
    }
    ResetButton.draw(window);
    PauseButton.draw(window);
    if (!gameStarted) {
        StartButton.draw(window);
    }

    if (showPreview && previewTower) {
        previewTower->draw(window);
        sf::CircleShape rangeCircle(previewTower->getRange());
        rangeCircle.setFillColor(sf::Color(255, 255, 255, 50));
        rangeCircle.setPosition(previewTower->getPosition());
        rangeCircle.setOrigin(previewTower->getRange(), previewTower->getRange());
        window.draw(rangeCircle);
    }
    if (showTowerMenu) {
        window.draw(towerRangeCircle);
    }

    if (currentState == GameState::Paused) {
        window.draw(pauseOverlay);
        window.draw(pausedText);
    }
    if( gameOver ){
        window.draw(pauseOverlay);
        gameOverText.setFont(gameFont);
        gameOverText.setString("Game Over! Your score is " + std::to_string(arena->getScore()));
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setStyle(sf::Text::Bold);
        sf::FloatRect textRect1 = gameOverText.getLocalBounds();
        gameOverText.setOrigin(textRect1.left + textRect1.width / 2.0f, textRect1.top + textRect1.height / 2.0f);
        gameOverText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        window.draw(gameOverText);
    }

    window.display();
}

void Game::displayInfoBar(sf::RenderWindow& window) {
    window.draw(infoBar);
    window.draw(timeText);
    window.draw(levelText);
    window.draw(livesText);
    window.draw(goldText);
    window.draw(scoreText);
}

void Game::displayTowerMenu(sf::RenderWindow& window) {
    window.draw(towerInfoBar);
    window.draw(_towerInfoText);
    window.draw(_towerCost);
    window.draw(_towerRange);
    window.draw(_towerSpeed);
    window.draw(_towerDamage);
    window.draw(towerRangeCircle);

    window.draw(upgradeTowerInfoBar);
    window.draw(_upgradeTowerInfoText);
    window.draw(_upgradeTowerCost);
    window.draw(_upgradeTowerDamage);
    window.draw(_upgradeTowerRange);
    window.draw(_upgradeTowerSpeed);
}

// TO używamy gdy stawiamy wieżyczke
void Game::displayTowerInfo(sf::RenderWindow& window) {
    if (currentState == GameState::PlacingTower) {
        window.draw(towerInfoBar);
        window.draw(_towerInfoText);
        window.draw(_towerCost);
        window.draw(_towerRange);
        window.draw(_towerSpeed);
        window.draw(_towerDamage);
    }
}

void Game::setGameState(GameState newState) { currentState = newState; }

void Game::handleMouseClick(sf::Vector2i clickPosition) {
    sf::Vector2f worldPos = window.mapPixelToCoords(clickPosition);
    sf::Vector2f snappedPos = arena->snapToGrid(worldPos);
    if (ResetButton.isMouseOver(window)) {
        resetGame();
    }
    if (SellTowerButton.isMouseOver(window) && showTowerMenu == true) {
        arena->deleteTower(towerRangeCircle.getPosition());
    }
    if (UpgradeTowerButton.isMouseOver(window) && showTowerMenu == true) {
        arena->upgradeTower(towerRangeCircle.getPosition());
    }
    
    switch (currentState) {
        case GameState::PlacingTower: {
            placeTower(currentlyPlacing, worldPos);
            break;
        }

        case GameState::Default: {
            // std::cout << "Executing handleMouseClick for state: Default" << std::endl;
            int gridX = getGridPosition(worldPos).x;
            int gridY = getGridPosition(worldPos).y;
            if (gridX >= 1 && gridX <= 26 && gridY >= 1 && gridY <= 2 * GRID_HEIGHT - 1) {
                if (arena->grid[gridX][gridY] == true) {
                    // std::cout << "There are " << arena->getTowers().size() << "worldPos.y" << std::endl;
                    for (const auto& uniquePtr : arena->getTowers()) {
                        Tower& tower = *uniquePtr;
                        if (abs(tower.getPosition().x - worldPos.x) <= GRID_CELL_SIZE && abs(tower.getPosition().y - worldPos.y) <= GRID_CELL_SIZE) {
                            // std::cout << "tower at Position " << tower.getPosition().x << " " << tower.getPosition().y << std::endl;
                            // std::cout << "Mouse at position " << worldPos.x << " " << worldPos.y << std::endl;

                            updateTowerMenuInfo(tower);
                            showTowerMenu = true;
                            // std::cout << "HERE " << std::endl;
                            return;
                        }
                    }
                }
            }
            showTowerMenu = false;
            break;
        }
        case GameState::TowerSelected: {
            // std::cout << "Executing handleMouseClick for state: TowerSelected" << std::endl;
            showTowerMenu = false;
            // Handle tower selected state mouse clicks
            break;
        }
        default: {
            // std::cout << "Mouse click received in an undefined game state." << std::endl;
            showTowerMenu = false;
            break;
        }
    }
}

void Game::handleMouseHover(sf::Vector2i hoverPosition) {
    sf::Vector2f worldPos = window.mapPixelToCoords(hoverPosition);
    sf::Vector2f snappedPos = arena->snapToGrid(worldPos);
    dot.setPosition(snappedPos);  // Set the position of the dot
    if (currentState == GameState::PlacingTower) {
        if (arena->isWithinBounds(snappedPos)) {
            if (!previewTower) {
                previewTower = std::make_unique<PelletTower>(snappedPos);
            } else {
                previewTower->setPosition(snappedPos);
            }

            // Check if the space is free and update the color accordingly
            if (arena->isSpaceFree(snappedPos) && arena->getGold() >= previewTower->cost) {
                showPreview = true;
                previewTower->setColor(sf::Color::Green);  // Default color
            } else {
                showPreview = true;
                previewTower->setColor(sf::Color::Red);  // Indicate the space is taken
            }
        } else {
            showPreview = false;
        }
    } else {
        showPreview = false;
    }
}

void Game::setCurrentlyPlacing(TowerType type) { currentlyPlacing = type; }

void Game::resetGame() {
    addCommand([this]() {
        std::unique_lock<std::mutex> lock(gameMutex);
        arena = std::make_unique<Arena>(ARENA_WIDTH, ARENA_HEIGHT, sf::Vector2f(LEFT_OFFSET, TOP_OFFSET));
        gameOver = false;
        currentState = GameState::Default;
        currentlyPlacing = TowerType::None;
        previewTower = nullptr;
        showPreview = false;
        showTowerMenu = false;
        gameStarted = false;
    });
}

sf::Vector2i Game::getGridPosition(const sf::Vector2f& position) const {
    // Assuming LEFT_OFFSET, TOP_OFFSET, and GRID_CELL_SIZE are accessible within Game
    int gridX = static_cast<int>((position.x - LEFT_OFFSET) / GRID_CELL_SIZE);
    int gridY = static_cast<int>((position.y - TOP_OFFSET) / GRID_CELL_SIZE);

    // Check if the calculated grid positions are within the bounds of the grid
    if (gridX >= 0 && gridX < GRID_WIDTH * 2 && gridY >= 0 && gridY < GRID_HEIGHT * 2) {
        return sf::Vector2i(gridX, gridY);
    } else {
        // Return an invalid grid position if the coordinates are out of the grid bounds
        return sf::Vector2i(-1, -1);
    }
}

void Game::placeTower(TowerType towerType, sf::Vector2f worldPos) {
    if (towerType == TowerType::None) {
        std::cerr << "ERROR: No tower type selected!\n";
        window.close();
        return;
    }

    std::unique_ptr<Tower> tower;

    switch (towerType) {
        case TowerType::Pellet:
            tower = std::make_unique<PelletTower>(worldPos);
            break;
        case TowerType::Bash:
            // Assuming BashTower is a valid class inheriting from Tower
            tower = std::make_unique<BashTower>(worldPos);
            break;
        case TowerType::Squirt:
            tower = std::make_unique<SquirtTower>(worldPos);
            break;
        case TowerType::Frost:
            tower = std::make_unique<FrostTower>(worldPos);
            break;
        case TowerType::Swarm:
            tower = std::make_unique<SwarmTower>(worldPos);
            break;
        case TowerType::Dart:
            // Assuming DartTower is a valid class inheriting from Tower
            tower = std::make_unique<DartTower>(worldPos);
            break;
        default:
            std::cerr << "ERROR: Unknown tower type!\n";
            break;
    }
    currentState = GameState::Default;

    int towerCost = tower->cost;
    if( tower ){
        if (towerCost > arena->getGold()) {
            // std::cout << "Not enough arena->getGold()! " << std::endl;
            currentState = GameState::Default;
            return;
        }
        if (arena->addTower(std::move(tower))) {
            currentState = GameState::Default;
        }
    }else{
        std::cerr << "Error: Failed to create tower!" << std::endl;
    }
    showTowerMenu = false;
    return;  // Break after handling the placement state
}

bool Game::getGameOver() const {
    std::lock_guard<std::mutex> lock(gameMutex);
    return gameOver;
}

void Game::placeTowerAtGrid(TowerType towerType, int gridX, int gridY) {
    addCommand([this, towerType, gridX, gridY]() {
        sf::Vector2f position = getGridCellPosition(gridX, gridY);
        placeTower(towerType, position);
    });
}

    void Game::sellTowerAtGrid(int gridX, int gridY) {
        addCommand([this, gridX, gridY]() {
            sf::Vector2f position = getGridCellPosition(gridX, gridY);
            std::unique_lock<std::mutex> lock(gameMutex);
            std::cout << "Selling tower at grid (" << gridX << ", " << gridY << ") which translates to position (" << position.x << ", " << position.y << ")\n";
            if (arena->deleteTower(position)) {
                std::cout << "Tower sold successfully.\n";
            } else {
                std::cout << "No tower found at the given position.\n";
            }
        });
    }

void Game::upgradeTowerAtGrid(int gridX, int gridY) {
    addCommand([this, gridX, gridY]() {
        sf::Vector2f position = getGridCellPosition(gridX, gridY);
        std::unique_lock<std::mutex> lock(gameMutex);
        std::cout << "Upgrading tower at grid (" << gridX << ", " << gridY << ") which translates to position (" << position.x << ", " << position.y << ")\n";
        if (arena->upgradeTower(position)) {
            std::cout << "Tower upgraded successfully.\n";
        } else {
            std::cout << "Failed to upgrade tower.\n";
        }
    });
}

void Game::startGame() {
    addCommand([this]() {
        std::unique_lock<std::mutex> lock(gameMutex);
        gameStarted = true;
    });
}

void Game::addCommand(std::function<void()> command) {
    commandQueue.push(command);
}

sf::Vector2f Game::getGridCellPosition(int gridX, int gridY) const {
    float xPos = LEFT_OFFSET + (gridX + 1) * GRID_CELL_SIZE;
    float yPos = TOP_OFFSET + (gridY + 1) * GRID_CELL_SIZE;
    return sf::Vector2f(xPos, yPos);
}
