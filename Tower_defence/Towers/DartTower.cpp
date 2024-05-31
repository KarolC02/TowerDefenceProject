#include "DartTower.hpp"
#include "DEFINE.hpp"
#include "ExplosiveBullet.hpp"
#include <iostream>

DartTower::DartTower(sf::Vector2f position)
    : Tower(position, DART_TOWER_COLOR, 0.f, 6 * GRID_CELL_SIZE, "D", 8, 20) {
    TowerParameters params = loadTowerParameters("DartTower");
    attackSpeed = params.attackSpeed;
    range = params.range;
    damage = params.damage;
    cost = params.cost;
    upgrades = params.upgrades;
    info = "Dart Tower";
}

void DartTower::fireBullet(const Enemy* targetEnemy, float bulletSpeed) {
    if (canAttack()) {
        float explosionRadius = 50.0f; // Define an appropriate explosion radius
        bullets.emplace_back(std::make_unique<ExplosiveBullet>(shape.getPosition(), targetEnemy, bulletSpeed, damage, explosionRadius));
        resetCooldown();
    }
}

void DartTower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
    // Cooldown timer decrement
    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
    }

    // Fire bullets at enemies in range
    for (auto& enemy : enemies) {
        if (isInRange(enemy->getPosition()) && canAttack()) {
            fireBullet(enemy.get(), bulletSpeed);
            break; // Stop checking after finding the first enemy in range
        }
    }

    // Update bullets and remove out-of-screen bullets
    auto bulletIter = bullets.begin();
    while (bulletIter != bullets.end()) {
        (*bulletIter)->update(deltaTime, enemies); // Update the bullet's position

        // Check if the bullet should be removed
        if ((*bulletIter)->shouldBeRemoved()) {
            bulletIter = bullets.erase(bulletIter);
        } else {
            ++bulletIter;
        }
    }
}
