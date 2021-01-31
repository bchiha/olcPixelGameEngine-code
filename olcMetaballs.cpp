
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "olcPGEX_AdditionalColours.h"

// Ball Structure
struct sBall
{
    olc::vf2d pt;
    olc::vf2d vel;
    olc::vf2d acc;
    float radius;
};


// Override base class with your custom functionality
class olcMetaballs : public olc::PixelGameEngine
{
public:
    olcMetaballs()
    {
        // Name you application
        sAppName = "Metaballs";
    }

private:
    std::vector<sBall> vBalls;

    void AddBall(const olc::vf2d& pos, float r = 40.0f)
    {
        sBall b;
        b.pt = pos;
        olc::vf2d vel;
        vel.x = 1 + (int)rand() % 10;
        vel.y = 1 + (int)rand() % 10;
        vel.norm();
        b.vel = vel * 10 ;
        b.radius = r;
        vBalls.emplace_back(b);
    }

    float map(float s, float a1, float a2, float b1, float b2)
    {
        return b1 + (s-a1)*(b2-b1)/(a2-a1);
    }


public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        srand(time(NULL));
        for (int i = 0; i < 7; i++)
        {
            olc::vf2d RandBall;
            RandBall.x = rand() % ScreenWidth();
            RandBall.y = rand() % ScreenHeight();
            AddBall(RandBall, (float)(rand() % 30 + 2));
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame, draws random coloured pixels
        for (auto &ball :vBalls)
        {
            ball.pt += ball.vel * fElapsedTime;

            if (ball.pt.x < 0 || ball.pt.x >= ScreenWidth()) ball.vel.x *= -1;
            if (ball.pt.y < 0 || ball.pt.y >= ScreenHeight()) ball.vel.y *= -1;
        }

        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
            {
                float sum = 0.0f;
                for (auto ball :vBalls)
                {
                    olc::vf2d vPoint {(float)x, (float)y};
                    sum += 200 * ball.radius / vPoint.dist(ball.pt);
                }
                Draw(x, y, olc::Colours::FromHsv(sum, 255, 255));
            }

        return true;
    }
};

int main()
{
    olcMetaballs demo;
    if (demo.Construct(400, 300, 2, 2))
        demo.Start();
    return 0;
}