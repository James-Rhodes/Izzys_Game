#pragma once
#include "raylib.h"
#include "../EngineFiles/AnimationManager.h"
#include "../EngineFiles/Entity.h"
#include "../EngineFiles/ECS.h"

#include <vector>

// This file defines the interface for a scene chunk (block of designed "levels") for generation.

class SceneChunk
{
public:
    SceneChunk() = default;
    virtual ~SceneChunk() = default;

    // Create Oranges in below function but do not add them to scene components; This will allow them to roll in to the next scene if that occurs. Logic is within Orange to delete itself when needed
    virtual void GenerateChunk(Vector2 worldCenterPos) = 0;

    void SetECS(ECS *_ecs) { ecs = _ecs; };
    void SetTerrainBlockAnimationManager(AnimationManager *_animManager) { terrainBlocks = _animManager; };

    void SetSceneMovementSpeed(float speed)
    {
        sceneMovementSpeed = -speed;
        for (auto ent : sceneComponents)
        {

            b2Vec2 diff;
            diff.x = sceneMovementSpeed * GetFrameTime();
            ent->physBody->SetLinearVelocity(diff);
            ent->physBody->SetAwake(true);
        }
    };

    void DeleteChunk()
    {
        for (auto ent : sceneComponents)
        {
            ecs->RemoveEntity(ent->id);
        }
    };

    float sceneMovementSpeed = 0;
    std::vector<Entity *> sceneComponents;
    ECS *ecs;
    AnimationManager *terrainBlocks;
};