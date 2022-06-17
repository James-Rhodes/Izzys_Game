#include "raylib.h"
// #include "./src/ECS.h"
#include "./src/Game.h"
#include <iostream>
#include <string>

int main()
{ // Create game object, scene object. Game contains scene. Scene contains ecs, physManager and performs run/update functions
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    const int screenWidth = 640;
    const int screenHeight = 360;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Raylib Template");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Game game;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        game.Run();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}