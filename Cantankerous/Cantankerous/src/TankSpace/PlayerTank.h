#pragma once

#include "Tank.h"

const float PLAYERTANK_BOOST_SPEED_DECAY = 11.f; //units of velocity per second (ie. deceleration)
const float PLAYERTANK_BOOST_RECHARGE    = .1f; //units of charge per second. (max charge is 1.f)
const float PLAYERTANK_BOOST_COST        = .3f; //in units of boost charge

class Gate;

class PlayerTank : public Tank
{
public:
	PlayerTank(
		std::shared_ptr<fw::Texture> tankTexture,
		std::shared_ptr<fw::Texture> cannonTexture,
		std::shared_ptr<fw::Texture> missileTexture,
		fw::PhysicsSpace* physicsSpace,
		fw::Vec2f initPos,
		int pixelsPerMetre,
		SparkEmitter* sparkEmitter
	);

	inline void addGatePtr(std::shared_ptr<Gate> gate) { m_gates.push_back(gate); };

	virtual void handleInput(const fw::Input& input);
	virtual void update(const float& deltaTime);

	virtual void collisionResponse(GameObject* other);

	inline float getBoostCharge() { return m_boost.currentCharge; };

private:
	enum class InputMode
	{
		KeysAndMouse,
		Xbox
	};

	// input sub functions:
	void handleInputLinearMovement(const fw::Input& input, InputMode inputMode);
	void handleInputCannonRotation(const fw::Input& input, InputMode inputMode);
	void handleInputFireMissiles(const fw::Input& input, InputMode inputMode);

	struct Boost
	{
		float currentSpeed;
		float currentCharge;
		void update(const float& deltaTime);
		void boostNow();
	};

	Boost m_boost;

	//fw::Vec2f m_inputVelocity;

	std::vector<std::shared_ptr<Gate>> m_gates;
	bool m_paralysed;
};

