#include "Score.hpp"

const fw::Colour SCORE_TXT_COLOUR   = fw::Colour::Green;
const fw::Vec2f  SCORE_TXT_POSITION = fw::Vec2f(350.f, 10.f);

Score::Score()
	:
	GameObject(SCORE_TXT_POSITION),
	m_score(0)
{
	m_font.loadFromFile("font/arial.ttf");

	m_text = std::make_shared<fw::TextComponent>(
		this,
		m_font,
		SCORE_TXT_COLOUR,
		SCORE_TXT_POSITION,
		scoreString()
	);
	m_text->setCharacterSize(20);
	addComponent(m_text);
}

std::string Score::scoreString()
{
	return "SCORE: " + std::to_string(m_score);
}
