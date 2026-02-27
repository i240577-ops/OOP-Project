#pragma once
#include <SFML/Audio.hpp>
#include <string>
using namespace sf;
using namespace std;
const int MAX_SOUND_BUFFERS = 50;
const int MAX_SOUND_CHANNELS = 20;

struct NamedSound {
    string name;
    SoundBuffer buffer;
    bool loaded;
};

class SoundManager {
private:
    static NamedSound soundAssets[MAX_SOUND_BUFFERS];
    static Sound soundChannels[MAX_SOUND_CHANNELS];
    static Music music;

public:
    static void loadAll();
    static void playSound(const string& name);
    static void playMusic(const string& filename);
    static void stopMusic();
    static SoundBuffer* getBuffer(const string& name);
};
