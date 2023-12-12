#include "SparkEmitter.hpp"

const float      SPARK_DEFAULT_TTL          = 0.16f;
const fw::Colour SPARK_COLOUR               = fw::Colour::Yellow;
const float      SPARK_PARTICLES_PER_SECOND = 0.f;
const fw::Vec2f  SPARK_MAX_VELOCITY         = fw::Vec2f(260.f);
const float      SPARK_ALPHA_DELTA          = -3.5f;

const int NUM_SPARKS_TO_EMIT = 10;

SparkEmitter::SparkEmitter(std::shared_ptr<fw::Texture> texture)
{
	setMoribundWhenParentIsMoribund(false);

	m_spawnPoint = std::make_shared<fw::PointParticleSourceArea>(fw::Vec2f(0.f));

	m_emitter = std::make_shared<fw::ParticleSystemComponent>(
		this,
		SPARK_DEFAULT_TTL,
		SPARK_COLOUR,
		texture,
		m_spawnPoint,
		0.f,
		-SPARK_MAX_VELOCITY,
		SPARK_MAX_VELOCITY,
		SPARK_ALPHA_DELTA
	);

	addComponent(m_emitter);
}

void SparkEmitter::emitSparks(const fw::Vec2f& position)
{
	//m_spawnPoint->updateSourcePoint(position);
	//m_emitter->emitParticles(NUM_SPARKS_TO_EMIT);
	m_emitter->emitParticlesAtPosition(NUM_SPARKS_TO_EMIT, position);
}