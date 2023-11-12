#include "TankSpace.h"

#include "FunBall.h"
#include "Wall.h"
#include "Gate.h"

const int TANKSPACE_PIXELS_PER_METRE = 40;

const fw::Vec2f TANK_INITIAL_POSITION = fw::Vec2f(100, 300);

const std::string TANK_TEX_PATH        = "gfx/tank2.png";
const std::string CANNON_TEX_PATH      = "gfx/enemyCannon.png";
const std::string MISSILE_TEX_PATH     = "gfx/missile.png";

const std::string BALL_SMALL_TEX_PATH  = "gfx/sun_small.png";
const std::string BALL_MEDIUM_TEX_PATH = "gfx/sun_medium.png";

const std::string WALL_HORIZONTAL_TEX_PATH = "gfx/low_wall.png";
const std::string WALL_VERTICAL_TEX_PATH   = "gfx/side_wall.png";

const std::string SPARK_TEX_PATH = "gfx/spark.png";

//const std::string DOOD_TEX_PATH = "gfx/dood1.png";

TankSpace::TankSpace()
	:
	PhysicsSpace::PhysicsSpace(
		TANKSPACE_PIXELS_PER_METRE,
		fw::Vec2f(0.f, 0.f)
	)
{
	texManager.addTexture("tankTex", TANK_TEX_PATH);
	texManager.addTexture("cannonTex", CANNON_TEX_PATH);
	texManager.addTexture("missile", MISSILE_TEX_PATH);

	m_tank = std::make_shared<PlayerTank>(
		texManager.getTexture("tankTex"),
		texManager.getTexture("cannonTex"),
		texManager.getTexture("missile"),
		getWorld().get(),
		TANK_INITIAL_POSITION,
		TANKSPACE_PIXELS_PER_METRE
	);
	addGameObject(m_tank);


	texManager.addTexture("ballSmall",  BALL_SMALL_TEX_PATH);
	texManager.addTexture("ballMedium", BALL_MEDIUM_TEX_PATH);

	auto ballMaker = [&](
		std::shared_ptr<sf::Texture> texture,
		fw::Vec2f initPos
	){
		return std::make_shared<Ball>(
			texture,
			getWorld().get(),
			initPos,
			TANKSPACE_PIXELS_PER_METRE
		);
	};
	
#if 0
	for (float y = 80; y <= 720; y += 20)
	{
		for (float x = 200; x <= 800; x += 20)
		{
			addGameObject(
				ballMaker(
					texManager.getTexture("ballSmall"),
					fw::Vec2f(x, y)
				)
			);
		}
	}
#else

	for (float y = 80; y <= 780; y += 120)
	{
		for (float x = 200; x <= 800; x += 120)
		{
			addGameObject(
				ballMaker(
					texManager.getTexture("ballMedium"),
					fw::Vec2f(x, y)
				)
			);
		}
	}

#endif

	//WALLS:
	texManager.addTexture("wallHorizontal", WALL_HORIZONTAL_TEX_PATH);
	texManager.addTexture("wallVertical", WALL_VERTICAL_TEX_PATH);
	
	// low
	{
		auto wall = std::make_shared<Wall>(
			texManager.getTexture("wallHorizontal"),
			getWorld().get(),
			fw::Vec2f(200, 800),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall);
	}

	// high
	{
		auto wall = std::make_shared<Wall>(
			texManager.getTexture("wallHorizontal"),
			getWorld().get(),
			fw::Vec2f(300, 0),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall);
	}

	//left
	{
		auto wall = std::make_shared<Wall>(
			texManager.getTexture("wallVertical"),
			getWorld().get(),
			fw::Vec2f(0, 300),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall);
	}

	// right
	{
		auto wall = std::make_shared<Wall>(
			texManager.getTexture("wallVertical"),
			getWorld().get(),
			fw::Vec2f(1200, 300),
			TANKSPACE_PIXELS_PER_METRE
		);
		addGameObject(wall);
	}

	texManager.addTexture("spark", SPARK_TEX_PATH);
	
	sf::Uint8 imageArray[4];
	sf::Image pixImage;
	pixImage.create(2, 2, imageArray);
	auto pixTex = std::make_shared<fw::Texture>();
	pixTex->loadFromImage(pixImage);
	texManager.addTexture("pixel", pixTex);

	// gate:
	auto gate = std::make_shared<Gate>(
		fw::Vec2f(600, 300),
		fw::Vec2f(350, 35),
		texManager.getTexture("spark")
	);
	addGameObject(gate);

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
	m_boostGauge->update(m_tank->getBoostCharge());
}

void TankSpace::render(fw::RenderTarget* window)
{
	window->clear(sf::Color::Black);

	Space::render(window);

	m_boostGauge->render(window);
}