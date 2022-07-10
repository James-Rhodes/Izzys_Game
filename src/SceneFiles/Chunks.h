#pragma once
#include "SceneChunkInterface.h"
#include "SceneComponents.h"
#include "raymath.h"
// This will have all of the chunks defined in it

class FlatChunk : public SceneChunk
// Simply just the ground in the scene
{
public:
    FlatChunk(int *counter) : terrainCounter(counter){};
    ~FlatChunk(){};

    void GenerateChunk(Vector2 worldCenterPos) override;

    int *terrainCounter;
};