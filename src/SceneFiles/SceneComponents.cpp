#include "SceneComponents.h"

void Ground::Register()
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

    float sensorWidth = width * 0.01;
    b2PolygonShape rect;
    rect.SetAsBox(sensorWidth, height / 2, b2Vec2(-width * 0.5 + sensorWidth, 0), 0);

    b2FixtureDef sideSensors;
    sideSensors.isSensor = true;
    sideSensors.shape = &rect;

    physBody->CreateFixture(&sideSensors);

    rect.SetAsBox(sensorWidth, height / 2, b2Vec2(width * 0.5 - sensorWidth, 0), 0);
    sideSensors.shape = &rect;
    physBody->CreateFixture(&sideSensors);

    groundFixture = physBody->GetFixtureList();
    while (groundFixture->IsSensor())
    {
        groundFixture = groundFixture->GetNext(); // Gets the main ground fixture as it is the one that is not a sensor
    }

    // Below are the coords for the start pos for future reference
    // animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 97, 32, 36);
}

void Ground::Update()
{

    pos = {physBody->GetPosition().x, physBody->GetPosition().y};

    if (numSidePlayerCollisionsCapy != 0 && numSidePlayerCollisionsFrog != 0)
    {
        groundFixture->SetFriction(0);
    }
    else
    {
        groundFixture->SetFriction(friction);
    }
};

void Ground::Draw()
{

    float alphaDetailOffset = 5.0 / 64.0f; // 5 pixel offset for some more detail that has alpha back ground (can be walked through)
    Vector2 renderPos = PixelPerfectClamp(pos, 64);
    DrawTextureTiledWithinCamera(ecs->GetSpriteSheet(), srcRect, (Rectangle){renderPos.x - (width / 2.0f), renderPos.y + (height / 2.0f) + alphaDetailOffset, width, -(height + alphaDetailOffset)}, {0, 0}, 0, 64, RAYWHITE);
}

void Ground::OnCollision(Entity *collidedEntity, bool detectedBySensor)
{
    if (detectedBySensor)
    {
        if (collidedEntity->id == "Capy")
        {
            ((Capy *)collidedEntity)->isTouchingSideOfTerrain = true;
            numSidePlayerCollisionsCapy++;
        }
        else if (collidedEntity->id == "Frog")
        {
            ((Frog *)collidedEntity)->isTouchingSideOfTerrain = true;
            numSidePlayerCollisionsFrog++;
        }
    }
}

void Ground::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor)
{
    if (detectedBySensor)
    {

        if (collidedEntity->id == "Capy")
        {
            ((Capy *)collidedEntity)->isTouchingSideOfTerrain = false;
            numSidePlayerCollisionsCapy--;
        }
        else if (collidedEntity->id == "Frog")
        {
            ((Frog *)collidedEntity)->isTouchingSideOfTerrain = false;
            numSidePlayerCollisionsFrog--;
        }
    }
}