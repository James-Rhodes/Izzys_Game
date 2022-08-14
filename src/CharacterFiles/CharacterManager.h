#pragma once

#include "raylib.h"
#include "../EngineFiles/ECS.h"
#include "../CharacterFiles/Capybara.h"
#include "../CharacterFiles/Frog.h"
#include "../CharacterFiles/CapyFrogHybrid.h"
#include "../SceneFiles/TerrainManager.h"

// This should facilitate the joining of capy and frog. Potentially deleting frog and capy and replacing with joined sprite/class

class CharacterManager : public Entity
{
public:
    CharacterManager(Vector2 _initCapyPos, Vector2 _initFrogPos) : initCapyPos(_initCapyPos), initFrogPos(_initFrogPos){};

    void Register() override;

    void Update() override;

    bool CapyAndFrogAreGameOver();

    void JoinCapyAndFrog();

    void SeperateCapyAndFrog();

    Vector2 initCapyPos;
    Vector2 initFrogPos;

    int numOrangesCollected = 0;
    int score = 0;
    int distanceTravelled = 0;

    bool capyAndFrogAreJoined = false;

    bool isGameOver = false;
    bool showGameOverScreen = false;

    float scrollOnDeathSlowDown = 2; // Time in seconds til the scene completely slows down to a stop
    float timeSinceGameOver = 0;
    float sceneScrollSpeedAtGameOver = 0;

    Capy *capy = nullptr;
    Frog *frog = nullptr;
    CapyFrogHybrid *capyFrogHybrid = nullptr;
};