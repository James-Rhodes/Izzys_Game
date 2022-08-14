#pragma once
#define RAYGUI_IMPLEMENTATION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wenum-compare"
#include "raygui.h"
#pragma GCC diagnostic pop

#include "./src/Scene.h"

class Game
{
public:
    Game(){

    };

    ~Game()
    {
        delete scene;
    }

    void Init()
    {
        scene = new Scene();
    }

    void Run()
    {
        scene->Run();
    };

    Scene *scene;
};