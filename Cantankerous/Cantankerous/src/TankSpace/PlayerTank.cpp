#include "PlayerTank.h"

float PLAYERTANK_INITIAL_ROTATION = 1.f;

PlayerTank::PlayerTank(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	fw::World* world,
	fw::Vec2f initPos,
	int pixelsPerMetre,
	SparkEmitter* sparkEmitter
)
	:
	Tank(
		tankTexture,
		cannonTexture,
		missileTexture,
		world,
		initPos,
		PLAYERTANK_INITIAL_ROTATION,
		pixelsPerMetre,
		this,
		sparkEmitter
	)
{
	m_boost.currentCharge = 1.f;
	m_boost.currentSpeed  = TANK_NORMAL_SPEED;
}

void PlayerTank::handleInput(const fw::Input& input)
{
	GameObject::handleInput(input);

	handleInputLinearMovement(input);
	handleInputCannonRotation(input);
	handleInputFireMissiles(input);
}

void PlayerTank::update(float deltaTime)
{
	Tank::update(deltaTime);

	updateTankRotation(m_inputVelocity);

	m_boost.update(deltaTime);
}

void PlayerTank::collisionResponse(GameObject* other)
{
	Tank::collisionResponse(other);
}

//
// PRIVATE:
//

void PlayerTank::handleInputLinearMovement(const fw::Input& input)
{
	if (input.isAnyKeyDown())
	{
		m_inputVelocity = fw::Vec2f(0.f);
		if (input.isKeyDown(sf::Keyboard::W))
		{
			m_inputVelocity.y += -1.f;
		}
		if (input.isKeyDown(sf::Keyboard::S))
		{
			m_inputVelocity.y += 1.f;
		}
		if (input.isKeyDown(sf::Keyboard::A))
		{
			m_inputVelocity.x += -1.f;
		}
		if (input.isKeyDown(sf::Keyboard::D))
		{
			m_inputVelocity.x += 1.f;
		}

		if (input.isKeyPressedNow(sf::Keyboard::Space) && !m_inputVelocity.isZero())
		{
			m_boost.boostNow();
		}

		m_inputVelocity.normalise();
		m_inputVelocity *= m_boost.currentSpeed;

		m_body->setLinearVelocity(m_inputVelocity);

	}
	else // no key is pressed
	{
		if (m_body->getLinearVelocity().magnitudeSquared() != 0.f)
		{
			m_body->setLinearVelocity(fw::Vec2f(0.f, 0.f));
		}
	}
}

void PlayerTank::handleInputCannonRotation(const fw::Input& input)
{
	fw::Vec2f cannonDir = input.getMousePosition() - getPosition();
	float cannonAngle   = fw::util::directionToAngle(cannonDir);

	m_cannonSprite->setRotation(cannonAngle);

	//std::cout << "CannonAngle: " << cannonAngle << " TankAngle: " << tankAngle << std::endl;
}

void PlayerTank::handleInputFireMissiles(const fw::Input& input)
{
	if (input.isMouseLeftClickedNow())
	{
		fw::Vec2f cannonDir = input.getMousePosition() - getPosition();
		assert(!cannonDir.isZero());
		fireMissile(cannonDir);
	}
}

//
//  BOOST STRUCT FUNCTIONS:
//

void PlayerTank::Boost::update(float deltaTime)
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
