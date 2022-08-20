#include "Chunks.h"

void FlatChunk::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {0, -2.8}, 10.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Orange>("Orange", {0, -2.2});
    AddSceneComponent<Orange>("Orange", {-3, -2.2});
    AddSceneComponent<Orange>("Orange", {3, -2.2});
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

    AddSceneComponent<Plank>("Ground", {-3.3, -1.6}, 1.01, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));
    AddSceneComponent<Plank>("Ground", {-4.6, -0.4}, 1.02, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));
    AddSceneComponent<Orange>("Orange", {-4.6, -0.1});
    AddSceneComponent<Plank>("Ground", {-3.3, 0.8}, 1.02, 0.2, terrainBlocks->GetTextureRectangleAtState("Plank"));

    AddSceneComponent<Fly>("Fly", {2, 2});
};

void BouncePyramid::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-4.5, -2.8}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {4.5, -2.8}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<BouncyPlatform>("Bouncy", {-3.25, -1.4}, 1.5, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));
    AddSceneComponent<BouncyPlatform>("Bouncy", {0, 0}, 2, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));
    AddSceneComponent<BouncyPlatform>("Bouncy", {3.25, -1.4}, 1.5, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));

    AddSceneComponent<Orange>("Orange", {0, -0.6});
    AddSceneComponent<Orange>("Orange", {0.5, -0.6});
    AddSceneComponent<Orange>("Orange", {-0.5, -0.6});
}

void CapyTreeGap::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3, -2.8}, 4.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {3, -2.8}, 4.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

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

void LongFlyBounceChain1::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-4, -2.8}, 2.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<BouncyPlatform>("Bouncy", {1.5, -1.9}, 2, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));

    AddSceneComponent<Fly>("Fly", {0, 2});

    AddSceneComponent<Orange>("Orange", {-1.25, 0});
    AddSceneComponent<Orange>("Orange", {4.5, 0});
}

SceneChunk *LongFlyBounceChain1::GetMandatoryNeighboringChunk()
{
    return new LongFlyBounceChain2(terrainCounter);
}

void LongFlyBounceChain2::GenerateChunk()
{
    AddSceneComponent<BouncyPlatform>("Bouncy", {-2.5, -1.9}, 2, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));
    AddSceneComponent<BouncyPlatform>("Bouncy", {4, -1.9}, 2, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));

    AddSceneComponent<Fly>("Fly", {-4, 2});
    AddSceneComponent<Fly>("Fly", {2, 2});

    AddSceneComponent<Orange>("Orange", {0.75, 0});
}

SceneChunk *LongFlyBounceChain2::GetMandatoryNeighboringChunk()
{
    return new LongFlyBounceChain3(terrainCounter);
}

void LongFlyBounceChain3::GenerateChunk()
{
    AddSceneComponent<BouncyPlatform>("Bouncy", {0, -1.9}, 2, 0.2, terrainBlocks->GetTextureRectangleAtState("Bouncy"));

    AddSceneComponent<Fly>("Fly", {-2, 2});

    AddSceneComponent<Ground>("Ground", {4, -2.8}, 2.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Orange>("Orange", {-3, 0});
    AddSceneComponent<Orange>("Orange", {2, 0});
}

void LongFlyChain1::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-4, -2.8}, 2.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Fly>("Fly", {0, 2});

    AddSceneComponent<Orange>("Orange", {-1.25, 0});
    AddSceneComponent<Orange>("Orange", {4.5, 0});
}

SceneChunk *LongFlyChain1::GetMandatoryNeighboringChunk()
{
    return new LongFlyChain2(terrainCounter);
}

void LongFlyChain2::GenerateChunk()
{

    AddSceneComponent<Fly>("Fly", {-4, 2});
    AddSceneComponent<Fly>("Fly", {2, 2});

    AddSceneComponent<Orange>("Orange", {0.75, 0});
}

SceneChunk *LongFlyChain2::GetMandatoryNeighboringChunk()
{
    return new LongFlyChain3(terrainCounter);
}

void LongFlyChain3::GenerateChunk()
{

    AddSceneComponent<Fly>("Fly", {-2, 2});

    AddSceneComponent<Ground>("Ground", {4, -2.8}, 2.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Orange>("Orange", {-3, 0});
    AddSceneComponent<Orange>("Orange", {2, 0});
}

void OlFashionedLadder1::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3.5, -2.8}, 3.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {0, -1.8}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {3, -0.8}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Orange>("Orange", {0, -1.15});
    AddSceneComponent<Orange>("Orange", {3, -0.15});
}

SceneChunk *OlFashionedLadder1::GetMandatoryNeighboringChunk()
{
    return new OlFashionedLadder2(terrainCounter);
}

void OlFashionedLadder2::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-4, 0.2}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {-1, 1.2}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {2, 0.2}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Orange>("Orange", {-1, 1.85});
}

SceneChunk *OlFashionedLadder2::GetMandatoryNeighboringChunk()
{
    return new OlFashionedLadder3(terrainCounter);
}

void OlFashionedLadder3::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-5, -0.8}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {-2, -1.8}, 1.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {3, -2.8}, 4.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Orange>("Orange", {-5, -0.15});
    AddSceneComponent<Orange>("Orange", {-2, -1.15});
}

void BigJump::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3.5, -2.8}, 3.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {2, -0.8}, 2.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {4, 0.5}, 1.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Orange>("Orange", {-0.5, -0.05});
    AddSceneComponent<Orange>("Orange", {3.9, -0.05});
}

void BouncyBigJump::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3.5, -2.8}, 3.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {2.1, 0.2}, 2.02, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Orange>("Orange", {-0.3, 2});
    AddSceneComponent<Orange>("Orange", {2.1, 0.9});

    AddSceneComponent<BouncyPlatform>("Bouncy", {-0.25, -1.4}, 2, 0.3, terrainBlocks->GetTextureRectangleAtState("Bouncy"));
}

void MovingBlocker::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3.75, -2.8}, 2.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {3.75, -2.8}, 2.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Fly>("Fly", {0, 1.5});
    AddSceneComponent<Orange>("Orange", {0, 0});

    AddSceneComponent<Ground>("Ground", {1, 0}, 0.5, 2, terrainBlocks->GetTextureRectangleAtState("Rock"), 1, [](Vector2 currPos, Vector2 scrollSpeed, float time) -> Vector2
                              { return {scrollSpeed.x, 8 * cos(2 * time)}; });
}

void SplitThemUp::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3, -2.8}, 4.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {4, -2.8}, 2.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));

    AddSceneComponent<Ground>("Ground", {0, -0.25}, 4, 0.3, terrainBlocks->GetTextureRectangleAtState("Rock"), 0, [](Vector2 currPos, Vector2 scrollSpeed, float time) -> Vector2
                              { return {scrollSpeed.x + 2 * 4 * cos(4 * time), scrollSpeed.y}; });

    AddSceneComponent<Orange>("Orange", {-1.5, 0.75});
    AddSceneComponent<Orange>("Orange", {1, 0.75});
    AddSceneComponent<Orange>("Orange", {0, -2});
    AddSceneComponent<Orange>("Orange", {1, -2});
    AddSceneComponent<Orange>("Orange", {2, -2});

    AddSceneComponent<Ground>("Ground", {-4, -0.25}, 2.05, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock"));
    AddSceneComponent<Ground>("Ground", {-4.525, 1.25}, 1, 3, terrainBlocks->GetTextureRectangleAtState("Rock"));
    AddSceneComponent<Ground>("Ground", {2, -0.25}, 4.5, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock"));
}

void CirclingPlatforms::GenerateChunk()
{
    AddSceneComponent<Ground>("Ground", {-3.75, -2.8}, 2.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Ground>("Ground", {3.75, -2.8}, 2.52, 0.5, terrainBlocks->GetTextureRectangleAtState("Rock and Grass"));
    AddSceneComponent<Orange>("Orange", {0, 0});
    AddSceneComponent<Orange>("Orange", {0, 1});
    AddSceneComponent<Orange>("Orange", {0, -1});

    AddSceneComponent<Orange>("Orange", {2, 0});
    AddSceneComponent<Orange>("Orange", {-2, 0});

    AddSceneComponent<Ground>("Ground", {2, 0}, 1, 0.3, terrainBlocks->GetTextureRectangleAtState("Rock"), 1, [](Vector2 currPos, Vector2 scrollSpeed, float time) -> Vector2
                              { return {scrollSpeed.x - CIRCLING_RADIUS * sin(time), CIRCLING_RADIUS * cos(time)}; });

    AddSceneComponent<Ground>("Ground", {0, -2}, 1, 0.3, terrainBlocks->GetTextureRectangleAtState("Rock"), 1, [](Vector2 currPos, Vector2 scrollSpeed, float time) -> Vector2
                              { return {scrollSpeed.x - CIRCLING_RADIUS * sin(time - PI / 2), CIRCLING_RADIUS * cos(time - PI / 2)}; });

    AddSceneComponent<Ground>("Ground", {-2, 0}, 1, 0.3, terrainBlocks->GetTextureRectangleAtState("Rock"), 1, [](Vector2 currPos, Vector2 scrollSpeed, float time) -> Vector2
                              { return {scrollSpeed.x - CIRCLING_RADIUS * sin(time - PI), CIRCLING_RADIUS * cos(time - PI)}; });

    AddSceneComponent<Ground>("Ground", {0, 2}, 1, 0.3, terrainBlocks->GetTextureRectangleAtState("Rock"), 1, [](Vector2 currPos, Vector2 scrollSpeed, float time) -> Vector2
                              { return {scrollSpeed.x - CIRCLING_RADIUS * sin(time - 3 * PI / 2), CIRCLING_RADIUS * cos(time - 3 * PI / 2)}; });
}