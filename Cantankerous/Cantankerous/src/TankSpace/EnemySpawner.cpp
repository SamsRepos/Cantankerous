#include "EnemySpawner.hpp"

#include "EnemyTank.hpp"
#include "Gate.hpp"
#include "Difficulty.hpp"

const int MAX_ENEMY_TANKS = 8;

const float MAX_TIME_TILL_NEXT_ENEMY_SPAWN = 5.f;
const float MIN_TIME_TILL_NEXT_ENEMY_SPAWN = 1.f;

EnemySpawner::EnemySpawner(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	fw::PhysicsSpace* physicsSpace,
	int pixelsPerMetre,
	std::shared_ptr<PlayerTank> playerTank,
	std::shared_ptr<Difficulty> difficulty,
	fw::Rectangle gameBounds,
	std::shared_ptr<fw::Texture> sparkTexture
)
	:
	m_tankTexture(tankTexture),
	m_cannonTexture(cannonTexture),
	m_missileTexture(missileTexture),
	m_physicsSpace(physicsSpace),
	m_pixelsPerMetre(pixelsPerMetre),
	m_playerTank(playerTank),
	m_difficulty(difficulty),
	m_gameBoundsRect(gameBounds),
	m_sparkTexture(sparkTexture)
{
	fw::Vec2f gameBoundsTopLeft(gameBounds.left,                        gameBounds.top);
	fw::Vec2f gameBoundsTopRight(gameBounds.left + gameBounds.width,    gameBounds.top);
	fw::Vec2f gameBoundsBottomLeft(gameBounds.left,                     gameBounds.top + gameBounds.height);
	fw::Vec2f gameBoundsBottomRight(gameBounds.left + gameBounds.width, gameBounds.top + gameBounds.height);

	m_gameBoundsLines.push_back(fw::LineSegment(gameBoundsTopLeft,    gameBoundsTopRight));
	m_gameBoundsLines.push_back(fw::LineSegment(gameBoundsTopLeft,    gameBoundsBottomLeft));
	m_gameBoundsLines.push_back(fw::LineSegment(gameBoundsTopRight,   gameBoundsBottomRight));
	m_gameBoundsLines.push_back(fw::LineSegment(gameBoundsBottomLeft, gameBoundsBottomRight));

	resetTimeTillNextSpawn();
}

void EnemySpawner::addGatePtr(std::shared_ptr<Gate> gate)
{
	m_gates.push_back(gate);
}

//void EnemySpawner::addWallPtr(std::shared_ptr<Wall> wall)
//{
//	m_walls.push_back(wall);
//}

void EnemySpawner::update(const float& deltaTime)
{
	GameObject::update(deltaTime);

	if (m_enemyTanks.empty())
	{
		spawnEnemyNow();
		resetTimeTillNextSpawn();
		return;
	}

	if (m_enemyTanks.size() >= MAX_ENEMY_TANKS) return;

	m_timeTillNextSpawn -= deltaTime;
	if (m_timeTillNextSpawn <= 0.f)
	{
		spawnEnemyNow();
		resetTimeTillNextSpawn();
	}
}

std::list<std::shared_ptr<fw::GameObject>> EnemySpawner::lateUpdate()
{
	fw::util::removeMoribundGameObjects(m_enemyTanks);

	return GameObject::lateUpdate();
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
			m_physicsSpace,
			gate.get(),
			m_pixelsPerMetre,
			m_playerTank,
			&getChildren(),
			&m_gates,
			m_gameBoundsRect,
			m_gameBoundsLines,
			m_difficulty,
			m_sparkTexture
		);
		addChild(newTank);
		m_enemyTanks.push_back(newTank);
	}
}

std::shared_ptr<Gate> EnemySpawner::randomAvailableGate()
{
	auto isGateAvailable = [&](std::shared_ptr<Gate> gate) {
		for (auto& enemyTank : m_enemyTanks)
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

void EnemySpawner::resetTimeTillNextSpawn()
{
	m_timeTillNextSpawn = fw::util::lerp(
		MAX_TIME_TILL_NEXT_ENEMY_SPAWN,
		MIN_TIME_TILL_NEXT_ENEMY_SPAWN,
		m_difficulty->getDynamicDifficulty()
	);
}