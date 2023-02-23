#include "bullets.h"
#include "game.h"
#include "ship.h"



using namespace sf;
using namespace std;

unsigned char Bullet::bulletPointer;
Bullet Bullet::bullets[256];

//Create definition for the constructor
//...
Bullet::Bullet() {
};


void Bullet::Update(const float& dt) {
	for (int i = 0; i < 256; i++) {
		bullets[i]._Update(dt);
	}
};

void Bullet::Render(sf::RenderWindow& window) {
	for (const auto s : bullets) {
        if (s.getPosition().y < -32 || s.getPosition().y > gameHeight + 32) {
            //off screen - do nothing
        }
        else {
            window.draw(s);
        }
	}
};

void Bullet::Fire(const sf::Vector2f& pos, const bool mode) {
	// Choose next bullet in the pool and set its position and mode
    bullets[bulletPointer].setPosition(pos);
    bullets[bulletPointer]._mode = mode;
    bulletPointer++;

};

void Bullet::Init() {

    bulletPointer = 0;

	for (int i = 0; i < 256; i++) {
		bullets[i].setTexture(spritesheet);
		bullets[i].setTextureRect(IntRect(Vector2(32 * 2, 32), Vector2(32, 32)));
		bullets[i].setOrigin(Vector2f(16.f, 16.f));
		bullets[i].setPosition({ -100, -100 });
	}

};

void Bullet::_Update(const float& dt) {
    if (getPosition().y < -32 || getPosition().y > gameHeight + 32) {
        //off screen - do nothing
        return;
    }
    else {
        move(Vector2f(0, dt * 200.0f * (_mode ? 1.0f : -1.0f)));
        const FloatRect boundingBox = getGlobalBounds();

        for (auto s : ships) {
            if (!_mode && s == player) {
                //player bulelts don't collide with player
                continue;
            }
            if (_mode && s != player) {
                //invader bullets don't collide with other invaders
                continue;
            }

            if (!s->is_exploded() &&
                s->getGlobalBounds().findIntersection(boundingBox)) {
                //Explode the ship
                s->Explode();
                //warp bullet off-screen
                setPosition(Vector2f(-100, -100));
                if (s != player)
                {
                    //score++;
                }
                return;
            }

        }
    }
};

