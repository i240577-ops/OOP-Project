#pragma once
#include "BasicEnemy.h"
#include "FastEnemy.h"
#include "PatternEnemy.h"
#include "Boss.h" 
#include "Ship.h"
#include <SFML/Graphics.hpp>

class WaveManager {
private:
    static const int MAX_ENEMIES = 50;

    BasicEnemy   basicEnemies[MAX_ENEMIES];
    FastEnemy    fastEnemies[MAX_ENEMIES];
    PatternEnemy patternEnemies[MAX_ENEMIES];

    Boss boss;

    int pendingPowerups[MAX_ENEMIES];

    int currentLevel;
    int currentWave;

    float spawnTimer;
    float spawnInterval;
    int enemiesToSpawn;
    int enemiesSpawnedCount;

    bool spawningEnabled;
    bool waveInProgress;

    void spawnNextEnemy();
    bool areAllEnemiesDead() const;
    void distributePowerupsForWave(int enemyCount);

public:
    WaveManager();

    void reset();
    void update(float dt, const sf::Vector2f& playerPos);
    void render(sf::RenderWindow& window);

    int getMaxEnemies() const { return MAX_ENEMIES; }

    BasicEnemy& getBasicEnemy(int i) { return basicEnemies[i]; }
    FastEnemy& getFastEnemy(int i) { return fastEnemies[i]; }
    PatternEnemy& getPatternEnemy(int i) { return patternEnemies[i]; }

    Boss& getBoss() { return boss; }

    int getCurrentLevel() const { return currentLevel; }
    void setSpawningEnabled(bool enabled) { spawningEnabled = enabled; }

    float getLowestEnemyX() const;
    void spawnBossReinforcements();
};