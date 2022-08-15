#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
#include "../SceneFiles/TerrainManager.h"
#include "Pelican.h"

class PelicanLauncher : public Entity
{
public:
    PelicanLauncher(){};

    void Register() override;
    void Update() override;

    void LaunchPelican();

    float distanceLastPelicanLaunched = 0;
    float distanceLastPelicanAttemptedLaunch = 0;
    Pelican *pelican;
    float maxDistanceBetweenPelicans = 50;
    float minDistBetweenPelicans = 10;
};