#include "AnimationManager.h"

void AnimationManager::AddAnimation(std::string _state, std::vector<int> _frames, float accumulatorLimit)
{
    if (m_animMap.find(_state) == m_animMap.end())
    {
        m_animMap[_state] = std::make_unique<AnimationInfo>(_frames, accumulatorLimit);
    }
    else
    {

        throw std::invalid_argument("There is already an animation with that name registered.");
        return;
    }
}

Rectangle AnimationManager::GetTextureRectangle()
{
    if (m_animMap.find(currState) == m_animMap.end())
    {
        throw std::invalid_argument("Could not get texture rectangle because the current state of the animation manager has not been set to a valid animation");
        return (Rectangle){0, 0, 0, 0};
    }
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
    if (m_animMap.find(_state) == m_animMap.end())
    {
        throw std::invalid_argument("The animation state you tried to use has not been initialised with the animation manager (ie. does not exist)");
        return;
    }
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
    if (m_animMap.find(_state) == m_animMap.end())
    {
        throw std::invalid_argument("The animation state you tried to use has not been initialised with the animation manager (ie. does not exist)");
        return;
    }
    isLocked = true;
    lockStartTime = GetTime();
    lockTime = _lockTime;
    currState = _state;
}

std::string AnimationManager::GetCurrentState()
{
    return currState;
}

void DrawTextureTiledWithinCamera(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float cameraZoom, Color tint)
{
    // Could probably be done better but this will do for now!

    if ((texture.id <= 0) || (cameraZoom <= 0.0f))
    {
        return; // Wanna see a infinite loop?!...just delete this line!
    }
    if ((source.width == 0) || (source.height == 0))
    {
        return;
    }
    float invCameraZoom = 1 / cameraZoom;
    float destWidthPixels = std::abs(dest.width * cameraZoom);
    float destHeightPixels = std::abs(dest.height * cameraZoom);

    int numXTiles = std::ceil(destWidthPixels / source.width);
    int numYTiles = std::ceil(destHeightPixels / source.height);

    float dx = source.width * invCameraZoom;
    float dy = source.height * invCameraZoom;

    Vector2 currentPos = {dest.x, dest.y};

    Vector2 boundary = {dest.x + dest.width, dest.y + dest.height};

    for (int y = -1; y < numYTiles; y++)
    {
        if (y == -1)
        {
            y = 0; // To make sure the loop runs at least once
        }
        currentPos.x = dest.x;
        for (int x = -1; x < numXTiles; x++)
        {
            if (x == -1)
            {
                x = 0; // To make sure the loop runs at least once
            }
            float remainderX = std::abs(boundary.x - currentPos.x);
            float remainderY = std::abs(boundary.y - currentPos.y);
            Rectangle tempSrc = source;
            tempSrc.width = std::min(source.width, remainderX * cameraZoom);
            tempSrc.height = std::min(source.height, remainderY * cameraZoom);

            Rectangle tempDest = {currentPos.x, currentPos.y, std::min(dx, remainderX), -std::min(dy, remainderY)};
            DrawTexturePro(texture, tempSrc, tempDest, origin, rotation, tint);
            currentPos.x += dx;
        }
        currentPos.y -= dy;
    }
}

Rectangle AnimationManager::GetTextureRectangleAtState(std::string _state)
{
    std::string prevState = currState;

    SetState(_state);
    Rectangle result = GetTextureRectangle();
    SetState(prevState);

    return result;
};