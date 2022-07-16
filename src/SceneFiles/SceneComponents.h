#pragma once
#include "raylib.h"
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
#include "../CharacterFiles/Capybara.h"
#include "../CharacterFiles/Frog.h"

class Orange : public Entity
{
public:
    Orange(Vector2 _pos) : pos(_pos){};

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

    void OnCollision(Entity *collidedEntity, bool detectedBySensor)
    {
        if (collidedEntity->id == "Capy" || collidedEntity->id == "Frog")
        {
            ecs->RemoveEntity(id);
            // Add logic for increasing the score of players perhaps in character manager
        }
    }

    Vector2 pos;
    float radius = 0.1;
};

class Ground : public Entity
{
public:
    // Pos is in world coords
    Ground(Vector2 _pos, float _width, float _height, Rectangle src, float _friction = 1) : pos(_pos), width(_width), height(_height), srcRect(src), friction(_friction){};

    void Register();

    void Update();

    void Draw();

    void OnCollision(Entity *collidedEntity, bool detectedBySensor) override;

    void OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor) override;

    Vector2 pos;
    float width;
    float height;
    Rectangle srcRect;
    float friction;
    int numSidePlayerCollisions = 0;
    b2Fixture *groundFixture;
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
        config.isSensor = true;

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