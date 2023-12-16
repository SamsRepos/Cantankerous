#pragma once

#include "RechargeGauge.hpp"

class GunGauge : public RechargeGauge
{
public:
	GunGauge(std::shared_ptr<PlayerTank> playerTank);

protected:
	virtual void update(const float& deltaTime);

private:
};

