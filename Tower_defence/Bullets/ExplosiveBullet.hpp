#ifndef EXPLOSIVE_BULLET_HPP
#define EXPLOSIVE_BULLET_HPP

#include "Bullet.hpp"
#include <SFML/Graphics.hpp>

class ExplosiveBullet : public Bullet {
private:
    float explosionRadius;  // Radius of the explosion

public:
    ExplosiveBullet(sf::Vector2f startPos, const Enemy* targetEnemy, float spd, int dmg, float explosionRadius);
    float getExplosionRadius() const;

    void update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif // EXPLOSIVE_BULLET_HPP
