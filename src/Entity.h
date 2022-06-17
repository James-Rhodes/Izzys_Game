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

struct CirclePhysicsObjectConfig
{
    // This object is passed in to ecs as physics object to define a circular physics object
    CirclePhysicsObjectConfig(){};
    CirclePhysicsObjectConfig(Vector2 _pos, float _radius, bool _isDynamic = false, bool _isRollable = false) : pos(_pos), radius(_radius), isDynamic(_isDynamic), isRollable(_isRollable){};
    Vector2 pos = {};
    float radius = 1.0;
    bool isDynamic = false;
    bool isRollable = false;
};
struct RectanglePhysicsObjectConfig
{
    // This object is passed in to ecs as physics object to define a circular physics object
    RectanglePhysicsObjectConfig(){};
    RectanglePhysicsObjectConfig(Vector2 _pos, float _width, float _height, bool _isDynamic = false, bool _isRollable = false) : pos(_pos), width(_width), height(_height), isDynamic(_isDynamic), isRollable(_isRollable){};

    Vector2 pos = {};
    float width = 1.0;
    float height = 1.0;
    bool isDynamic = false;
    bool isRollable = false;
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