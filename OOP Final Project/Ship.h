#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
using namespace sf;
using namespace std;
class Ship {
private:
    Sprite sprite;
    Sprite helperSprite;
    CircleShape shieldShape;

    Vector2f velocity;
    Vector2f moveDir;

    float maxSpeed;

    int lives;
    int maxLives;

    bool isInvincible;
    float invincibilityTimer;
    float flickerTimer;

    bool doubleShotActive;
    float doubleShotTimer;

    bool helperActive;
    float helperTimer;
    Vector2f helperPos;
    float helperShootTimer;

    bool greaterPowerActive;
    float greaterPowerTimer;

    int shieldStrength;
    float shieldTimer;

    Clock shootClock;
    float shootCooldown;

public:
    Ship();

    void update(float dt, float targetX);
    void render(RenderWindow& window);
    void setMoveDirection(const Vector2f& dir);

    bool canShoot() const;
    void shoot();

    bool tryHelperAutoShoot();

    void loseLife();
    void gainLife();
    void setLives(int amount);

    void activateDoubleShot();
    void activateHelper();
    void activateGreaterPower();

    void addShield(int amount);
    void setShield(int amount);
    int getShieldStrength() const { return shieldStrength; }

    void setPosition(float x, float y);
    Vector2f getPosition() const;
    Vector2f getHelperPosition() const;

    FloatRect getBounds() const;

    int getLives() const { return lives; }
    int getMaxLives() const { return maxLives; }

    bool isDoubleShot() const { return doubleShotActive; }
    bool isHelperActive() const { return helperActive; }
    bool isGreaterPower() const { return greaterPowerActive; }
    bool isShieldActive() const { return shieldStrength > 0; }
    bool isInvulnerable() const { return isInvincible; }

    float getHealth() const { return static_cast<float>(lives); }
    float getMaxHealth() const { return static_cast<float>(maxLives); }
    void takeDamage(float amount);

    void accelerate(float) {}
    void decelerate(float) {}
    void rotateLeft(float) {}
    void rotateRight(float) {}

    // SECTION 7: Friend function declaration
    // Allows this standalone function to access private members of Ship
    friend void logShipStatus(const Ship& s);
};
