#pragma once
#include "Enemy.h"
using namespace sf;

class PatternEnemy : public Enemy {
private:
    float waveTimer;

public:
    PatternEnemy();

    void updateAI(float deltaTime, const Vector2f& playerPos) override;
};