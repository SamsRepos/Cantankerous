#include "TankSpace.h"

#include "PlayerTank.h"
#include "BoostGauge.h"
#include "Wall.h"
#include "Gate.h"
#include "EnemySpawner.h"
#include "SparkEmitter.h"

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

namespace 
{
	const fw::Rectangle& GAME_BOUNDS(const fw::Vec2f& windowSize)
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

TankSpace::TankSpace(const fw::Vec2f& windowSize, std::shared_ptr<Difficulty> difficulty)
	:
	PhysicsSpace::PhysicsSpace(
		GAME_BOUNDS(windowSize),
		TANKSPACE_PIXELS_PER_METRE,
		fw::Vec2f(0.f, 0.f)
	),
	m_difficulty(difficulty),
	m_windowSize(windowSize)
{
	fw::Vec2f halfWindowSize = windowSize / 2.f;

	auto bgObj    = std::make_shared<fw::GameObject>(halfWindowSize);
	auto bgSprite = std::make_shared<fw::SpriteComponent>(
		bgObj.get(),
		m_texManager.addTexture("bgTex", BG_TEX_PATH)
	);
	bgSprite->setSize(windowSize);
	bgObj->addComponent(bgSprite);
	addGameObject(bgObj);

	auto tankTex    = m_texManager.addTexture("tank", TANK_TEX_PATH);
	auto cannonTex  = m_texManager.addTexture("cannon", CANNON_TEX_PATH);
	auto missileTex = m_texManager.addTexture("missile", MISSILE_TEX_PATH);

#if 0
	// textures made like this render poorly in release mode
	auto sparkTex = fw::Texture::createPlainTexture(SPARK_WIDTH, SPARK_WIDTH);
	m_texManager.addTexture("spark", sparkTex);
#else
	auto sparkTex = m_texManager.addTexture("spark", SPARK_TEX_PATH);
#endif

	m_sparkEmitter = std::make_shared<SparkEmitter>(sparkTex);
	addGameObject(m_sparkEmitter);

	m_playerTank = std::make_shared<PlayerTank>(
		m_texManager.getTexture("tank"),
		m_texManager.getTexture("cannon"),
		m_texManager.getTexture("missile"),
		this,
		halfWindowSize, //PLAYER_TANK_INITIAL_POSITION,
		TANKSPACE_PIXELS_PER_METRE,
		m_sparkEmitter.get()
	);
	addGameObject(m_playerTank);


//	m_texManager.addTexture("ballSmall",  BALL_SMALL_TEX_PATH);
//	m_texManager.addTexture("ballMedium", BALL_MEDIUM_TEX_PATH);
//
//	auto ballMaker = [&](
//		std::shared_ptr<sf::Texture> texture,
//		fw::Vec2f initPos
//	){
//		return std::make_shared<Ball>(
//			texture,
//			getWorld().get(),
//			initPos,
//			TANKSPACE_PIXELS_PER_METRE
//		);
//	};
//	
//#if 0
//	for (float y = 80; y <= 720; y += 20)
//	{
//		for (float x = 200; x <= 800; x += 20)
//		{
//			addGameObject(
//				ballMaker(
//					m_texManager.getTexture("ballSmall"),
//					fw::Vec2f(x, y)
//				)
//			);
//		}
//	}
//#else
//
//	for (float y = 80; y <= 780; y += 120)
//	{
//		for (float x = 200; x <= 800; x += 120)
//		{
//			addGameObject(
//				ballMaker(
//					m_texManager.getTexture("ballMedium"),
//					fw::Vec2f(x, y)
//				)
//			);
//		}
//	}
//
//#endif

	m_enemySpawner = std::make_shared<EnemySpawner>(
		tankTex,
		cannonTex,
		missileTex,
		this,
		TANKSPACE_PIXELS_PER_METRE,
		m_playerTank,
		m_difficulty,
		getBounds(),
		m_sparkEmitter.get()
		);
	addGameObject(m_enemySpawner);

	initWallsAndGates();

	//boost gauge:
	m_boostGauge = std::make_shared<BoostGauge>(
		/*PLAYERTANK_BOOST_COST,
		fw::Vec2f(11, 11),
		fw::Vec2f(222, 33),
		2*/
	);
}

void TankSpace::update(const float& deltaTime)
{
	PhysicsSpace::update(deltaTime);
	m_boostGauge->updateHealth(m_playerTank->getBoostCharge());
}

void TankSpace::render(fw::RenderTarget* window)
{
	window->clear(sf::Color::Black);

	Space::render(window);

	m_boostGauge->render(window);
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

	float horizWallX  = horizontalTex->getSize().x;
	float horizWallY = horizontalTex->getSize().y;
	float vertWallX   = verticalTex->getSize().x;
	float vertWallY  = verticalTex->getSize().y;

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
			m_windowSize.x - ((horizWallX * 2) + (vertWallX * 2)),
			tankSize.y + horizWallY + GATE_DEPTH
		);

		fw::Rectangle gateParticleArea(
			gateSpawnArea.left,
			horizWallY,
			gateSpawnArea.width,
			GATE_DEPTH
		);

		auto gate = std::make_shared<Gate>(
			gateSpawnPos,
			fw::Vec2f::unitDown(),
			gateSpawnArea,
			gateParticleArea,
			sparkTex,
			m_playerTank
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
			tankSize.x + vertWallX + GATE_DEPTH,
			m_windowSize.y - ((vertWallY * 2.f) + (horizWallY * 2))
		);

		fw::Rectangle gateParticleArea(
			vertWallX,
			gateSpawnArea.top,
			GATE_DEPTH,
			gateSpawnArea.width
		);

		auto gate = std::make_shared<Gate>(
			gateSpawnPos,
			fw::Vec2f::unitRight(),
			gateSpawnArea,
			gateParticleArea,
			sparkTex,
			m_playerTank
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
			m_windowSize.x - (vertWallX + GATE_DEPTH),
			vertWallY + horizWallY,
			GATE_DEPTH + vertWallX + tankSize.x,
			m_windowSize.y - ((vertWallY * 2.f) + (horizWallY * 2.f))
		);

		fw::Rectangle gateParticleArea(
			m_windowSize.x - (vertWallX + GATE_DEPTH),
			gateSpawnArea.top,
			GATE_DEPTH,
			gateSpawnArea.height
		);

		auto gate = std::make_shared<Gate>(
			gateSpawnPos,
			fw::Vec2f::unitLeft(),
			gateSpawnArea,
			gateParticleArea,
			sparkTex,
			m_playerTank
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
			m_windowSize.y - (horizWallY + GATE_DEPTH),
			m_windowSize.x - ((horizWallX * 2) + (vertWallX * 2)),
			GATE_DEPTH + horizWallY + tankSize.y
		);

		fw::Rectangle gateParticleArea(
			gateSpawnArea.left,
			m_windowSize.y - (horizWallY + GATE_DEPTH),
			gateSpawnArea.width,
			GATE_DEPTH
		);

		auto gate = std::make_shared<Gate>(
			gateSpawnPos,
			fw::Vec2f::unitUp(),
			gateSpawnArea,
			gateParticleArea,
			sparkTex,
			m_playerTank
		);
		addGameObject(gate);
		m_enemySpawner->addGatePtr(gate);
		m_playerTank->addGatePtr(gate);
	}

}