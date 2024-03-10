#include "Difficulty.hpp"

#include "Score.hpp"

const float NORMAL_INIT_DYNAMIC_DIFFICULTY       = 0.f;
const float HARD_INIT_DYNAMIC_DIFFICULTY         = .4f;
const float CANTANKEROUS_INIT_DYNAMIC_DIFFICULTY = 1.f;

Difficulty::Difficulty(DifficultySetting setting, std::shared_ptr<Score> score)
	:
	m_setting(setting),
	m_score(score)
{
	switch (m_setting)
	{
	case(DifficultySetting::Normal):
		m_dynamicDifficulty = NORMAL_INIT_DYNAMIC_DIFFICULTY;
		break;
	case(DifficultySetting::Hard):
		m_dynamicDifficulty = HARD_INIT_DYNAMIC_DIFFICULTY;
		break;
	case(DifficultySetting::Cantankerous):
		m_dynamicDifficulty = CANTANKEROUS_INIT_DYNAMIC_DIFFICULTY;
		break;
	default:
		throw setting;
	}
}

void Difficulty::update(const float& deltaTime)
{
	fw::GameObject::update(deltaTime);
}
