#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Ball Structure
struct sParticle
{
    olc::vf2d pos;
    olc::vf2d vel;
    olc::vf2d acc;
    olc::Pixel col;
    float fLifespan;
    bool bIsExplosion;

    int id;
};

// Override base class with your custom functionality
class olcFireworks : public olc::PixelGameEngine
{
public:
    olcFireworks()
    {
        // Name you application
        sAppName = "Fireworks";
    }

private:
    const float PI = acos(-1);
    std::vector<sParticle> vecFireworks;
    float gravity = 60.0f;

    void CreateParticle(const olc::vf2d& pos, const olc::vf2d& vel, const olc::Pixel& col = olc::WHITE, const bool isX = false) {
        sParticle p;
        p.pos = pos;
        p.vel = vel;
        p.acc = {0.0f, 0.0f};
        p.col = col;
        p.bIsExplosion = isX;
        p.fLifespan = 0xFF;

        p.id = vecFireworks.size();
        vecFireworks.emplace_back(p);
    }

    void CreateExplosion(const olc::vf2d& pos) {
        olc::Pixel colour = olc::Pixel(rand() % 256, rand() % 256, rand() % 256);
        for (float angle = 0.0f; angle < 2 * PI; angle += 1 / ( 2 * PI))
        {
            olc::vf2d RandVel;

            RandVel.x = sin(angle);
            RandVel.y = cos(angle);
            RandVel *= (rand() % 40);

            CreateParticle(pos, RandVel, colour, true);
        }
    }


public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        srand(time(NULL));

        olc::vf2d RandParticle, RandVel;
        RandParticle.y = ScreenHeight();
        RandVel.x = 0.0f;

        for (int i = 0; i < 7; i++)
        {
            RandParticle.x = rand() % ScreenWidth();
            RandVel.y = - (150 + (rand() % 30));
            CreateParticle(RandParticle, RandVel);
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame, draws random coloured pixels
        Clear(olc::BLACK);

        // remove dead explosions
        for (auto it = vecFireworks.begin(); it != vecFireworks.end(); /* NOTHING */)
        {
            sParticle p = *it;
            if (p.bIsExplosion && p.fLifespan <= 10.0f)
                it = vecFireworks.erase(it);
            else
                ++it;
        }

        // move and draw active particles  
        for (auto &particle :vecFireworks)
        {
            //Move Fireworks
            particle.acc.y += gravity;
            particle.vel += particle.acc * fElapsedTime;
            particle.pos += particle.vel * fElapsedTime;
            particle.acc *= 0.0f;
            
            if (particle.bIsExplosion)
            {
                particle.fLifespan -= 0.1;
                particle.col = olc::Pixel(particle.col.r, particle.col.g, particle.col.b, particle.fLifespan); 
            }
            else if (particle.vel.y >= -20.0f) //nearing peak height
            {
                CreateExplosion(particle.pos);
                //reset firework
                particle.pos.x = rand() % ScreenWidth();
                particle.pos.y = ScreenHeight();
                particle.vel.y = - (150 + (rand() % 30));
            }
            
            Draw(particle.pos, particle.col);
        }

        return true;
    }
};

int main()
{
    olcFireworks demo;
    if (demo.Construct(600, 300, 2, 2))
        demo.Start();
    return 0;
}
