#ifndef BASHTOWER_HPP
#define BASHTOWER_HPP

#include "Tower.hpp"
#include <random>
#include "Enemy.hpp"
#include "Particle.hpp"

class BashTower : public Tower {
public:
    BashTower(sf::Vector2f position);
    void fireBullet(const Enemy* targetEnemy, float bulletSpeed) override;
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window)  override;
    void draw(sf::RenderWindow& window) override;
    float getChanceToStun() const;

private:
    std::vector<Particle> particles;
    void createParticles(sf::Vector2f position);
    mutable sf::CircleShape stompEffect;
    float stompEffectAlpha;
    bool showStompEffect;
};

#endif // BASHTOWER_HPP
