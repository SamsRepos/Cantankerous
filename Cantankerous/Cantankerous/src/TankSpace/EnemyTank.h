#pragma once

#include "Tank.h"

class PlayerTank;

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

// roaming behaviour:
// move away from other enemy tanks and player tank
// move away from nearby world boundaries
// stronger repulsion the closer it gets
// otherwise, move in a random direction, which can change randomly

class EnemyTank : public Tank
{
public:
	EnemyTank(
		std::shared_ptr<fw::Texture> tankTexture,
		std::shared_ptr<fw::Texture> cannonTexture,
		std::shared_ptr<fw::Texture> missileTexture,
		fw::World* world,
		fw::Vec2f initPos,
		int pixelsPerMetre,
		std::shared_ptr<PlayerTank> playerTank
	);

	virtual void update(float deltaTime);

	virtual void collisionResponse(fw::GameObject* other);

	inline EnemyTankState getState() { return m_state; };

private:
	EnemyTankState m_state;
	std::shared_ptr<PlayerTank> m_playerTank;

	// update sub functions
	void updateNascent();
	void updateRoaming();
	void updateTargeting();
};

