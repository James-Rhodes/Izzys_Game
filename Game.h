#pragma once
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "./src/Scene.h"

class Game
{
public:
    Game(){

        // scene.Init();
    };

    ~Game()
    {
        delete scene;
    }

    void Init()
    {
        scene = new Scene();
        scene->Init();
    }

    void Run()
    {
        scene->Run();
    };

    Scene *scene;
};