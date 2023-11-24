#pragma once

#include "Tank.h"

class PlayerTank;
class Gate;
class Wall;
class Difficulty;
class EnemySpawner;

enum class EnemyTankState
{
	Nascent,
	Roaming,
	Targeting
};

class EnemyTank : public Tank
{
public:
	EnemyTank(
		std::shared_ptr<fw::Texture> tankTexture,
		std::shared_ptr<fw::Texture> cannonTexture,
		std::shared_ptr<fw::Texture> missileTexture,
		fw::PhysicsSpace* physicsSpace,
		Gate* spawningGate,
		int pixelsPerMetre,
		std::shared_ptr<PlayerTank> playerTank,
		std::list<std::shared_ptr<GameObject>>* enemyTanks,
		std::vector<std::shared_ptr<Gate>>* gates,
		fw::Rectangle gameBoundsRect,
		std::vector<fw::LineSegment> gameBoundsLines,
		std::shared_ptr<Difficulty> difficulty,
		GameObject* parentForSpawnedMissiles,
		SparkEmitter* sparkEmitter
	);

	virtual void update(const float& deltaTime);
	virtual void render(fw::RenderTarget* window);

	virtual void collisionResponse(fw::GameObject* other);

	inline EnemyTankState getState() { return m_state; };

private:
	EnemyTankState m_state;
	fw::Vec2f m_direction;
	fw::Vec2f m_heading;
	float m_timeToStateChange;
	float m_timeToHeadingChange;
	fw::PhysicsSpace* m_physicsSpace;

	std::shared_ptr<PlayerTank> m_playerTank;
	std::list<std::shared_ptr<GameObject>>* m_enemyTanks;
	std::vector<std::shared_ptr<Gate>>* m_gates;
	fw::Rectangle m_gameBoundsRect;
	std::vector<fw::LineSegment> m_gameBoundsLines;
	Gate* m_spawningGate;

	std::shared_ptr<Difficulty> m_difficulty;

	// laser
	std::shared_ptr<fw::LineComponent> m_lineComponent;
	float m_timeTargeting; // used to update laser line colour by knowing the total time targeting
	fw::Vec2f m_laserMidPoint;

	// update sub functions
	void updateNascent();
	void updateRoaming(const float& deltaTime);
	void updateTargeting(const float& deltaTime);

	void transitionToRoaming();
	void transitionToTargeting();
	
	void resetHeading();
	fw::Vec2f getRepulsion();

	void initLaser();
	void updateLaser(const float& deltaTime);
	void cleanUpLaser();

	fw::LineSegment rayCastFromCannon(bool* hitsPlayerTank = nullptr);
};

