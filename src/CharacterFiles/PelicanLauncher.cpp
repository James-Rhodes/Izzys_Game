#include "PelicanLauncher.h"
void PelicanLauncher::Register()
{
    pelican = &ecs->CreateEntity<Pelican>("Pelican", (Vector2){-7, 0}); // Create pelican off screen to the left
}
void PelicanLauncher::Update()
{
    TerrainManager &terrainManager = ecs->GetEntity<TerrainManager>("TerrainManager");
    float distance = terrainManager.GetDistanceTravelled();

    if (distance - distanceLastPelicanLaunched > maxDistanceBetweenPelicans)
    {
        // If no pelicans launched for 50m then launch one
        LaunchPelican();
        distanceLastPelicanAttemptedLaunch = distance;
    }

    if (distance - distanceLastPelicanAttemptedLaunch > minDistBetweenPelicans)
    {
        distanceLastPelicanAttemptedLaunch = distance;
        int randValue = GetRandomValue(0, 3);
        if (randValue == 1)
        {
            // if it matches 1->(1/4) then launch a pelican

            LaunchPelican();
        }
    }
}

void PelicanLauncher::LaunchPelican()
{
    TerrainManager &terrainManager = ecs->GetEntity<TerrainManager>("TerrainManager");
    distanceLastPelicanLaunched = terrainManager.GetDistanceTravelled();
    float launchSpeed = terrainManager.GetSceneScrollSpeed() * 3;

    float randHeight = ((float)GetRandomValue(0, 100) * 0.01 * 5.6) - 2.8; // Rand height
    pelican->physBody->SetTransform({7, randHeight}, 0);
    pelican->SetSpeed(launchSpeed);
}
