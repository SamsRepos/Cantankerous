#pragma once

#include <FlatWhite.hpp>

class SmokeEmitter : public fw::GameObject
{
public:
	SmokeEmitter();

	virtual void update(const float& deltaTime);

private:
	static fw::Texture m_texture;
	std::shared_ptr<fw::ParticleSystemComponent> m_emitter;
};