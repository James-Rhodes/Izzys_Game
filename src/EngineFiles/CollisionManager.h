#pragma once

#include "box2d.h"
#include "Entity.h"

// Note: All collision boxes are drawn relative to the center of the shape. Rectangles should in
// turn be drawn relative to the center not the corner. Circles are the same. DrawRectanglePro draws from the center
class CollisionManager : public b2ContactListener
{
public:
    void BeginContact(b2Contact *contact)
    {
        Entity *ent1 = (Entity *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        Entity *ent2 = (Entity *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

        if (ent1 && ent2)
        {
            ent1->OnCollision(ent2, contact->GetFixtureA()->IsSensor());
            ent2->OnCollision(ent1, contact->GetFixtureB()->IsSensor());
        }
    };

    void EndContact(b2Contact *contact)
    {
        Entity *ent1 = (Entity *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        Entity *ent2 = (Entity *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

        if (ent1 && ent2)
        {
            ent1->OnCollisionEnd(ent2, contact->GetFixtureA()->IsSensor());
            ent2->OnCollisionEnd(ent1, contact->GetFixtureB()->IsSensor());
        }
    };
};