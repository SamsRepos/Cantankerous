#include "FunBall.h"

#include <FlatWhite.h>

#include "Missile.h"

FunBall::FunBall(
	std::shared_ptr<sf::Texture> texture,
	fw::World* world,
	const fw::Vec2f& initPos,
	int pixelsPerMetre
)
	:
	Ball::Ball(
		texture, 
		world,
		initPos,
		pixelsPerMetre
	)
{
	m_sprite->setTint(randomColour());
}

FunBall::~FunBall()
{

}
void FunBall::collisionResponse(GameObject* other)
{
	if (fw::util::isType<GameObject, Missile>(other))
	{
		m_sprite->setTint(randomColour());
	}
}

//
// PRIVATE:
// 

fw::Colour FunBall::randomColour()
{
	uint8 r, g, b;
	r = rand() % 0xff;
	g = rand() % 0xff;
	b = rand() % 0xff;

	return fw::Colour(r, g, b);
}