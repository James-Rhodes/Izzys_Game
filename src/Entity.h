#pragma once

#include <string>
#include "raymath.h"
#include <memory>
#include <iostream>
#include "box2d.h"

class ECS;

struct PhysicsConfig
{
    Vector2 pos = {};
    bool isDynamic = false;
    bool isRollable = false;
    float density = 1.0f;
    float friction = 0.3f;
    float restitution = 0.3f;
};
struct CirclePhysicsObjectConfig : public PhysicsConfig
{
    // This object is passed in to ecs as physics object to define a circular physics object
    CirclePhysicsObjectConfig(){};
    float radius = 1.0;
};
struct RectanglePhysicsObjectConfig : public PhysicsConfig
{
    // This object is passed in to ecs as physics object to define a Rectangle physics object
    RectanglePhysicsObjectConfig(){};

    float width = 1.0;
    float height = 1.0;
};

class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void Update() = 0;
    virtual void Draw(){};
    virtual void Register(){};
    virtual void OnCollision(Entity *collidedEntity){};
    virtual void OnCollisionEnd(Entity *collidedEntity){};

    void SetID(const std::string _id)
    {
        id = _id;
    }

    void SetECS(ECS *_ecs)
    {
        ecs = _ecs;
    };

    std::string id;
    ECS *ecs;
    // PhysicsObject *physObject;
    b2Body *physBody;
};