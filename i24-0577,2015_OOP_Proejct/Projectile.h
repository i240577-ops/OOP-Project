#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
using namespace sf;
class Projectile : public Entity {
private:
    Sprite sprite;
    Vector2f velocity;
    float lifetime;
    bool active;

    bool isPower;
    bool isHostile;

public:
    Projectile();
    ~Projectile() override;

    void fireFrom(const Vector2f& startPos, float angleDegrees, float speed, bool isPower, bool isHostile);

    void setPower(bool isStrong);

    void update(float deltaTime) override;
    void render(RenderWindow& window) override;

    Vector2f getPosition() const override;
    FloatRect getBounds() const override;
    float getRadius() const override;
    void takeDamage(int damage) override;

    bool isActive() const { return active; }
    void deactivate() { active = false; }

    bool isPowerBullet() const { return isPower; }
    bool isHostileBullet() const { return isHostile; }
};