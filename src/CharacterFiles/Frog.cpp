#include "Frog.h"

// void PrintVector2(Vector2 vec)
// {
//     std::cout << vec.x << " , " << vec.y << std::endl;
// }

void Frog::Register()
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
    rect.SetAsBox(width / 2 - 0.02, height / 8, b2Vec2(0, -height / 2), 0);

    b2FixtureDef feetSensorDef;
    feetSensorDef.isSensor = true;
    feetSensorDef.shape = &rect;

    feetSensor = physBody->CreateFixture(&feetSensorDef);
    physBody->SetBullet(true);

    physBody->SetLinearDamping(2);

    animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 33, 32, 64);

    animManager.AddAnimation("Run", {0, 1, 0, 2}, 0.3);
    animManager.AddAnimation("Stand_Still", {0});
    animManager.AddAnimation("Swing", {3});
    animManager.AddAnimation("Dead", {4});
    animManager.SetState("Stand_Still");

    tongue = Tongue(3);
    tongue.SetBeginBody(physBody, (b2Vec2){0, height / 6});

    screenScrollSpeed = &ecs->GetEntity<TerrainManager>("TerrainManager").sceneScrollSpeed;
}

void Frog::Update()
{
    UpdateController();
    Vector2 nextPos = GetPosition();
    float remainingPhysTime = *ecs->GetFrameData<float>();
    renderPos = Vector2Add(Vector2Scale(nextPos, remainingPhysTime), Vector2Scale(pos, 1.0 - remainingPhysTime));

    // renderPos = Vector2Lerp(nextPos, pos, remainingPhysTime);
    // std::cout << "Vectors:" << std::endl;
    // PrintVector2(pos);
    // PrintVector2(nextPos);

    // PrintVector2(renderPos);

    pos = nextPos;
}

void Frog::Draw()
{

    // DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, GREEN);
    renderPos = PixelPerfectClamp(renderPos, 64);

    Texture2D texture = ecs->GetSpriteSheet();
    Rectangle src = animManager.GetTextureRectangle();
    if (currDirection == 1)
    {
        DrawTexturePro(texture, src, (Rectangle){renderPos.x - (width / 2), renderPos.y + (height / 2), width, -height}, {0, 0}, 0, RAYWHITE);
    }
    else
    {
        DrawTexturePro(texture, src, (Rectangle){renderPos.x - (currDirection * width / 2), renderPos.y + (height / 2), -width, -height}, {0, 0}, 0, RAYWHITE);
    }

    tongue.Draw();
}

Vector2 Frog::GetPosition()
{
    b2Vec2 tempPos = physBody->GetPosition();
    return {tempPos.x, tempPos.y};
}

void Frog::UpdateController()
{
    bool keyWasPressed = false;
    if (isAlive)
    {

        if (IsKeyDown(KEY_A) && (!isTouchingSideOfTerrain || isOnGround))
        {
            b2Vec2 currVel = physBody->GetLinearVelocity();
            if (-currVel.x < speed)
            {
                b2Vec2 force;

                if (isSwinging)
                {
                    Vector2 tangentialForce = Vector2Scale(GetSwingTangentVector(pos, tongue.GetEndPos()), -swingStrength * physBody->GetMass() * (speed / GetFrameTime()));

                    force = tangentialForce.x <= 0 ? b2Vec2(tangentialForce.x, tangentialForce.y) : b2Vec2(0, 0);
                }
                else
                {
                    force = b2Vec2(-physBody->GetMass() * (speed / GetFrameTime()), 0);
                }

                physBody->ApplyForceToCenter(force, true);
            }

            currDirection = -1;
            animManager.SetState("Run", GetFrameTime());
            keyWasPressed = true;
        }

        if (IsKeyDown(KEY_D) && (!isTouchingSideOfTerrain || isOnGround))
        {
            b2Vec2 currVel = physBody->GetLinearVelocity();
            if (currVel.x < speed)
            {
                b2Vec2 force;

                if (isSwinging)
                {
                    Vector2 tangentialForce = Vector2Scale(GetSwingTangentVector(pos, tongue.GetEndPos()), swingStrength * physBody->GetMass() * (speed / GetFrameTime()));

                    force = tangentialForce.x >= 0 ? b2Vec2(tangentialForce.x, tangentialForce.y) : b2Vec2(0, 0);
                }
                else
                {
                    force = b2Vec2(physBody->GetMass() * (speed / GetFrameTime()), 0);
                }
                physBody->ApplyForceToCenter(force, true);
            }

            currDirection = 1;
            animManager.SetState("Run", GetFrameTime());
            keyWasPressed = true;
        }

        if (IsKeyPressed(KEY_W))

        {
            if (isOnGround)
            {
                float gravity = physBody->GetWorld()->GetGravity().y;
                // float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * physBody->GetGravityScale() * gravity);
                float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * 8 * gravity); // 8 is the gravity scale downwards

                physBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpForce), true);
                keyWasPressed = true;
            }
        }

        if (IsKeyPressed(KEY_C))
        {
            if (!capybaraIsOnHead)
            {
                if (isSwinging)
                {
                    b2Vec2 currentVel = physBody->GetPosition();
                    currentVel.Normalize();

                    tongue.Delete(ecs->GetPhysicsManager());

                    isInSwingDismount = true;
                    isSwinging = false;
                }
                else
                {
                    bool succeededInTongueExtension = false;
                    Entity *nearestFly = GetNearestFly();
                    if (nearestFly != nullptr)
                    {
                        float flySign = nearestFly->physBody->GetPosition().x - physBody->GetPosition().x;
                        int flyDirection = (flySign > 0) - (flySign < 0);

                        if (flyDirection == currDirection)
                        {
                            tongue.Create(ecs->GetPhysicsManager(), nearestFly->physBody, (b2Vec2){0, 0});
                            isSwinging = !isSwinging;
                            succeededInTongueExtension = true;
                        }
                    }

                    if (!succeededInTongueExtension)
                    {
                        tongue.SetDrawFalseExtension(currDirection);
                        // To Fix.
                    }
                }
            }
        }

        if (isInSwingDismount && !isOnGround)
        { // In air after dismount
            physBody->SetGravityScale(7);
        }
        else if (isInSwingDismount && isOnGround) // landed after swing dismount
        {
            isInSwingDismount = false;
        }
        else
        {
            physBody->SetGravityScale(physBody->GetLinearVelocity().y < 0 ? 13 : 8);
        }

        if (!keyWasPressed || !isOnGround)
        {
            animManager.SetState("Stand_Still");
        }
        if (isSwinging)
        {
            animManager.SetState("Swing");
        }
    }
    else
    {
        animManager.SetState("Dead");
    }
}

void Frog::OnCollision(Entity *collidedEntity, bool detectedBySensor)
{

    if (detectedBySensor)
    {
        isOnGround++;
    }

    if (collidedEntity->id == "Capy")
    {
        capybaraIsOnHead = collidedEntity->physBody->GetPosition().y > (pos.y + height / 2);
    }
}
void Frog::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor)
{

    if (detectedBySensor)
    {
        isOnGround--;
    }

    if (collidedEntity->id == "Capy")
    {
        capybaraIsOnHead = false;
    }
}

Entity *Frog::GetNearestFly()
{
    float minDistSquared = INFINITY;
    Entity *closestEnt = nullptr;

    for (auto ent : ecs->GetAllPhysicsObjects())
    {
        if (ent->id.rfind("Fly", 0) == 0)
        {
            float currentDistanceSq = b2DistanceSquared(physBody->GetPosition(), ent->physBody->GetPosition());
            if (currentDistanceSq < minDistSquared)
            {
                minDistSquared = currentDistanceSq;
                closestEnt = ent;
            }
        }
    }
    return minDistSquared < maxDistFromFlyToSwing * maxDistFromFlyToSwing ? closestEnt : nullptr;
}

Vector2 Frog::GetSwingTangentVector(Vector2 bodyPos, Vector2 circleCenter)
{
    Vector3 outOfScreen = {0, 0, 1};
    Vector3 towardsCenter = {circleCenter.x - bodyPos.x, circleCenter.y - bodyPos.y, 0};

    Vector3 cross = Vector3CrossProduct(towardsCenter, outOfScreen);
    Vector2 result = {cross.x, cross.y};
    return Vector2Normalize(result);
}