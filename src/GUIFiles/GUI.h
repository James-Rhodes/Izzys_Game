#pragma once

#ifdef RAYGUI_IMPLEMENTATION
#undef RAYGUI_IMPLEMENTATION
#endif

#include "raygui.h"
#include "raylib.h"
#include "./../EngineFiles/ECS.h"
#include "./../CharacterFiles/CharacterManager.h"

enum GUIStates
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

    Vector2 GetCenterTextPos(const char *text, int fontSize);

    GUIStates state = GUIStates::PLAY;
    ECS *ecs;
};