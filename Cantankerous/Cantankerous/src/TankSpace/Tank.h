#pragma once

#include <FlatWhite.h>

#include "Missile.h"

const float TANK_NORMAL_SPEED = 1.1f;
const float TANK_MAX_SPEED = 5.f;

const float TANK_ROTATION_COEFF = 10.f;

const float TANK_MAX_HEALTH         = 100.f;
const float TANK_MISSILE_DAMAGE_MIN = 10.f;
const float TANK_MISSILE_DAMAGE_MAX = 35.f;


class Tank : public fw::GameObject
{
public:
	Tank(
		std::shared_ptr<fw::Texture> tankTexture,
		std::shared_ptr<fw::Texture> cannonTexture,
		std::shared_ptr<fw::Texture> missileTexture,
		fw::PhysicsSpace* physicsSpace,
		fw::Vec2f initialPosition,
		float initialRotation,
		int pixelsPerMetre,
		GameObject* parentForSpawnedMissiles,
		SparkEmitter* sparkEmitter
	);

	virtual void update(const float& deltaTime);
	virtual void render(fw::RenderTarget* window);

	virtual void collisionResponse(GameObject* other);

	inline fw::SpriteComponent* getTankSprite() { return m_tankSprite.get(); };
	inline fw::BodyComponent* getBody() { return m_body.get(); };

protected:
	// update sub functions for derived classes:
	void updateTankRotation(const fw::Vec2f& direction);

	void fireMissile(fw::Vec2f missileDirection);

	std::shared_ptr<fw::SpriteComponent>           m_tankSprite;
	std::shared_ptr<fw::SpriteComponent>           m_cannonSprite;
	std::shared_ptr<fw::BodyComponent>             m_body;
	std::shared_ptr<fw::SpawnerComponent<Missile>> m_missileSpawner;
	
	std::shared_ptr<fw::Texture> m_missileTexture;

	float m_health;

private:
	SparkEmitter* m_sparkEmitter;
	
};

