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
}

void CharacterManager::SeperateCapyAndFrog()
{
}
