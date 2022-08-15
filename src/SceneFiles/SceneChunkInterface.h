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
    SceneChunk(int *counter) : terrainCounter(counter){};
    SceneChunk() = delete;
    virtual ~SceneChunk() = default;

    void GenerateChunk(Vector2 _worldCenterPos)
    {
        SetWorldCenterPos(_worldCenterPos);
        GenerateChunk();
    };

    virtual void GenerateChunk() = 0;

    void SetECS(ECS *_ecs) { ecs = _ecs; };
    void SetTerrainBlockAnimationManager(AnimationManager *_animManager) { terrainBlocks = _animManager; };

    void SetSceneMovementSpeed(float speed)
    {
        sceneMovementSpeed = -speed;
        for (auto ent : sceneComponents)
        {

            b2Vec2 diff;
            diff.x = sceneMovementSpeed;
            diff.y = 0;
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

    virtual SceneChunk *GetMandatoryNeighboringChunk()
    {
        return nullptr;
    }

    template <typename T, typename... Args>
    void AddSceneComponent(const std::string &id, const Vector2 &pos, Args &&...args)
    {
        // Helper function to automatically add components to the scene and ensure they have unique ID's
        std::string terrainId = id + std::to_string((*terrainCounter)++);
        sceneComponents.push_back(&ecs->CreateEntity<T>(terrainId, Vector2Add(worldCenterPos, pos), std::forward<Args>(args)...));
    }

    void SetWorldCenterPos(Vector2 _worldCenterPos)
    {
        worldCenterPos = _worldCenterPos;
    };

    float sceneMovementSpeed = 0;
    std::vector<Entity *> sceneComponents;
    ECS *ecs;
    AnimationManager *terrainBlocks;
    Vector2 worldCenterPos;
    int *terrainCounter;
};