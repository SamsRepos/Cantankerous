#include "BoostGauge.hpp"

#include "PlayerTank.hpp"

const fw::Colour DEFAULT_GOOD_COLOUR = fw::Colour::Yellow;
const fw::Colour DEFAULT_LOW_COLOUR  = fw::Colour::Red;

const float INITIAL_LEVEL = 1.f;

const float DIST_FROM_SIDE = 300.f;

const fw::Vec2f BOOST_GAUGE_SIZE     = fw::Vec2f(250.f, 20.f);
const fw::Vec2f BOOST_GAUGE_POSITION = fw::Vec2f(DIST_FROM_SIDE, 20.f);

const fw::Vec2f LABEL_OFFSET = fw::Vec2f(-10.f, 0.f);

BoostGauge::BoostGauge(std::shared_ptr<PlayerTank> playerTank, const fw::Font& font)
	:
	RechargeGauge(
		playerTank,
		DEFAULT_GOOD_COLOUR,
		DEFAULT_LOW_COLOUR,
		PLAYERTANK_BOOST_COST,
		BOOST_GAUGE_POSITION,
		BOOST_GAUGE_SIZE,
		INITIAL_LEVEL
	)
{
	addComponent(
		std::make_shared<fw::TextComponent>(
			this,
			font,
			fw::Colour::White,
			BOOST_GAUGE_POSITION + LABEL_OFFSET,
			"BOOST",
			fw::OriginPoints::CENTRE_RIGHT
		)
	);
}

//
// PROTECTED:
//

void BoostGauge::update(const float& deltaTime)
{
	updateHealth(m_playerTank->getBoostCharge());

	RechargeGauge::update(deltaTime);
}

