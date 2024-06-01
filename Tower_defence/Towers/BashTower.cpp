#include "BashTower.hpp"
#include "DEFINE.hpp"
#include <iostream>
#include <algorithm> // For std::remove_if
#include "JsonUtils.hpp"

BashTower::BashTower(sf::Vector2f position)
    : Tower(position, BASH_TOWER_COLOR, 0.f , 3 * GRID_CELL_SIZE, "B", 10, 30) {
    TowerParameters params = loadTowerParameters("BashTower");
    attackSpeed = params.attackSpeed;
    range = params.range;
    damage = params.damage;
    cost = params.cost;
    upgrades = params.upgrades;
    // TODO stunDurations = params.stunDurations; // Assuming stunDurations is part of TowerParameters
    info = "Bash Tower";

    // Initialize stomp effect
    stompEffect.setRadius(getRange());
    stompEffect.setFillColor(sf::Color(0, 0, 0, 100)); // Black with some transparency
    stompEffect.setOrigin(stompEffect.getRadius(), stompEffect.getRadius());
    stompEffectAlpha = 100;
    showStompEffect = false;
}

void BashTower::fireBullet(const Enemy* targetEnemy, float bulletSpeed) {
    std::cout << "BashTower firing bullet" << std::endl;
}

void BashTower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
    static std::random_device rd;  // Random device to seed the generator
    static std::mt19937 gen(rd()); // Mersenne Twister random number generator
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f); // Uniform distribution [0, 1]

    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
    }

    if (canAttack()) {
        sf::Vector2f towerPos = getPosition();
        float attackRange = getRange();
        float stunChance = getChanceToStun();
        std::cout << "THE CHANCE TO STUN IS "<< stunChance << std::endl;
        float randomValue = dis(gen);
        bool doWeStun = (randomValue <= stunChance);

        bool attacked = false;
        for (auto& enemy : enemies) {
            sf::Vector2f enemyPos = enemy->getPosition();
            float distance = std::hypot(towerPos.x - enemyPos.x, towerPos.y - enemyPos.y);

            if (distance <= attackRange) {
                enemy->damage(getDamage());
                attacked = true;
                // Check if the enemy should be stunned
                if (doWeStun) {
                    float stunDuration = getJsonValue(CONFIG_PATH, "STUN_DURATION" + std::to_string(currentLevel));
                    std::cout << "We are stunning for "<<  stunDuration << std::endl;
                    enemy->stun(stunDuration);  // Stun the enemy
                }
            }
        }

        if (attacked) {
            resetCooldown();  // Reset cooldown after attack
            showStompEffect = true;
            stompEffect.setPosition(towerPos);
            stompEffectAlpha = 100;

            if (getJsonValue(CONFIG_PATH, "PARTICLES_ON")) {
                // Create particles at the tower's position and move them immediately
                createParticles(towerPos);
            }
        }
    }

    // Update stomp effect visibility
    if (showStompEffect) {
        stompEffectAlpha -= 100.0f * deltaTime; // Fade out effect
        if (stompEffectAlpha <= 0) {
            stompEffectAlpha = 0;
            showStompEffect = false;
        }
    }

    if (getJsonValue(CONFIG_PATH, "PARTICLES_ON")) {
        // Update particles
        for (auto& particle : particles) {
            particle.update(deltaTime);
        }

        // Remove dead particles
        particles.erase(std::remove_if(particles.begin(), particles.end(),
                        [](const Particle& p) { return !p.isAlive(); }), particles.end());
    }
}

void BashTower::draw(sf::RenderWindow& window) {
    // Draw the tower
    window.draw(shape);
    window.draw(text);
    for (const auto& bullet : bullets) {
        bullet->draw(window);
    }

    // Draw the stomp effect
    if (showStompEffect) {
        stompEffect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(stompEffectAlpha)));
        window.draw(stompEffect);
    }

    if (getJsonValue(CONFIG_PATH, "PARTICLES_ON")){
        // Draw particles
        for (const auto& particle : particles) {
            particle.draw(window);
        }
    }
}

float BashTower::getChanceToStun() const {
    std::cout << "The chance to stun is "<< getJsonValue(CONFIG_PATH, "STUN_CHANCE" + std::to_string(currentLevel)) << std::endl;
    return (getJsonValue(CONFIG_PATH, "STUN_CHANCE" + std::to_string(currentLevel)));
}

void BashTower::createParticles(sf::Vector2f position) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    for (int i = 0; i < 50; ++i) {
        sf::Vector2f velocity(range / attackCooldown * dis(gen) * 3, range / attackCooldown * dis(gen) * 3); // Random velocity
        particles.emplace_back(position, velocity, attackCooldown / 4);
    }
}
