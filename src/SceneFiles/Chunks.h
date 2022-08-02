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

class BouncePyramid : public SceneChunk
// A ladder of terrain that can be jumped up and then you can jump from the top to the other side
{
public:
    BouncePyramid(int *counter) : SceneChunk(counter){};
    ~BouncePyramid(){};

    void GenerateChunk() override;
};

class CapyTreeGap : public SceneChunk
{
public:
    CapyTreeGap(int *counter) : SceneChunk(counter){};
    ~CapyTreeGap(){};

    void GenerateChunk() override;
};

class LongFlyBounceChain1 : public SceneChunk
{
public:
    LongFlyBounceChain1(int *counter) : SceneChunk(counter){};
    ~LongFlyBounceChain1(){};

    void GenerateChunk() override;
    SceneChunk *GetMandatoryNeighboringChunk() override;
};

class LongFlyBounceChain2 : public SceneChunk
{
public:
    LongFlyBounceChain2(int *counter) : SceneChunk(counter){};
    ~LongFlyBounceChain2(){};

    void GenerateChunk() override;
    SceneChunk *GetMandatoryNeighboringChunk() override;
};

class LongFlyBounceChain3 : public SceneChunk
{
public:
    LongFlyBounceChain3(int *counter) : SceneChunk(counter){};
    ~LongFlyBounceChain3(){};

    void GenerateChunk() override;
};

class LongFlyChain1 : public SceneChunk
{
public:
    LongFlyChain1(int *counter) : SceneChunk(counter){};
    ~LongFlyChain1(){};

    void GenerateChunk() override;
    SceneChunk *GetMandatoryNeighboringChunk() override;
};

class LongFlyChain2 : public SceneChunk
{
public:
    LongFlyChain2(int *counter) : SceneChunk(counter){};
    ~LongFlyChain2(){};

    void GenerateChunk() override;
    SceneChunk *GetMandatoryNeighboringChunk() override;
};

class LongFlyChain3 : public SceneChunk
{
public:
    LongFlyChain3(int *counter) : SceneChunk(counter){};
    ~LongFlyChain3(){};

    void GenerateChunk() override;
};

class OlFashionedLadder1 : public SceneChunk
{
public:
    OlFashionedLadder1(int *counter) : SceneChunk(counter){};
    ~OlFashionedLadder1(){};

    void GenerateChunk() override;
    SceneChunk *GetMandatoryNeighboringChunk() override;
};

class OlFashionedLadder2 : public SceneChunk
{
public:
    OlFashionedLadder2(int *counter) : SceneChunk(counter){};
    ~OlFashionedLadder2(){};

    void GenerateChunk() override;
    SceneChunk *GetMandatoryNeighboringChunk() override;
};

class OlFashionedLadder3 : public SceneChunk
{
public:
    OlFashionedLadder3(int *counter) : SceneChunk(counter){};
    ~OlFashionedLadder3(){};

    void GenerateChunk() override;
};

class BigJump : public SceneChunk
{
public:
    BigJump(int *counter) : SceneChunk(counter){};
    ~BigJump(){};

    void GenerateChunk() override;
};