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
	m_playerTank(playerTank),
	m_enemyTanks(enemyTanks),
	m_gates(gates),
	m_gameBoundsRect(gameBoundsRect),
	m_gameBoundsLines(gameBoundsLines),
	m_spawningGate(spawningGate),
	m_difficulty(difficulty),
	m_laserLine(fw::Vec2f::zero(), fw::Vec2f::zero())
{
	setTankTint(fw::Colour::Red);

	updateTankDirection(spawningGate->getDirectionToGameSpace());

	m_lineComponent = std::make_shared<fw::LineComponent>(this);
	addComponent(m_lineComponent);
}

void EnemyTank::update(const float& deltaTime)
{
	Tank::update(deltaTime);

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
		updateTargeting(deltaTime);
	}
	break;
	}

	if (m_state == EnemyTankState::Nascent|| m_state == EnemyTankState::Roaming)
	{
		//updateTankDirection(m_direction);
		fw::Vec2f dispToPlayer = getPosition().displacementTo(m_playerTank->getPosition());
		updateCannonDirection(dispToPlayer);
	}

	if (m_state == EnemyTankState::Roaming || m_state == EnemyTankState::Targeting)
	{
		m_timeToStateChange -= deltaTime;
	}
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
	updateTankDirection(m_spawningGate->getDirectionToGameSpace());

	auto bounds = getGlobalBounds();
	auto spawnArea = m_spawningGate->getSpawnArea();

	if(!(bounds.intersects(spawnArea)))
	{
		transitionToRoaming();
	}
}

void EnemyTank::updateRoaming(const float& deltaTime)
{
	m_timeToDirectionChange -= deltaTime;

	if ((m_timeToDirectionChange <= 0.f) || !(getRepulsion().isZero()))
	{
		resetDirection();
	}

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

void EnemyTank::updateTargeting(const float& deltaTime)
{
	stayHalted();
	
	updateLaser(deltaTime);

	if (m_timeToStateChange <= 0.f)
	{
		fireMissile(getCannonDirection());
		transitionToRoaming();
	}
}

void EnemyTank::transitionToRoaming()
{
	setTankTint(fw::Colour::Blue);

	m_state = EnemyTankState::Roaming;

	cleanUpLaser();

	resetDirection();

	m_timeToStateChange = ENEMY_TANK_ROAMING_TIME_LONGEST;

}

void EnemyTank::transitionToTargeting()
{
	m_state = EnemyTankState::Targeting;

	initLaser();

	stayHalted();

	m_timeToStateChange = m_timeTargeting = ENEMY_TANK_TARGETING_TIME_LONGEST;

}

void EnemyTank::resetDirection()
{
	//m_heading = (fw::util::randomUnitVec2f() + (getRepulsion() * ENEMY_TANK_REPULSION_COEFF));
	fw::Vec2f repulsion = getRepulsion();
	if(!(repulsion.isZero()))
	{
		updateTankDirection(repulsion);
	}
	else
	{
		updateTankDirection(fw::util::randomUnitVec2f());
	}

	m_timeToDirectionChange = ENEMY_TANK_HEADING_CHANGE_TIME_LONGEST;
}

void EnemyTank::initLaser()
{
	m_lineComponent->setVisible(); // addLineSegment(rayCastFromCannon());

	fw::LineSegment laserLine = rayCastFromCannon();

	fw::LineSegment line0(laserLine.getStartPoint(), laserLine.getStartPoint());
	fw::LineSegment line1(laserLine.getStartPoint(), laserLine.getEndPoint());
	m_laserMidPoint = laserLine.getStartPoint();

	m_lineComponent->updateLineSegment(0, line0);
	m_lineComponent->setSegmentGradient(0, LASER_FIRING_COLOUR, LASER_FIRING_COLOUR);// LASER_INIT_COLOUR);

	m_lineComponent->updateLineSegment(1, line1);
	m_lineComponent->setSegmentGradient(1, LASER_INIT_COLOUR, LASER_INIT_COLOUR);
}

void EnemyTank::updateLaser(const float& deltaTime)
{
	float t = fw::util::inverseLerp(0.f, m_timeTargeting, m_timeToStateChange);
	t = 1.f - t;
	t = fw::util::clamp(0.f, t, 1.f);

	float r = fw::util::lerp(LASER_INIT_COLOUR.r, LASER_FIRING_COLOUR.r, t);
	float g = fw::util::lerp(LASER_INIT_COLOUR.g, LASER_FIRING_COLOUR.g, t);
	float b = fw::util::lerp(LASER_INIT_COLOUR.b, LASER_FIRING_COLOUR.b, t);
	fw::Colour midColour(r, g, b);
	
	m_laserLine = rayCastFromCannon();

	fw::Vec2f targetMidPoint = fw::util::lerp(
		m_laserLine.getStartPoint(),
		m_laserLine.getEndPoint(),
		t
	);

	const static float LASER_MID_T_COEFF = 12.f;
	float midT = deltaTime * LASER_MID_T_COEFF;
	midT = fw::util::clamp(0.f, midT, 1.f);
	m_laserMidPoint = fw::util::lerp(m_laserMidPoint, targetMidPoint, midT);
	
	// ensuring midpoint is not knocked out of the laser line
	fw::Vec2f startToEnd = m_laserLine.getStartToEndDisplacement();
	fw::Vec2f startToMid = m_laserLine.getStartPoint().displacementTo(m_laserMidPoint);
	startToMid           = startToEnd.normalised() * startToMid.magnitude();
	m_laserMidPoint      = m_laserLine.getStartPoint() + startToMid;

	fw::LineSegment line0(m_laserLine.getStartPoint(), m_laserMidPoint);
	fw::LineSegment line1(m_laserMidPoint, m_laserLine.getEndPoint());

	m_lineComponent->updateLineSegment(0, line0);
	m_lineComponent->setSegmentGradient(0, LASER_FIRING_COLOUR, midColour);

	m_lineComponent->updateLineSegment(1, line1);
	m_lineComponent->setSegmentGradient(1, midColour, LASER_INIT_COLOUR);
}

void EnemyTank::cleanUpLaser()
{
	m_lineComponent->setInvisible();
}

fw::Vec2f EnemyTank::getRepulsion()
{
	/*const static float REPULSION_FROM_PLAYERTANK_WEIGHT = 1.f;
	const static float REPULSION_FROM_ENEMYTANKS_WEIGHT = 2.f;
	const static float REPULSION_FROM_WALLS_WEIGHT      = 2.f;*/

	auto repulsionFromObj = [&](GameObject* object)
	{
		fw::Vec2f displacementFromObj = getPosition().displacementFrom(object->getPosition());
		float distance = displacementFromObj.magnitude();
		if(distance > REPULSION_HORIZON_PIXELS) return fw::Vec2f(0.f);
		float repulsionMagnitude = REPULSION_HORIZON_PIXELS - distance;
		return fw::Vec2f(displacementFromObj.normalised() * repulsionMagnitude);
	};

	auto repulsionFromLine = [&](const fw::LineSegment& lineSegment)
	{
		fw::Vec2f displacementFromLine = lineSegment.getShortestDisplacementToPoint(getPosition());
		float distance = displacementFromLine.magnitude();
		if (distance > REPULSION_HORIZON_PIXELS) return fw::Vec2f(0.f);
		float repulsionMagnitude = REPULSION_HORIZON_PIXELS - distance;
		return fw::Vec2f(displacementFromLine.normalised() * repulsionMagnitude);
	};

	fw::Vec2f repulsion(fw::Vec2f::zero());

	repulsion += repulsionFromObj(m_playerTank.get());

	for (auto& enemyTank : *m_enemyTanks)
	{
		if(enemyTank.get() == this) continue;
		repulsion += repulsionFromObj(enemyTank.get());
		if(EnemyTank* enemyPtr = static_cast<EnemyTank*>(enemyTank.get()))
		{
			if (enemyPtr->getState() == EnemyTankState::Targeting)
			{
				repulsion += repulsionFromLine(enemyPtr->getLaserLine());
			}
		}
	}

	for (auto& wall : m_gameBoundsLines)
	{
		repulsion += repulsionFromLine(wall);
	}

	return repulsion;
}

fw::LineSegment EnemyTank::rayCastFromCannon(bool* hitsPlayerTank)
{
	if(hitsPlayerTank) *hitsPlayerTank = false;

	fw::Vec2f direction = getCannonDirection().normalised();

	fw::Vec2f laserStartPt = getPosition() + direction * originToCannonTip();

	fw::Vec2f laserEndPt = laserStartPt;
	while (m_gameBoundsRect.contains(laserEndPt))
	{
		if (m_playerTank->containsPoint(laserEndPt))
		{
			if (hitsPlayerTank) *hitsPlayerTank = true;
			return fw::LineSegment(laserStartPt, laserEndPt);
		}
		for (const std::shared_ptr<fw::GameObject>& enemyTank : *m_enemyTanks)
		{
			if (enemyTank.get() == this) continue;

			if (fw::util::isType<GameObject, Tank>(enemyTank))
			{
				if (std::reinterpret_pointer_cast<Tank>(enemyTank)->containsPoint(laserEndPt))
				{
					return fw::LineSegment(laserStartPt, laserEndPt);
				}
			}
		}

		laserEndPt += direction;
	}

	return fw::LineSegment(laserStartPt, laserEndPt);
}
