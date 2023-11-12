#pragma once

#include <FlatWhite.h>

class Wall : public fw::GameObject
{
public:
	Wall(
		std::shared_ptr<fw::Texture> texture,
		fw::World* world,
		fw::Vec2f initPos,
		int pixelsPerMetre
	);
	~Wall();

	virtual void collisionResponse(GameObject* other);

protected:
	

private:
	
};

