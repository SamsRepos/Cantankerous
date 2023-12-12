#include "SmokeEmitter.hpp"

const float      SMOKE_DEFAULT_TTL = 0.16f;
const fw::Colour SMOKE_COLOUR = fw::Colour::Yellow;
const float      SMOKE_PARTICLES_PER_SECOND = 0.f;
const fw::Vec2f  SMOKE_MAX_VELOCITY = fw::Vec2f(260.f);
const float      SMOKE_ALPHA_DELTA = -4.5f;

SmokeEmitter::SmokeEmitter(
	std::shared_ptr<fw::Texture> texture,
	const fw::Vec2f& initialPosition,
	const float& radius
)
{
	m_spawnArea = std::make_shared<fw::CircleParticleSourceArea>(initialPosition, radius);

	m_emitter = std::make_shared<fw::ParticleSystemComponent>(
		this,
		SMOKE_DEFAULT_TTL,
		SMOKE_COLOUR,
		texture,
		m_spawnArea
	);
}

void SmokeEmitter::update(const float& deltaTime)
{
	m_emitter->update(deltaTime);
}
