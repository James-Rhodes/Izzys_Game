#include "TerrainManager.h"

void TerrainManager::Register()
{

    terrainBlocks = AnimationManager(ecs->GetSpriteSheet(), 0, 97, 32, 36);
    terrainBlocks.AddAnimation("Rock and Grass", {0});
    terrainBlocks.AddAnimation("Rock", {1});
    terrainBlocks.AddAnimation("Plank", {2});
    terrainBlocks.AddAnimation("Ice", {3});
    terrainBlocks.AddAnimation("Bouncy", {4});
    terrainBlocks.AddAnimation("Tree", {5});

    terrainBlocks.SetState("Rock and Grass");

    // Initialise Current Scene and Next Scene
    currentChunk = GenerateNextChunk(true);
    nextChunk = GenerateNextChunk();

    currentChunk->SetSceneMovementSpeed(0);
    nextChunk->SetSceneMovementSpeed(0);

    timeStart = GetTime();
};

void TerrainManager::Update()
{
    if (GetTime() - timeStart > timeBeforeMovement && !hasStarted)
    {
        SetSceneScrollSpeed(speedMileStones[mileStonesIndex]);
        hasStarted = true;
    }

    currentChunkPos = Vector2Add({GetFrameTime() * -sceneScrollSpeed, 0}, currentChunkPos);
    nextChunkPos = Vector2Add({GetFrameTime() * -sceneScrollSpeed, 0}, nextChunkPos);
    distanceTravelled += GetFrameTime() * sceneScrollSpeed;

    if (distanceTravelled > distanceMileStones[mileStonesIndex] && mileStonesIndex != NUM_MILESTONES - 1)
    {
        mileStonesIndex++;
        // SetSceneScrollSpeed(speedMileStones[mileStonesIndex]);
    }

    if (terrainCounter > 100)
    {
        // There should never be more than one hundred scene components on the screen at once
        terrainCounter = 0;
    }

    if (currentChunkPos.x <= -10)
    {
        currentChunk->DeleteChunk();
        delete currentChunk;
        currentChunk = nextChunk;

        currentChunkPos = nextChunkPos;

        nextChunk = GenerateNextChunk();
        nextChunk->SetSceneMovementSpeed(sceneScrollSpeed);
    }

    if (IsKeyPressed(KEY_B))
    {
        SetSceneScrollSpeed(sceneScrollSpeed != 0 ? 0 : 1);
    }
};

SceneChunk *TerrainManager::GenerateNextChunk(bool firstChunk)
{
    SceneChunk *chunk;
    if (firstChunk)
    {
        chunk = new FlatChunk(&terrainCounter);
        chunk->SetECS(ecs);
        chunk->SetTerrainBlockAnimationManager(&terrainBlocks);

        currentChunkPos = {0, 0};
        chunk->GenerateChunk(currentChunkPos); // Center of screen position for chunk?
    }
    else
    {
        chunk = GetChunk();
        chunk->SetECS(ecs);
        chunk->SetTerrainBlockAnimationManager(&terrainBlocks);
        nextChunkPos = {10, 0};
        chunk->GenerateChunk(nextChunkPos); // Next position for chunk?
    }

    return chunk;
}

SceneChunk *TerrainManager::GetChunk()
{
    SceneChunk *newChunk = currentChunk->GetMandatoryNeighboringChunk();

    if (newChunk != nullptr)
        return newChunk;

    return new SplitThemUp(&terrainCounter);

    int randChunkIndex = GetRandomValue(0, numChunkTypes - 1);
    while (randChunkIndex == prevRandChunkIndex)
    {
        randChunkIndex = GetRandomValue(0, numChunkTypes - 1);
    }
    prevRandChunkIndex = randChunkIndex;

    switch (randChunkIndex)
    {
    case (0):
        newChunk = new FlatChunk(&terrainCounter);
        break;
    case (1):
        newChunk = new GapChunk(&terrainCounter);
        break;
    case (2):
        newChunk = new LadderJumpChunk(&terrainCounter);
        break;
    case (3):
        newChunk = new BouncePyramid(&terrainCounter);
        break;
    case (4):
        newChunk = new CapyTreeGap(&terrainCounter);
        break;
    case (5):
        newChunk = new LongFlyBounceChain1(&terrainCounter);
        break;
    case (6):
        newChunk = new LongFlyChain1(&terrainCounter);
        break;
    case (7):
        newChunk = new OlFashionedLadder1(&terrainCounter);
        break;
    case (8):
        newChunk = new BigJump(&terrainCounter);
        break;
    case (9):
        newChunk = new BouncyBigJump(&terrainCounter);
        break;
    case (10):
        newChunk = new MovingBlocker(&terrainCounter);
        break;
    case (11):
        newChunk = new SplitThemUp(&terrainCounter);
        break;
    }

    return newChunk;
}

float TerrainManager::GetDistanceTravelled()
{
    return distanceTravelled;
}

float TerrainManager::GetSceneScrollSpeed()
{
    return sceneScrollSpeed;
}

void TerrainManager::SetSceneScrollSpeed(float _sceneScrollSpeed)
{
    sceneScrollSpeed = _sceneScrollSpeed;
    currentChunk->SetSceneMovementSpeed(sceneScrollSpeed);
    nextChunk->SetSceneMovementSpeed(sceneScrollSpeed);
}