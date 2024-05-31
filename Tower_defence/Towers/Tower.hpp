#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "TowerUpgrade.h"

class Tower {
protected:
    sf::Text text;
    sf::RectangleShape shape; // Visual representation of the tower
    float attackSpeed;        // How often the tower can attack
    int damage;               // Damage dealt by the tower
    float attackCooldown;     // Cooldown timer for attacks
    int range;                // Attack range of the tower
    std::vector<std::unique_ptr<Bullet>> bullets; // Bullets fired by the tower
    std::vector<TowerUpgrade> upgrades;
    int currentLevel;         // Current upgrade level of the tower
    std::string info;
    float bulletSpeed;

public:
    int cost; // Cost of the tower
    Tower(sf::Vector2f position, sf::Color color, float atkSpeed, int rng, const std::string& typeSymbol, int dmg, int cst);
    
    virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, const sf::RenderWindow& window);
    virtual void fireBullet(const Enemy* targetEnemy, float bulletSpeed);
    
    const sf::RectangleShape& getShape() const { return shape; }
    virtual void draw(sf::RenderWindow& window);
    bool canAttack() const;
    void resetCooldown();
    void eraseOutOfScreenBullets(const sf::RenderWindow& window);
    bool isInRange(const sf::Vector2f& enemyPosition) const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    void setColor(sf::Color color);
    
    int getCost() const;
    int getDamage() const;
    int getSpeed() const;
    int getRange() const;
    std::string getInfo() const;
    const std::vector<std::unique_ptr<Bullet>>& getBullets() const;
    std::vector<std::unique_ptr<Bullet>>& getBullets();
    
    // Upgrade methods
    bool canUpgrade() const;
    void upgrade();
    std::string getNextUpgradeName() const;
    int getNextUpgradeCost() const;
    int getNextUpgradeDamage() const;
    int getNextUpgradeSpeed() const;
    int getNextUpgradeRange() const;
};
