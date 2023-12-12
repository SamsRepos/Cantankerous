#pragma once

#include "Tank.hpp"

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
		std::shared_ptr<fw::Texture> sparkTexture
	);

	virtual void update(const float& deltaTime);
	virtual void render(fw::RenderTarget* window);

	virtual void collisionResponse(fw::GameObject* other);

	inline EnemyTankState getState() { return m_state; };

	inline const fw::LineSegment& getLaserLine() { return m_laserLine; };

private:
	EnemyTankState m_state;
	float m_timeToStateChange;

	float m_timeToDirectionChange;

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
	fw::LineSegment m_laserLine;
	fw::Vec2f m_laserMidPoint;
	float m_timeTargeting; // used to update laser line colour by knowing the total time targeting

	// update sub functions
	void updateNascent();
	void updateRoaming(const float& deltaTime);
	void updateTargeting(const float& deltaTime);

	void transitionToRoaming();
	void transitionToTargeting();
	
	void resetDirection();
	fw::Vec2f getRepulsion();

	void initLaser();
	void updateLaser(const float& deltaTime);
	void cleanUpLaser();

	fw::LineSegment rayCastFromCannon(bool* hitsPlayerTank = nullptr);
};

