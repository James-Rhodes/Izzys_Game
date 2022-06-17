#include "Scene.h"

void Scene::Run()
{
    UpdatePhysics();
    Update();
    Draw();
}

void Scene::Init()
{
    ecs.CreateEntity<Ground>("Ground", (Vector2){-10, 0}, 20, 2);
    ecs.CreateEntity<Ground>("Ground2", (Vector2){5, 5}, 10, 2);

    ecs.CreateEntity<Circle>("Circle", (Vector2){-11, 5}, 1);
    ecs.CreateEntity<Box>("Box", (Vector2){-1, 5}, 1, 2);
}

void Scene::Update()
{
    // for (const auto &ent : ecs.m_entityMap)
    for (auto it = ecs.m_entityMap.cbegin(); it != ecs.m_entityMap.cend();)
    {

        if (!it->second->toBeDeleted)
        {
            it->second->entityPointer->Update();
            ++it;
        }
        else
        {
            ecs.m_entityMap.erase(it++);
        }
    }
}

void Scene::UpdatePhysics()
{
    // physManager.Update(GetFrameTime(), ecs.GetAllPhysicsObjects());
    physManager->Step(1 / 60.0f, 6, 2);
}

void Scene::Draw()
{
    camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

    // BeginDrawing();
    BeginTextureMode(screenBuffer);

    BeginMode2D(camera);
    // rlPushMatrix();

    // rlScalef(1, -1, 1);

    ClearBackground(RAYWHITE);

    auto drawables = ecs.GetAllDrawableObjects();
    for (auto drawer : drawables)
    {
        drawer->Draw();
    }

    // rlPopMatrix();

    EndMode2D();

    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextureRec(screenBuffer.texture, (Rectangle){0, 0, (float)screenBuffer.texture.width, (float)screenBuffer.texture.height}, (Vector2){0, 0}, WHITE);

    DrawFPS(10, 10);

    EndDrawing();
}