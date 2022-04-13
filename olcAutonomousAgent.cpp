#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


struct sVehicle
{
    olc::vf2d acceleration = { 0.0f, 0.0f };
    olc::vf2d velocity = { 0.0f, -2.0f };
    olc::vf2d position;
    float fMaxSpeed = 100.0f;
    float fMaxForce = 4.0f;

    void create(olc::vi2d pos)
    {
        position = pos;
    }

    void update(float fElapsedTime) {
        // Update velocity
        velocity += acceleration;
        // Limit speed
        float fMagSq = velocity.mag2();
        if (fMagSq > fMaxSpeed * fMaxSpeed)
        {
            velocity /= std::sqrt(fMagSq);
            velocity *= fMaxSpeed;
        }
        position += velocity  * fElapsedTime;
        // Reset accelerationelertion to 0 each cycle
        acceleration *= 0.0f;
    }

    void applyForce(olc::vf2d& force) {
        acceleration += force;
    }

    void seek(olc::vf2d target, bool fAvoid = false) {
        olc::vf2d desired = target - position;
        // Scale to maximum speed
        desired.norm();
        desired *= fMaxSpeed;
        // Steering = Desired minus velocity
        olc::vf2d steer = desired - velocity;
        float fMagSq = steer.mag2();
        if (fMagSq > fMaxForce * fMaxForce)
        {
            steer /= std::sqrt(fMagSq);
            steer *= fMaxForce;
        }

        if (fAvoid)
        {
            steer *= -0.7f;
        }

        applyForce(steer);
    }

};



// Override base class with your custom functionality
class olcAutonomousAgent : public olc::PixelGameEngine
{
public:
    olcAutonomousAgent()
    {
        // Name your application
        sAppName = "Autonomous Agent";
    }

private:
    std::vector<sVehicle> vehicles;
    std::vector<olc::vf2d> foods, spiders;
    olc::Renderable gfxFly, gfxPoo, gfxSpider;
    olc::vf2d center;

    int nFlys = 10;
    int nPoos = 100;
    int nSpiders = 10;

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        srand(time(NULL));

        // Load Decals
        gfxFly.Load("./assets/fly.png");
        gfxPoo.Load("./assets/poo.png");
        gfxSpider.Load("./assets/spider.png");

        // Add Vehicles
        for (int i = 0; i < nFlys; i++)
        {
            sVehicle v;
            v.create({ rand() % ScreenWidth(), rand() % ScreenHeight() });
            vehicles.emplace_back(v);
        };

        // Add Food
        for (int i = 0; i < nPoos; i++)
        {
            olc::vi2d f = { rand() % ScreenWidth(), rand() % ScreenHeight() };
            foods.emplace_back(f);
        }

        // Add Spiders
        for (int i = 0; i < nSpiders; i++)
        {
            olc::vi2d f = { rand() % ScreenWidth(), rand() % ScreenHeight() };
            spiders.emplace_back(f);
        }

        center = { ScreenWidth() / 2.0f, ScreenHeight() / 2.0f };

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Called once per frame, draws random coloured pixels
        Clear(olc::VERY_DARK_GREY);

        // Draw foods
        for (auto &f :foods)
        {
            DrawDecal(f - olc::vf2d({ 12, 12 }), gfxPoo.Decal(), { 0.15f, 0.15f });
        }
        DrawDecal(center - olc::vf2d({ 25, 25 }), gfxPoo.Decal(), { 0.5f, 0.5f });

        // Draw and move spiders
        for (auto &s :spiders)
        {
            olc::vi2d movement = { (rand() % 3) - 1, (rand() % 3) - 1 };
            s += movement;
            olc::vf2d offset = { 12, 12 };  //Decal isn't centered!
            DrawDecal(s - offset, gfxSpider.Decal(), { 0.08f, 0.08f });
        }

        //Draw and move vehicle
        for (auto &v :vehicles)
        {
            //seek nearest food
            olc::vf2d closest = { 0.0f, 0.0f };
            float distance = 1000.0f;
        
            for (auto &f :foods)
            {
                float dist = v.position.dist(f);
                if (dist < distance)
                {
                    distance = dist;
                    closest = f;
                }
            }

            if (closest.x != 0.0f && closest.y != 0.0f)
                v.seek(closest);
            else
                v.seek(center);

            //avoid nearest spider
            closest = { 0.0f, 0.0f };
            distance = 1000.0f;
        
            for (auto &s :spiders)
            {
                float dist = v.position.dist(s);
                if (dist < distance)
                {
                    distance = dist;
                    closest = s;
                }
            }

            v.seek(closest, true);

            //move vehicle based on force
            v.update(fElapsedTime);
            DrawRotatedDecal(v.position, gfxFly.Decal(), v.velocity.polar().y, { 30.0f, 40.0f }, { 0.3f, 0.3f });

            //Remove food if vehicle is close
            for (std::vector<olc::vf2d>::iterator it = foods.begin(); it != foods.end(); )
            {
                float dist = v.position.dist(*it);
                if (dist < 5.0f)
                    it = foods.erase(it);
                else 
                    ++it;
            }

        }

        //Remove vehicle if spider is close
        for (auto &s :spiders)
        {
            for (std::vector<sVehicle>::iterator it = vehicles.begin(); it != vehicles.end(); )
            {            
                    float dist = it->position.dist(s);
                    if (dist < 10.0f)
                        it = vehicles.erase(it);
                    else 
                        ++it;
            }
        }

        return true;
    }
};

int main()
{
    olcAutonomousAgent demo;
    if (demo.Construct(640, 360, 2, 2))
        demo.Start();
    return 0;
}