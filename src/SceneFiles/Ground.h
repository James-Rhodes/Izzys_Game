#pragma once
#include "raylib.h"
#include "../EngineFiles/ECS.h"

class Coin : public Entity
{
public:
    Coin(Vector2 _pos, float _radius) : pos(_pos), radius(_radius){};

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
        DrawCircleV(pos, radius, GOLD);
    }

    Vector2 pos;
    float radius;
};
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
            physBody->SetLinearVelocity(b2Vec2((0 * physBody->GetLinearVelocity().x), physBody->GetLinearVelocity().y));

            physBody->SetAwake(true);
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            physBody->SetTransform(physBody->GetPosition() + b2Vec2(0.09, 0), 0);

            physBody->SetLinearVelocity(b2Vec2((0 * physBody->GetLinearVelocity().x), physBody->GetLinearVelocity().y));
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

        if (collidedEntity->id == "Coin")
        {
            ecs->RemoveEntity("Coin");
        }
    }

    void OnCollisionEnd(Entity *collidedEntity)
    {
        if (collidedEntity->id == "Circle")
        {
            colliding = false;
        }
    }

    Vector2 pos;
    float width;
    float height;
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
        config.restitution = 0;

        ecs->RegisterEntityAsPhysicsObject(id, config);
    }

    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    };

    void Draw()
    {
        DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, DARKGREEN);
        // DrawRectangleV(pos, {width, height}, BLACK);
    }

    Vector2 pos;
    float width;
    float height;
};