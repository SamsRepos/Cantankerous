#include "EnemyTank.h"

#include "PlayerTank.h"
#include "Gate.h"
#include "Wall.h"

const float ENEMY_TANK_HEADING_CHANGE_TIME_SHORTEST = 1.2f;
const float ENEMY_TANK_HEADING_CHANGE_TIME_LONGEST  = 7.2f;

const float ENEMY_TANK_ROAMING_TIME_SHORTEST = 2.6f;
const float ENEMY_TANK_ROAMING_TIME_LONGEST  = 6.8f;

const float ENEMY_TANK_TARGETING_TIME_SHORTEST = 1.1f;
const float ENEMY_TANK_TARGETING_TIME_LONGEST  = 3.3f;

const float REPULSION_HORIZON_PIXELS = 160.f;
const float REPULSION_THRESHOLD      = 1.7f;

const fw::Colour LASER_INIT_COLOUR   = fw::Colour::Green;
const fw::Colour LASER_FIRING_COLOUR = fw::Colour::Magenta;

EnemyTank::EnemyTank(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	fw::PhysicsSpace* physicsSpace,
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
		physicsSpace,
		spawningGate->getSpawnPos(),
		fw::util::directionToAngle(spawningGate->getDirectionToGameSpace()),
		pixelsPerMetre,
		parentForSpawnedMissiles,
		sparkEmitter
	),
	m_physicsSpace(physicsSpace),
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

void EnemyTank::update(const float& deltaTime)
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
		updateRoaming(deltaTime);
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

void EnemyTank::updateRoaming(const float& deltaTime)
{
	m_timeToHeadingChange -= deltaTime;

	fw::Vec2f repulsion = getRepulsion();

	if ((m_timeToHeadingChange <= 0.f) || repulsion.magnitude() > REPULSION_THRESHOLD)
	{
		resetHeading();
	}

	m_direction = m_heading.normalised() * TANK_NORMAL_SPEED;

	m_body->setLinearVelocity(m_direction);

	if (m_timeToStateChange <= 0.f)
	{
		bool hitsPlayerTank;
		rayCastFromCannon(&hitsPlayerTank);
		if(hitsPlayerTank)
		{
			transitionToTargeting();
		}
		else // wanting to stop EnemyTanks targeting player the instant another enemy moves out of the way - it can get wonky
		{
			m_timeToStateChange = ENEMY_TANK_TARGETING_TIME_LONGEST;
		}
	}
}

void EnemyTank::updateTargeting()
{
	m_lineComponent->updateLineSegment(0, rayCastFromCannon());

	m_body->setLinearVelocity(fw::Vec2f(0.f));
	m_body->setAngularVelocity(0.f);

	float t = fw::util::inverseLerp(0.f, m_timeTargeting, m_timeToStateChange);

	float r = fw::util::lerp(LASER_FIRING_COLOUR.r, LASER_INIT_COLOUR.r, t);
	float g = fw::util::lerp(LASER_FIRING_COLOUR.g, LASER_INIT_COLOUR.g, t);
	float b = fw::util::lerp(LASER_FIRING_COLOUR.b, LASER_INIT_COLOUR.b, t);
	fw::Colour laserColour(r, g, b);
	m_lineComponent->setColour(laserColour);

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
	m_state = EnemyTankState::Roaming;

	m_lineComponent->setInvisible(); // clear();

	resetHeading();

	m_timeToStateChange = ENEMY_TANK_ROAMING_TIME_LONGEST;

}

void EnemyTank::transitionToTargeting()
{
	m_state = EnemyTankState::Targeting;

	m_lineComponent->setVisible(); // addLineSegment(rayCastFromCannon());
	m_lineComponent->setColour(LASER_INIT_COLOUR);

	//m_body->setLinearVelocity(fw::Vec2f(0.f));
	//m_body->setAngularVelocity(0.f);

	m_timeToStateChange = m_timeTargeting = ENEMY_TANK_TARGETING_TIME_LONGEST;

}

void EnemyTank::resetHeading()
{
	//m_heading = (fw::util::randomUnitVec2f() + (getRepulsion() * ENEMY_TANK_REPULSION_COEFF));
	fw::Vec2f repulsion = getRepulsion();
	if(repulsion.magnitude() > REPULSION_THRESHOLD)
	{
		m_heading = repulsion;
	}
	else
	{
		m_heading = fw::util::randomUnitVec2f();
	}
	
	m_heading.normalise();

	m_timeToHeadingChange = ENEMY_TANK_HEADING_CHANGE_TIME_LONGEST;
}

fw::Vec2f EnemyTank::getRepulsion()
{
	const static float REPULSION_FROM_PLAYERTANK_WEIGHT = 1.f;
	const static float REPULSION_FROM_ENEMYTANKS_WEIGHT = 2.f;
	const static float REPULSION_FROM_WALLS_WEIGHT      = 15.f;

	// for now, make repulsion from other gameObjects weighted by the reciprocal of distance	
	auto repulsionFromObj = [&](GameObject* object)
	{
		fw::Vec2f directionFromObj = getPosition() - object->getPosition();
		float magnitude = directionFromObj.magnitude();
		if(magnitude > REPULSION_HORIZON_PIXELS) return fw::Vec2f(0.f);
		float repulsionMagnitude = 1.f / magnitude;
		return fw::Vec2f(directionFromObj.normalised() * repulsionMagnitude);
	};

	auto repulsionFromLine = [&](const fw::LineSegment& lineSegment)
	{
		fw::Vec2f directionFromLine = lineSegment.getShortestDirectionToPoint(getPosition());
		float magnitude = directionFromLine.magnitude();
		if (magnitude > REPULSION_HORIZON_PIXELS) return fw::Vec2f(0.f);
		float repulsionMagnitude = 1.f / magnitude;
		return fw::Vec2f(directionFromLine.normalised() * repulsionMagnitude);
	};

	fw::Vec2f repulsion(0.f);

	repulsion += repulsionFromObj(m_playerTank.get());

	for (auto& enemyTank : *m_enemyTanks)
	{
		if (enemyTank.get() == this) continue;
		repulsion += (repulsionFromObj(enemyTank.get()) * REPULSION_FROM_ENEMYTANKS_WEIGHT);
	}

	for (auto& wall : m_gameBoundsLines)
	{
		repulsion += (repulsionFromLine(wall) * REPULSION_FROM_WALLS_WEIGHT);
	}

	return repulsion;
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
