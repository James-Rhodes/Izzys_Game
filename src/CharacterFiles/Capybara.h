#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "CharacterController.h"

class CapyController : public CharacterController
{
public:
    CapyController() = default;
    CapyController(float _speed, float _jumpHeight) : speed(_speed), jumpHeight(_jumpHeight){};

    CharacterStates Update(b2Body *physBody) override
    {
        CharacterStates state = STANDING_STILL;
        if (IsKeyDown(KEY_LEFT))
        {
            b2Vec2 currentPos = physBody->GetPosition();
            physBody->SetTransform(currentPos + b2Vec2(-speed * GetFrameTime(), 0), 0);
            b2Vec2 newVel = physBody->GetLinearVelocity().x < 0 ? b2Vec2(physBody->GetLinearVelocity().x, physBody->GetLinearVelocity().y) : b2Vec2(0, physBody->GetLinearVelocity().y);

            physBody->SetLinearVelocity(newVel);

            physBody->SetAwake(true);
            state = RUNNING_LEFT;
        }

        if (IsKeyDown(KEY_RIGHT))
        {
            b2Vec2 currentPos = physBody->GetPosition();
            physBody->SetTransform(currentPos + b2Vec2(speed * GetFrameTime(), 0), 0);

            b2Vec2 newVel = physBody->GetLinearVelocity().x >= 0 ? b2Vec2(physBody->GetLinearVelocity().x, physBody->GetLinearVelocity().y) : b2Vec2(0, physBody->GetLinearVelocity().y);
            physBody->SetLinearVelocity(newVel);
            physBody->SetAwake(true);
            state = RUNNING_RIGHT;
        }

        if (IsKeyPressed(KEY_UP))

        {
            if (physBody->GetLinearVelocity().y == 0)
            {
                float gravity = physBody->GetWorld()->GetGravity().y;
                float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * physBody->GetGravityScale() * gravity);
                physBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpForce), true);
                state = JUMPING;
            }
        }

        if (IsKeyPressed(KEY_M))
        {
            if (GetTime() - timeOfLastDash > dashRechargeTime)
            {
                float dashForce = 10 * physBody->GetMass();

                physBody->ApplyLinearImpulseToCenter(b2Vec2(dashForce, 0), true);
                timeOfLastDash = GetTime();
            }
        }

        physBody->SetGravityScale(physBody->GetLinearVelocity().y < 0 ? 6 : 4);

        return state;
    };

    float speed = 1;
    float jumpHeight = 1;
    float timeOfLastDash = 0;
    float dashRechargeTime = 0.8;
};
class Capy : public Entity
{
public:
    Capy(Vector2 initPosition) : pos(initPosition), controller(speed, jumpHeight)
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

    Vector2 GetPosition();

    Vector2 pos;
    float width = 1;
    float height = 0.5;
    float speed = 3;
    float jumpHeight = 3;

    CapyController controller;
};