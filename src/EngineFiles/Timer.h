#pragma once

#include "raylib.h"

#define COYOTE_TIME 0.06 // This is a bad place for this... I also don't know where else to put it. It is the coyote time for frog, capy and the capy frog hybrid

class Timer
{
public:
    Timer(float _amountOfTime) : amountOfTime(_amountOfTime)
    {
    }

    void Start()
    {
        timeOfBeginTimer = GetTime();
        isTimerFinished = false;
    };

    void Update()
    {
        isTimerFinished = GetTime() - timeOfBeginTimer > amountOfTime;
    };

    bool isFinished()
    {
        return isTimerFinished;
    }

    void Stop()
    {
        isTimerFinished = true;
        timeOfBeginTimer = 0;
    }

    float amountOfTime = 0;
    bool isTimerFinished = false;
    float timeOfBeginTimer = 0;
};