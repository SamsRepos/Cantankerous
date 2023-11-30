#include "RechargeGauge.hpp"

const sf::Color DEFAULT_RCGAUGE_BACKGROUND_COLOUR = sf::Color::Black;

RechargeGauge::RechargeGauge(
	std::shared_ptr<PlayerTank> playerTank,
	fw::Colour goodColour,
	fw::Colour lowColour,
	float threshold,
	fw::Vec2f initPosition,
	fw::Vec2f size,
	float initialLevel
)
	:
	m_playerTank(playerTank),
	m_targetLevel(initialLevel),
	m_currentLevel(initialLevel)
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

void RechargeGauge::update(const float& deltaTime)
{
	GameObject::update(deltaTime);

	const static float LERP_AMOUNT_PER_SECOND = 91.f;
	float t = fw::util::clamp(
		0.f, 
		LERP_AMOUNT_PER_SECOND * deltaTime, 
		1.f
	);

	m_currentLevel = fw::util::lerp(m_currentLevel, m_targetLevel, t);

	m_gauge->updateHealth(m_currentLevel);
}

void RechargeGauge::updatePosition(const fw::Vec2f& position)
{
	m_gauge->updatePosition(position);
}

void RechargeGauge::updateHealth(const float& health)
{
	m_targetLevel = health;
}