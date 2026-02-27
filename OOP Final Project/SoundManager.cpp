#include "SoundManager.h"
#include <iostream>
using namespace sf;
using namespace std;
NamedSound SoundManager::soundAssets[MAX_SOUND_BUFFERS];
Sound SoundManager::soundChannels[MAX_SOUND_CHANNELS];
Music SoundManager::music;

void SoundManager::loadAll() {
    for (int i = 0; i < MAX_SOUND_BUFFERS; ++i) {
        soundAssets[i].loaded = false;
    }

    int index = 0;
    auto load = [&](const string& name, const string& path) {
        if (index >= MAX_SOUND_BUFFERS) return;
        if (soundAssets[index].buffer.loadFromFile(path)) {
            soundAssets[index].name = name;
            soundAssets[index].loaded = true;
            index++;
        }
        else {
            cerr << "Failed to load sound: " << path << "\n";
        }
        };

    load("intro", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\intro.wav");

    load("fire_simple", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\laser_simple.wav");
    load("fire_strong", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\laser_strong.wav");
    load("fire_helper", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\laser_helper.wav");

    load("destroy_enemy", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\explosion_enemy.wav");
    load("destroy_asteroid", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\explosion_asteroid.wav");
    load("destroy_boss", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\explosion_boss.wav");

    load("powerup_spawn", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\powerup_spawn.wav");
    load("powerup_catch", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\powerup_catch.wav");

    load("boss_motion", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\boss_motion.wav");
    load("boss_drop", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\boss_drop.wav");
    load("boss_warn", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\warning.wav");

    load("level_complete", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\level_complete.wav");
    load("victory", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\victory.wav");
    load("hurt", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\hurt.wav");
    load("game_over", "C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\game_over.wav");
}

void SoundManager::playSound(const string& name) {
    SoundBuffer* bufferToPlay = nullptr;
    for (int i = 0; i < MAX_SOUND_BUFFERS; ++i) {
        if (soundAssets[i].loaded && soundAssets[i].name == name) {
            bufferToPlay = &soundAssets[i].buffer;
            break;
        }
    }

    if (!bufferToPlay) return;

    for (int i = 0; i < MAX_SOUND_CHANNELS; ++i) {
        if (soundChannels[i].getStatus() == sf::Sound::Stopped) {
            soundChannels[i].setBuffer(*bufferToPlay);
            soundChannels[i].play();
            return;
        }
    }
}

void SoundManager::playMusic(const string& filename) {
    if (music.openFromFile(filename)) {
        music.setLoop(true);
        music.setVolume(50.f);
        music.play();
    }
    else {
        cerr << "Failed to load music: " << filename << "\n";
    }
}

void SoundManager::stopMusic() {
    music.stop();
}

SoundBuffer* SoundManager::getBuffer(const string& name) {
    for (int i = 0; i < MAX_SOUND_BUFFERS; ++i) {
        if (soundAssets[i].loaded && soundAssets[i].name == name) {
            return &soundAssets[i].buffer;
        }
    }
    return nullptr;
}