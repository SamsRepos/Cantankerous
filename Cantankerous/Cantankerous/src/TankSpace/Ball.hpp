#pragma once

#include <FlatWhite.hpp>

class SpriteComponent;

class Ball : public fw::GameObject
{
public:
	Ball(
		std::shared_ptr<fw::Texture> texture,
		fw::PhysicsSpace* physicsSpace,
		const fw::Vec2f& initPos,
		int pixelsPerMetre
	);

	void collisionResponse(GameObject* other);

protected:
	std::shared_ptr<fw::SpriteComponent> m_sprite;

};

