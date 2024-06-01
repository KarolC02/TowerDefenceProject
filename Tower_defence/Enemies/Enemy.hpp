#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>

class Enemy {
protected:
    
    float _stunTime;
    
    sf::CircleShape hitbox; // Hitbox calculation
    sf::CircleShape circleShape; // Circle shape
    sf::ConvexShape triangleShape; // Triangle shape
    
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;
    sf::Vector2f position;
    float speed;
    float _originalSpeed;
    int health;
    int maxHealth;
    bool isDead;
    int value;
    void updateHealthBar(); // Made private to enforce encapsulation
    sf::Vector2f snapToGrid(const sf::Vector2f& position) const;
    
    size_t nextCellIndex;
    void moveTowards(const sf::Vector2f& targetPosition, float deltaTime);
    
    bool _slowedDown;
    bool isStunned;
    sf::Clock _slowClock;
    sf::Clock stunTimer;
    void updateAppearance();
public:
    
    bool isFlying() const;
    bool spawn;
    bool slowImmune;
    bool flying;
    bool fast;
    std::vector<std::pair<int, int>> path;
    sf::Vector2i getGridPosition() const;
    void NewPath();
    bool immune;
    Enemy(sf::Vector2f startPosition, int initialHealth, int value, bool flying, bool spn, bool slw, bool fst);
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window) const;
    int getMaxHealth() {return maxHealth;}
    // Getters for private members
    const sf::CircleShape& getShape() const;
    int getHealth() const { return health; }
    bool getIsDead() const { return isDead; }
    int getValue() const {return value; }
    // Setters if needed
    bool gotPath;
    std::vector<std::pair<int, int>> getPath() const { return path; }
    void setPath( std::vector<std::pair<int, int>> path1);
    void setHealth(int newHealth);
    void setIsDead(bool dead);
    bool isImmune() const;
    sf::Vector2f getPosition() const;
    void damage(int hitPoints);
    void applySlow(float factor, float duration);
    void stun(float duration); // Add method to stun the enemy

};

#endif // ENEMY_HPP
