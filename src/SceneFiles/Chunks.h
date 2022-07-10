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

class GapChunk : public SceneChunk
// Just two pieces of ground with a gap between. Fly in the gap for frog and capy can make the leap
{
public:
    GapChunk(int *counter) : terrainCounter(counter){};
    ~GapChunk(){};

    void GenerateChunk(Vector2 worldCenterPos) override;

    int *terrainCounter;
};

class LadderJumpChunk : public SceneChunk
// A ladder of terrain that can be jumped up and then you can jump from the top to the other side
{
public:
    LadderJumpChunk(int *counter) : terrainCounter(counter){};
    ~LadderJumpChunk(){};

    void GenerateChunk(Vector2 worldCenterPos) override;

    int *terrainCounter;
};