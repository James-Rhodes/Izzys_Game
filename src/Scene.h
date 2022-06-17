#pragma once
#include "raylib.h"
#include "ECS.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"
#include <memory>
#include <cmath>
#include "rlgl.h"

class Box : public Entity
{
public:
    Box(Vector2 _pos, float _width, float _height) : pos(_pos), width(_width), height(_height){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);
        RectanglePhysicsObjectConfig config;
        config.pos = pos;
        config.width = width;
        config.height = height;
        config.isDynamic = true;
        config.isRollable = false;
        config.friction = 0.01;
        ecs->RegisterEntityAsPhysicsObject(id, config);
    }
    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};

        if (IsKeyPressed(KEY_SPACE))
        {
            // physBody->ApplyForceToCenter(b2Vec2(-10, 0), true);
            // physBody->SetTransform()

            physBody->SetLinearVelocity(physBody->GetLinearVelocity() + b2Vec2(-1, 0));
        }
        if (IsKeyDown(KEY_LEFT))
        {
            physBody->SetTransform(physBody->GetPosition() + b2Vec2(-0.09, 0), 0);
            physBody->SetAwake(true);
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            physBody->SetTransform(physBody->GetPosition() + b2Vec2(0.09, 0), 0);
            physBody->SetAwake(true);
        }
        if (IsKeyPressed(KEY_UP))
        {
            physBody->SetLinearVelocity(physBody->GetLinearVelocity() + b2Vec2(0, 10));
            physBody->SetAwake(true);
        }
    }

    void Draw()
    {
        Color colour = colliding ? RED : GREEN;
        DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, colour);
    };

    void OnCollision(Entity *collidedEntity)
    {
        if (collidedEntity->id == "Circle")
        {
            colliding = true;
        }
    }

    void OnCollisionEnd(Entity *collidedEntity)
    {
        if (collidedEntity->id == "Circle")
        {
            colliding = false;
        }
    }

    float width;
    float height;
    Vector2 pos;
    bool colliding = false;
};
class Circle : public Entity
{
public:
    Circle(Vector2 _pos, float _radius) : pos(_pos), radius(_radius){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);

        CirclePhysicsObjectConfig config;
        config.radius = radius;
        config.pos = pos;
        config.isDynamic = true;
        config.isRollable = false;
        config.restitution = 0.5;
        ecs->RegisterEntityAsPhysicsObject(id, config);
    }
    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    }

    void Draw()
    {
        DrawCircleV(pos, radius, RED);
    }

    Vector2 pos;
    float radius;
};

class Ground : public Entity
{
public:
    Ground(Vector2 _pos, float _width, float _height) : pos(_pos), width(_width), height(_height){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);
        RectanglePhysicsObjectConfig config;

        config.pos = pos;
        config.width = width;
        config.height = height;
        config.isDynamic = false;

        ecs->RegisterEntityAsPhysicsObject(id, config);
    }

    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    };

    void Draw()
    {
        DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, BLACK);
        DrawCircleV(pos, 0.2, RED);
        // DrawRectangleV(pos, {width, height}, BLACK);
    }

    Vector2 pos;
    float width;
    float height;
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

        screenBuffer = LoadRenderTexture(640, 360);
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
    RenderTexture2D screenBuffer;
};