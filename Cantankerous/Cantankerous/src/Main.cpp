#include <iostream>

#include <FlatWhite.hpp>

#include "TankSpace/TankSpace.hpp"
#include "TankSpace/Difficulty.hpp"
#include "MainMenuSpace/MainMenuSpace.hpp"
#include "GlobalConsts.hpp"

const int WINDOW_WIDTH         = 1366;
const int WINDOW_HEIGHT        = 768;
const fw::Vec2f WINDOW_SIZE    = fw::Vec2f(WINDOW_WIDTH, WINDOW_HEIGHT);
const int HALF_WINDOW_WIDTH    = WINDOW_WIDTH / 2;
const int HALF_WINDOW_HEIGHT   = WINDOW_HEIGHT / 2;
const std::string WINDOW_TITLE = "Cantankerous";


int main() 
{
	fw::GlobalStore::setInt(GlobalConsts::DIFFICULTY_SETTING_KEY, int(DifficultySettings::Normal));
	fw::GlobalStore::setInt(GlobalConsts::SCORE_KEY, 0);

	fw::Game game;
	
	game.setWindowDimensions(
		WINDOW_WIDTH, 
		WINDOW_HEIGHT
	);

	game.setWindowTitle(WINDOW_TITLE);

	auto menuSpace = std::make_shared<MainMenuSpace>(&game, WINDOW_SIZE);

	game.pushSpace(menuSpace);

	game.run();

}