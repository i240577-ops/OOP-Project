#include "WaveManager.h"
#include "Constants.h"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace sf;
WaveManager::WaveManager() {
    reset();
}

void WaveManager::reset() {
    currentLevel = 1;
    currentWave = 1;
    spawnTimer = 0.f;
    spawnInterval = 0.8f;
    enemiesToSpawn = 0;
    enemiesSpawnedCount = 0;
    spawningEnabled = true;
    waveInProgress = false;

    boss.takeDamage(9999);

    for (int i = 0; i < MAX_ENEMIES; ++i) {
        basicEnemies[i].deactivate();
        fastEnemies[i].deactivate();
        patternEnemies[i].deactivate();
        pendingPowerups[i] = POW_NONE;
    }
}

void WaveManager::distributePowerupsForWave(int count) {
    for (int i = 0; i < MAX_ENEMIES; ++i) pendingPowerups[i] = POW_NONE;
    if (count < 1) return;
    int indices[MAX_ENEMIES];
    for (int i = 0; i < count; ++i) indices[i] = i;
    for (int i = 0; i < count; ++i) {
        int r = rand() % count;
        int temp = indices[i];
        indices[i] = indices[r];
        indices[r] = temp;
    }
    int r = rand() % 3;
    int chosenType = POW_DOUBLE;
    if (r == 1) chosenType = POW_HELPER;
    if (r == 2) chosenType = POW_GREATER;
    pendingPowerups[indices[0]] = chosenType;
    if (currentLevel > 1 && count > 5 && (rand() % 100 < 30)) {
        pendingPowerups[indices[1]] = POW_DOUBLE;
    }
}

void WaveManager::update(float dt, const Vector2f& playerPos) {

    if (boss.isActive()) {
        boss.update(dt, playerPos);

        if (boss.wantsToSummon()) {
            spawnBossReinforcements();
        }
    }
    for (int i = 0; i < MAX_ENEMIES; ++i) {
        basicEnemies[i].updateAI(dt, playerPos);
        basicEnemies[i].update(dt);
        fastEnemies[i].updateAI(dt, playerPos);
        fastEnemies[i].update(dt);
        patternEnemies[i].updateAI(dt, playerPos);
        patternEnemies[i].update(dt);
    }

    if (!spawningEnabled) return;
    if (currentLevel == 4) {
        if (!boss.isActive()) {
            boss.spawn();
            waveInProgress = false;
        }
        return;
    }
    if (!waveInProgress) {
        int count = 0;
        const int WAVE_SIZE = 20;

        if (currentLevel == 1) { if (currentWave == 1) count = WAVE_SIZE; }
        else if (currentLevel == 2) { if (currentWave <= 2) count = WAVE_SIZE; }
        else if (currentLevel == 3) { if (currentWave <= 3) count = WAVE_SIZE; }

        if (count > 0) {
            enemiesToSpawn = count;
            enemiesSpawnedCount = 0;
            waveInProgress = true;
            spawnTimer = 0.f;
            distributePowerupsForWave(count);
        }
    }

    if (waveInProgress && enemiesToSpawn > 0) {
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.f;
            spawnNextEnemy();
            enemiesToSpawn--;
            enemiesSpawnedCount++;
        }
    }

    if (waveInProgress && enemiesToSpawn == 0) {
        if (areAllEnemiesDead()) {
            waveInProgress = false;
            currentWave++;

            bool levelUp = false;
            if (currentLevel == 1 && currentWave > 1) levelUp = true;
            if (currentLevel == 2 && currentWave > 2) levelUp = true;
            if (currentLevel == 3 && currentWave > 3) levelUp = true;

            if (levelUp) {
                currentLevel++;
                currentWave = 1;
            }
        }
    }
}

void WaveManager::spawnNextEnemy() {
    Vector2f pos(40.f, 70.f);
    int pType = pendingPowerups[enemiesSpawnedCount];
    int type = 0;
    if (currentLevel >= 2 && currentWave >= 2) type = 1;
    if (currentLevel >= 3 && currentWave >= 3) type = 2;

    if (type == 0) { for (int i = 0; i < MAX_ENEMIES; ++i) if (!basicEnemies[i].isActive()) { basicEnemies[i].spawnAt(pos); basicEnemies[i].setPowerup(pType); break; } }
    else if (type == 1) { for (int i = 0; i < MAX_ENEMIES; ++i) if (!fastEnemies[i].isActive()) { fastEnemies[i].spawnAt(pos); fastEnemies[i].setPowerup(pType); break; } }
    else if (type == 2) { for (int i = 0; i < MAX_ENEMIES; ++i) if (!patternEnemies[i].isActive()) { patternEnemies[i].spawnAt(pos); patternEnemies[i].setPowerup(pType); break; } }
}

void WaveManager::spawnBossReinforcements() {
    float y = 50.f;
    fastEnemies[0].spawnAt(Vector2f(100.f, y));
    fastEnemies[1].spawnAt(Vector2f(WINDOW_WIDTH - 100.f, y));
    patternEnemies[0].spawnAt(Vector2f(200.f, y + 50.f));
    patternEnemies[1].spawnAt(Vector2f(WINDOW_WIDTH - 200.f, y + 50.f));
}

bool WaveManager::areAllEnemiesDead() const {
    for (int i = 0; i < MAX_ENEMIES; ++i) {
        if (basicEnemies[i].isActive()) return false;
        if (fastEnemies[i].isActive()) return false;
        if (patternEnemies[i].isActive()) return false;
    }
    return true;
}

void WaveManager::render(RenderWindow& window) {
    if (boss.isActive()) boss.render(window);

    for (int i = 0; i < MAX_ENEMIES; ++i) {
        basicEnemies[i].render(window);
        fastEnemies[i].render(window);
        patternEnemies[i].render(window);
    }
}

float WaveManager::getLowestEnemyX() const {
    float lowestY = -9999.f;
    float targetX = -1.f;
    if (boss.isActive()) return boss.getPosition().x;

    for (int i = 0; i < MAX_ENEMIES; ++i) {
        if (basicEnemies[i].isActive() && basicEnemies[i].getPosition().y > lowestY) { lowestY = basicEnemies[i].getPosition().y; targetX = basicEnemies[i].getPosition().x; }
        if (fastEnemies[i].isActive() && fastEnemies[i].getPosition().y > lowestY) { lowestY = fastEnemies[i].getPosition().y; targetX = fastEnemies[i].getPosition().x; }
        if (patternEnemies[i].isActive() && patternEnemies[i].getPosition().y > lowestY) { lowestY = patternEnemies[i].getPosition().y; targetX = patternEnemies[i].getPosition().x; }
    }
    return targetX;
}