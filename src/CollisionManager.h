#pragma once

#include "box2d.h"
#include "Entity.h"

class CollisionManager : public b2ContactListener
{
public:
    void BeginContact(b2Contact *contact)
    {
        Entity *ent1 = (Entity *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        Entity *ent2 = (Entity *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

        if (ent1 && ent2)
        {
            ent1->OnCollision(ent2);
            ent2->OnCollision(ent1);
        }
    };

    void EndContact(b2Contact *contact)
    {
        Entity *ent1 = (Entity *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        Entity *ent2 = (Entity *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

        if (ent1 && ent2)
        {
            ent1->OnCollisionEnd(ent2);
            ent2->OnCollisionEnd(ent1);
        }
    };
};