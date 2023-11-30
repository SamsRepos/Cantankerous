#pragma once

#include <FlatWhite.hpp>

class SmoothGauge : public fw::GameObject
{
public:
	SmoothGauge(
		const fw::Vec2f& initPosition,
		const fw::Vec2f& size,
		float initialLevel,
		const fw::Colour& fullColour,
		float lerpAmountPerSecond
	);

	virtual void update(const float& deltaTime);

	virtual void updatePosition(const fw::Vec2f& position);
	void updateHealth(const float& health);

protected:
	std::shared_ptr<fw::GaugeComponent> m_gauge;

	float m_targetLevel; // based on actual health
	float m_currentLevel; // a little behind, lerps towards m_targetLevel, for smoother changes

	float m_lerpAmountPerSecond;
};

