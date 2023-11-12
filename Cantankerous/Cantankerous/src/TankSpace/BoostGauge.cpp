#include "BoostGauge.h"

const sf::Color DEFAULT_GOOD_COLOUR       = sf::Color::Yellow;
const sf::Color DEFAULT_LOW_COLOUR        = sf::Color::Red;
const sf::Color DEFAULT_BACKGROUND_COLOUR = sf::Color::Black;

BoostGauge::BoostGauge(
	
)

{
	m_gauge = std::make_shared<fw::GaugeComponent>(
		this,
		DEFAULT_GOOD_COLOUR,
		DEFAULT_BACKGROUND_COLOUR,
		fw::Vec2f(100.f, 20.f),
		fw::Vec2f(200.f, 20.f)
	);

	m_gauge->addColourThreshold(0.3f, DEFAULT_LOW_COLOUR);

	addComponent(m_gauge);
}

void BoostGauge::update(const fw::Vec2f& position, float health)
{
	m_gauge->update(position, health);
}


void BoostGauge::update(float health)
{
	m_gauge->update(health);
}
