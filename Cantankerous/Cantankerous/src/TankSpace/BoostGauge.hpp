#pragma once

#include "RechargeGauge.hpp"

class BoostGauge : public RechargeGauge
{
public:
	BoostGauge(std::shared_ptr<PlayerTank> playerTank);

	virtual void update(const float& deltaTime);

private:
};

