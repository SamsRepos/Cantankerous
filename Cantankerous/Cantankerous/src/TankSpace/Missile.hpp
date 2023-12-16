#pragma once

#include <FlatWhite.hpp>

class SparkEmitter;
class SmokeEmitter;

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
		std::shared_ptr<fw::Texture> sparkTexture,
		std::shared_ptr<fw::Texture> smokeTexture
	);

protected:
	virtual void update(const float& deltaTime);

	virtual void collisionResponse(GameObject* other);

private:
	std::shared_ptr<SparkEmitter> m_sparkEmitter;
	std::shared_ptr<SmokeEmitter> m_smokeEmitter;
};

