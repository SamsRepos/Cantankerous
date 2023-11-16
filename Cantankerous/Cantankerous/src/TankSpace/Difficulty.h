#pragma once

enum class DifficultySetting
{
	Normal,
	Hard,
	Cantankerous
};

class Difficulty
{
public:
	Difficulty(DifficultySetting setting);
	
	void update(float deltaTime);

	// 0.f <= dynamic difficulty <= 1.f
	// - Normal:       gradual increase from 0 to 1
	// - Hard:         will start higher, and increase faster
	// - Cantankerous: always 1
	inline float getDynamicDifficulty() const { return m_dynamicDifficulty; };
	
private:
	DifficultySetting m_setting;
	float m_dynamicDifficulty;  // 0.f <= dynamic difficulty <= 1.f
};