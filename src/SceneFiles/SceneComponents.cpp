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

void Ground::OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
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
        else if (collidedEntity->id == "CapyFrogHybrid")
        {
            ((CapyFrogHybrid *)collidedEntity)->isTouchingSideOfTerrain = true;
            numSidePlayerCollisionsCapyFrogHybrid++;
        }
    }
}

void Ground::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
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
        else if (collidedEntity->id == "CapyFrogHybrid")
        {
            ((CapyFrogHybrid *)collidedEntity)->isTouchingSideOfTerrain = false;
            numSidePlayerCollisionsCapyFrogHybrid--;
        }
    }
}

void Plank::OnPreSolve(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{
    if (collidedEntity->id == "Capy")
    {
        Capy *capy = (Capy *)collidedEntity;
        float minDistBetween = capy->height * 0.5 + height * 0.5;
        bool contactResult = capy->pos.y - physBody->GetPosition().y >= minDistBetween;
        contact->SetEnabled(contactResult);
    }
    else if (collidedEntity->id == "Frog")
    {
        Frog *frog = (Frog *)collidedEntity;
        float minDistBetween = frog->height * 0.5 + height * 0.5;
        bool contactResult = frog->pos.y - physBody->GetPosition().y >= minDistBetween;
        contact->SetEnabled(contactResult);
    }
    else if (collidedEntity->id == "CapyFrogHybrid")
    {
        CapyFrogHybrid *capyFrog = (CapyFrogHybrid *)collidedEntity;
        float minDistBetween = capyFrog->height * 0.5 + height * 0.5;
        bool contactResult = capyFrog->pos.y - physBody->GetPosition().y >= minDistBetween;
        contact->SetEnabled(contactResult);
    }
}

void BouncyPlatform::OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{

    Ground::OnCollision(collidedEntity, detectedBySensor, contact);

    if (collidedEntity->id == "Capy")
    {
        Capy *capy = (Capy *)collidedEntity;
        float minDistBetween = capy->height * 0.5 + height * 0.5;
        bool contactFromAbove = capy->pos.y - physBody->GetPosition().y >= minDistBetween;
        if (contactFromAbove)
        {
            b2Vec2 capyVel = capy->physBody->GetLinearVelocity();
            capyVel.y *= 0;
            capy->physBody->SetLinearVelocity(capyVel);
            capy->physBody->ApplyLinearImpulseToCenter(capy->physBody->GetMass() * bounceForce, true);
        }
    }
    else if (collidedEntity->id == "Frog")
    {
        Frog *frog = (Frog *)collidedEntity;
        float minDistBetween = frog->height * 0.5 + height * 0.5;
        bool contactFromAbove = frog->pos.y - physBody->GetPosition().y >= minDistBetween;
        if (contactFromAbove)
        {
            b2Vec2 frogVel = frog->physBody->GetLinearVelocity();
            frogVel.y *= 0;
            frog->physBody->SetLinearVelocity(frogVel);
            frog->physBody->ApplyLinearImpulseToCenter(frog->physBody->GetMass() * bounceForce, true);
        }
    }
    else if (collidedEntity->id == "CapyFrogHybrid")
    {
        CapyFrogHybrid *capyFrogHybrid = (CapyFrogHybrid *)collidedEntity;
        float minDistBetween = capyFrogHybrid->height * 0.5 + height * 0.5;
        bool contactFromAbove = capyFrogHybrid->pos.y - physBody->GetPosition().y >= minDistBetween;
        if (contactFromAbove)
        {
            b2Vec2 capyFrogHybridVel = capyFrogHybrid->physBody->GetLinearVelocity();
            capyFrogHybridVel.y *= 0;
            capyFrogHybrid->physBody->SetLinearVelocity(capyFrogHybridVel);
            capyFrogHybrid->physBody->ApplyLinearImpulseToCenter(capyFrogHybrid->physBody->GetMass() * bounceForce, true);
        }
    }
}

void Tree::OnPreSolve(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{
    contact->SetEnabled(!isSmashed);
}

void Tree::OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{
    Ground::OnCollision(collidedEntity, detectedBySensor, contact);

    if (collidedEntity->id == "Capy")
    {
        Capy *capy = (Capy *)collidedEntity;
        if (capy->stateWasPreviouslyLocked)
        { // Dashing
            isSmashed = true;
        }
    }
}

void Tree::Update()
{
    Ground::Update();

    if (isSmashed)
    {
        ecs->RemoveEntity(id);
    }
}