#pragma once

#include <FlatWhite.h>

class Gate;
class PlayerTank;
class Difficulty;

class EnemySpawner : public fw::GameObject
{
public:
	//static EnemySpawner& getInstance()
	//{
	//	static EnemySpawner instance;
	//	return instance;
	//}

	EnemySpawner(
		std::shared_ptr<fw::Texture> tankTexture,
		std::shared_ptr<fw::Texture> cannonTexture,
		std::shared_ptr<fw::Texture> missileTexture,
		std::shared_ptr<fw::World> physicsWorld,
		int pixelsPerMetre,
		std::shared_ptr<PlayerTank> playerTank,
		std::shared_ptr<Difficulty> difficulty
	);

	void addGatePtr(std::shared_ptr<Gate> gate);
	virtual void update(float deltaTime);
	
private:
	void spawnEnemyNow();
	std::shared_ptr<Gate> randomAvailableGate();

	std::vector<std::shared_ptr<Gate>> m_gates;

	std::shared_ptr<fw::Texture> m_tankTexture;
	std::shared_ptr<fw::Texture> m_cannonTexture;
	std::shared_ptr<fw::Texture> m_missileTexture;
	std::shared_ptr<fw::World> m_physicsWorld;
	int m_pixelsPerMetre;
	std::shared_ptr<PlayerTank> m_playerTank;
	std::shared_ptr<Difficulty> m_difficulty;

	// singleton:
//private:
//	EnemySpawner() {};
//	/*EnemySpawner(EnemySpawner const&);
//	void operator=(EnemySpawner const&);*/
//
//public:
//	EnemySpawner(EnemySpawner const&)   = delete;
//	void operator=(EnemySpawner const&) = delete;
//	friend std::unique_ptr<EnemySpawner> std::make_unique<EnemySpawner>();

	
};