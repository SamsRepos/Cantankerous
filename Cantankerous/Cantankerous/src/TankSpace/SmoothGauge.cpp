#include "SmoothGauge.hpp"

const sf::Color DEFAULT_GAUGE_BACKGROUND_COLOUR = sf::Color::Black;

SmoothGauge::SmoothGauge(
	const fw::Vec2f& initPosition,
	const fw::Vec2f& size,
	float initialLevel,
	const fw::Colour& fullColour,
	float lerpAmountPerSecond
)
	:
	m_targetLevel(initialLevel),
	m_currentLevel(initialLevel),
	m_lerpAmountPerSecond(lerpAmountPerSecond)
{
	m_gauge = std::make_shared<fw::GaugeComponent>(
		this,
		fullColour,
		DEFAULT_GAUGE_BACKGROUND_COLOUR,
		initPosition,
		size
	);

	addComponent(m_gauge);
}

void SmoothGauge::update(const float& deltaTime)
{
	GameObject::update(deltaTime);

	float t = fw::util::clamp(
		0.f,
		m_lerpAmountPerSecond * deltaTime,
		1.f
	);

	m_currentLevel = fw::util::lerp(m_currentLevel, m_targetLevel, t);

	m_gauge->updateHealth(m_currentLevel);
}

void SmoothGauge::updatePosition(const fw::Vec2f& position)
{
	m_gauge->updatePosition(position);
}

void SmoothGauge::updateHealth(const float& health)
{
	m_targetLevel = health;
}