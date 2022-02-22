#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class olcPrimeSpiral : public olc::PixelGameEngine
{
public:
    olcPrimeSpiral()
    {
        // Name you application
        sAppName = "olcPrimeSpiral";
    }

private:
    olc::vi2d current, previous;
    int count = 0;  //Number count
    int stepsRemaining = 0;  //Steps per movement
    int stepsNeeded = 1; //Number of steps needed
    int walkDir = 0;   //Movement Direction
    int gapIndex = 4; //Pixels per step index 
    int gapSizes[16] = {1,2,4,5,8,10,16,20,25,32,40,50,80,100,160,200}; //good gap sizes
    bool drawSpiral = false; //draw spiral

    bool isPrime(int n)
    {
        // Corner case
        if (n <= 1)
            return false;
      
        // Check from 2 to sqrt(n)
        for (int i = 2; i <= std::sqrt(n); i++)
            if (n % i == 0)
                return false;
      
        return true;
    }

    void walk(int dir)
    {
        //move current, decrease steps, increase steps needed
        switch(dir) {
            case 0 : current.x += gapSizes[gapIndex];
                     break;
            case 1 : current.y -= gapSizes[gapIndex];
                     break;
            case 2 : current.x -= gapSizes[gapIndex];
                     break;
            case 3 : current.y += gapSizes[gapIndex];
                     break;
        }
        stepsRemaining--;
    }

    void resetSpiral()
    {
        Clear(olc::BLACK);
        count = 0;
        stepsNeeded = 1;
        stepsRemaining = stepsNeeded;
        walkDir = 0;
        current = { ScreenWidth() / 2, ScreenHeight() / 2 };
    }


public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        resetSpiral();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {

        if (GetKey(olc::Key::R).bPressed)  //Reset Spiral
            resetSpiral();

        if (GetKey(olc::Key::T).bPressed)  //Draw Spiral
        {
            resetSpiral();
            drawSpiral = !drawSpiral;
        }

        if (GetKey(olc::Key::LEFT).bPressed) //Decrease gap size
        {
            resetSpiral();
            if (gapIndex > 0)
                gapIndex--;
        }

        if (GetKey(olc::Key::RIGHT).bPressed) //Increase gap size
        {
            resetSpiral();
            if (gapIndex < 15)
                gapIndex++;
        }

        if (stepsNeeded * gapSizes[gapIndex] <= ScreenWidth())  //Update if still on screen
        {
            //Increase number count
            count++;
            //Draw if prime
            if (isPrime(count))
                FillCircle(current, gapSizes[gapIndex] / 4, olc::WHITE);

            //Move current
            previous = current;
            walk(walkDir);
            if (drawSpiral)
                DrawLine(previous, current, olc::RED);

            //Update Steps Remaining and walk direction
            if (stepsRemaining == 0)
            {
                walkDir++;    //Change direction
                walkDir %= 4; //Loop value between 0 and 3
                //Increase steps if two sizes are done
                if (walkDir == 0 || walkDir == 2)
                    stepsNeeded++;
                stepsRemaining = stepsNeeded;

            }
        }

        //Draw Menu
        FillRect({ 40, 740 }, { 720, 50 }, olc::BLUE);
        DrawString({ 50, 750 }, "Keys: (R)eset Spiral, (T)oggle Line, ", olc::GREEN, 2);
        DrawString({ 50, 770 }, "      (<-) Decrease Gap, (->) Increase Gap", olc::GREEN, 2);
        FillRect({ 600, 20 }, { 150, 30 }, olc::BLUE);
        DrawString({ 610, 30 }, "GAP: " + std::to_string(gapSizes[gapIndex]), olc::GREEN, 2);

        return true;
    }
};

int main()
{
    olcPrimeSpiral demo;
    if (demo.Construct(800, 800, 1, 1))
        demo.Start();
    return 0;
}
