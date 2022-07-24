#pragma once
#include "raylib.h"
#include <math.h>
#include "../EngineFiles/ECS.h"
#include "../EngineFiles/AnimationManager.h"
#include "../SceneFiles/TerrainManager.h"

class CapyFrogHybrid : public Entity
{
public:
    CapyFrogHybrid(Vector2 initPos) : pos(initPos){};

    void Register() override;

    void Update();

    void Draw();

    void UpdateController();

    void OnCollision(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact);
    void OnCollisionEnd(Entity *collidedEntity, bool detectedBySensor, b2Contact *contact);

    Vector2 GetPosition();

    Entity *GetNearestFly();
    Vector2 GetSwingTangentVector(Vector2 bodyPos, Vector2 circleCenter);

    void SetIsAlive(bool _isAlive)
    {
        isAlive = _isAlive;
    };

    Vector2 pos;
    float width = 1;
    float height = 1;
    float speed = 5;
    float jumpHeight = 4;
    float timeOfLastDash = 0;
    float dashRechargeTime = 0.8;
    int isOnGround = 0;
    bool stateWasPreviouslyLocked = false;

    float *screenScrollSpeed = nullptr;

    int currDirection = 1; // 1 = right facing, -1 = left facing
    AnimationManager animManager;
    b2Fixture *feetSensor;

    bool isTouchingSideOfTerrain = false;

    bool isSwinging = false;
    bool isInSwingDismount = false;
    float swingStrength = 0.5;
    float maxDistFromFlyToSwing = 3.5; // Can change this to alter the maximum distance before attaching to a fly fails

    bool isAlive = true;

    Tongue tongue;
};