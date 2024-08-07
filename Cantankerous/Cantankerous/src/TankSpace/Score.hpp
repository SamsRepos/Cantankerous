#pragma once

#include <FlatWhite.hpp>

const int DEFAULT_INCREMENT_AMOUNT = 10;
const int DEFAULT_DECREMENT_AMOUNT = 10;

class Score : public fw::GameObject
{
public:
	Score(
		const fw::Font& font,
		int defaultIncrementAmount = DEFAULT_INCREMENT_AMOUNT,
		int defaultDecrementAmount = DEFAULT_DECREMENT_AMOUNT
	);
	void increment();
	void increment(int amount);
	void decrement();
	void decrement(int amount);

private:
	static std::string scoreString(int score);

	std::shared_ptr<fw::TextComponent> m_text;
	fw::Font m_font;

	int m_defaultIncrementAmount;
	int m_defaultDecrementAmount;
};