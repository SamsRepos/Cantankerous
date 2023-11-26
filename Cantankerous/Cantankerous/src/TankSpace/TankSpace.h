#pragma once

#include <FlatWhite.h>

class Difficulty;
class PlayerTank;
class BoostGauge;
class EnemySpawner;
class SparkEmitter;

class TankSpace : public fw::PhysicsSpace
{
public:
	TankSpace(const fw::Vec2f& windowSize, std::shared_ptr<Difficulty> difficulty);

	virtual void update(const float& deltaTime);
	virtual void render(fw::RenderTarget* window);

private:
	void initWallsAndGates();

	std::shared_ptr<Difficulty> m_difficulty;

	std::shared_ptr<SparkEmitter> m_sparkEmitter;
	std::shared_ptr<PlayerTank> m_playerTank;
	std::shared_ptr<BoostGauge> m_boostGauge;
	std::shared_ptr<EnemySpawner> m_enemySpawner;

	fw::TextureManager m_texManager;
	
	fw::Vec2f m_windowSize;
};
