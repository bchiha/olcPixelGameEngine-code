// olcFourierSeries
//
// Draw a square wave using Fourier Series.

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

enum wave_t {SQUARE, SAWTOOTH, TRIANGLE, CUBIC, NUM_WAVES};

// Override base class with your custom functionality
class olcFourierSeries : public olc::PixelGameEngine
{
public:
    olcFourierSeries()
    {
        // Name you application
        sAppName = "olc Fourier Series";
    }

private:
    float fDelta = 0.0f;  //change in time
    const float pi = acos(-1);
    int nDepth = 1; //initial depth of fourier iterations
    wave_t eWaveType = SQUARE;
    olc::vf2d circle_orig = {150.0f, 240.0f};  //initial circle origin
    std::vector<float> vWave; //store output wave points


public:
    bool OnUserCreate() override
    {
        vWave.clear();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        //Keyboard Read
        if (GetKey(olc::Key::LEFT).bPressed)
        {
            if (nDepth > 1)
                nDepth -= 1;    //decrease depth
        }
        if (GetKey(olc::Key::RIGHT).bPressed)
        {
            if (nDepth < 50)
                nDepth += 1;    //increase depth
        }
        if (GetKey(olc::Key::SPACE).bPressed)
        {
            //switch between wave types
            eWaveType = static_cast<wave_t>((eWaveType + 1) % NUM_WAVES); 
        }

        fDelta += fElapsedTime * 2;
        olc::vf2d origin = circle_orig;

        //Draw Circles where i is the number of iterations
        for (int i = 0; i < nDepth; i++)
        {
            olc::vf2d pre_orig = origin;  //Save previous origin

            int n = 0;
            float fRadius = 0.0f;
            if (eWaveType == SQUARE)
            {
                // Square Wave Approximation Formula
                n = 2 * i + 1;
                fRadius = 70.0f * (4 / (n * pi));
            }
            else if (eWaveType == SAWTOOTH)
            {
                // Sawtooth Wave Approximation Formula
                n = i + 1;
                if ( n % 2 != 0)
                    n *= -1;  //negate every odd number
                fRadius = 70.0f * (2 / (n * pi));
            }
            else if (eWaveType == TRIANGLE)
            {
                // Triange Wave Approximation Formula
                n = 2 * i + 1;
                fRadius = 70.0f * (8 / ((n * pi) * (n * pi)));
                if (i % 2 == 0)  //every second odd number
                    fRadius *= -1;
            }
            else if (eWaveType == CUBIC)
            {
                // Cubic Wave Approximation Formula
                n = i + 1;
                fRadius = 5.0f * (2 * ((n * n) * (pi * pi) - 6) / (n * n * n));
                if (n % 2 != 0)  //every second odd number
                    fRadius *= -1;
            }
            origin.x += fRadius * cos(abs(n) * fDelta);
            origin.y += fRadius * sin(abs(n) * fDelta);


            DrawCircle(pre_orig,fabs(fRadius),olc::DARK_GREY);

            //Draw previous origin to origin
            FillCircle(origin,1.0f);
            //Draw vector
            DrawLine(pre_orig, origin);
        }

        //Add last Y value to wave vector
        vWave.insert(vWave.begin(), origin.y); //Save Vertical position

        //Draw wave
        for (int i = 0; i < vWave.size() - 1; i++)
        {
            float x = i/8 + circle_orig.x + 150;
            DrawLine(x, vWave[i], x, vWave[i+1], olc::GREEN);
        }
        //Draw virtual pen for wave
        DrawLine(origin.x, origin.y, circle_orig.x + 150, vWave.front(), olc::GREEN);

        //Remove excess points on wave
        if (vWave.size() > 2000)
            vWave.pop_back();

        //Draw instructions
        std::string sWave;
        if (eWaveType == SQUARE)
            sWave = "Square";
        if (eWaveType == SAWTOOTH)
            sWave = "Sawtooth";
        if (eWaveType == TRIANGLE)
            sWave = "Triangle";
        if (eWaveType == CUBIC)
            sWave = "Cubic";

        DrawString(100, 20, "Fourier Series " + sWave + " Wave", olc::WHITE, 2);
        DrawString(100, 420,"Use Arrow keys to change n", olc::WHITE, 2);
        DrawString(100, 450,"Press Space to change Wave", olc::WHITE, 2);

        //Draw current value of n
        DrawString(135, 340, "n = " + std::to_string(nDepth));
        
        return true;
    }
};

int main()
{
    olcFourierSeries demo;
    if (demo.Construct(640, 480, 1, 1))
        demo.Start();
    return 0;
}