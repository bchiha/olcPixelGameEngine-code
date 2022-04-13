#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct sCircle
{
    olc::vi2d vPos = { 0, 0 };
    float fRadius = 0.0f;
    bool bCanGrow = true;
    int nId = 0;
};

// Override base class with your custom functionality
class olcCirclePacking : public olc::PixelGameEngine
{
public:
    olcCirclePacking()
    {
        // Name you application
        sAppName = "Circle Packing";
    }

private:
    std::vector<sCircle>  vCircles;
    std::vector<olc::vi2d> vSpots;
    float fInitRadius = 0.0f;
    olc::Sprite *sprBG;


    void AddCircle(const olc::vi2d& _vPos)
    {
        sCircle c;
        c.vPos = _vPos;
        c.fRadius = fInitRadius;
        c.nId = vCircles.size();
        vCircles.emplace_back(c);
    }

    bool HitEdge(sCircle c)
    {
        if (c.vPos.x + c.fRadius >= ScreenWidth() - 1.0 || c.vPos.x - c.fRadius <= 0.0f )
            return true;
        if (c.vPos.y + c.fRadius >= ScreenHeight() - 1.0 || c.vPos.y - c.fRadius <= 0.0f )
            return true;
        return false;
    }

    bool CanAdd(olc::vf2d vNewCircle)
    {
        for (auto circle : vCircles) 
        {
            if (vNewCircle.dist(circle.vPos) <= circle.fRadius + fInitRadius)
                return false;
        }
        return true;
    }

    bool HitCircle(sCircle c)
    {
        for (auto circle : vCircles) 
        {
                if (c.vPos != circle.vPos && c.vPos.dist(circle.vPos) < c.fRadius + circle.fRadius)
                    return true;
        }
        return false;
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        srand(time(NULL));

        sprBG = new olc::Sprite("./assets/olc.png");
        for (int y = 0; y < ScreenHeight(); y++)
        {
            for (int x = 0; x < ScreenWidth(); x++)
            {
                olc::Pixel p = sprBG->GetPixel(x,y);
                if (p == olc::WHITE)
                {
                    olc::vi2d pt = {x,y};
                    vSpots.emplace_back(pt);
                }
            }
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {

        Clear(olc::BLACK);

        int nSpotIndex = rand() % vSpots.size();
        olc::vi2d RandCircle = vSpots[nSpotIndex];
        
        if (CanAdd(RandCircle))
            AddCircle(RandCircle);

        for (auto &circle : vCircles)
        {
            DrawCircle(circle.vPos, circle.fRadius);
            if (HitEdge(circle) || HitCircle(circle))
                circle.bCanGrow = false;
            if (circle.bCanGrow)
                circle.fRadius += 0.05;
        }
        
        return true;
    }
};

int main()
{
    olcCirclePacking demo;
    if (demo.Construct(640, 640, 1, 1))
        demo.Start();
    return 0;
}
