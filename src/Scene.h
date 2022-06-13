#pragma once
#include "raylib.h"
#include "ECS.h"
#include "PhysicsManager.h"
#include <memory>
#include <cmath>

class testEntity : public Entity
{
public:
    testEntity(Vector2 beginPos, float _radius = 10, bool _isRectangle = false)
    {
        pos = beginPos;
        radius = _radius;
        isRectangle = _isRectangle;
    };

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);
        ecs->RegisterEntityAsPhysicsObject(id);

        // collider = std::make_unique<RectangleCollider>(pos, 10, 10);
        if (!isRectangle)
        {
            collider = std::make_unique<CircleCollider>(pos, 10);
        }
        else
        {
            collider = std::make_unique<RectangleCollider>(pos, 10, 10);
        }

        collider->SetPhysicsToDynamic();
    }

    void Update() override
    {
        // mousePos = GetMousePosition();
        // pos = GetMousePosition();
        if (IsKeyPressed(KEY_SPACE))
        {
            collider->Accelerate((Vector2){GetRandomValue(-10000, 10000), GetRandomValue(-10000, 10000)});
            std::cout << "Hello " << GetRandomValue(-10000, 10000) << std::endl;
        }
    }

    void Draw() override
    {
        // DrawRectangle(pos.x, pos.y, 10, 10, RED);
        if (!isRectangle)
        {
            DrawCircle(pos.x, pos.y, 10, RED);
        }
        else
        {
            DrawRectangle(pos.x, pos.y, 10, 10, RED);
        }
    };

    Vector2 pos{};
    float radius;
    bool isRectangle = false;
};

class StaticTest : public Entity
{
public:
    StaticTest(Vector2 _pos, float _width, float _height)
    {
        pos = _pos;
        width = _width;
        height = _height;
    }
    void Register() override
    {
        ecs->RegisterEntityAsDrawable(id);
        ecs->RegisterEntityAsPhysicsObject(id);
        collider = std::make_unique<RectangleCollider>(pos, width, height);
        // collider = std::make_unique<CircleCollider>(pos, 200);
    }

    void Update() override
    {
        // pos.x = (GetScreenWidth() / 2) + 20 * sin(20 * GetTime());
    }

    void Draw() override
    {
        // DrawCircle(pos.x, pos.y, 200, BLUE);
        DrawRectangle(pos.x, pos.y, width, height, BLUE);
    }

    Vector2 pos;
    float width;
    float height;
    // Vector2 pos = {(GetScreenWidth() / 2), 300};
};

class Scene
{
public:
    Scene(){};

    void Init();
    void Run();
    void Update();
    void Draw();
    void UpdatePhysics();

    ECS ecs;
    PhysicsManager physManager;
};