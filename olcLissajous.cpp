#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class olcLissajous : public olc::PixelGameEngine
{
public:
    olcLissajous()
    {
        // Name you application
        sAppName = "Lissajous Curve";
    }

private:
    float fAngle = 0.0f;
    int   nGridWidth = 100;
    float fDiameter;
    int   nCol = 8;
    float fPhase = 0.0f;
    const float PI = acos(-1);

    std::vector<olc::vf2d> vPoints;  //All points
    std::vector<olc::vf2d> cur_pts;  //Working points

    //string precision
    template <typename T>
    std::string to_string_with_precision(const T a_value, const int n = 6)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << a_value;
        return out.str();
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        nGridWidth = ScreenWidth() / (nCol + 1);
        fDiameter = (nGridWidth * 0.875f) / 2.0f;
        cur_pts.clear();
        vPoints.clear();
        for (int i = 0; i < nCol*nCol; i++)
        {
            cur_pts.emplace_back(olc::vf2d{ 0.0f, 0.0f });
        }
        fAngle = 0;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame, draws random coloured pixels
        if (GetKey(olc::Key::LEFT).bPressed) //Decrease column size
        {
            if (nCol > 1)
                nCol -= 1;
            OnUserCreate();
        }

        if (GetKey(olc::Key::RIGHT).bPressed) //Increase column size
        {
            if (nCol < 20)
                nCol += 1;
            OnUserCreate();
        }
        if (GetKey(olc::Key::DOWN).bPressed) //Decrease phase
        {
            if (fPhase > 0.0f)
                fPhase -= 0.25;
            OnUserCreate();
        }

        if (GetKey(olc::Key::UP).bPressed) //Increase phase
        {
            if (fPhase < 2.0f)
                fPhase += 0.25;
            OnUserCreate();
        }

        Clear(olc::BLACK);

        //Horizontal
        for (int i = 0; i < nCol; i++)
        {
            olc::vf2d circle = { nGridWidth * 1.5f + i * nGridWidth, nGridWidth / 2.0f };
            olc::vf2d point = { fDiameter, fAngle * (i + 1) };
            olc::vf2d circlepoint = circle + point.cart();
            DrawCircle(circle, fDiameter, olc::GREY);
            DrawLine( circlepoint.x, circlepoint.y , circlepoint.x, ScreenWidth(), olc::DARK_GREY );
            FillCircle(circlepoint, 2, olc::GREEN);
            //Save x points
            for (int x = 0; x < nCol; x++)
            {
                cur_pts[x + (i * nCol)].x = circlepoint.x;
            }
        }
        //Vertical
        for (int i = 0; i < nCol; i++)
        {
            olc::vf2d circle = { nGridWidth / 2.0f, nGridWidth * 1.5f + i * nGridWidth };
            olc::vf2d point = { fDiameter, fAngle * (i + 1) + (PI * fPhase) };
            olc::vf2d circlepoint = circle + point.cart();
            DrawCircle(circle, fDiameter, olc::GREY);
            DrawLine( circlepoint.x, circlepoint.y, ScreenHeight()-40, circlepoint.y, olc::DARK_GREY );
            FillCircle(circlepoint, 2, olc::GREEN);
            //Save y points
            for (int y = 0; y < nCol; y++)
            {
                cur_pts[(y * nCol) + i].y = circlepoint.y;
            }
        }
        //Draw x,y points
        for (auto &vPoint : vPoints)
        {
            Draw(vPoint, olc::RED);
        }
        //Store completed x,y points until they repeat
        for (int i = 0; i < nCol*nCol; i++)
        {
            FillCircle(cur_pts[i], 2, olc::WHITE);
            if (fAngle <= 2 * PI)
            {
                vPoints.emplace_back(cur_pts[i]);
            }
        }
        //Draw Menu
        FillRect({ 0, ScreenHeight() - 40 }, { ScreenWidth(), 40 }, olc::BLUE);
        DrawString({ 20, 810 }, "Use Arrows to change columns/phase  C:" + std::to_string(nCol) + " P:" + to_string_with_precision(fPhase,2), olc::WHITE, 2);
        
        //Update Angle
        fAngle += 0.5 * fElapsedTime;

        return true;
    }
};

int main()
{
    olcLissajous demo;
    if (demo.Construct(800, 840, 1, 1))
        demo.Start();
    return 0;
}
