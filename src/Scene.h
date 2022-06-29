#pragma once
#include "raylib.h"
#include "EngineFiles/ECS.h"
#include "EngineFiles/CollisionManager.h"
#include "EngineFiles/B2D_Debug_Drawer.h"
#include <memory>
#include <cmath>
#include "rlgl.h"

#include "./CharacterFiles/Capybara.h"
#include "./CharacterFiles/Frog.h"
#include "./SceneFiles/Ground.h"

class Scene
{
public:
    Scene()
    {
        minScreenWidth = 640;
        minScreenHeight = 360;
        currScreenWidth = minScreenWidth;
        currScreenHeight = minScreenHeight;

        screenBuffer = LoadRenderTexture(minScreenWidth, minScreenHeight);
        SetTextureFilter(screenBuffer.texture, TEXTURE_FILTER_POINT);
        SetWindowMinSize(minScreenWidth, minScreenHeight);
        aspectRatio = (float)minScreenWidth / (float)minScreenHeight;
        camera.offset = {(float)screenBuffer.texture.width / 2, (float)screenBuffer.texture.height / 2};
        camera.target = {0, 0};
        camera.zoom = 64;
        camera.rotation = 0;

        rlDisableBackfaceCulling();

        b2Vec2 gravity(0.0f, -10.0f);
        physManager = new b2World(gravity);
        physManager->SetContactListener(&collisionManager);
        physManager->SetDebugDraw(&debugDrawer);
        debugDrawer.SetFlags(b2Draw::e_shapeBit);
        debugDrawer.AppendFlags(b2Draw::e_jointBit);
        debugDrawer.enabled = true;

        ecs.SetPhysicsManager(physManager);
        ecs.SetSpriteSheet(LoadTexture("./assets/Capy_Sprite_Sheet.png"));
    };
    ~Scene()
    {
        delete physManager;
    }

    void Init();
    void Run();
    void Update();
    void Draw();
    void UpdatePhysics();

    ECS ecs;
    b2World *physManager;
    Camera2D camera = {};
    CollisionManager collisionManager;
    DebugDrawer debugDrawer;
    RenderTexture2D screenBuffer;
    float aspectRatio;
    int minScreenWidth;
    int minScreenHeight;
    int currScreenWidth;
    int currScreenHeight;
};