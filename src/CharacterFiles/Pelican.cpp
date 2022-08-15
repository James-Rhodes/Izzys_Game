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
    animManager.SetState("Flying", GetFrameTime());
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

void Pelican::OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact)
{
    if (collidedEntity->id == "Capy")
    {
        // ((Capy *)collidedEntity)->isTouchingSideOfTerrain = false;
        // numSidePlayerCollisionsCapy--;
        std::cout << "Capy Hit me" << std::endl;
    }
    else if (collidedEntity->id == "Frog")
    {
        // ((Frog *)collidedEntity)->isTouchingSideOfTerrain = false;
        // numSidePlayerCollisionsFrog--;
        std::cout << "Frog Hit me" << std::endl;
    }
    else if (collidedEntity->id == "CapyFrogHybrid")
    {
        // ((CapyFrogHybrid *)collidedEntity)->isTouchingSideOfTerrain = false;
        // numSidePlayerCollisionsCapyFrogHybrid--;
        std::cout << "Capy hybrid hit me" << std::endl;
    }
}
