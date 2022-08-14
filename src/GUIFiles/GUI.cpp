#include "GUI.h"

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
    int fontSize = 0.06 * GetScreenHeight();

    int xPos = GetCenterTextPos(scoreText, fontSize).x;

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
    int fontSize = 0.25 * GetScreenHeight();

    Vector2 textPos = GetCenterTextPos(gameOverText, fontSize);

    DrawText(gameOverText, textPos.x, textPos.y, fontSize, RED);
}

Vector2 GUIManager::GetCenterTextPos(const char *text, int fontSize)
{
    int textWidth = MeasureText(text, fontSize);

    return {GetScreenWidth() * 0.5f - textWidth * 0.5f, GetScreenHeight() * 0.5f - fontSize * 0.5f};
}
