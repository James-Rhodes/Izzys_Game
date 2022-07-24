#pragma once

#include "raylib.h"
#include "../EngineFiles/ECS.h"
#include "../CharacterFiles/Capybara.h"
#include "../CharacterFiles/Frog.h"

// This should facilitate the joining of capy and frog. Potentially deleting frog and capy and replacing with joined sprite/class

class CharacterManager : public Entity
{
public:
    CharacterManager(Vector2 _initCapyPos, Vector2 _initFrogPos) : initCapyPos(_initCapyPos), initFrogPos(_initFrogPos){};

    void Update() override;

    void Draw() override; // Draw text for score etc

    void DrawGameOver();

    void JoinCapyAndFrog();

    void SeperateCapyAndFrog();

    Vector2 initCapyPos;
    Vector2 initFrogPos;

    float currDistanceTravelled = 0;
    float numOrangesCollected = 0;

    Capy *capy = nullptr;
    Frog *frog = nullptr;
};