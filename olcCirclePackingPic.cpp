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
class olcCirclePackingPic : public olc::PixelGameEngine
{
public:
    olcCirclePackingPic()
    {
        // Name you application
        sAppName = "Circle Packing of Picture";
    }

private:
    std::vector<sCircle>  vCircles;
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

        sprBG = new olc::Sprite("./assets/doggy.png");

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {

        Clear(olc::BLACK);

        olc::vi2d RandCircle = { rand() % ScreenWidth(), rand() % ScreenHeight() };
        
        if (CanAdd(RandCircle))
            AddCircle(RandCircle);

        for (auto &circle : vCircles)
        {
            FillCircle(circle.vPos, circle.fRadius, sprBG->GetPixel(circle.vPos));
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
    olcCirclePackingPic demo;
    if (demo.Construct(640, 556, 1, 1))
        demo.Start();
    return 0;
}
