#include "Button.hpp"
#include "ResourcePath.hpp"

sf::Font Button::buttonFont;

Button::Button() {
    if (buttonFont.getInfo().family.empty()) {
        if (!buttonFont.loadFromFile(resourcePath() + "arial.ttf")) {
            throw std::runtime_error("Failed to load font!");
        }
    }
}

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, sf::Color color, sf::Color textColor) {
    if (buttonFont.getInfo().family.empty()) {
        if (!buttonFont.loadFromFile(resourcePath() + "arial.ttf")) {
            throw std::runtime_error("Failed to load font!");
        }
    }
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(color);

    buttonText.setFont(buttonFont);
    buttonText.setString(text);
    buttonText.setCharacterSize(static_cast<unsigned int>(size.y * 0.5)); // Adjust size as needed
    buttonText.setFillColor(textColor);
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(buttonText);
}

bool Button::isMouseOver(const sf::RenderWindow& window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

void Button::setText(const std::string& text) {
    buttonText.setString(text);
    // Re-center text
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f, shape.getPosition().y + shape.getSize().y / 2.0f);
}

void Button::setPosition(const sf::Vector2f& position) {
    shape.setPosition(position);
    buttonText.setPosition(position.x + shape.getSize().x / 2.0f, position.y + shape.getSize().y / 2.0f);
}

void Button::setSize(const sf::Vector2f& size) {
    shape.setSize(size);
    buttonText.setCharacterSize(static_cast<unsigned int>(size.y * 0.5)); // Adjust the font size based on the button size
    setText(buttonText.getString()); // Update text to realign
}

void Button::setColor(sf::Color color) {
    shape.setFillColor(color);
}

void Button::setTextColor(sf::Color color) {
    buttonText.setFillColor(color);
}
