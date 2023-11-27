#include "Missile.hpp"

#include "SparkEmitter.hpp"

const float MISSILE_DENSITY     = 10.f;
const float MISSILE_RESTITUTION = 0.f;
const float MISSILE_FRICTION    = 0.f;

const float MISSILE_SPEED = 32.f; // 42.f;

const fw::Colour MISSILE_TINT = fw::Colour(0xbb, 0x88, 0x88);

Missile::Missile(
	std::shared_ptr<fw::Texture> texture,
	fw::PhysicsSpace* physicsSpace,
	fw::Vec2f initPosition,
	float initRotation,
	fw::Vec2f direction,
	int pixelsPerMetre,
	SparkEmitter* sparkEmitter
)
	:
	GameObject(initPosition, initRotation),
	m_sparkEmitter(sparkEmitter)
{
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

	fw::Vec2f velocity = direction.normalised() * MISSILE_SPEED;
	body->setLinearVelocity(velocity);
}


void Missile::collisionResponse(GameObject* other)
{
	setMoribund();
	m_sparkEmitter->emitSparks(getPosition());
}