#pragma once

#include <FlatWhite.h>

class PlayerTank;
class BoostGauge;
class EnemySpawner;

class TankSpace : public fw::PhysicsSpace
{
public:
	TankSpace(const fw::Vec2f& windowSize);

	virtual void update(float deltaTime);
	virtual void render(fw::RenderTarget* window);

private:
	std::shared_ptr<PlayerTank> m_playerTank;
	std::shared_ptr<BoostGauge> m_boostGauge;
	std::shared_ptr<EnemySpawner> m_enemySpawner;

	fw::TextureManager texManager;
};
