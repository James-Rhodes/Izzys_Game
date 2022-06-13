#include "Scene.h"

void Scene::Run()
{
    UpdatePhysics();
    Update();
    Draw();
}

void Scene::Init()
{
    // ecs.CreateEntity<testEntity>("test", (Vector2){(GetScreenWidth() / 2) - 100, 30});
    // ecs.CreateEntity<testEntity>("test1", (Vector2){(GetScreenWidth() / 2) - 100, -10});

    // ecs.CreateEntity<testEntity>("test1", (Vector2){(GetScreenWidth() / 2) - 5, 30});
    // ecs.CreateEntity<testEntity>("test2", (Vector2){(GetScreenWidth() / 2) + 100, 30}, 10, true);
    // ecs.CreateEntity<testEntity>("test2", (Vector2){(GetScreenWidth() / 2) + 100, 30});

    for (int i = 0; i < 10; i++)
    {
        ecs.CreateEntity<testEntity>("test" + std::to_string(i), (Vector2){30 + 30 * i, 30});
    }

    // ecs.CreateEntity<testEntity>("test3", (Vector2){(GetScreenWidth() / 2) + 5, -30}, 30);

    ecs.CreateEntity<StaticTest>("Bottom", (Vector2){0, GetScreenHeight() - 10}, GetScreenWidth(), 15);
    ecs.CreateEntity<StaticTest>("Left", (Vector2){0, 10}, 15, GetScreenHeight());

    ecs.CreateEntity<StaticTest>("Right", (Vector2){GetScreenWidth() - 15, 0}, 15, GetScreenHeight());
    ecs.CreateEntity<StaticTest>("Top", (Vector2){0, 10}, GetScreenWidth(), 15);
}

void Scene::Update()
{
    for (const auto &ent : ecs.m_entityMap)
    {
        ent.second->entityPointer->Update();
    }
}

void Scene::UpdatePhysics()
{
    physManager.Update(GetFrameTime(), ecs.GetAllPhysicsObjects());
}

void Scene::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    auto drawables = ecs.GetAllDrawableObjects();
    for (auto drawer : drawables)
    {
        drawer->Draw();
    }

    DrawFPS(10, 10);

    EndDrawing();
}