#include "GunGauge.hpp"

#include "PlayerTank.hpp"

const sf::Color DEFAULT_GOOD_COLOUR = sf::Color::Yellow;
const sf::Color DEFAULT_LOW_COLOUR = sf::Color::Red;

GunGauge::GunGauge(std::shared_ptr<PlayerTank> playerTank)
	:
	RechargeGauge(
		playerTank,
		DEFAULT_GOOD_COLOUR,
		DEFAULT_LOW_COLOUR,
		PLAYERTANK_BOOST_COST,
		fw::Vec2f(1000.f, 20.f),
		fw::Vec2f(200.f, 20.f)
	)
{
}

void GunGauge::update(const float& deltaTime)
{
	updateHealth(m_playerTank->getGunCharge());
}