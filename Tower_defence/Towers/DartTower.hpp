#ifndef DART_TOWER_HPP
#define DART_TOWER_HPP

#include "Tower.hpp"
#include "JsonUtils.hpp"

class DartTower : public Tower {
public:
    DartTower(sf::Vector2f position);
    void fireBullet(const Enemy* targetEnemy, float bulletSpeed) override;
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) override;
};

#endif // DART_TOWER_HPP
