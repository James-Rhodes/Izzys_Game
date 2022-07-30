#include "raylib.h"
// #include "./src/ECS.h"
#include "Game.h"
#include <iostream>
#include <string>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame();

const int screenWidth = 640;
const int screenHeight = 360;

Game game;
bool isFirstFrame = true;

int main()
{ // Create game object, scene object. Game contains scene. Scene contains ecs, physManager and performs run/update functions
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // const int screenWidth = 640;
    // const int screenHeight = 360;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Raylib Template");

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