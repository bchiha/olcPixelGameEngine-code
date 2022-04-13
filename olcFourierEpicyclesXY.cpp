// olcFourierSeries
//
// Draw a picture using Fourier Transforms.

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <complex>

//store DFT epicycle information
struct t_fourier
{
    std::complex<float> com{0.0f,0.0f};
    size_t nFreq = 0;
    float fAmp = 0.0f;
    float fPhase = 0.0f;
};

// Override base class with your custom functionality
class olcFourierEpicycles : public olc::PixelGameEngine
{
public:
    olcFourierEpicycles()
    {
        // Name you application
        sAppName = "olc Fourier Epicycles";
    }

private:
    const float PI = acos(-1);
    float fDelta = 0.0f;  //change in time
    float fTick = 0.0f;
    int nPic = 0; //initial picture
    std::string sPictures[5] = {"peace.dat","plant.dat","hydra.dat","fouriertransform.dat","note.dat"};

    olc::vf2d y_circle_orig;  //initial circle origin
    olc::vf2d x_circle_orig;  //initial circle origin
    std::vector<olc::vf2d> vPreviousPath; //store previously drawn path
    std::vector<olc::vf2d> vCurrentPath; //store active path

    std::vector<t_fourier> vFourierX;           //Fourier X
    std::vector<std::complex<float>> vSignalX;  //Series X
    std::vector<t_fourier> vFourierY;           //Fourier Y
    std::vector<std::complex<float>> vSignalY;  //Series Y

    //Load Signals with X,Y points from data file
    void getSignalData(std::string filename) 
    {
    	// Open data file
    	float x, y;
    	std::ifstream datafile ("./assets/"+filename);  //datafile

        vSignalX.clear();
        vSignalY.clear();

        if (datafile.is_open())
		{
	        while (datafile.good())
	        {
	    		datafile >> x >> y;
	    		vSignalX.push_back({x,0.0f});
	    		vSignalY.push_back({y,0.0f});
	        }
	        datafile.close();
	    }
    }

    //Calculate the Discrete Fourier Transform on a set of X,Y points
    std::vector<t_fourier> computeDFT(const std::vector<std::complex<float>> &input) {
        std::vector<t_fourier> output;
        const size_t N = input.size();
        for (size_t k = 0; k < N; k++) {  // For each output element
            t_fourier f;
            f.com = {0.0f,0.0f};
            for (size_t n = 0; n < N; n++) {  // For each input element
                float phi = -2 * PI * n * k / N;
                f.com += input[n] * exp(std::complex<float>(0, -phi));
            }
            f.com /= (float)N;
            f.nFreq = k;
            f.fAmp = std::sqrt(f.com.real() * f.com.real() + f.com.imag() * f.com.imag());
            f.fPhase = atan2(f.com.imag(), f.com.real());
            output.push_back(f);
        }

        // sort in decending amplitude size
        std::sort(output.begin(), output.end(), [](const t_fourier &a, const t_fourier &b)
        	{
        		return a.fAmp > b.fAmp;
        	});

        return output;
    }

    //Load Signal data and convert to Fourier data
    void createDFTFromPicture()
    {
    	getSignalData(sPictures[nPic]);

        vFourierX = computeDFT(vSignalX);
        vFourierY = computeDFT(vSignalY);

    }

    //Draw a set of Epicycles based on the Fourier convertion.    
    olc::vf2d drawEpicycle(olc::vf2d origin, float rotation, std::vector<t_fourier> vFourier)
    {
        //Draw Circles where i is the number of iterations
        for (auto &vF : vFourier)
        {
            if (vF.nFreq != 0)  //No rotation, just ignore
            {
                olc::vf2d pre_orig = origin;  //Save previous origin

                size_t nFreq = vF.nFreq;
                float fRadius = vF.fAmp;
                float fPhase = vF.fPhase;
                origin += {fRadius * cos(nFreq * fDelta + fPhase + rotation),
                	       fRadius * sin(nFreq * fDelta + fPhase + rotation)};

                DrawCircle(pre_orig,fabs(fRadius),olc::VERY_DARK_GREY);

                //Draw previous origin to origin
                DrawLine(pre_orig, origin);
            }
        }
        return origin;
    }


public:
    bool OnUserCreate() override
    {

    	y_circle_orig = {100.0f, (float)ScreenHeight()/2};  //initial circle origin
    	x_circle_orig = {(float)ScreenWidth()/2, 50.0f};  //initial circle origin

    	createDFTFromPicture();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {

        //cycle to next picture
        if (GetKey(olc::Key::SPACE).bPressed)
        {
            nPic += 1;
            if (nPic == 5) nPic = 0;
            createDFTFromPicture();
            vCurrentPath.clear();
            vPreviousPath.clear();
            fDelta = 0;
        }

		fTick += fElapsedTime;
        if (fTick > 0.05f)      //Slow down update
        {
	        Clear(olc::BLACK);

			//Draw Epicycles
			olc::vf2d vx1 = drawEpicycle(x_circle_orig, 0.0f, vFourierX);
			olc::vf2d vy1 = drawEpicycle(y_circle_orig, (PI/2), vFourierY);

	        //Add last X,Y value to path vector
	        olc::vf2d v2 = {vx1.x, vy1.y};
	        vCurrentPath.insert(vCurrentPath.begin(), v2); //Save Position

	        //Draw virtual pen between last epicycle and path
	        DrawLine(vx1, v2, olc::GREEN);
	        DrawLine(vy1, v2, olc::GREEN);

            //Draw previous path
            if (!vPreviousPath.empty())
            {
                for (int i = 0; i < vPreviousPath.size() - 1; i++)
                {               
                    DrawLine(vPreviousPath[i], vPreviousPath[i+1], olc::DARK_GREEN);
                }
                //need to join the last and first points at the end..
                DrawLine(vPreviousPath.front(), vPreviousPath.back(), olc::DARK_GREEN);
            }
	        //Draw current path
	        for (int i = 0; i < vCurrentPath.size() - 1; i++)
	        {
	            DrawLine(vCurrentPath[i], vCurrentPath[i+1], olc::GREEN);
	        }

            //Draw text
	        DrawString(50, 20, "Fourier Epicycles", olc::WHITE, 2);
	        DrawString(100, 430,"Use Space to change drawing", olc::WHITE, 2);
	        DrawString(50, 50, "Epicycles used: " + std::to_string(vFourierX.size()));
        	
            //Move delta by fraction of total size.
        	fDelta += (2 * PI) / vFourierX.size();
	        //Remove excess points on wave
	        if (fDelta > (2 * PI))
	        {
	            vPreviousPath = vCurrentPath;
                vCurrentPath.clear();
	            fDelta = 0;
	        }

        	fTick = 0.0f;
        }
        return true;
    }
};

int main()
{
    olcFourierEpicycles demo;
    if (demo.Construct(640, 480, 1, 1))
        demo.Start();
    return 0;
}