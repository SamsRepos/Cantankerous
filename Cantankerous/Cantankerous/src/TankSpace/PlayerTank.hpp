#pragma once

#include "Tank.hpp"

const float PLAYERTANK_BOOST_SPEED_DECAY = 11.f; //units of velocity per second (ie. deceleration)
const float PLAYERTANK_BOOST_RECHARGE    = 0.1f;  //units of charge per second. (max charge is 1.f)
const float PLAYERTANK_BOOST_COST        = 0.3f;  //in units of boost charge

const float PLAYERTANK_GUN_RECHARGE = 0.27f; //units of charge per secont (max charge is 1.f)
const float PLAYERTANK_GUN_COST     = 0.3f; //missile cost, in units of boost charge

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
		std::shared_ptr<fw::Texture> sparkTexture,
		std::shared_ptr<fw::Texture> smokeTexture
	);

	inline void addGatePtr(std::shared_ptr<Gate> gate) { m_gates.push_back(gate); };

	inline float getBoostCharge() const { return m_boost.currentCharge; };
	inline float getGunCharge() const { return m_gunRecharge.currentCharge; };

protected:
	virtual void handleInput(const fw::Input& input);
	virtual void update(const float& deltaTime);


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

	void fireMissile(fw::Vec2f missileDirection);

	struct Boost
	{
		float currentSpeed;
		float currentCharge;
		void update(const float& deltaTime);
		void boostNow();
	};
	Boost m_boost;

	struct GunRecharge
	{
		float currentCharge;
		void update(const float& deltaTime);
		bool canFireNow();
		void fireNow();
	};
	GunRecharge m_gunRecharge;

	std::vector<std::shared_ptr<Gate>> m_gates;
	bool m_paralysed;
};

