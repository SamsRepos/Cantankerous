#pragma once

#include <FlatWhite.hpp>

#include "Missile.hpp"

class HealthGauge;
class SmokeEmitter;

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
		std::shared_ptr<fw::Texture> sparkTexture,
		std::shared_ptr<fw::Texture> smokeTexture
	);

	//inline fw::SpriteComponent* getTankSprite() { return m_tankSprite.get(); };
	//inline fw::BodyComponent* getBody() { return m_body.get(); };

	bool containsPoint(const fw::Vec2f& point);

protected:
	virtual void update(const float& deltaTime);
	virtual void collisionResponse(GameObject* other);

	// update sub functions for derived classes:
	void updateTankDirection(const fw::Vec2f& direction);
	const fw::Vec2f& getTankDirection() const;
	void updateCannonDirection(const fw::Vec2f& direction);
	const fw::Vec2f& getCannonDirection() const;

	void setSpeed(float speed);

	const fw::Rectangle& getGlobalBounds() const;

	void fireMissile(fw::Vec2f missileDirection);

	void stayHalted();

	float originToCannonTip() const;

	void setTankTint(const fw::Colour& colour);

	

private:
	void updateTankRotation();
	void updateCannonRotation(const float& deltaTime);
	
	void takeDamage(float damage);
	
	float m_speed;
	fw::Vec2f m_tankDirection;
	fw::Vec2f m_cannonTargetDirection;
	fw::Vec2f m_cannonDirection;

	std::shared_ptr<fw::SpriteComponent>           m_tankSprite;
	std::shared_ptr<fw::SpriteComponent>           m_cannonSprite;
	std::shared_ptr<fw::BodyComponent>             m_body;
	std::shared_ptr<fw::SpawnerComponent<Missile>> m_missileSpawner;
	std::shared_ptr<HealthGauge>                   m_healthGauge;
	std::shared_ptr<fw::Texture>                   m_sparkTexture;
	std::shared_ptr<fw::Texture>                   m_smokeTexture;
	std::shared_ptr<SmokeEmitter>                  m_smokeEmitter;
	float m_health;

	std::shared_ptr<fw::Texture> m_missileTexture;
};

