#pragma once

#include "./src/Scene.h"

class Game
{
public:
    Game()
    {

        scene.Init();
    };

    void Run()
    {
        scene.Run();
    };

    Scene scene;
};