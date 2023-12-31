#include "BoostGauge.hpp"

#include "PlayerTank.hpp"

const sf::Color DEFAULT_GOOD_COLOUR       = sf::Color::Yellow;
const sf::Color DEFAULT_LOW_COLOUR        = sf::Color::Red;

const float INITIAL_LEVEL = 1.f;

BoostGauge::BoostGauge(std::shared_ptr<PlayerTank> playerTank)
	:
	RechargeGauge(
		playerTank,
		DEFAULT_GOOD_COLOUR,
		DEFAULT_LOW_COLOUR,
		PLAYERTANK_BOOST_COST,
		fw::Vec2f(100.f, 20.f),
		fw::Vec2f(200.f, 20.f),
		INITIAL_LEVEL
	)
{
}

//
// PROTECTED:
//

void BoostGauge::update(const float& deltaTime)
{
	updateHealth(m_playerTank->getBoostCharge());

	RechargeGauge::update(deltaTime);
}

