#pragma once
#include <list>
#include "Entity.h"
#include <iostream>
#include <math.h>
// #include "raylib.h"
class PhysicsManager
{
public:
    PhysicsManager(){};

    void Update(float dt, const std::list<Entity *> &physEntities)
    {
        int numPhysicsSteps = 20;
        float subDT = dt / numPhysicsSteps;
        // float subDT = dt;

        for (int i = 0; i < numPhysicsSteps; i++)
        {
            ApplyGravity(physEntities);
            SolveCollisions(physEntities);
            UpdatePositions(subDT, physEntities);
        }
    };

    // This is used to keep an object in a bounding box
    void ApplyConstraints(const std::list<Entity *> &physEntities){

    };

    // Solves collisions of all physics entities Call OnCOllision of entity object passing in the info -> accumulate acceleration
    void SolveCollisions(const std::list<Entity *> &physEntities)
    {
        for (auto obj1 = physEntities.begin(); obj1 != physEntities.end(); obj1++)
        {
            for (auto obj2 = std::next(obj1, 1); obj2 != physEntities.end(); obj2++)
            {
                // std::cout << (*obj1)->id << " " + (*obj2)->id << std::endl;
                ColliderType obj1Type = (*obj1)->collider->colliderType;
                ColliderType obj2Type = (*obj2)->collider->colliderType;

                if (obj1Type == ColliderType::CIRCLE && obj2Type == ColliderType::CIRCLE)
                {
                    SolveCollisionCircleToCircle(*obj1, *obj2);
                }
                else if (obj1Type == ColliderType::RECTANGLE && obj2Type == ColliderType::CIRCLE)
                {
                    SolveCollisionCircleToRect(*obj2, *obj1);
                }
                else if (obj1Type == ColliderType::CIRCLE && obj2Type == ColliderType::RECTANGLE)
                {
                    SolveCollisionCircleToRect(*obj1, *obj2);
                }
                else if (obj1Type == ColliderType::RECTANGLE && obj2Type == ColliderType::RECTANGLE)
                {
                    SolveCollisionRectToRect(*obj1, *obj2);
                }
            }
        }
    };

    // Applies acceleration due to gravity
    void ApplyGravity(const std::list<Entity *> &physEntities)
    {
        for (auto ent : physEntities)
        {
            if (ent->collider->physType == PhysType::DYNAMIC)
            {
                ent->collider->Accelerate(gravity);
            }
        }
    };

    // Call UpdatePhysics of Entity
    void UpdatePositions(float dt, const std::list<Entity *> &physEntities)
    {
        for (auto ent : physEntities)
        {
            if (ent->collider->physType == PhysType::DYNAMIC)
            {
                ent->collider->UpdatePosition(dt);
            }
        }
    };

    void ApplyCollisions(Entity *obj1, Entity *obj2, const Vector2 &collisionAxis, float overlapAmount)
    {
        CollisionInfo collision1 = {obj2->id, {0, 0}, Vector2Scale(collisionAxis, -1)};
        CollisionInfo collision2 = {obj1->id, {0, 0}, collisionAxis};

        if (obj1->collider->physType == PhysType::DYNAMIC && obj2->collider->physType == PhysType::DYNAMIC)
        {
            float moveAmount1 = overlapAmount * (obj2->collider->mass / (obj1->collider->mass + obj2->collider->mass));
            float moveAmount2 = overlapAmount * (obj1->collider->mass / (obj1->collider->mass + obj2->collider->mass));
            Vector2 colAxisNormal = Vector2Normalize(collisionAxis);
            collision1.moveAmount = Vector2Scale(colAxisNormal, moveAmount1);
            collision2.moveAmount = Vector2Scale(colAxisNormal, -moveAmount2);
        }
        else if (obj1->collider->physType == PhysType::DYNAMIC && obj2->collider->physType == PhysType::STATIC)
        {
            collision1.moveAmount = Vector2Scale(Vector2Normalize(collisionAxis), overlapAmount);
        }
        else if (obj1->collider->physType == PhysType::STATIC && obj2->collider->physType == PhysType::DYNAMIC)
        {
            collision2.moveAmount = Vector2Scale(Vector2Normalize(collisionAxis), -overlapAmount);
        }
        // else if (col1->physType == PhysType::STATIC && col2->physType == PhysType::STATIC)
        // {
        //     // Do nothing
        // }

        obj1->OnCollision(collision1);
        obj2->OnCollision(collision2);
    }

    void SolveCollisionRectToRect(Entity *obj1, Entity *obj2)
    {
        RectangleCollider *col1 = (RectangleCollider *)obj1->collider.get();
        RectangleCollider *col2 = (RectangleCollider *)obj2->collider.get();

        Vector2 rec1P1 = col1->pos;                                       // Top left corner
        Vector2 rec1P2 = Vector2Add(rec1P1, {col1->width, col1->height}); // Bottom right corner

        Vector2 rec2P1 = col2->pos;                                       // Top left corner
        Vector2 rec2P2 = Vector2Add(rec2P1, {col2->width, col2->height}); // Bottom right corner

        bool collided = rec1P1.x < rec2P2.x && rec1P2.x > rec2P1.x && rec1P1.y < rec2P2.y && rec1P2.y > rec2P1.y;
        if (collided)
        {
            // std::cout << "We collided" << std::endl;

            Vector2 overlappingBox = {}; // Box created from the two overlapping rectangles. x will be the width and y will be the height

            overlappingBox.x = std::min(rec1P1.x < rec2P1.x ? (rec1P2.x - rec2P1.x) : (rec2P2.x - rec1P1.x), std::min(col1->height, col2->height));
            overlappingBox.y = std::min(rec1P1.y < rec2P1.y ? (rec1P2.y - rec2P1.y) : (rec2P2.y - rec1P1.y), std::min(col1->width, col2->width));

            Vector2 collisionAxis = overlappingBox.x > overlappingBox.y ? (Vector2){0, 1} : (Vector2){1, 0};
            float overlapAmount = std::min(overlappingBox.x, overlappingBox.y);

            ApplyCollisions(obj1, obj2, collisionAxis, -1 * overlapAmount);
            return;
        }
    };

    void SolveCollisionCircleToCircle(Entity *obj1, Entity *obj2)
    {
        CircleCollider *col1 = (CircleCollider *)obj1->collider.get();
        CircleCollider *col2 = (CircleCollider *)obj2->collider.get();

        Vector2 collisionAxis = Vector2Subtract(col1->pos, col2->pos);

        if (Vector2LengthSqr(collisionAxis) < (col1->radius + col2->radius) * (col1->radius + col2->radius))
        {
            // Collided do stuff
            float overlapAmount = (col1->radius + col2->radius) - Vector2Length(collisionAxis);

            ApplyCollisions(obj1, obj2, collisionAxis, overlapAmount);
            return;
        }
    };

    void SolveCollisionCircleToRect(Entity *obj1, Entity *obj2)
    {
        CircleCollider *col1 = (CircleCollider *)obj1->collider.get();
        RectangleCollider *col2 = (RectangleCollider *)obj2->collider.get();

        Vector2 recCenter = Vector2Add(col2->pos, {col2->width / 2, col2->height / 2}); // Top left corner
        Vector2 distance = {abs(col1->pos.x - recCenter.x), abs(col1->pos.y - recCenter.y)};

        float centerDistanceSq = Vector2LengthSqr(Vector2Subtract(distance, {col2->width / 2, col2->height / 2}));

        bool collided = (distance.x <= (col2->width / 2 + col1->radius) && distance.y <= (col2->height / 2 + col1->radius)) || centerDistanceSq <= (col1->radius * col1->radius);
        if (collided)
        {

            float nearestX = std::max(col2->pos.x, std::min(col1->pos.x, col2->pos.x + col2->width));

            float nearestY = std::max(col2->pos.y, std::min(col1->pos.y, col2->pos.y + col2->height));

            Vector2 dist = {col1->pos.x - nearestX, col1->pos.y - nearestY};

            float penetrationDepth = col1->radius - Vector2Length(dist);
            Vector2 penetrationVector = Vector2Scale(Vector2Normalize(dist), penetrationDepth);

            ApplyCollisions(obj1, obj2, penetrationVector, penetrationDepth);
        };
    };

    Vector2 gravity = {0, 300};
};