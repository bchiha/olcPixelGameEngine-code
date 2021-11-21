#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "olcPGEX_AdditionalColours.h"
#include <math.h>

// Override base class with your custom functionality
class olcHilbert : public olc::PixelGameEngine
{
public:
    olcHilbert()
    {
        // Name you application
        sAppName = "Hilbert Curve";
    }

private:
    int nOrder = 1;
    int nTotalPoints = 0;
    std::vector<olc::vi2d> vPath;
    float fTick = 0.0f;
    int nCounter = 0;
    bool bIsDrawing = true;

    float map(float s, float a1, float a2, float b1, float b2)
    {
        return b1 + (s-a1)*(b2-b1)/(a2-a1);
    }

    olc::vi2d hilbert(int i)
    {
        olc::vi2d points[] = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
        olc::vi2d point = points[i & 3];
        
        int nTempI = i;

        for (int  j = 1; j < nOrder; j++)
        {
            nTempI = nTempI >> 2;
            int nIndex = nTempI & 3;            
            int nLen = (int)pow(2,j);

            if (nIndex == 0)
            {
                int nTempX = point.x;
                point.x = point.y;
                point.y = nTempX;
            } else if (nIndex == 1)
            {
                point.y += nLen;
            } else if (nIndex == 2)
            {
                point.x += nLen;
                point.y += nLen;
            } else if (nIndex == 3)
            {
                int nTempXM = nLen-1-point.x;
                point.x = nLen-1-point.y;
                point.y = nTempXM;
                point.x += nLen;
            }
        }

        return point;
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        int nPower = (int)pow(2, nOrder);
        int nLen = ScreenWidth() / nPower;
        nTotalPoints = nPower * nPower;

        vPath.clear();

        for (int i = 0; i < nTotalPoints; i++)
        {
            olc::vi2d vPoint = hilbert(i) * nLen;
            vPoint += olc::vi2d(nLen / 2 , nLen / 2);
            vPath.emplace_back(vPoint);
        }

        nCounter = 1;
        bIsDrawing = true;
        Clear(olc::BLACK);
        DrawString(20, ScreenHeight() - 20, "Hilbert Curve Order :" + std::to_string(nOrder));
        DrawString(220, ScreenHeight() - 20, "Use UP/DOWN Arrows to change order");

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame, draws random coloured pixels
        fTick += fElapsedTime;

        //Keys
        if (GetKey(olc::Key::UP).bPressed)
        {
            nOrder++;
            if (nOrder > 9)
                nOrder = 9;
            OnUserCreate();
        }
        if (GetKey(olc::Key::DOWN).bPressed)
        {
            nOrder--;
            if (nOrder < 1)
                nOrder = 1;
            OnUserCreate();
        }
            

        if (fTick > (0.025f / pow(nOrder,2)) && bIsDrawing)
        {
            // draw line
            DrawLine(vPath[nCounter-1],vPath[nCounter],olc::Colours::FromHsv(map(nCounter,0,vPath.size(),0,360), 1.0f, 1.0f));
            fTick = 0.0f;
            nCounter++;
            if (nCounter == vPath.size())
                bIsDrawing = false;
        }

        return true;
    }
};

int main()
{
    olcHilbert demo;
    if (demo.Construct(512, 542, 1, 1))
        demo.Start();
    return 0;
}
