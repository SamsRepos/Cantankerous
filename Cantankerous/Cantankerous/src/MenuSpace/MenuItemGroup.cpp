#include "MenuItemGroup.hpp"

#include "MenuItem.hpp"

MenuItemGroup::MenuItemGroup(
    std::vector<std::shared_ptr<MenuItem>> menuItems, 
    MenuInputDirectionFlags inputDirections, 
    MenuInputTypeFlags inputTypes,
    std::function<void()> commonPayload
)
    :
    m_inputDirections(inputDirections),
    m_inputTypes(inputTypes),
    m_highlightedIndex(0),
    M_MENU_ITEMS_MAX_INDEX(menuItems.size() - 1),
    m_commonPayload(commonPayload)
{
    if(menuItems.size() == 0)
    {
        throw menuItems;
    };

    m_menuItems = menuItems;
    for(std::shared_ptr<MenuItem>& menuItem : menuItems)
    {
        addChild(menuItem);
    }
        
    m_menuItems[0]->setHighlighted(true);    
}

void MenuItemGroup::handleInput(const fw::Input& input)
{
    fw::Vec2f dPad      = input.getXboxStickIfExceededThresholdNow(fw::XboxStick::DPad, 20.f);
    fw::Vec2f leftStick = input.getXboxStickIfExceededThresholdNow(fw::XboxStick::Left, 20.f);

    auto previousNow = [&](){

        if(m_inputTypes & MenuInputTypeFlags::KEYBOARD)
        {
            if(m_inputDirections & MenuInputDirectionFlags::VERTICAL)
            {
                if(
                    input.isKeyPressedNow(fw::Keyboard::Up) ||
                    input.isKeyPressedNow(fw::Keyboard::W)
                )
                {
                    return true;
                }
            }
            if(m_inputDirections & MenuInputDirectionFlags::HORIZONTAL)
            {
                if(
                    input.isKeyPressedNow(fw::Keyboard::Left) ||
                    input.isKeyPressedNow(fw::Keyboard::A)
                )
                {
                    return true;
                }
            }
        }

        if(m_inputTypes & MenuInputTypeFlags::XBOX)
        {
            if(m_inputDirections & MenuInputDirectionFlags::VERTICAL)
            {
                if(
                    dPad.y < 0.f ||
                    leftStick.y < 0.f
                )
                {
                    return true;
                }
            }
            if(m_inputDirections & MenuInputDirectionFlags::HORIZONTAL)
            {
                if(
                    dPad.x < 0.f ||
                    leftStick.x < 0.f
                )
                {
                    return true;
                }
            }
        }

        return false;
    };

    auto nextNow = [&](){

        if(m_inputTypes & MenuInputTypeFlags::KEYBOARD)
        {
            if(m_inputDirections & MenuInputDirectionFlags::VERTICAL)
            {
                if(
                    input.isKeyPressedNow(fw::Keyboard::Down) ||
                    input.isKeyPressedNow(fw::Keyboard::S)
                )
                {
                    return true;
                }
            }
            if(m_inputDirections & MenuInputDirectionFlags::HORIZONTAL)
            {
                if(
                    input.isKeyPressedNow(fw::Keyboard::Right) ||
                    input.isKeyPressedNow(fw::Keyboard::D)
                )
                {
                    return true;
                }
            }
        }

        if(m_inputTypes & MenuInputTypeFlags::XBOX)
        {
            if(m_inputDirections & MenuInputDirectionFlags::VERTICAL)
            {
                if(
                    dPad.y > 0.f ||
                    leftStick.y > 0.f
                )
                {
                    return true;
                }
            }
            if(m_inputDirections & MenuInputDirectionFlags::HORIZONTAL)
            {
                if(
                    dPad.x > 0.f ||
                    leftStick.x > 0.f
                )
                {
                    return true;
                }
            }
        }

        return false;
    };

    auto selectNow = [&](){
        if(m_inputTypes & MenuInputTypeFlags::XBOX)
        {
            if(input.isXboxButtonPressedNow(fw::XboxButton::A))
            {
                return true;
            }
        }

        if(m_inputTypes & MenuInputTypeFlags::KEYBOARD)
        {
            if(input.isKeyPressedNow(fw::Keyboard::Enter))
            {
                return true;
            }
        }

        return false;
    };

    if(previousNow())
    {
        m_menuItems[m_highlightedIndex]->setHighlighted(false);
        m_highlightedIndex = std::max(int(m_highlightedIndex - 1), 0);
        m_menuItems[m_highlightedIndex]->setHighlighted(true);
    }

    if(nextNow())
    {
        m_menuItems[m_highlightedIndex]->setHighlighted(false);
        m_highlightedIndex = std::min(size_t(m_highlightedIndex + 1), M_MENU_ITEMS_MAX_INDEX);
        m_menuItems[m_highlightedIndex]->setHighlighted(true);
    }

    if(selectNow())
    {
        if(m_commonPayload)
        {
            m_commonPayload();
        }
        m_menuItems[m_highlightedIndex]->runPayload();
    }
}

