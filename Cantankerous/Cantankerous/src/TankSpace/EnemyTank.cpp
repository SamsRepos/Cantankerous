#include "EnemyTank.h"

EnemyTank::EnemyTank(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	fw::World* world,
	fw::Vec2f initPos,
	int pixelsPerMetre,
	std::shared_ptr<PlayerTank> playerTank
)
	:
	Tank(
		tankTexture,
		cannonTexture,
		missileTexture,
		world,
		initPos,
		pixelsPerMetre
	),
	m_state(EnemyTankState::Nascent),
	m_playerTank(playerTank)
{
	m_tankSprite->setTint(fw::Colour::Red);
}

void EnemyTank::update(float deltaTime)
{
	switch(m_state)
	{
	case(EnemyTankState::Nascent):
	{
		updateNascent();
	}
	break;
	case(EnemyTankState::Roaming):
	{
		updateRoaming();
	}
	break;
	case(EnemyTankState::Targeting):
	{
		updateTargeting();
	}
	break;
	}
}

void EnemyTank::collisionResponse(fw::GameObject* other)
{

}

//
//  PRIVATE:
//

void EnemyTank::updateNascent()
{

}

void EnemyTank::updateRoaming()
{

}

void EnemyTank::updateTargeting()
{

}
