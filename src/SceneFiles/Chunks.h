#pragma once
#include "SceneChunkInterface.h"
#include "SceneComponents.h"
#include "raymath.h"
// This will have all of the chunks defined in it

class FlatChunk : public SceneChunk
// Simply just the ground in the scene
{
public:
    FlatChunk(int *counter) : SceneChunk(counter){};
    ~FlatChunk(){};

    void GenerateChunk() override;
};

class GapChunk : public SceneChunk
// Just two pieces of ground with a gap between. Fly in the gap for frog and capy can make the leap
{
public:
    GapChunk(int *counter) : SceneChunk(counter){};
    ~GapChunk(){};

    void GenerateChunk() override;
};

class LadderJumpChunk : public SceneChunk
// A ladder of terrain that can be jumped up and then you can jump from the top to the other side
{
public:
    LadderJumpChunk(int *counter) : SceneChunk(counter){};
    ~LadderJumpChunk(){};

    void GenerateChunk() override;
};