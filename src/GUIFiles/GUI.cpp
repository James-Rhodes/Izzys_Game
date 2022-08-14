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
    int distance = charManager.distanceTravelled;
    int numOranges = charManager.numOrangesCollected;
    int score = charManager.score;

    const char *scoreText = TextFormat("Score: %d", score);
    int fontSize = GUIUtilities::GetFontSizeFromPercent(0.06);

    Vector2 textPos = GUIUtilities::GetTextPosFromPercent({0.5f, 0.1f}, scoreText, fontSize);

    DrawText(scoreText, textPos.x, textPos.y, fontSize, BLACK);

    if (charManager.showGameOverScreen)
    {
        state = GUIStates::GAME_OVER;
    }
}

void GUIManager::DrawPauseScreen()
{
}

void GUIManager::DrawMainMenuScreen()
{
}

void GUIManager::DrawGameOver()
{

    CharacterManager &charManager = ecs->GetEntity<CharacterManager>("CharacterManager");
    int distance = charManager.distanceTravelled;
    int numOranges = charManager.numOrangesCollected;
    int score = charManager.score;

    const char *gameOverText = "Game Over";
    const char *scoreText = TextFormat("Score: %d\nNumber of Oranges Collected: %d\nDistanceTravelled: %d", score, numOranges, distance);

    int gameOverFontSize = GUIUtilities::GetFontSizeFromPercent(0.25);
    int scoreFontSize = GUIUtilities::GetFontSizeFromPercent(0.05);

    Vector2 gameOverPos = GUIUtilities::GetTextPosFromPercent({0.5f, 0.2f}, gameOverText, gameOverFontSize);
    Vector2 scorePos = GUIUtilities::GetTextPosFromPercent({0.5, 0.35}, scoreText, scoreFontSize);

    DrawText(gameOverText, gameOverPos.x, gameOverPos.y, gameOverFontSize, RED);
    DrawText(scoreText, scorePos.x, scorePos.y, scoreFontSize, BLACK);

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
}
