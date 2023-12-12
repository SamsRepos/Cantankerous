#include "Gate.hpp"

#include "PlayerTank.hpp"

const float      GATE_DEFAULT_TTL          = 0.13f;
const fw::Colour SHOCKING_PINK             = fw::Colour(0xfc, 0x0f, 0xc0);
const fw::Colour ULTRA_PINK                = fw::Colour(0xff, 0x6f, 0xff);
const fw::Colour GATE_COLOUR               = ULTRA_PINK;
const float      GATE_PARTICLES_PER_SECOND = 10000;
const float      FIZZLER_MAX_SPEED         = 260.f;
const float      FIZZLER_ALPHA_DELTA       = -4.5f;

Gate::Gate(
	const fw::Vec2f& enemySpawnPosition,
	const fw::Vec2f& directionToGameSpace,
	const fw::Rectangle& spawnArea,
	const fw::Rectangle& fizzlerArea,
	std::shared_ptr<fw::Texture> particleTexture
)
	:
	GameObject(enemySpawnPosition),
	m_spawnPos(enemySpawnPosition),
	m_directionToGameSpace(directionToGameSpace),
	m_spawnArea(spawnArea)

{
	m_particleSourceArea = std::make_shared<fw::RectangleParticleSourceArea>(fizzlerArea);

	auto particleSystem = std::make_shared<fw::ParticleSystemComponent>(
		this,
		GATE_DEFAULT_TTL,
		GATE_COLOUR,
		particleTexture,
		m_particleSourceArea,
		GATE_PARTICLES_PER_SECOND
	);
	particleSystem->m_maxSpeed    = FIZZLER_MAX_SPEED;
	particleSystem->m_alphaChange = FIZZLER_ALPHA_DELTA;

	addComponent(particleSystem);
}
