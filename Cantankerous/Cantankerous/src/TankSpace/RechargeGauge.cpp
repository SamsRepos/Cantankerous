#include "RechargeGauge.hpp"

const sf::Color DEFAULT_RCGAUGE_BACKGROUND_COLOUR = sf::Color::Black;

RechargeGauge::RechargeGauge(
	std::shared_ptr<PlayerTank> playerTank,
	fw::Colour goodColour,
	fw::Colour lowColour,
	float threshold,
	fw::Vec2f initPosition,
	fw::Vec2f size
)
	:
	m_playerTank(playerTank)
{
	m_gauge = std::make_shared<fw::GaugeComponent>(
		this,
		goodColour,
		DEFAULT_RCGAUGE_BACKGROUND_COLOUR,
		initPosition,
		size
	);

	m_gauge->addColourThreshold(threshold, lowColour);

	addComponent(m_gauge);
}

void RechargeGauge::updatePosition(const fw::Vec2f& position)
{
	m_gauge->updatePosition(position);
}

void RechargeGauge::updateHealth(const float& health)
{
	m_gauge->updateHealth(health);
}