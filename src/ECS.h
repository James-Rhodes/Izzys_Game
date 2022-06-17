#pragma once
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
    std::list<Entity *>::iterator physicsObjectIterator;
    bool isDrawable = false;
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

    void RegisterEntity(Entity &entity);

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
    void RemoveEntity(const std::string &id)
    {
        EntityContainer *entContainer = m_entityMap[id].get();

        T *entPointer = (T *)entContainer->entityPointer;

        if (entContainer->isDrawable)
        {
            m_drawableEntities.erase(entContainer->drawableIterator);
        }

        if (entContainer->isPhysicsObject)
        {
            physManager->DestroyBody(entContainer->entityPointer->physBody);
            m_physicsEntities.erase(entContainer->physicsObjectIterator);
        }

        entContainer->toBeDeleted = true;

        delete entPointer;
    }

    void RegisterEntityAsDrawable(const std::string &id);

    void RegisterEntityAsPhysicsObject(const std::string &id, b2Body *body);
    void RegisterEntityAsPhysicsObject(const std::string &id, CirclePhysicsObjectConfig config);
    void RegisterEntityAsPhysicsObject(const std::string &id, RectanglePhysicsObjectConfig config);

    void SetPhysicsManager(b2World *_physManager);
    std::list<Entity *> &GetAllDrawableObjects();
    std::list<Entity *> &GetAllPhysicsObjects();

    template <typename T>
    T &GetEntity(const std::string &id)
    {
        // Object must derive from Entity for this to work
        return *(T *)(m_entityMap[id].get()->entityPointer);
    };

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

    std::unordered_map<std::string, std::unique_ptr<EntityContainer>> m_entityMap;

private:
    std::list<Entity *> m_drawableEntities;
    std::list<Entity *> m_physicsEntities;
    b2World *physManager;
};