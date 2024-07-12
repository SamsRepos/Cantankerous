#pragma once

#include "RechargeGauge.hpp"

class GunGauge : public RechargeGauge
{
public:
	GunGauge(std::shared_ptr<PlayerTank> playerTank, const fw::Font& font);

protected:
	virtual void update(const float& deltaTime);

private:
};

