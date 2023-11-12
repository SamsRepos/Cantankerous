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
		WINDOW_WIDTH, 
		WINDOW_HEIGHT,
		WINDOW_TITLE
	);

	srand(time(NULL));

	fw::DeltaClock deltaClock;

	fw::Input input;

	auto tankSpace = std::make_shared<TankSpace>();

	std::shared_ptr<fw::Space> space(tankSpace);

	////AD HOC INIT:
	//sf::Font font;
	//if (!font.loadFromFile("font/arial.ttf")) {
	//	std::cout << "Error loading font." << std::endl;
	//}
	//sf::Text text("hello", font);
	//text.setPosition(0, 0);
	//text.setFillColor(sf::Color::Blue);

	//LOOP:
	while (window->isOpen()) 
	{
		input.perFrameUpdate();
		window->pollAllEvents(&input);
		
		space->handleInput(input);
		space->update(deltaClock.getDeltaTime());
		space->lateUpdate();

		window->clear();
		space->render(window.get());
		window->display();
	
	}

}