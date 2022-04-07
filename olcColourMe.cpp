#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

/**************************************

Colour ME

Pixel Drawing Game by Brian Chiha

Use the Mouse to select a colour, match the colour count on the right and bottom to
draw the colour in the correct grid.  Hold Mouse to draw continually.

*/
class olcColourMe : public olc::PixelGameEngine
{
public:
    olcColourMe()
    {
        // Name you application
        sAppName = "Colour Me Pixel Drawing";
    }

private:
    const static int nGridSizeX = 20;
    const static int nGridSizeY = 20;
    int nCellWidth = 10;
    olc::Pixel pActiveColour;  //Current colour
    olc::vi2d nPreviousCell;
    bool bAlreadySet = false;  //If colour set on key held.
    olc::vi2d GridSelectPos = { 285, 252 };
    olc::vi2d HeldCountPos = { 230, 300 };
    int nActivePicture = 1;  // Current Picture
    int nPictureCount = 4;   // Max Pictures
    olc::vi2d PicBtnPos = { 230, 350 };
    olc::vi2d ButtonSize = { (nCellWidth+1) * 2, (nCellWidth+1) * 2 };

    //Title
    std::pair<char , olc::Pixel> title[9] = { {'C', olc::RED}, {'O', olc::BLUE}, {'L', olc::GREEN}, {'O', olc::WHITE}, {'U', olc::YELLOW},
                                              {'R', olc::PURPLE}, {' ', olc::BLANK}, {'M', olc::PINK}, {'E', olc::CYAN} };

    int nColourCount = 1;

    std::array<olc::Pixel, nGridSizeX * nGridSizeY> Cells; 
    std::array<olc::Pixel, nGridSizeX * nGridSizeY> Picture;
    std::vector<olc::Pixel> vColours;

    //Picture Data
    std::vector<std::pair<olc::Pixel, int>> m_pic_data1 = {
        {olc::BLANK,20}, {olc::BLANK,20}, {olc::BLANK, 7}, {olc::RED,   5}, {olc::BLANK, 8}, {olc::BLANK, 6},
        {olc::RED,   9}, {olc::BLANK, 5}, {olc::BLANK, 6}, {olc::BLACK, 3}, {olc::PINK,  2}, {olc::BLACK, 1}, 
        {olc::PINK,  1}, {olc::BLANK, 7}, {olc::BLANK, 5}, {olc::BLACK, 1}, {olc::PINK,  1}, {olc::BLACK, 1},
        {olc::PINK,  3}, {olc::BLACK, 1}, {olc::PINK,  3}, {olc::BLANK, 5}, {olc::BLANK, 5}, {olc::BLACK, 1},
        {olc::PINK,  1}, {olc::BLACK, 2}, {olc::PINK,  3}, {olc::BLACK, 1}, {olc::PINK,  3}, {olc::BLANK, 4},
        {olc::BLANK, 5}, {olc::BLACK, 2}, {olc::PINK,  4}, {olc::BLACK, 4}, {olc::BLANK, 5}, {olc::BLANK, 7}, 
        {olc::PINK,  7}, {olc::BLANK, 6}, {olc::BLANK, 6}, {olc::BLUE,  2}, {olc::RED,   1}, {olc::BLUE,  4},
        {olc::BLANK, 7}, {olc::BLANK, 5}, {olc::BLUE,  3}, {olc::RED,   1}, {olc::BLUE,  2}, {olc::RED,   1},
        {olc::BLUE,  3}, {olc::BLANK, 5}, {olc::BLANK, 4}, {olc::BLUE,  4}, {olc::RED,   1}, {olc::BLUE,  2},
        {olc::RED,   1}, {olc::BLUE,  4}, {olc::BLANK, 4}, {olc::BLANK, 4}, {olc::PINK,  2}, {olc::BLUE,  1},
        {olc::RED,   1}, {olc::YELLOW,1}, {olc::RED,   2}, {olc::YELLOW,1}, {olc::RED,   1}, {olc::BLUE,  1},
        {olc::PINK,  2}, {olc::BLANK, 4}, {olc::BLANK, 4}, {olc::PINK,  3}, {olc::RED,   6}, {olc::PINK,  3},
        {olc::BLANK, 4}, {olc::BLANK, 4}, {olc::PINK,  2}, {olc::RED,   8}, {olc::PINK,  2}, {olc::BLANK, 4},
        {olc::BLANK, 6}, {olc::RED,   3}, {olc::BLANK, 2}, {olc::RED,   3}, {olc::BLANK, 6}, {olc::BLANK, 5},
        {olc::BROWN, 3}, {olc::BLANK, 4}, {olc::BROWN, 3}, {olc::BLANK, 5}, {olc::BLANK, 4}, {olc::BROWN, 4},
        {olc::BLANK, 4}, {olc::BROWN, 4}, {olc::BLANK, 4}, {olc::BLANK,20}, {olc::BLANK,20}
    };

    std::vector<std::pair<olc::Pixel, int>> m_pic_data2 = {
        {olc::BLANK, 1}, {olc::YELLOW,2}, {olc::BLANK, 2}, {olc::BLACK, 3}, {olc::BLANK,12}, {olc::YELLOW,1},
        {olc::RED,   1}, {olc::YELLOW,2}, {olc::BLANK, 2}, {olc::BLACK, 4}, {olc::BLANK, 5}, {olc::YELLOW,2},
        {olc::BLANK, 3}, {olc::YELLOW,4}, {olc::BLANK, 3}, {olc::BLACK, 3}, {olc::BLANK, 3}, {olc::BLACK, 1},
        {olc::YELLOW,2}, {olc::RED,   1}, {olc::YELLOW,1}, {olc::BLANK, 2}, {olc::BLANK, 1}, {olc::YELLOW,1},
        {olc::RED,   1}, {olc::BLACK, 3}, {olc::BLANK, 1}, {olc::BLACK, 4}, {olc::BLANK, 2}, {olc::BLACK, 1},
        {olc::RED,   1}, {olc::YELLOW,3}, {olc::BLANK, 2}, {olc::BLANK, 1}, {olc::BLACK, 7}, {olc::WHITE, 1},
        {olc::BLACK, 4}, {olc::BLANK, 2}, {olc::YELLOW,1}, {olc::RED,   1}, {olc::BLANK, 3}, {olc::BLANK, 3},
        {olc::BLACK, 6}, {olc::WHITE, 1}, {olc::BLACK, 4}, {olc::BLANK, 6}, {olc::BLANK, 5}, {olc::BLACK, 5},
        {olc::WHITE, 1}, {olc::BLACK, 4}, {olc::BLANK, 2}, {olc::BLACK, 2}, {olc::BLANK, 1}, {olc::BLANK, 3},
        {olc::BLACK, 6}, {olc::WHITE, 1}, {olc::BLACK, 7}, {olc::BLANK, 3}, {olc::BLACK, 7}, {olc::WHITE, 1},
        {olc::BLACK, 1}, {olc::WHITE, 2}, {olc::BLACK, 1}, {olc::WHITE, 1}, {olc::BLACK, 4}, {olc::BLANK, 3},
        {olc::BLANK, 4}, {olc::BLACK, 3}, {olc::WHITE, 5}, {olc::BLACK, 8}, {olc::BLANK, 3}, {olc::BLACK, 2},
        {olc::WHITE, 2}, {olc::BLANK, 1}, {olc::BLACK, 1}, {olc::WHITE, 1}, {olc::BLACK, 1}, {olc::BLANK, 1},
        {olc::WHITE, 2}, {olc::BLACK, 2}, {olc::BLANK, 4}, {olc::BLANK, 1}, {olc::YELLOW,1}, {olc::RED,   1},
        {olc::BLANK, 2}, {olc::BLACK, 1}, {olc::WHITE, 7}, {olc::BLACK, 2}, {olc::BLANK, 5}, {olc::RED,   1},
        {olc::YELLOW,3}, {olc::BLANK, 3}, {olc::WHITE, 2}, {olc::BLACK, 1}, {olc::WHITE, 2}, {olc::BLANK, 8},
        {olc::YELLOW,3}, {olc::RED,   1}, {olc::BLANK, 1}, {olc::ORANGE,3}, {olc::WHITE, 3}, {olc::ORANGE,3},
        {olc::BLANK, 3}, {olc::YELLOW,2}, {olc::BLANK, 1}, {olc::BLANK, 1}, {olc::RED,   1}, {olc::YELLOW,1},
        {olc::BLANK, 1}, {olc::BLUE,  2}, {olc::ORANGE,2}, {olc::BLUE,  3}, {olc::ORANGE,2}, {olc::BLUE,  2},
        {olc::BLANK, 1}, {olc::YELLOW,2}, {olc::RED,   1}, {olc::YELLOW,1}, {olc::BLANK, 4}, {olc::WHITE, 2},
        {olc::ORANGE,3}, {olc::BLUE,  1}, {olc::ORANGE,3}, {olc::WHITE, 2}, {olc::BLANK, 1}, {olc::YELLOW,4},
        {olc::BLANK, 3}, {olc::WHITE, 3}, {olc::ORANGE,7}, {olc::WHITE, 3}, {olc::BLANK, 1}, {olc::YELLOW,2},
        {olc::BLANK, 1}, {olc::BLANK, 3}, {olc::BLUE,  2}, {olc::BLANK, 1}, {olc::BLUE,  7}, {olc::BLANK, 1},
        {olc::BLUE,  2}, {olc::BLANK, 4}, {olc::BLANK, 3}, {olc::WHITE, 2}, {olc::BLANK, 1}, {olc::ORANGE,2},
        {olc::BLUE,  1}, {olc::ORANGE,1}, {olc::BLUE,  1}, {olc::ORANGE,2}, {olc::BLANK, 1}, {olc::WHITE, 2},
        {olc::BLANK, 4}, {olc::BLANK, 3}, {olc::WHITE, 2}, {olc::ORANGE,3}, {olc::BLUE,  1}, {olc::ORANGE,2},
        {olc::BLUE,  2}, {olc::ORANGE,1}, {olc::WHITE, 2}, {olc::BLANK, 4}
    };

    std::vector<std::pair<olc::Pixel, int>> m_pic_data3 = {
        {olc::BLANK, 8}, {olc::BLACK, 9}, {olc::BLANK, 3}, {olc::BLANK, 7}, {olc::BLACK, 2}, {olc::BROWN, 7}, 
        {olc::BLACK, 1}, {olc::BLANK, 3}, {olc::BLANK, 6}, {olc::BLACK, 2}, {olc::BROWN, 9}, {olc::BLACK, 1},
        {olc::BLANK, 2}, {olc::BLANK, 6}, {olc::BLACK, 1}, {olc::BROWN, 4}, {olc::WHITE, 5}, {olc::BROWN, 1},
        {olc::BLACK, 1}, {olc::BLANK, 2}, {olc::BLANK, 5}, {olc::BLACK, 2}, {olc::BROWN, 3}, {olc::WHITE, 2},
        {olc::BLACK, 1}, {olc::WHITE, 1}, {olc::BLACK, 1}, {olc::WHITE, 1}, {olc::BLACK, 2}, {olc::BLANK, 2},
        {olc::BLANK, 5}, {olc::BLACK, 1}, {olc::BROWN, 4}, {olc::WHITE, 2}, {olc::BLACK, 1}, {olc::WHITE, 1},
        {olc::BLACK, 1}, {olc::WHITE, 1}, {olc::BLACK, 1}, {olc::BLANK, 3}, {olc::BLANK, 5}, {olc::BLACK, 1},
        {olc::BROWN, 3}, {olc::WHITE, 7}, {olc::BLACK, 1}, {olc::BLANK, 3}, {olc::BLANK, 6}, {olc::BLACK, 2},
        {olc::BROWN, 1}, {olc::WHITE, 1}, {olc::BLACK, 5}, {olc::WHITE, 1}, {olc::BLACK, 1}, {olc::BLANK, 3},
        {olc::BLANK, 6}, {olc::RED,   1}, {olc::BLACK, 2}, {olc::WHITE, 2}, {olc::BLACK, 4}, {olc::WHITE, 1},
        {olc::BLACK, 1}, {olc::BLANK, 3}, {olc::BLANK, 5}, {olc::RED,   1}, {olc::BLACK, 4}, {olc::WHITE, 5},
        {olc::BLACK, 3}, {olc::BLANK, 2}, {olc::BLANK, 4}, {olc::RED,   1}, {olc::BLACK, 2}, {olc::BLUE,  2},
        {olc::BLACK, 7}, {olc::BLUE,  1}, {olc::BLACK, 1}, {olc::BLANK, 2}, {olc::BLANK, 4}, {olc::RED,   1},
        {olc::BLACK, 1}, {olc::BLUE,  2}, {olc::BLACK, 2}, {olc::BLUE,  1}, {olc::WHITE, 2}, {olc::BLUE,  1},
        {olc::BLACK, 1}, {olc::BLUE,  2}, {olc::BLACK, 2}, {olc::BLANK, 1}, {olc::BLANK, 3}, {olc::RED,   1},
        {olc::BLACK, 1}, {olc::BLUE,  2}, {olc::BLACK, 2}, {olc::BLUE,  5}, {olc::BLACK, 1}, {olc::BLUE,  1},
        {olc::WHITE, 2}, {olc::BLACK, 1}, {olc::BLANK, 1}, {olc::BLANK, 2}, {olc::RED,   2}, {olc::BLACK, 1},
        {olc::WHITE, 2}, {olc::BLACK, 1}, {olc::BLUE,  6}, {olc::BLACK, 1}, {olc::WHITE, 2}, {olc::BLACK, 2},
        {olc::BLANK, 1}, {olc::BLANK, 1}, {olc::RED,   2}, {olc::BLACK, 1}, {olc::WHITE, 3}, {olc::BLACK, 3},
        {olc::BROWN, 2}, {olc::BLACK, 6}, {olc::BLANK, 2}, {olc::BLANK, 1}, {olc::RED,   2}, {olc::BLACK, 1},
        {olc::WHITE, 2}, {olc::BLACK, 2}, {olc::CYAN,  6}, {olc::BLACK, 1}, {olc::BLANK, 5}, {olc::BLANK, 1},
        {olc::RED,   2}, {olc::BLACK, 3}, {olc::RED,   1}, {olc::BLACK, 2}, {olc::CYAN,  4}, {olc::BLACK, 2},
        {olc::BLANK, 5}, {olc::BLANK, 2}, {olc::RED,   6}, {olc::BLACK, 6}, {olc::BLANK, 6}, {olc::BLANK, 5},
        {olc::BLACK, 4}, {olc::BLANK, 4}, {olc::BLACK, 4}, {olc::BLANK, 3}, {olc::BLANK, 4}, {olc::BLACK, 6},
        {olc::BLANK, 2}, {olc::BLACK, 6}, {olc::BLANK, 2}
    };

    std::vector<std::pair<olc::Pixel, int>> m_pic_data4 = {
        {olc::BLANK,16}, {olc::PURPLE,4}, {olc::BLANK,15}, {olc::PURPLE,1}, {olc::PINK,  2}, {olc::PURPLE,1},
        {olc::BLANK, 1}, {olc::BLANK,14}, {olc::PURPLE,1}, {olc::PINK,  1}, {olc::PURPLE,2}, {olc::BLANK, 2},
        {olc::BLANK,13}, {olc::PURPLE,3}, {olc::BLANK, 4}, {olc::BLANK, 9}, {olc::PURPLE,5}, {olc::BLANK, 1},
        {olc::PURPLE,5}, {olc::BLANK, 8}, {olc::PURPLE,8}, {olc::PINK,  3}, {olc::BLANK, 1}, {olc::BLANK, 7},
        {olc::PURPLE,7}, {olc::BLANK, 6}, {olc::BLANK, 6}, {olc::PURPLE,2}, {olc::BLANK, 2}, {olc::PURPLE,5},
        {olc::BLANK, 5}, {olc::BLANK, 5}, {olc::PINK,  1}, {olc::PURPLE,2}, {olc::BLACK, 1}, {olc::BLANK, 1},
        {olc::PURPLE,5}, {olc::BLANK, 5}, {olc::BLANK, 6}, {olc::PURPLE,3}, {olc::PINK,  2}, {olc::PURPLE,4},
        {olc::BLANK, 5}, {olc::BLANK, 1}, {olc::GREEN, 2}, {olc::BLANK, 4}, {olc::PURPLE,7}, {olc::BLANK, 6},
        {olc::GREEN, 2}, {olc::BLANK, 1}, {olc::GREEN ,1}, {olc::BLANK, 4}, {olc::PURPLE,5}, {olc::BLANK, 7},
        {olc::BLANK, 2}, {olc::GREEN, 4}, {olc::ORANGE,1}, {olc::BLANK, 3}, {olc::PURPLE,4}, {olc::BLANK, 6},
        {olc::BLANK, 1}, {olc::GREEN, 2}, {olc::BLANK, 1}, {olc::GREEN, 1}, {olc::ORANGE,3}, {olc::BLANK, 1},
        {olc::PURPLE,2}, {olc::PINK,  2}, {olc::PURPLE,2}, {olc::BLANK, 5}, {olc::BLANK, 4}, {olc::ORANGE,3},
        {olc::PURPLE,4}, {olc::PINK,  1}, {olc::PURPLE,4}, {olc::BLANK, 4}, {olc::BLANK, 5}, {olc::ORANGE,4},
        {olc::BLANK, 1}, {olc::PINK,  2}, {olc::PURPLE,5}, {olc::BLANK, 3}, {olc::BLANK, 6}, {olc::ORANGE,4},
        {olc::BLANK, 1}, {olc::PINK,  1}, {olc::PURPLE,5}, {olc::PINK,  2}, {olc::BLANK, 1}, {olc::BLANK, 8},
        {olc::ORANGE,2}, {olc::PURPLE,7}, {olc::PINK,  2}, {olc::BLANK, 1}, {olc::BLANK, 8}, {olc::PURPLE,9},
        {olc::BLANK, 3}, {olc::BLANK, 9}, {olc::PURPLE,7}, {olc::BLANK, 4}
    };

    std::vector<std::pair<olc::Pixel, int>> m_pictures[4] = {m_pic_data1, m_pic_data2, m_pic_data3, m_pic_data4};
    
    // Update cell colour
    void SetActiveColour(olc::vi2d active_cell)
    {
        if (Cells[active_cell.y * nGridSizeX + active_cell.x] == olc::BLANK)
        {
            Cells[active_cell.y * nGridSizeX + active_cell.x] = pActiveColour;
        } 
        else
        {
            Cells[active_cell.y * nGridSizeX + active_cell.x] = olc::BLANK;
        }
    }

    // Calculate number colour to be either White or Black depending on the intensity of the colour
    olc::Pixel GetViewableColour(olc::Pixel col)
    {
        if (col.r + col.g + col.b < (765 / 2))
            return olc::WHITE;
        else
            return olc::BLACK;   
    }

    // Draw Active Picture in Cells array
    void FillPicture(int active_picture)
    {
        //fill picture with pixel colour data
        vColours.clear();
        int index = 0;
        for (const auto& p : m_pictures[active_picture-1])
        {
            int count = p.second;
            while (count != 0)
            {
                Picture[index++] = p.first;
                count--;
            }
            vColours.emplace_back(p.first);
        }
        //remove duplicate colours from colour selection list
        std::sort(vColours.begin(), vColours.end(), [](olc::Pixel a, olc::Pixel b) {
            return  a.n > b.n;
        });
        auto last = std::unique(vColours.begin(), vColours.end());
        vColours.erase(last, vColours.end());
    }

public:
    bool OnUserCreate() override
    {
        //Clear screen
        Clear(olc::BLACK);
        //Default Cells to BLANK
        for (int y = 0; y < nGridSizeY; y++)
        {
            for (int x = 0; x < nGridSizeX; x++)
            {
                Cells[y * nGridSizeX + x] = olc::BLANK;
            }
        }
        //Load Picture
        FillPicture(nActivePicture);

        //Draw Number Clues Across
        int nRows = 0;
        for (int y = nCellWidth; y < (nGridSizeY * (nCellWidth+1)); y += nCellWidth+1)
        {
            int nXIndex = 0;
            int nXStart = nCellWidth + ((nCellWidth+1) * nGridSizeX);
            int nCount = 0;
            olc::Pixel col, nextCol;

            while(nXIndex < nGridSizeX)
            {
                col = Picture[nXIndex + (nRows * nGridSizeX)];
                nextCol = col;
                //look ahead
                while(col == nextCol && nXIndex < nGridSizeX)
                {
                    nCount++;
                    nXIndex++;
                    nextCol = Picture[nXIndex + (nRows * nGridSizeX)];
                }
                if (col != olc::BLANK)  //Ignore blanks
                {
                    DrawRect(nXStart, y, nCellWidth+1, nCellWidth+1);
                    FillRect(nXStart+1, y+1, nCellWidth, nCellWidth, col);
                    DrawString(nXStart+2, y+2, std::to_string(nCount), GetViewableColour(col));
                    nXStart += nCellWidth+1;
                }
                nCount = 0;
            }
            nRows++;
        }        

        //Draw Number Clues Down
        int nCols = 0;
        for (int x = nCellWidth; x < (nGridSizeX * (nCellWidth+1)); x += nCellWidth+1)
        {
            int nYIndex = 0;
            int nYStart = nCellWidth + ((nCellWidth+1) * nGridSizeY);
            int nCount = 0;
            olc::Pixel col, nextCol;
            while(nYIndex < nGridSizeY)
            {
                col = Picture[nCols + (nYIndex * nGridSizeX)];
                nextCol = col;
                //look ahead
                while(col == nextCol && nYIndex < nGridSizeY)
                {
                    nCount++;
                    nYIndex++;
                    nextCol = Picture[nCols + (nYIndex * nGridSizeX)];
                }
                if (col != olc::BLANK)  //Ignore blanks
                {
                    DrawRect(x, nYStart, nCellWidth+1, nCellWidth+1);
                    FillRect(x+1, nYStart+1, nCellWidth, nCellWidth, col);
                    DrawString(x+2, nYStart+2, std::to_string(nCount), GetViewableColour(col));
                    nYStart += nCellWidth+1;
                }
                nCount = 0;
            }
            nCols++;
        }        

        //Draw Repeat Counter box
        DrawRect(HeldCountPos, {42, 25}, olc::GREY);
        DrawString({HeldCountPos.x+2, HeldCountPos.y+4}, "Count");

        //Draw Picture Selection Buttons
        for (int i = 0; i < nPictureCount; i++)
        {
            FillRect({PicBtnPos.x + (i * (ButtonSize.x + nCellWidth+1)), PicBtnPos.y}, ButtonSize, olc::BLUE);
            olc::vi2d text_size = GetTextSize(std::to_string(i+1));
            DrawString({PicBtnPos.x + (i * (ButtonSize.x + nCellWidth+1)) + (ButtonSize.x - text_size.x) / 2 , PicBtnPos.y + (ButtonSize.y - text_size.y) / 2}, std::to_string(i+1), olc::WHITE);
        }
        DrawString(PicBtnPos.x + 5, PicBtnPos.y + ButtonSize.y + 5, "Choose Picture");
        
        //Draw Title
        olc::vi2d TitlePos = { 380, 245 };
        for (int i = 0; i < 9; i++)
        {
            DrawString(TitlePos.x, TitlePos.y + (10*i), std::string(1,title[i].first),title[i].second);
        }
        //Set Active Colour
        pActiveColour = vColours[0];

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        //draw current cell colours
        for (int y = 0; y < nGridSizeY; y++)
        {
            for (int x = 0; x < nGridSizeX; x++)
            {
                //Draw Blank Grid
                DrawLine(nCellWidth, nCellWidth + (y * (nCellWidth+1)), (nGridSizeX+1) * (nCellWidth+1)-1, nCellWidth + (y * (nCellWidth+1)), olc::DARK_GREY);
                DrawLine(nCellWidth + (x * (nCellWidth+1)), nCellWidth, nCellWidth + (x * (nCellWidth+1)), (nGridSizeY+1) * (nCellWidth+1)-1, olc::DARK_GREY);
                //Draw Cells
                FillRect(nCellWidth+1 + (x * (nCellWidth+1)), nCellWidth+1 + (y * (nCellWidth+1)), nCellWidth, nCellWidth, (Cells[y * nGridSizeX + x] == olc::BLANK ? olc::GREY : Cells[y * nGridSizeX + x]));
            }
        }

        //Draw Active Picture Button Highlight
        for (int i = 0; i < nPictureCount; i++)
        {
            DrawRect({PicBtnPos.x + (i * (ButtonSize.x + nCellWidth+1)), PicBtnPos.y}, ButtonSize, (i+1 == nActivePicture ? olc::RED : olc::BLUE));
        }

        //get mouse position in cell size
        olc::vi2d nSelectedCell = { GetMouseX() / (nCellWidth+1) - 1, GetMouseY() / (nCellWidth+1) - 1 };
        //update cell on mouse click with current selected colour or invert
        olc::vi2d text_size = GetTextSize(std::to_string(nColourCount));
        if (GetMouse(0).bPressed || (GetMouse(0).bHeld && !bAlreadySet))
        {
            //Mouse Over Grid
            if (nSelectedCell.x >= 0 && nSelectedCell.x < nGridSizeX && nSelectedCell.y >= 0 && nSelectedCell.y < nGridSizeY)
            {
                SetActiveColour(nSelectedCell);
                //Display Cell Count
                FillRect({HeldCountPos.x+21 - (text_size.x / 2), HeldCountPos.y+15}, text_size, olc::BLACK);
                DrawString(HeldCountPos.x+21 - (text_size.x / 2), HeldCountPos.y+15, std::to_string(nColourCount), olc::RED);
            }
            //Mouse Over Colour Selection
            if (nSelectedCell.x == 25 && nSelectedCell.y >= 22 && nSelectedCell.y < 22 + vColours.size())
            {
                pActiveColour = vColours[nSelectedCell.y - 22];
            }
            //Mouse Over Picture Selection
            if (nSelectedCell.y > 30 && nSelectedCell.y <= 32)
            {
                if (nSelectedCell.x >= 20 && nSelectedCell.x <= 21)
                {
                    nActivePicture = 1;
                    OnUserCreate();
                }
                if (nSelectedCell.x >= 23 && nSelectedCell.x <= 24)
                {
                    nActivePicture = 2;
                    OnUserCreate();
                }
                if (nSelectedCell.x >= 26 && nSelectedCell.x <= 27)
                {
                    nActivePicture = 3;
                    OnUserCreate();
                }
                if (nSelectedCell.x >= 29 && nSelectedCell.x <= 30)
                {
                    nActivePicture = 4;
                    OnUserCreate();
                }
            }
        }
        if (GetMouse(0).bReleased)
        {
            nColourCount = 1;
            FillRect({HeldCountPos.x+21 - (text_size.x / 2), HeldCountPos.y+15}, text_size, olc::BLACK);
        }

        //Draw New Edge Lines
        DrawLine(nCellWidth, nCellWidth + (nGridSizeY * (nCellWidth+1)), (nGridSizeX+1) * (nCellWidth+1)-1, nCellWidth + (nGridSizeY * (nCellWidth+1)), olc::DARK_GREY);
        DrawLine(nCellWidth + (nGridSizeX * (nCellWidth+1)), nCellWidth, nCellWidth + (nGridSizeX * (nCellWidth+1)), (nGridSizeY+1) * (nCellWidth+1)-1, olc::DARK_GREY);
        //Create mouse hover highlights
        if (nSelectedCell.x >= 0 && nSelectedCell.x < nGridSizeX && nSelectedCell.y >= 0 && nSelectedCell.y < nGridSizeY )
        {
            //Draw Cell Outline
            DrawRect(nCellWidth+1 + (nSelectedCell.x * (nCellWidth+1)), nCellWidth+1 + (nSelectedCell.y * (nCellWidth+1)), nCellWidth-1, nCellWidth-1, GetViewableColour(Cells[nSelectedCell.y * nGridSizeX + nSelectedCell.x]));
            //Draw Highlight for selected cell
            DrawLine(nCellWidth + (nSelectedCell.x * (nCellWidth+1)), nCellWidth + (nGridSizeY * (nCellWidth+1)), (nCellWidth*2) + (nSelectedCell.x * (nCellWidth+1)), nCellWidth + (nGridSizeY * (nCellWidth+1)), olc::RED);
            DrawLine(nCellWidth + (nGridSizeX * (nCellWidth+1)), nCellWidth+1 + (nSelectedCell.y * (nCellWidth+1)), nCellWidth + (nGridSizeX * (nCellWidth+1)), (nCellWidth*2) + (nSelectedCell.y * (nCellWidth+1)), olc::RED);
        }

        //update AlreadySet to handle Auto Key Colour setting
        if (nSelectedCell != nPreviousCell)
        {
            bAlreadySet = false;
            if (GetMouse(0).bHeld)
                nColourCount++;
        } 
        else
        {
            bAlreadySet = true;
        }
        nPreviousCell = nSelectedCell;

        //Draw Colour Selection
        int nYPos = GridSelectPos.y;
        int nYSelectPos = 0;
        FillRect(GridSelectPos.x - 10, nYPos - 10, 90, 25 + (vColours.size() * nCellWidth), olc::GREY);
        FillRect(GridSelectPos.x - 5, nYPos - 5, 80, 15 + (vColours.size() * nCellWidth), olc::BLACK);

        //Draw Cell Selection retangle
        for (auto &col :vColours)
        {
            DrawRect(GridSelectPos.x, nYPos, nCellWidth+1, nCellWidth+1);
            FillRect(GridSelectPos.x + 1, nYPos + 1, nCellWidth, nCellWidth, col);
            if (col == olc::BLANK)
            {
                DrawString(GridSelectPos.x + 3, nYPos + 3, "X", olc::RED);
            }
            if (col == pActiveColour)
            {
                nYSelectPos = nYPos;
            }
            nYPos += nCellWidth+1;
        }
        //Expand Active Colour
        FillRect(GridSelectPos.x - 2, nYSelectPos - 2, nCellWidth+5, nCellWidth+5, pActiveColour);
        DrawRect(GridSelectPos.x - 2, nYSelectPos - 2, nCellWidth+5, nCellWidth+5);
        if (pActiveColour == olc::BLANK)
        {
            DrawString(GridSelectPos.x + 3, nYSelectPos + 3, "X", olc::RED);
        }
        DrawString({305,255}, "Colour\n\nList");

        return true;
    }
};

int main()
{
    olcColourMe game;
    if (game.Construct(400, 400, 2, 2))
        game.Start();
    return 0;
}
