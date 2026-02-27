#include "Ship.h"
#include "Constants.h"
#include "Utils.h"
#include <cmath>
#include <iostream>
using namespace std;
using namespace sf;
static Texture playerTex;
static Texture helperTex;
static bool shipTexturesLoaded = false;

void logShipStatus(const Ship& s) {
    cout << "[DEBUG] Ship Status - Lives: " << s.lives
        << " | Shield: " << s.shieldStrength
        << " | DoubleShot: " << (s.doubleShotActive ? "ON" : "OFF") << "\n";
}

Ship::Ship()
{
    if (!shipTexturesLoaded) {
        if (!playerTex.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\player.png")) cerr << "Failed to load assets/player.png\n";
        if (!helperTex.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\helper.png")) cerr << "Failed to load assets/helper.png\n";
        shipTexturesLoaded = true;
    }

    sprite.setTexture(playerTex);
    if (playerTex.getSize().x > 0) {
        float pScale = SHIP_SIZE_X / static_cast<float>(playerTex.getSize().x);
        sprite.setScale(pScale, pScale);
    }
    else { sprite.setScale(1.f, 1.f); }

    FloatRect pb = sprite.getLocalBounds();
    sprite.setOrigin(pb.width / 2.f, pb.height / 2.f);
    sprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    helperSprite.setTexture(helperTex);
    if (helperTex.getSize().x > 0) {
        float hScale = HELPER_SIZE_X / static_cast<float>(helperTex.getSize().x);
        helperSprite.setScale(hScale, hScale);
    }
    else { helperSprite.setScale(1.f, 1.f); }

    FloatRect hb = helperSprite.getLocalBounds();
    helperSprite.setOrigin(hb.width / 2.f, hb.height / 2.f);
    helperPos = Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT + 100.f);

    float shieldRadius = (SHIP_SIZE_X / 2.f) + 10.f;
    shieldShape.setRadius(shieldRadius);
    shieldShape.setFillColor(Color::Transparent);
    shieldShape.setOutlineColor(Color::Blue);
    shieldShape.setOutlineThickness(3.f);
    shieldShape.setOrigin(shieldRadius, shieldRadius);

    velocity = Vector2f(0.f, 0.f);
    moveDir = Vector2f(0.f, 0.f);

    maxSpeed = 300.f;
    maxLives = 10;
    lives = 3;

    isInvincible = false;
    invincibilityTimer = 0.f;
    flickerTimer = 0.f;

    doubleShotActive = false;
    doubleShotTimer = 0.f;

    helperActive = false;
    helperTimer = 0.f;
    helperShootTimer = 0.f;

    greaterPowerActive = false;
    greaterPowerTimer = 0.f;

    shieldStrength = 0;
    shieldTimer = 0.f;

    shootCooldown = 0.25f;
}

void Ship::update(float dt, float targetX)
{
    velocity.x = moveDir.x * maxSpeed;
    velocity.y = moveDir.y * maxSpeed;
    sprite.move(velocity * dt);

    Vector2f pos = sprite.getPosition();

    pos.x = clamp<float>(pos.x, 0.f, static_cast<float>(WINDOW_WIDTH));
    pos.y = clamp<float>(pos.y, 0.f, static_cast<float>(WINDOW_HEIGHT));

    sprite.setPosition(pos);

    if (helperActive) {
        helperTimer -= dt;
        if (helperTimer <= 0.f) helperActive = false;
        helperShootTimer -= dt;

        float hoverY = WINDOW_HEIGHT - 150.f;
        float destX = (targetX < 0) ? pos.x + 50.f : targetX;
        if (targetX < 0) hoverY = pos.y;

        float speed = 200.f * dt;
        if (helperPos.x < destX) helperPos.x += speed;
        if (helperPos.x > destX) helperPos.x -= speed;
        if (helperPos.y < hoverY) helperPos.y += speed;
        if (helperPos.y > hoverY) helperPos.y -= speed;

        helperSprite.setPosition(helperPos);
    }

    if (shieldStrength > 0) {
        shieldShape.setPosition(pos);
        shieldTimer -= dt;
        if (shieldTimer < 3.0f && fmod(shieldTimer, 0.2f) < 0.1f) {
            shieldShape.setOutlineColor(Color(0, 0, 255, 100));
        }
        else {
            shieldShape.setOutlineColor(Color::Blue);
        }

        if (shieldTimer <= 0.f) {
            shieldStrength = 0;
        }
    }

    if (isInvincible) {
        invincibilityTimer -= dt;
        flickerTimer += dt;
        if (fmod(flickerTimer, 0.2f) < 0.1f) sprite.setColor(Color(255, 255, 255, 128));
        else sprite.setColor(Color::White);

        if (invincibilityTimer <= 0.f) {
            isInvincible = false;
            sprite.setColor(Color::White);
        }
    }

    if (doubleShotActive) {
        doubleShotTimer -= dt;
        if (doubleShotTimer <= 0.f) doubleShotActive = false;
    }

    if (greaterPowerActive) {
        greaterPowerTimer -= dt;
        if (greaterPowerTimer <= 0.f) greaterPowerActive = false;
    }
}

bool Ship::tryHelperAutoShoot() {
    if (helperActive && helperShootTimer <= 0.f) {
        helperShootTimer = 0.5f;
        return true;
    }
    return false;
}

void Ship::render(RenderWindow& window)
{
    window.draw(sprite);
    if (helperActive) window.draw(helperSprite);
    if (shieldStrength > 0) window.draw(shieldShape);
}

void Ship::setMoveDirection(const Vector2f& dir) { moveDir = dir; }
bool Ship::canShoot() const { return shootClock.getElapsedTime().asSeconds() >= shootCooldown; }
void Ship::shoot() { shootClock.restart(); }

void Ship::loseLife() {
    if (isInvincible) return;
    if (shieldStrength > 0) {
        shieldStrength--;
        isInvincible = true;
        invincibilityTimer = 1.0f;
        flickerTimer = 0.f;
        return;
    }
    lives--;
    if (lives < 0) lives = 0;
    isInvincible = true;
    invincibilityTimer = 3.0f;
    flickerTimer = 0.f;
}

void Ship::gainLife() {
    lives++;
    if (lives > maxLives) lives = maxLives;
}

void Ship::setLives(int amount) {
    lives = amount;
    if (lives > maxLives) lives = maxLives;
}

void Ship::activateDoubleShot() {
    doubleShotActive = true;
    doubleShotTimer = 10.0f;
}

void Ship::activateHelper() {
    helperActive = true;
    helperTimer = 15.0f;
    helperPos = sprite.getPosition();
    helperPos.x += 50.f;
}

void Ship::activateGreaterPower() {
    greaterPowerActive = true;
    greaterPowerTimer = 10.0f;
}

void Ship::addShield(int amount) {
    shieldStrength += amount;
    shieldTimer = 10.0f;
}

void Ship::setShield(int amount) {
    shieldStrength = amount;
    shieldTimer = 10.0f;
}

Vector2f Ship::getHelperPosition() const {
    return helperSprite.getPosition();
}

void Ship::takeDamage(float) { loseLife(); }
void Ship::setPosition(float x, float y) { sprite.setPosition(x, y); }
Vector2f Ship::getPosition() const { return sprite.getPosition(); }
FloatRect Ship::getBounds() const { return sprite.getGlobalBounds(); }