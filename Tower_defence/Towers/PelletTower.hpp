#ifndef PELLET_TOWER_HPP
#define PELLET_TOWER_HPP

#include "Tower.hpp"
#include "JsonUtils.hpp"

class PelletTower : public Tower {
public:
    PelletTower(sf::Vector2f position );
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) override;
};

#endif // PELLET_TOWER_HPP
