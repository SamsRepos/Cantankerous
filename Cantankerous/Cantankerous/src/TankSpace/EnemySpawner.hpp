#pragma once

#include <FlatWhite.hpp>

class Gate;
class Wall;
class PlayerTank;
class Difficulty;
class SparkEmitter;
class EnemyTank;

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
		fw::PhysicsSpace* physicsSpace,
		int pixelsPerMetre,
		std::shared_ptr<PlayerTank> playerTank,
		std::shared_ptr<Difficulty> difficulty,
		fw::Rectangle gameBounds,
		std::shared_ptr<fw::Texture> sparkTexture,
		std::shared_ptr<fw::Texture> smokeTexture
	);

	void addGatePtr(std::shared_ptr<Gate> gate);

	inline std::list<std::shared_ptr<GameObject>> const& getEnemyTanks() { return m_enemyTanks; };
	
protected:
	virtual void update(const float& deltaTime);
	virtual std::list<std::shared_ptr<fw::GameObject>> lateUpdate();

private:
	void spawnEnemyNow();
	std::shared_ptr<Gate> randomAvailableGate();
	void resetTimeTillNextSpawn();

	std::list<std::shared_ptr<GameObject>> m_enemyTanks; // missiles are kept as children also, so maintaining a list of tanks separately 

	std::vector<std::shared_ptr<Gate>> m_gates;
	fw::Rectangle m_gameBoundsRect;
	std::vector<fw::LineSegment> m_gameBoundsLines;

	std::shared_ptr<fw::Texture>      m_tankTexture;
	std::shared_ptr<fw::Texture>      m_cannonTexture;
	std::shared_ptr<fw::Texture>      m_missileTexture;
	fw::PhysicsSpace* m_physicsSpace;
	int m_pixelsPerMetre;
	std::shared_ptr<PlayerTank> m_playerTank;
	std::shared_ptr<Difficulty> m_difficulty;
	std::shared_ptr<fw::Texture> m_sparkTexture;
	std::shared_ptr<fw::Texture> m_smokeTexture;

	float m_timeTillNextSpawn;

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