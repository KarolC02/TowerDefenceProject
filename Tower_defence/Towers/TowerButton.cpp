// Button.cpp
#include "TowerButton.hpp"
#include "ResourcePath.hpp"

static sf::Font towerFont;

// Constructor without font parameter
TowerButton::TowerButton() {
    // Empty implementation or default member initializations
}

TowerButton::TowerButton(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, sf::Color color, TowerType type) : _towerType(type) {
    if (!towerFont.loadFromFile(resourcePath() + "arial.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }
    shape.setSize(sf::Vector2f(TOWER_SIDE_LENGTH, TOWER_SIDE_LENGTH));
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(TOWER_SIDE_LENGTH / 2, TOWER_SIDE_LENGTH / 2);
    shape.setPosition(position);
    shape.setFillColor(color);
    
    shape.setOutlineThickness(-2.0f);  // Negative value for an inner border
    shape.setOutlineColor(sf::Color::Black);  // Black color for the border
    
    buttonText.setFont(towerFont);
    buttonText.setString(text);
    buttonText.setCharacterSize(TOWER_SIDE_LENGTH / 2);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
    buttonText.setPosition(position);
}


void TowerButton::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(buttonText);
}

bool TowerButton::isMouseOver(const sf::RenderWindow& window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

void TowerButton::setText(const std::string& text, const sf::Font& font) {
    buttonText.setFont(font);
    buttonText.setString(text);
    // Optionally, adjust the text position here if needed
}

void TowerButton::setPosition(const sf::Vector2f& position) {
    shape.setPosition(position);
    // Adjust the buttonText position relative to the new shape position
    buttonText.setPosition(position + sf::Vector2f(10, 10)); // Adjust for padding
}

sf::Vector2f TowerButton::getPosition() const {
    return shape.getPosition();
}


void TowerButton::setSize(const sf::Vector2f& size) {
    shape.setSize(size);
    // You might want to adjust buttonText size or position based on new button size
}

