#pragma once

#include <FlatWhite.hpp>

class TextInfo : public fw::GameObject
{
public:
	TextInfo(const fw::Font& font, fw::Vec2f windowSize);

protected:
	virtual void update(const float& deltaTime);

private:
	std::shared_ptr<fw::TextComponent> m_scoreTextComponent;
	std::shared_ptr<fw::TextComponent> m_difficultySettingTextComponent;

	std::string getScoreText();
	std::string getDifficultySettingText();
};