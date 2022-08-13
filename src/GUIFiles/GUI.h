#pragma once

#ifdef RAYGUI_IMPLEMENTATION
#undef RAYGUI_IMPLEMENTATION
#endif

// #define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"

class GUIManager
{
public:
    GUIManager() = default;

    void Draw()
    {
        // GuiButton((Rectangle){30, 320, 115, 30}, "NORMAL");
    }
};