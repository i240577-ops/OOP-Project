#include "Game.h"
#include "Resource.h"
#include "SoundManager.h"
#include "GameException.h" 
#include <iostream>
#include <fstream>
#include <ctime>  
#include <cstdlib>
#include <string>
using namespace std;
using namespace sf;
static float bossShootTimer = 0.f;

// --- STRING HELPER FUNCTIONS ---
void copyString(char* dest, const char* src, int& pos) {
    int i = 0;
    while (src[i] != '\0') { dest[pos++] = src[i++]; }
}
void appendInt(char* dest, int val, int& pos) {
    if (val == 0) { dest[pos++] = '0'; return; }
    char temp[16]; int t = 0; int v = val;
    while (v > 0) { temp[t++] = (v % 10) + '0'; v /= 10; }
    while (t > 0) { dest[pos++] = temp[--t]; }
}
// --------------------------------------------------

void Game::loadLevelBackground(int level) {
    string path;
    string musicPath;

    switch (level) {
    case 1: path = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\level1.png"; musicPath = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\level1.ogg"; break;
    case 2: path = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\level2.png"; musicPath = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\level2.ogg"; break;
    case 3: path = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\level3.png"; musicPath = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\level3.ogg"; break;
    case 4: path = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\bosslevel.png"; musicPath = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\boss_theme.ogg"; break;
    default: path = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\level1.png"; musicPath = "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\level1.ogg"; break;
    }

    SoundManager::playMusic(musicPath);

    bool firstLoad = (bgTexture.getSize().x == 0);
    if (firstLoad) {
        if (!bgTexture.loadFromFile(path)) {
            if (!bgTexture.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\Background.png")) {}
        }
        bgSprite.setTexture(bgTexture);
        if (bgTexture.getSize().x > 0) {
            float sX = (float)WINDOW_WIDTH / bgTexture.getSize().x;
            float sY = (float)WINDOW_HEIGHT / bgTexture.getSize().y;
            bgSprite.setScale(sX, sY);
        }
    }
    else {
        if (!bgTextureNext.loadFromFile(path)) { bgTextureNext.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\Background.png"); }
        bgSpriteNext.setTexture(bgTextureNext);
        if (bgTextureNext.getSize().x > 0) {
            float sX = (float)WINDOW_WIDTH / bgTextureNext.getSize().x;
            float sY = (float)WINDOW_HEIGHT / bgTextureNext.getSize().y;
            bgSpriteNext.setScale(sX, sY);
        }
        bgAlpha = 0.f;
        isBgTransitioning = true;
        bgSpriteNext.setColor(Color(255, 255, 255, 0));
    }
}

Game::Game()
    : window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Galaxy Wars"),
    playerShip(),
    isGameOver(false),
    score(0),
    mode(MODE_PLAYING),
    pauseSelection(0),
    asteroidSpawnTimer(0.f),
    bgAlpha(0.f),
    isBgTransitioning(false),
    totalGameTime(0.f)
{
    srand(static_cast<unsigned int>(time(NULL)));
    window.setFramerateLimit(static_cast<unsigned int>(FPS));

    loadLevelBackground(1);

    playerShip.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    asteroid.spawnRandom();

    if (!loadFont()) {
        throw GameException("CRITICAL ERROR: Failed to load font (BRLNSDB.TTF)!");
    }

    scoreText.setFont(font); scoreText.setCharacterSize(24); scoreText.setFillColor(Color::White); scoreText.setPosition(10.f, 10.f);
    healthText.setFont(font); healthText.setCharacterSize(24); healthText.setFillColor(Color::White); healthText.setPosition(10.f, 40.f);

    // Initial Game Over setup
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setString("GAME OVER");
    FloatRect tb = gameOverText.getLocalBounds();
    gameOverText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    gameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 80.f);

    pauseTitle.setFont(font); pauseTitle.setCharacterSize(40); pauseTitle.setFillColor(Color::Yellow);
    pauseTitle.setString("Paused");
    FloatRect pb = pauseTitle.getLocalBounds();
    pauseTitle.setOrigin(pb.left + pb.width / 2.f, pb.top + pb.height / 2.f);
    pauseTitle.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 80.f);

    const char* labels[3] = { "Resume", "Restart", "Quit to Menu" };
    for (int i = 0; i < 3; ++i) {
        pauseOptions[i].setFont(font); pauseOptions[i].setCharacterSize(30);
        pauseOptions[i].setString(labels[i]);
        pauseOptions[i].setFillColor(i == 0 ? Color::Cyan : Color::White);
        pauseOptions[i].setPosition(WINDOW_WIDTH / 2.f - 80.f, WINDOW_HEIGHT / 2.f - 10.f + i * 40.f);
    }

    lastLevelShown = waveManager.getCurrentLevel();
    levelText.setFont(font); levelText.setCharacterSize(36); levelText.setFillColor(Color::Yellow); levelText.setStyle(Text::Bold);
    char buf[50]; int p = 0; copyString(buf, "LEVEL 0", p); appendInt(buf, lastLevelShown, p); buf[p] = '\0';
    levelText.setString(buf);
    FloatRect lb = levelText.getLocalBounds();
    levelText.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    levelText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    levelMessageTimer = 0.5f;
    levelMessageActive = true;
    levelMessageState = MSG_START;
    waveManager.setSpawningEnabled(false);
}

bool Game::loadFont() {
    // Priority: Demi-Bold -> Regular -> Bold
    const char* fontPaths[] = { "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Fonts\\BRLNSDB.TTF", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Fonts\\BRLNSR.TTF", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Fonts\\BRLNSB.TTF" };
    for (int i = 0; i < 3; ++i) {
        if (font.loadFromFile(fontPaths[i])) return true;
    }
    return false;
}

void Game::saveScore(int finalScore, const string& name) {
    try {
        ofstream outFile("C:/Users/HP Elite book/OneDrive/Desktop/SEMESTER 3/OOP/i24-0577, i24-2015_B_GALAXY_WARS/i24-0577, i24-2015_B_GALAXY_WARS/scores.txt", ios::app);
        if (outFile.is_open()) {
            outFile << name << " " << finalScore << " " << static_cast<int>(totalGameTime) << "\n";
            outFile.close();
        }
    }
    catch (...) {}
}

void Game::askNameAndSave(int finalScore) {
    string name;

    // Update Game Over Text to include Score
    string msg = (mode == MODE_VICTORY) ? "VICTORY!" : "GAME OVER";
    msg += "\nFinal Score: " + to_string(finalScore);

    gameOverText.setString(msg);
    FloatRect tb = gameOverText.getLocalBounds();
    gameOverText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    gameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 100.f);

    Text prompt, nameTx;
    prompt.setFont(font); prompt.setString("Enter Name:"); prompt.setPosition(WINDOW_WIDTH / 2.f - 60.f, WINDOW_HEIGHT / 2.f);
    prompt.setFillColor(Color::White); prompt.setCharacterSize(28);
    nameTx.setFont(font); nameTx.setPosition(WINDOW_WIDTH / 2.f - 60.f, WINDOW_HEIGHT / 2.f + 40.f);
    nameTx.setFillColor(Color::Cyan); nameTx.setCharacterSize(28);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            else if (event.type == Event::TextEntered) {
                if (event.text.unicode == 8 && !name.empty()) name.pop_back();
                else if (event.text.unicode >= 32 && event.text.unicode < 127) name.push_back(static_cast<char>(event.text.unicode));
            }
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                if (!name.empty()) saveScore(finalScore, name);
                window.close();
            }
        }
        nameTx.setString(name);
        window.clear(Color::Black);
        window.draw(gameOverText);
        window.draw(prompt);
        window.draw(nameTx);
        window.display();
    }
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        processEvents();
        update(dt);
        render();
    }
    if (isGameOver || mode == MODE_VICTORY) {
        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
            mode == MODE_VICTORY ? "VICTORY!" : "Game Over");
        askNameAndSave(score);
    }
}

void Game::processEvents() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) window.close();
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
            mode = (mode == MODE_PLAYING) ? MODE_PAUSED : MODE_PLAYING;
        }
        if (event.type == Event::KeyPressed && mode == MODE_PAUSED) {
            if (event.key.code == Keyboard::Up) pauseSelection = (pauseSelection + 2) % 3;
            else if (event.key.code == Keyboard::Down) pauseSelection = (pauseSelection + 1) % 3;
            else if (event.key.code == Keyboard::Enter) {
                if (pauseSelection == 0) mode = MODE_PLAYING;
                else window.close();
            }
            for (int i = 0; i < 3; ++i) pauseOptions[i].setFillColor(i == pauseSelection ? Color::Cyan : Color::White);
        }
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::D) {
            logShipStatus(playerShip);
        }
    }
}

void Game::update(float dt) {
    if (mode == MODE_PAUSED) return;
    if (isGameOver || mode == MODE_GAME_OVER || mode == MODE_VICTORY) return;

    totalGameTime += dt;

    if (isBgTransitioning) {
        bgAlpha += 150.f * dt;
        if (bgAlpha >= 255.f) {
            bgAlpha = 255.f;
            isBgTransitioning = false;
            bgTexture = bgTextureNext;
            bgSprite.setTexture(bgTexture);
            if (bgTexture.getSize().x > 0) {
                float sX = (float)WINDOW_WIDTH / bgTexture.getSize().x;
                float sY = (float)WINDOW_HEIGHT / bgTexture.getSize().y;
                bgSprite.setScale(sX, sY);
            }
        }
        bgSpriteNext.setColor(Color(255, 255, 255, static_cast<Uint8>(bgAlpha)));
    }

    Vector2f moveDir(0.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Left)) moveDir.x = -1.f;
    else if (Keyboard::isKeyPressed(Keyboard::Right)) moveDir.x = 1.f;
    if (Keyboard::isKeyPressed(Keyboard::Up)) moveDir.y = -1.f;
    else if (Keyboard::isKeyPressed(Keyboard::Down)) moveDir.y = 1.f;
    playerShip.setMoveDirection(moveDir);

    float targetX = waveManager.getLowestEnemyX();
    playerShip.update(dt, targetX);

    if (playerShip.tryHelperAutoShoot()) {
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (!bullets[i].isActive()) {
                bullets[i].fireFrom(playerShip.getHelperPosition(), 0.f, 600.0f, false, false);
                SoundManager::playSound("fire_helper");
                break;
            }
        }
    }

    handleShooting(dt);

    Boss& boss = waveManager.getBoss();
    if (boss.isActive()) {
        bossShootTimer -= dt;
        if (bossShootTimer <= 0.f) {
            bossShootTimer = 2.5f;
            float angles[] = { 180.f, 200.f, 160.f };
            int fired = 0;
            for (int i = 0; i < MAX_BULLETS && fired < 3; ++i) {
                if (!bullets[i].isActive()) {
                    bullets[i].fireFrom(boss.getGunPosition(), angles[fired], 400.0f, false, true);
                    fired++;
                }
            }
            if (fired > 0) SoundManager::playSound("fire_simple");
        }
    }

    for (int i = 0; i < MAX_BULLETS; ++i) bullets[i].update(dt);
    for (int i = 0; i < MAX_POWERUPS; ++i) powerups[i].update(dt);

    if (asteroid.isActive()) {
        asteroid.update(dt);
    }
    else {
        asteroidSpawnTimer += dt;
        float randomDelay = 3.0f + (static_cast<float>(rand() % 300) / 100.f);
        if (asteroidSpawnTimer >= randomDelay) {
            asteroid.spawnRandom(); asteroidSpawnTimer = 0.f;
        }
    }

    waveManager.update(dt, playerShip.getPosition());

    int currentLevel = waveManager.getCurrentLevel();
    if (currentLevel != lastLevelShown && levelMessageState == MSG_NONE) {
        SoundManager::playSound("level_complete");

        char buf[50]; int p = 0; copyString(buf, "LEVEL 0", p); appendInt(buf, lastLevelShown, p); copyString(buf, " COMPLETED!", p); buf[p] = '\0';
        levelText.setString(buf);

        FloatRect lb = levelText.getLocalBounds();
        levelText.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        levelText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
        levelMessageTimer = 0.5f;
        levelMessageActive = true;
        levelMessageState = MSG_COMPLETE;
        waveManager.setSpawningEnabled(false);
        lastLevelShown = currentLevel;
    }

    if (levelMessageActive) {
        levelMessageTimer -= dt;
        if (levelMessageTimer <= 0.f) {
            levelMessageActive = false;
            if (levelMessageState == MSG_START) {
                levelMessageState = MSG_WAIT;
                levelMessageTimer = 0.1f;
            }
            else if (levelMessageState == MSG_COMPLETE) {
                levelMessageState = MSG_WAIT;
                levelMessageTimer = 0.2f;
            }
        }
    }
    else if (levelMessageState == MSG_WAIT) {
        levelMessageTimer -= dt;
        if (levelMessageTimer <= 0.f) {
            if (!levelMessageActive) {
                static bool pendingStartText = false;
                if (!pendingStartText) {

                    loadLevelBackground(lastLevelShown);

                    char buf[50]; int p = 0;
                    if (lastLevelShown == 4) copyString(buf, "BOSS LEVEL", p);
                    else { copyString(buf, "LEVEL 0", p); appendInt(buf, lastLevelShown, p); }
                    buf[p] = '\0';

                    levelText.setString(buf);
                    FloatRect lb2 = levelText.getLocalBounds();
                    levelText.setOrigin(lb2.left + lb2.width / 2.f, lb2.top + lb2.height / 2.f);
                    levelText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
                    levelMessageTimer = 0.5f;
                    levelMessageActive = true;
                    levelMessageState = MSG_START;
                    pendingStartText = true;
                }
                else {
                    waveManager.setSpawningEnabled(true);
                    levelMessageState = MSG_NONE;
                    pendingStartText = false;
                }
            }
        }
    }

    handleCollisions();
    updateUI();

    if (playerShip.getLives() <= 0 && !isGameOver) {
        isGameOver = true;
        mode = MODE_GAME_OVER;
        SoundManager::stopMusic();
        SoundManager::playSound("game_over");
        window.close();
    }
}

void Game::updateUI() {
    char sBuf[50]; int sp = 0; copyString(sBuf, "Score: ", sp); appendInt(sBuf, score, sp); sBuf[sp] = '\0';
    scoreText.setString(sBuf);

    char hBuf[150]; int hp = 0; copyString(hBuf, "Lives: ", hp); appendInt(hBuf, playerShip.getLives(), hp);
    if (playerShip.isInvulnerable()) copyString(hBuf, " (INV)", hp);
    if (playerShip.isDoubleShot()) copyString(hBuf, " (2X)", hp);
    if (playerShip.isHelperActive()) copyString(hBuf, " (+SHIP)", hp);
    if (playerShip.isGreaterPower()) copyString(hBuf, " (POWER)", hp);
    if (playerShip.isShieldActive()) {
        copyString(hBuf, " [SHIELD ", hp); appendInt(hBuf, playerShip.getShieldStrength(), hp); copyString(hBuf, "]", hp);
    }
    hBuf[hp] = '\0';
    healthText.setString(hBuf);
}

void Game::render() {
    window.clear(Color::Black);
    window.draw(bgSprite);
    if (isBgTransitioning) window.draw(bgSpriteNext);

    playerShip.render(window);
    for (int i = 0; i < MAX_BULLETS; ++i) bullets[i].render(window);
    for (int i = 0; i < MAX_POWERUPS; ++i) powerups[i].render(window);
    asteroid.render(window);
    waveManager.render(window);
    window.draw(scoreText);
    window.draw(healthText);

    if (mode == MODE_PAUSED) {
        RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(Color(0, 0, 0, 150));
        window.draw(overlay); window.draw(pauseTitle);
        for (int i = 0; i < 3; ++i) window.draw(pauseOptions[i]);
    }

    if (isGameOver || mode == MODE_GAME_OVER) {
        window.draw(gameOverText);
    }
    else if (mode == MODE_VICTORY) {
        gameOverText.setString("CONGRATULATIONS!\nYOU WIN!");
        gameOverText.setFillColor(Color::Green);
        FloatRect vb = gameOverText.getLocalBounds();
        gameOverText.setOrigin(vb.left + vb.width / 2.f, vb.top + vb.height / 2.f);
        gameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
        window.draw(gameOverText);
    }

    if (levelMessageActive) window.draw(levelText);
    window.display();
}

void Game::spawnPowerup(Vector2f pos, int type) {
    if (type == POW_NONE) return;
    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (!powerups[i].isActive()) {
            powerups[i].spawn(pos, type);
            SoundManager::playSound("powerup_spawn");
            break;
        }
    }
}

void Game::handleShooting(float) {
    if (Keyboard::isKeyPressed(Keyboard::Space) && playerShip.canShoot()) {
        int bulletsToFire = 0;
        Vector2f spawnPos[3];
        bool highPower = playerShip.isGreaterPower();

        if (playerShip.isDoubleShot()) {
            spawnPos[bulletsToFire++] = playerShip.getPosition() + Vector2f(-20.f, 0.f);
            spawnPos[bulletsToFire++] = playerShip.getPosition() + Vector2f(20.f, 0.f);
        }
        else {
            spawnPos[bulletsToFire++] = playerShip.getPosition();
        }

        if (playerShip.isHelperActive()) {
            spawnPos[bulletsToFire++] = playerShip.getHelperPosition();
        }

        int spawned = 0;
        for (int i = 0; i < MAX_BULLETS && spawned < bulletsToFire; ++i) {
            if (!bullets[i].isActive()) {
                bullets[i].fireFrom(spawnPos[spawned], 0.f, 600.0f, highPower, false);
                spawned++;
            }
        }
        playerShip.shoot();

        if (highPower) SoundManager::playSound("fire_strong");
        else SoundManager::playSound("fire_simple");
    }
}

void Game::handleCollisions() {
    int damageAmount = 1;
    if (playerShip.isGreaterPower()) damageAmount = 2;

    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!bullets[i].isActive()) continue;

        if (bullets[i].isHostileBullet()) {
            if (!playerShip.isInvulnerable() && playerShip.getBounds().intersects(bullets[i].getBounds())) {
                playerShip.loseLife();
                SoundManager::playSound("hurt");
                bullets[i].deactivate();
            }
            continue;
        }

        Boss& boss = waveManager.getBoss();
        if (boss.isActive() && bullets[i].getBounds().intersects(boss.getBounds())) {
            bullets[i].deactivate();
            int dmg = bullets[i].isPowerBullet() ? 2 : 1;

            int prevHealth = boss.getHealth();
            boss.takeDamage(dmg);

            if (boss.getHealth() / 50 < prevHealth / 50) {
                float randomX = static_cast<float>(rand() % (WINDOW_WIDTH - 50) + 25);
                int pType = (rand() % 5) + 1;
                spawnPowerup(Vector2f(randomX, -50.f), pType);
            }

            score += 100;
            if (!boss.isActive()) {
                mode = MODE_VICTORY;
                score += 10000;
                SoundManager::stopMusic();
                SoundManager::playSound("destroy_boss");
                SoundManager::playSound("victory");
                window.close();
            }
        }

        if (asteroid.isActive() && bullets[i].getBounds().intersects(asteroid.getBounds())) {
            bullets[i].deactivate();
            int dmg = bullets[i].isPowerBullet() ? 2 : 1;
            asteroid.takeDamage(dmg);
            score += 10;
            if (!asteroid.isActive()) {
                SoundManager::playSound("destroy_asteroid");
                if (rand() % 100 < 20) {
                    if (rand() % 2 == 0) spawnPowerup(asteroid.getPosition(), POW_LIFE);
                    else spawnPowerup(asteroid.getPosition(), POW_SHIELD);
                }
            }
        }
    }

    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (powerups[i].isActive()) {
            if (playerShip.getBounds().intersects(powerups[i].getBounds())) {
                int t = powerups[i].getType();
                powerups[i].deactivate();
                SoundManager::playSound("powerup_catch");

                if (t == POW_DOUBLE) playerShip.activateDoubleShot();
                else if (t == POW_HELPER) playerShip.activateHelper();
                else if (t == POW_GREATER) playerShip.activateGreaterPower();
                else if (t == POW_SHIELD) {
                    Resource currentShield(playerShip.getShieldStrength());
                    Resource newShield(1);
                    Resource totalShield = currentShield + newShield;
                    playerShip.setShield(totalShield.getValue());
                }
                else if (t == POW_LIFE) {
                    Resource currentLives(playerShip.getLives());
                    Resource healthPack(1);
                    Resource newTotal = currentLives + healthPack;
                    playerShip.setLives(newTotal.getValue());
                }
            }
        }
    }

    int maxEnemies = waveManager.getMaxEnemies();
    for (int i = 0; i < maxEnemies; ++i) {
        Enemy* enemies[3] = { &waveManager.getBasicEnemy(i), &waveManager.getFastEnemy(i), &waveManager.getPatternEnemy(i) };
        for (int t = 0; t < 3; ++t) {
            Enemy* e = enemies[t];
            if (!e->isActive()) continue;

            for (int j = 0; j < MAX_BULLETS; ++j) {
                if (!bullets[j].isHostileBullet() && bullets[j].isActive() && bullets[j].getBounds().intersects(e->getBounds())) {
                    bullets[j].deactivate();
                    int dmg = bullets[j].isPowerBullet() ? 2 : 1;
                    e->takeDamage(dmg);
                    score += 50;
                    if (!e->isActive()) {
                        SoundManager::playSound("destroy_enemy");
                        if (e->hasPowerup()) spawnPowerup(e->getPosition(), e->getPowerup());
                    }
                }
            }
            if (!playerShip.isInvulnerable() && playerShip.getBounds().intersects(e->getBounds())) {
                e->collideWithPlayer(playerShip);
                playerShip.loseLife();
                SoundManager::playSound("hurt");
            }
        }
    }

    if (asteroid.isActive() && !playerShip.isInvulnerable()) {
        if (playerShip.getBounds().intersects(asteroid.getBounds())) {
            playerShip.loseLife();
            SoundManager::playSound("hurt");
            asteroid.deactivate();
            SoundManager::playSound("destroy_asteroid");
        }
    }

    Boss& boss = waveManager.getBoss();
    if (boss.isActive() && !playerShip.isInvulnerable()) {
        if (playerShip.getBounds().intersects(boss.getBounds())) {
            playerShip.loseLife();
            SoundManager::playSound("hurt");
        }
    }
}