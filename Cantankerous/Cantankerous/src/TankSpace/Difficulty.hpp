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
	float getDynamicDifficulty() const;
	
protected:

private:
	DifficultySetting m_setting;
	std::shared_ptr<Score> m_score;

	float m_initDynamicDifficulty;
	float m_maxDynamicDifficulty;
	int   m_scoreToMaxDifficulty;
};