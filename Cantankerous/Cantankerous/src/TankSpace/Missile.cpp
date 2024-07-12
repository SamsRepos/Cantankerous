#include "Missile.hpp"

#include "SparkEmitter.hpp"
#include "SmokeEmitter.hpp"

const float MISSILE_DENSITY     = 10.f;
const float MISSILE_RESTITUTION = 0.f;
const float MISSILE_FRICTION    = 0.f;

const float MISSILE_SPEED = 45.f;

const float MISSILE_SMOKE_RADIUS = 1.42f;

const fw::Colour MISSILE_TINT = fw::Colour(0xbb, 0x88, 0x88);

Missile::Missile(
	std::shared_ptr<fw::Texture> texture,
	fw::PhysicsSpace* physicsSpace,
	fw::Vec2f initPosition,
	float initRotation,
	fw::Vec2f direction,
	int pixelsPerMetre,
	std::shared_ptr<fw::Texture> sparkTexture,
	std::shared_ptr<fw::Texture> smokeTexture
)
	:
	GameObject(initPosition, initRotation)
{
	setMoribundWhenParentIsMoribund(false);

	auto sprite = std::make_shared<fw::SpriteComponent>(
		this,
		texture
	);
	sprite->setTint(MISSILE_TINT);
	addComponent(sprite);

	auto body = std::make_shared<fw::BodyComponent>(
		this,
		physicsSpace,
		pixelsPerMetre,
		sprite->getSize(),
		fw::BodyShape::Box,
		MISSILE_DENSITY,
		MISSILE_RESTITUTION,
		MISSILE_FRICTION
	);
	addComponent(body);

	m_sparkEmitter = std::make_shared<SparkEmitter>(sparkTexture);
	addChild(m_sparkEmitter);

	m_smokeEmitter = std::make_shared<SmokeEmitter>(
		smokeTexture,
		getPosition(),
		MISSILE_SMOKE_RADIUS
	);
	addChild(m_smokeEmitter);

	fw::Vec2f velocity = direction.normalised() * MISSILE_SPEED;
	body->setLinearVelocity(velocity);
}

//
// PROTECTED:
//

void Missile::update(const float& deltaTime)
{
	GameObject::update(deltaTime);

	m_smokeEmitter->setPosition(getPosition());
}

void Missile::collisionResponse(GameObject* other)
{
	setMoribund();
	m_sparkEmitter->emitSparks(getPosition());
	m_smokeEmitter->stopEmitting();
}