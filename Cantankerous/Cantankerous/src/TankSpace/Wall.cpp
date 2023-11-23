#include "Wall.h"

#include <FlatWhite.h>

//
// CONSTRUCTOR AND DESTRUCTOR
//

Wall::Wall(
	std::shared_ptr<fw::Texture> texture,
	fw::PhysicsSpace* physicsSpace,
	fw::Vec2f initPos,
	int pixelsPerMetre
)
	:
	GameObject::GameObject(initPos)
{
	auto sprite = std::make_shared<fw::SpriteComponent>(
		this,
		texture
	);
	addComponent(sprite);

	auto body = std::make_shared<fw::BodyComponent>(
		this,
		physicsSpace,
		pixelsPerMetre,
		sprite->getSize(),
		fw::BodyShape::Box,
		1.f,
		1.f,
		0.1f,
		b2_staticBody
	);
}

Wall::~Wall()
{

}

//
//  PUBLIC:
//

void Wall::collisionResponse(GameObject* other)
{

}