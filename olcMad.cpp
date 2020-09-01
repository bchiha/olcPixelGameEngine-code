// Mad Magazine
//
// Pixel Game Engine version of the Mad Magazine code that draws the face of Alfred E Neuman.
//
// Uses almost that same code to load data point pairs and draw a line between them.  I had to 
// slow the draw down to simulate an old 8-bit processor!  Lines are drawn every 5 milliseconds.

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct sLine
{
    float sx, sy; // Start coordinate
    float ex, ey; // End coordinate
};

// Override base class with your custom functionality
class olcMad : public olc::PixelGameEngine
{
public:
    olcMad()
    {
        // Name you application
        sAppName = "MAD";
    }

private:
    
    std::vector<sLine> vecLines;   //line coordinates
    float fSize = 1.2f;       //zoom size
    float fXCenter = ScreenWidth()/2;  //center of screen
    float fYCenter = ScreenHeight()/2;
    float fXstart, fXend, fYstart, fYend; //line data
    float fTick = 0.0f;      //timer for line drawing
    bool bIsDrawing = true;  //used to ignore screen update when done.
    std::ifstream datafile;  //datafile

public:
    bool OnUserCreate() override
    {
        // Open data file
        datafile.open("maddata.txt");  //Open data pair file
        if (!datafile.is_open())
            return false;
        
        Clear(olc::WHITE);
        return true;
    }

    // data file is structured in groups of 4.  start x,y and end x,y.  I load all four at once
    // and then with some screen center offsets, draw a line.  I draw a second line using the same
    // points, just shifted one pixel to the left to give it a brush stroke thicker effect.
    bool OnUserUpdate(float fElapsedTime) override
    {
        fTick += fElapsedTime;
        if (fTick > 0.05f && bIsDrawing)   // do if only 5 milliseconds has passed
        {        
            if (!datafile.eof())  // if more data
            {
                sLine l;
                datafile >> l.sx >> l.sy >> l.ex >> l.ey;
                fXstart = l.sx * fSize + fXCenter; 
                fYstart = 226.0f - (l.sy + fYCenter);
                fXend = l.ex * fSize + fXCenter; 
                fYend = 226.0f - (l.ey + fYCenter);
                DrawLine(fXstart, fYstart, fXend, fYend, olc::BLACK);
                DrawLine(fXstart + 1, fYstart, fXend + 1, fYend, olc::BLACK);
            }
            else  // at the end of the drawing, write the end text.
            {
                DrawString(75, 200, "WHAT, ME WORRY?", olc::BLACK,1);
                FillRect(0,180,20,60,olc::BLACK);
                FillRect(236,180,20,60,olc::BLACK);
                bIsDrawing = false;               
            }
            fTick = 0.0f;
        }

        return true;
    }
};

int main()
{
    olcMad demo;
    if (demo.Construct(256, 240, 3, 3))
        demo.Start();
    return 0;
}