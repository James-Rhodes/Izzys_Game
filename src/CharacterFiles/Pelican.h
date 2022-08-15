#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
#include "../SceneFiles/TerrainManager.h"
#include <iostream>

class Pelican : public Entity
{
public:
    Pelican(Vector2 _pos, float _speed) : pos(_pos), speed(_speed) {}

    void Register() override;
    void Update() override;
    void Draw() override;

    Vector2 pos;
    float speed = 0;
    float width = 1;
    float height = 0.5;
    float renderWidth = 1.25;
    float renderHeight = 0.6875;
    AnimationManager animManager;
};