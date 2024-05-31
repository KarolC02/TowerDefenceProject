#include "Particle.hpp"

Particle::Particle(sf::Vector2f position, sf::Vector2f velocity, float lifetime)
    : velocity(velocity), lifetime(lifetime), age(0) {
    shape.setRadius(2);
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
}

void Particle::update(float deltaTime) {
    age += deltaTime;
    if (isAlive()) {
        shape.move(velocity * deltaTime);
        // Optionally fade out the particle
        float alpha = 255 * (1 - age / lifetime);
        shape.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
    }
}

void Particle::draw(sf::RenderWindow& window) const {
    if (isAlive()) {
        window.draw(shape);
    }
}

bool Particle::isAlive() const {
    return age < lifetime;
}
