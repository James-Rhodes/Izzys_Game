#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"

class Capy : public Entity
{
public:
    Capy(Vector2 initPosition) : pos(initPosition)
    {
    }

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);

        RectanglePhysicsObjectConfig config;
        // config.density =
        config.friction = 1.0f;
        config.isDynamic = true;
        config.isRollable = false;
        config.restitution = 0;
        config.pos = pos;
        config.width = width;
        config.height = height;

        ecs->RegisterEntityAsPhysicsObject(id, config);
    }

    void Update();

    void Draw();

    void UpdateController();

    void OnCollision(Entity *collidedEntity);
    void OnCollisionEnd(Entity *collidedEntity);

    Vector2 GetPosition();

    Vector2 pos;
    float width = 1;
    float height = 0.5;
    float speed = 3;
    float jumpHeight = 3;
    float timeOfLastDash = 0;
    float dashRechargeTime = 0.8;
    bool isOnGround = false;
};