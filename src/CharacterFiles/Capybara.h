#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
class Capy : public Entity
{
public:
    Capy(Vector2 initPosition) : pos(initPosition)
    {
        // texture = LoadTexture("./assets/Capy_Sprite_Sheet.png");
        // frameSize = {(float)texture.width / 4.0f, (float)texture.height};
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
        b2PolygonShape rect;
        rect.SetAsBox(width / 2 - 0.01, height / 8, b2Vec2(0, -height / 2), 0);

        b2FixtureDef feetSensorDef;
        feetSensorDef.isSensor = true;
        feetSensorDef.shape = &rect;

        feetSensor = physBody->CreateFixture(&feetSensorDef);
        physBody->SetBullet(true);

        animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 0, 66, 32);

        animManager.AddAnimation("Run", {0, 1, 0, 2}, 0.3);
        animManager.AddAnimation("Stand_Still", {0});
        animManager.AddAnimation("Dash", {3});
        animManager.SetState("Stand_Still");
    }

    void Update();

    void Draw();

    void UpdateController();

    void OnCollision(Entity *collidedEntity, bool detectedBySensor);
    void OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor);

    Vector2 GetPosition();

    Vector2 pos;
    float width = 1;
    float height = 0.5;
    float speed = 5;
    float jumpHeight = 1.5;
    float timeOfLastDash = 0;
    float dashRechargeTime = 0.8;
    int isOnGround = 0;

    int currDirection = 1; // 1 = right facing, -1 = left facing
    AnimationManager animManager;
    b2Fixture *feetSensor;
};