#pragma once

#include "SmoothGauge.hpp"

class HealthGauge : public SmoothGauge
{
public:
	HealthGauge(const fw::Vec2f& initPosition);

	virtual void updatePosition(const fw::Vec2f& position);
};