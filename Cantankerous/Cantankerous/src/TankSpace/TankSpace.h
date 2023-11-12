#pragma once

#include <FlatWhite.h>

#include "PlayerTank.h"
#include "BoostGauge.h"

class TankSpace : public fw::PhysicsSpace
{
public:
	TankSpace();

	virtual void update(float deltaTime);
	virtual void render(fw::RenderTarget* window);

private:
	std::shared_ptr<PlayerTank> m_tank;
	std::shared_ptr<BoostGauge> m_boostGauge;

	fw::TextureManager texManager;
};
