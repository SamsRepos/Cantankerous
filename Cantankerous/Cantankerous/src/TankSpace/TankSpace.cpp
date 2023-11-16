#include "TankSpace.h"

#include "PlayerTank.h"
#include "BoostGauge.h"
#include "Wall.h"
#include "Gate.h"
#include "EnemySpawner.h"

const int TANKSPACE_PIXELS_PER_METRE = 40;

//const fw::Vec2f PLAYER_TANK_INITIAL_POSITION = fw::Vec2f(250, 300);

const std::string TANK_TEX_PATH        = "gfx/tank2.png";
const std::string CANNON_TEX_PATH      = "gfx/enemyCannon.png";
const std::string MISSILE_TEX_PATH     = "gfx/missile.png";

//const std::string BALL_SMALL_TEX_PATH  = "gfx/sun_small.png";
//const std::string BALL_MEDIUM_TEX_PATH = "gfx/sun_medium.png";

const std::string WALL_HORIZONTAL_TEX_PATH = "gfx/wall_horizontal.png";
const std::string WALL_VERTICAL_TEX_PATH   = "gfx/wall_vertical.png";

const std::string SPARK_TEX_PATH = "gfx/spark.png";

TankSpace::TankSpace(const fw::Vec2f& windowSize, std::shared_ptr<Difficulty> difficulty)
	:
	PhysicsSpace::PhysicsSpace(
		TANKSPACE_PIXELS_PER_METRE,
		fw::Vec2f(0.f, 0.f)
	),
	m_difficulty(difficulty)
{
	auto tankTex    = m_texManager.addTexture("tank", TANK_TEX_PATH);
	auto cannonTex  = m_texManager.addTexture("cannon", CANNON_TEX_PATH);
	auto missileTex = m_texManager.addTexture("missile", MISSILE_TEX_PATH);

	m_playerTank = std::make_shared<PlayerTank>(
		m_texManager.getTexture("tank"),
		m_texManager.getTexture("cannon"),
		m_texManager.getTexture("missile"),
		getWorld().get(),
		(windowSize / 2.f), //PLAYER_TANK_INITIAL_POSITION,
		TANKSPACE_PIXELS_PER_METRE
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
		getWorld(),
		TANKSPACE_PIXELS_PER_METRE,
		m_playerTank,
		m_difficulty
	);
	addGameObject(m_enemySpawner);

	//WALLS AND GATES:
	{
		m_texManager.addTexture("wallHorizontal", WALL_HORIZONTAL_TEX_PATH);
		m_texManager.addTexture("wallVertical", WALL_VERTICAL_TEX_PATH);
		auto horizontalTex = m_texManager.getTexture("wallHorizontal");
		auto verticalTex = m_texManager.getTexture("wallVertical");

		sf::Uint8 imageArray[4];
		sf::Image pixImage;
		pixImage.create(2, 2, imageArray);
		auto pixTex = std::make_shared<fw::Texture>();
		pixTex->loadFromImage(pixImage);
		m_texManager.addTexture("pixel", pixTex);

		fw::Vec2f tankSize(
			m_texManager.getTexture("tank")->getSize().x,
			m_texManager.getTexture("tank")->getSize().y
		);

		// high
		{
			auto wall = std::make_shared<Wall>(
				horizontalTex,
				getWorld().get(),
				fw::Vec2f(
					(horizontalTex->getSize().x / 2.f) + verticalTex->getSize().x,
					(horizontalTex->getSize().y / 2.f)
				),
				TANKSPACE_PIXELS_PER_METRE
			);
			addGameObject(wall);
			m_enemySpawner->addWallPtr(wall);
		}
		{
			auto wall2 = std::make_shared<Wall>(
				horizontalTex,
				getWorld().get(),
				fw::Vec2f(
					windowSize.x - ((horizontalTex->getSize().x / 2.f) + verticalTex->getSize().x),
					(horizontalTex->getSize().y / 2.f)
				),
				TANKSPACE_PIXELS_PER_METRE
			);
			addGameObject(wall2);
			m_enemySpawner->addWallPtr(wall2);
		}
		{
			// gate
			fw::Vec2f gateSpawnPos(
				(windowSize.x / 2.f),
				0.f - (tankSize.y / 2.f)
			);

			fw::Rectangle gateSpawnArea(
				horizontalTex->getSize().x + verticalTex->getSize().x,
				0.f - tankSize.y,
				((windowSize.x / 2.f) - (horizontalTex->getSize().x + verticalTex->getSize().x)) * 2.f,
				tankSize.y * 1.5f
			);

			fw::Rectangle gateParticleArea(
				horizontalTex->getSize().x + verticalTex->getSize().x,
				horizontalTex->getSize().y,
				((windowSize.x / 2.f) - (horizontalTex->getSize().x + verticalTex->getSize().x)) * 2.f,
				4
			);

			auto gate = std::make_shared<Gate>(
				gateSpawnPos,
				fw::Vec2f(0.f, 1.f),
				gateSpawnArea,
				gateParticleArea,
				m_texManager.getTexture("pixel")
			);
			addGameObject(gate);
			m_enemySpawner->addGatePtr(gate);
		}

		//// low
		//{
		//	auto wall = std::make_shared<Wall>(
		//		m_texManager.getTexture("wallHorizontal"),
		//		getWorld().get(),
		//		fw::Vec2f(200, 800),
		//		TANKSPACE_PIXELS_PER_METRE
		//	);
		//	addGameObject(wall);
		//}

		////left
		//{
		//	auto wall = std::make_shared<Wall>(
		//		m_texManager.getTexture("wallVertical"),
		//		getWorld().get(),
		//		fw::Vec2f(0, 300),
		//		TANKSPACE_PIXELS_PER_METRE
		//	);
		//	addGameObject(wall);
		//}

		//// right
		//{
		//	auto wall = std::make_shared<Wall>(
		//		m_texManager.getTexture("wallVertical"),
		//		getWorld().get(),
		//		fw::Vec2f(1200, 300),
		//		TANKSPACE_PIXELS_PER_METRE
		//	);
		//	addGameObject(wall);
		//}

		//m_texManager.addTexture("spark", SPARK_TEX_PATH);

	}
	

	//boost gauge:
	m_boostGauge = std::make_shared<BoostGauge>(
		/*PLAYERTANK_BOOST_COST,
		fw::Vec2f(11, 11),
		fw::Vec2f(222, 33),
		2*/
	);
}

void TankSpace::update(float deltaTime)
{
	PhysicsSpace::update(deltaTime);
	m_boostGauge->update(m_playerTank->getBoostCharge());
}

void TankSpace::render(fw::RenderTarget* window)
{
	window->clear(sf::Color::Black);

	Space::render(window);

	m_boostGauge->render(window);
}