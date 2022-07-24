#include "Capybara.h"

void Capy::Register()
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

    animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 0, 64, 32);

    animManager.AddAnimation("Run", {0, 1, 0, 2}, 0.3);
    animManager.AddAnimation("Stand_Still", {0});
    animManager.AddAnimation("Dash", {3});
    animManager.AddAnimation("Dead", {4});
    animManager.SetState("Stand_Still");
}

void Capy::Update()
{
    UpdateController();

    pos = GetPosition();
}

void Capy::Draw()
{
    // DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, BROWN);
    // DrawTexture(texture, 0, 0, RAYWHITE);
    Vector2 renderPos = PixelPerfectClamp({pos.x - (currDirection * width / 2), pos.y + (height / 2)}, 64);
    Vector2 renderDimensions = PixelPerfectClamp({currDirection * width, -height}, 64);
    Texture2D texture = ecs->GetSpriteSheet();
    Rectangle src = animManager.GetTextureRectangle();

    DrawTexturePro(texture, src, (Rectangle){renderPos.x, renderPos.y, renderDimensions.x, renderDimensions.y},
                   {0, 0}, 0, RAYWHITE);
}

Vector2 Capy::GetPosition()
{
    b2Vec2 tempPos = physBody->GetPosition();
    return {tempPos.x, tempPos.y};
}

void Capy::UpdateController()
{
    bool keyWasPressed = false;

    if (isAlive)
    {

        if (IsKeyDown(KEY_LEFT) && (!isTouchingSideOfTerrain || isOnGround) && (animManager.GetCurrentState() != "Dash"))
        {
            b2Vec2 currVel = physBody->GetLinearVelocity();
            if (-currVel.x < speed)
            {

                physBody->ApplyForceToCenter(b2Vec2(-physBody->GetMass() * (speed / GetFrameTime()), 0), true);
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

                physBody->ApplyForceToCenter(b2Vec2(physBody->GetMass() * (speed / GetFrameTime()), 0), true);
            }

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
                float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * 8 * gravity); // 8 is the gravity scale downwards

                physBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpForce), true);
                keyWasPressed = true;
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

        if (stateWasPreviouslyLocked && !animManager.GetIsStateLocked())
        {
            // State just unlocked
            stateWasPreviouslyLocked = false;
            physBody->SetLinearVelocity({0, 0});
        }

        physBody->SetGravityScale(physBody->GetLinearVelocity().y < 0 ? 13 : 8);
        if (animManager.GetCurrentState() == "Dash")
        {
            physBody->SetGravityScale(0);
        }

        if (!keyWasPressed || !isOnGround)
        {
            animManager.SetState("Stand_Still");
        }
    }
    else
    {
        animManager.SetState("Dead");
    }
}

void Capy::OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{

    if (detectedBySensor)
    {
        isOnGround++;
    }

    if (collidedEntity->id == "Frog")
    {
        isTouchingFrog = true;
    }
}
void Capy::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{

    if (detectedBySensor)
    {
        isOnGround--;
    }

    if (collidedEntity->id == "Frog")
    {
        isTouchingFrog = false;
    }
}