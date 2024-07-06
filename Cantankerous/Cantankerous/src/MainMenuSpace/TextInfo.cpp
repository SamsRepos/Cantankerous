#include "TextInfo.hpp"

#include "../GlobalConsts.hpp"
#include "../TankSpace/Difficulty.hpp"

const fw::Colour TEXT_COLOUR = fw::Colour::White;

TextInfo::TextInfo(const fw::Font& font, fw::Vec2f windowSize)
{
    fw::Vec2f windowHalfSize    = windowSize / 2.f;
    fw::Vec2f windowQuarterSize = windowSize / 4.f;

    fw::Vec2f scorePosition      = windowHalfSize + fw::Vec2f(-(windowQuarterSize.x), windowQuarterSize.y);
    fw::Vec2f difficultyPosition = windowHalfSize + windowQuarterSize;

    m_scoreTextComponent = std::make_shared<fw::TextComponent>(
        this,
        font,
        TEXT_COLOUR,
        scorePosition,
        getScoreText(),
        fw::OriginPoints::CENTRE
    );

    addComponent(m_scoreTextComponent);


    m_difficultySettingTextComponent = std::make_shared<fw::TextComponent>(
        this,
        font,
        TEXT_COLOUR,
        difficultyPosition,
        getDifficultySettingText(),
        fw::OriginPoints::CENTRE
    );

    addComponent(m_difficultySettingTextComponent);
}

void TextInfo::update(const float& deltaTime)
{
    m_scoreTextComponent->setContent(getScoreText());
    m_difficultySettingTextComponent->setContent(getDifficultySettingText());
}

std::string TextInfo::getScoreText()
{
    int score = fw::GlobalStore::getInt(GlobalConsts::SCORE_KEY);

    if(score == 0) return std::string();

    return "SCORE: " + std::to_string(score);
}

std::string TextInfo::getDifficultySettingText()
{
    DifficultySettings difficulty = DifficultySettings(fw::GlobalStore::getInt(GlobalConsts::DIFFICULTY_SETTING_KEY));

    return "DIFFICULTY: " + Difficulty::difficultySettingString(difficulty);
}
