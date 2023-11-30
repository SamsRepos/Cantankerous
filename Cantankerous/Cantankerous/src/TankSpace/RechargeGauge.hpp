#pragma once

#include "SmoothGauge.hpp"

class PlayerTank;

class RechargeGauge : public SmoothGauge
{
public:
	RechargeGauge(
		std::shared_ptr<PlayerTank> playerTank,
		fw::Colour goodColour,
		fw::Colour lowColour,
		float threshold,
		fw::Vec2f initPosition,
		fw::Vec2f size,
		float initialLevel
	);

protected:
	std::shared_ptr<PlayerTank> m_playerTank;
};