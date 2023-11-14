#include "EnemySpawner.h"

#include "EnemyTank.h"
#include "Gate.h"

//const int MAX_ENEMY_TANKS 

EnemySpawner::EnemySpawner(
	std::shared_ptr<fw::Texture> tankTexture,
	std::shared_ptr<fw::Texture> cannonTexture,
	std::shared_ptr<fw::Texture> missileTexture,
	std::shared_ptr<fw::World> physicsWorld,
	int pixelsPerMetre,
	std::shared_ptr<PlayerTank> playerTank
)
	:
	m_tankTexture(tankTexture),
	m_cannonTexture(cannonTexture),
	m_missileTexture(missileTexture),
	m_physicsWorld(physicsWorld),
	m_pixelsPerMetre(pixelsPerMetre),
	m_playerTank(playerTank)
{

}

void EnemySpawner::addGatePtr(std::shared_ptr<Gate> gate)
{
	m_gates.push_back(gate);
}

void EnemySpawner::setDifficulty(float difficulty) // 0.f <= difficulty <= 1.f
{
	m_difficulty = fw::util::clamp(0.f, difficulty, 1.f);
}

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
			gate->getSpawnPos(),
			gate->getDirectionToGameSpace(),
			m_pixelsPerMetre,
			m_playerTank
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