#include "MainMenuSpace.hpp"

#include "../TankSpace/TankSpace.hpp"
#include "../GlobalConsts.hpp"
#include "TextInfo.hpp"

const fw::Vec2f MENU_ITEMS_SPACING = fw::Vec2f(0.f, 50.f);

MainMenuSpace::MainMenuSpace(
    fw::Game* game, 
    const fw::Vec2f& windowSize,
    const fw::Font& font
)
    : 
    Space(game, fw::Rectangle(fw::Vec2f::zero(), windowSize)),
    m_font(font)
{
    std::vector<std::shared_ptr<fw::MenuItem>> mainMenuItems;

    mainMenuItems.push_back(std::make_shared<fw::MenuItem>(
        "START",
        m_font,
        (windowSize / 2.f) - MENU_ITEMS_SPACING,
        [&](){
	        auto tankSpace = std::make_shared<TankSpace>(getGame(), windowSize, font);
            getGame()->pushSpace(tankSpace);
        }
    ));

    mainMenuItems.push_back(std::make_shared<fw::MenuItem>(
        "DIFFICULTY",
        m_font,
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
        m_font,
        (windowSize / 2.f) + MENU_ITEMS_SPACING,
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
        GlobalConsts::NORMAL_DIFFICULTY_STR,
        m_font,
        (windowSize / 2.f) - MENU_ITEMS_SPACING,
        [&](){
	        fw::GlobalStore::setInt(GlobalConsts::DIFFICULTY_SETTING_KEY, int(DifficultySettings::Normal));
        }
    ));

    difficultyMenuItems.push_back(std::make_shared<fw::MenuItem>(
        GlobalConsts::HARD_DIFFICULTY_STR,
        m_font,
        windowSize / 2.f,
        [&](){
            fw::GlobalStore::setInt(GlobalConsts::DIFFICULTY_SETTING_KEY, int(DifficultySettings::Hard));
        }
    ));

    difficultyMenuItems.push_back(std::make_shared<fw::MenuItem>(
        GlobalConsts::CANTANKEROUS_DIFFICULTY_STR,
        m_font,
        (windowSize / 2.f) + MENU_ITEMS_SPACING,
        [&](){
            fw::GlobalStore::setInt(GlobalConsts::DIFFICULTY_SETTING_KEY, int(DifficultySettings::Cantankerous));
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

    std::shared_ptr<TextInfo> textInfo = std::make_shared<TextInfo>(m_font, windowSize);

    addGameObject(textInfo);
}

