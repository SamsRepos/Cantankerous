#pragma once

#include <FlatWhite.hpp>

class Score;

enum class DifficultySettings
{
	Normal,
	Hard,
	Cantankerous
};

class Difficulty : public fw::GameObject
{
public:
	
	Difficulty(
		DifficultySettings setting, 
		std::shared_ptr<Score> score,
		const fw::Font& font
	);
	
	// 0.f <= dynamic difficulty <= 1.f
	float getDynamicDifficulty();

	static std::string difficultySettingString(DifficultySettings difficultySetting);
	
protected:


private:
	float m_difficulty;

	DifficultySettings m_setting;
	std::shared_ptr<Score> m_score;

	std::shared_ptr<fw::TextComponent> m_text;
	fw::Font m_font;
	std::string difficultyString();

	float m_initDynamicDifficulty;
	float m_maxDynamicDifficulty;
	int   m_scoreToMaxDifficulty;
};