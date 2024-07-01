#pragma once

#include <FlatWhite.hpp>

class MenuItemGroup;

class MenuSpace : public fw::Space
{
public:
	MenuSpace(const fw::Vec2f& windowSize);

protected:
	virtual void update(const float& deltaTime);

	std::shared_ptr<MenuItemGroup> m_menuItemGroup;
};