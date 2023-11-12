#pragma once

#include "Ball.h"

class FunBall : public Ball
{
public:
	FunBall(
		std::shared_ptr<sf::Texture> texture,
		fw::World* world,
		const fw::Vec2f& initPos,
		int pixelsPerMetre
	);
	~FunBall();

	virtual void collisionResponse(GameObject* other);

protected:

private:
	fw::Colour randomColour();
};

