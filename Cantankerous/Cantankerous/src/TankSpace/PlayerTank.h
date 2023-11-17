#pragma once

#include "Tank.h"

const float PLAYERTANK_BOOST_SPEED_DECAY = 5.f; //units of velocity per second (ie. deceleration)
const float PLAYERTANK_BOOST_RECHARGE    = .1f; //units of charge per second. (max charge is 1.f)
const float PLAYERTANK_BOOST_COST        = .3f; //in units of boost charge

class PlayerTank : public Tank
{
public:
	PlayerTank(
		std::shared_ptr<fw::Texture> tankTexture,
		std::shared_ptr<fw::Texture> cannonTexture,
		std::shared_ptr<fw::Texture> missileTexture,
		fw::World* world,
		fw::Vec2f initPos,
		int pixelsPerMetre
	);

	virtual void handleInput(const fw::Input& input);
	virtual void update(float deltaTime);

	virtual void collisionResponse(GameObject* other);

	inline float getBoostCharge() { return m_boost.currentCharge; };
private:
	// input sub functions:
	void handleInputLinearMovement(const fw::Input& input);
	void handleInputCannonRotation(const fw::Input& input);
	void handleInputFireMissiles(const fw::Input& input);

	struct Boost {
		float currentSpeed;
		float currentCharge;
		void update(float deltaTime);
		void boostNow();
	};

	Boost m_boost;

	fw::Vec2f m_inputVelocity;
};
