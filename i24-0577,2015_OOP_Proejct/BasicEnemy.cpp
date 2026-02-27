#include "BasicEnemy.h"
#include "Constants.h"
#include <iostream>

using namespace std;
using namespace sf;

static Texture basicTexture;
static bool basicTextureLoaded = false;

BasicEnemy::BasicEnemy() {
    if (!basicTextureLoaded) {
        if (!basicTexture.loadFromFile(R"(C:\Users\HP Elite book\OneDrive\Desktop\SEMESTER 3\OOP\i24-0577, i24-2015_B_GALAXY_WARS\i24-0577, i24-2015_B_GALAXY_WARS\Sprites\enemy1.png)")) {
            cerr << "Failed to load assets/enemy_basic.png\n";
        }
        basicTextureLoaded = true;
    }

    setTexture(basicTexture);

    float scale = ENEMY_BASIC_SIZE / static_cast<float>(basicTexture.getSize().x);
    sprite.setScale(scale, scale);

    moveSpeed = 150.f;
    health = 2;
    maxHealth = 2;
}

void BasicEnemy::updateAI(float dt, const Vector2f& p) {
    Enemy::updateAI(dt, p);
}
