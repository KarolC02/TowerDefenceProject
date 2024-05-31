#ifndef SQUIRT_TOWER_HPP
#define SQUIRT_TOWER_HPP

#include "Tower.hpp"
#include "JsonUtils.hpp"

class SquirtTower : public Tower {
public:
    SquirtTower(sf::Vector2f position);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) override;
};

#endif // SQUIRT_TOWER_HPP
