#include "Score.hpp"

#include "../GlobalConsts.hpp"

const fw::Colour SCORE_TXT_COLOUR   = fw::Colour::White;
const fw::Vec2f  SCORE_TXT_POSITION = fw::Vec2f(650.f, 10.f);

Score::Score(
	const fw::Font& font,
	int defaultIncrementAmount, 
	int defaultDecrementAmount
)
	:
	GameObject(SCORE_TXT_POSITION),
	m_defaultIncrementAmount(defaultIncrementAmount),
	m_defaultDecrementAmount(defaultDecrementAmount),
	m_font(font)
{
	m_text = std::make_shared<fw::TextComponent>(
		this,
		m_font,
		SCORE_TXT_COLOUR,
		SCORE_TXT_POSITION,
		scoreString(fw::GlobalStore::getInt(GlobalConsts::SCORE_KEY))
	);
	//m_text->setCharacterSize(20);
	addComponent(m_text);
}

//
// PUBLIC:
//

void Score::increment()
{
	increment(m_defaultIncrementAmount);
}

void Score::increment(int amount)
{
	int score = fw::GlobalStore::getInt(GlobalConsts::SCORE_KEY);
	
	score += amount;

	fw::GlobalStore::setInt(GlobalConsts::SCORE_KEY, score);

	m_text->setContent(scoreString(score));
}

void Score::decrement()
{
	decrement(m_defaultDecrementAmount);
}

void Score::decrement(int amount)
{
	int score = fw::GlobalStore::getInt(GlobalConsts::SCORE_KEY);
	
	score -= amount;

	fw::GlobalStore::setInt(GlobalConsts::SCORE_KEY, score);

	m_text->setContent(scoreString(score));
}

//
// PRIVATE:
//

std::string Score::scoreString(int score)
{
	return "SCORE: " + std::to_string(score);
}
