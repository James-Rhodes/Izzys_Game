#include "Pelican.h"

void Pelican::Register()
{

    ecs->RegisterEntityAsDrawable(id);

    RectanglePhysicsObjectConfig config;
    config.pos = pos;
    config.width = width;
    config.height = height;
    config.isSensor = true;

    ecs->RegisterEntityAsPhysicsObject(id, config);

    animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 237, 80, 44);

    animManager.AddAnimation("Flying", {0, 1, 2, 3, 2, 1}, 0.1);

    animManager.SetState("Flying");
}
void Pelican::Update()
{
    pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    animManager.SetState("Flying", GetFrameTime());

    if (physBody->GetPosition().x < -7)
    {
        SetSpeed(0);
    }
}

void Pelican::Draw()
{
    Vector2 renderPos = PixelPerfectClamp({pos.x, pos.y}, 64);

    Vector2 renderDimensions = PixelPerfectClamp({-renderWidth, -renderHeight}, 64);
    Texture2D texture = ecs->GetSpriteSheet();
    Rectangle src = animManager.GetTextureRectangle();
    Vector2 offset = {renderWidth * 0.5f + -renderWidth, -renderHeight * 0.5f};
    float angle = physBody->GetAngle() * RAD2DEG;

    DrawTexturePro(texture, src, (Rectangle){renderPos.x, renderPos.y, renderDimensions.x, renderDimensions.y},
                   offset, angle, RAYWHITE);
}

void Pelican::SetSpeed(float _speed)
{
    speed = _speed;
    physBody->SetLinearVelocity({-speed, 0});
}
