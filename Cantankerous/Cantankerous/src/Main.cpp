#include <iostream>

#include <FlatWhite.h>

#include "TankSpace/TankSpace.h"

const int WINDOW_WIDTH         = 1366;
const int WINDOW_HEIGHT        = 768;
const fw::Vec2f WINDOW_SIZE    = fw::Vec2f(WINDOW_WIDTH, WINDOW_HEIGHT);
const int HALF_WINDOW_WIDTH    = WINDOW_WIDTH / 2;
const int HALF_WINDOW_HEIGHT   = WINDOW_HEIGHT / 2;
const std::string WINDOW_TITLE = "Cantankerous";



int main() {

	//ESSENTIAL INIT:
	auto window = std::make_shared<fw::RenderWindow>(
		sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE
	);

	srand(time(NULL));

	sf::Clock deltaClock;
	float deltaTime;

	fw::Input input;

	auto tankSpace = std::make_shared<TankSpace>();

	std::shared_ptr<fw::Space> space(tankSpace);

	//AD HOC INIT:
	sf::Font font;
	if (!font.loadFromFile("font/arial.ttf")) {
		std::cout << "Error loading font." << std::endl;
	}
	sf::Text text("hello", font);
	text.setPosition(0, 0);
	text.setFillColor(sf::Color::Blue);

	//LOOP:
	while (window->isOpen()) {
		sf::Event event;
		input.perFrameUpdate();
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) 
			{
				window->close();
			}
			if (fw::Input::eventIsInput(event))
			{
				input.eventUpdate(event);
			}
		}

		
		
		/*std::string txtStr = "keys down: ";
		for (size_t i = 0; i < KEYS_RANGE; ++i) 
		{
			if (input.isKeyDown(i))
				txtStr.append(std::to_string(char(i)) + " ");
		}
		txtStr.append("\n");
		txtStr.append("keys pressed: ");
		for (size_t i = 0; i < KEYS_RANGE; ++i)
		{
			if (input.isKeyPressedNow(i))
				txtStr.append(std::to_string(char(i)) + " ");
		}
		txtStr.append("\n");
		txtStr.append("keys up: ");
		for (size_t i = 0; i < KEYS_RANGE; ++i)
		{
			if (input.isKeyUp(i))
				txtStr.append(std::to_string(char(i)) + " ");
		}

		text.setString(txtStr);

		window->draw(text);*/
		

		deltaTime = deltaClock.restart().asSeconds();

		space->handleInput(input);
		space->update(deltaTime);
		space->lateUpdate();

		//RENDER:
		window->clear();
		space->render(window.get());
		
		window->display();
	
	}

}