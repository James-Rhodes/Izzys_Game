#pragma once

#include "../EngineFiles/AnimationManager.h"
#include "../EngineFiles/Entity.h"
#include "SceneChunkInterface.h"
#include "Chunks.h"
// Will contain the terrain manager that fascilitates the speed changes of the chunks as well as choosing which chunk is next/current

class TerrainManager : public Entity
{
public:
    TerrainManager() = default;
    void Register();
    void Update();

    SceneChunk *GenerateNextChunk(bool firstChunk = false);

    SceneChunk *currentChunk;
    SceneChunk *nextChunk;
    AnimationManager terrainBlocks;
    float sceneScrollSpeed = 5;

    int terrainCounter = 0;
};
