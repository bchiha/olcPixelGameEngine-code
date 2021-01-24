#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class olcCardioid : public olc::PixelGameEngine
{
public:
    olcCardioid()
    {
        // Name you application
        sAppName = "Cardioid Times Tables";
    }

private:
    const float PI = acos(-1);
    olc::Pixel colour[3] = {olc::RED,olc::GREEN,olc::BLUE};
    float fFactor = 0.0f;
    int nTotalPoints = 0.0f;
    float fRadius = 0.0f;
    olc::vf2d vOffset;

    float map(float s, float a1, float a2, float b1, float b2)
    {
        return b1 + (s-a1)*(b2-b1)/(a2-a1);
    }

    olc::vf2d GetVector(int nIndex)
    {
        float fAngle = map(nIndex % nTotalPoints, 0, nTotalPoints, 0, 2 * PI);
        return {fRadius * cos(fAngle + PI), fRadius * sin(fAngle + PI)};
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        fRadius = (ScreenWidth() - 40.0f) / 2.0f;
        vOffset = {ScreenWidth() / 2.0f, ScreenHeight() / 2.0f};
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame, draws random coloured pixels
        Clear(olc::BLACK);
        DrawCircle(vOffset, fRadius, olc::DARK_GREEN);

        fFactor += 0.005;
        nTotalPoints = (int)map(GetMouseX(), 0, ScreenWidth(), 0, 201);
        DrawString(10, 10, "Points: " + std::to_string(nTotalPoints));
        DrawString(10, 20, "Factor: " + std::to_string(fFactor));
        DrawString(10, ScreenHeight() - 20, "Move Mouse to change points");

        for (int i = 0; i < nTotalPoints; i++)
            FillCircle(GetVector(i) + vOffset, 2);

        for (int i = 0; i < nTotalPoints; i++)
        {
            olc::vf2d vPointA = GetVector(i);
            olc::vf2d vPointB = GetVector(i * fFactor);
            DrawLine(vPointA + vOffset, vPointB + vOffset, colour[i % 3]);
        }


        return true;
    }
};

int main()
{
    olcCardioid demo;
    if (demo.Construct(640, 640, 1, 1))
        demo.Start();
    return 0;
}
