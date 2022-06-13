#pragma once

#include <string>
#include "raymath.h"
#include <memory>
#include <iostream>
// #include "ECS.h"

class ECS;

enum ColliderType
{
    CIRCLE,
    RECTANGLE
};

enum PhysType
{
    STATIC,
    DYNAMIC
};

struct Collider
{
    Collider() = delete;
    Collider(Vector2 &_pos) : pos(_pos), prevPos(_pos){};

    virtual ~Collider() = default;

    void UpdatePosition(float dt)
    {
        vel = Vector2Subtract(pos, prevPos);

        prevPos = pos;

        // Verlet Integration
        pos = Vector2Add(Vector2Add(pos, vel), Vector2Scale(acc, dt * dt));

        acc = {0, 0};
    };

    void Accelerate(Vector2 _acc)
    {
        acc = Vector2Add(acc, _acc);
    };

    void SetPhysicsToDynamic()
    {
        physType = PhysType::DYNAMIC;
    }

    void SetMass(float newMass)
    {
        mass = newMass;
    }

    ColliderType colliderType;
    PhysType physType = PhysType::STATIC;
    Vector2 &pos;
    Vector2 prevPos;
    Vector2 vel;
    Vector2 acc;
    float mass = 1;
};

struct CircleCollider : public Collider
{
    CircleCollider() = delete;
    CircleCollider(Vector2 &_pos, float _radius) : Collider(_pos), radius(_radius)
    {
        colliderType = ColliderType::CIRCLE;
        SetMass(radius);
    }

    float radius;
};

struct RectangleCollider : public Collider
{
    RectangleCollider() = delete;
    RectangleCollider(Vector2 &_pos, float _width, float _height) : Collider(_pos), width(_width), height(_height)
    {
        colliderType = ColliderType::RECTANGLE;
        SetMass(width * height);
    }

    float width;
    float height;
};

struct CollisionInfo
{
    std::string id;
    Vector2 moveAmount = {};
    Vector2 collisionAxis = {};
}; // What it collided with, how much to move by,
class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void Update() = 0;
    virtual void Draw(){};
    virtual void Register(){};
    virtual void OnCollision(const CollisionInfo &collisionInfo)
    {
        if (collider->physType == PhysType::DYNAMIC)
        {
            DefaultCollisionReaction(collisionInfo);
        }
    };

    void SetID(const std::string _id)
    {
        id = _id;
    }

    void SetECS(ECS *_ecs)
    {
        ecs = _ecs;
    };

    void DefaultCollisionReaction(const CollisionInfo &collisionInfo, float restitution = 1, float coefficientOfFriction = 1)
    {
        collider->pos = Vector2Add(collider->pos, collisionInfo.moveAmount);

        Vector2 normal = Vector2Normalize(collisionInfo.collisionAxis);
        Vector2 tangent = Vector2Rotate(normal, PI / 2);

        Vector2 newVel = Vector2Reflect(collider->vel, normal);

        Vector2 normalVelocity = Vector2Scale(Vector2Scale(normal, Vector2DotProduct(newVel, normal)), restitution);
        Vector2 tangentialVelocity = Vector2Scale(Vector2Scale(tangent, Vector2DotProduct(newVel, tangent)), coefficientOfFriction);

        newVel = Vector2Add(normalVelocity, tangentialVelocity);

        collider->prevPos = Vector2Subtract(collider->pos, newVel);
    }
    std::string id;
    ECS *ecs;
    std::unique_ptr<Collider> collider;
};