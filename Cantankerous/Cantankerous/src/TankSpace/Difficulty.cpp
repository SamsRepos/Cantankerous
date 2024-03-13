#include "Difficulty.hpp"

#include "Score.hpp"


const float NORMAL_INIT_DYNAMIC_DIFFICULTY = 0.10f;
const float NORMAL_MAX_DYNAMIC_DIFFICULTY  = 0.85f;
const int   NORMAL_SCORE_TO_MAX_DIFFICULTY = 500;

const float HARD_INIT_DYNAMIC_DIFFICULTY   = 0.4f;
const float HARD_MAX_DYNAMIC_DIFFICULTY    = 1.0f;
const int   HARD_SCORE_TO_MAX_DIFFICULTY   = 200;

const float CANTANKEROUS_DYNAMIC_DIFFICULTY = 1.f;

Difficulty::Difficulty(DifficultySetting setting, std::shared_ptr<Score> score)
	:
	m_setting(setting),
	m_score(score)
{
	switch (m_setting)
	{
	case(DifficultySetting::Normal):
		m_initDynamicDifficulty = NORMAL_INIT_DYNAMIC_DIFFICULTY;
		m_maxDynamicDifficulty  = NORMAL_MAX_DYNAMIC_DIFFICULTY;
		m_scoreToMaxDifficulty  = NORMAL_SCORE_TO_MAX_DIFFICULTY;
		break;
	case(DifficultySetting::Hard):
		m_initDynamicDifficulty = HARD_INIT_DYNAMIC_DIFFICULTY;
		m_maxDynamicDifficulty  = HARD_MAX_DYNAMIC_DIFFICULTY;
		m_scoreToMaxDifficulty  = HARD_SCORE_TO_MAX_DIFFICULTY;
		break;
	}
}


float Difficulty::getDynamicDifficulty() const
{
	if(m_setting == DifficultySetting::Cantankerous) return CANTANKEROUS_DYNAMIC_DIFFICULTY;

	int score = m_score->getScore();

	return fw::util::clamp(
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
}