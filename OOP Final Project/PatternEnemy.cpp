#include "PatternEnemy.h"
#include "Constants.h"
#include <cmath>
#include <iostream>
using namespace std;
using namespace sf;
static Texture patternTexture;
static bool patternTextureLoaded = false;

PatternEnemy::PatternEnemy()
    : waveTimer(0.f)
{
    if (!patternTextureLoaded) {
        if (!patternTexture.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\enemy3.png")) {
            cerr << "Failed to load assets/enemy_pattern.png\n";
        }
        patternTextureLoaded = true;
    }
    setTexture(patternTexture);
    float scale = ENEMY_PATTERN_SIZE / static_cast<float>(patternTexture.getSize().x);
    sprite.setScale(scale, scale);

    moveSpeed = 180.f;
    health = 4;
    maxHealth = 4;
}

void PatternEnemy::updateAI(float dt, const sf::Vector2f& p) {
    Enemy::updateAI(dt, p);

    waveTimer += dt;
    float amplitude = 150.f;
    float frequency = 4.0f;
    velocity.y += sin(waveTimer * frequency) * amplitude;
}