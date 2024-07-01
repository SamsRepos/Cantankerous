#include "MenuSpace.hpp"

#include "MenuItemGroup.hpp"
#include "MenuItem.hpp"

MenuSpace::MenuSpace(const fw::Vec2f& windowSize)
    : Space(fw::Rectangle(fw::Vec2f::zero(), windowSize))
{
    std::vector<std::shared_ptr<MenuItem>> menuItems;

    fw::Vec2f offset = fw::Vec2f(0.f, 50.f);

    menuItems.push_back(std::make_shared<MenuItem>(
        "START",
        (windowSize / 2.f) - offset
    ));

    menuItems.push_back(std::make_shared<MenuItem>(
        "DIFFICULTY",
        windowSize / 2.f
    ));

    menuItems.push_back(std::make_shared<MenuItem>(
        "HIGH SCORES",
        (windowSize / 2.f) + offset
    ));

    m_menuItemGroup = std::make_shared<MenuItemGroup>(menuItems);
    addGameObject(m_menuItemGroup);
}


void MenuSpace::update(const float& deltaTime)
{
}
