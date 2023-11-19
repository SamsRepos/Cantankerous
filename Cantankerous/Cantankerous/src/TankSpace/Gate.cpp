#include "Gate.h"

const float      GATE_DEFAULT_TTL          = 0.13f;
const fw::Colour SHOCKING_PINK             = fw::Colour(0xfc, 0x0f, 0xc0);
const fw::Colour ULTRA_PINK                = fw::Colour(0xff, 0x6f, 0xff);
const fw::Colour GATE_COLOUR               = ULTRA_PINK;
const float      GATE_PARTICLES_PER_SECOND = 10000;
const fw::Vec2f  GATE_MAX_VELOCITY         = fw::Vec2f(260.f);
const float      GATE_ALPHA_DELTA          = -3.5f;

Gate::Gate(
	const fw::Vec2f& enemySpawnPosition,
	const fw::Vec2f& directionToGameSpace,
	const fw::Rectangle& spawnArea,
	const fw::Rectangle& gateArea,
	std::shared_ptr<fw::Texture> particleTexture
)
	:
	GameObject(enemySpawnPosition),
	m_spawnPos(enemySpawnPosition),
	m_directionToGameSpace(directionToGameSpace),
	m_spawnArea(spawnArea)

{
	auto particleSystem = std::make_shared<fw::ParticleSystemComponent>(
		this,
		GATE_DEFAULT_TTL,
		GATE_COLOUR,
		particleTexture,
		gateArea.getPosition(),
		gateArea.getPosition() + gateArea.getSize(),
		GATE_PARTICLES_PER_SECOND,
		GATE_MAX_VELOCITY,
		-GATE_MAX_VELOCITY,
		GATE_ALPHA_DELTA

	);
	addComponent(particleSystem);
}