#include "CharacterManager.h"

void CharacterManager::Register()
{
    ecs->RegisterEntityAsScreenSpaceDrawable(id);

    camera = ecs->GetCamera();
    capy = &ecs->CreateEntity<Capy>("Capy", initCapyPos, &numOrangesCollected);
    frog = &ecs->CreateEntity<Frog>("Frog", initFrogPos, &numOrangesCollected);
}

void CharacterManager::Update()
{
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
        OnGameOver();
    }
}

void CharacterManager::Draw()
{
    float distanceTravelled = ecs->GetEntity<TerrainManager>("TerrainManager").GetDistanceTravelled();

    if (!isGameOver)
    {
        const char *text = TextFormat("Distance: %.2f  Oranges: %d", distanceTravelled, numOrangesCollected);
        int textWidth = MeasureText(text, 20);

        DrawText(text, GetScreenWidth() * 0.5 - textWidth * 0.5, 10, 20, BLACK);
    }
    else
    {
        DrawGameOver();
    }
}

void CharacterManager::DrawGameOver()
{
    // std::cout << "Game be over my dude" << std::endl;
}

void CharacterManager::OnGameOver()
{
    std::cout << "Game Ended" << std::endl;
    ecs->GetEntity<TerrainManager>("TerrainManager").SetSceneScrollSpeed(0);
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
