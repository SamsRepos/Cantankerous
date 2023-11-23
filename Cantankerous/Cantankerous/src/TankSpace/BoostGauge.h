#pragma once

#include <FlatWhite.h>

class BoostGauge : public fw::GameObject
{
public:
	BoostGauge();

	void updatePosition(const fw::Vec2f& position);
	void updateHealth(const float& health);

private:
	std::shared_ptr<fw::GaugeComponent> m_gauge;

};

