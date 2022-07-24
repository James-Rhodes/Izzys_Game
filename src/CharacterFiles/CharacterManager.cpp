#include "CharacterManager.h"

void CharacterManager::Register()
{
    ecs->RegisterEntityAsScreenSpaceDrawable(id);

    camera = ecs->GetCamera();
    capy = &ecs->CreateEntity<Capy>("Capy", initCapyPos);
    frog = &ecs->CreateEntity<Frog>("Frog", initFrogPos);
}

void CharacterManager::Update()
{
    if (IsKeyPressed(KEY_SPACE))
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
}

void CharacterManager::Draw()
{
    float distanceTravelled = ecs->GetEntity<TerrainManager>("TerrainManager").GetDistanceTravelled();
    const char *text = TextFormat("Distance: %.2f", distanceTravelled);
    int textWidth = MeasureText(text, 20);

    DrawText(text, GetScreenWidth() * 0.5 - textWidth * 0.5, 10, 20, BLACK);
    // DrawTextPro(*ecs->GetFont(), "HelloWorl", {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}, {0, 0}, 180, 12, 2, BLACK);
}

void CharacterManager::DrawGameOver()
{
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

        capyFrogHybrid = &ecs->CreateEntity<CapyFrogHybrid>("CapyFrogHybrid", spawnPoint);

        capyAndFrogAreJoined = true;
    }
}

void CharacterManager::SeperateCapyAndFrog()
{
    Vector2 spawnPoint = capyFrogHybrid->GetPosition();

    ecs->RemoveEntity("CapyFrogHybrid");
    capyFrogHybrid = nullptr;

    frog = &ecs->CreateEntity<Frog>("Frog", Vector2Add(spawnPoint, {0.5, 0}));
    capy = &ecs->CreateEntity<Capy>("Capy", Vector2Add(spawnPoint, {-0.5, 0}));
    capyAndFrogAreJoined = false;
}
