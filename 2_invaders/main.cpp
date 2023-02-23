#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"
#include "bullets.h"

using namespace sf;
using namespace std;

std::vector<Ship *> ships;

const int gameWidth = 800;
const int gameHeight = 600;
const int invaders_rows = 8;
const int invaders_columns = 6;
Ship* player = new Player();
sf::Texture spritesheet;
sf::Sprite invader;

void Load() {
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    
    for (int r = 0; r < invaders_rows; ++r) 
    { 
        int i = rand() % 6;
        auto rect = IntRect(Vector2(i * 32, 0), Vector2(32, 32)); 

        for (int c = 0; c < invaders_columns; ++c) {
            Vector2f position = { r * 32.f + 100,c * 32.f + 100 };
            Invader* inv = new Invader(rect, position);
          
            ships.push_back(inv);
        }
       
    }
   
    ships.push_back(player);
    Bullet::Init();
    
    Invader::speed = 100.f;
    Invader::direction = true;
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
    Bullet::Update(dt);
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
        window.clear();
        Update(window);
        Bullet::Render(window);
        Render(window);
        for (const auto s : ships) {
            window.draw(*s);
        }
        window.display();
    }
    return 0;
}