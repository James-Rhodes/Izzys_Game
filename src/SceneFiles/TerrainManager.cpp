#include "TerrainManager.h"

void TerrainManager::Register()
{
    // Initialise Current Scene and Next Scene
    currentChunk = GenerateNextChunk(true);
    nextChunk = GenerateNextChunk();

    currentChunk->SetSceneMovementSpeed(0);
    nextChunk->SetSceneMovementSpeed(0);
};

void TerrainManager::Update()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        sceneScrollSpeed = 2;
        currentChunk->SetSceneMovementSpeed(sceneScrollSpeed);
        nextChunk->SetSceneMovementSpeed(sceneScrollSpeed);
    }

    currentChunkPos = Vector2Add({GetFrameTime() * -sceneScrollSpeed, 0}, currentChunkPos);
    nextChunkPos = Vector2Add({GetFrameTime() * -sceneScrollSpeed, 0}, nextChunkPos);

    if (currentChunkPos.x <= -10)
    {
        currentChunk->DeleteChunk();
        delete currentChunk;
        currentChunk = nextChunk;

        currentChunkPos = nextChunkPos;
        nextChunkPos = {10, 0};

        nextChunk = GenerateNextChunk();
        nextChunk->SetSceneMovementSpeed(sceneScrollSpeed);
    }
};

SceneChunk *TerrainManager::GenerateNextChunk(bool firstChunk)
{
    // For now just generate flat chunk
    FlatChunk *flatChunk = new FlatChunk(&terrainCounter);
    flatChunk->SetECS(ecs);
    flatChunk->SetTerrainBlockAnimationManager(&terrainBlocks);
    if (firstChunk)
    {
        flatChunk->GenerateChunk((Vector2){0, 0}); // Center of screen position for chunk?
        currentChunkPos = {0, 0};
    }
    else
    {
        flatChunk->GenerateChunk((Vector2){10, 0}); // Next position for chunk?
        nextChunkPos = {10, 0};
    }
    return flatChunk;
}