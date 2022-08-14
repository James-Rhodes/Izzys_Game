#include "CharacterManager.h"

void CharacterManager::Register()
{
    capy = &ecs->CreateEntity<Capy>("Capy", initCapyPos, &numOrangesCollected);
    frog = &ecs->CreateEntity<Frog>("Frog", initFrogPos, &numOrangesCollected);
}

void CharacterManager::Update()
{
    distanceTravelled = (int)ecs->GetEntity<TerrainManager>("TerrainManager").GetDistanceTravelled();
    score = distanceTravelled + 10 * numOrangesCollected;

    if (!isGameOver && IsKeyPressed(KEY_SPACE))
    {
        if (capyAndFrogAreJoined)
        {
            SeperateCapyAndFrog();
        }
        else
        {
            JoinCapyAndFrog();
        }
    }
    bool prevGameOverState = isGameOver;
    isGameOver = CapyAndFrogAreGameOver();

    if (prevGameOverState != isGameOver)
    {
        sceneScrollSpeedAtGameOver = ecs->GetEntity<TerrainManager>("TerrainManager").GetSceneScrollSpeed();
    }

    if (isGameOver)
    {
        if (!capyAndFrogAreJoined)
        {
            if (capy->isAlive)
            {
                capy->OnDeath();
            }
            else if (frog->isAlive)
            {
                frog->OnDeath();
            }
        }
        // Slow down the scroll speed rather than abrupt stop
        ecs->GetEntity<TerrainManager>("TerrainManager").SetSceneScrollSpeed(Lerp(sceneScrollSpeedAtGameOver, 0, std::min(timeSinceGameOver / scrollOnDeathSlowDown, 1.0f)));
        if (timeSinceGameOver < scrollOnDeathSlowDown)
        {
            timeSinceGameOver += GetFrameTime();
        }
        else
        {
            showGameOverScreen = true;
        }
    }
}

bool CharacterManager::CapyAndFrogAreGameOver()
{
    if (capyAndFrogAreJoined)
    {
        return !capyFrogHybrid->isAlive;
    }

    return !capy->isAlive || !frog->isAlive;
}

void CharacterManager::JoinCapyAndFrog()
{
    if (capy->isTouchingFrog || frog->isTouchingCapy)
    {

        Vector2 capyPos = capy->GetPosition();
        Vector2 frogPos = frog->GetPosition();

        Vector2 spawnPoint = Vector2Lerp(capyPos, frogPos, 0.5); // Midway between the two positions

        ecs->RemoveEntity("Capy");
        ecs->RemoveEntity("Frog");

        capy = nullptr;
        frog = nullptr;

        capyFrogHybrid = &ecs->CreateEntity<CapyFrogHybrid>("CapyFrogHybrid", spawnPoint, &numOrangesCollected);

        capyAndFrogAreJoined = true;
    }
}

void CharacterManager::SeperateCapyAndFrog()
{
    Vector2 spawnPoint = capyFrogHybrid->GetPosition();

    ecs->RemoveEntity("CapyFrogHybrid");
    capyFrogHybrid = nullptr;

    frog = &ecs->CreateEntity<Frog>("Frog", Vector2Add(spawnPoint, {0.5, 0}), &numOrangesCollected);
    capy = &ecs->CreateEntity<Capy>("Capy", Vector2Add(spawnPoint, {-0.5, 0}), &numOrangesCollected);
    capyAndFrogAreJoined = false;
}
