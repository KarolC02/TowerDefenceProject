#ifndef FROST_TOWER_HPP
#define FROST_TOWER_HPP

#include "Tower.hpp"
#include "JsonUtils.hpp"

class FrostTower : public Tower {
public:
    FrostTower(sf::Vector2f position);
    void fireBullet(const Enemy* targetEnemy, float bulletSpeed) override;
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) override;
    
};

#endif // FROST_TOWER_HPP
