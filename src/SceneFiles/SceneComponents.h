#pragma once
#include "raylib.h"
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"

class Orange : public Entity
{
public:
    Orange(Vector2 _pos, float _radius) : pos(_pos), radius(_radius){};

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
        DrawCircleV(pos, radius, ORANGE);
    }

    Vector2 pos;
    float radius;
};

class Ground : public Entity
{
public:
    // Pos is in world coords
    Ground(Vector2 _pos, float _width, float _height, Rectangle src, float _friction = 1) : pos(_pos), width(_width), height(_height), srcRect(src), friction(_friction){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);
        RectanglePhysicsObjectConfig config;

        config.pos = pos;
        config.width = width;
        config.height = height;
        config.isDynamic = false;
        config.restitution = 0;
        config.friction = friction;

        ecs->RegisterEntityAsPhysicsObject(id, config);
        // Below are the coords for the start pos for future reference
        // animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 97, 32, 36);
    }

    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    };

    void Draw()
    {

        float alphaDetailOffset = 5.0 / 64.0f; // 5 pixel offset for some more detail that has alpha back ground (can be walked through)

        DrawTextureTiledWithinCamera(ecs->GetSpriteSheet(), srcRect, (Rectangle){pos.x - (width / 2), pos.y + (height / 2) + alphaDetailOffset, width, -(height + alphaDetailOffset)}, {0, 0}, 0, 64, RAYWHITE);
    }

    Vector2 pos;
    float width;
    float height;
    Rectangle srcRect;
    float friction;
};

class Fly : public Entity
{
public:
    // Pos is in world coords
    Fly(Vector2 _pos) : pos(_pos){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);
        RectanglePhysicsObjectConfig config;

        config.pos = pos;
        config.width = width;
        config.height = width;
        config.isDynamic = false;
        config.restitution = 0;
        config.friction = 1;

        ecs->RegisterEntityAsPhysicsObject(id, config);
    }

    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    };

    void Draw()
    {
        // DrawTextureTiledWithinCamera(ecs->GetSpriteSheet(), srcRect, (Rectangle){pos.x - (width / 2), pos.y + (height / 2) + alphaDetailOffset, width, -(height + alphaDetailOffset)}, {0, 0}, 0, 64, RAYWHITE);
        DrawRectanglePro((Rectangle){pos.x, pos.y, width, width}, {width / 2, width / 2}, 0, BLACK);
        // Will eventually need an animation Manager once I have drawn the fly sprite
    }

    Vector2 pos;
    float width = 0.2;
};