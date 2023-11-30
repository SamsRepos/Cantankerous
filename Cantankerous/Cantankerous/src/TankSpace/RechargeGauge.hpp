#pragma once
#include <FlatWhite.hpp>

class PlayerTank;

class RechargeGauge : public fw::GameObject
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

	virtual void update(const float& deltaTime);

	void updatePosition(const fw::Vec2f& position);
	void updateHealth(const float& health);

protected:
	std::shared_ptr<fw::GaugeComponent> m_gauge;
	std::shared_ptr<PlayerTank> m_playerTank;

	float m_targetLevel; // based on actual health
	float m_currentLevel; // a little behind, lerps towards m_targetLevel, for smoother changes
};