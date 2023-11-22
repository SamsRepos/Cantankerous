#include "EnemyTank.h"

#include "PlayerTank.h"
#include "Gate.h"
#include "Wall.h"

const float ENEMY_TANK_ROAMING_TIME_COEFF      = 3.f;
const float ENEMY_TANK_TARGETING_TIME_SHORTEST = 1.1f;
const float ENEMY_TANK_TARGETING_TIME_LONGEST  = 3.3f;

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
	fw::Rectangle gameBoundsRect,
	std::vector<fw::LineSegment> gameBoundsLines,
	std::shared_ptr<Difficulty> difficulty,
	GameObject* parentForSpawnedMissiles,
	SparkEmitter* sparkEmitter
)
	:
	Tank(
		tankTexture,
		cannonTexture,
		missileTexture,
		world,
		spawningGate->getSpawnPos(),
		fw::util::directionToAngle(spawningGate->getDirectionToGameSpace()),
		pixelsPerMetre,
		parentForSpawnedMissiles,
		sparkEmitter
	),
	m_world(world),
	m_state(EnemyTankState::Nascent),
	m_direction(spawningGate->getDirectionToGameSpace()),
	m_playerTank(playerTank),
	m_enemyTanks(enemyTanks),
	m_gates(gates),
	m_gameBoundsRect(gameBoundsRect),
	m_gameBoundsLines(gameBoundsLines),
	m_spawningGate(spawningGate),
	m_difficulty(difficulty)
{
	m_tankSprite->setTint(fw::Colour::Red);

	m_lineComponent = std::make_shared<fw::LineComponent>(this);
	addComponent(m_lineComponent);
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

	if (m_state == EnemyTankState::Nascent|| m_state == EnemyTankState::Roaming)
	{
		updateTankRotation(m_direction);
		fw::Vec2f dirToPlayer = m_playerTank->getPosition() - getPosition();
		m_cannonSprite->setRotation(fw::util::directionToAngle(dirToPlayer));
	}

	if (m_state == EnemyTankState::Roaming || m_state == EnemyTankState::Targeting)
	{
		m_timeToStateChange -= deltaTime;
	}

	Tank::update(deltaTime);

	
}

void EnemyTank::render(fw::RenderTarget* window)
{
	Tank::render(window);

	
}

void EnemyTank::collisionResponse(fw::GameObject* other)
{
	Tank::collisionResponse(other);
}

//
//  PRIVATE:
//

void EnemyTank::updateNascent()
{
	m_body->setLinearVelocity(m_direction.normalised() * TANK_NORMAL_SPEED);

	if (!(m_spawningGate->getSpawnArea().intersects(m_tankSprite->getGlobalBounds())))
	{
		transitionToRoaming();
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
	auto repulsionFromObj = [&](GameObject* object)
	{
		fw::Vec2f directionToObj = object->getPosition() - getPosition();
		float repulsionMagnitude = 1.f / directionToObj.magnitude();
		return -directionToObj.normalised() * repulsionMagnitude;
	};

	auto repulsionFromLine = [&](const fw::LineSegment& lineSegment)
	{
		fw::Vec2f directionFromLine = lineSegment.getShortestDirectionToPoint(getPosition());
		float repulsionMagnitude = 1.f / directionFromLine.magnitude();
		return directionFromLine.normalised() * repulsionMagnitude;
	};
	
	fw::Vec2f repulsion(0.f);

	repulsion += repulsionFromObj(m_playerTank.get());

	for (auto& enemyTank : *m_enemyTanks)
	{
		if(enemyTank.get() == this) continue;

		repulsion += repulsionFromObj(enemyTank.get());
	}

	for (auto& wall : m_gameBoundsLines)
	{
		repulsion += repulsionFromLine(wall);
	}

	m_direction = repulsion;

	m_body->setLinearVelocity(repulsion.normalised() * TANK_NORMAL_SPEED);

	if (m_timeToStateChange <= 0.f)
	{
		bool hitsPlayerTank;
		rayCastFromCannon(&hitsPlayerTank);
		if(hitsPlayerTank)
		{
			transitionToTargeting();
		}
	}
}

void EnemyTank::updateTargeting()
{
	m_lineComponent->clear();
	m_lineComponent->addLineSegment(rayCastFromCannon());

	if (m_timeToStateChange <= 0.f)
	{
		float cannonAngle = m_cannonSprite->getRotation();
		fw::Vec2f missileDir = fw::util::angleToDirection(cannonAngle) ;
		assert(!missileDir.isZero());

		fireMissile(missileDir);

		transitionToRoaming();
	}
}

void EnemyTank::transitionToRoaming()
{
	m_lineComponent->clear();

	m_timeToStateChange = ENEMY_TANK_ROAMING_TIME_COEFF;

	m_state = EnemyTankState::Roaming;
}

void EnemyTank::transitionToTargeting()
{
	m_body->setLinearVelocity(fw::Vec2f(0.f));
	m_body->setAngularVelocity(0.f);

	m_timeToStateChange = ENEMY_TANK_TARGETING_TIME_LONGEST;

	m_state = EnemyTankState::Targeting;
}

fw::LineSegment EnemyTank::rayCastFromCannon(bool* hitsPlayerTank)
{
	if(hitsPlayerTank) *hitsPlayerTank = false;

	fw::Vec2f direction = fw::util::angleToDirection(m_cannonSprite->getRotation());

	fw::Vec2f laserStartPt = getPosition() + direction * (m_cannonSprite->getSize().y / 2.f);

	fw::Vec2f laserEndPt = laserStartPt;
	while (m_gameBoundsRect.contains(laserEndPt))
	{
		if (m_playerTank->getTankSprite()->contains(laserEndPt))
		{
			if (hitsPlayerTank) *hitsPlayerTank = true;
			return fw::LineSegment(laserStartPt, laserEndPt);
		}
		for (const std::shared_ptr<fw::GameObject>& enemyTank : *m_enemyTanks)
		{
			if (enemyTank.get() == this) continue;

			if (fw::util::isType<GameObject, Tank>(enemyTank))
			{
				if (std::reinterpret_pointer_cast<Tank>(enemyTank)->getTankSprite()->contains(laserEndPt))
				{
					return fw::LineSegment(laserStartPt, laserEndPt);
				}
			}
		}

		laserEndPt += direction;
	}

	return fw::LineSegment(laserStartPt, laserEndPt);
}