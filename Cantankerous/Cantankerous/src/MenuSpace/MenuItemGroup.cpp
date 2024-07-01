#include "MenuItemGroup.hpp"

#include "MenuItem.hpp"

MenuItemGroup::MenuItemGroup(std::vector<std::shared_ptr<MenuItem>> menuItems)
    :
    m_highlightedIndex(0)
{
    if(menuItems.size() == 0)
    {
        throw menuItems;
    };

    m_menuItems = menuItems;
    for(std::shared_ptr<MenuItem>& menuItem : menuItems) addChild(menuItem);

    m_menuItems[0]->setHighlighted(true);
}

void MenuItemGroup::handleInput(const fw::Input& input)
{
    fw::Vec2f dPad      = input.getXboxStick(fw::XboxStick::DPad);
    fw::Vec2f leftStick = input.getXboxStick(fw::XboxStick::Left);

    auto downNow = [&](){
        if(
            input.isKeyPressedNow(fw::Keyboard::Down) ||
            input.isKeyPressedNow(fw::Keyboard::S)
        )
        {
            return true;
        }


        if(
            dPad.y > 0.f ||
            leftStick.y > 0.f
        )
        {
            return true;
        }

        return false;
    };

    auto upNow = [&](){
        if(
            input.isKeyPressedNow(fw::Keyboard::Up) ||
            input.isKeyPressedNow(fw::Keyboard::W)
        )
        {
            return true;
        }

        if(
            dPad.y < 0.f ||
            leftStick.y < 0.f
        )
        {
            return true;
        }

        return false;
    };

    auto selectNow = [&](){
        if(
            input.isKeyPressedNow(fw::Keyboard::Return) ||
            input.isKeyPressedNow(fw::Keyboard::Enter)  ||
            input.isXboxButtonPressedNow(fw::XboxButton::A)
        )
        {
            return true;
        }

        return false;
    };

    if(upNow())
    {
        m_menuItems[m_highlightedIndex]->setHighlighted(false);
        m_highlightedIndex = std::max(int(m_highlightedIndex - 1), 0);
        m_menuItems[m_highlightedIndex]->setHighlighted(true);
    }

    if(downNow())
    {
        m_menuItems[m_highlightedIndex]->setHighlighted(false);
        m_highlightedIndex = std::min(int(m_highlightedIndex + 1), int(m_menuItems.size() - 1));
        m_menuItems[m_highlightedIndex]->setHighlighted(true);
    }
}

