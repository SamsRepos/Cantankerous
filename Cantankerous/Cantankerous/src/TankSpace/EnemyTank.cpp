#include "EnemyTank.h"

#include "PlayerTank.h"
#include "Gate.h"
#include "Wall.h"

EnemyTank::EnemyTank(
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
)
	:
	Tank(
		tankTexture,
		cannonTexture,
		missileTexture,
		world,
		spawningGate->getSpawnPos(),
		fw::util::directionToAngle(spawningGate->getDirectionToGameSpace()),
		pixelsPerMetre
	),
	m_state(EnemyTankState::Nascent),
	m_direction(spawningGate->getDirectionToGameSpace()),
	m_playerTank(playerTank),
	m_enemyTanks(enemyTanks),
	m_gates(gates),
	m_walls(walls),
	m_spawningGate(spawningGate)
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

	if (!(m_spawningGate->getSpawnArea().contains(getPosition())))
	{
		m_state = EnemyTankState::Roaming;
	}
}

// roaming behaviour:
// move away from other enemy tanks and player tank
// move away from nearby world boundaries
// stronger repulsion the closer it gets
// otherwise, move in a random direction, which can change randomly

void EnemyTank::updateRoaming()
{
	// for now, make repulsion from other gameObjects weighted by the reciprocal of distance	
	auto repulsionFrom = [&](GameObject* object)
	{
		fw::Vec2f directionToObj = object->getPosition() - getPosition();
		float repulsionMagnitude = 1.f / directionToObj.magnitude();
		return -directionToObj.normalised() * repulsionMagnitude;
	};
	
	fw::Vec2f repulsion(0.f);

	repulsion += repulsionFrom(m_playerTank.get());

	for (auto& enemyTank : *m_enemyTanks)
	{
		if(enemyTank.get() == this) continue;

		repulsion += repulsionFrom(enemyTank.get());
	}

	for (auto& gate : *m_gates)
	{
		repulsion += repulsionFrom(gate.get());
	}

	for (auto& wall : *m_walls)
	{
		repulsion += repulsionFrom(wall.get());
	}

	m_body->setLinearVelocity(repulsion.normalised() * TANK_NORMAL_SPEED);
}

void EnemyTank::updateTargeting()
{

}
