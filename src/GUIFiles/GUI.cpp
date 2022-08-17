#include "GUI.h"

namespace GUIUtilities
{
    Vector2 GetXYFromPercent(Vector2 percents)
    {
        return {percents.x * GetScreenWidth(), percents.y * GetScreenHeight()};
    }

    int GetFontSizeFromPercent(float percent)
    {
        return (int)(percent * GetScreenHeight());
    }

    Vector2 GetTextPosFromPercent(Vector2 percents, const char *text, int fontSize)
    {
        int textWidth = MeasureText(text, fontSize);

        return {GetScreenWidth() * percents.x - textWidth * 0.5f, GetScreenHeight() * percents.y - fontSize * 0.5f};
    }
    void SetFontSize(int fontSize)
    {
        GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, fontSize);
    }
}

void GUIManager::Draw()
{
    prevState = state;
    switch (state)
    {
    case (GUIStates::PLAY):
        DrawPlayScreen();
        break;

    case (GUIStates::PAUSE):
        DrawPauseScreen();
        break;

    case (GUIStates::MAIN_MENU):
        DrawMainMenuScreen();
        break;
    case (GUIStates::GAME_OVER):
        DrawGameOver();
        break;
    };
}

void GUIManager::DrawPlayScreen()
{
    CharacterManager &charManager = ecs->GetEntity<CharacterManager>("CharacterManager");
    int score = charManager.score;

    const char *scoreText = TextFormat("Score: %d", score);
    int fontSize = GUIUtilities::GetFontSizeFromPercent(0.06);

    Vector2 textPos = GUIUtilities::GetTextPosFromPercent({0.5f, 0.1f}, scoreText, fontSize);

    DrawText(scoreText, textPos.x, textPos.y, fontSize, BLACK);

    if (charManager.showGameOverScreen)
    {
        firstRenderOfGameOverScreen = true;
        renderNewHighScore = false;
        state = GUIStates::GAME_OVER;
    }
}

void GUIManager::DrawPauseScreen()
{
}

void GUIManager::DrawMainMenuScreen()
{
    const char *text = "Izzy's Game";
    int fontSize = GUIUtilities::GetFontSizeFromPercent(0.2);
    Vector2 pos = GUIUtilities::GetTextPosFromPercent({0.5, 0.3}, text, fontSize);
    DrawText(text, pos.x, pos.y, fontSize, BLACK);

    GUIUtilities::SetFontSize(GUIUtilities::GetFontSizeFromPercent(0.1));
    Vector2 playButtonPos = GUIUtilities::GetXYFromPercent({0.5, 0.65});
    Vector2 buttonDimensions = GUIUtilities::GetXYFromPercent({0.4, 0.1});

    bool playButtonPressed = GuiButton({playButtonPos.x - 0.5f * buttonDimensions.x, playButtonPos.y, buttonDimensions.x, buttonDimensions.y}, "Play");

    if (playButtonPressed)
    {
        state = GUIStates::PLAY;
    }
}

void GUIManager::DrawGameOver()
{

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {255, 255, 255, 80});

    CharacterManager &charManager = ecs->GetEntity<CharacterManager>("CharacterManager");
    int distance = charManager.distanceTravelled;
    int numOranges = charManager.numOrangesCollected;
    int score = charManager.score;

#ifdef PLATFORM_WEB
    if (firstRenderOfGameOverScreen)
    {
        prevHighScore = EM_ASM_INT({
            return GetCurrentHighScore();
        });

        if (prevHighScore < score)
        {
            EM_ASM({SaveHighScore($0)}, score);
            renderNewHighScore = true;
        }
    }
#endif

    const char *gameOverText = "Game Over";
    const char *prevHighScoreText;
    const char *scoreText;
    Color scoreColor;
    if (renderNewHighScore)
    {
        scoreText = TextFormat("NEW HIGH SCORE: %d\nNumber of Oranges Collected: %d\nDistanceTravelled: %d", score, numOranges, distance);
        scoreColor = GOLD;
        prevHighScoreText = TextFormat("");
    }
    else
    {
        scoreText = TextFormat("Score: %d\nNumber of Oranges Collected: %d\nDistanceTravelled: %d\nCurrent High Score: %d", score, numOranges, distance, prevHighScore);
        scoreColor = BLACK;
    }
    int gameOverFontSize = GUIUtilities::GetFontSizeFromPercent(0.25);
    int scoreFontSize = GUIUtilities::GetFontSizeFromPercent(0.05);

    Vector2 gameOverPos = GUIUtilities::GetTextPosFromPercent({0.5f, 0.2f}, gameOverText, gameOverFontSize);
    Vector2 scorePos = GUIUtilities::GetTextPosFromPercent({0.5, 0.35}, scoreText, scoreFontSize);

    DrawText(gameOverText, gameOverPos.x, gameOverPos.y, gameOverFontSize, RED);
    DrawText(scoreText, scorePos.x, scorePos.y, scoreFontSize, scoreColor);

    GUIUtilities::SetFontSize(GUIUtilities::GetFontSizeFromPercent(0.05));
    Vector2 restartButtonPos = GUIUtilities::GetXYFromPercent({0.5, 0.65});
    Vector2 backToMenuButtonPos = GUIUtilities::GetXYFromPercent({0.5, 0.8});
    Vector2 buttonDimensions = GUIUtilities::GetXYFromPercent({0.4, 0.1});

    bool restartButtonPressed = GuiButton({restartButtonPos.x - 0.5f * buttonDimensions.x, restartButtonPos.y, buttonDimensions.x, buttonDimensions.y}, "Restart");
    bool backToMenuButtonPressed = GuiButton({backToMenuButtonPos.x - 0.5f * buttonDimensions.x, backToMenuButtonPos.y, buttonDimensions.x, buttonDimensions.y}, "Back To Main Menu");

    if (restartButtonPressed)
    {
        state = GUIStates::PLAY;
    }
    else if (backToMenuButtonPressed)
    {
        state = GUIStates::MAIN_MENU;
    }

    firstRenderOfGameOverScreen = false;
}
