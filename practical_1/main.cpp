#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

sf::Font font;
sf::Text text;

const Keyboard::Key controls[4] = {
	Keyboard::W, //Player1 up
	Keyboard::S, //Player Down
	Keyboard::Up,
	Keyboard::Down,
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
Vector2f ballVelocity;
bool server = false;
int leftScore = 0;
int rightScore = 0;
bool aI = true;

CircleShape ball;
RectangleShape paddles[2];

void Load() {
	
	//set size and origin of paddles
	for (auto& p : paddles) {
		p.setSize(paddleSize - Vector2f(3,3));
		p.setOrigin(paddleSize / 2.f);
	}

	// Load font-face from res dir
	font.loadFromFile("C:/Programming/GamesEngineering/TEST build/bin/Debug/res/fonts/RobotoMono-Regular.ttf");
	// Set text element to use font
	text.setFont(font);
	// set the character size to 24 pixels
	text.setCharacterSize(24);
	
	//load score
	string scoreString = to_string(leftScore) + " : " + to_string(rightScore);
	text.setString(scoreString);
	text.setPosition(Vector2((gameWidth * .5f) - (text.getLocalBounds().width * .5f), 0.f));
	
	//set sie and origin of ball
	ball.setRadius(ballRadius);
	ball.setOrigin(Vector2f(ballRadius / 2.f , ballRadius / 2.f));

	//reset paddle position
	paddles[0].setPosition(Vector2(10.f + paddleSize.x / 2.f, gameHeight / 2.f));
	paddles[1].setPosition(Vector2(gameWidth - paddleSize.x + 10.f/ 2.f, gameHeight / 2.f));

	//reset ball
	ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight /2.f));
	ballVelocity = { (server ? 200.0f : -200.0f), 60.0f };
}

void Reset() {
	//reset paddle position
	paddles[0].setPosition(Vector2(10.f + paddleSize.x / 2.f, gameHeight / 2.f));
	paddles[1].setPosition(Vector2(gameWidth - paddleSize.x + 10.f / 2.f, gameHeight / 2.f));
	
	if (server) {
		leftScore++;
	}
	else {
		rightScore++;
	}
	// Update Score Text
	string scoreString = to_string(leftScore) + " : " + to_string(rightScore);
	text.setString(scoreString);
	// Keep Score Text Centered
	//text.setPosition(Vector2((gameWidth * .5f) - (text.getLocalBounds().width * .5f), 0.f));

	//reset ball
	ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
	ballVelocity = { (server ? 200.0f : -200.0f), 60.0f };
	printf("SCORE!!!");
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
	//move ball
	ball.move(ballVelocity * dt);

	//Quit Via ESC Key
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}

	//handle paddle movement
	float direction = 0.0f;
	if (Keyboard::isKeyPressed(controls[0])) {
		direction--;
	}
	if (Keyboard::isKeyPressed(controls[1])) {
		direction++;
	}
	paddles[0].move(Vector2(0.f, direction * paddleSpeed * dt));
	
	if (Keyboard::isKeyPressed(Keyboard::M) && aI == true) {
		aI = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::M)) {
		aI = true;
	}

	if (!aI) {
		float oDir = 0.0;
		if (Keyboard::isKeyPressed(controls[2])) {
			oDir--;
		}
		if (Keyboard::isKeyPressed(controls[3])) {
			oDir++;
		}
		paddles[1].move(Vector2(0.f, oDir * paddleSpeed * dt));
	}
	else if (aI) {
		if (ball.getPosition().y > paddles[1].getPosition().y) {
			paddles[1].move(Vector2(0.f, 0.8f * paddleSpeed * dt));
		}
		else if (ball.getPosition().y < paddles[1].getPosition().y) {
			paddles[1].move(Vector2(0.f, -0.8f * paddleSpeed * dt));
		}
	}

	

	// check ball collision
	const float bx = ball.getPosition().x;
	const float by = ball.getPosition().y;

	if (by > gameHeight) { //bottom wall
	  // bottom wall
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(Vector2(0.f, -10.f));
	}
	else if (by < 0) { //top wall
   // top wall
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(Vector2(0.f, 10.f));
	}
	else if (bx > gameWidth) {
		// right wall
		server = true;
		Reset();
		
	}
	else if (bx < 0) {
		// left wall
		server = false;
		Reset();
		
	}

	else if (
		//ball is inline or behind paddle
		bx < paddleSize.x &&
		//AND ball is below top edge of paddle
		by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
		//AND ball is above bottom edge of paddle
		by < paddles[0].getPosition().y + (paddleSize.y * 0.5)
		)
	{
		// bounce off left paddle
		ballVelocity.x *= -1.1f;
		ballVelocity.y *= 1.1f;
		ball.move(Vector2(+10.f, 0.f));
	}

	else if (//ball is inline or behind paddle
		bx > gameWidth - paddleSize.x &&
		//AND ball is below top edge of paddle
		by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
		//AND ball is above bottom edge of paddle
		by < paddles[1].getPosition().y + (paddleSize.y * 0.5))
	{
		// bounce off right paddle
		ballVelocity.x *= -1.1f;
		ballVelocity.y *= 1.1f;
		ball.move(Vector2(-15.f, 0.f));
	}

	//dont let paddles go off screen
	const float lPaddlePostion = paddles[0].getPosition().y;
	const float rPaddlePostion = paddles[1].getPosition().y;
	if (lPaddlePostion + 50.f > gameHeight) {
		paddles[0].move(Vector2(0.f, -2.f));

	}
	if (lPaddlePostion - 50.f < 0) {
		paddles[0].move(Vector2(0.f, +2.f));

	}
	if (rPaddlePostion + 50.f > gameHeight) {
		paddles[1].move(Vector2(0.f, -2.f));

	}
	if (rPaddlePostion - 50.f < 0) {
		paddles[1].move(Vector2(0.f, +2.f));

	}
}



void Render(RenderWindow& window) {
	// Draw Everything
	window.draw(text);
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
}

int main() {
	RenderWindow window(VideoMode({ gameWidth, gameHeight}), "PONG");
	Load();
	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;
}