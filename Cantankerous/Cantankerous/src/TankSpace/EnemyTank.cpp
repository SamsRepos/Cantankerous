#include "EnemyTank.h"

EnemyTank::EnemyTank(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	fw::World* world,
	fw::Vec2f initialPosition,
	fw::Vec2f initialDirection,
	int pixelsPerMetre,
	std::shared_ptr<PlayerTank> playerTank,
	std::shared_ptr<Difficulty> difficulty
)
	:
	Tank(
		tankTexture,
		cannonTexture,
		missileTexture,
		world,
		initialPosition,
		fw::util::directionToAngle(initialDirection),
		pixelsPerMetre
	),
	m_state(EnemyTankState::Nascent),
	m_direction(initialDirection),
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

	Tank::update(deltaTime);
}

void EnemyTank::collisionResponse(fw::GameObject* other)
{

}

//
//  PRIVATE:
//

void EnemyTank::updateNascent()
{
	m_body->setLinearVelocity(m_direction.normalised() * TANK_NORMAL_SPEED);
}

void EnemyTank::updateRoaming()
{

}

void EnemyTank::updateTargeting()
{

}
