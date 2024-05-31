#include "Enemy.hpp"
#include <iostream>
#include "DEFINE.hpp"
#include "JsonUtils.hpp"

Enemy::Enemy(sf::Vector2f startPosition, int initialHealth, int val, bool flying, bool spn, bool slw, bool fst)
    : position(startPosition), health(initialHealth), maxHealth(initialHealth), isDead(false), immune(true), gotPath(false), value(val), _originalSpeed(speed), flying(flying), isStunned(false), spawn(spn), slowImmune(slw), fast(fst) {
        
        
        // Access the ENEMY_SPEED from the Config singleton instance
    speed = getJsonValue(CONFIG_PATH, "ENEMY_SPEED");
    _originalSpeed = speed;

    if (fast) {
        speed *= 1.5;
        _originalSpeed = speed;
    }

    // Initialize the hitbox
    hitbox.setRadius(10);
    hitbox.setOrigin(hitbox.getRadius(), hitbox.getRadius());
    hitbox.setPosition(position);

    // Initialize the circle shape for non-flying enemies
    circleShape.setRadius(10);
    circleShape.setOrigin(circleShape.getRadius(), circleShape.getRadius());
    circleShape.setPosition(position);

    // Initialize the triangle shape for flying enemies
    triangleShape.setPointCount(3);  // Make it a triangle
    triangleShape.setPoint(0, sf::Vector2f(15, 0));  // Right point
    triangleShape.setPoint(1, sf::Vector2f(-15, 15));  // Bottom left point
    triangleShape.setPoint(2, sf::Vector2f(-15, -15));  // Top left point
    triangleShape.setOrigin(0, 0);
    triangleShape.setPosition(position);

    // Initialize health bar background
    healthBarBackground.setSize(sf::Vector2f(20, 5));
    healthBarBackground.setFillColor(sf::Color(50, 50, 50)); // Dark gray color
    healthBarBackground.setOrigin(healthBarBackground.getSize().x / 2, healthBarBackground.getSize().y / 2);

    // Initialize health bar foreground
    healthBarForeground.setSize(sf::Vector2f(20, 5));
    healthBarForeground.setFillColor(sf::Color::Green);
    healthBarForeground.setOrigin(healthBarForeground.getSize().x / 2, healthBarForeground.getSize().y / 2);

    updateAppearance(); // Set the initial appearance
}

void Enemy::update(float deltaTime) {
    if (isStunned && stunTimer.getElapsedTime().asSeconds() > STUN_TIME) {
        isStunned = false;
        speed = _originalSpeed;
    }
    if (position.x > LEFT_OFFSET) {
        immune = false;
    }

    if (!isStunned) {
        if (_slowClock.getElapsedTime().asSeconds() > 2.0f) {
            _slowedDown = false;
            speed = _originalSpeed;
        }

        if (flying) {
            position.x += speed * deltaTime;
        } else if (position.x >= LEFT_OFFSET + GRID_CELL_SIZE * 27.5 &&
                   position.y >= TOP_OFFSET + GRID_CELL_SIZE * 9.5 &&
                   position.y <= TOP_OFFSET + ARENA_HEIGHT - 9.5 * GRID_CELL_SIZE) {
            position.x += speed * deltaTime;
        } else if (!immune && gotPath && !path.empty()) {
            sf::Vector2f targetPosition = {
                static_cast<float>(path[nextCellIndex].first * GRID_CELL_SIZE + LEFT_OFFSET + GRID_CELL_SIZE / 2),
                static_cast<float>(path[nextCellIndex].second * GRID_CELL_SIZE + TOP_OFFSET + GRID_CELL_SIZE / 2)
            };

            moveTowards(targetPosition, deltaTime);

            if (std::hypot(position.x - targetPosition.x, position.y - targetPosition.y) < speed * deltaTime) {
                position = targetPosition;
                nextCellIndex++;

                if (nextCellIndex >= path.size()) {
                    gotPath = false;
                    nextCellIndex = 0;
                }
            }
        } else {
            position.x += speed * deltaTime;
        }
    }

    hitbox.setPosition(position); // Update the hitbox position
    if (flying) {
        triangleShape.setPosition(position);
    } else {
        circleShape.setPosition(position);
    }
    updateHealthBar();

    if (health <= 0) {
        isDead = true;
    }
}

void Enemy::stun(float duration) {
    isStunned = true;
    stunTimer.restart();
    speed = 0;
}

void Enemy::moveTowards(const sf::Vector2f& targetPosition, float deltaTime) {
    sf::Vector2f direction = targetPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction /= length; // Normalize
        position += direction * speed * deltaTime;
        hitbox.setPosition(position);
        if (flying) {
            triangleShape.setPosition(position);
        } else {
            circleShape.setPosition(position);
        }
    }
}

void Enemy::updateHealthBar() {
    if( !isDead ){
        healthBarBackground.setPosition(position.x, position.y - 20);
        healthBarForeground.setPosition(position.x, position.y - 20);
        float healthRatio = static_cast<float>(health) / maxHealth;
        healthBarForeground.setSize(sf::Vector2f(20 * healthRatio, 5));
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    if (flying) {
        window.draw(triangleShape);
    } else {
        window.draw(circleShape);
    }
    // window.draw(hitbox); // Draw the hitbox (optional, for debugging)
    window.draw(healthBarBackground);
    window.draw(healthBarForeground);
}

void Enemy::setHealth(int newHealth) {
    health = newHealth;
    updateHealthBar();
}

void Enemy::setIsDead(bool dead) {
    isDead = dead;
}

void Enemy::setPath(std::vector<std::pair<int, int>> path1) {
    path = path1;
    nextCellIndex = 1;
    gotPath = true;
}

void Enemy::damage(int hitPoints) {
    health -= hitPoints;
    updateHealthBar();
    if (health <= 0) {
        isDead = true;
        health = 0;
    }
}

sf::Vector2i Enemy::getGridPosition() const {
    if (immune) {
        return sf::Vector2i(-1, -1);
    } else {
        int gridX = static_cast<int>((position.x - LEFT_OFFSET) / GRID_CELL_SIZE);
        int gridY = static_cast<int>((position.y - TOP_OFFSET) / GRID_CELL_SIZE);
        return sf::Vector2i(gridX, gridY);
    }
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

bool Enemy::isImmune() const {
    return immune;
}

sf::Vector2f Enemy::snapToGrid(const sf::Vector2f& position) const {
    sf::Vector2f snappedPos;
    snappedPos.x = std::floor((position.x - LEFT_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + LEFT_OFFSET;
    snappedPos.y = std::floor((position.y - TOP_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + TOP_OFFSET;
    return snappedPos;
}

void Enemy::applySlow(float factor, float duration) {
    if (!slowImmune) {
        if (!isImmune() && !_slowedDown) {
            speed *= (1 - factor);
            _slowClock.restart();
            _slowedDown = true;
        }
    }
}

void Enemy::updateAppearance() {
    if (flying) {
        triangleShape.setFillColor(sf::Color::Yellow);  // Yellow for flying
    } else {
        circleShape.setRadius(10);
        circleShape.setOrigin(circleShape.getRadius(), circleShape.getRadius());
        if (slowImmune) {
            circleShape.setFillColor(sf::Color(255, 192, 203));  // Pink for immune
        } else if (spawn) {
            circleShape.setFillColor(sf::Color::Green);  // Blue for group
        } else if (fast) {
            circleShape.setFillColor(sf::Color(0, 100, 0));  // Dark Green for fast
        } else {
            circleShape.setFillColor(sf::Color(128, 128, 128));  // Gray by default
        }
    }
}

const sf::CircleShape& Enemy::getShape() const {
    return hitbox;
}

bool Enemy::isFlying() const{
    return flying;
}
