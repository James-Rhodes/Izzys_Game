#pragma once
#include "raylib.h"
#include <array>

class BackgroundManager
{
public:
    BackgroundManager(Texture2D *_backgroundTexture = nullptr, float *_sceneSpeed = nullptr) : backgroundTexture(_backgroundTexture), sceneSpeed(_sceneSpeed)
    {
        for (std::size_t i = 0; i < srcRectangles.size(); i++)
        {
            srcRectangles[i] = {0, i * 360.0f, 640, 360};

            destRectangles[i] = {0, 360, 640, -360};
        }
        parallaxCoefficients = {0, 0.1, 0.3, 0.5, 0.8};
        // There is a wasted draw call here because technically the sky could just be drawn as a flat bg colour
        // seeing as thats what the texture contains. Can remove this for performance later if needed
    };

    void Draw();

    Texture2D *backgroundTexture;
    float *sceneSpeed;
    std::array<Rectangle, 5> srcRectangles;
    std::array<Rectangle, 5> destRectangles;
    std::array<float, 5> parallaxCoefficients;
};