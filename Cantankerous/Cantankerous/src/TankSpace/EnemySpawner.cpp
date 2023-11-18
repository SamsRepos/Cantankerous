#include "EnemySpawner.h"

#include "EnemyTank.h"
#include "Gate.h"
#include "Difficulty.h"

//const int MAX_ENEMY_TANKS 

EnemySpawner::EnemySpawner(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	std::shared_ptr<fw::World> physicsWorld,
	int pixelsPerMetre,
	std::shared_ptr<PlayerTank> playerTank,
	std::shared_ptr<Difficulty> difficulty,
	fw::Rectangle gameBounds
)
	:
	m_tankTexture(tankTexture),
	m_cannonTexture(cannonTexture),
	m_missileTexture(missileTexture),
	m_physicsWorld(physicsWorld),
	m_pixelsPerMetre(pixelsPerMetre),
	m_playerTank(playerTank),
	m_difficulty(difficulty),
	m_gameBoundsRect(gameBounds)
{
	fw::Vec2f gameBoundsTopLeft(gameBounds.left,                        gameBounds.top);
	fw::Vec2f gameBoundsTopRight(gameBounds.left + gameBounds.width,    gameBounds.top);
	fw::Vec2f gameBoundsBottomLeft(gameBounds.left,                     gameBounds.top + gameBounds.height);
	fw::Vec2f gameBoundsBottomRight(gameBounds.left + gameBounds.width, gameBounds.top + gameBounds.height);

	m_gameBoundsLines.push_back(fw::LineSegment(gameBoundsTopLeft,    gameBoundsTopRight));
	m_gameBoundsLines.push_back(fw::LineSegment(gameBoundsTopLeft,    gameBoundsBottomLeft));
	m_gameBoundsLines.push_back(fw::LineSegment(gameBoundsTopRight,   gameBoundsBottomRight));
	m_gameBoundsLines.push_back(fw::LineSegment(gameBoundsBottomLeft, gameBoundsBottomRight));
}

void EnemySpawner::addGatePtr(std::shared_ptr<Gate> gate)
{
	m_gates.push_back(gate);
}

//void EnemySpawner::addWallPtr(std::shared_ptr<Wall> wall)
//{
//	m_walls.push_back(wall);
//}

void EnemySpawner::update(float deltaTime)
{
	GameObject::update(deltaTime);

	if (getChildren().empty())
	{
		spawnEnemyNow();
		return;
	}
}

//
// PRIVATE:
//

void EnemySpawner::spawnEnemyNow()
{
	if (auto gate = randomAvailableGate())
	{
		auto newTank = std::make_shared<EnemyTank>(
			m_tankTexture,
			m_cannonTexture,
			m_missileTexture,
			m_physicsWorld.get(),
			gate.get(),
			m_pixelsPerMetre,
			m_playerTank,
			&getChildren(),
			&m_gates,
			m_gameBoundsRect,
			m_gameBoundsLines,
			m_difficulty,
			this
		);
		addChild(newTank);
	}
}

std::shared_ptr<Gate> EnemySpawner::randomAvailableGate()
{
	auto isGateAvailable = [&](std::shared_ptr<Gate> gate) {
		for (auto& enemyTank : getChildren())
		{
			if (gate->getSpawnArea().contains(enemyTank->getPosition()))
			{
				return false;
			}
		}
		return true;
	};

	int numGates = m_gates.size();
	int initGateNum, currentGateNum;
	initGateNum = currentGateNum = rand() % numGates;

	auto gate = m_gates[initGateNum];

	do
	{
		if (isGateAvailable(gate)) return gate;

		currentGateNum++;
		currentGateNum %= numGates;
		gate = m_gates[currentGateNum];

	} while (currentGateNum != initGateNum);

	return nullptr;

}