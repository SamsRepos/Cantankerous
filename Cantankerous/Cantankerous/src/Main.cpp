#include <iostream>

#include <FlatWhite.hpp>

#include "TankSpace/TankSpace.hpp"
#include "TankSpace/Difficulty.hpp"

const int WINDOW_WIDTH         = 1366;
const int WINDOW_HEIGHT        = 768;
const fw::Vec2f WINDOW_SIZE    = fw::Vec2f(WINDOW_WIDTH, WINDOW_HEIGHT);
const int HALF_WINDOW_WIDTH    = WINDOW_WIDTH / 2;
const int HALF_WINDOW_HEIGHT   = WINDOW_HEIGHT / 2;
const std::string WINDOW_TITLE = "Cantankerous";



int main() 
{

	fw::Game game;
	
	game.setWindowDimensions(
		WINDOW_WIDTH, 
		WINDOW_HEIGHT
	);

	game.setWindowTitle(WINDOW_TITLE);

	auto tankSpace = std::make_shared<TankSpace>(WINDOW_SIZE, DifficultySetting::Normal);

	game.addSpace(tankSpace);

	game.run();

}