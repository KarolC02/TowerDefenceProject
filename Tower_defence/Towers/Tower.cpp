#include "Tower.hpp"
#include "DEFINE.hpp"
#include "ResourcePath.hpp"
#include <cmath>
#include <stdexcept>
#include "JsonUtils.hpp"

Tower::Tower(sf::Vector2f position, sf::Color color, float atkSpeed, int rng, const std::string& typeSymbol, int dmg, int cst)
    : attackSpeed(atkSpeed), range(rng), damage(dmg), cost(cst), attackCooldown(0.0f), currentLevel(0) {

        
    if (!towerFont.loadFromFile(resourcePath() + "arial.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }


    bulletSpeed = getJsonValue(CONFIG_PATH, "BULLET_SPEED");

    // Setup visual appearance as before
    shape.setSize(sf::Vector2f(TOWER_SIDE_LENGTH, TOWER_SIDE_LENGTH));
    shape.setFillColor(color);
    shape.setOrigin(TOWER_SIDE_LENGTH / 2, TOWER_SIDE_LENGTH / 2);
    shape.setPosition(position);
    shape.setOutlineThickness(-2.0f);
    shape.setOutlineColor(sf::Color::Black);

    text.setFont(towerFont);
    text.setString(typeSymbol);
    text.setCharacterSize(TOWER_SIDE_LENGTH / 2);
    text.setFillColor(sf::Color::White);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
    text.setPosition(position);
}

// Implementation of new upgrade methods
bool Tower::canUpgrade() const {
    return currentLevel < upgrades.size();
}

void Tower::upgrade() {
    if (canUpgrade()) {
        info = upgrades[currentLevel].name;
        attackSpeed = upgrades[currentLevel].attackSpeed;
        range = upgrades[currentLevel].range;
        damage = upgrades[currentLevel].damage;
        cost += upgrades[currentLevel].cost;
        currentLevel++;
    }
}

std::string Tower::getNextUpgradeName() const {
    if (canUpgrade()) {
        return upgrades[currentLevel].name;
    }
    return "Max Level";
}

int Tower::getNextUpgradeCost() const {
    if (canUpgrade()) {
        return upgrades[currentLevel].cost;
    }
    return 0;
}

int Tower::getNextUpgradeDamage() const {
    if (canUpgrade()) {
        return upgrades[currentLevel].damage;
    }
    return 0;
}

int Tower::getNextUpgradeSpeed() const {
    if (canUpgrade()) {
        return static_cast<int>(upgrades[currentLevel].attackSpeed);
    }
    return 0;
}

int Tower::getNextUpgradeRange() const {
    if (canUpgrade()) {
        return upgrades[currentLevel].range;
    }
    return 0;
}

void Tower::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

void Tower::drawBullets(sf::RenderWindow& window){
    for (const auto& bullet : bullets) {
        if( bullet ){
            bullet->draw(window);
        }
    }
}

bool Tower::canAttack() const {
    return attackCooldown <= 0;
}

void Tower::resetCooldown() {
    attackCooldown = attackSpeed;
}

sf::Vector2f Tower::getPosition() const {
    return shape.getPosition();
}

void Tower::eraseOutOfScreenBullets(const sf::RenderWindow& window) {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [&window](const std::unique_ptr<Bullet>& bullet) {
                                     return bullet->shouldBeRemoved();
                                 }),
                  bullets.end());
}

void Tower::fireBullet(const Enemy* targetEnemy, float bulletSpeed) {
    if (canAttack()) {
        // std::cout << "Firing bullet at enemy: " << targetEnemy->getPosition().x << ", " << targetEnemy->getPosition().y << std::endl;
        bullets.emplace_back(std::make_unique<Bullet>(shape.getPosition(), targetEnemy, bulletSpeed, damage));
        resetCooldown();
    }
}

void Tower::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window) {
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

bool Tower::isInRange(const sf::Vector2f& enemyPosition) const {
    float dx = enemyPosition.x - shape.getPosition().x;
    float dy = enemyPosition.y - shape.getPosition().y;
    return std::sqrt(dx * dx + dy * dy) <= range;
}

void Tower::setPosition(sf::Vector2f pos) {
    shape.setPosition(pos.x, pos.y);
    text.setPosition(pos.x, pos.y);
}

void Tower::setColor(sf::Color color) {
    shape.setFillColor(color);
}

const std::vector<std::unique_ptr<Bullet>>& Tower::getBullets() const {
    return bullets;
}

// Non-const version: allows modification of the bullets vector
std::vector<std::unique_ptr<Bullet>>& Tower::getBullets() {
    return bullets;
}

int Tower::getCost() const {
    return cost;
}

int Tower::getDamage() const {
    return damage;
}

int Tower::getSpeed() const {
    return attackSpeed;
}

int Tower::getRange() const {
    return range;
}

std::string Tower::getInfo() const {
    return info;
}

void Tower::clearBullets() {
    bullets.clear();
}
