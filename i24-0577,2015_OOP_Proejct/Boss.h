#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

const int B_HOVER = 0;
const int B_WARN_DROP = 1;
const int B_DROP = 2;
const int B_RISE = 3;
const int B_WARN_SUMMON = 4;
const int B_SUMMON = 5;
class Boss : public Entity {
private:
    Sprite sprite;
    CircleShape shadowShape;

    RectangleShape hpBarBack;
    RectangleShape hpBarFront;

    int state;
    float stateTimer;
    float attackIntervalTimer;
    float shootTimer;
    Vector2f velocity;
    bool requestSummon;
    Sound motionSound;
public:
    Boss();
    ~Boss() override;
    void spawn();
    void update(float deltaTime) override;
    void update(float deltaTime, const Vector2f& playerPos);
    void render(RenderWindow& window) override;
    Vector2f getPosition() const override;
    FloatRect getBounds() const override;
    float getRadius() const override;
    void takeDamage(int damage) override;
    bool isActive() const { return health > 0; }
    bool wantsToSummon() {
        if (requestSummon) { requestSummon = false; return true; }
        return false;
    }
    Vector2f getGunPosition() const;
};
