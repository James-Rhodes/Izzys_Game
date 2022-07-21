#pragma once
#include "raylib.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <cmath>

class AnimationManager
{
public:
    AnimationManager(Texture2D _spriteSheet, int _startPosX, int _startPosY, int _spriteWidth, int _spriteHeight) : spriteSheet(_spriteSheet), startPosX(_startPosX), startPosY(_startPosY), spriteWidth(_spriteWidth), spriteHeight(_spriteHeight){};
    AnimationManager() = default;

    // State is the name of the animation, frames is a vector of indices that relates to which images to show in order going left to right top to bottom
    // Accumulator limit is the amount for the accumulator to pass before transitioning to next frame
    void AddAnimation(std::string _state, std::vector<int> _frames, float accumulatorLimit = -1);

    // Returns the Rectangle that is to be used as src for DrawTexturePro
    Rectangle GetTextureRectangle();

    // Sets the current state if not currently locked. Also adds to the accumulator to fascilitate frame change
    void SetState(std::string _state, float valueToAddToAccumulator = 0);

    // Locks the state to a single state for a set amount of time lockTime. This time is updated on each call to GetTextureRectangle.
    void SetStateLock(std::string _state, float _lockTime);

    std::string GetCurrentState();

    // Returns the texture rectangle for a given state. Does not set the state of the object. Will also restart any animations currently running as well as will fail if state is currently locked. Be Warned
    Rectangle GetTextureRectangleAtState(std::string _state);

private:
    struct AnimationInfo
    {
        AnimationInfo(std::vector<int> &_frames, float _accumulatorLimit) : frames(_frames), accumulatorLimit(_accumulatorLimit){};

        std::vector<int> frames;
        float accumulatorLimit;
    };

    std::unordered_map<std::string, std::unique_ptr<AnimationInfo>> m_animMap;
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
    Rectangle GetTextureRectangleAtFrame(int frame);
};

void DrawTextureTiledWithinCamera(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float cameraZoom, Color tint);
Vector2 PixelPerfectClamp(Vector2 vec, int pixelsPerUnit);