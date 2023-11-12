#pragma once

#include <FlatWhite.h>

class SpriteComponent;

class Ball : public fw::GameObject
{
public:
	Ball(
		std::shared_ptr<fw::Texture> texture,
		fw::World* world,
		const fw::Vec2f& initPos,
		int pixelsPerMetre
	);
	~Ball();

	void collisionResponse(GameObject* other);

protected:
	std::shared_ptr<fw::SpriteComponent> m_sprite;

};

