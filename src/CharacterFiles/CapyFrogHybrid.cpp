#include "CapyFrogHybrid.h"

void CapyFrogHybrid::Register()
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

    animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 149, 64, 88);

    animManager.AddAnimation("Run", {0, 1, 0, 2}, 0.3);
    animManager.AddAnimation("Stand_Still", {0});
    animManager.AddAnimation("Swing", {3});
    animManager.AddAnimation("Dash", {4});
    animManager.AddAnimation("Dead", {5});

    animManager.SetState("Stand_Still");

    tongue = Tongue(3);
    tongue.SetBeginBody(physBody, (b2Vec2){0, 3 * height / 12});
}

void CapyFrogHybrid::Update()
{
    UpdateController();
    pos = GetPosition();
}

void CapyFrogHybrid::Draw()
{

    // DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, BLUE);
    Vector2 renderPos = PixelPerfectClamp({pos.x - (currDirection * width / 2), pos.y + (height / 2)}, 64);
    Vector2 renderDimensions = PixelPerfectClamp({currDirection * width, -height}, 64);
    Texture2D texture = ecs->GetSpriteSheet();
    Rectangle src = animManager.GetTextureRectangle();

    DrawTexturePro(texture, src, (Rectangle){renderPos.x, renderPos.y, renderDimensions.x, renderDimensions.y},
                   {0, 0}, 0, RAYWHITE);
    tongue.Draw();
}

Vector2 CapyFrogHybrid::GetPosition()
{
    b2Vec2 tempPos = physBody->GetPosition();
    return {tempPos.x, tempPos.y};
}

void CapyFrogHybrid::UpdateController()
{
    bool keyWasPressed = false;
    if (isAlive)
    {

        if (IsKeyDown(KEY_LEFT) && (!isTouchingSideOfTerrain || isOnGround) && (animManager.GetCurrentState() != "Dash"))
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

        if (IsKeyDown(KEY_RIGHT) && (!isTouchingSideOfTerrain || isOnGround) && (animManager.GetCurrentState() != "Dash"))
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

        if (IsKeyPressed(KEY_W) && isOnGround)

        {

            float gravity = physBody->GetWorld()->GetGravity().y;
            // float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * physBody->GetGravityScale() * gravity);
            float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * 8 * gravity); // 8 is the gravity scale downwards

            physBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpForce), true);
            keyWasPressed = true;
        }

        if (IsKeyPressed(KEY_C))
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

        if (IsKeyPressed(KEY_M))
        {
            if (GetTime() - timeOfLastDash > dashRechargeTime)
            {
                float dashForce = 20 * physBody->GetMass();
                b2Vec2 newVel = b2Vec2(0, 0);
                physBody->SetLinearVelocity(newVel);
                physBody->ApplyLinearImpulseToCenter(b2Vec2(currDirection * dashForce, 0), true);
                timeOfLastDash = GetTime();
                animManager.SetStateLock("Dash", 0.2);
                keyWasPressed = true;
                stateWasPreviouslyLocked = true;
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
        if (animManager.GetCurrentState() == "Dash")
        {
            physBody->SetGravityScale(0);
        }

        if (stateWasPreviouslyLocked && !animManager.GetIsStateLocked())
        {
            // State just unlocked
            stateWasPreviouslyLocked = false;
            physBody->SetLinearVelocity({0, 0});
        }
    }
    else
    {
        animManager.SetState("Dead");
    }
}

void CapyFrogHybrid::OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{

    if (detectedBySensor)
    {
        isOnGround++;
    }
}
void CapyFrogHybrid::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{

    if (detectedBySensor)
    {
        isOnGround--;
    }
}

Entity *CapyFrogHybrid::GetNearestFly()
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

Vector2 CapyFrogHybrid::GetSwingTangentVector(Vector2 bodyPos, Vector2 circleCenter)
{
    Vector3 outOfScreen = {0, 0, 1};
    Vector3 towardsCenter = {circleCenter.x - bodyPos.x, circleCenter.y - bodyPos.y, 0};

    Vector3 cross = Vector3CrossProduct(towardsCenter, outOfScreen);
    Vector2 result = {cross.x, cross.y};
    return Vector2Normalize(result);
}