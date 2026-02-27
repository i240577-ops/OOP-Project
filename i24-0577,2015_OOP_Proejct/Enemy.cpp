#include "Enemy.h"
#include "Ship.h"
#include <iostream>
#include "Constants.h"

using namespace sf;

Enemy::Enemy()
    : velocity(0.f, 0.f),
    active(false),
    moveSpeed(100.f),
    movingRight(true),
    carriedPowerup(POW_NONE)
{
    health = 2;
    maxHealth = 2;
}

Enemy::~Enemy() = default;

void Enemy::setTexture(const Texture& tex) {
    sprite.setTexture(tex);
    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Enemy::spawnAt(Vector2f spawnPos) {
    position = spawnPos;
    active = true;
    health = maxHealth;
    movingRight = true;

    carriedPowerup = POW_NONE;
    sprite.setColor(Color::White);

    sprite.setPosition(position);
}

void Enemy::setPowerup(int type) {
    carriedPowerup = type;
}

void Enemy::updateAI(float, const Vector2f&) {
    velocity.y = 0.f;
    if (movingRight) velocity.x = moveSpeed;
    else velocity.x = -moveSpeed;

    const float padding = 30.f;
    const float dropAmount = 100.f;

    if (movingRight && position.x > WINDOW_WIDTH - padding) {
        movingRight = false;
        position.x = WINDOW_WIDTH - padding;
        position.y += dropAmount;
    }
    else if (!movingRight && position.x < padding) {
        movingRight = true;
        position.x = padding;
        position.y += dropAmount;
    }
}

void Enemy::update(float deltaTime) {
    if (!active) return;

    position += velocity * deltaTime;
    sprite.setPosition(position);

    if (carriedPowerup == POW_DOUBLE) {
        sprite.setColor(Color(100, 255, 100)); // Green tint
    }
    else if (carriedPowerup == POW_HELPER) {
        sprite.setColor(Color(100, 255, 255)); // Cyan tint
    }
    else if (carriedPowerup == POW_GREATER) {
        sprite.setColor(Color(255, 100, 255)); // Magenta tint
    }
    else {
        sprite.setColor(Color::White); // Normal
    }

    if (position.y > WINDOW_HEIGHT + 50.f) active = false;
    if (health <= 0) active = false;
}

void Enemy::render(RenderWindow& window) {
    if (active) window.draw(sprite);
}

Vector2f Enemy::getPosition() const { return position; }
FloatRect Enemy::getBounds() const { return sprite.getGlobalBounds(); }
float Enemy::getRadius() const { return 20.f; }

void Enemy::takeDamage(int damage) {
    if (!active) return;
    health -= damage;
    if (health <= 0) active = false;
}

void Enemy::collideWithPlayer(Ship& player) {
    if (!active) return;
    active = false;
    player.loseLife();
}
