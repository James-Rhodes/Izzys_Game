#include "Scene.h"

void Scene::Run()
{
    if (gui.state == GUIStates::PLAY)
    {
        UpdatePhysics();
    }
    Update();
    Draw();
}

void Scene::Init()
{
    TerrainManager &terrainManager = ecs.CreateEntity<TerrainManager>("TerrainManager");

    // ecs.CreateEntity<Capy>("Capy", (Vector2){0, 0});
    // ecs.CreateEntity<Frog>("Frog", (Vector2){-2, 0});

    ecs.CreateEntity<CharacterManager>("CharacterManager", (Vector2){2, 0}, (Vector2){-2, 0});

    ecs.CreateEntity<Pelican>("Pelican", (Vector2){0, 0}, 0);

    bgManager.SetSceneSpeedPointer(&terrainManager.sceneScrollSpeed);
    sceneToReset = false;
}

void Scene::Update()
{
    bool guiHasChangedState = gui.state != gui.prevState;
    if (guiHasChangedState && gui.state == GUIStates::PLAY)
    {
        ResetScene();
    }
    else if (guiHasChangedState && gui.state == GUIStates::MAIN_MENU)
    {
        ClearAllEntities();
    }

    for (auto it = ecs.m_entityMap.cbegin(); it != ecs.m_entityMap.cend();)
    {

        if (!it->second->toBeDeleted)
        {
            it->second->entityPointer->Update();
            ++it;
        }
        else
        {
            std::string key = it->first;
            it++;
            ecs.PermanentlyDeleteEntity(key);
        }
    }

    if (sceneToReset)
    {
        Init();
    }
}

void Scene::UpdatePhysics()
{
    physicsAccumulator += GetFrameTime();

    int numPhysicsIterations = 0;
    while (physicsAccumulator >= physTime)
    {
        physicsAccumulator -= physTime;
        physManager->Step(physTime, 10, 4);
        numPhysicsIterations++;

        if (numPhysicsIterations >= maxNumPhysicsSteps && !firstFrame)
        {
            std::cout << "You Used Too Many Physics Steps..." << std::endl;
            break;
        }
    }
    firstFrame = false;
}

void Scene::Draw()
{

    BeginTextureMode(screenBuffer);

    ClearBackground(RAYWHITE);
    bgManager.Draw();
    BeginMode2D(camera);

    auto drawables = ecs.GetAllDrawableObjects();
    for (auto drawer : drawables)
    {
        drawer->Draw();
    }

    if (debugDrawer.enabled)
    {
        physManager->DebugDraw();
    }

    EndMode2D();

    EndTextureMode();

    if (currScreenWidth != GetScreenWidth() || currScreenHeight != GetScreenHeight())
    {
        if (currScreenHeight != GetScreenHeight())
        {
            currScreenHeight = GetScreenHeight();
            currScreenWidth = currScreenHeight * (aspectRatio);
        }
        else
        {
            currScreenWidth = GetScreenWidth();
            currScreenHeight = currScreenWidth * (1 / aspectRatio);
        }
        SetWindowSize(currScreenWidth, currScreenHeight);
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(screenBuffer.texture, (Rectangle){0, 0, (float)screenBuffer.texture.width, (float)screenBuffer.texture.height}, (Rectangle){0, 0, (float)currScreenWidth, (float)currScreenHeight}, {0, 0}, 0, RAYWHITE);

    DrawFPS(10, 10);
    // Draws the screen space objects
    auto screenSpaceDrawables = ecs.GetAllScreenSpaceDrawableObjects();
    for (auto drawer : screenSpaceDrawables)
    {
        drawer->Draw();
    }

    gui.Draw();

    EndDrawing();
}

void Scene::ResetScene()
{
    ClearAllEntities();
    sceneToReset = true;
}

void Scene::ClearAllEntities()
{
    for (auto it = ecs.m_entityMap.cbegin(); it != ecs.m_entityMap.cend();)
    {
        ecs.RemoveEntity(it->second->entityPointer->id);
        ++it;
    }

    bgManager.SetSceneSpeedPointer(nullptr);
}