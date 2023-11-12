#pragma once

#include <FlatWhite.h>

class BoostGauge : public fw::GameObject
{
public:
	BoostGauge();

	virtual void update(const fw::Vec2f& position, float health);
	virtual void update(float health);

private:
	std::shared_ptr<fw::GaugeComponent> m_gauge;

};

