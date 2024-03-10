#pragma once

#include <FlatWhite.hpp>

class PlayerTank;
class BoostGauge;
class EnemySpawner;
class SparkEmitter;
class Score;
class Difficulty;
enum class DifficultySetting;

class TankSpace : public fw::PhysicsSpace
{
public:
	TankSpace(const fw::Vec2f& windowSize, DifficultySetting difficultySetting);

protected:
	virtual void handleInput(const fw::Input& input);
	virtual void update(const float& deltaTime);

private:
	void initWallsAndGates();

	bool m_paused;

	std::shared_ptr<Score>        m_score;
	std::shared_ptr<Difficulty>   m_difficulty;
	std::shared_ptr<SparkEmitter> m_sparkEmitter;
	std::shared_ptr<PlayerTank>   m_playerTank;
	std::shared_ptr<BoostGauge>   m_boostGauge;
	std::shared_ptr<EnemySpawner> m_enemySpawner;

	fw::TextureManager m_texManager;
	
	fw::Vec2f m_windowSize;
};
