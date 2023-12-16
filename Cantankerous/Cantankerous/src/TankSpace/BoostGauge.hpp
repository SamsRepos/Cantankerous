#pragma once

#include "RechargeGauge.hpp"

class BoostGauge : public RechargeGauge
{
public:
	BoostGauge(std::shared_ptr<PlayerTank> playerTank);

protected:
	virtual void update(const float& deltaTime);
};

