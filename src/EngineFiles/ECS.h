#pragma once
#include "raylib.h"
#include "Entity.h"
// #include "PhysicsManager.h"
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <list>
#include <memory>
#include <iostream>
#include "box2d.h"

struct EntityContainer
{
    Entity *entityPointer;

    std::list<Entity *>::iterator drawableIterator;
    std::list<Entity *>::iterator screenSpaceDrawableIterator;

    std::list<Entity *>::iterator physicsObjectIterator;
    bool isDrawable = false;
    bool isScreenSpaceDrawable = false;
    bool isPhysicsObject = false;
    bool toBeDeleted = false;

    EntityContainer(Entity *ent) : entityPointer(ent){};
    // ~EntityContainer()
    // {
    //     delete entityPointer;
    // }
};

class ECS
{
    // Entity Component System

public:
    ECS() = default;
    ~ECS() = default;

    template <typename T, typename... Args>
    T &CreateEntity(const std::string &id, Args &&...args)
    {
        T *ent = new T(std::forward<Args>(args)...);

        ent->SetID(id);
        ent->SetECS(this);

        RegisterEntity(*ent);
        ent->Register();
        return *ent;
    }

    template <typename T>
    T &GetEntity(const std::string &id)
    {
        // Object must derive from Entity for this to work
        return *(T *)(m_entityMap[id].get()->entityPointer);
    };

    void RemoveEntity(const std::string &id);

    void RegisterEntity(Entity &entity);
    void RegisterEntityAsDrawable(const std::string &id);

    void RegisterEntityAsScreenSpaceDrawable(const std::string &id);

    void RegisterEntityAsPhysicsObject(const std::string &id, b2Body *body);
    void RegisterEntityAsPhysicsObject(const std::string &id, CirclePhysicsObjectConfig config);
    void RegisterEntityAsPhysicsObject(const std::string &id, RectanglePhysicsObjectConfig config);

    void SetPhysicsManager(b2World *_physManager);
    b2World *GetPhysicsManager();

    std::list<Entity *> &GetAllDrawableObjects();
    std::list<Entity *> &GetAllScreenSpaceDrawableObjects();
    std::list<Entity *> &GetAllPhysicsObjects();
    void SetSpriteSheet(Texture2D _spriteSheet);
    Texture2D GetSpriteSheet();
    void SetCamera(Camera2D *cam);
    Camera2D *GetCamera();
    void SetFont(Font *_font);
    Font *GetFont();

    void SetFrameData(void *data)
    {
        frameData = data;
    };
    template <typename T>
    T *GetFrameData()
    {
        if (frameData != nullptr)
        {
            return (T *)frameData;
        }
        return nullptr;
    }

    void PrintDrawableEntities()
    {
        std::cout << "Drawable Entities: " << std::endl;
        for (Entity *ent : m_drawableEntities)
        {
            std::cout << ent->id << std::endl;
        }
    }

    void PrintPhysicsEntities()
    {
        std::cout << "Physics Entities: " << std::endl;
        for (Entity *ent : m_physicsEntities)
        {
            std::cout << ent->id << std::endl;
        }
    }

    void PrintAllEntities()
    {
        std::cout << "All Entities: " << std::endl;
        for (const auto &ent : m_entityMap)
        {
            std::cout << ent.first << std::endl;
        }
    }

private:
    std::unordered_map<std::string, std::unique_ptr<EntityContainer>> m_entityMap;
    std::list<Entity *> m_drawableEntities;
    std::list<Entity *> m_screenSpaceDrawableEntities;
    std::list<Entity *> m_physicsEntities;
    Texture2D spriteSheet;
    b2World *physManager;
    Camera2D *camera;
    Font *font;

    void *frameData = nullptr; // Ptr to memory that contains any data I want passed around each frame

    void PermanentlyDeleteEntity(const std::string id);

    // void DeleteEntity()
    friend class Scene;
};