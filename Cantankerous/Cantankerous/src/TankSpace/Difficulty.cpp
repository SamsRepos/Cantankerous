#include "Difficulty.hpp"

#include "Score.hpp"
#include "../GlobalConsts.hpp"

const std::map<DifficultySettings, std::string> DIFFICULTY_SETTING_TO_STRING = {
	{ DifficultySettings::TooEasy,      GlobalConsts::DIFFICULTY_STR_TOO_EASY     },
	{ DifficultySettings::SlowCook,     GlobalConsts::DIFFICULTY_STR_SLOW_COOK    },
	{ DifficultySettings::HardEnough,   GlobalConsts::DIFFICULTY_STR_HARD_ENOUGH  },
	{ DifficultySettings::Cantankerous, GlobalConsts::DIFFICULTY_STR_CANTANKEROUS }
};

// 0.6 difficulty good to start for normal difficulty
// 0.8 is a good general level for hard difficulty

const float INIT_DYNAMIC_DIFFICULTY_TOO_EASY = 0.2f;
const float MAX_DYNAMIC_DIFFICULTY_TOO_EASY  = 0.5f;
const int   SCORE_TO_MAX_DIFFICULTY_TOO_EASY = 300;

const float INIT_DYNAMIC_DIFFICULTY_SLOW_COOK = 0.1f;
const float MAX_DYNAMIC_DIFFICULTY_SLOW_COOK  = 1.f;
const int   SCORE_TO_MAX_DIFFICULTY_SLOW_COOK = 500;

const float INIT_DYNAMIC_DIFFICULTY_HARD_ENOUGH = 0.6f;
const float MAX_DYNAMIC_DIFFICULTY_HARD_ENOUGH  = 0.8f;
const int   HARD_SCORE_TO_MAX_DIFFICULTY        = 200;

const float CANTANKEROUS_DYNAMIC_DIFFICULTY = 1.f;

const fw::Colour DIFFICULTY_TXT_COLOUR   = fw::Colour::Green;
const fw::Vec2f  DIFFICULTY_TXT_POSITION = fw::Vec2f(
	1920.f - 80.f, 
	1080.f - 10.f
);

Difficulty::Difficulty(
	DifficultySettings setting, 
	std::shared_ptr<Score> score,
	const fw::Font& font
)
	:
	m_setting(setting),
	m_score(score),
	m_font(font)
{

	switch (m_setting)
	{
	case(DifficultySettings::TooEasy):
		m_initDynamicDifficulty = INIT_DYNAMIC_DIFFICULTY_TOO_EASY;
		m_maxDynamicDifficulty  = MAX_DYNAMIC_DIFFICULTY_TOO_EASY;
		m_scoreToMaxDifficulty  = SCORE_TO_MAX_DIFFICULTY_TOO_EASY;
		break;
	case(DifficultySettings::SlowCook):
		m_initDynamicDifficulty = INIT_DYNAMIC_DIFFICULTY_SLOW_COOK;
		m_maxDynamicDifficulty  = MAX_DYNAMIC_DIFFICULTY_SLOW_COOK;
		m_scoreToMaxDifficulty  = SCORE_TO_MAX_DIFFICULTY_SLOW_COOK;
		break;
	case(DifficultySettings::HardEnough):
		m_initDynamicDifficulty = INIT_DYNAMIC_DIFFICULTY_HARD_ENOUGH;
		m_maxDynamicDifficulty  = MAX_DYNAMIC_DIFFICULTY_HARD_ENOUGH;
		m_scoreToMaxDifficulty  = HARD_SCORE_TO_MAX_DIFFICULTY;
		break;
	case(DifficultySettings::Cantankerous):
		m_difficulty = CANTANKEROUS_DYNAMIC_DIFFICULTY;
		break;
	}

	m_text = std::make_shared<fw::TextComponent>(
		this,
		m_font,
		DIFFICULTY_TXT_COLOUR,
		DIFFICULTY_TXT_POSITION,
		difficultyString(),
		fw::OriginPoints::BOTTOM_RIGHT
	);
	m_text->setCharacterSize(20);
	addComponent(m_text);
}


float Difficulty::getDynamicDifficulty()
{
	if(m_setting == DifficultySettings::Cantankerous) return CANTANKEROUS_DYNAMIC_DIFFICULTY;

	int score = fw::GlobalStore::getInt(GlobalConsts::SCORE_KEY);

	m_difficulty = fw::util::clamp(
		0.f,
		fw::util::lerp(
			m_initDynamicDifficulty,
			m_maxDynamicDifficulty,
			fw::util::inverseLerp(
				0.f,
				m_scoreToMaxDifficulty,
				score
			)
		),
		1.f
	);

	m_text->setContent(difficultyString());
	return m_difficulty;
}

std::string Difficulty::difficultySettingString(DifficultySettings difficultySetting)
{
	return DIFFICULTY_SETTING_TO_STRING.at(difficultySetting);
}

//
// PRIVATE:
//

std::string Difficulty::difficultyString()
{
	return "difficulty setting: " + DIFFICULTY_SETTING_TO_STRING.at(m_setting) + "\n" + 
		   "difficulty now: " + std::to_string(m_difficulty);
}
