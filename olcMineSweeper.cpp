#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


//Cell data, 
struct sCell
{
    bool bVisible = false;  //is it visible
    bool bHasMine = false;  //does it have a mine
    bool bFlagged = false;  //has it been flagged
    int nMineCount = 0;     //Neighbour mine count
};

// Override base class with your custom functionality
class olcMineSweeper : public olc::PixelGameEngine
{
public:
    olcMineSweeper()
    {
        // Name you application
        sAppName = "Mine Sweeper";
    }

private:
    sCell *cells = nullptr;  //cells array
    int nCells = 10;         //number of X Y cells
    int nCellWidth = 9;      //cell width for drawing
    int nMines = 0;          //total mines on board
    int nNonVisibleCells = nCells * nCells; //intial number of non visible cells
    int bGameOver = false;   //is game over state
    // colours for cell neighbours count
    olc::Pixel countColours[9] = {olc::BLANK, olc::BLUE, olc::DARK_GREEN, olc::RED, olc::DARK_BLUE,
                                  olc::MAGENTA, olc::DARK_CYAN, olc::BLACK, olc::GREY};

    //given a cell, return its contents as a string for display
    std::string cellContents(int nCell)
    {
        sCell cell = cells[nCell];
        if (cell.bHasMine)
            return "#";
        else if (cell.nMineCount != 0)
            return std::to_string(cell.nMineCount);
        else
            return " ";
    }

    //return the cell colour for the mine count
    olc::Pixel cellColour(int nCell)
    {
        sCell cell = cells[nCell];
        if (cell.bHasMine)
            return olc::WHITE;
        return countColours[cell.nMineCount];
    }

    //return the cell colour if not visible and marked
    olc::Pixel cellMarked(int nCell)
    {
        sCell cell = cells[nCell];
        if (cell.bFlagged)
            return olc::RED;
        return olc::GREY;
    }

    //Recursively Reveal cells look up, down, left and right
    void floodReveal(int x, int y)
    {
        //cant be bomb if here because entry is by a 0 non bomb and 
        //bombs are surrounder by non zero Mine Counts
        if (cells[y * nCells + x].bVisible)
            return;

        cells[y * nCells + x].bVisible = true;
        nNonVisibleCells--;

        if (cells[y * nCells + x].nMineCount != 0)
            return;
        // look up, down, left and right
        if (x > 0) floodReveal(x - 1, y); // left
        if (x < nCells - 1) floodReveal(x + 1, y); // right
        if (y > 0) floodReveal(x, y - 1); // up
        if (y < nCells - 1) floodReveal(x, y + 1); // down
    }

public:
    bool OnUserCreate() override
    {
        int nTotalCells = nCells * nCells;
        srand (time(NULL));

        // Create the cell nodes
        if (cells != nullptr)
            delete cells;
        cells = new sCell[nTotalCells];

        nNonVisibleCells = nCells * nCells;
        // Populate with Mines.  1 Bomb for every 10 cells, if same cell then use less
        nMines = 0;
        for (int i = 0; i < (nTotalCells / 10); i++)
        {
            int nMineCell = rand() % nTotalCells;
            if (!cells[nMineCell].bHasMine)
                nMines++;
            cells[nMineCell].bHasMine = true;
        }

        // Calculate surrounding mine count for each cell
        for (int x = 0; x < nCells; x++)
            for (int y = 0; y < nCells; y++)
            {
                // if cell has a mine, increase Mine count around it
                if (cells[y * nCells + x].bHasMine)
                {
                    if (x > 0 && y > 0) cells[(y - 1) * nCells + (x - 1)].nMineCount++;  //top left
                    if (x > 0 && y < nCells - 1) cells[(y + 1) * nCells + (x - 1)].nMineCount++;  //bottom left
                    if (x < nCells - 1 && y > 0) cells[(y - 1) * nCells + (x + 1)].nMineCount++;  //top right
                    if (x < nCells - 1 && y < nCells - 1) cells[(y + 1) * nCells + (x + 1)].nMineCount++;  //bottom right         

                    if (x > 0) cells[y * nCells + (x - 1)].nMineCount++; // middle left
                    if (x < nCells - 1) cells[y * nCells + (x + 1)].nMineCount++; // middle right

                    if (y > 0) cells[(y - 1) * nCells + x].nMineCount++;  //top
                    if (y < nCells - 1) cells[(y + 1) * nCells + x].nMineCount++;  //bottom
                }
            }

        //Clear screen
        Clear(olc::BLACK);
        //Draw blank grid
        for (int x = 0; x <= nCells; x++)
        {
            DrawLine(nCellWidth, nCellWidth + (x * (nCellWidth+1)), (nCells+1) * (nCellWidth+1)-1, nCellWidth + (x * (nCellWidth+1)));
            DrawLine(nCellWidth + (x * (nCellWidth+1)), nCellWidth, nCellWidth + (x * (nCellWidth+1)), (nCells+1) * (nCellWidth+1)-1);
        }
 
        //Draw Total Mines to find
        DrawString(10,180,"Total Mines to find: " + std::to_string(nMines));
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        //get mouse position in cell size
        int nSelectedCellX = GetMouseX() / (nCellWidth+1) - 1;
        int nSelectedCellY = GetMouseY() / (nCellWidth+1) - 1;

        //if mouse pressed, show the cell if not already visible
        if (GetMouse(0).bReleased)
        {
            if (bGameOver)  //if in game over state, restart game
            {
                bGameOver = false;
                OnUserCreate();
            }

            // if mouse is within a valid cell, reveal it if not already visible.
            if ((nSelectedCellX >= 0 && nSelectedCellX < nCells) && (nSelectedCellY >= 0 && nSelectedCellY < nCells))
                if (!cells[nSelectedCellY * nCells + nSelectedCellX].bVisible)
                {
                    ///Reveal celected cell
                    // If cell is Mine, reveal all and end of game
                    if (cells[nSelectedCellY * nCells + nSelectedCellX].bHasMine)
                    {
                        for (int x = 0; x < nCells; x++)
                            for (int y = 0; y < nCells; y++)
                                cells[y * nCells + x].bVisible = true;
                        //game over
                        bGameOver = true;
                        DrawString(75,160,"You Lose!",olc::RED);
                    }
                    // if cell is not a bomb and has no bombs near, recursively flood fill around it
                    else if (cells[nSelectedCellY * nCells + nSelectedCellX].nMineCount == 0)
                        floodReveal(nSelectedCellX, nSelectedCellY);
                    else //must be a non bomb but with neighbours, just reveal the cell
                    {
                        cells[nSelectedCellY * nCells + nSelectedCellX].bVisible = true;
                        nNonVisibleCells--;
                    }
                }
        }

        //if right mouse button pressed, flag or unflag cell
        if (GetMouse(1).bReleased)
        {
            if ((nSelectedCellX >= 0 && nSelectedCellX < nCells) && (nSelectedCellY >= 0 && nSelectedCellY < nCells))
                if (!cells[nSelectedCellY * nCells + nSelectedCellX].bVisible)
                {
                    cells[nSelectedCellY * nCells + nSelectedCellX].bFlagged = !cells[nSelectedCellY * nCells + nSelectedCellX].bFlagged;
                }
        }


        //draw all the visible cells
        for (int x = 0; x < nCells; x++)
            for (int y = 0; y < nCells; y++)
                if (cells[y * nCells + x].bVisible)
                {
                    FillRect((nCellWidth+1)*(x+1), (nCellWidth+1)*(y+1), nCellWidth, nCellWidth, olc::BLACK);
                    DrawString((nCellWidth+1)*(x+1)+1, (nCellWidth+1)*(y+1)+1, cellContents(y * nCells + x), cellColour(y * nCells + x));
                }
                else
                {
                    FillRect((nCellWidth+1)*(x+1), (nCellWidth+1)*(y+1), nCellWidth, nCellWidth, cellMarked(y * nCells + x));
                }

        //victory
        if (!bGameOver && nNonVisibleCells == nMines)
        {
            bGameOver = true;
            DrawString(75,160,"You Win!",olc::GREEN);
        }
        return true;
    }
};

int main()
{
    olcMineSweeper game;
    if (game.Construct(200, 200, 4, 4))
        game.Start();
    return 0;
}