#pragma once

#include <FlatWhite.h>

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
		SparkEmitter* sparkEmitter
	);

	virtual void collisionResponse(GameObject* other);

private:
	SparkEmitter* m_sparkEmitter;
};

