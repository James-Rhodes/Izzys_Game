#pragma once

#include "../EngineFiles/AnimationManager.h"
#include "../EngineFiles/Entity.h"
#include "SceneChunkInterface.h"
#include "Chunks.h"
// Will contain the terrain manager that fascilitates the speed changes of the chunks as well as choosing which chunk is next/current

class TerrainManager : public Entity
{
public:
    TerrainManager(){

    };
    void Register();
    void Update();

    SceneChunk *GenerateNextChunk(bool firstChunk = false);
    SceneChunk *GetChunk();

    float GetDistanceTravelled();

    SceneChunk *currentChunk;
    SceneChunk *nextChunk;
    AnimationManager terrainBlocks;
    float sceneScrollSpeed = 0;

    Vector2 currentChunkPos;
    Vector2 nextChunkPos;

    int terrainCounter = 0;

    int numChunkTypes = 5;

    float distanceTravelled = 0;

    int prevRandChunkIndex = -1;
};
