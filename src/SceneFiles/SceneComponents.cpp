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
    Vector2 nextPos = {physBody->GetPosition().x, physBody->GetPosition().y};

    // if (id == "Ground0")
    // std::cout << "currentPos: " << pos.x << " , " << pos.y << " Next Pos: " << nextPos.x << " , " << nextPos.y << " Delta: " << nextPos.x - pos.x << " , " << nextPos.y - pos.y << std::endl;

    pos = nextPos;

    if (numSidePlayerCollisions != 0)
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

    DrawTextureTiledWithinCamera(ecs->GetSpriteSheet(), srcRect, (Rectangle){pos.x - (width / 2), pos.y + (height / 2) + alphaDetailOffset, width, -(height + alphaDetailOffset)}, {0, 0}, 0, 64, RAYWHITE);
}

void Ground::OnCollision(Entity *collidedEntity, bool detectedBySensor)
{
    if (detectedBySensor)
    {
        if (collidedEntity->id == "Capy")
        {
            ((Capy *)collidedEntity)->isTouchingSideOfTerrain = true;
            numSidePlayerCollisions++;
        }
        else if (collidedEntity->id == "Frog")
        {
            ((Frog *)collidedEntity)->isTouchingSideOfTerrain = true;
            numSidePlayerCollisions++;
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
            numSidePlayerCollisions--;
        }
        else if (collidedEntity->id == "Frog")
        {
            ((Frog *)collidedEntity)->isTouchingSideOfTerrain = false;
            numSidePlayerCollisions--;
        }
    }
}