#pragma once

#include <FlatWhite.hpp>

class Score : public fw::GameObject
{
public:
	Score();

private:
	int m_score;
	std::string scoreString();

	std::shared_ptr<fw::TextComponent> m_text;
	fw::Font m_font;
};