//ship.cpp
#include "ship.h"
#include "game.h"
#include "bullets.h"
using namespace sf;
using namespace std;

bool Invader::direction;
float Invader::speed;
float player_speed = 150.f;

Ship::Ship() {};

Ship::Ship(IntRect ir) : Sprite() {
	_sprite = ir;
	setTexture(spritesheet);
	setTextureRect(_sprite);
};

void Ship::Update(const float& dt) {}
void Ship::MoveDown(){}

void Ship::Explode() {
	setTextureRect(IntRect(Vector2(128, 32), Vector2(32, 32)));
	_exploded = true;
}

bool Ship::is_exploded() const {
	return this->_exploded;

}

//Define the ship deconstructor. 
//Although we set this to pure virtual, we still have to define it.
Ship::~Ship() = default;


//ship.cpp
Player::Player() : Ship(IntRect(Vector2(160, 32), Vector2(32, 32))) {
	setPosition({ gameWidth * .5f, gameHeight - 32.f });
}

void Player::Update(const float& dt) {
	Ship::Update(dt);

	float dir = 0.0;
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		dir--;
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		dir++;
	}
	this->move(Vector2(dir * player_speed * dt, 0.f));
	
	static float firetime = 0.0f;
	
	firetime -= dt;
	
	if (firetime <= 0 && Keyboard::isKeyPressed(Keyboard::Space)) {
		Bullet::Fire(getPosition(),false);
		firetime = 0.7f;
		
	}
	
}
void Player::MoveDown() {

}



Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
	setOrigin(Vector2f(16.f, 16.f));
	setPosition(pos);
}

void Invader::MoveDown() {
		this->move(Vector2f(0.0f, 24.0f));
}

void Invader::Update(const float& dt) {
	Ship::Update(dt);

	move(Vector2f(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f));
	
	if ((direction && getPosition().x > gameWidth - 16) ||
		(!direction && getPosition().x < 16)) {
		direction = !direction;
		for (int i = 0; i < ships.size(); ++i) {
			ships[i]->MoveDown();
		}	
	}

	static float firetime = 0.0f;
	firetime -= dt;
	if (firetime <= 0 && rand()%100 == 0){
		Bullet::Fire(getPosition(), true);
		firetime = 4.0f + (rand() % 60);
	}

}

