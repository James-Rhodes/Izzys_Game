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
    Pelican() = default;
    Pelican(Vector2 _pos) : pos(_pos) {}

    void Register() override;
    void Update() override;
    void Draw() override;

    void SetSpeed(float _speed);

    Vector2 pos;
    float speed = 0;
    float width = 1;
    float height = 0.3;
    float renderWidth = 1.25;
    float renderHeight = 0.6875;
    AnimationManager animManager;
};