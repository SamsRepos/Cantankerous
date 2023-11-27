#include "Ball.hpp"

#include <FlatWhite.hpp>

#include "Missile.hpp"

const float BALL_DENSITY     = .01f;
const float BALL_RESTITUTION = .9f; // .8f;
const float BALL_FRICTION    = 1.f;

Ball::Ball(
	std::shared_ptr<fw::Texture> texture,
	fw::PhysicsSpace* physicsSpace,
	const fw::Vec2f& initPos,
	int pixelsPerMetre
)
	:
	GameObject::GameObject(initPos)
{
	m_sprite = std::make_shared<fw::SpriteComponent>(
		this,
		texture
	);
	addComponent(m_sprite);

	auto body = std::make_shared<fw::BodyComponent>(
		this,
		physicsSpace,
		pixelsPerMetre,
		m_sprite->getSize(),
		fw::BodyShape::Ball,
		BALL_DENSITY,
		BALL_RESTITUTION,
		BALL_FRICTION
	);
	addComponent(body);

	/*b2MassData massData;
	massData.mass = 1;
	massData.center = b2Vec2(0.f, 0.f);
	massData.I = .1f;*/

	//getBody()->SetMassData(&massData);
}

//
//  PUBLIC
//

void Ball::collisionResponse(GameObject* other)
{
	if (fw::util::isType<GameObject, Missile>(other))
	{
		setMoribund();
	}
}

//
//  PRIVATE
//