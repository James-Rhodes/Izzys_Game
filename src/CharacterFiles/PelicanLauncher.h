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

    void Update() override;
};