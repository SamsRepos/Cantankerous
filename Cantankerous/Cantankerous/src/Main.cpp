#include <iostream>

#include <FlatWhite.hpp>

#include "TankSpace/TankSpace.hpp"
#include "TankSpace/Difficulty.hpp"
#include "MainMenuSpace/MainMenuSpace.hpp"
#include "GlobalConsts.hpp"

const int WINDOW_WIDTH         = 1920;
const int WINDOW_HEIGHT        = 1080;
const fw::Vec2f WINDOW_SIZE    = fw::Vec2f(WINDOW_WIDTH, WINDOW_HEIGHT);
const std::string WINDOW_TITLE = "CANTANKEROUS";

int main() 
{
	fw::GlobalStore::setInt(GlobalConsts::DIFFICULTY_SETTING_KEY, int(DifficultySettings::HardEnough));
	fw::GlobalStore::setInt(GlobalConsts::SCORE_KEY, 0);

	fw::GameConfig gameConfig;

	gameConfig.windowWidth  = WINDOW_WIDTH;
	gameConfig.windowHeight = WINDOW_HEIGHT;
	gameConfig.windowTitle  = WINDOW_TITLE;

	fw::Vec2f desktopResolution = fw::util::getDesktopResolution();

	if(desktopResolution.x < WINDOW_WIDTH || desktopResolution.y < WINDOW_HEIGHT)
	{
		// TODO, polite message that the screen resolution is too small to run this game
		return -1;
	}

	gameConfig.startFullscreen     = !(desktopResolution.x > WINDOW_WIDTH || desktopResolution.y > WINDOW_HEIGHT);
	gameConfig.fullScreenTogglable = true;
	gameConfig.windowAdjustable    = false;

	fw::Game game(gameConfig);

	fw::Font font;
	font.loadFromFile("font/arial.ttf");

	auto menuSpace = std::make_shared<MainMenuSpace>(&game, WINDOW_SIZE, font);

	game.pushSpace(menuSpace);

	game.run();

	return 0;
}