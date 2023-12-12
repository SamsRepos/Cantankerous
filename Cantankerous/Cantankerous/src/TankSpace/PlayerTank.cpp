#include "PlayerTank.hpp"

#include "Gate.hpp"

const float PLAYERTANK_INITIAL_ROTATION = 1.f;

PlayerTank::PlayerTank(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	fw::PhysicsSpace* physicsSpace,
	fw::Vec2f initPos,
	int pixelsPerMetre,
	std::shared_ptr<fw::Texture> sparkTexture
)
	:
	Tank(
		tankTexture,
		cannonTexture,
		missileTexture,
		physicsSpace,
		initPos,
		PLAYERTANK_INITIAL_ROTATION,
		pixelsPerMetre,
		sparkTexture
	),
	m_paralysed(false)
{
	m_boost.currentCharge = 1.f;
	m_boost.currentSpeed  = TANK_NORMAL_SPEED;

	m_gunRecharge.currentCharge = 1.f;
}

void PlayerTank::handleInput(const fw::Input& input)
{
	GameObject::handleInput(input);

	InputMode inputMode = input.isXboxControllerConnected() ? InputMode::Xbox : InputMode::KeysAndMouse;

	handleInputLinearMovement(input, inputMode);
	handleInputCannonRotation(input, inputMode);
	handleInputFireMissiles(input, inputMode);
}

void PlayerTank::update(const float& deltaTime)
{
	Tank::update(deltaTime);

	fw::Rectangle bounds = getGlobalBounds();
	bool anyGateIntersects = false;
	for(auto gate : m_gates)
	{
		fw::Rectangle gateSpawnArea = gate->getSpawnArea();
		if(gateSpawnArea.intersects(bounds))
		{
			updateTankDirection(gate->getDirectionToGameSpace());
			m_paralysed = true;
			anyGateIntersects = true;
			break;
		}
	}
	if(!anyGateIntersects) m_paralysed = false;

	m_boost.update(deltaTime);
	setSpeed(m_boost.currentSpeed);

	m_gunRecharge.update(deltaTime);
}

void PlayerTank::collisionResponse(GameObject* other)
{
	Tank::collisionResponse(other);
}


//
// PRIVATE:
//

void PlayerTank::handleInputLinearMovement(const fw::Input& input, InputMode inputMode)
{
	if (m_paralysed)
	{
		setTankTint(fw::Colour::Green);
		return;
	}
	else
	{
		setTankTint(fw::Colour::Magenta);
	}

	switch(inputMode)
	{
	case InputMode::KeysAndMouse:
	{
		if (input.isAnyKeyDown())
		{
			auto inputDirection = fw::Vec2f(0.f);
			if (input.isKeyDown(sf::Keyboard::W))
			{
				inputDirection.y += -1.f;
			}
			if (input.isKeyDown(sf::Keyboard::S))
			{
				inputDirection.y += 1.f;
			}
			if (input.isKeyDown(sf::Keyboard::A))
			{
				inputDirection.x += -1.f;
			}
			if (input.isKeyDown(sf::Keyboard::D))
			{
				inputDirection.x += 1.f;
			}

			if (input.isKeyPressedNow(sf::Keyboard::Space) && !inputDirection.isZero())
			{
				m_boost.boostNow();
			}

			updateTankDirection(inputDirection);
		}
		else // no key held down now
		{
			stayHalted();
		}
	}
	break;
	case InputMode::Xbox:
	{
		auto inputDirection = input.getXboxStick(fw::XboxStick::Left);

		if (input.isXboxButtonPressedNow(fw::XboxButton::LB) && !inputDirection.isZero())
		{
			m_boost.boostNow();
		}
		updateTankDirection(inputDirection);
	}
	break;

	}
	
}

void PlayerTank::handleInputCannonRotation(const fw::Input& input, InputMode inputMode)
{
	switch (inputMode)
	{
	case InputMode::KeysAndMouse:
	{
		fw::Vec2f cannonDir = getPosition().displacementTo(input.getMousePosition());
		if (!cannonDir.isZero()) {
			updateCannonDirection(cannonDir);
		}
	}
	break;
	case InputMode::Xbox:
	{
		fw::Vec2f cannonDir = input.getXboxStick(fw::XboxStick::Right);
		if (!cannonDir.isZero()) {
			updateCannonDirection(cannonDir);
		}
	}
	break;
	}
}

void PlayerTank::handleInputFireMissiles(const fw::Input& input, InputMode inputMode)
{
	switch (inputMode)
	{
	case InputMode::KeysAndMouse:
	{
		if (input.isMouseLeftClickedNow())
		{
			//fw::Vec2f cannonDir = input.getMousePosition() - getPosition();
			//if (cannonDir.isZero()) return;
			//fireMissile(cannonDir);
			
			fireMissile(getCannonDirection());
		}
	}
	break;
	case InputMode::Xbox:
	{
		if (input.isXboxButtonPressedNow(fw::XboxButton::RB))
		{
			//fw::Vec2f cannonDir = input.getXboxStick(fw::XboxStick::Right);
			//if (cannonDir.isZero()) return;
			//fireMissile(cannonDir);

			fireMissile(getCannonDirection());
		}
	}
	break;

	}
}

void PlayerTank::fireMissile(fw::Vec2f missileDirection)
{
	if(m_gunRecharge.canFireNow())
	{
		Tank::fireMissile(missileDirection);
		m_gunRecharge.fireNow();
	}	
}

//
//  BOOST STRUCT FUNCTIONS:
//

void PlayerTank::Boost::update(const float& deltaTime)
{
	if (currentCharge < 1.f)
	{
		currentCharge += PLAYERTANK_BOOST_RECHARGE * deltaTime;
		currentCharge = std::min(currentCharge, 1.f);
	}

	if (currentSpeed > TANK_NORMAL_SPEED)
	{
		currentSpeed -= PLAYERTANK_BOOST_SPEED_DECAY * deltaTime;
		currentSpeed = std::max(currentSpeed, TANK_NORMAL_SPEED);
	}
}

void PlayerTank::Boost::boostNow()
{
	if (currentCharge > PLAYERTANK_BOOST_COST)
	{
		currentCharge -= PLAYERTANK_BOOST_COST;
		currentSpeed = TANK_MAX_SPEED;
	}
}

void PlayerTank::GunRecharge::update(const float& deltaTime)
{
	if (currentCharge < 1.f)
	{
		currentCharge += PLAYERTANK_GUN_RECHARGE * deltaTime;
		currentCharge = std::min(currentCharge, 1.f);
	}
}

bool PlayerTank::GunRecharge::canFireNow()
{
	return (currentCharge > PLAYERTANK_GUN_COST);
}

void PlayerTank::GunRecharge::fireNow()
{
	if (currentCharge > PLAYERTANK_GUN_COST)
	{
		currentCharge -= PLAYERTANK_GUN_COST;
	}
}

