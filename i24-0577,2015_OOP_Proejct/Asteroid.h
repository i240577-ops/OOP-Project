#pragma once

#include "Entity.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>
using namespace sf;

class Asteroid : public Entity {
private:
    Sprite sprite;
    Vector2f velocity;
    bool active;
    float rotationSpeed;
public:
    Asteroid();
    ~Asteroid() override;

    void spawnRandom();
    bool isActive() const { return active; }
    void deactivate() { active = false; }

    void update(float deltaTime) override;
    void render(RenderWindow& window) override;

    Vector2f getPosition() const override;
    FloatRect getBounds() const override;
    float getRadius() const override;
    void takeDamage(int damage) override;
};