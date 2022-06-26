#include "Frog.h"

void Frog::Update()
{
    UpdateController();
    pos = GetPosition();
}

void Frog::Draw()
{
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

            physBody->ApplyForceToCenter(b2Vec2(-physBody->GetMass() * (speed / GetFrameTime()), 0), true);
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

            physBody->ApplyForceToCenter(b2Vec2(physBody->GetMass() * (speed / GetFrameTime()), 0), true);
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
        if (GetTime() - timeOfLastDash > dashRechargeTime)
        {
            float dashForce = 10 * physBody->GetMass();

            physBody->ApplyLinearImpulseToCenter(b2Vec2(currDirection * dashForce, 0), true);
            timeOfLastDash = GetTime();
            // animManager.SetStateLock("Dash", 0.5);
            keyWasPressed = true;
        }
    }
    physBody->SetGravityScale(physBody->GetLinearVelocity().y < 0 ? 13 : 8);
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