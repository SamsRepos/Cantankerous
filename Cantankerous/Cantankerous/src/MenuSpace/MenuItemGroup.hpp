#include <FlatWhite.hpp>

class MenuItem;

class MenuItemGroup : public fw::GameObject
{
public:
    MenuItemGroup(
        std::vector<std::shared_ptr<MenuItem>> menuItems
    );
    
    void handleInput(const fw::Input& input);

private:
    std::vector<std::shared_ptr<MenuItem>> m_menuItems;

    size_t m_highlightedIndex;
};