#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h" 
using namespace sf;

class MainMenu {
private:
    RenderWindow window;
    Font font;
    Texture bgTexture;
    Sprite bgSprite;
    Text title;
    Text titleShadow;
    Text options[3];
    Text optionsShadow[3];

    int selectedIndex;
    float pulseTimer;

    void updateVisuals(float dt);
    void playIntro();

public:
    MainMenu();

    int run();
};
