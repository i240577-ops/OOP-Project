#include "Projectile.h"
#include "Constants.h"
#include <cmath>
#include <iostream>
using namespace std;
using namespace sf;
static Texture bulletTexture;
static Texture powerBulletTexture;
static bool texturesLoaded = false;

Projectile::Projectile()
    : velocity(0.f, 0.f),
    lifetime(0.f),
    active(false),
    isPower(false),
    isHostile(false)
{
    if (!texturesLoaded) {
        if (!bulletTexture.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\bullet.png")) {
            cerr << "Failed to load assets/bullet.png\n";
        }
        if (!powerBulletTexture.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\sbullet.png")) {
            cerr << "Failed to load assets/bullet_power.png\n";
        }
        texturesLoaded = true;
    }

    sprite.setTexture(bulletTexture);
    if (bulletTexture.getSize().x > 0) {
        float scale = BULLET_SIZE_X / static_cast<float>(bulletTexture.getSize().x);
        sprite.setScale(scale, scale);
    }
    else { sprite.setScale(1.f, 1.f); }

    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    position = Vector2f(0.f, 0.f);
    health = 1;
    maxHealth = 1;
}

Projectile::~Projectile() = default;

void Projectile::fireFrom(const Vector2f& startPos, float angleDegrees, float speed, bool powerState, bool hostileState) {
    position = startPos;
    isPower = powerState;
    isHostile = hostileState;

    float rad = angleDegrees * 3.14159265f / 180.f;
    velocity = Vector2f(std::sin(rad), -std::cos(rad)) * speed;

    lifetime = 4.0f;
    active = true;

    sprite.setPosition(position);
    sprite.setRotation(angleDegrees);

    if (isHostile) {

        sprite.setTexture(bulletTexture);
        sprite.setColor(sf::Color(255, 100, 0));

        if (bulletTexture.getSize().x > 0) {
            float scale = (BULLET_SIZE_X * 1.2f) / static_cast<float>(bulletTexture.getSize().x);
            sprite.setScale(scale, scale);
        }
    }
    else {

        setPower(isPower);
    }
}

void Projectile::setPower(bool isStrong) {
    if (isHostile) return;

    isPower = isStrong;
    if (isStrong) {
        sprite.setTexture(powerBulletTexture);
        sprite.setColor(sf::Color::White);
        if (powerBulletTexture.getSize().x > 0) {
            float scale = (BULLET_SIZE_X * 1.5f) / static_cast<float>(powerBulletTexture.getSize().x);
            sprite.setScale(scale, scale);
        }
    }
    else {
        sprite.setTexture(bulletTexture);
        sprite.setColor(Color::White);
        if (bulletTexture.getSize().x > 0) {
            float scale = BULLET_SIZE_X / static_cast<float>(bulletTexture.getSize().x);
            sprite.setScale(scale, scale);
        }
    }
    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Projectile::update(float deltaTime) {
    if (!active) return;

    lifetime -= deltaTime;
    if (lifetime <= 0.f) {
        active = false;
        return;
    }

    position += velocity * deltaTime;
    sprite.setPosition(position);

    if (position.x < -50 || position.x > WINDOW_WIDTH + 50 ||
        position.y < -50 || position.y > WINDOW_HEIGHT + 50) {
        active = false;
    }
}

void Projectile::render(RenderWindow& window) {
    if (active) window.draw(sprite);
}

Vector2f Projectile::getPosition() const { return position; }
FloatRect Projectile::getBounds() const { return sprite.getGlobalBounds(); }
float Projectile::getRadius() const { return 5.f; }
void Projectile::takeDamage(int) { active = false; }