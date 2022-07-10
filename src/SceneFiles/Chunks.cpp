#include "Chunks.h"

void FlatChunk::GenerateChunk(Vector2 worldCenterPos)
{
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {0, -2.8}), 10.5, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass")));
};

void GapChunk::GenerateChunk(Vector2 worldCenterPos)
{
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {-3.75, -2.8}), 2.5, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass")));
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {3.75, -2.8}), 2.5, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass")));
    sceneComponents.push_back(&ecs->CreateEntity<Fly>("Fly" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {0, 1.5})));
};

void LadderJumpChunk::GenerateChunk(Vector2 worldCenterPos)
{
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {-4.5, -2.8}), 1, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass")));
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {4.5, -2.8}), 1, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass")));

    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {-3.3, -1.6}), 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Rock")));
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {-4.6, -0.4}), 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Rock")));
    sceneComponents.push_back(&ecs->CreateEntity<Ground>("Ground" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {-3.3, 0.8}), 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Rock")));

    sceneComponents.push_back(&ecs->CreateEntity<Fly>("Fly" + std::to_string((*terrainCounter)++), Vector2Add(worldCenterPos, {0, 2})));
};