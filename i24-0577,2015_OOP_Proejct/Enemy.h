#pragma once
#include "Entity.h"
#include "Powerup.h" 
#include <SFML/Graphics.hpp>
using namespace sf;

class Ship;

class Enemy : public Entity {
protected:
    Sprite sprite;
    Vector2f velocity;
    bool active;
    float moveSpeed;
    bool movingRight;

    int carriedPowerup;
public:
    Enemy();
    ~Enemy() override;
    void spawnAt(Vector2f spawnPos);
    bool isActive() const { return active; }
    void deactivate() { active = false; }
    void setPowerup(int type);
    int getPowerup() const { return carriedPowerup; }
    bool hasPowerup() const { return carriedPowerup != POW_NONE; }

    // from Entity
    void update(float deltaTime) override;
    void render(RenderWindow& window) override;
    Vector2f getPosition() const override;
    FloatRect getBounds() const override;
    float getRadius() const override;
    void takeDamage(int damage) override;

    virtual void updateAI(float deltaTime, const Vector2f& playerPos);
    void collideWithPlayer(Ship& player);
    void setTexture(const Texture& tex);
};