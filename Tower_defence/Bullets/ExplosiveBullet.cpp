#include "ExplosiveBullet.hpp"

ExplosiveBullet::ExplosiveBullet(sf::Vector2f startPos, std::shared_ptr<Enemy> targetEnemy, float spd, int dmg, float explosionRadius)
    : Bullet(startPos, targetEnemy, spd, dmg, sf::Color::Red), explosionRadius(explosionRadius) {
}

float ExplosiveBullet::getExplosionRadius() const {
    return explosionRadius;
}

void ExplosiveBullet::update(float deltaTime, const std::vector<std::shared_ptr<Enemy>>& enemies) {
    Bullet::update(deltaTime, enemies);
}

void ExplosiveBullet::draw(sf::RenderWindow& window) const {
    Bullet::draw(window);
}
