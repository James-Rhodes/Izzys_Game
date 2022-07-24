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
    DrawText("Hello Worl", GetScreenWidth() / 2, GetScreenHeight() / 2, 0.5, BLACK);
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
