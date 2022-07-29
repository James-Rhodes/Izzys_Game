#include "Chunks.h"

void FlatChunk::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {0, -2.8}, 10.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Orange>("Orange", {0, -2.4});
    AddSceneComponent<Orange>("Orange", {-3, -2.4});
    AddSceneComponent<Orange>("Orange", {3, -2.4});
};

void GapChunk::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3.75, -2.8}, 2.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {3.75, -2.8}, 2.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Fly>("Fly", {0, 1.5});
    AddSceneComponent<Orange>("Orange", {0, 0});
};

void LadderJumpChunk::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-4.5, -2.8}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Ground>("Ground", {4, -2.8}, 2.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Plank>("Ground", {-3.3, -1.6}, 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));
    AddSceneComponent<Plank>("Ground", {-4.6, -0.4}, 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));
    AddSceneComponent<Orange>("Orange", {-4.6, -0.1});
    AddSceneComponent<Plank>("Ground", {-3.3, 0.8}, 1, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));

    AddSceneComponent<Fly>("Fly", {2, 2});
};

void BouncePyramid::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-4.5, -2.8}, 1.01, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {4.5, -2.8}, 1.01, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<BouncyPlatform>("Bouncy", {-3.25, -1.4}, 1.5, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));
    AddSceneComponent<BouncyPlatform>("Bouncy", {0, 0}, 2, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));
    AddSceneComponent<BouncyPlatform>("Bouncy", {3.25, -1.4}, 1.5, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));

    AddSceneComponent<Orange>("Orange", {0, -0.6});
    AddSceneComponent<Orange>("Orange", {0.5, -0.6});
    AddSceneComponent<Orange>("Orange", {-0.5, -0.6});
}

void CapyTreeGap::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3, -2.8}, 4.01, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {3, -2.8}, 4.01, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Tree>("Tree", {-1.25, -2.3}, terrainBlocks->GetTextureRectangleAtState("Tree"));
    AddSceneComponent<Tree>("Tree", {1.25, -2.3}, terrainBlocks->GetTextureRectangleAtState("Tree"));

    AddSceneComponent<Ice>("Ice", {0, -1.7}, 3, 0.5, terrainBlocks->GetTextureRectangleAtState("Ice"));
    AddSceneComponent<Ice>("Ice", {2, -1}, 3, 0.5, terrainBlocks->GetTextureRectangleAtState("Ice"));

    AddSceneComponent<Orange>("Orange", {0, -2.2});
    AddSceneComponent<Orange>("Orange", {0.5, -2.2});
    AddSceneComponent<Orange>("Orange", {-0.5, -2.2});

    AddSceneComponent<Fly>("Fly", {-2.6, 1.65});
    AddSceneComponent<Orange>("Orange", {-4.6, 1.65});
}