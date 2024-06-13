#ifndef FROST_BULLET_HPP
#define FROST_BULLET_HPP

#include "Bullet.hpp"
#include <SFML/Graphics.hpp>

class FrostBullet : public Bullet {
private:
    float slowFactor;  // Factor to slow down the enemies
    float areaEffectRadius;  // Area effect radius
    float slowDuration;  // Duration of the slow effect

public:
    FrostBullet(sf::Vector2f startPos, std::shared_ptr<Enemy> targetEnemy, float spd, int dmg, float slowFactor, float areaEffectRadius, float slowDuration);
    float getSlowFactor() const;
    float getAreaEffectRadius() const;
    float getSlowDuration() const;

    void update(float deltaTime, const std::vector<std::shared_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif // FROST_BULLET_HPP
