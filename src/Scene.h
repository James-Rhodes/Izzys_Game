#pragma once
#include "raylib.h"
#include "ECS.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"
#include <memory>
#include <cmath>
#include "rlgl.h"

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

        CirclePhysicsObjectConfig config;
        config.isDynamic = true;
        config.isRollable = true;
        config.radius = radius;
        config.pos = pos;
        ecs->RegisterEntityAsPhysicsObject(id, config);
    }

    void Update() override
    {

        if (IsKeyPressed(KEY_SPACE))
        {

            if (id == "test")
            {
                ecs->RemoveEntity<testEntity>(id);
                return;
            }
        }
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    }

    void Draw() override
    {
        // DrawRectangle(pos.x, pos.y, 10, 10, RED);
        // if (!isRectangle)
        // {
        //     DrawCircle(pos.x, pos.y, 10, RED);
        // }
        // else
        // {
        //     DrawRectangle(pos.x, pos.y, 10, 10, RED);
        // }
        if (colliding)
        {
            DrawCircleV(pos, radius, RED);
        }
        else
        {
            DrawCircleV(pos, radius, PURPLE);
        }
    };

    void OnCollision(Entity *collidedEntity)
    {
        colliding = true;
    }
    void OnCollisionEnd(Entity *collidedEntity)
    {
        colliding = false;
    }

    Vector2 pos{};
    float radius;
    bool isRectangle = false;
    bool colliding = false;
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
        // ecs->RegisterEntityAsPhysicsObject(id);
        // collider = std::make_unique<RectangleCollider>(pos, width, height);
        // collider = std::make_unique<CircleCollider>(pos, 200);
        RectanglePhysicsObjectConfig config;
        config.width = width;
        config.height = height;
        config.isDynamic = false;
        config.isRollable = false;
        config.pos = pos;
        config.density = 10.f;
        config.restitution = 1.0f;
        ecs->RegisterEntityAsPhysicsObject(id, config);
    }

    void Update() override
    {
        // pos.x = (GetScreenWidth() / 2) + 20 * sin(20 * GetTime());
    }

    void Draw() override
    {
        // DrawCircle(pos.x, pos.y, 200, BLUE);
        // DrawRectangle(pos.x, pos.y, width, height, BLUE);
        DrawRectangleV(pos, {width, height}, BLUE);
    }

    void OnCollision(Entity *collidedEntity) override{
        // std::cout << collidedEntity->id << std::endl;
    };
    Vector2 pos;
    float width;
    float height;
    // Vector2 pos = {(GetScreenWidth() / 2), 300};
};

class Scene
{
public:
    Scene()
    {
        camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
        camera.target = {0, 0};
        camera.zoom = 10;
        camera.rotation = 0;

        rlDisableBackfaceCulling();

        b2Vec2 gravity(0.0f, -20.0f);
        physManager = new b2World(gravity);
        physManager->SetContactListener(&collisionManager);
        ecs.SetPhysicsManager(physManager);
    };
    ~Scene()
    {
        delete physManager;
    }

    void Init();
    void Run();
    void Update();
    void Draw();
    void UpdatePhysics();

    ECS ecs;
    b2World *physManager;
    Camera2D camera = {};
    CollisionManager collisionManager;
};