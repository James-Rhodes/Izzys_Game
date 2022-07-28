#include "Chunks.h"

void FlatChunk::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {0, -2.8}, 10.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
};

void GapChunk::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3.75, -2.8}, 2.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {3.75, -2.8}, 2.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Fly>("Fly", {0, 1.5});
};

void LadderJumpChunk::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-4.5, -2.8}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Ground>("Ground", {4, -2.8}, 2.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Plank>("Ground", {-3.3, -1.6}, 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));
    AddSceneComponent<Plank>("Ground", {-4.6, -0.4}, 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));
    AddSceneComponent<Plank>("Ground", {-3.3, 0.8}, 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));

    AddSceneComponent<Fly>("Fly", {2, 2});
};