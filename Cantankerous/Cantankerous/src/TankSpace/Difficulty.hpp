#pragma once

#include <FlatWhite.hpp>

class Score;

enum class DifficultySetting
{
	Normal,
	Hard,
	Cantankerous
};

class Difficulty : public fw::GameObject
{
public:
	
	Difficulty(DifficultySetting setting, std::shared_ptr<Score> score);
	
	// 0.f <= dynamic difficulty <= 1.f
	// - Normal:       gradual increase from 0 to 1
	// - Hard:         will start higher, and increase faster
	// - Cantankerous: always 1
	inline float getDynamicDifficulty() const { return m_dynamicDifficulty; };
	
protected:
	virtual void update(const float& deltaTime);

private:
	DifficultySetting m_setting;
	std::shared_ptr<Score> m_score;

	float m_dynamicDifficulty;  // 0.f <= dynamic difficulty <= 1.f
};