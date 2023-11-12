#pragma once

#include <FlatWhite.h>

#include "Missile.h"

const float TANK_NORMAL_SPEED = 1.4f;
const float TANK_MAX_SPEED = 5.f;

const float TANK_ROTATION_COEFF = 10.f;

class Tank : public fw::GameObject
{
public:
	Tank(
		std::shared_ptr<fw::Texture> tankTexture,
		std::shared_ptr<fw::Texture> cannonTexture,
		std::shared_ptr<fw::Texture> missileTexture,
		fw::World* world,
		fw::Vec2f initPos,
		int pixelsPerMetre
	);

	virtual void update(float deltaTime);
	virtual void render(fw::RenderTarget* window);

	virtual void collisionResponse(GameObject* other);

protected:
	std::shared_ptr<fw::SpriteComponent>           m_tankSprite;
	std::shared_ptr<fw::SpriteComponent>           m_cannonSprite;
	std::shared_ptr<fw::BodyComponent>             m_body;
	std::shared_ptr<fw::SpawnerComponent<Missile>> m_missileSpawner;
	
	std::shared_ptr<fw::Texture> m_missileTexture;

private:
	
};

