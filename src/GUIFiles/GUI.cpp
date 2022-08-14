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
}

void GUIManager::Draw()
{
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

    const char *scoreText = TextFormat("Score: %d, Oranges: %d, Distance: %d", score, numOranges, distance);
    int fontSize = GUIUtilities::GetFontSizeFromPercent(0.06);

    int xPos = GUIUtilities::GetTextPosFromPercent({0.5f, 0.1f}, scoreText, fontSize).x;

    DrawText(scoreText, xPos, 0, fontSize, BLACK);

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
    const char *gameOverText = "Game Over";
    int fontSize = GUIUtilities::GetFontSizeFromPercent(0.25);

    Vector2 textPos = GUIUtilities::GetTextPosFromPercent({0.5f, 0.5f}, gameOverText, fontSize);

    DrawText(gameOverText, textPos.x, textPos.y, fontSize, RED);
}
