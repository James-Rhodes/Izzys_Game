#pragma once

#ifdef RAYGUI_IMPLEMENTATION
#undef RAYGUI_IMPLEMENTATION
#endif

#include "raygui.h"
#include "raylib.h"
#include "./../EngineFiles/ECS.h"
#include "./../CharacterFiles/CharacterManager.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace GUIUtilities
{
    Vector2 GetXYFromPercent(Vector2 percents);

    int GetFontSizeFromPercent(float percent);

    Vector2 GetTextPosFromPercent(Vector2 percents, const char *text, int fontSize);

    void SetFontSize(int fontSize);
};

enum class GUIStates
{
    PLAY,
    PAUSE,
    MAIN_MENU,
    GAME_OVER
};

class GUIManager
{
public:
    GUIManager() = default;
    GUIManager(ECS *_ecs) : ecs(_ecs){};

    void Draw();

    void DrawPlayScreen();
    void DrawPauseScreen();
    void DrawMainMenuScreen();
    void DrawGameOver();

    GUIStates state = GUIStates::MAIN_MENU;
    GUIStates prevState = GUIStates::MAIN_MENU;
    ECS *ecs;

    int prevHighScore = 0;
    bool firstRenderOfGameOverScreen = true;
    bool renderNewHighScore = false;
};