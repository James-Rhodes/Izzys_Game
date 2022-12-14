#pragma once

#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
#include "../SceneFiles/TerrainManager.h"

#include "Tongue.h"
#include "../EngineFiles/Timer.h"

class Frog : public Entity
{
public:
    Frog(Vector2 initPosition, int *orangeScoreKeeper) : pos(initPosition), orangeScore(orangeScoreKeeper)
    {
        // texture = LoadTexture("./assets/Capy_Sprite_Sheet.png");
        // frameSize = {(float)texture.width / 4.0f, (float)texture.height};
    }

    void Register() override;

    void Update();

    void Draw();

    void UpdateController();
    void DoJump();

    void OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact);
    void OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact);

    Vector2 GetPosition();

    Entity *GetNearestFly();
    Vector2 GetSwingTangentVector(Vector2 bodyPos, Vector2 circleCenter);
    bool PositionIsValid();
    void OnDeath();
    void SetIsAlive(bool _isAlive)
    {
        isAlive = _isAlive;
    };

    Vector2 pos;
    float width = 0.5;
    float height = 1;
    float speed = 4;
    float jumpHeight = 4;
    int isOnGround = 0;
    bool capybaraIsOnHead = false;

    int currDirection = 1; // 1 = right facing, -1 = left facing
    AnimationManager animManager;
    b2Fixture *feetSensor;
    bool isSwinging = false;
    bool isInSwingDismount = false;
    float swingStrength = 0.5;
    float maxDistFromFlyToSwing = 3.5; // Can change this to alter the maximum distance before attaching to a fly fails

    bool isAlive = true;

    bool isTouchingSideOfTerrain = false;

    int isTouchingCapy = 0;

    bool hitPelican = false;

    Tongue tongue;

    int *orangeScore = nullptr;

    // COYOTE_TIME is defined in timer. Yes this is a bad place for it to be defined
    Timer coyoteTimer = Timer(COYOTE_TIME);
    bool jumpWasPressed = false;
};