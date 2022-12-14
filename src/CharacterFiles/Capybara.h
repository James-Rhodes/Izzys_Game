#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
#include "../SceneFiles/TerrainManager.h"

#include "../EngineFiles/Timer.h"

class Capy : public Entity
{
public:
    Capy(Vector2 initPosition, int *orangeScoreKeeper) : pos(initPosition), orangeScore(orangeScoreKeeper)
    {
        // texture = LoadTexture("./assets/Capy_Sprite_Sheet.png");
        // frameSize = {(float)texture.width / 4.0f, (float)texture.height};
    }

    void Register() override;

    void Update();

    void Draw();

    void UpdateController();

    void OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact);
    void OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact);
    bool PositionIsValid();
    void OnDeath();
    void DoJump();

    Vector2 GetPosition();
    void SetIsAlive(bool _isAlive)
    {
        isAlive = _isAlive;
    };

    Vector2 pos;
    float width = 1;
    float height = 0.5;
    float speed = 5;
    float jumpHeight = 2;
    float timeOfLastDash = 0;
    float dashRechargeTime = 0.8;
    int isOnGround = 0;
    bool isAlive = true;
    bool hitPelican = false;
    bool stateWasPreviouslyLocked = false;

    int currDirection = 1; // 1 = right facing, -1 = left facing
    AnimationManager animManager;
    b2Fixture *feetSensor;

    bool isTouchingSideOfTerrain = false;

    int isTouchingFrog = 0;

    int *orangeScore = nullptr;

    // COYOTE_TIME is defined in timer. Yes this is a bad place for it to be defined
    Timer coyoteTimer = Timer(COYOTE_TIME);
    bool jumpWasPressed = false;
};