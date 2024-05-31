#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button {
public:
    Button();
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, sf::Color color, sf::Color textColor = sf::Color::White);

    void draw(sf::RenderWindow& window) const;
    bool isMouseOver(const sf::RenderWindow& window) const;

    void setText(const std::string& text);
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void setColor(sf::Color color);
    void setTextColor(sf::Color color);

private:
    sf::RectangleShape shape;
    sf::Text buttonText;
    static sf::Font buttonFont;
};

#endif // BUTTON_HPP
