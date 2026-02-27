#pragma once
#include "Enemy.h"

class FastEnemy : public Enemy {
public:
    FastEnemy();

    void updateAI(float deltaTime,
        const sf::Vector2f& playerPos) override;
};
