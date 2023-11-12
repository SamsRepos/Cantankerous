#include "Gate.h"

const float GATE_DEFAULT_TTL          = 0.08f;
const float GATE_PARTICLES_PER_SECOND = 1000000000;
const fw::Vec2f GATE_MAX_VELOCITY     = fw::Vec2f(260.f);

Gate::Gate(
	const fw::Vec2f& position,
	const fw::Vec2f& size,
	std::shared_ptr<fw::Texture> texture
)
	:
	GameObject(position)

{
#if 0
	auto partSys = std::make_shared<ParticleSystemComponent>(
		this,
		GATE_DEFAULT_TTL,
		fw::Colour::White,
		texture,
		position,
		position + size,
		GATE_PARTICLES_PER_SECOND,
		GATE_MAX_VELOCITY,
		-GATE_MAX_VELOCITY,
		-0.9f
	);
#else
	auto partSys = std::make_shared<fw::ParticleSystemComponent>(
		this,
		10,
		fw::Colour::White,
		texture,
		position,
		position + size,
		10,
		GATE_MAX_VELOCITY,
		-GATE_MAX_VELOCITY,
		-0.99f
		);
#endif
	addComponent(partSys);
}