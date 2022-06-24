#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "../AnimationManager.h"
class Capy : public Entity
{
public:
    Capy(Vector2 initPosition) : pos(initPosition)
    {
        // texture = LoadTexture("./assets/Capy_Sprite_Sheet.png");
        // frameSize = {(float)texture.width / 4.0f, (float)texture.height};
    }

    void Register()
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
        animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 0, 66, 32);

        animManager.AddAnimation("Run", {0, 1, 0, 2}, 0.3);
        animManager.AddAnimation("Stand_Still", {0});
        animManager.AddAnimation("Dash", {3});
        animManager.SetState("Stand_Still");
    }

    void Update();

    void Draw();

    void UpdateController();

    void OnCollision(Entity *collidedEntity);
    void OnCollisionEnd(Entity *collidedEntity);

    Vector2 GetPosition();

    Vector2 pos;
    float width = 1;
    float height = 0.5;
    float speed = 3;
    float jumpHeight = 3;
    float timeOfLastDash = 0;
    float dashRechargeTime = 0.8;
    bool isOnGround = false;
    // Texture2D texture;
    // Vector2 frameSize;
    // float currTime = 0;
    // float frameTime = 0.2;
    // int currFrame = 0;
    int currDirection = 1; // 1 = right facing, -1 = left facing
    AnimationManager animManager;
};