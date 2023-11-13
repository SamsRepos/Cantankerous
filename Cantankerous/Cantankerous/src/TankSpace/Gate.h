#pragma once

#include <FlatWhite.h>

class Gate : public fw::GameObject
{
public:
	Gate(
		const fw::Vec2f& enemySpawnPosition,
		const fw::Vec2f& directionToGameSpace,
		const fw::Rectangle& gateArea,
		std::shared_ptr<fw::Texture> particleTexture
	);
};

