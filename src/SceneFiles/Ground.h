#pragma once
#include "raylib.h"
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"

class Coin : public Entity
{
public:
    Coin(Vector2 _pos, float _radius) : pos(_pos), radius(_radius){};

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
        DrawCircleV(pos, radius, GOLD);
    }

    Vector2 pos;
    float radius;
};
class Box : public Entity
{
public:
    Box(Vector2 _pos, float _width, float _height) : pos(_pos), width(_width), height(_height){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);
        RectanglePhysicsObjectConfig config;
        config.pos = pos;
        config.width = width;
        config.height = height;
        config.isDynamic = true;
        config.isRollable = false;
        config.friction = 0.01;
        ecs->RegisterEntityAsPhysicsObject(id, config);
    }
    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};

        if (IsKeyPressed(KEY_SPACE))
        {
            // physBody->ApplyForceToCenter(b2Vec2(-10, 0), true);
            // physBody->SetTransform()

            physBody->SetLinearVelocity(physBody->GetLinearVelocity() + b2Vec2(-1, 0));
        }
        if (IsKeyDown(KEY_LEFT))
        {
            physBody->SetTransform(physBody->GetPosition() + b2Vec2(-0.09, 0), 0);
            physBody->SetLinearVelocity(b2Vec2((0 * physBody->GetLinearVelocity().x), physBody->GetLinearVelocity().y));

            physBody->SetAwake(true);
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            physBody->SetTransform(physBody->GetPosition() + b2Vec2(0.09, 0), 0);

            physBody->SetLinearVelocity(b2Vec2((0 * physBody->GetLinearVelocity().x), physBody->GetLinearVelocity().y));
            physBody->SetAwake(true);
        }
        if (IsKeyPressed(KEY_UP))
        {
            physBody->SetLinearVelocity(physBody->GetLinearVelocity() + b2Vec2(0, 10));
            physBody->SetAwake(true);
        }
    }

    void Draw()
    {
        Color colour = colliding ? RED : GREEN;
        DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, colour);
    };

    void OnCollision(Entity *collidedEntity)
    {
        if (collidedEntity->id == "Circle")
        {
            colliding = true;
        }

        if (collidedEntity->id == "Coin")
        {
            ecs->RemoveEntity("Coin");
        }
    }

    void OnCollisionEnd(Entity *collidedEntity)
    {
        if (collidedEntity->id == "Circle")
        {
            colliding = false;
        }
    }

    Vector2 pos;
    float width;
    float height;
    bool colliding = false;
};
class Circle : public Entity
{
public:
    Circle(Vector2 _pos, float _radius) : pos(_pos), radius(_radius){};

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
        DrawCircleV(pos, radius, RED);
    }

    Vector2 pos;
    float radius;
};

class Ground : public Entity
{
public:
    Ground(Vector2 _pos, float _width, float _height) : pos(_pos), width(_width), height(_height){};

    void Register()
    {
        ecs->RegisterEntityAsDrawable(id);
        RectanglePhysicsObjectConfig config;

        config.pos = pos;
        config.width = width;
        config.height = height;
        config.isDynamic = false;
        config.restitution = 0;

        ecs->RegisterEntityAsPhysicsObject(id, config);

        animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 97, 32, 36);
        animManager.AddAnimation("Static", {0});
        animManager.SetState("Static");
    }

    void Update()
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
    };

    void Draw()
    {
        // DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, DARKGREEN);
        // DrawRectangleV(pos, {width, height}, BLACK);

        Texture2D texture = ecs->GetSpriteSheet();
        Rectangle src = animManager.GetTextureRectangle();
        float flowerOffset = 4.0 / 64.0f;

        DrawTextureTiledWithinCamera(texture, src, (Rectangle){pos.x - (width / 2), pos.y + (height / 2) + flowerOffset, width, -(height + flowerOffset)}, {0, 0}, 0, 64, RAYWHITE);
    }

    void DrawTextureTiledWithinCamera(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float cameraZoom, Color tint)
    {
        // Could probably be done better but this will do for now!

        if ((texture.id <= 0) || (cameraZoom <= 0.0f))
        {
            return; // Wanna see a infinite loop?!...just delete this line!
        }
        if ((source.width == 0) || (source.height == 0))
        {
            return;
        }
        float invCameraZoom = 1 / cameraZoom;
        float destWidthPixels = std::abs(dest.width * cameraZoom);
        float destHeightPixels = std::abs(dest.height * cameraZoom);

        int numXTiles = std::ceil(destWidthPixels / source.width);
        int numYTiles = std::ceil(destHeightPixels / source.height);

        float dx = source.width * invCameraZoom;
        float dy = source.height * invCameraZoom;

        Vector2 currentPos = {dest.x, dest.y};

        Vector2 boundary = {dest.x + dest.width, dest.y + dest.height};

        for (int y = -1; y < numYTiles; y++)
        {
            if (y == -1)
            {
                y = 0; // To make sure the loop runs at least once
            }
            currentPos.x = dest.x;
            for (int x = -1; x < numXTiles; x++)
            {
                if (x == -1)
                {
                    x = 0; // To make sure the loop runs at least once
                }
                float remainderX = std::abs(boundary.x - currentPos.x);
                float remainderY = std::abs(boundary.y - currentPos.y);
                Rectangle tempSrc = source;
                tempSrc.width = std::min(source.width, remainderX * cameraZoom);
                tempSrc.height = std::min(source.height, remainderY * cameraZoom);

                Rectangle tempDest = {currentPos.x, currentPos.y, std::min(dx, remainderX), -std::min(dy, remainderY)};
                DrawTexturePro(texture, tempSrc, tempDest, origin, rotation, tint);
                currentPos.x += dx;
            }
            currentPos.y -= dy;
        }

        // Tiling logic goes here
        // DrawTexturePro(texture, source, dest, origin, rotation, RAYWHITE);
    }
    Vector2 pos;
    float width;
    float height;
    AnimationManager animManager;
};