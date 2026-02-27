#include "Powerup.h"
#include "Constants.h"
#include <iostream>
using namespace std;
using namespace sf;
static Texture texDouble;
static Texture texHelper;
static Texture texGreater;
static Texture texShield;
static Texture texLife;
static bool powerupsLoaded = false;

Powerup::Powerup()
    : active(false),
    velocity(0.f, 150.f),
    type(POW_NONE)
{
    if (!powerupsLoaded) {
        texDouble.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\enemy3.png");
        texHelper.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\hpowerup.png");
        texGreater.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\spowerup.png");
        texShield.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\shpowerup.png");
        texLife.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\lpowerup.png");
        powerupsLoaded = true;
    }

    sprite.setScale(1.f, 1.f);

    health = 1;
    maxHealth = 1;
}

Powerup::~Powerup() = default;

void Powerup::spawn(Vector2f startPos, int pType) {
    position = startPos;
    type = pType;
    active = true;

    Texture* currentTex = nullptr;

    if (type == POW_DOUBLE) currentTex = &texDouble;
    else if (type == POW_HELPER) currentTex = &texHelper;
    else if (type == POW_GREATER) currentTex = &texGreater;
    else if (type == POW_SHIELD) currentTex = &texShield;
    else if (type == POW_LIFE) currentTex = &texLife;

    if (currentTex && currentTex->getSize().x > 0) {
        sprite.setTexture(*currentTex);

        float scale = POWERUP_SIZE / static_cast<float>(currentTex->getSize().x);
        sprite.setScale(scale, scale);

        FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

        sprite.setColor(Color::White);
    }
    else {
        sprite.setScale(1.f, 1.f);
    }

    sprite.setPosition(position);
}

void Powerup::update(float deltaTime) {
    if (!active) return;

    position += velocity * deltaTime;
    sprite.setPosition(position);

    if (position.y > WINDOW_HEIGHT + 20.f) {
        active = false;
    }
}

void Powerup::render(RenderWindow& window) {
    if (active) window.draw(sprite);
}

Vector2f Powerup::getPosition() const { return position; }
FloatRect Powerup::getBounds() const { return sprite.getGlobalBounds(); }
float Powerup::getRadius() const { return 15.f; }
void Powerup::takeDamage(int) {}