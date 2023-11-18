#pragma once

#include "Tank.h"

class PlayerTank;
class Gate;
class Wall;
class Difficulty;

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
		fw::World* world,
		Gate* spawningGate,
		int pixelsPerMetre,
		std::shared_ptr<PlayerTank> playerTank,
		std::list<std::shared_ptr<GameObject>>* enemyTanks,
		std::vector<std::shared_ptr<Gate>>* gates,
		fw::Rectangle gameBoundsRect,
		std::vector<fw::LineSegment> gameBoundsLines,
		std::shared_ptr<Difficulty> difficulty
	);

	virtual void update(float deltaTime);
	virtual void render(fw::RenderTarget* window);

	virtual void collisionResponse(fw::GameObject* other);

	inline EnemyTankState getState() { return m_state; };

private:
	EnemyTankState m_state;
	fw::Vec2f m_direction;
	float m_timeToStateChange;
	fw::World* m_world;

	std::shared_ptr<PlayerTank> m_playerTank;
	std::list<std::shared_ptr<GameObject>>* m_enemyTanks;
	std::vector<std::shared_ptr<Gate>>* m_gates;
	fw::Rectangle m_gameBoundsRect;
	std::vector<fw::LineSegment> m_gameBoundsLines;
	std::shared_ptr<Difficulty> m_difficulty;

	Gate* m_spawningGate;

	std::shared_ptr<fw::LineComponent> m_lineComponent;

	// update sub functions
	void updateNascent();
	void updateRoaming();
	void updateTargeting();

	void transitionToRoaming();
	void transitionToTargeting();

	fw::LineSegment rayCastFromCannon();
};

