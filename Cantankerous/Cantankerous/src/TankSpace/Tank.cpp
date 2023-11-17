#include "Tank.h"

#include <iostream>

const float TANK_DENSITY     = 100.f;
const float TANK_RESTITUTION = 0.1f;
const float TANK_FRICTION    = 1.f;

Tank::Tank(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	fw::World* world,
	fw::Vec2f initialPosition,
	float initialRotation,
	int pixelsPerMetre
)
	:
	GameObject(initialPosition, initialRotation),
	m_missileTexture(missileTexture)
{
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
		world,
		pixelsPerMetre,
		m_tankSprite->getSize(),
		fw::BodyShape::Box,
		TANK_DENSITY,
		TANK_RESTITUTION,
		TANK_FRICTION
	);
	addComponent(m_body);

	/*b2MassData massData;
	massData.mass = 100;
	massData.center = b2Vec2(0.f, 0.f);
	massData.I = 1.f;*/

	//getBody()->SetMassData(&massData);

	m_missileSpawner = std::make_shared<fw::SpawnerComponent<Missile>>(this);
	addComponent(m_missileSpawner);
}

void Tank::update(float deltaTime)
{
	GameObject::update(deltaTime);
}

void Tank::collisionResponse(GameObject* other)
{

}

void Tank::render(fw::RenderTarget* window)
{
	GameObject::render(window);
}

//
//  PROTECTED:
//

void Tank::updateTankRotation(const fw::Vec2f direction)
{
	static float rotationTarget = 0.f;
	if (!direction.isZero())
	{
		rotationTarget = fw::util::directionToAngle(direction);
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

//
//  PRIVATE:
//


