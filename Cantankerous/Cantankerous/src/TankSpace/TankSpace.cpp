#include "TankSpace.hpp"

#include "PlayerTank.hpp"
#include "BoostGauge.hpp"
#include "GunGauge.hpp"
#include "Wall.hpp"
#include "Gate.hpp"
#include "EnemySpawner.hpp"
#include "SparkEmitter.hpp"
#include "Score.hpp"
#include "Difficulty.hpp"

const int TANKSPACE_PIXELS_PER_METRE = 40;

const float GATE_DEPTH = 4.f;

const std::string BG_TEX_PATH          = "gfx/backgroundDarker.png";
const std::string TANK_TEX_PATH        = "gfx/tank2.png";
const std::string CANNON_TEX_PATH      = "gfx/enemyCannon.png";
const std::string MISSILE_TEX_PATH     = "gfx/missile.png";

//const std::string BALL_SMALL_TEX_PATH  = "gfx/sun_small.png";
//const std::string BALL_MEDIUM_TEX_PATH = "gfx/sun_medium.png";

const std::string WALL_HORIZONTAL_TEX_PATH = "gfx/wall_horizontal.png";
const std::string WALL_VERTICAL_TEX_PATH   = "gfx/wall_vertical.png";

const std::string SPARK_TEX_PATH = "gfx/spark.png";
const std::string SMOKE_TEX_PATH = "gfx/smokeParticle.png";

namespace 
{
	fw::Rectangle GAME_BOUNDS(const fw::Vec2f& windowSize)
	{
		auto horizontalTex = std::make_shared<fw::Texture>();
		horizontalTex->loadFromFile(WALL_HORIZONTAL_TEX_PATH);
		float marginY = horizontalTex->getSize().y;

		auto verticalTex = std::make_shared<fw::Texture>();
		verticalTex->loadFromFile(WALL_VERTICAL_TEX_PATH);
		float marginX = verticalTex->getSize().x;

		return fw::Rectangle(
			marginX,
			marginY,
			windowSize.x - (marginX * 2.f),
			windowSize.y - (marginY * 2.f)
		);
	}
}

TankSpace::TankSpace(fw::Game* game, const fw::Vec2f& windowSize, DifficultySettings difficultySetting)
	:
	PhysicsSpace::PhysicsSpace(
		game,
		GAME_BOUNDS(windowSize),
		TANKSPACE_PIXELS_PER_METRE,
		fw::Vec2f(0.f, 0.f)
	),
	m_windowSize(windowSize),
	m_paused(false)
{
	m_score = std::make_shared<Score>();
	addGameObject(m_score);

	m_difficulty = std::make_shared<Difficulty>(
		difficultySetting,
		m_score
	);
	addGameObject(m_difficulty);

	fw::Vec2f halfWindowSize = windowSize / 2.f;

	auto bgObj    = std::make_shared<fw::GameObject>(halfWindowSize);
	auto bgSprite = std::make_shared<fw::SpriteComponent>(
		bgObj.get(),
		m_texManager.addTexture("bgTex", BG_TEX_PATH),
		-2.f
	);
	bgSprite->setSize(windowSize);
	bgObj->addComponent(bgSprite);
	addGameObject(bgObj);

	auto tankTex    = m_texManager.addTexture("tank",    TANK_TEX_PATH);
	auto cannonTex  = m_texManager.addTexture("cannon",  CANNON_TEX_PATH);
	auto missileTex = m_texManager.addTexture("missile", MISSILE_TEX_PATH);

#if 0
	// textures made like this render poorly in release mode
	auto sparkTex = fw::Texture::createPlainTexture(SPARK_WIDTH, SPARK_WIDTH);
	m_texManager.addTexture("spark", sparkTex);
#else
	auto sparkTex = m_texManager.addTexture("spark", SPARK_TEX_PATH);
#endif

	auto smokeTex = m_texManager.addTexture("smoke", SMOKE_TEX_PATH);

	m_playerTank = std::make_shared<PlayerTank>(
		m_texManager.getTexture("tank"),
		m_texManager.getTexture("cannon"),
		m_texManager.getTexture("missile"),
		this,
		halfWindowSize, //PLAYER_TANK_INITIAL_POSITION,
		TANKSPACE_PIXELS_PER_METRE,
		sparkTex,
		smokeTex
	);
	addGameObject(m_playerTank);

	m_enemySpawner = std::make_shared<EnemySpawner>(
		tankTex,
		cannonTex,
		missileTex,
		this,
		TANKSPACE_PIXELS_PER_METRE,
		m_playerTank,
		m_difficulty,
		getBounds(),
		sparkTex,
		smokeTex
	);
	addGameObject(m_enemySpawner);

	initWallsAndGates();

	m_boostGauge = std::make_shared<BoostGauge>(m_playerTank);
	addGameObject(m_boostGauge);

	auto gunGauge = std::make_shared<GunGauge>(m_playerTank);
	addGameObject(gunGauge);
}

//
// PROTECTED UPDATES:
//

void TankSpace::handleInput(const fw::Input& input)
{
	PhysicsSpace::handleInput(input);

	if (input.isKeyPressedNow(fw::Keyboard::Escape)) m_paused = !m_paused;

	if (input.isKeyPressedNow(fw::Keyboard::P))
	{
		fw::util::DumpGameObjectHierarchy(getGameObjects());
	}
}

void TankSpace::update(const float& deltaTime)
{
	if (!m_paused)
	{
		PhysicsSpace::update(deltaTime);
	}

	for (const auto& enemy : m_enemySpawner->getEnemyTanks())
	{
		if (enemy->isMoribund())
		{
			m_score->increment();
		}
	}

	if(m_playerTank->isMoribund())
	{
		this->setMoribund();
	}
}

//
// PRIVATE:
//

void TankSpace::initWallsAndGates()
{
	m_texManager.addTexture("wallHorizontal", WALL_HORIZONTAL_TEX_PATH);
	m_texManager.addTexture("wallVertical", WALL_VERTICAL_TEX_PATH);
	auto horizontalTex = m_texManager.getTexture("wallHorizontal");
	auto verticalTex = m_texManager.getTexture("wallVertical");

	auto sparkTex = m_texManager.getTexture("spark");

	fw::Vec2f tankSize(
		m_texManager.getTexture("tank")->getSize().x,
		m_texManager.getTexture("tank")->getSize().y
	);

	float horizWallX = horizontalTex->getSize().x;
	float horizWallY = horizontalTex->getSize().y;
	float vertWallX  = verticalTex->getSize().x;
	float vertWallY  = verticalTex->getSize().y;

	float halfGateDepth = GATE_DEPTH / 2.f;

	// top
	{
		auto wall = std::make_shared<Wall>(
			horizontalTex,
			this,
			fw::Vec2f(
				(horizWallX / 2.f) + vertWallX,
				(horizWallY / 2.f)
			),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall);
	}
	{
		auto wall2 = std::make_shared<Wall>(
			horizontalTex,
			this,
			fw::Vec2f(
				m_windowSize.x - ((horizWallX / 2.f) + vertWallX),
				(horizWallY / 2.f)
			),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall2);
	}
	{
		fw::Vec2f gateSpawnPos(
			(m_windowSize.x / 2.f),
			0.f - (tankSize.y / 2.f)
		);

		fw::Rectangle gateSpawnArea(
			horizWallX + vertWallX,
			0.f - tankSize.y,
			m_windowSize.x - ((horizWallX * 2.f) + (vertWallX * 2.f)),
			tankSize.y + horizWallY + halfGateDepth
		);

		fw::Rectangle gateParticleArea(
			gateSpawnArea.left,
			horizWallY - halfGateDepth,
			gateSpawnArea.width,
			halfGateDepth
		);

		auto gate = std::make_shared<Gate>(
			gateSpawnPos,
			fw::Vec2f::unitDown(),
			gateSpawnArea,
			gateParticleArea,
			sparkTex
		);
		addGameObject(gate);
		m_enemySpawner->addGatePtr(gate);
		m_playerTank->addGatePtr(gate);
	}

	//left
	{
		auto wall = std::make_shared<Wall>(
			verticalTex,
			this,
			fw::Vec2f(
				(vertWallX / 2.f),
				(vertWallY / 2.f) + horizWallY
			),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall);
	}
	{
		auto wall2 = std::make_shared<Wall>(
			verticalTex,
			this,
			fw::Vec2f(
				(vertWallX / 2.f),
				m_windowSize.y - ((vertWallY / 2.f) + horizWallY)
			),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall2);
	}
	{
		fw::Vec2f gateSpawnPos(
			0.f - (tankSize.x / 2.f),
			(m_windowSize.y / 2.f)
		);

		fw::Rectangle gateSpawnArea(
			(0.f - tankSize.x),
			vertWallY + horizWallY,
			tankSize.x + vertWallX + halfGateDepth,
			m_windowSize.y - ((vertWallY * 2.f) + (horizWallY * 2.f))
		);

		fw::Rectangle gateParticleArea(
			vertWallX - halfGateDepth,
			gateSpawnArea.top,
			GATE_DEPTH,
			gateSpawnArea.height
		);

		auto gate = std::make_shared<Gate>(
			gateSpawnPos,
			fw::Vec2f::unitRight(),
			gateSpawnArea,
			gateParticleArea,
			sparkTex
		);
		addGameObject(gate);
		m_enemySpawner->addGatePtr(gate);
		m_playerTank->addGatePtr(gate);
	}

	// right
	{
		auto wall = std::make_shared<Wall>(
			verticalTex,
			this,
			fw::Vec2f(
				m_windowSize.x - (vertWallX / 2.f),
				(vertWallY / 2.f) + horizWallY
			),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall);
	}
	{
		auto wall2 = std::make_shared<Wall>(
			verticalTex,
			this,
			fw::Vec2f(
				m_windowSize.x - (vertWallX / 2.f),
				m_windowSize.y - ((vertWallY / 2.f) + horizWallY)
			),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall2);
	}
	{
		fw::Vec2f gateSpawnPos(
			m_windowSize.x + (tankSize.x / 2.f),
			(m_windowSize.y / 2.f)
		);

		fw::Rectangle gateSpawnArea(
			m_windowSize.x - (vertWallX + halfGateDepth),
			vertWallY + horizWallY,
			GATE_DEPTH + vertWallX + tankSize.x,
			m_windowSize.y - ((vertWallY * 2.f) + (horizWallY * 2.f))
		);

		fw::Rectangle gateParticleArea(
			gateSpawnArea.left,
			gateSpawnArea.top,
			GATE_DEPTH,
			gateSpawnArea.height
		);

		auto gate = std::make_shared<Gate>(
			gateSpawnPos,
			fw::Vec2f::unitLeft(),
			gateSpawnArea,
			gateParticleArea,
			sparkTex
		);
		addGameObject(gate);
		m_enemySpawner->addGatePtr(gate);
		m_playerTank->addGatePtr(gate);
	}

	// bottom
	{
		auto wall = std::make_shared<Wall>(
			horizontalTex,
			this,
			fw::Vec2f(
				(horizWallX / 2.f) + vertWallX,
				m_windowSize.y - (horizWallY / 2.f)
			),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall);
	}
	{
		auto wall2 = std::make_shared<Wall>(
			horizontalTex,
			this,
			fw::Vec2f(
				m_windowSize.x - ((horizWallX / 2.f) + vertWallX),
				m_windowSize.y - (horizWallY / 2.f)
			),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall2);
	}
	{
		fw::Vec2f gateSpawnPos(
			(m_windowSize.x / 2.f),
			m_windowSize.y + (tankSize.y / 2.f)
		);

		fw::Rectangle gateSpawnArea(
			horizWallX + vertWallX,
			m_windowSize.y - (horizWallY + halfGateDepth),
			m_windowSize.x - ((horizWallX * 2.f) + (vertWallX * 2.f)),
			GATE_DEPTH + horizWallY + tankSize.y
		);

		fw::Rectangle gateParticleArea(
			gateSpawnArea.left,
			gateSpawnArea.top,
			gateSpawnArea.width,
			GATE_DEPTH
		);

		auto gate = std::make_shared<Gate>(
			gateSpawnPos,
			fw::Vec2f::unitUp(),
			gateSpawnArea,
			gateParticleArea,
			sparkTex
		);
		addGameObject(gate);
		m_enemySpawner->addGatePtr(gate);
		m_playerTank->addGatePtr(gate);
	}

}