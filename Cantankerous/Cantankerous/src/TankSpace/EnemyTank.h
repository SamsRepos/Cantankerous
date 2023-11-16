#pragma once

#include "Tank.h"

class PlayerTank;
class Gate;
class Wall;
class Difficulty;

enum class EnemyTankState
{
	Nascent,
	Roaming,
	Targeting
};

struct EnemySpawnPoint
{
	fw::Vec2f spawnPosition;
	fw::Vec2f directionToGameWorld;
};

class EnemyTank : public Tank
{
public:
	EnemyTank(
		std::shared_ptr<fw::Texture> tankTexture,
		std::shared_ptr<fw::Texture> cannonTexture,
		std::shared_ptr<fw::Texture> missileTexture,
		fw::World* world,
		Gate* spawningGate,
		int pixelsPerMetre,
		std::shared_ptr<PlayerTank> playerTank,
		std::list<std::shared_ptr<GameObject>>* enemyTanks,
		std::vector<std::shared_ptr<Gate>>* gates,
		std::vector<std::shared_ptr<Wall>>* walls,
		std::shared_ptr<Difficulty> difficulty
	);

	virtual void update(float deltaTime);

	virtual void collisionResponse(fw::GameObject* other);

	inline EnemyTankState getState() { return m_state; };

private:
	EnemyTankState m_state;
	fw::Vec2f m_direction;

	std::shared_ptr<PlayerTank> m_playerTank;
	std::list<std::shared_ptr<GameObject>>* m_enemyTanks;
	std::vector<std::shared_ptr<Gate>>* m_gates;
	std::vector<std::shared_ptr<Wall>>* m_walls;

	Gate* m_spawningGate;

	// update sub functions
	void updateNascent();
	void updateRoaming();
	void updateTargeting();
};

