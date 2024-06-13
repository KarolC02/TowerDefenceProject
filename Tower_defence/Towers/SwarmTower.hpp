#ifndef SWARMTOWER_HPP
#define SWARMTOWER_HPP

#include "Tower.hpp"
#include "JsonUtils.hpp"

class SwarmTower : public Tower {
public:
    SwarmTower(sf::Vector2f position);
    void fireBullet(const std::shared_ptr<Enemy> targetEnemy, float bulletSpeed) override;
    void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const sf::RenderWindow& window) override;
};

#endif // SWARMTOWER_HPP
