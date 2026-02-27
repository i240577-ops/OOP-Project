#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Ship.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "WaveManager.h"
#include "Powerup.h" 
using namespace sf;
using namespace std;

// GAME MODES
const int MODE_PLAYING = 0;
const int MODE_PAUSED = 1;
const int MODE_GAME_OVER = 2;
const int MODE_VICTORY = 3;

// LEVEL MESSAGES
const int MSG_NONE = 0;
const int MSG_START = 1;
const int MSG_COMPLETE = 2;
const int MSG_WAIT = 3;

class Game {
private:
    RenderWindow window;

    Texture bgTexture;
    Sprite bgSprite;

    Texture bgTextureNext;
    Sprite bgSpriteNext;
    float bgAlpha;
    bool isBgTransitioning;

    Ship playerShip;

    static const int MAX_BULLETS = 50;
    Projectile bullets[MAX_BULLETS];

    static const int MAX_POWERUPS = 5;
    Powerup powerups[MAX_POWERUPS];

    Asteroid asteroid;
    float asteroidSpawnTimer;

    WaveManager waveManager;

    Clock clock;

    bool isGameOver;
    int score;
    float totalGameTime;

    Font font;
    Text scoreText;
    Text gameOverText;
    Text healthText;

    int mode;
    int pauseSelection;
    Text pauseTitle;
    Text pauseOptions[3];

    int lastLevelShown;
    Text levelText;
    float levelMessageTimer;
    bool levelMessageActive;
    int levelMessageState;

    void loadLevelBackground(int level);

public:
    Game();
    void run();
    int getScore() const { return score; }

private:
    void processEvents();
    void update(float dt);
    void render();
    void handleShooting(float dt);
    void handleCollisions();

    void spawnPowerup(sf::Vector2f pos, int type);

    void updateUI();
    bool loadFont();

    void saveScore(int finalScore, const std::string& name);
    void askNameAndSave(int finalScore);
};