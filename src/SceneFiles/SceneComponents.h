#pragma once
#include "raylib.h"
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"

class Orange : public Entity
{
public:
    Orange(Vector2 _pos, float _radius) : pos(_pos), radius(_radius){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);

        CirclePhysicsObjectConfig config;
        config.radius = radius;
        config.pos = pos;
        config.isDynamic = true;
        config.isRollable = false;
        config.restitution = 0.5;
        ecs->RegisterEntityAsPhysicsObject(id, config);
    }
    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    }

    void Draw()
    {
        DrawCircleV(pos, radius, ORANGE);
    }

    Vector2 pos;
    float radius;
};

class Ground : public Entity
{
public:
    // Pos is in world coords
    Ground(Vector2 _pos, float _width, float _height, Rectangle src, float _friction = 1) : pos(_pos), width(_width), height(_height), srcRect(src), friction(_friction){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);
        RectanglePhysicsObjectConfig config;

        config.pos = pos;
        config.width = width;
        config.height = height;
        config.isDynamic = false;
        config.restitution = 0;
        config.friction = 1;

        ecs->RegisterEntityAsPhysicsObject(id, config);
        // Below are the coords for the start pos for future reference
        // animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 97, 32, 36);
    }

    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
        if (IsKeyPressed(KEY_SPACE))
        {
            shouldMove = !shouldMove;
        }

        if (shouldMove)
        {
            float speed = -8;
            b2Vec2 diff;
            diff.x = speed * GetFrameTime();
            // physBody->SetTransform(physBody->GetPosition() + diff, 0);
            physBody->SetLinearVelocity(diff);
            physBody->SetAwake(true);
        }
    };

    void Draw()
    {
        // DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, DARKGREEN);
        // DrawRectangleV(pos, {width, height}, BLACK);

        // Texture2D texture = ecs->GetSpriteSheet();
        float alphaDetailOffset = 5.0 / 64.0f; // 5 pixel offset for some more detail that has alpha back ground (can be walked through)

        DrawTextureTiledWithinCamera(ecs->GetSpriteSheet(), srcRect, (Rectangle){pos.x - (width / 2), pos.y + (height / 2) + alphaDetailOffset, width, -(height + alphaDetailOffset)}, {0, 0}, 0, 64, RAYWHITE);
    }

    // void DrawTextureTiledWithinCamera(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float cameraZoom, Color tint)
    // {
    //     // Could probably be done better but this will do for now!

    //     if ((texture.id <= 0) || (cameraZoom <= 0.0f))
    //     {
    //         return; // Wanna see a infinite loop?!...just delete this line!
    //     }
    //     if ((source.width == 0) || (source.height == 0))
    //     {
    //         return;
    //     }
    //     float invCameraZoom = 1 / cameraZoom;
    //     float destWidthPixels = std::abs(dest.width * cameraZoom);
    //     float destHeightPixels = std::abs(dest.height * cameraZoom);

    //     int numXTiles = std::ceil(destWidthPixels / source.width);
    //     int numYTiles = std::ceil(destHeightPixels / source.height);

    //     float dx = source.width * invCameraZoom;
    //     float dy = source.height * invCameraZoom;

    //     Vector2 currentPos = {dest.x, dest.y};

    //     Vector2 boundary = {dest.x + dest.width, dest.y + dest.height};

    //     for (int y = -1; y < numYTiles; y++)
    //     {
    //         if (y == -1)
    //         {
    //             y = 0; // To make sure the loop runs at least once
    //         }
    //         currentPos.x = dest.x;
    //         for (int x = -1; x < numXTiles; x++)
    //         {
    //             if (x == -1)
    //             {
    //                 x = 0; // To make sure the loop runs at least once
    //             }
    //             float remainderX = std::abs(boundary.x - currentPos.x);
    //             float remainderY = std::abs(boundary.y - currentPos.y);
    //             Rectangle tempSrc = source;
    //             tempSrc.width = std::min(source.width, remainderX * cameraZoom);
    //             tempSrc.height = std::min(source.height, remainderY * cameraZoom);

    //             Rectangle tempDest = {currentPos.x, currentPos.y, std::min(dx, remainderX), -std::min(dy, remainderY)};
    //             DrawTexturePro(texture, tempSrc, tempDest, origin, rotation, tint);
    //             currentPos.x += dx;
    //         }
    //         currentPos.y -= dy;
    //     }

    //     // Tiling logic goes here
    //     // DrawTexturePro(texture, source, dest, origin, rotation, RAYWHITE);
    // }
    Vector2 pos;
    float width;
    float height;
    bool shouldMove = false;
    Rectangle srcRect;
    float friction;
};

// class RockGround : public Entity
// {
// public:
//     RockGround(Vector2 _pos, float _width, float _height) : pos(_pos), width(_width), height(_height){};

//     void Register()
//     {
//         ecs->RegisterEntityAsDrawable(id);
//         RectanglePhysicsObjectConfig config;

//         config.pos = pos;
//         config.width = width;
//         config.height = height;
//         config.isDynamic = false;
//         config.restitution = 0;

//         ecs->RegisterEntityAsPhysicsObject(id, config);

//         animManager = AnimationManager(ecs->GetSpriteSheet(), 32, 102, 32, 32);
//         animManager.AddAnimation("Static", {0});
//         animManager.SetState("Static");
//     }

//     void Update()
//     {
//         pos = {physBody->GetPosition().x, physBody->GetPosition().y};
//         if (IsKeyPressed(KEY_SPACE))
//         {
//             shouldMove = !shouldMove;
//         }

//         if (shouldMove)
//         {
//             float speed = -8;
//             b2Vec2 diff;
//             diff.x = speed * GetFrameTime();
//             // physBody->SetTransform(physBody->GetPosition() + diff, 0);
//             physBody->SetLinearVelocity(diff);
//             physBody->SetAwake(true);
//         }
//     };

//     void Draw()
//     {
//         // DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, DARKGREEN);
//         // DrawRectangleV(pos, {width, height}, BLACK);

//         Texture2D texture = ecs->GetSpriteSheet();
//         Rectangle src = animManager.GetTextureRectangle();

//         DrawTextureTiledWithinCamera(texture, src, (Rectangle){pos.x - (width / 2), pos.y + (height / 2), width, -(height)}, {0, 0}, 0, 64, RAYWHITE);
//     }

//     Vector2 pos;
//     float width;
//     float height;
//     AnimationManager animManager;
//     bool shouldMove = false;
// };