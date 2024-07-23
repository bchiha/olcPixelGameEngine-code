#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_QUICKGUI
#include "olcPGEX_QuickGUI.h"

class GLCDGraphic : public olc::PixelGameEngine
{
private:
    int nColumns = 8, nRows = 8, nCellWidth;
    olc::vi2d nPreviousCell = { -1, -1};
    std::vector<bool> vCells;

    olc::QuickGUI::Manager guiManager;
	olc::QuickGUI::Slider* guiColumns = nullptr;
	olc::QuickGUI::Slider* guiRows = nullptr;
    olc::QuickGUI::Button* guiExport = nullptr;
    olc::QuickGUI::Button* guiClear = nullptr;
    olc::QuickGUI::CheckBox* guiLock = nullptr;
    olc::QuickGUI::CheckBox* guiHelp = nullptr;

    template< typename T > std::string int_to_string( T i )
    {
    std::stringstream stream;
    stream << i;
    return stream.str();
    }

    void ClearCells()
    {
        // Clear Cells
        vCells.clear();
        for (int i = 0; i < (nColumns * nRows); i++)
        {
            vCells.emplace_back(false);
        }
    }

    void ExportToTEC()
    {
        //shift bits to the left to fill up byte
        int nByte;
        std::cout << "\n; Sprite Data\n" << std::hex << std::uppercase;
        for (int y = 0; y < nRows; y++)
        {
            int nBitCount = 0;
            std::cout << "    .db ";
            for (int x = 0; x < nColumns; x++)
            {
                if (x % 8 == 0)
                    nByte = 0;
                nByte = nByte << 1;
                nBitCount++;
                if (vCells[y * nColumns + x])
                    nByte++;
                if (nBitCount == 8)
                {                    
                    std::cout << std::setfill('0') << std::setw(2) << nByte << "H";
                    nBitCount = 0;
                    if (x != nColumns - 1)
                        std::cout << ", ";
                }                
            }
            if (nBitCount != 0)
                std::cout << std::setfill('0') << std::setw(2) << nByte << "H" << "\n";
            else
                std::cout << "\n";
        }
    }

public:
    GLCDGraphic()
    {
        // Name your application
        sAppName = "GLCD Graphics Designer";
    }
    
public:
    bool OnUserCreate() override
    {
        // Column and Row sliders
        guiColumns = new olc::QuickGUI::Slider(guiManager,
            { 10.0f, 10.0f }, { 140.0f, 10.0f}, 8, 128, nColumns);
        guiRows = new olc::QuickGUI::Slider(guiManager,
            { 160.0f, 10.0f }, { 290.0f, 10.0f}, 8, 64, nRows);
        guiLock = new olc::QuickGUI::CheckBox(guiManager, "Lock Grid", true,
            { 300.0f, 3.0f }, { 90.0f, 15.0f }); 
        guiClear = new olc::QuickGUI::Button(guiManager, "Clear",
            { 400.0f, 3.0f }, { 90.0f, 15.0f });
        guiExport = new olc::QuickGUI::Button(guiManager, "Export",
            { 500.0f, 3.0f }, { 90.0f, 15.0f });
        guiHelp = new olc::QuickGUI::CheckBox(guiManager, "Help", false,
            { 600.0f, 3.0f}, { 40.0f, 15.0f });
        guiManager.colHover = olc::DARK_RED;
        guiManager.colClick = olc::RED;
        // Get Cell width
        nCellWidth = (640 / nColumns) < (320 / nRows) ? (640 / nColumns) : (320 / nRows);
        // Clear screen
        Clear(olc::BLACK);
        // Draw Static Text
        DrawString({ 40, 20 }, "Colums:");
        DrawString({ 200, 20 }, "Rows:");
        // Initialise Cells
        vCells.reserve(128 * 64); //Max size
        ClearCells();

        return true;
    }
    
    bool OnUserUpdate(float fElapsedTime) override
    {
        // Update GUI
        guiManager.Update(this);        
        if (guiLock->bChecked && (guiColumns->bHeld || guiRows->bHeld))
        {
            nColumns = std::trunc(guiColumns->fValue);
            nRows = std::trunc(guiRows->fValue);
            nCellWidth = (640 / nColumns) < (320 / nRows) ? (640 / nColumns) : (320 / nRows);
            // resize cells to new Columns/Rows
            ClearCells();
            // Clear grid screen for redrawing
            FillRect({ 10, 30 }, { ScreenWidth() - 10, ScreenHeight() - 30}, olc::BLACK);
        }
        if (guiLock->bChecked && guiClear->bPressed)
            ClearCells();
        if (guiExport->bPressed)
            ExportToTEC();
        // Draw the GUI
        guiManager.DrawDecal(this);
        // Display current Col/Rows
        FillRect({ 95, 19 }, {30, 10}, olc::BLACK);
        FillRect({ 240, 19 }, {30, 10}, olc::BLACK);
        DrawString({ 95, 20 }, int_to_string(std::trunc(guiColumns->fValue)));
        DrawString({ 240, 20 }, int_to_string(std::trunc(guiRows->fValue)));
        // Display help
        if (guiHelp->bChecked)
        {
            FillRect({ 10, 30 }, { ScreenWidth() - 10, ScreenHeight() - 30}, olc::BLACK);
            FillRect({ 110, 90 }, { 430, 120 }, olc::DARK_BLUE);
            DrawRect({ 110, 90 }, { 430, 120 }, olc::WHITE);
            DrawString({ 120, 100}, "GLCD Sprite Designer for the TEC-1G Z80 Computer");
            DrawString({ 120, 120}, "- Click or Click+Drag MOUSE to draw & undraw pixels");
            DrawString({ 120, 130}, "- Use SLIDERS to change grid size");
            DrawString({ 120, 140}, "- LOCK GRID to preserve sprite if sliders are moved");
            DrawString({ 120, 150}, "- CLEAR to start afresh");
            DrawString({ 120, 160}, "- EXPORT to generate Z80 data for the GLCD (console)");
            DrawString({ 120, 170}, "- Press HELP to close this screen");
            DrawString({ 120, 190}, "Created by B Chiha (Ready? Z80) 2024");
            return true; //exit to disable mouse updates and grid draw
        }
        if (guiHelp->bPressed && !guiHelp->bChecked)
        {
            // Clear help
            FillRect({ 10, 30 }, { ScreenWidth() - 10, ScreenHeight() - 30}, olc::BLACK);
        }
        // Draw Cells
        for (int y = 0; y < nRows; y++)
        {
            for (int x = 0; x < nColumns; x++)
            {
                FillRect({ 10 + (x * (nCellWidth)), 30 + (y * (nCellWidth)) },
                         { nCellWidth - 1, nCellWidth - 1},
                         vCells[y * nColumns + x] ? olc::DARK_GREEN : olc::DARK_GREY);
            }
        }
        // Draw Lock frame
        DrawRect({ 8, 28 }, { nColumns * nCellWidth + 2, nRows * nCellWidth + 2},
                 !guiLock->bChecked ? olc::RED : olc::BLACK );
        // Get mouse position in cell size
        olc::vi2d nSelectedCell = { (GetMouseX() - 10) / nCellWidth,
                                    (GetMouseY() - 30) / nCellWidth };
        //Create mouse hover highlights if mouse in over cells
        if ((GetMouseX() - 10) >=0 && (GetMouseY() - 30) >=0 && nSelectedCell.x >= 0 && nSelectedCell.x < nColumns && nSelectedCell.y >= 0 && nSelectedCell.y < nRows )
        {
            FillRect({ 10 + (nSelectedCell.x * (nCellWidth)),
                        30 + (nSelectedCell.y * (nCellWidth)) },
                        { nCellWidth - 1, nCellWidth - 1},
                        vCells[nSelectedCell.y * nColumns + nSelectedCell.x] == true ? olc::GREEN : olc::GREY);
        }
            //Display mouse X,Y
            FillRect({10,355},{100,10},olc::BLACK);
            DrawString({10,355},"(" + std::to_string(nSelectedCell.x) + "," +
                    std::to_string(nSelectedCell.y) + ")");
        //Set or unset Cell on mouse press or held
        if (GetMouse(0).bHeld)
        {
            if ((GetMouseX() - 10) >=0 && (GetMouseY() - 30) >=0 && nSelectedCell.x >= 0 && nSelectedCell.x < nColumns && nSelectedCell.y >= 0 && nSelectedCell.y < nRows && nSelectedCell != nPreviousCell)
            {
                vCells[nSelectedCell.y * nColumns + nSelectedCell.x] = !vCells[nSelectedCell.y * nColumns + nSelectedCell.x];
                nPreviousCell = nSelectedCell;
            }
        }
        if (GetMouse(0).bReleased)
            nPreviousCell = { -1, -1 };

        return true;
    }
};

int main()
{
    GLCDGraphic app;
    if (app.Construct(660, 365, 1, 1))
        app.Start();
    return 0;
}