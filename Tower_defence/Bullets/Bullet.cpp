#include "Bullet.hpp"
#include <cmath> // For std::sqrt
#include <iostream>
#include "DEFINE.hpp"

Bullet::Bullet(const sf::Vector2f& startPosition, std::shared_ptr<Enemy> targetEnemy, float bulletSpeed, int damage)
    : targetEnemy(targetEnemy), speed(bulletSpeed), damage(damage) {
    shape.setRadius(5);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(startPosition);
    updateDirection();
}

Bullet::Bullet(const sf::Vector2f& startPosition, std::shared_ptr<Enemy> targetEnemy, float bulletSpeed, int damage, sf::Color color)
    : targetEnemy(targetEnemy), speed(bulletSpeed), damage(damage) {
    shape.setRadius(5);
    shape.setFillColor(color);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(startPosition);
    updateDirection();
}

void Bullet::updateDirection() {
    if (targetEnemy && !targetEnemy->getIsDead()) {
        std::cout<< "UPDATING DIRECTION"<< std::endl;
        std::cout<< targetEnemy->getIsDead() << std::endl;
        sf::Vector2f targetPosition = targetEnemy->getPosition();
        sf::Vector2f directionTemp = targetPosition - shape.getPosition();
        if( directionTemp.x != 0 && directionTemp.y != 0 ){
            direction = directionTemp;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length != 0) {
                direction /= length;
            }
        }
    }
}

void Bullet::update(float deltaTime, const std::vector<std::shared_ptr<Enemy>>& enemies) {
    if (targetEnemy && !targetEnemy->getIsDead()) {
        updateDirection();
    }

    sf::Vector2f movement = direction * speed * deltaTime;
    shape.move(movement);

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

void Bullet::clearTarget() {
    targetEnemy.reset();
}

bool Bullet::hasTarget(const std::shared_ptr<Enemy>& target) const {
    return targetEnemy == target;
}
