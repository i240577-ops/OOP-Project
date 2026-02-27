#include "Menu.h"
#include "SoundManager.h"
#include <iostream>
#include <cmath> 
using namespace std;
using namespace sf;
MainMenu::MainMenu()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Galaxy Wars"),
    selectedIndex(0),
    pulseTimer(0.f)
{
    window.setFramerateLimit(60);
    SoundManager::loadAll();
    SoundManager::playMusic("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\menu_music.ogg");

    if (!font.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Fonts\\BRLNSDB.TTF")) {
        if (!font.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Fonts\\BRLNSR.TTF")) {
            if (!font.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Fonts\\BRLNSB.TTF")) {
                cout << "ERROR: could not load font (BRLNSDB.TTF).\n";
            }
        }
    }

    if (!bgTexture.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\mbackground.png")) {
        cout << "Failed to load menu background.\n";
    }
    bgSprite.setTexture(bgTexture);
    float scaleX = static_cast<float>(WINDOW_WIDTH) / bgTexture.getSize().x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / bgTexture.getSize().y;
    bgSprite.setScale(scaleX, scaleY);

    title.setFont(font);
    title.setString("GALAXY WARS");
    title.setCharacterSize(80);
    title.setStyle(Text::Bold);
    title.setFillColor(Color(0, 255, 255));

    FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    title.setPosition(WINDOW_WIDTH / 2.f, 150.f);

    titleShadow = title;
    titleShadow.setFillColor(Color(0, 0, 0, 150));
    titleShadow.setPosition(WINDOW_WIDTH / 2.f + 5.f, 150.f + 5.f);

    const char* labels[3] = { "START GAME", "HIGH SCORES", "EXIT" };

    for (int i = 0; i < 3; ++i) {
        optionsShadow[i].setFont(font);
        optionsShadow[i].setString(labels[i]);
        optionsShadow[i].setCharacterSize(32);
        optionsShadow[i].setFillColor(Color::Black);
        FloatRect rb = optionsShadow[i].getLocalBounds();
        optionsShadow[i].setOrigin(rb.left + rb.width / 2.f, rb.top + rb.height / 2.f);
        optionsShadow[i].setPosition(WINDOW_WIDTH / 2.f + 3.f, 400.f + 80.f * i + 3.f);

        options[i].setFont(font);
        options[i].setString(labels[i]);
        options[i].setCharacterSize(32);
        options[i].setFillColor(Color::White);
        options[i].setOrigin(rb.left + rb.width / 2.f, rb.top + rb.height / 2.f);
        options[i].setPosition(WINDOW_WIDTH / 2.f, 400.f + 80.f * i);
    }
}

void MainMenu::playIntro() {
    static bool isFirstTime = true;

    if (isFirstTime) {
        SoundManager::stopMusic();
        SoundManager::playSound("intro");

        Texture introTexture;
        Sprite introSprite;
        bool hasIntroImage = true;

        if (!introTexture.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Sprites\\intro.png")) {
            hasIntroImage = false;
        }

        if (hasIntroImage) {
            introSprite.setTexture(introTexture);
            float sX = static_cast<float>(WINDOW_WIDTH) / introTexture.getSize().x;
            float sY = static_cast<float>(WINDOW_HEIGHT) / introTexture.getSize().y;
            introSprite.setScale(sX, sY);

            sf::Clock clock;
            float timer = 0.f;
            float fadeInTime = 1.5f; float holdTime = 2.0f; float fadeOutTime = 1.0f;
            float totalTime = fadeInTime + holdTime + fadeOutTime;

            while (window.isOpen()) {
                float dt = clock.restart().asSeconds();
                timer += dt;

                Event event;
                while (window.pollEvent(event)) {
                    if (event.type == Event::KeyPressed) {
                        if (event.key.code == Keyboard::Enter || event.key.code == Keyboard::Space) {
                            timer = totalTime;
                        }
                    }
                }
                if (timer >= totalTime) break;

                float alpha = 0.f;
                if (timer < fadeInTime) alpha = (timer / fadeInTime) * 255.f;
                else if (timer < fadeInTime + holdTime) alpha = 255.f;
                else {
                    float fadeOutProgress = (timer - (fadeInTime + holdTime)) / fadeOutTime;
                    alpha = 255.f - (fadeOutProgress * 255.f);
                }
                if (alpha < 0) alpha = 0; if (alpha > 255) alpha = 255;

                introSprite.setColor(Color(255, 255, 255, static_cast<Uint8>(alpha)));
                window.clear(Color::Black);
                window.draw(introSprite);
                window.display();
            }
        }
        isFirstTime = false;
        SoundManager::playMusic("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Audio\\menu_music.ogg");
    }

    bgSprite.setColor(Color(255, 255, 255, 255));
    title.setScale(1.f, 1.f);
    title.setFillColor(Color(0, 255, 255, 255));
    titleShadow.setFillColor(Color(0, 0, 0, 150));
}

void MainMenu::updateVisuals(float dt) {
    pulseTimer += dt * 5.f;
    for (int i = 0; i < 3; ++i) {
        if (i == selectedIndex) {
            options[i].setFillColor(Color::Yellow);
            float scale = 1.0f + (sin(pulseTimer) + 1.f) * 0.05f;
            options[i].setScale(scale, scale);
            optionsShadow[i].setScale(scale, scale);
        }
        else {
            options[i].setFillColor(Color(200, 200, 200));
            options[i].setScale(1.f, 1.f);
            optionsShadow[i].setScale(1.f, 1.f);
        }
    }
}

int MainMenu::run() {
    playIntro();
    Clock clock;
    bool upPressed = false; bool downPressed = false; bool enterPressed = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) { window.close(); return 2; }
        }

        if (Keyboard::isKeyPressed( Keyboard::Up)) {
            if (!upPressed) { selectedIndex--; if (selectedIndex < 0) selectedIndex = 2; upPressed = true; }
        }
        else upPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            if (!downPressed) { selectedIndex++; if (selectedIndex > 2) selectedIndex = 0; downPressed = true; }
        }
        else downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (!enterPressed) { window.close(); return selectedIndex; }
            enterPressed = true;
        }
        else enterPressed = false;

        updateVisuals(dt);
        window.clear(sf::Color::Black);
        window.draw(bgSprite);
        window.draw(titleShadow);
        window.draw(title);
        for (int i = 0; i < 3; ++i) { window.draw(optionsShadow[i]); window.draw(options[i]); }
        window.display();
    }
    return 2;
}