#include "FrostTower.hpp"
#include "DEFINE.hpp"
#include "FrostBullet.hpp"

FrostTower::FrostTower(sf::Vector2f position)
    : Tower(position, FROST_TOWER_COLOR, 0.f, 3 * GRID_CELL_SIZE + 1 / 3 * GRID_CELL_SIZE, "F", 10, 50) {
    TowerParameters params = loadTowerParameters("FrostTower");
    attackSpeed = params.attackSpeed;
    range = params.range;
    damage = params.damage;
    cost = params.cost;
    upgrades = params.upgrades;
    info = "Frost Tower";
}

void FrostTower::fireBullet(const Enemy* targetEnemy, float bulletSpeed) {
    if (canAttack()) {
        float slowFactor = getJsonValue(CONFIG_PATH, "SLOW_FACTOR" + std::to_string(currentLevel));
        float areaEffectRadius = 50.f;
        bullets.emplace_back(std::make_unique<FrostBullet>(shape.getPosition(), targetEnemy, bulletSpeed, damage, slowFactor, areaEffectRadius, 2.0f));
        resetCooldown();
    }
}

void FrostTower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
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
