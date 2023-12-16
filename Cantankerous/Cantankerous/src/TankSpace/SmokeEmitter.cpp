#include "SmokeEmitter.hpp"

const float      SMOKE_DEFAULT_TTL           = 0.35f;
const float      SMOKE_PARTICLES_PER_SECOND_RADIUS_COEFF  = 130.f;
const fw::Colour SMOKE_COLOUR                = fw::Colour(0x50, 0x50, 0x50);
const fw::Vec2f  SMOKE_MAX_VELOCITY          = fw::Vec2f(260.f);
const float      SMOKE_ALPHA_DELTA           = -.5f;
const float      SMOKE_MIN_SCALE             = .14f;
const float      SMOKE_MAX_SCALE             = .29f;
const float      SMOKE_MIN_ALPHA             = .04f;
const float      SMOKE_MAX_ALPHA             = .08f;
const float      SMOKE_MIN_ROTATION_SPEED    = 1.f;
const float      SMOKE_MAX_ROTATION_SPEED    = 50.f;

SmokeEmitter::SmokeEmitter(
	std::shared_ptr<fw::Texture> texture,
	const fw::Vec2f& initialPosition,
	const float& radius
)
	:
	m_particlesPerSecond(SMOKE_PARTICLES_PER_SECOND_RADIUS_COEFF * radius * radius)
{
	setMoribundWhenParentIsMoribund(false);

	m_spawnArea = std::make_shared<fw::CircleParticleSourceArea>(initialPosition, radius);

	m_emitter = std::make_shared<fw::ParticleSystemComponent>(
		this,
		SMOKE_DEFAULT_TTL,
		SMOKE_COLOUR,
		texture,
		m_spawnArea,
		m_particlesPerSecond
	);
	m_emitter->m_alphaChange       = SMOKE_ALPHA_DELTA;
	m_emitter->m_minScale          = SMOKE_MIN_SCALE;
	m_emitter->m_maxScale          = SMOKE_MAX_SCALE;
	m_emitter->m_minAlpha          = SMOKE_MIN_ALPHA;
	m_emitter->m_maxAlpha          = SMOKE_MAX_ALPHA;
	m_emitter->m_minRotationSpeed  = SMOKE_MIN_ROTATION_SPEED;
	m_emitter->m_maxRotationSpeed  = SMOKE_MAX_ROTATION_SPEED;
	addComponent(m_emitter);
}

void SmokeEmitter::resumeEmitting()
{
	m_emitter->setParticlesPerSecond(m_particlesPerSecond);
}

void SmokeEmitter::stopEmitting() 
{ 
	m_emitter->setParticlesPerSecond(0.f); 
};

//
// PROTECTED:
//

void SmokeEmitter::update(const float& deltaTime)
{
	m_spawnArea->updateSourcePosition(getPosition());
	m_emitter->update(deltaTime);
}