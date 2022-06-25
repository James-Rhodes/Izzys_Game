#include "Capybara.h"

void Capy::Update()
{
    // controller.Update(physBody);
    UpdateController();
    pos = GetPosition();

    // std::cout<<feetSensor->
}

void Capy::Draw()
{
    // DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, BROWN);
    // DrawTexture(texture, 0, 0, RAYWHITE);
    Texture2D texture = ecs->GetSpriteSheet();
    Rectangle src = animManager.GetTextureRectangle();
    // std::cout << src.x << " , " << src.y << " , " << src.width << " , " << src.height << std::endl;
    if (currDirection == 1)
    {
        DrawTexturePro(texture, src, (Rectangle){pos.x - (width / 2), pos.y + (height / 2), width, -height}, {0, 0}, 0, RAYWHITE);
    }
    else
    {
        DrawTexturePro(texture, src, (Rectangle){pos.x - (currDirection * width / 2), pos.y + (height / 2), -width, -height}, {0, 0}, 0, RAYWHITE);
    }
}

Vector2 Capy::GetPosition()
{
    b2Vec2 tempPos = physBody->GetPosition();
    return {tempPos.x, tempPos.y};
}

void Capy::UpdateController()
{
    bool keyWasPressed = false;

    if (IsKeyDown(KEY_LEFT))
    {
        // physBody->SetLinearVelocity(b2Vec2(-speed + physBody->GetLinearVelocity().x, physBody->GetLinearVelocity().y));
        b2Vec2 currVel = physBody->GetLinearVelocity();
        bool speedLessThanMaxSpeed = (-currVel.x < speed);
        if (speedLessThanMaxSpeed)
        {
            if (isOnGround || currVel.LengthSquared() > 0.001)
            {
                physBody->ApplyForceToCenter(b2Vec2(-physBody->GetMass() * (speed / GetFrameTime()), 0), true);
            }
        }
        // b2Vec2 currentPos = physBody->GetPosition();
        // physBody->SetTransform(currentPos + b2Vec2(-speed * GetFrameTime(), 0), 0);
        // b2Vec2 newVel = physBody->GetLinearVelocity().x < 0 ? b2Vec2(physBody->GetLinearVelocity().x, physBody->GetLinearVelocity().y) : b2Vec2(0, physBody->GetLinearVelocity().y);

        // physBody->SetLinearVelocity(newVel);

        physBody->SetAwake(true);
        currDirection = -1;
        animManager.SetState("Run", GetFrameTime());
        keyWasPressed = true;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        // physBody->SetLinearVelocity(b2Vec2(speed + physBody->GetLinearVelocity().x, physBody->GetLinearVelocity().y));
        b2Vec2 currVel = physBody->GetLinearVelocity();
        bool speedLessThanMaxSpeed = (currVel.x < speed);
        if (speedLessThanMaxSpeed)
        {
            if (isOnGround || currVel.LengthSquared() > 0.001)
            {
                physBody->ApplyForceToCenter(b2Vec2(physBody->GetMass() * (speed / GetFrameTime()), 0), true);
            }
        }
        // b2Vec2 currentPos = physBody->GetPosition();
        // physBody->SetTransform(currentPos + b2Vec2(speed * GetFrameTime(), 0), 0);

        // b2Vec2 newVel = physBody->GetLinearVelocity().x >= 0 ? b2Vec2(physBody->GetLinearVelocity().x, physBody->GetLinearVelocity().y) : b2Vec2(0, physBody->GetLinearVelocity().y);
        // physBody->SetLinearVelocity(newVel);
        physBody->SetAwake(true);
        currDirection = 1;
        animManager.SetState("Run", GetFrameTime());
        keyWasPressed = true;
    }

    if (IsKeyPressed(KEY_UP))

    {
        if (isOnGround)
        {
            float gravity = physBody->GetWorld()->GetGravity().y;
            // float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * physBody->GetGravityScale() * gravity);
            float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * 8 * gravity);

            physBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpForce), true);
            keyWasPressed = true;
        }
    }

    if (IsKeyPressed(KEY_M))
    {
        if (GetTime() - timeOfLastDash > dashRechargeTime)
        {
            float dashForce = 10 * physBody->GetMass();

            physBody->ApplyLinearImpulseToCenter(b2Vec2(currDirection * dashForce, 0), true);
            timeOfLastDash = GetTime();
            animManager.SetStateLock("Dash", 0.5);
            keyWasPressed = true;
        }
    }
    physBody->SetGravityScale(physBody->GetLinearVelocity().y < 0 ? 13 : 8);
    if (!keyWasPressed || !isOnGround)
    {
        animManager.SetState("Stand_Still");
    }
    // std::cout << isOnGround << std::endl;
}

void Capy::OnCollision(Entity *collidedEntity, bool detectedBySensor)
{

    if (detectedBySensor)
    {
        isOnGround = true;
    }
}
void Capy::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor)
{

    if (detectedBySensor)
    {
        std::cout << "We Here" << std::endl;
        isOnGround = false;
    }
}