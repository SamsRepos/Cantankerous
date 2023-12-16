#pragma once

#include <FlatWhite.hpp>

class Wall : public fw::GameObject
{
public:
	Wall(
		std::shared_ptr<fw::Texture> texture,
		fw::PhysicsSpace* physicsSpace,
		fw::Vec2f initPos,
		int pixelsPerMetre
	);
	~Wall();

protected:
	virtual void collisionResponse(GameObject* other);

private:
	
};

