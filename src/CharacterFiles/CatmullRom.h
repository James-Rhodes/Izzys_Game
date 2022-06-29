#pragma once
#include "raylib.h"
#include "raymath.h"
#include "box2d.h"

class CatmullRomSplineDrawer
{
public:
    // CatmullRomSplineDrawer() = default;
    CatmullRomSplineDrawer(int _smoothness = 100, float _alpha = 0.5) : smoothness(_smoothness), alpha(_alpha){};

    void DrawCatmullRomSpline(const std::vector<b2Body *> &P, Vector2 beginPos, Vector2 endPos, float thickness, Color col, bool drawControlPts = false)
    {
        int numPts = P.size() + 2;
        if (P.size() < 4)
        {
            throw std::invalid_argument("ERROR: Not enough points for drawing Catmull Rom Spline. At least 4 is required.");
            return;
        }

        // Calculate Control Pts
        Vector2 beginControlPt = b2Vec2ToVector2(P[0]->GetPosition() + (P[0]->GetPosition() - P[1]->GetPosition()));
        Vector2 endControlPt = b2Vec2ToVector2(P[numPts - 1]->GetPosition() + (P[numPts - 1]->GetPosition() - P[numPts - 2]->GetPosition()));

        if (drawControlPts)
        {
            DrawCircleV(beginControlPt, 0.1, RED);
            DrawCircleV(endControlPt, 0.1, RED);
        }

        Vector2 *spline;
        // spline = CatmullRom4Pts(beginControlPt, b2Vec2ToVector2(P[0]->GetPosition()), b2Vec2ToVector2(P[1]->GetPosition()), b2Vec2ToVector2(P[2]->GetPosition()));
        spline = CatmullRom4Pts(beginControlPt, beginPos, b2Vec2ToVector2(P[0]->GetPosition()), b2Vec2ToVector2(P[1]->GetPosition()));

        for (int i = 0; i < smoothness - 1; i++)
        {
            DrawLineEx(spline[i], spline[i + 1], thickness, col);
        }
        delete[] spline;

        for (int i = 0; i < numPts - 3; i++)
        {
            spline = CatmullRom4Pts(b2Vec2ToVector2(P[i]->GetPosition()), b2Vec2ToVector2(P[i + 1]->GetPosition()), b2Vec2ToVector2(P[i + 2]->GetPosition()), b2Vec2ToVector2(P[i + 3]->GetPosition()));
            for (int j = 0; j < smoothness - 1; j++)
            {
                DrawLineEx(spline[j], spline[j + 1], thickness, col);
            }
            delete[] spline;
        }
        spline = CatmullRom4Pts(b2Vec2ToVector2(P[numPts - 3]->GetPosition()), b2Vec2ToVector2(P[numPts - 2]->GetPosition()), b2Vec2ToVector2(P[numPts - 1]->GetPosition()), endPos);
        for (int i = 0; i < smoothness - 1; i++)
        {
            DrawLineEx(spline[i], spline[i + 1], thickness, col);
        }
        delete[] spline;
    }

private:
    int smoothness;
    float alpha;

    Vector2 b2Vec2ToVector2(b2Vec2 pt)
    {
        return (Vector2){pt.x, pt.y};
    };

    Vector2 *CatmullRom4Pts(const Vector2 &P0, const Vector2 &P1, const Vector2 &P2, const Vector2 &P3)
    {
        float t0 = 0;
        float t1 = tj(t0, P0, P1);
        float t2 = tj(t1, P1, P2);
        float t3 = tj(t2, P2, P3);

        float *t = linSpace(t1, t2, smoothness);
        Vector2 *C = new Vector2[smoothness];
        for (int i = 0; i < smoothness; i++)
        {
            Vector2 A1 = Vector2Add(Vector2Scale(P0, ((t1 - t[i]) / (t1 - t0))), Vector2Scale(P1, ((t[i] - t0) / (t1 - t0))));
            Vector2 A2 = Vector2Add(Vector2Scale(P1, ((t2 - t[i]) / (t2 - t1))), Vector2Scale(P2, ((t[i] - t1) / (t2 - t1))));
            Vector2 A3 = Vector2Add(Vector2Scale(P2, ((t3 - t[i]) / (t3 - t2))), Vector2Scale(P3, ((t[i] - t2) / (t3 - t2))));

            Vector2 B1 = Vector2Add(Vector2Scale(A1, ((t2 - t[i]) / (t2 - t0))), Vector2Scale(A2, ((t[i] - t0) / (t2 - t0))));
            Vector2 B2 = Vector2Add(Vector2Scale(A2, ((t3 - t[i]) / (t3 - t1))), Vector2Scale(A3, ((t[i] - t1) / (t3 - t1))));

            C[i] = Vector2Add(Vector2Scale(B1, ((t2 - t[i]) / (t2 - t1))), Vector2Scale(B2, ((t[i] - t1) / (t2 - t1))));
        }

        delete[] t;
        return C;
    }

    float tj(float ti, const Vector2 &Pi, const Vector2 &Pj)
    {
        float xi = Pi.x;
        float yi = Pi.y;
        float xj = Pj.x;
        float yj = Pj.y;

        return pow(((xj - xi) * (xj - xi)) + ((yj - yi) * (yj - yi)), alpha / 2) + ti;
    }

    float *linSpace(float start, float end, int numPoints)
    {
        float *arr = new float[numPoints];
        float diff = (end - start) / (numPoints - 1);
        arr[0] = start;
        for (int i = 1; i < numPoints; i++)
        {
            arr[i] = arr[i - 1] + diff;
        }
        return arr;
    }
};