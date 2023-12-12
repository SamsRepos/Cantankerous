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

	virtual void update(const float& deltaTime);

private:
	std::shared_ptr<fw::ParticleSourceArea> m_spawnArea;
	std::shared_ptr<fw::ParticleSystemComponent> m_emitter;
};