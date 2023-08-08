#include <chrono>
#include <thread>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std::chrono_literals;

// Override base class with your custom functionality
class Maze : public olc::PixelGameEngine
{
public:
    Maze()
    {
        // Name you application
        sAppName = "Maze Generator";
    }

private:
    // const static int nRoomSize = 4;
    // const static int nWallSize = 1;
    // const static int nGridSizeY = (600 / (nRoomSize + nWallSize));  // 12
    // const static int nGridSizeX = (800 / (nRoomSize + nWallSize)); // 25
    const static int nRoomSize = 4;
    const static int nWallSize = 1;
    const static int nGridSizeY = (60 / (nRoomSize + nWallSize));  // 12
    const static int nGridSizeX = (125 / (nRoomSize + nWallSize)); // 25
    std::array<uint8_t, nGridSizeX * nGridSizeY> Grid;             // visited
    std::vector<olc::vi2d> Backtrack;                              // points visited
    olc::vi2d vCurr = {0, 0};

    // -> 25
    // W W W W W W W W W W  I
    // W s s s s W s s s s  V
    // W s s s s W s s s s  12
    // W s s s s W s s s s
    // W s s s s W s s s s

    // ---------------------------------------------------
    // |0| | | | | | | | | | | | | | | | | | | | | | | |24|
    // ---------------------------------------------------
    // |25| | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------
    // | | | | | | | | | | | | | | | | | | | | | | | | | |
    // ---------------------------------------------------

    void CarvePassage()
    {
        bool bValidPoint = false;
        uint8_t nWallLeft = 0x0F;
        olc::vi2d vNext;
        vNext = vCurr;
        Grid[vCurr.y * nGridSizeX + vCurr.x] &= 0x03;
        while (nWallLeft != 0 && bValidPoint == false)
        {
            int nDir = (int)rand() % 4;
            if (nDir == 0 && nWallLeft & 0x01) // North
            {
                nWallLeft ^= 0x01;
                if (vCurr.y - 1 >= 0 && Grid[(vCurr.y - 1) * nGridSizeX + vCurr.x] & 0x80)
                {
                    bValidPoint = true;
                    vNext.y -= 1;
                }
            }
            if (nDir == 1 && nWallLeft & 0x02) // East
            {
                nWallLeft ^= 0x02;
                if (vCurr.x + 1 < nGridSizeX && Grid[(vCurr.y) * nGridSizeX + vCurr.x + 1] & 0x80)
                {
                    bValidPoint = true;
                    vNext.x += 1;
                }
            }
            if (nDir == 2 && nWallLeft & 0x04) // South
            {
                nWallLeft ^= 0x04;
                if (vCurr.y + 1 < nGridSizeY && Grid[(vCurr.y + 1) * nGridSizeX + vCurr.x] & 0x80)
                {
                    bValidPoint = true;
                    vNext.y += 1;
                }
            }
            if (nDir == 3 && nWallLeft & 0x08) // West
            {
                nWallLeft ^= 0x08;
                if (vCurr.x - 1 >= 0 && Grid[(vCurr.y) * nGridSizeX + vCurr.x - 1] & 0x80)
                {
                    bValidPoint = true;
                    vNext.x -= 1;
                }
            }
        }
        if (bValidPoint)
        {
            int nDiff = vNext.x - vCurr.x;
            if (nDiff != 0)
            {
                if (nDiff < 0) // left
                    Grid[vCurr.y * nGridSizeX + vCurr.x] ^= 0x02;
                else // right
                    Grid[vNext.y * nGridSizeX + vNext.x] ^= 0x02;
            }
            nDiff = vNext.y - vCurr.y;
            if (nDiff != 0)
            {
                if (nDiff < 0) // up
                    Grid[vCurr.y * nGridSizeX + vCurr.x] ^= 0x01;
                else // down
                    Grid[vNext.y * nGridSizeX + vNext.x] ^= 0x01;
            }
            vCurr = vNext;
            Backtrack.push_back(vCurr);
        }
        else
        {
            Backtrack.pop_back();
            vCurr = Backtrack.back();
        }
    }

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        srand(time(NULL));
        for (int y = 0; y < nGridSizeY; y++)
            for (int x = 0; x < nGridSizeX; x++)
                Grid[y * nGridSizeX + x] = 0x83;
        Backtrack.push_back(vCurr);

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        std::this_thread::sleep_for(50ms);
        if (Backtrack.size() != 0)
            CarvePassage();

        if (GetKey(olc::Key::SPACE).bPressed)
        {
            for (int y = 0; y < nGridSizeY; y++)
                for (int x = 0; x < nGridSizeX; x++)
                    Grid[y * nGridSizeX + x] = 0x83;
            Backtrack.clear();
            vCurr = {rand() % nGridSizeX, rand() % nGridSizeY};
            Backtrack.push_back(vCurr);
        }

        Clear(olc::BLACK);
        // nGridSizeY = 12, nGridSizeX = 25
        // draw background blocks
        uint8_t walls;
        for (int y = 0; y < nGridSizeY; y++)
            for (int x = 0; x < nGridSizeX; x++)
            {
                walls = Grid[y * nGridSizeX + x];
                if ((walls & 0x80) == 0)
                    FillRect(x * 5, y * 5, 5, 5, olc::BLUE);
            }
        for (auto &cell : Backtrack)
        {
            FillRect(cell.x * 5, cell.y * 5, 5, 5, olc::RED);
        }

        if (Backtrack.size() != 0)
            FillRect(vCurr.x * 5 + 1, vCurr.y * 5 + 1, 4, 4, olc::GREEN);

        // draw walls
        for (int y = 0; y < nGridSizeY; y++)
            for (int x = 0; x < nGridSizeX; x++)
            {
                walls = Grid[y * nGridSizeX + x];
                if (walls & 0x01) // North
                    DrawLine(x * (5), y * (5), x * (5) + 5, y * (5));
                // if (walls & 0x02) // East
                //     DrawLine(x * (5) + 5, y * (5), x * (5) + 5, y * (5) + 5);
                // if (walls & 0x04) // South
                //     DrawLine(x * (5), y * (5) + 5, x * (5) + 5, y * (5) + 5);
                // if (walls & 0x08) // West
                //     DrawLine(x * (5), y * (5), x * (5), y * (5) + 5);
                if (walls & 0x02) // West
                    DrawLine(x * (5), y * (5), x * (5), y * (5) + 5);
            }
        DrawLine(0,60,125,60); //South border
        DrawLine(125,0,125,60); //East border

        DrawString(0,62,"SPACE = new maze");
        return true;
    }
};

int main()
{
    Maze demo;
    if (demo.Construct(130, 71, 8, 8))
        demo.Start();
    return 0;
}
