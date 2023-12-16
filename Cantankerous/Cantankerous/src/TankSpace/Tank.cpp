#include "Tank.hpp"

#include "HealthGauge.hpp"
#include "SmokeEmitter.hpp"

const float TANK_DENSITY     = 100.f;
const float TANK_RESTITUTION = 0.1f;
const float TANK_FRICTION    = 1.f;

const float TANK_SMOKE_RADIUS = 40.f;

Tank::Tank(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	fw::PhysicsSpace* physicsSpace,
	fw::Vec2f initialPosition,
	float initialRotation,
	int pixelsPerMetre,
	std::shared_ptr<fw::Texture> sparkTexture,
	std::shared_ptr<fw::Texture> smokeTexture
)
	:
	GameObject(initialPosition, initialRotation),
	m_missileTexture(missileTexture),
	m_health(TANK_MAX_HEALTH),
	m_speed(TANK_NORMAL_SPEED),
	m_sparkTexture(sparkTexture),
	m_smokeTexture(smokeTexture)
{
	m_cannonTargetDirection = m_cannonDirection = fw::util::angleToDirection(initialRotation);

	m_tankSprite = std::make_shared<fw::SpriteComponent>(
		this,
		tankTexture
	);
	addComponent(m_tankSprite);

	m_cannonSprite = std::make_shared<fw::SpriteComponent>(
		this,
		cannonTexture
	);
	m_cannonSprite->setRotationLocked(false);
	addComponent(m_cannonSprite);

	m_body = std::make_shared<fw::BodyComponent>(
		this,
		physicsSpace,
		pixelsPerMetre,
		m_tankSprite->getSize(),
		fw::BodyShape::Box,
		TANK_DENSITY,
		TANK_RESTITUTION,
		TANK_FRICTION
	);
	addComponent(m_body);

	//b2MassData massData;
	//massData.mass = 100;
	//massData.center = b2Vec2(0.f, 0.f);
	//massData.I = 1.f;
	//
	//getBody()->SetMassData(&massData);

	m_missileSpawner = std::make_shared<fw::SpawnerComponent<Missile>>(this, this);
	addComponent(m_missileSpawner);

	m_healthGauge = std::make_shared<HealthGauge>(
		getPosition()
	);
	addChild(m_healthGauge);

	m_smokeEmitter = std::make_shared<SmokeEmitter>(
		m_smokeTexture,
		getPosition(),
		TANK_SMOKE_RADIUS
	);
	addChild(m_smokeEmitter);
}


bool Tank::containsPoint(const fw::Vec2f& point)
{
	return m_tankSprite->contains(point);
}

//
//  PROTECTED:
//

void Tank::update(const float& deltaTime)
{
	GameObject::update(deltaTime);

	updateTankRotation();
	updateCannonRotation(deltaTime);

	m_healthGauge->updatePosition(getPosition());

	if (m_speed > FLT_EPSILON)
	{
		m_smokeEmitter->resumeEmitting();
		m_smokeEmitter->setPosition(getPosition());
	}
}

void Tank::render(fw::RenderTarget* window)
{
	GameObject::render(window);
}

void Tank::collisionResponse(GameObject* other)
{
	if (fw::util::isType<GameObject, Missile>(other))
	{
		float damage = fw::util::lerp(
			TANK_MISSILE_DAMAGE_MIN,
			TANK_MISSILE_DAMAGE_MAX,
			fw::util::randomFloat()
		);

		takeDamage(damage);
	}


}

void Tank::updateTankDirection(const fw::Vec2f& direction)
{
	m_tankDirection = direction;
	m_body->setLinearVelocity(m_tankDirection.normalised() * m_speed);
}

const fw::Vec2f& Tank::getTankDirection() const
{
	return m_tankDirection;
}

void Tank::updateCannonDirection(const fw::Vec2f& direction)
{
	m_cannonTargetDirection = direction;
}

const fw::Vec2f& Tank::getCannonDirection() const
{
	return m_cannonDirection;
}

void Tank::setSpeed(float speed)
{
	m_speed = speed;
}

const fw::Rectangle& Tank::getGlobalBounds() const
{
	fw::Rectangle bounds = m_tankSprite->getGlobalBounds();
	return bounds;
}

void Tank::fireMissile(fw::Vec2f missileDirection)
{
	missileDirection.normalise();

	float missileAngle = fw::util::directionToAngle(missileDirection);

	fw::Vec2f missileSpawnPos = getPosition();
	while (m_body->containsPointPixels(missileSpawnPos))
	{
		missileSpawnPos += missileDirection;
	}
	float missileLengthPush = std::max(
		m_missileTexture->getSize().x,
		m_missileTexture->getSize().y
	);
	missileSpawnPos += missileDirection * missileLengthPush;

	m_missileSpawner->spawnObject(
		m_missileTexture,
		m_body->getPhysicsSpace(),
		missileSpawnPos,
		missileAngle,
		missileDirection,
		m_body->getPixelsPerMetre(),
		m_sparkTexture,
		m_smokeTexture
	);
}

void Tank::stayHalted()
{
	m_body->setLinearVelocity(fw::Vec2f(0.f));
	m_body->setAngularVelocity(0.f);
	m_smokeEmitter->stopEmitting();
}

float Tank::originToCannonTip() const
{
	return (m_cannonSprite->getSize().y / 2.f);
}

void Tank::setTankTint(const fw::Colour& colour)
{
	m_tankSprite->setTint(colour);
}

//
//  PRIVATE:
//

void Tank::updateTankRotation()
{
	static float rotationTarget = 0.f;
	if (!m_tankDirection.isZero())
	{
		rotationTarget = fw::util::directionToAngle(m_tankDirection);
	}

	float tankAngle = getRotation();

	while (tankAngle > fw::util::PI)
	{
		tankAngle -= fw::util::TWO_PI;
	}
	while (tankAngle < -fw::util::PI)
	{
		tankAngle += fw::util::TWO_PI;
	}
	// such that -PI <= angle <= PI

	//std::cout << "tankAngle: " << tankAngle << " rotationTarget: " << rotationTarget << std::endl;

	float rotationDelta = rotationTarget - tankAngle;
	if (abs(rotationDelta) > fw::util::PI)
	{
		if (rotationTarget > 0) {
			tankAngle += fw::util::TWO_PI;
			rotationDelta = rotationTarget - tankAngle;
		}
		else if (rotationTarget < 0)
		{
			rotationTarget += fw::util::TWO_PI;
			rotationDelta = rotationTarget - tankAngle;
		}
	}

	m_body->setAngularVelocity(rotationDelta * TANK_ROTATION_COEFF);

}

void Tank::updateCannonRotation(const float& deltaTime)
{
	const static float LERP_PER_SECOND = 17.f;

	float t = fw::util::clamp(0.f, LERP_PER_SECOND * deltaTime, 1.f);

	m_cannonDirection = fw::util::lerp(m_cannonDirection, m_cannonTargetDirection, t);

	m_cannonSprite->setRotation(fw::util::directionToAngle(m_cannonDirection));
}

void Tank::takeDamage(float damage)
{
	m_health -= damage;
	m_health = std::max(m_health, 0.f);
	if (m_health <= 0.f)
	{
		setMoribund();
		m_smokeEmitter->stopEmitting();
	}
	m_healthGauge->updateHealth(m_health / TANK_MAX_HEALTH);
}