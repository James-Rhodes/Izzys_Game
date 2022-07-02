#pragma once

#include "raylib.h"
#include "raymath.h"
#include "box2d.h"
#include <vector>
#include "CatmullRom.h"
class Tongue
{
public:
    Tongue() = default;
    Tongue(int _numChainLinks) : numChainLinks(_numChainLinks)
    {
        chainBodies.resize(numChainLinks);
        std::fill(chainBodies.begin(), chainBodies.end(), nullptr);

        // splineDrawer = CatmullRomSplineDrawer()
    };

    void Create(b2World *world, b2Body *_beginBody, b2Vec2 beginBodyLocalCoords, b2Body *_endBody, b2Vec2 endBodyLocalCoords)
    {
        beginBody = _beginBody;
        endBody = _endBody;

        beginBodyOffset = beginBodyLocalCoords;
        endBodyOffset = endBodyLocalCoords;

        Vector2 beginPos = (Vector2){beginBody->GetPosition().x + beginBodyLocalCoords.x, beginBody->GetPosition().y + beginBodyLocalCoords.y};
        Vector2 endPos = (Vector2){endBody->GetPosition().x + endBodyLocalCoords.x, endBody->GetPosition().y + endBodyLocalCoords.y};

        float distance = Vector2Distance(beginPos, endPos);
        Vector2 gradient = Vector2Divide(Vector2Subtract(endPos, beginPos), {distance, distance});
        float angle = atan2f(gradient.y, gradient.x);

        float boxMoveAmt = 1.0f / numChainLinks;
        float boxWidth = 0.5 * distance / (numChainLinks);
        float currentBoxPosT = boxMoveAmt * 0.5;
        Vector2 boxPos = Vector2Lerp(beginPos, endPos, currentBoxPosT);

        // body and fixture defs are common to all chain links
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(boxPos.x, boxPos.y);
        bodyDef.angle = angle;
        b2FixtureDef fixtureDef;
        fixtureDef.density = 10;
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(boxWidth, 0.05);
        fixtureDef.shape = &polygonShape;

        // create first link
        b2Body *link = world->CreateBody(&bodyDef);
        link->CreateFixture(&fixtureDef);

        b2RevoluteJointDef revoluteJointDef;

        revoluteJointDef.localAnchorA.Set(beginBodyLocalCoords.x, beginBodyLocalCoords.y);
        revoluteJointDef.localAnchorB.Set(-boxWidth, 0);

        revoluteJointDef.bodyA = beginBody;
        revoluteJointDef.bodyB = link;
        world->CreateJoint(&revoluteJointDef);

        revoluteJointDef.localAnchorA.Set(boxWidth, 0);
        revoluteJointDef.localAnchorB.Set(-boxWidth, 0);

        revoluteJointDef.lowerAngle = -maxDeflectionAngle;
        revoluteJointDef.upperAngle = maxDeflectionAngle;
        revoluteJointDef.enableLimit = true;

        // use same definitions to create multiple bodies
        for (int i = 0; i < numChainLinks - 1; i++)
        {
            chainBodies[i] = link;
            currentBoxPosT += boxMoveAmt;
            Vector2 boxPos = Vector2Lerp(beginPos, endPos, currentBoxPosT);
            bodyDef.position.Set(boxPos.x, boxPos.y);
            b2Body *newLink = world->CreateBody(&bodyDef);
            newLink->CreateFixture(&fixtureDef);

            revoluteJointDef.bodyA = link;
            revoluteJointDef.bodyB = newLink;
            world->CreateJoint(&revoluteJointDef);

            link = newLink; // prepare for next iteration
        }
        chainBodies[chainBodies.size() - 1] = link;

        revoluteJointDef.localAnchorB.Set(endBodyLocalCoords.x, endBodyLocalCoords.y);
        revoluteJointDef.bodyA = link;
        revoluteJointDef.bodyB = endBody;
        revoluteJointDef.enableLimit = false;

        world->CreateJoint(&revoluteJointDef);
        isActive = true;
    }

    void Delete(b2World *world)
    {
        if (isActive)
        {
            for (b2Body *body : chainBodies)
            {
                world->DestroyBody(body);
                body = nullptr;
            }
            isActive = false;
        }
        timeSinceTongueActivated = 0;
    };

    void Draw()
    {
        if (isActive)
        {
            timeSinceTongueActivated += GetFrameTime();
            Vector2 beginPos = {beginBody->GetPosition().x + beginBodyOffset.x, beginBody->GetPosition().y + beginBodyOffset.y};
            Vector2 endPos = {endBody->GetPosition().x + endBodyOffset.x, endBody->GetPosition().y + endBodyOffset.y};

            if (timeSinceTongueActivated > tongueExtendTime)
            {
                splineDrawer.DrawCatmullRomSpline(chainBodies, beginBody, beginBodyOffset, endBody, endBodyOffset, 0.1, PINK, false);
                DrawCircleV(endPos, 0.05, PINK);
            }
            else
            {
                float extendPercent = timeSinceTongueActivated / tongueExtendTime;
                Vector2 currentDrawPos = Vector2Lerp(beginPos, endPos, extendPercent);

                DrawLineEx(beginPos, currentDrawPos, 0.1, PINK);
                DrawCircleV(currentDrawPos, 0.05, PINK);
            }

            DrawCircleV(beginPos, 0.05, PINK);
        }
        // DrawLineEx(beginPt, endPt, 0.1, PINK);
    };

    Vector2 GetBeginPos()
    {
        return {beginBody->GetPosition().x, beginBody->GetPosition().y};
    }

    Vector2 GetEndPos()
    {
        return {endBody->GetPosition().x, endBody->GetPosition().y};
    }

private:
    int numChainLinks;
    std::vector<b2Body *> chainBodies;
    b2Body *beginBody;
    b2Body *endBody;
    b2Vec2 beginBodyOffset;
    b2Vec2 endBodyOffset;
    bool isActive = false;
    CatmullRomSplineDrawer splineDrawer;
    float maxDeflectionAngle = PI / 8;

    float tongueExtendTime = 0.075;
    float timeSinceTongueActivated = 0;
};