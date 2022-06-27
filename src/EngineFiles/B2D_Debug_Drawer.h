#pragma once
#include "raylib.h"
#include "box2d.h"
#include <iostream>
// #include <cfloat>

class DebugDrawer : public b2Draw
{
public:
    bool enabled = false;

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override
    {
        Color col = (Color){(unsigned char)(color.r * 255), (unsigned char)(color.g * 255), (unsigned char)(color.b * 255), (unsigned char)(color.a * 255)};

        for (int i = 0; i < vertexCount; i++)
        {
            Vector2 startPos = {vertices[i].x, vertices[i].y};
            Vector2 endPos = {vertices[(i + 1) % vertexCount].x, vertices[(i + 1) % vertexCount].y};
            DrawLineV(startPos, endPos, col);
        }
    }
    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override
    {
        Color col = (Color){(unsigned char)(color.r * 255), (unsigned char)(color.g * 255), (unsigned char)(color.b * 255), (unsigned char)(color.a * 255)};

        for (int i = 0; i < vertexCount; i++)
        {
            Vector2 startPos = {vertices[i].x, vertices[i].y};
            Vector2 endPos = {vertices[(i + 1) % vertexCount].x, vertices[(i + 1) % vertexCount].y};
            DrawLineV(startPos, endPos, col);
        }
    }
    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override
    {
        Color col = (Color){(unsigned char)(color.r * 255), (unsigned char)(color.g * 255), (unsigned char)(color.b * 255), (unsigned char)(color.a * 255)};
        DrawRing((Vector2){center.x, center.y}, 3 * radius / 4, radius, 0, 360, 10, col);
    }
    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override
    {
        Color col = (Color){(unsigned char)(color.r * 255), (unsigned char)(color.g * 255), (unsigned char)(color.b * 255), (unsigned char)(color.a * 255)};
        DrawCircleV((Vector2){center.x, center.y}, radius, col);
    }
    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override
    {
        Color col = (Color){(unsigned char)(color.r * 255), (unsigned char)(color.g * 255), (unsigned char)(color.b * 255), (unsigned char)(color.a * 255)};
        // DrawLineV((Vector2){p1.x, p1.y}, (Vector2){p2.x, p2.y}, col);
        DrawLineEx((Vector2){p1.x, p1.y}, (Vector2){p2.x, p2.y}, 0.1, col);
    }
    void DrawTransform(const b2Transform &xf) override
    {
        // Can improve later
        DrawCircleV((Vector2){xf.p.x, xf.p.y}, 0.1, RED);
    }

    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override
    {
        Color col = (Color){(unsigned char)(color.r * 255), (unsigned char)(color.g * 255), (unsigned char)(color.b * 255), (unsigned char)(color.a * 255)};
        DrawCircleV((Vector2){p.x, p.y}, size / 60, col);
    }
};