#include "HealthGauge.hpp"

const fw::Vec2f HEALTH_GAUGE_POSITION_OFFSET = fw::Vec2f::unitUp() * 25.5f;
const fw::Vec2f HEALTH_GAUGE_SIZE            = fw::Vec2f(69.f, 8.f);

const fw::Colour FULL_HEALTH_COLOUR = fw::Colour::Green;

const float HEALTH_GAUGE_LERP_PER_SECOND = 16.f;

HealthGauge::HealthGauge(const fw::Vec2f& initPosition)
	:
	SmoothGauge(
		initPosition,
		HEALTH_GAUGE_SIZE,
		1.f,
		FULL_HEALTH_COLOUR,
		HEALTH_GAUGE_LERP_PER_SECOND
	)
{
	m_gauge->addColourThreshold(0.67f, fw::Colour::Yellow);
	m_gauge->addColourThreshold(0.33f, fw::Colour::Red);

}

void HealthGauge::updatePosition(const fw::Vec2f& position)
{
	m_gauge->updatePosition(position + HEALTH_GAUGE_POSITION_OFFSET);
}