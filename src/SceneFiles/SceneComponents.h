#pragma once
#include "raylib.h"
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
#include "../CharacterFiles/Capybara.h"
#include "../CharacterFiles/Frog.h"
#include "../CharacterFiles/CapyFrogHybrid.h"
#include "TerrainManager.h"
#include <math.h>

#define DEBUG_POSITION 1

class Orange : public Entity
{
public:
    Orange(Vector2 _pos) : pos(_pos){};

    void Register() override
    {
        ecs->RegisterEntityAsDrawable(id);

        CirclePhysicsObjectConfig config;
        config.radius = radius;
        config.pos = pos;
        config.isDynamic = false;
        config.isRollable = false;
        config.restitution = 0.5;
        config.isSensor = true;
        ecs->RegisterEntityAsPhysicsObject(id, config);

        animManager = AnimationManager(ecs->GetSpriteSheet(), 64, 133, 16, 16);

        animManager.AddAnimation("Float", {0, 1, 2, 3, 2, 1}, 0.15);
        animManager.SetState("Float", GetFrameTime());
        phaseShift = PI * GetRandomValue(0, 10);
    }
    void Update() override
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
        animManager.SetState("Float", GetFrameTime());

#ifdef DEBUG_POSITION

        Camera2D *cam = ecs->GetCamera();

        Vector2 clickPos = GetScreenToWorld2D(GetMousePosition(), *cam);
        clickPos.y *= -1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (isSelected)
            {
                isSelected = false;
                std::cout << "Placed " << id << " at " << clickPos.x - chunkWorldCenter.x << " , " << clickPos.y << std::endl;
            }
            else
            {
                if (Vector2Distance(clickPos, pos) <= radius)
                {
                    isSelected = true;
                    std::cout << id << " is selected" << std::endl;
                }
            }
        }

        if (isSelected)
        {
            physBody->SetTransform({clickPos.x, clickPos.y}, 0);
        }

        chunkWorldCenter.x += physBody->GetLinearVelocity().x * GetFrameTime();
#endif
    }

    void Draw() override
    {
        if (!isConsumed) // Rather than deleting the orange when there is a collision, simply stop drawing it. The scene will then delete it once it moves outside of the bounds
        {
            // DrawCircleV(pos, radius, ORANGE);
            drawPos = pos;
            drawPos.y += floatAmplitude * sin(GetTime() * floatSpeed + phaseShift);
            Texture2D texture = ecs->GetSpriteSheet();
            Rectangle src = animManager.GetTextureRectangle();
            DrawTexturePro(texture, src, (Rectangle){drawPos.x - (-radius / 2), drawPos.y + (radius / 2), -radius, -radius}, {0, 0}, 0, RAYWHITE);
        }
    }

    void OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact) override
    {
        if (!isConsumed)
        {
            if (collidedEntity->id == "Capy" || collidedEntity->id == "Frog" || collidedEntity->id == "CapyFrogHybrid")
            {
                isConsumed = true;
            }
        }
    }

    Vector2 pos;
    Vector2 drawPos;
    float radius = 0.2;
    bool isConsumed = false;
    AnimationManager animManager;

    float phaseShift = 0;
    float floatSpeed = 3;
    float floatAmplitude = 0.025;

#ifdef DEBUG_POSITION
    bool isSelected = false;
    Vector2 chunkWorldCenter = {10, 0};
#endif
};

class Ground : public Entity
{
public:
    // Pos is in world coords. Func takes in the current position, sceneScroll speed and a float which can be the time or some accumulator used to manipulate the movement
    Ground(Vector2 _pos, float _width, float _height, Rectangle src, float _friction = 1, Vector2 (*func)(Vector2, Vector2, float) = nullptr) : pos(_pos), width(_width), height(_height), srcRect(src), friction(_friction), setVelocity(func){};

    void Register();

    void Update();

    void Draw();

    void OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact) override;

    void OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact) override;

    Vector2 pos;
    float width;
    float height;
    Rectangle srcRect;
    float friction;
    int numSidePlayerCollisionsCapy = 0;
    int numSidePlayerCollisionsFrog = 0;
    int numSidePlayerCollisionsCapyFrogHybrid = 0;

    b2Fixture *groundFixture;

    Vector2 (*setVelocity)(Vector2, Vector2, float);

#ifdef DEBUG_POSITION
    bool isSelected = false;
    Vector2 chunkWorldCenter = {10, 0};
#endif
};

class Ice : public Ground
{
public:
    Ice(Vector2 _pos, float _width, float _height, Rectangle src, Vector2 (*func)(Vector2, Vector2, float) = nullptr) : Ground(_pos, _width, _height, src, 0, func){};
};

class Fly : public Entity
{
public:
    // Pos is in world coords
    Fly(Vector2 _pos) : pos(_pos){};

    void Register() override
    {
        ecs->RegisterEntityAsDrawable(id);
        RectanglePhysicsObjectConfig config;

        config.pos = pos;
        config.width = width;
        config.height = width;
        config.isDynamic = false;
        config.restitution = 0;
        config.friction = 1;
        config.isSensor = true;

        ecs->RegisterEntityAsPhysicsObject(id, config);

        animManager = AnimationManager(ecs->GetSpriteSheet(), 0, 133, 16, 16);
        animManager.AddAnimation("Fly", {0, 1, 2, 3, 2, 1}, 0.1);
        animManager.SetState("Fly", GetFrameTime());
    }

    void Update() override
    {
        pos = {physBody->GetPosition().x, physBody->GetPosition().y};
        animManager.SetState("Fly", GetFrameTime());

#ifdef DEBUG_POSITION

        Camera2D *cam = ecs->GetCamera();

        Vector2 clickPos = GetScreenToWorld2D(GetMousePosition(), *cam);
        clickPos.y *= -1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (isSelected)
            {
                isSelected = false;
                std::cout << "Placed " << id << " at " << clickPos.x - chunkWorldCenter.x << " , " << clickPos.y << std::endl;
            }
            else
            {
                if (Vector2Distance(clickPos, pos) <= width)
                {
                    isSelected = true;
                    std::cout << id << " is selected" << std::endl;
                }
            }
        }

        if (isSelected)
        {
            physBody->SetTransform({clickPos.x, clickPos.y}, 0);
        }

        chunkWorldCenter.x += physBody->GetLinearVelocity().x * GetFrameTime();
#endif
    };

    void Draw() override
    {
        // DrawTextureTiledWithinCamera(ecs->GetSpriteSheet(), srcRect, (Rectangle){pos.x - (width / 2), pos.y + (height / 2) + alphaDetailOffset, width, -(height + alphaDetailOffset)}, {0, 0}, 0, 64, RAYWHITE);
        // DrawRectanglePro((Rectangle){pos.x, pos.y, width, width}, {width / 2, width / 2}, 0, BLACK);
        Texture2D texture = ecs->GetSpriteSheet();
        Rectangle src = animManager.GetTextureRectangle();

        DrawTexturePro(texture, src, (Rectangle){pos.x - (-width / 2), pos.y + (width / 2), -width, -width}, {0, 0}, 0, RAYWHITE);
        // Will eventually need an animation Manager once I have drawn the fly sprite
    }

    Vector2 pos;
    float width = 0.2;
    AnimationManager animManager;

#ifdef DEBUG_POSITION
    bool isSelected = false;
    Vector2 chunkWorldCenter = {10, 0};
#endif
};

class Plank : public Ground
{
public:
    Plank(Vector2 _pos, float _width, float _height, Rectangle src, float _friction = 1, Vector2 (*func)(Vector2, Vector2, float) = nullptr) : Ground(_pos, _width, _height, src, _friction, func){};
    void OnPreSolve(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact) override;
};

class BouncyPlatform : public Ground
{
public:
    // Pos is in world coords
    BouncyPlatform(Vector2 _pos, float _width, float _height, Rectangle src, float _friction = 1, Vector2 (*func)(Vector2, Vector2, float) = nullptr) : Ground(_pos, _width, _height, src, _friction, func){};

    void Register();
    void OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact) override;
    float jumpHeight = 5;
    float bounceForce = 0;
};

class Tree : public Ground
{
public:
    // Pos is
    Tree(Vector2 _pos, Rectangle src, Vector2 (*func)(Vector2, Vector2, float) = nullptr) : Ground(_pos, 0.5, 0.5, src, 1, func){};

    void Draw() override;

    void OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact) override;

    void OnPreSolve(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact) override;

    bool isSmashed = false;
};
