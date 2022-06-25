#include "Frog.h"

void Frog::Update()
{
    // controller.Update(physBody);
    UpdateController();
    pos = GetPosition();
}

void Frog::Draw()
{
    DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, GREEN);
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
        b2Vec2 currentPos = physBody->GetPosition();
        physBody->SetTransform(currentPos + b2Vec2(-speed * GetFrameTime(), 0), 0);
        b2Vec2 newVel = physBody->GetLinearVelocity().x < 0 ? b2Vec2(physBody->GetLinearVelocity().x, physBody->GetLinearVelocity().y) : b2Vec2(0, physBody->GetLinearVelocity().y);

        physBody->SetLinearVelocity(newVel);

        physBody->SetAwake(true);
        currDirection = -1;
        keyWasPressed = true;
    }

    if (IsKeyDown(KEY_D))
    {
        b2Vec2 currentPos = physBody->GetPosition();
        physBody->SetTransform(currentPos + b2Vec2(speed * GetFrameTime(), 0), 0);

        b2Vec2 newVel = physBody->GetLinearVelocity().x >= 0 ? b2Vec2(physBody->GetLinearVelocity().x, physBody->GetLinearVelocity().y) : b2Vec2(0, physBody->GetLinearVelocity().y);
        physBody->SetLinearVelocity(newVel);
        physBody->SetAwake(true);
        currDirection = 1;
        keyWasPressed = true;
    }

    if (IsKeyPressed(KEY_W))

    {
        if (isOnGround)
        {
            float gravity = physBody->GetWorld()->GetGravity().y;
            float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * 8 * gravity);
            std::cout << jumpForce << " , " << physBody->GetGravityScale() << std::endl;
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
            keyWasPressed = true;
        }
    }
    physBody->SetGravityScale(physBody->GetLinearVelocity().y < 0 ? 13 : 8);
    if (!keyWasPressed || !isOnGround)
    {
    }
}

void Frog::OnCollision(Entity *collidedEntity, bool detectedBySensor)
{
    if (detectedBySensor)
    {
        isOnGround = true;
    }
}
void Frog::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor)
{
    if (detectedBySensor)
    {
        isOnGround = false;
    }
}