#include "ExplosiveBullet.hpp"

// Constructor with initialization list corrected
ExplosiveBullet::ExplosiveBullet(sf::Vector2f startPos, const Enemy* targetEnemy, float spd, int dmg, float explosionRadius)
    : Bullet(startPos, targetEnemy, spd, dmg, sf::Color::Red), explosionRadius(explosionRadius) {
}

float ExplosiveBullet::getExplosionRadius() const {
    return explosionRadius;
}

void ExplosiveBullet::update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Custom update logic if necessary
    Bullet::update(deltaTime, enemies);
}

void ExplosiveBullet::draw(sf::RenderWindow& window) const {
    // Custom draw logic if necessary
    Bullet::draw(window);
}
