#pragma once

#include "Tank.h"

class PlayerTank;

enum class EnemyTankState
{
	Nascent,
	Roaming,
	Targeting
};

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

