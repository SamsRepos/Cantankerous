#include "RechargeGauge.hpp"

const float RC_GAUGE_LERP_AMOUNT_PER_SECOND = 91.f;

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
	SmoothGauge(
		initPosition,
		size,
		initialLevel,
		goodColour,
		RC_GAUGE_LERP_AMOUNT_PER_SECOND
	),
	m_playerTank(playerTank)
{
	m_gauge->addColourThreshold(threshold, lowColour);
}