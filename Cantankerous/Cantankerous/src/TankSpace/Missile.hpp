#pragma once

#include <FlatWhite.hpp>

class SparkEmitter;

class Missile : public fw::GameObject
{
public:
	Missile(
		std::shared_ptr<fw::Texture> texture,
		fw::PhysicsSpace* physicsSpace,
		fw::Vec2f initPosition,
		float initRotation,
		fw::Vec2f direction,
		int pixelsPerMetre,
		std::shared_ptr<fw::Texture> sparkTexture
	);

	virtual void collisionResponse(GameObject* other);

private:
	std::shared_ptr<SparkEmitter> m_sparkEmitter;
};

