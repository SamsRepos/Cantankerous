#include <FlatWhite.hpp>

class MenuItem : public fw::GameObject
{
public:
    MenuItem(std::string text, fw::Vec2f position, std::function<void()> payload);
    MenuItem(
        std::string text, 
        fw::Vec2f position, 
        std::function<void()> payload,
        fw::Colour defaultColour, 
        fw::Colour highlightedColour,
        fw::OriginPoints originPoint
    );

    void setHighlighted(bool highlighted);

    void runPayload();

private:
    std::shared_ptr<fw::TextComponent> m_text;
    fw::Font m_font;

    bool m_highlighted;

    fw::Colour m_defaultColour;
    fw::Colour m_highlightedColour;

    std::function<void()> m_payload;
};