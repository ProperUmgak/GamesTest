#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

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
        window.draw(invader);
    }
    return 0;
}