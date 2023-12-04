#include "SmokeEmitter.hpp"

const float      SMOKE_DEFAULT_TTL = 0.16f;
const fw::Colour SMOKE_COLOUR = fw::Colour::Yellow;
const float      SMOKE_PARTICLES_PER_SECOND = 0.f;
const fw::Vec2f  SMOKE_MAX_VELOCITY = fw::Vec2f(260.f);
const float      SMOKE_ALPHA_DELTA = -4.5f;

SmokeEmitter::SmokeEmitter()
{
	/*m_emitter = std::make_shared<fw::ParticleSystemComponent>(
		this,

	);*/
}

void SmokeEmitter::update(const float& deltaTime)
{

}
