#include "Missile.h"

const float MISSILE_DENSITY     = 10.f;
const float MISSILE_RESTITUTION = 0.f;
const float MISSILE_FRICTION    = 0.f;

const float MISSILE_SPEED = 42.f;

Missile::Missile(
	std::shared_ptr<fw::Texture> texture,
	fw::World* world,
	fw::Vec2f initPosition,
	float initRotation,
	fw::Vec2f direction,
	int pixelsPerMetre
)
	:
	GameObject(initPosition, initRotation)
{
	auto sprite = std::make_shared<fw::SpriteComponent>(
		this,
		texture
	);
	addComponent(sprite);

	auto body = std::make_shared<fw::BodyComponent>(
		this,
		world,
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
}