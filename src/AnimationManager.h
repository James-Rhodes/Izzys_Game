#pragma once
#include "raylib.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

class AnimationManager
{
public:
    AnimationManager(Texture2D _spriteSheet, int _startPosX, int _startPosY, int _spriteWidth, int _spriteHeight) : spriteSheet(_spriteSheet), startPosX(_startPosX), startPosY(_startPosY), spriteWidth(_spriteWidth), spriteHeight(_spriteHeight){};
    AnimationManager() = default;

    void AddAnimation(std::string _state, std::vector<int> _frames, float accumulatorLimit = -1);

    Rectangle GetTextureRectangle();
    Rectangle GetTextureRectangleAtFrame(int frame);
    void SetState(std::string _state, float valueToAddToAccumulator = 0);
    void SetStateLock(std::string _state, float _lockTime);
    std::string GetCurrentState();

    Texture2D spriteSheet;
    int startPosX;
    int startPosY;
    int spriteWidth;
    int spriteHeight;
    std::string currState = "";
    float accumulator = 0;
    int currFrameIndex = 0;
    float lockStartTime = 0;
    bool isLocked = false;
    float lockTime = 0;

private:
    struct AnimationInfo
    {
        AnimationInfo(std::vector<int> &_frames, float _accumulatorLimit) : frames(_frames), accumulatorLimit(_accumulatorLimit){};

        std::vector<int> frames;
        float accumulatorLimit;
    };

    std::unordered_map<std::string, std::unique_ptr<AnimationInfo>> m_animMap;
};
