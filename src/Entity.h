#pragma once

#include <string>
#include "raymath.h"
#include <memory>
#include <iostream>
#include "box2d.h"
// #include "ECS.h"

// enum PhysicsReactionType
// {
//     STATIC,
//     DYNAMIC
// };

// enum PhysicsShape
// {
//     CIRCLE,
//     RECTANGLE
// };

class ECS;
// class PhysicsObject
// {
//     // Provides an interface to the b2Body class. Nothing more nothing less
// public:
//     PhysicsObject(){};
//     void SetPos(Vector2 pos){};

//     b2Body *physicsBody;
//     PhysicsReactionType reaction;
//     PhysicsShape shape;
// };
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
    virtual void OnCollision(){

    };

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