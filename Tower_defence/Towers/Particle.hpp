#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>

class Particle {
public:
    Particle(sf::Vector2f position, sf::Vector2f velocity, float lifetime);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isAlive() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;
    float age;
};

#endif // PARTICLE_HPP
