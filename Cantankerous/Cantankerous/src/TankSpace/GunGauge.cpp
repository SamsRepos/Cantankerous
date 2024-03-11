#include "GunGauge.hpp"

#include "PlayerTank.hpp"

const fw::Colour DEFAULT_GOOD_COLOUR = fw::Colour::Yellow;
const fw::Colour DEFAULT_LOW_COLOUR = fw::Colour::Red;

const float INITIAL_LEVEL = 1.f;

GunGauge::GunGauge(std::shared_ptr<PlayerTank> playerTank)
	:
	RechargeGauge(
		playerTank,
		DEFAULT_GOOD_COLOUR,
		DEFAULT_LOW_COLOUR,
		PLAYERTANK_BOOST_COST,
		fw::Vec2f(1000.f, 20.f),
		fw::Vec2f(200.f, 20.f),
		INITIAL_LEVEL
	)
{
}

//
// PROTECTED:
//

void GunGauge::update(const float& deltaTime)
{
	updateHealth(m_playerTank->getGunCharge());

	RechargeGauge::update(deltaTime);
}