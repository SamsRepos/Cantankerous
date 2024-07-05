#include "MenuSpace.hpp"

#include "MenuItemGroup.hpp"
#include "MenuItem.hpp"
#include "../TankSpace/TankSpace.hpp"



MenuSpace::MenuSpace(fw::Game* game, const fw::Vec2f& windowSize)
    : 
    Space(game, fw::Rectangle(fw::Vec2f::zero(), windowSize)),
    m_difficultySetting(DifficultySettings::Normal)
{
    fw::Vec2f offset = fw::Vec2f(0.f, 50.f);

    std::vector<std::shared_ptr<MenuItem>> mainMenuItems;

    mainMenuItems.push_back(std::make_shared<MenuItem>(
        "START",
        (windowSize / 2.f) - offset,
        [&](){
	        auto tankSpace = std::make_shared<TankSpace>(getGame(), windowSize, m_difficultySetting);
            getGame()->pushSpace(tankSpace);
        }
    ));

    mainMenuItems.push_back(std::make_shared<MenuItem>(
        "DIFFICULTY",
        windowSize / 2.f,
        [&](){
            m_mainMenuItemGroup->setAwakeOnNextFrame(false);
            m_mainMenuItemGroup->setVisibleOnNextFrame(false);

            m_difficultyMenuItemGroup->setAwakeOnNextFrame(true);
            m_difficultyMenuItemGroup->setVisibleOnNextFrame(true);
        }
    ));

    mainMenuItems.push_back(std::make_shared<MenuItem>(
        "HIGH SCORES",
        (windowSize / 2.f) + offset,
        [](){}
    ));

    m_mainMenuItemGroup = std::make_shared<MenuItemGroup>(
        mainMenuItems,
        MenuInputDirectionFlags::HORIZONTAL | MenuInputDirectionFlags::VERTICAL,
        MenuInputTypeFlags::KEYBOARD | MenuInputTypeFlags::XBOX
    );

    addGameObject(m_mainMenuItemGroup);


    std::vector<std::shared_ptr<MenuItem>> difficultyMenuItems;

    difficultyMenuItems.push_back(std::make_shared<MenuItem>(
        "NORMAL",
        (windowSize / 2.f) - offset,
        [&](){
	        m_difficultySetting = DifficultySettings::Normal;
        }
    ));

    difficultyMenuItems.push_back(std::make_shared<MenuItem>(
        "HARD",
        windowSize / 2.f,
        [&](){
            m_difficultySetting = DifficultySettings::Normal;
        }
    ));

    difficultyMenuItems.push_back(std::make_shared<MenuItem>(
        "CANTANKEROUS",
        (windowSize / 2.f) + offset,
        [&](){
            m_difficultySetting = DifficultySettings::Cantankerous;
        }
    ));

    m_difficultyMenuItemGroup = std::make_shared<MenuItemGroup>(
        difficultyMenuItems,
        MenuInputDirectionFlags::HORIZONTAL | MenuInputDirectionFlags::VERTICAL,
        MenuInputTypeFlags::KEYBOARD | MenuInputTypeFlags::XBOX,
        [&](){
            m_mainMenuItemGroup->setAwakeOnNextFrame(true);
            m_mainMenuItemGroup->setVisibleOnNextFrame(true);

            m_difficultyMenuItemGroup->setAwakeOnNextFrame(false);
            m_difficultyMenuItemGroup->setVisibleOnNextFrame(false);
        }
    );

    addGameObject(m_difficultyMenuItemGroup);

    m_difficultyMenuItemGroup->setAwakeOnNextFrame(false);
    m_difficultyMenuItemGroup->setVisibleOnNextFrame(false);
}


void MenuSpace::update(const float& deltaTime)
{
}
