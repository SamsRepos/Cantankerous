#pragma once

#include <FlatWhite.hpp>

#include "../TankSpace/Difficulty.hpp"

class MainMenuSpace : public fw::Space
{
public:
	MainMenuSpace(fw::Game* game, const fw::Vec2f& windowSize);

protected:
	virtual void update(const float& deltaTime);

private:
	std::shared_ptr<fw::Menu> m_mainMenu;
	std::shared_ptr<fw::Menu> m_difficultyMenu;

	DifficultySettings m_difficultySetting;
};