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
    config.width = width * 0.5;
    config.height = height - 0.05;

    ecs->RegisterEntityAsPhysicsObject(id, config);
    b2PolygonShape rect;
    rect.SetAsBox(width / 2 - 0.02, height / 8, b2Vec2(0, -height / 2), 0);

    b2FixtureDef feetSensorDef;
    feetSensorDef.isSensor = true;
    feetSensorDef.shape = &rect;

    feetSensor = physBody->CreateFixture(&feetSensorDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.25;
    circleShape.m_p = {width / 4, 0};

    b2FixtureDef roundingCircles;
    roundingCircles.friction = 1;
    roundingCircles.restitution = 0;
    roundingCircles.density = 1;
    roundingCircles.shape = &circleShape;

    physBody->CreateFixture(&roundingCircles);
    circleShape.m_p = {-width / 4, 0};
    physBody->CreateFixture(&roundingCircles);

    physBody->SetBullet(true);

    physBody->SetLinearDamping(2);

    animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 0, 64, 32);

    animManager.AddAnimation("Run", {0, 1, 0, 2}, 0.1);
    animManager.AddAnimation("Stand_Still", {0});
    animManager.AddAnimation("Dash", {3});
    animManager.AddAnimation("Dead", {4});
    animManager.SetState("Stand_Still");
}

void Capy::Update()
{
    if (isAlive)
    {
        if (PositionIsValid() && !hitPelican)
        {
            coyoteTimer.Update();
            UpdateController();
        }
        else
        {
            OnDeath();
        }
    }
    pos = GetPosition();
}

void Capy::Draw()
{
    // DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, BROWN);
    // DrawTexture(texture, 0, 0, RAYWHITE);
    Vector2 renderPos = PixelPerfectClamp({pos.x, pos.y}, 64);

    Vector2 renderDimensions = PixelPerfectClamp({currDirection * width, -height}, 64);
    Texture2D texture = ecs->GetSpriteSheet();
    Rectangle src = animManager.GetTextureRectangle();
    Vector2 offset = {width * 0.5f + ((float)(currDirection == -1) * -width), -height * 0.5f};
    float angle = physBody->GetAngle() * RAD2DEG;

    DrawTexturePro(texture, src, (Rectangle){renderPos.x, renderPos.y, renderDimensions.x, renderDimensions.y},
                   offset, angle, RAYWHITE);
}

Vector2 Capy::GetPosition()
{
    b2Vec2 tempPos = physBody->GetPosition();
    return {tempPos.x, tempPos.y};
}

void Capy::UpdateController()
{
    bool keyWasPressed = false;
    jumpWasPressed = false;

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

    if (IsKeyPressed(KEY_UP) && (isOnGround || !coyoteTimer.isFinished()))

    {
        jumpWasPressed = true;
        DoJump();
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
    physBody->SetLinearDamping(2);
    if (animManager.GetCurrentState() == "Dash")
    {
        physBody->SetGravityScale(0);
        physBody->SetLinearDamping(0);
    }

    if (!keyWasPressed || !isOnGround)
    {
        animManager.SetState("Stand_Still");
    }
}

void Capy::DoJump()
{
    float gravity = physBody->GetWorld()->GetGravity().y;
    // float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * physBody->GetGravityScale() * gravity);
    float jumpForce = physBody->GetMass() * sqrt(jumpHeight * -2 * 8 * gravity); // 8 is the gravity scale downwards

    physBody->ApplyLinearImpulseToCenter(b2Vec2(0, jumpForce), true);
}

void Capy::OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{

    if (detectedBySensor)
    {
        isOnGround++;
    }

    if (collidedEntity->id == "Frog")
    {
        isTouchingFrog++;
    }
    else if (collidedEntity->id == "Pelican" && isAlive)
    {
        OnDeath();
    }
    else if (collidedEntity->id.rfind("Orange", 0) == 0)
    {
        Orange *orange = (Orange *)collidedEntity;
        if (!orange->isConsumed)
        {
            (*orangeScore)++;
        }
    }
}
void Capy::OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{

    if (detectedBySensor)
    {
        isOnGround--;
        if (isOnGround == 0 && !jumpWasPressed)
        {
            coyoteTimer.Start();
        }
    }

    if (collidedEntity->id == "Frog")
    {
        isTouchingFrog--;
    }
}

bool Capy::PositionIsValid()
{
    float hWidth = 0.5 * width;
    float hHeight = 0.5 * height;

    Vector2 boundariesUpper = Vector2Add(GetScreenToWorld2D({640, 360}, *ecs->GetCamera()), {-hWidth, hHeight});
    Vector2 boundariesLower = Vector2Add(GetScreenToWorld2D({0, 0}, *ecs->GetCamera()), {-hWidth, -hHeight});

    if (pos.x > boundariesUpper.x)
    {
        physBody->SetTransform({boundariesUpper.x, pos.y}, 0);

        b2Vec2 vel = physBody->GetLinearVelocity();
        physBody->SetLinearVelocity({0, vel.y});
    }

    if (pos.x < boundariesLower.x)
    {
        physBody->ApplyLinearImpulseToCenter({10, 0}, true);
        return false;
    }

    if (pos.y < boundariesLower.y)
    {
        return false;
    }

    return true;
}

void Capy::OnDeath()
{
    isAlive = false;
    physBody->SetFixedRotation(false);
    physBody->SetAngularVelocity(6.28); // ~2pi

    b2Fixture *body = physBody->GetFixtureList();
    while (body->IsSensor())
    {
        body = body->GetNext(); // Gets the main ground fixture as it is the one that is not a sensor
    }

    body->SetRestitution(0.5);
    body->SetFriction(0.5);
    DoJump();
    DoJump();
    animManager.SetState("Dead");
}