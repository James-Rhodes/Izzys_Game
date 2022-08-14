#include "BackgroundManager.h"

void BackgroundManager::Draw()
{
    float thisFramesSceneSpeed = sceneSpeed == nullptr ? 0 : (*sceneSpeed);
    for (std::size_t i = 0; i < destRectangles.size(); i++)
    {
        float speedPixelsPerSecond = thisFramesSceneSpeed * 64;
        destRectangles[i].x -= GetFrameTime() * speedPixelsPerSecond * parallaxCoefficients[i];

        if (destRectangles[i].x <= -destRectangles[i].width)
        {
            destRectangles[i].x = 0;
        }

        Rectangle nextBackgroundChunk = destRectangles[i];
        nextBackgroundChunk.x += nextBackgroundChunk.width;
        DrawTexturePro(*backgroundTexture, srcRectangles[i], destRectangles[i], {0, 0}, 0, RAYWHITE);
        DrawTexturePro(*backgroundTexture, srcRectangles[i], nextBackgroundChunk, {0, 0}, 0, RAYWHITE);
    }
}

void BackgroundManager::SetSceneSpeedPointer(float *_sceneSpeed)
{
    sceneSpeed = _sceneSpeed;
}
