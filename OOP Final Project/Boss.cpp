#include "Boss.h"
#include "Constants.h"
#include "SoundManager.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;
using namespace sf;

static Texture bossTex;
static bool bossTexLoaded = false;

Boss::Boss()
    : state(B_HOVER), stateTimer(0.f),
    attackIntervalTimer(0.f), shootTimer(0.f), requestSummon(false)
{
    if (!bossTexLoaded) {
        if (!bossTex.loadFromFile(R"(C:\Users\HP Elite book\OneDrive\Desktop\SEMESTER 3\OOP\i24-0577, i24-2015_B_GALAXY_WARS\i24-0577, i24-2015_B_GALAXY_WARS\Sprites\boss.png)"))
            cerr << "Failed to load assets/boss.png\n";
        bossTexLoaded = true;
    }

    sprite.setTexture(bossTex);
    if (bossTex.getSize().x > 0) {
        float scale = BOSS_SIZE_X / static_cast<float>(bossTex.getSize().x);
        sprite.setScale(scale, scale);
    }
    else {
        sprite.setScale(1.f, 1.f);
    }

    FloatRect b = sprite.getLocalBounds();
    sprite.setOrigin(b.width / 2.f, b.height / 2.f);

    shadowShape.setRadius(40.f);
    shadowShape.setFillColor(Color(0, 0, 0, 100));
    shadowShape.setScale(1.f, 0.5f);
    shadowShape.setOrigin(40.f, 40.f);

    hpBarBack.setSize(Vector2f(BOSS_SIZE_X, 10.f));
    hpBarBack.setFillColor(Color::Red);
    hpBarBack.setOrigin(BOSS_SIZE_X / 2.f, 0.f);

    hpBarFront.setSize(Vector2f(BOSS_SIZE_X, 10.f));
    hpBarFront.setFillColor(Color::Green);
    hpBarFront.setOrigin(BOSS_SIZE_X / 2.f, 0.f);

    health = BOSS_MAX_HEALTH;
    maxHealth = BOSS_MAX_HEALTH;
    position = Vector2f(WINDOW_WIDTH / 2.f, -200.f);

    SoundBuffer* buf = SoundManager::getBuffer("boss_motion");
    if (buf) {
        motionSound.setBuffer(*buf);
        motionSound.setLoop(true);
        motionSound.setVolume(60.f);
    }
}

Boss::~Boss() {
    motionSound.stop();
}

void Boss::spawn() {
    position = Vector2f(WINDOW_WIDTH / 2.f, 150.f);
    sprite.setPosition(position);
    health = BOSS_MAX_HEALTH;
    state = B_HOVER;
    attackIntervalTimer = 15.0f;

    if (motionSound.getBuffer()) motionSound.play();
}

void Boss::update(float deltaTime) {
    update(deltaTime, Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT));
}

void Boss::update(float dt, const Vector2f& playerPos) {
    if (health <= 0) {
        if (motionSound.getStatus() == Sound::Playing) motionSound.stop();
        return;
    }

    switch (state) {
    case B_HOVER:
    {
        float speed = 100.f * dt;
        if (position.x < playerPos.x) position.x += speed;
        if (position.x > playerPos.x) position.x -= speed;
    }
    position.y = 150.f + sin(attackIntervalTimer * 2.f) * 10.f;
    attackIntervalTimer -= dt;

    if (attackIntervalTimer <= 3.0f && attackIntervalTimer > 0.f) {
        state = B_WARN_DROP;
        stateTimer = 3.0f;
        shadowShape.setPosition(position.x, WINDOW_HEIGHT - 50.f);
        SoundManager::playSound("boss_warn");
    }

    if (rand() % 1000 < 2) {
        state = B_WARN_SUMMON;
        stateTimer = 2.0f;
        SoundManager::playSound("boss_warn");
    }
    break;

    case B_WARN_DROP:
        stateTimer -= dt;
        {
            Vector2f sPos = shadowShape.getPosition();
            float speed = (playerPos.x - sPos.x) * 5.0f * dt;
            sPos.x += speed;
            shadowShape.setPosition(sPos);

            float bossSpeed = 150.f * dt;
            if (position.x < sPos.x) position.x += bossSpeed;
            if (position.x > sPos.x) position.x -= bossSpeed;
        }

        if (stateTimer <= 0.f) {
            state = B_DROP;
            SoundManager::playSound("boss_drop");
        }
        break;

    case B_DROP:
        position.y += 700.f * dt;
        if (position.y > WINDOW_HEIGHT + 200.f) {
            state = B_RISE;
            position.y = -200.f;
            position.x = WINDOW_WIDTH / 2.f;
        }
        break;

    case B_RISE:
        position.y += 300.f * dt;
        if (position.y >= 150.f) {
            position.y = 150.f;
            state = B_HOVER;
            attackIntervalTimer = 15.0f;
        }
        break;

    case B_WARN_SUMMON:
        stateTimer -= dt;
        if (stateTimer <= 0.f) {
            state = B_SUMMON;
            requestSummon = true;
            state = B_HOVER;
        }
        break;

    case B_SUMMON:
        state = B_HOVER;
        break;
    }

    sprite.setPosition(position);
    hpBarBack.setPosition(position.x, position.y - 80.f);
    hpBarFront.setPosition(position.x, position.y - 80.f);

    float hpPercent = (float)health / BOSS_MAX_HEALTH;
    if (hpPercent < 0) hpPercent = 0;

    hpBarFront.setSize(Vector2f(BOSS_SIZE_X * hpPercent, 10.f));
}

void Boss::render(RenderWindow& window) {
    if (health <= 0) return;
    if (state == B_WARN_DROP) {
        window.draw(shadowShape);
    }
    window.draw(sprite);
    window.draw(hpBarBack);
    window.draw(hpBarFront);
}

Vector2f Boss::getPosition() const { return position; }
FloatRect Boss::getBounds() const { return sprite.getGlobalBounds(); }
float Boss::getRadius() const { return BOSS_SIZE_X / 2.f; }
void Boss::takeDamage(int damage) { health -= damage; }

Vector2f Boss::getGunPosition() const {
    return Vector2f(position.x, position.y + 50.f);
}
