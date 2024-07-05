#include "MainMenuSpace.hpp"

#include "../TankSpace/TankSpace.hpp"



MainMenuSpace::MainMenuSpace(fw::Game* game, const fw::Vec2f& windowSize)
    : 
    Space(game, fw::Rectangle(fw::Vec2f::zero(), windowSize)),
    m_difficultySetting(DifficultySettings::Normal)
{
    fw::Vec2f offset = fw::Vec2f(0.f, 50.f);

    std::vector<std::shared_ptr<fw::MenuItem>> mainMenuItems;

    mainMenuItems.push_back(std::make_shared<fw::MenuItem>(
        "START",
        (windowSize / 2.f) - offset,
        [&](){
	        auto tankSpace = std::make_shared<TankSpace>(getGame(), windowSize, m_difficultySetting);
            getGame()->pushSpace(tankSpace);
        }
    ));

    mainMenuItems.push_back(std::make_shared<fw::MenuItem>(
        "DIFFICULTY",
        windowSize / 2.f,
        [&](){
            m_mainMenu->setAwakeOnNextFrame(false);
            m_mainMenu->setVisibleOnNextFrame(false);

            m_difficultyMenu->setAwakeOnNextFrame(true);
            m_difficultyMenu->setVisibleOnNextFrame(true);
        }
    ));

    mainMenuItems.push_back(std::make_shared<fw::MenuItem>(
        "HIGH SCORES",
        (windowSize / 2.f) + offset,
        [](){}
    ));

    m_mainMenu = std::make_shared<fw::Menu>(
        mainMenuItems,
        fw::MenuInputDirectionFlags::HORIZONTAL | fw::MenuInputDirectionFlags::VERTICAL,
        fw::MenuInputTypeFlags::KEYBOARD | fw::MenuInputTypeFlags::XBOX
    );

    addGameObject(m_mainMenu);


    std::vector<std::shared_ptr<fw::MenuItem>> difficultyMenuItems;

    difficultyMenuItems.push_back(std::make_shared<fw::MenuItem>(
        "NORMAL",
        (windowSize / 2.f) - offset,
        [&](){
	        m_difficultySetting = DifficultySettings::Normal;
        }
    ));

    difficultyMenuItems.push_back(std::make_shared<fw::MenuItem>(
        "HARD",
        windowSize / 2.f,
        [&](){
            m_difficultySetting = DifficultySettings::Normal;
        }
    ));

    difficultyMenuItems.push_back(std::make_shared<fw::MenuItem>(
        "CANTANKEROUS",
        (windowSize / 2.f) + offset,
        [&](){
            m_difficultySetting = DifficultySettings::Cantankerous;
        }
    ));

    m_difficultyMenu = std::make_shared<fw::Menu>(
        difficultyMenuItems,
        fw::MenuInputDirectionFlags::HORIZONTAL | fw::MenuInputDirectionFlags::VERTICAL,
        fw::MenuInputTypeFlags::KEYBOARD | fw::MenuInputTypeFlags::XBOX,
        [&](){
            m_mainMenu->setAwakeOnNextFrame(true);
            m_mainMenu->setVisibleOnNextFrame(true);

            m_difficultyMenu->setAwakeOnNextFrame(false);
            m_difficultyMenu->setVisibleOnNextFrame(false);
        }
    );

    addGameObject(m_difficultyMenu);

    m_difficultyMenu->setAwakeOnNextFrame(false);
    m_difficultyMenu->setVisibleOnNextFrame(false);
}


void MainMenuSpace::update(const float& deltaTime)
{
}
