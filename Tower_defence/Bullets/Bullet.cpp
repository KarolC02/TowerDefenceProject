#include "Bullet.hpp"
#include <cmath> // For std::sqrt
#include <iostream>
#include "DEFINE.hpp"

Bullet::Bullet(const sf::Vector2f& startPosition, const Enemy* targetEnemy, float bulletSpeed, int damage)
    : targetEnemy(targetEnemy), speed(bulletSpeed), damage(damage) {
    shape.setRadius(5);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(startPosition);
    updateDirection();
}

Bullet::Bullet(const sf::Vector2f& startPosition, const Enemy* targetEnemy, float bulletSpeed, int damage, sf::Color color)
    : targetEnemy(targetEnemy), speed(bulletSpeed), damage(damage) {
    shape.setRadius(5);
    shape.setFillColor(color);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(startPosition);
    updateDirection();
}

void Bullet::updateDirection() {
    if (targetEnemy && !targetEnemy->getIsDead()) {
        sf::Vector2f targetPosition = targetEnemy->getPosition();
        direction = targetPosition - shape.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;
        }
    }
}

void Bullet::update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Find the target enemy by ID if the target is valid and not dead
    if (targetEnemy && !targetEnemy->getIsDead()) {
        updateDirection();
    }

    // Move in the last known direction
    sf::Vector2f movement = direction * speed * deltaTime;
    shape.move(movement);

    // Debug prints to track bullet behavior
    std::cout << "Bullet position: " << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
    std::cout << "Bullet direction: " << direction.x << ", " << direction.y << std::endl;
    std::cout << "Target position: " << (targetEnemy ? targetEnemy->getPosition().x : -1) << ", " << (targetEnemy ? targetEnemy->getPosition().y : -1) << std::endl;
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}

bool Bullet::shouldBeRemoved() const {
    sf::Vector2f position = shape.getPosition();
    return position.x < 0 || position.y < 0 || position.x > WINDOW_WIDTH || position.y > WINDOW_HEIGHT;
}
