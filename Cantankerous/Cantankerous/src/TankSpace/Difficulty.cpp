#include "Difficulty.hpp"

#include "Score.hpp"
#include "../GlobalConsts.hpp"

const std::map<DifficultySettings, std::string> DIFFICULTY_SETTING_TO_STRING = {
	{ DifficultySettings::Normal,       GlobalConsts::NORMAL_DIFFICULTY_STR       },
	{ DifficultySettings::Hard,         GlobalConsts::HARD_DIFFICULTY_STR         },
	{ DifficultySettings::Cantankerous, GlobalConsts::CANTANKEROUS_DIFFICULTY_STR }
};

const float NORMAL_INIT_DYNAMIC_DIFFICULTY = 0.1f;
const float NORMAL_MAX_DYNAMIC_DIFFICULTY  = 0.85f;
const int   NORMAL_SCORE_TO_MAX_DIFFICULTY = 500;

const float HARD_INIT_DYNAMIC_DIFFICULTY   = 0.4f;
const float HARD_MAX_DYNAMIC_DIFFICULTY    = 1.0f;
const int   HARD_SCORE_TO_MAX_DIFFICULTY   = 200;

const float CANTANKEROUS_DYNAMIC_DIFFICULTY = 1.f;

const fw::Colour DIFFICULTY_TXT_COLOUR   = fw::Colour::Green;
const fw::Vec2f  DIFFICULTY_TXT_POSITION = fw::Vec2f(
	1920.f - 80.f, 
	1080.f - 25.f
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
	case(DifficultySettings::Normal):
		m_initDynamicDifficulty = NORMAL_INIT_DYNAMIC_DIFFICULTY;
		m_maxDynamicDifficulty  = NORMAL_MAX_DYNAMIC_DIFFICULTY;
		m_scoreToMaxDifficulty  = NORMAL_SCORE_TO_MAX_DIFFICULTY;
		break;
	case(DifficultySettings::Hard):
		m_initDynamicDifficulty = HARD_INIT_DYNAMIC_DIFFICULTY;
		m_maxDynamicDifficulty  = HARD_MAX_DYNAMIC_DIFFICULTY;
		m_scoreToMaxDifficulty  = HARD_SCORE_TO_MAX_DIFFICULTY;
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
	return "DIFFICULTY NOW: " + std::to_string(m_difficulty);
}
