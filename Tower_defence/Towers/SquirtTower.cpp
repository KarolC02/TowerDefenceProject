#include "SquirtTower.hpp"
#include "DEFINE.hpp"

SquirtTower::SquirtTower(sf::Vector2f position)
    : Tower(position, SQUIRT_TOWER_COLOR, 0.f, 70, "S", 5, 15) {
    TowerParameters params = loadTowerParameters("SquirtTower");
    attackSpeed = params.attackSpeed;
    range = params.range;
    damage = params.damage;
    cost = params.cost;
    upgrades = params.upgrades;
    info = "Squirt Tower";
}

void SquirtTower::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
    Tower::update(deltaTime, enemies, window);
}
