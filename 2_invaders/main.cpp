#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"
using namespace sf;
using namespace std;

std::vector<Ship *> ships;

const int gameWidth = 800;
const int gameHeight = 600;

sf::Texture spritesheet;
sf::Sprite invader;

void Load() {
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    invader.setTexture(spritesheet); //goes left,goes down
    invader.setTextureRect(IntRect(Vector2(0, 0), Vector2(32, 32)));
    Invader* inv = new Invader(sf::IntRect(Vector2(0, 0), Vector2(32, 32)), { 100,100 });
    ships.push_back(inv);
}

void Update(RenderWindow& window) {

    //Reset clock, recalc delta time
    static Clock clock;
    float dt = clock.restart().asSeconds();

    //check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }
    }

    //Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    for (auto& s : ships) {
        s->Update(dt);
    }
}

void Render(RenderWindow& window) {

}

int main() {
    RenderWindow window(VideoMode({ gameWidth, gameHeight }), "SPACE_INVADERS");
    Load();
    while (window.isOpen()) {
        //window.clear();
        Update(window);
        Render(window);
        window.display();
        for (const auto s : ships) {
            window.draw(*s);
        }
    }
    return 0;
}