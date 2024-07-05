#include "MenuSpace.hpp"

#include "MenuItemGroup.hpp"
#include "MenuItem.hpp"
#include "../TankSpace/TankSpace.hpp"
#include "../TankSpace/Difficulty.hpp"


MenuSpace::MenuSpace(fw::Game* game, const fw::Vec2f& windowSize)
    : Space(game, fw::Rectangle(fw::Vec2f::zero(), windowSize))
{
    std::vector<std::shared_ptr<MenuItem>> menuItems;

    fw::Vec2f offset = fw::Vec2f(0.f, 50.f);

    menuItems.push_back(std::make_shared<MenuItem>(
        "START",
        (windowSize / 2.f) - offset,
        [&](){
	        auto tankSpace = std::make_shared<TankSpace>(getGame(), windowSize, DifficultySettings::Normal);
            getGame()->pushSpace(tankSpace);
        }
    ));

    menuItems.push_back(std::make_shared<MenuItem>(
        "DIFFICULTY",
        windowSize / 2.f,
        [](){}
    ));

    menuItems.push_back(std::make_shared<MenuItem>(
        "HIGH SCORES",
        (windowSize / 2.f) + offset,
        [](){}
    ));

    m_menuItemGroup = std::make_shared<MenuItemGroup>(
        menuItems,
        MenuInputDirectionFlags::HORIZONTAL | MenuInputDirectionFlags::VERTICAL,
        MenuInputTypeFlags::KEYBOARD | MenuInputTypeFlags::XBOX
    );
    addGameObject(m_menuItemGroup);

    
    
}


void MenuSpace::update(const float& deltaTime)
{
}
