#include "Chunks.h"

void FlatChunk::GenerateChunk(Vector2 worldCenterPos)
{
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {0, -2.8}), 10.5, 0.5, (Rectangle){0, 97, 32, 36}));
};

void GapChunk::GenerateChunk(Vector2 worldCenterPos)
{
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {-3.75, -2.8}), 2.5, 0.5, (Rectangle){0, 97, 32, 36}));
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {3.75, -2.8}), 2.5, 0.5, (Rectangle){0, 97, 32, 36}));
    sceneComponents.push_back(&ecs->CreateEntity<Fly>("Fly" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {0, 1.5})));
};