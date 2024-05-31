#include "PelletTower.hpp"
#include "DEFINE.hpp"

PelletTower::PelletTower(sf::Vector2f position)
    : Tower(position, PELLET_TOWER_COLOR, 0.f , GRID_CELL_SIZE * 4, "P", 10, 5) {
    TowerParameters params = loadTowerParameters("PelletTower");
    attackSpeed = params.attackSpeed;
    range = params.range;
    damage = params.damage;
    cost = params.cost;
    upgrades = params.upgrades;
    info = "Pellet Tower";
}

void PelletTower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
    Tower::update(deltaTime, enemies, window);
}
