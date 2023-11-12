#pragma once

#include <FlatWhite.h>

//#include "Graphics.h"

class Missile : public fw::GameObject
{
public:
	Missile(
		std::shared_ptr<fw::Texture> texture,
		fw::World* world,
		fw::Vec2f initPosition,
		float initRotation,
		fw::Vec2f direction,
		int pixelsPerMetre
	);

	virtual void collisionResponse(GameObject* other);

private:
};

