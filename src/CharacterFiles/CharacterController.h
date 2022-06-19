#pragma once

#include "box2d.h"

enum CharacterStates
{
    STANDING_STILL,
    RUNNING_LEFT,
    RUNNING_RIGHT,
    JUMPING,
    DASHING
};

class CharacterController
{
public:
    CharacterController() = default;

    virtual CharacterStates Update(b2Body *physBody) = 0;
    CharacterStates currentState;
};