#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
using namespace sf;

const int POW_NONE = 0;
const int POW_DOUBLE = 1;
const int POW_HELPER = 2;
const int POW_GREATER = 3;
const int POW_SHIELD = 4;
const int POW_LIFE = 5;

class Powerup : public Entity {
private:
    Sprite sprite;
    Vector2f velocity;
    bool active;
    int type;

public:
    Powerup();
    ~Powerup() override;
    void spawn(Vector2f startPos, int pType);
    void update(float deltaTime) override;
    void render(RenderWindow& window) override;
    Vector2f getPosition() const override;
    FloatRect getBounds() const override;
    float getRadius() const override;
    void takeDamage(int damage) override;
    bool isActive() const { return active; }
    void deactivate() { active = false; }
    int getType() const { return type; }
}; 
