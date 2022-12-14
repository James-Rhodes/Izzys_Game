#include "raylib.h"
// #include "./src/ECS.h"
#include "Game.h"
#include <iostream>
#include <string>

// #define PLATFORM_WEB
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#ifdef _WIN32
#ifdef __cplusplus
extern "C"
{
#endif

    __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif
#endif

void UpdateDrawFrame();

const int screenWidth = 960;
const int screenHeight = 540;

Game game;
bool isFirstFrame = true;

int main()
{ // Create game object, scene object. Game contains scene. Scene contains ecs, physManager and performs run/update functions
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Izzy's Game");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Game game;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdateDrawFrame()
{
    if (isFirstFrame)
    {
        game.Init();
        isFirstFrame = false;
    }
    else
    {
        game.Run();
    }
    // game.Run();
}