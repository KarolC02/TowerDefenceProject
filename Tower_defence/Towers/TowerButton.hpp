#pragma once
#include <SFML/Graphics.hpp>
#include "DEFINE.HPP"

class TowerButton {
public:
    TowerButton();
    TowerButton(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, sf::Color color, TowerType type); // Removed the font parameter

    void draw(sf::RenderWindow& window) const;
    bool isMouseOver(const sf::RenderWindow& window) const;
    void setText(const std::string& text, const sf::Font& font); // Added font as a parameter for setText
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    sf::Vector2f getPosition() const;
private:
    sf::RectangleShape shape;
    sf::Text buttonText;
    TowerType _towerType;
};
