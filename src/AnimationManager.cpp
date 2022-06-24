#include "AnimationManager.h"

void AnimationManager::AddAnimation(std::string _state, std::vector<int> _frames, float accumulatorLimit)
{
    if (m_animMap.find(_state) == m_animMap.end())
    {
        m_animMap[_state] = std::make_unique<AnimationInfo>(_frames, accumulatorLimit);
    }
}

Rectangle AnimationManager::GetTextureRectangle()
{
    AnimationInfo *animInfo = m_animMap[currState].get();
    if (accumulator >= animInfo->accumulatorLimit)
    {
        currFrameIndex = (currFrameIndex + 1) % animInfo->frames.size();
        accumulator = 0;
    }
    if (isLocked)
    {
        isLocked = (GetTime() - lockStartTime) < lockTime;
    }

    // std::cout << currState << " , " << currFrameIndex << " , " << accumulator << " , " << animInfo->accumulatorLimit << std::endl;
    return GetTextureRectangleAtFrame(animInfo->frames[currFrameIndex]);
}

Rectangle AnimationManager::GetTextureRectangleAtFrame(int frame)
{
    int rectX = (startPosX + (frame * spriteWidth));
    int rectY = (startPosY + ((rectX / spriteSheet.width) * spriteHeight));

    rectX = rectX % spriteSheet.width;

    return (Rectangle){(float)rectX, (float)rectY, (float)spriteWidth, (float)spriteHeight};
}
void AnimationManager::SetState(std::string _state, float valueToAddToAccumulator)
{
    if (!isLocked)
    {
        if (_state != currState)
        {
            currState = _state;
            accumulator = 0;
        }
        else
        {
            accumulator += valueToAddToAccumulator;
        }
    }
}
void AnimationManager::SetStateLock(std::string _state, float _lockTime)
{
    isLocked = true;
    lockStartTime = GetTime();
    lockTime = _lockTime;
    currState = _state;
}

std::string AnimationManager::GetCurrentState()
{
    return currState;
}