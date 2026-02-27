#include "FastEnemy.h"
#include "Constants.h"
#include <iostream>

using namespace std;
using namespace sf;

static Texture fastTexture;
static bool fastTextureLoaded = false;

FastEnemy::FastEnemy() {
    if (!fastTextureLoaded) {
        if (!fastTexture.loadFromFile(R"(C:\Users\HP Elite book\OneDrive\Desktop\SEMESTER 3\OOP\i24-0577, i24-2015_B_GALAXY_WARS\i24-0577, i24-2015_B_GALAXY_WARS\Sprites\enemy2.png)")) {
            cerr << "Failed to load assets/enemy_fast.png\n";
        }
        fastTextureLoaded = true;
    }

    setTexture(fastTexture);

    float scale = ENEMY_FAST_SIZE / static_cast<float>(fastTexture.getSize().x);
    sprite.setScale(scale, scale);

    moveSpeed = 300.f;
    health = 3;
    maxHealth = 3;
}

void FastEnemy::updateAI(float dt, const Vector2f& p) {
    Enemy::updateAI(dt, p);
}
