#include "Asteroid.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace sf;

static Texture asteroidTexture;
static bool asteroidTextureLoaded = false;

Asteroid::Asteroid()
    : velocity(0.f, 0.f),
    active(false),
    rotationSpeed(0.f)
{
    if (!asteroidTextureLoaded) {
        if (!asteroidTexture.loadFromFile(R"(C:\Users\HP Elite book\OneDrive\Desktop\SEMESTER 3\OOP\i24-0577, i24-2015_B_GALAXY_WARS\i24-0577, i24-2015_B_GALAXY_WARS\Sprites\asteroid.png)")) {
            cerr << "Failed to load assets/asteroid.png\n";
        }
        asteroidTextureLoaded = true;
    }

    sprite.setTexture(asteroidTexture);
    float scale = ASTEROID_SIZE / static_cast<float>(asteroidTexture.getSize().x);
    sprite.setScale(scale, scale);

    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    health = 3;
    maxHealth = 3;
}

Asteroid::~Asteroid() = default;

void Asteroid::spawnRandom() {
    float x = static_cast<float>(rand() % WINDOW_WIDTH);
    position = Vector2f(x, -50.f);

    velocity = Vector2f(0.f, 150.f);
    rotationSpeed = (float)(rand() % 100 - 50);

    active = true;
    health = maxHealth;

    sprite.setPosition(position);
    sprite.setRotation(0.f);
}

void Asteroid::update(float deltaTime) {
    if (!active) return;

    position += velocity * deltaTime;
    sprite.setPosition(position);
    sprite.rotate(rotationSpeed * deltaTime);

    if (position.y > WINDOW_HEIGHT + 50.f) {
        active = false;
    }
}

void Asteroid::render(RenderWindow& window) {
    if (active)
        window.draw(sprite);
}

Vector2f Asteroid::getPosition() const {
    return position;
}

FloatRect Asteroid::getBounds() const {
    return sprite.getGlobalBounds();
}

float Asteroid::getRadius() const {
    return sprite.getGlobalBounds().width / 2.f;
}

void Asteroid::takeDamage(int damage) {
    if (!active) return;
    health -= damage;
    if (health <= 0) {
        active = false;
    }
}
