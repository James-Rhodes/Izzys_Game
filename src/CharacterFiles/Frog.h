#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
#include "../SceneFiles/Ground.h"

class Frog : public Entity
{
public:
    Frog(Vector2 initPosition) : pos(initPosition)
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

        jointDef.bodyA = physBody;

        jointDef.stiffness = 10000.0;
        jointDef.collideConnected = false;
        jointDef.localAnchorA = b2Vec2(0, 0);
        jointDef.localAnchorB = b2Vec2(0, 0);

                // rope = (b2DistanceJoint *)ecs->GetPhysicsManager()->CreateJoint(&jointDef);

        physBody->SetLinearDamping(2);

        // animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 0, 66, 32);

        // animManager.AddAnimation("Run", {0, 1, 0, 2}, 0.3);
        // animManager.AddAnimation("Stand_Still", {0});
        // animManager.AddAnimation("Dash", {3});
        // animManager.SetState("Stand_Still");
    }

    void Update();

    void Draw();

    void UpdateController();

    void OnCollision(Entity *collidedEntity, bool detectedBySensor);
    void OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor);

    Vector2 GetPosition();

    Entity *GetNearestFly();

    Vector2 pos;
    float width = 1;
    float height = 0.5;
    float speed = 4;
    float jumpHeight = 4;
    int isOnGround = 0;

    int currDirection = 1; // 1 = right facing, -1 = left facing
    AnimationManager animManager;
    b2Fixture *feetSensor;
    b2DistanceJoint *rope;
    b2DistanceJointDef jointDef;
    bool isSwinging = false;
};