#pragma once

#include <FlatWhite.hpp>

class SmokeEmitter : public fw::GameObject
{
public:
	SmokeEmitter(
		std::shared_ptr<fw::Texture> texture,
		const fw::Vec2f& initialPosition,
		const float& radius
	);

	void resumeEmitting();
	void stopEmitting();

protected:
	virtual void update(const float& deltaTime);

private:
	std::shared_ptr<fw::CircleParticleSourceArea> m_spawnArea;
	std::shared_ptr<fw::ParticleSystemComponent> m_emitter;

	float m_particlesPerSecond;
};