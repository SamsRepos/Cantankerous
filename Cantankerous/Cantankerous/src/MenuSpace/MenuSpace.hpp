#pragma once

#include <FlatWhite.hpp>

#include "../TankSpace/Difficulty.hpp"

class MenuItemGroup;

class MenuSpace : public fw::Space
{
public:
	MenuSpace(fw::Game* game, const fw::Vec2f& windowSize);

protected:
	virtual void update(const float& deltaTime);

private:
	std::shared_ptr<MenuItemGroup> m_mainMenuItemGroup;
	std::shared_ptr<MenuItemGroup> m_difficultyMenuItemGroup;

	DifficultySettings m_difficultySetting;
};