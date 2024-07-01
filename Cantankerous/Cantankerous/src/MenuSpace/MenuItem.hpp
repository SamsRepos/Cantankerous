#include <FlatWhite.hpp>

class MenuItem : public fw::GameObject
{
public:
    MenuItem(std::string text, fw::Vec2f position);
    MenuItem(
        std::string text, 
        fw::Vec2f position, 
        fw::Colour defaultColour, 
        fw::Colour highlightedColour,
        fw::OriginPoints originPoint
    );

    void setHighlighted(bool highlighted);

private:
    std::shared_ptr<fw::TextComponent> m_text;
    fw::Font m_font;

    bool m_highlighted;

    fw::Colour m_defaultColour;
    fw::Colour m_highlightedColour;
};