#include "MenuItem.hpp"

const fw::Colour MENU_ITEM_DEFAULT_NORMAL_COLOUR     = fw::Colour::White;
const fw::Colour MENU_ITEM_DEFAULT_HIGHLIGHTED_COLOUR = fw::Colour::Magenta;

const fw::OriginPoints MENU_ITEM_DEFAULT_ORIGIN = fw::OriginPoints::CENTRE;

MenuItem::MenuItem(std::string text, fw::Vec2f position, std::function<void()> payload)
    : 
    MenuItem(
        text,
        position,
        payload,
        MENU_ITEM_DEFAULT_NORMAL_COLOUR,
        MENU_ITEM_DEFAULT_HIGHLIGHTED_COLOUR,
        MENU_ITEM_DEFAULT_ORIGIN
    )
{
}

MenuItem::MenuItem(
    std::string text, 
    fw::Vec2f position, 
    std::function<void()> payload,
    fw::Colour defaultColour, 
    fw::Colour highlightedColour,
    fw::OriginPoints originPoint
)
    :
    m_highlighted(false),
    m_payload(payload)
{
    m_font.loadFromFile("font/arial.ttf");

    m_defaultColour = defaultColour;

    m_highlightedColour = highlightedColour;

    m_text = std::make_shared<fw::TextComponent>(
        this,
        m_font,
        m_defaultColour,
        position,
        text
    );

    m_text->setOrigin(originPoint);

    addComponent(m_text);

}

void MenuItem::setHighlighted(bool highlighted)
{
    m_highlighted = highlighted;

    m_text->setColour(highlighted ? m_highlightedColour : m_defaultColour);
}

void MenuItem::runPayload()
{
    m_payload();
}
