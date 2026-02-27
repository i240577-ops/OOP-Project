#include "Game.h"
#include "Menu.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
using namespace sf;
void showHighScores() {
    RenderWindow window(VideoMode(800, 600), "Galaxy Wars - High Scores");

    Font font;
    if (!font.loadFromFile("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\Fonts\\pricedown.otf")) {
        std::cout << "ERROR: could not load font (C:\\Windows\\Fonts\\arial.ttf) for high scores\n";
    }

    Text title;
    title.setFont(font);
    title.setString("High Scores (Descending)");
    title.setCharacterSize(32);
    title.setStyle(Text::Bold);
    title.setFillColor(Color::Cyan);

    FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    title.setPosition(400.f, 60.f);

    const int MAX_ENTRIES = 50;
    string names[MAX_ENTRIES];
    int scores[MAX_ENTRIES];
    int count = 0;

    ifstream in("C:\\Users\\HP Elite book\\OneDrive\\Desktop\\SEMESTER 3\\OOP\\i24-0577, i24-2015_B_GALAXY_WARS\\i24-0577, i24-2015_B_GALAXY_WARS\\scores.txt");
    if (in) {
        string name;
        int score;
        while (count < MAX_ENTRIES && (in >> name >> score)) {
            names[count] = name;
            scores[count] = score;
            ++count;
        }
        in.close();
    }

    if (count == 0) {
        Text msg;
        msg.setFont(font);
        msg.setString("No scores saved yet.");
        msg.setCharacterSize(24);
        msg.setFillColor(Color::White);
        msg.setPosition(200.f, 200.f);

        Text info;
        info.setFont(font);
        info.setString("Press ESC or Enter to return");
        info.setCharacterSize(20);
        info.setFillColor(Color(200, 200, 200));
        info.setPosition(10.f, 560.f);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape) ||
                Keyboard::isKeyPressed(Keyboard::Enter)) {
                window.close();
            }

            window.clear(Color::Black);
            window.draw(title);
            window.draw(msg);
            window.draw(info);
            window.display();
        }
        return;
    }

    //Bubble sort
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - 1 - i; ++j) {
            if (scores[j] < scores[j + 1]) {
                int ts = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = ts;

                string tn = names[j];
                names[j] = names[j + 1];
                names[j + 1] = tn;
            }
        }
    }
    const int MAX_DISPLAY = 10;
    Text scoreText[MAX_DISPLAY];
    int displayCount = (count < MAX_DISPLAY) ? count : MAX_DISPLAY;

    for (int i = 0; i < displayCount; ++i) {
        string line =
            to_string(i + 1) + ". " + names[i] + "  -  " + to_string(scores[i]);
        scoreText[i].setFont(font);
        scoreText[i].setString(line);
        scoreText[i].setCharacterSize(24);
        scoreText[i].setFillColor(Color::White);
        scoreText[i].setPosition(140.f, 130.f + i * 30.f);
    }

    Text info;
    info.setFont(font);
    info.setString("Press ESC or Enter to return");
    info.setCharacterSize(20);
    info.setFillColor(Color(200, 200, 200));
    info.setPosition(10.f, 560.f);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape) ||
            Keyboard::isKeyPressed(Keyboard::Enter)) {
            window.close();
        }

        window.clear(Color::Black);
        window.draw(title);
        for (int i = 0; i < displayCount; ++i) {
            window.draw(scoreText[i]);
        }
        window.draw(info);
        window.display();
    }
}

int main() {
    bool quit = false;

    while (!quit) {
        MainMenu menu;
        int choice = menu.run();

        if (choice == 0) {
            cout << "Starting game...\n";

            Game game;
            game.run();
            cout << "Game ended, returning to menu...\n";
        }
        else if (choice == 1) {
            showHighScores();
        }
        else {
            quit = true;
        }
    }

    cout << "Program terminated.\n";
    return 0;
}
