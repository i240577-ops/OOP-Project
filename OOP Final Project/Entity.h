#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class Entity {
protected:
    Vector2f position;
    int health;
    int maxHealth;

public:
    Entity();
    virtual ~Entity();

    virtual void update(float deltaTime) = 0;
    virtual void render(RenderWindow& window) = 0;

    virtual Vector2f getPosition() const = 0;
    virtual FloatRect getBounds() const = 0;
    virtual float getRadius() const = 0;

    virtual void takeDamage(int damage) = 0;
    int getHealth() const { return health; }
};