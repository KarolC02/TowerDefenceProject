#include "FrostBullet.hpp"

// Constructor with initialization list corrected
FrostBullet::FrostBullet(sf::Vector2f startPos, const Enemy* targetEnemy, float spd, int dmg, float slowFactor, float areaEffectRadius, float slowDuration)
    : Bullet(startPos, targetEnemy, spd, dmg, sf::Color::Blue), slowFactor(slowFactor), areaEffectRadius(areaEffectRadius), slowDuration(slowDuration) {
}

float FrostBullet::getSlowFactor() const {
    return slowFactor;
}

float FrostBullet::getAreaEffectRadius() const {
    return areaEffectRadius;
}

float FrostBullet::getSlowDuration() const {
    return slowDuration;
}

void FrostBullet::update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Custom update logic if necessary
    Bullet::update(deltaTime, enemies);
}

void FrostBullet::draw(sf::RenderWindow& window) const {
    // Custom draw logic if necessary
    Bullet::draw(window);
}
