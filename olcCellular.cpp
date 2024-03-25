#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Cellular : public olc::PixelGameEngine
{
private:
    int width = 5;
    int y = 0;
    int rule = 145;
    std::array<int, 640/5> cells;

    int calculateState(int a, int b, int c)
    {
        int rotCount = a*4 + b*2 + c*1;
        int bitRule = rule;
        for (int i = rotCount; i > 0; i--)
        {
            bitRule = bitRule >> 1;
        }
        bitRule = bitRule & 0b00000001;
        return bitRule;
    }

public:
    Cellular()
    {
        // Name you application
        sAppName = "Cellular Automata";
    }

public:
    bool OnUserCreate() override
    {
        Clear(olc::WHITE);
        for (int i = 0; i < cells.size(); i++)
        {
            cells[i] = 0;
        }
        cells[cells.size()/2]=1;
        y = 0;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if (y < 320)
        {
            int i = 0;
            for (auto &cell : cells)
            {
                int x = i++ * width;
                FillRect(x, y, width, width, cell == 1 ? olc::BLACK : olc::WHITE);
            }
            std::array<int, 640/5> nextCells;
            int len = cells.size();
            for (int i = 0; i < len; i++)
            {
                nextCells[i] = calculateState(cells[(i - 1 + len) % len], cells[i], cells[(i + 1 + len) % len]);
            }
            cells = nextCells;
            y += width;
        }
        olc::vi2d textSize = GetTextSize("Rule: " + std::to_string(rule)) + olc::vi2d({4,4});
        DrawRect({28, 7}, textSize + olc::vi2d({1, 1}), olc::BLUE);
        FillRect({29, 8}, textSize);
        DrawString({30, 10}, "Rule: " + std::to_string(rule), olc::BLUE);
        if (GetKey(olc::Key::LEFT).bPressed)
        {
            if (rule > 0)
            {
                rule -= 1;
            }
            OnUserCreate();
        }
        if (GetKey(olc::Key::RIGHT).bPressed)
        {
            if (rule < 255)
            {
                rule += 1;
            }
            OnUserCreate();
        }

        return true;
    }
};

int main()
{
    Cellular demo;
    if (demo.Construct(640, 320, 2, 2))
        demo.Start();
    return 0;
}
