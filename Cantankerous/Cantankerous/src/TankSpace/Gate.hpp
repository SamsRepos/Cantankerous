#pragma once

#include <FlatWhite.hpp>

class PlayerTank;

class Gate : public fw::GameObject
{
public:
	Gate(
		const fw::Vec2f& enemySpawnPosition,
		const fw::Vec2f& directionToGameSpace,
		const fw::Rectangle& spawnArea,
		const fw::Rectangle& gateArea,
		std::shared_ptr<fw::Texture> particleTexture
	);

	inline const fw::Vec2f& getSpawnPos() const { return m_spawnPos; };
	inline const fw::Vec2f& getDirectionToGameSpace() const { return m_directionToGameSpace; };
	inline const fw::Rectangle& getSpawnArea() const { return m_spawnArea; };

private:
	fw::Vec2f     m_spawnPos;
	fw::Vec2f     m_directionToGameSpace;
	fw::Rectangle m_spawnArea;

};

