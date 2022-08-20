#pragma once

#include "../EngineFiles/AnimationManager.h"
#include "../EngineFiles/Entity.h"
#include "SceneChunkInterface.h"
#include "Chunks.h"
// Will contain the terrain manager that fascilitates the speed changes of the chunks as well as choosing which chunk is next/current

#define NUM_MILESTONES 6
class TerrainManager : public Entity
{
public:
    TerrainManager(){

    };

    ~TerrainManager()
    {
        if (currentChunk != nullptr)
        {
            delete currentChunk;
        }

        if (nextChunk != nullptr)
        {
            delete nextChunk;
        }
    }
    void Register();
    void Update();

    SceneChunk *GenerateNextChunk(bool firstChunk = false);
    SceneChunk *GetChunk();

    float GetDistanceTravelled();

    float GetSceneScrollSpeed();
    void SetSceneScrollSpeed(float _sceneScrollSpeed);

    SceneChunk *currentChunk;
    SceneChunk *nextChunk;
    AnimationManager terrainBlocks;
    float sceneScrollSpeed = 0;

    Vector2 currentChunkPos;
    Vector2 nextChunkPos;

    int terrainCounter = 0;

    int numChunkTypes = 13;

    float distanceTravelled = 0;

    int prevRandChunkIndex = -1;

    float timeStart;
    float timeBeforeMovement = 1; // Seconds before scrolling starts
    bool hasStarted = false;

    int mileStonesIndex = 0;

    float distanceMileStones[NUM_MILESTONES] = {10, 50, 100, 200, 300, 500};

    // float speedMileStones[NUM_MILESTONES] = {0.5, 1, 2, 3, 4, 5};
    float speedMileStones[NUM_MILESTONES] = {1, 1, 2, 3, 4, 5};
};
