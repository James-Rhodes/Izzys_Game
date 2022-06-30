#include "Frog.h"

void Frog::Update()
{
    UpdateController();
    pos = GetPosition();
}

void Frog::Draw()
{
    if (isSwinging)
    {
        // Vector2 flyPos = (Vector2){jointDef.bodyB->GetPosition().x, jointDef.bodyB->GetPosition().y};
        // DrawLineEx(pos, flyPos, 0.2, PINK);
        // DrawCircleV(flyPos, 0.1, PINK);
        tongue.Draw();
    }
    DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, GREEN);
    // DrawTexture(texture, 0, 0, RAYWHITE);
    // Texture2D texture = ecs->GetSpriteSheet();
    // Rectangle src = animManager.GetTextureRectangle();
    // // std::cout << src.x << " , " << src.y << " , " << src.width << " , " << src.height << std::endl;
    // if (currDirection == 1)
    // {
    //     DrawTexturePro(texture, src, (Rectangle){pos.x - (width / 2), pos.y + (height / 2), width, -height}, {0, 0}, 0, RAYWHITE);
    // }
    // else
    // {
    //     DrawTexturePro(texture, src, (Rectangle){pos.x - (currDirection * width / 2), pos.y + (height / 2), -width, -height}, {0, 0}, 0, RAYWHITE);
    // }
    // tongue.Draw();
}

Vector2 Frog::GetPosition()
{
    b2Vec2 tempPos = physBody->GetPosition();
    return {tempPos.x, tempPos.y};
}

void Frog::UpdateController()
{
    bool keyWasPressed = false;

    if (IsKeyDown(KEY_A))
    {
        b2Vec2 currVel = physBody->GetLinearVelocity();
        if ((-currVel.x < speed) && (isOnGround || currVel.LengthSquared() > 0.001))
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
        // animManager.SetState("Run", GetFrameTime());
        keyWasPressed = true;
    }

    if (IsKeyDown(KEY_D))
    {
        b2Vec2 currVel = physBody->GetLinearVelocity();
        if ((currVel.x < speed) && (isOnGround || currVel.LengthSquared() > 0.001))
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
        // animManager.SetState("Run", GetFrameTime());
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

        if (isSwinging)
        {
            // ecs->GetPhysicsManager()->DestroyJoint(rope);
            b2Vec2 currentVel = physBody->GetPosition();
            currentVel.Normalize();

            tongue.Delete(ecs->GetPhysicsManager());
            // float swingStrength = 10; // Fix this, it is supposed to add a little hup to the tongue dismount
            // physBody->ApplyLinearImpulseToCenter(b2Vec2(currentVel.x * swingStrength, currentVel.y * swingStrength), true);
            isInSwingDismount = true;
        }
        else
        {
            Entity *nearestFly = GetNearestFly();
            if (nearestFly != nullptr)
            {
                tongue.Create(ecs->GetPhysicsManager(), physBody, (b2Vec2){0, height / 2}, nearestFly->physBody, (b2Vec2){0, 0});
            }
        }
        isSwinging = !isSwinging;
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
    // physBody->SetGravityScale(physBody->GetLinearVelocity().y < 0 ? 13 : 8);

    if (!keyWasPressed || !isOnGround)
    {
        // animManager.SetState("Stand_Still");
    }
}

void Frog::OnCollision(Entity *collidedEntity, bool detectedBySensor)
{

    if (detectedBySensor)
    {
        isOnGround++;
    }
}
void Frog::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor)
{

    if (detectedBySensor)
    {
        isOnGround--;
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
    return closestEnt;
}

Vector2 Frog::GetSwingTangentVector(Vector2 bodyPos, Vector2 circleCenter)
{
    Vector3 outOfScreen = {0, 0, 1};
    Vector3 towardsCenter = {circleCenter.x - bodyPos.x, circleCenter.y - bodyPos.y, 0};

    Vector3 cross = Vector3CrossProduct(towardsCenter, outOfScreen);
    Vector2 result = {cross.x, cross.y};
    return Vector2Normalize(result);
}