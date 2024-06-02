#include "SwarmTower.hpp"
#include "DEFINE.hpp"
#include <algorithm> // For std::min

SwarmTower::SwarmTower(sf::Vector2f position)
    : Tower(position, SWARM_TOWER_COLOR, 0.f , GRID_CELL_SIZE * 4, "SW", 20, 50) {
    TowerParameters params = loadTowerParameters("SwarmTower");
    attackSpeed = params.attackSpeed;
    range = params.range;
    damage = params.damage;
    cost = params.cost;
    upgrades = params.upgrades;
    info = "Swarm Tower 1";
}

void SwarmTower::fireBullet(const std::shared_ptr<Enemy> targetEnemy, float bulletSpeed) {
    bullets.emplace_back(std::make_unique<Bullet>(shape.getPosition(), targetEnemy, bulletSpeed, damage));
}

void SwarmTower::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
    }

    if (canAttack()) {
        std::vector<std::shared_ptr<Enemy>> targets;
        sf::Vector2f towerPos = getPosition();
        float attackRange = getRange();

        for (auto& enemy : enemies) {
            if (!enemy->isFlying()) {
                continue; // Skip non-flying enemies
            }

            sf::Vector2f enemyPos = enemy->getPosition();
            float distance = std::hypot(towerPos.x - enemyPos.x, towerPos.y - enemyPos.y);

            if (distance <= attackRange) {
                targets.push_back(enemy);
                if (targets.size() == 4) {
                    break; // Attack up to four enemies
                }
            }
        }

        if (!targets.empty()) {
            // std::cout << "FIRING " << targets.size() << " Bullets!" << std::endl;
            for (auto& target : targets) {
                fireBullet(target, bulletSpeed);  // Pass shared_ptr directly
            }
            resetCooldown(); // Reset cooldown after firing bullets
        }
    }

    // Update bullets and remove out-of-screen bullets
    auto bulletIter = bullets.begin();
    while (bulletIter != bullets.end()) {
        (*bulletIter)->update(deltaTime, enemies); // Update the bullet's position
        ++bulletIter;
    }

    eraseOutOfScreenBullets(window);
}
