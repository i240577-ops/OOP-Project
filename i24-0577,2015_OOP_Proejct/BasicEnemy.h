#pragma once
#include "Enemy.h"
using namespace sf;

class BasicEnemy : public Enemy {
public:
    BasicEnemy();

    void updateAI(float deltaTime,
        const Vector2f& playerPos) override;
};
