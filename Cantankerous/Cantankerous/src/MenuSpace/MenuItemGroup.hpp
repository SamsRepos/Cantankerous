#include <FlatWhite.hpp>

class MenuItem;

enum MenuInputDirectionFlags
{
    HORIZONTAL = 1 << 0,
    VERTICAL   = 1 << 1
};
inline MenuInputDirectionFlags operator|(MenuInputDirectionFlags a, MenuInputDirectionFlags b)
{
    return static_cast<MenuInputDirectionFlags>(static_cast<int>(a) | static_cast<int>(b));
}

enum MenuInputTypeFlags
{
    KEYBOARD = 1 << 0,
    XBOX     = 1 << 1,
    MOUSE    = 1 << 2
};
inline MenuInputTypeFlags operator|(MenuInputTypeFlags a, MenuInputTypeFlags b)
{
    return static_cast<MenuInputTypeFlags>(static_cast<int>(a) | static_cast<int>(b));
}

class MenuItemGroup : public fw::GameObject
{
public:
    MenuItemGroup(
        std::vector<std::shared_ptr<MenuItem>> menuItems,
        MenuInputDirectionFlags inputDirections,
        MenuInputTypeFlags inputTypes
    );
    
    void handleInput(const fw::Input& input);

private:
    std::vector<std::shared_ptr<MenuItem>> m_menuItems;
    MenuInputDirectionFlags m_inputDirections;
    MenuInputTypeFlags m_inputTypes;

    size_t m_highlightedIndex;
    const size_t M_MENU_ITEMS_MAX_INDEX;
};