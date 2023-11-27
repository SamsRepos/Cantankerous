#pragma once

#include <FlatWhite.hpp>

class SparkEmitter : public fw::GameObject
{
public:
	SparkEmitter(std::shared_ptr<fw::Texture> texture);

	void emitSparks(const fw::Vec2f& position);

private:
	std::shared_ptr<fw::ParticleSystemComponent> m_emitter;
};