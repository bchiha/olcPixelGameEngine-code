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
    olc::vf2d orig_origin; 
    std::vector<olc::vf2d> vPreviousPath; //store and keeps the last path
    std::vector<olc::vf2d> vCurrentPath; //store current drawing path

    std::vector<t_fourier> vFourier;	//DTF data
    std::vector<std::complex<float>> vSignal; //Input data

    //Load Signal with X,Y points from data file
    void getSignalData(std::string filename) 
    {
    	// Open data file
    	float x, y;
    	std::ifstream datafile ("./assets/"+filename);  //datafile

        vSignal.clear();

        if (datafile.is_open())
		{
	        while (datafile.good())
	        {
	    		datafile >> x >> y;
	    		vSignal.push_back({x, y});
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
        vFourier = computeDFT(vSignal);
    }

    //Draw a set of Epicycles based on the Fourier convertion.
    olc::vf2d drawEpicycle(olc::vf2d origin, float rotation, std::vector<t_fourier> vFourier)
    {
        //Draw Circles for each Fourier vector.  
        for (auto &vF : vFourier)
        {
            if (vF.nFreq != 0)  //No Rotation if 0 so just ignore.
            {
	            olc::vf2d pre_orig = origin;  //Save previous origin

	            size_t nFreq = vF.nFreq;
	            float fRadius = vF.fAmp;
	            float fPhase = vF.fPhase;
	            //update new origin.
	            origin += {fRadius * cos(nFreq * fDelta + fPhase + rotation),
	            	       fRadius * sin(nFreq * fDelta + fPhase + rotation)};

	            DrawCircle(pre_orig,fabs(fRadius),olc::VERY_DARK_GREY);

	            //Draw previous origin to origin
	            DrawLine(pre_orig, origin);
	        }
        }
        return origin;  //return the last point to for plotting.
    }


public:
    bool OnUserCreate() override
    {

    	orig_origin = {ScreenWidth()/2.0f, ScreenHeight()/2.0f};
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
        if (fTick > 0.05f) //slow down update
        {
	        Clear(olc::BLACK);

			//Draw Epicycles
			olc::vf2d vx1 = drawEpicycle(orig_origin, 0.0f, vFourier);

	        //Add last X,Y value to path vector
	        olc::vf2d v2 = {vx1.x, vx1.y};
	        vCurrentPath.insert(vCurrentPath.begin(), v2); //Save Position

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
	        DrawString(50, 50, "Epicycles used: " + std::to_string(vFourier.size()));
        	
        	//Move delta by fraction of total size.
        	fDelta += (2 * PI) / vFourier.size();
	        //Reset after full rotation
	        if (fDelta > (2 * PI))
	        {
	            //Save previous path
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