#include "PlayerTank.hpp"

#include "Gate.hpp"

const float PLAYERTANK_INITIAL_ROTATION = 1.f;

const std::vector<fw::Keyboard::Key> KEYS_SIGNIFYING_INPUT_MODE_KEYS_AND_MOUSE = {
    fw::Keyboard::W, fw::Keyboard::A, fw::Keyboard::S, fw::Keyboard::D, fw::Keyboard::Space
};

const std::vector<fw::XboxButton> XBOX_BUTTONS_SIGNIFYING_INPUT_MODE_XBOX = {
    fw::XboxButton::LB, fw::XboxButton::RB,
};

PlayerTank::PlayerTank(
    std::shared_ptr<fw::Texture> tankTexture,
    std::shared_ptr<fw::Texture> cannonTexture,
    std::shared_ptr<fw::Texture> missileTexture,
    fw::PhysicsSpace* physicsSpace,
    fw::Vec2f initPos,
    int pixelsPerMetre,
    std::shared_ptr<fw::Texture> sparkTexture,
    std::shared_ptr<fw::Texture> smokeTexture
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
        sparkTexture,
        smokeTexture
    ),
    m_paralysed(false)
{
    m_boost.currentCharge = 1.f;
    m_boost.currentSpeed  = TANK_NORMAL_SPEED;

    m_gunRecharge.currentCharge = 1.f;

    m_inputMode = InputModes::Uninitialiased;
}

//
// PROTECTED:
//

void PlayerTank::handleInput(const fw::Input& input)
{
    GameObject::handleInput(input);

    checkForInputModeChange(input);
    handleInputLinearMovement(input);
    handleInputCannonRotation(input);
    handleInputFireMissiles(input);
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


//
// PRIVATE:
//

void PlayerTank::checkForInputModeChange(const fw::Input& input)
{
    if(m_inputMode == InputModes::Uninitialiased)
    {
        m_inputMode = input.isXboxControllerConnected() ? InputModes::Xbox : InputModes::KeysAndMouse;
    }

    if(input.isXboxControllerConnected() == false
    || input.isAnyKeyPressedNow(KEYS_SIGNIFYING_INPUT_MODE_KEYS_AND_MOUSE)
    || input.isMouseLeftClickedNow()
    ) 
    {
        m_inputMode = InputModes::KeysAndMouse;
        return;
    }

    if(input.isAnyXboxButtonPressedNow(XBOX_BUTTONS_SIGNIFYING_INPUT_MODE_XBOX))
    {
        m_inputMode = InputModes::Xbox;
        return;
    }
}

void PlayerTank::handleInputLinearMovement(const fw::Input& input)
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

    switch(m_inputMode)
    {
    case InputModes::KeysAndMouse:
    {
        if (input.isAnyKeyDown())
        {
            auto inputDirection = fw::Vec2f(0.f);
            if(input.isKeyDown(fw::Keyboard::W))
            {
                inputDirection += fw::Vec2f::unitUp();
            }
            if(input.isKeyDown(fw::Keyboard::S))
            {
                inputDirection += fw::Vec2f::unitDown();
            }
            if(input.isKeyDown(fw::Keyboard::A))
            {
                inputDirection += fw::Vec2f::unitLeft();
            }
            if(input.isKeyDown(fw::Keyboard::D))
            {
                inputDirection += fw::Vec2f::unitRight();
            }

            if(input.isKeyPressedNow(fw::Keyboard::Space) && !inputDirection.isZero())
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
    case InputModes::Xbox:
    {
        fw::Vec2f inputDirection = input.getXboxStick(fw::XboxStick::Left);

        if(!inputDirection.isZero())
        {
            if(input.isXboxButtonPressedNow(fw::XboxButton::LB) && !inputDirection.isZero())
            {
                m_boost.boostNow();
            }

            updateTankDirection(inputDirection);
        }
        else
        {
            stayHalted();
        }
    }
    break;

    }
    
}

void PlayerTank::handleInputCannonRotation(const fw::Input& input)
{
    switch (m_inputMode)
    {
    case InputModes::KeysAndMouse:
    {
        fw::Vec2f cannonDir = getPosition().displacementTo(input.getMousePosition());
        if (!cannonDir.isZero()) {
            updateCannonDirection(cannonDir);
        }
    }
    break;
    case InputModes::Xbox:
    {
        fw::Vec2f cannonDir = input.getXboxStick(fw::XboxStick::Right);
        if (!cannonDir.isZero()) {
            updateCannonDirection(cannonDir);
        }
    }
    break;
    }
}

void PlayerTank::handleInputFireMissiles(const fw::Input& input)
{
    switch (m_inputMode)
    {
    case InputModes::KeysAndMouse:
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
    case InputModes::Xbox:
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

