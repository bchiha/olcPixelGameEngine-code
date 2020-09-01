// Maze Manic..
//
// Based of the Sensational Games for the Amstrad CPC 464
//
// Uses rectangles and triangles with a changing colour pallet to simulator
// 3D depth and paths.


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


enum Direction {WEST=1, NORTH, EAST, SOUTH};

// Override base class with your custom functionality
class MazeManic : public olc::PixelGameEngine
{
public:
    MazeManic()
    {
        // Name you application
        sAppName = "Maze Manic";
    }

private:
    //Pen Colour for walls.  15 in total.  colour 0 is the far back wall.  This colour never changes
    olc::Pixel pen[16] = {olc::DARK_RED};
    //Maze has 11 x 5 rooms.  The number represents total exits for the room
    // 8 = North exit, 4 = South exit, 2 = East exit, 1 = West exit 
    int nMaze[55] = { 4, 6, 1, 6, 7, 5, 4, 6, 5, 6, 1,
                      10, 13, 2, 9, 12, 10, 11, 9, 14, 11, 1,
                      6, 9, 6, 3, 9, 2, 5, 6, 9, 6, 5,
                      12, 6, 13, 2, 5, 6, 15, 9, 6, 9, 12,
                      10, 9, 10, 3, 9, 8, 10, 3, 9, 2, 9};
    int pos = 0;        //initial position
    int view = SOUTH;   //initial facing direction
    int nMoves = 0;     //initial moves made
    // Exit constants
    const int nHasNorth = 8, nHasSouth = 4, nHasEast = 2, nHasWest = 1;

    // Use for map drawing between previous position to new position
    std::vector<std::pair<olc::vi2d, olc::vi2d>> vMap;
    
    // Method based on view left and view right, sets the colour pallet to represents wall and
    // exits.  C = closed wall, O = open wall (exit).  It sets the pen colours
    // Walls use binary to represent open/closed wall.  LSB is far wall and MSB is near wall.
    void SetView(int vl, int vr)
    {
        //14 x 9 lookup table
        olc::Pixel views[211] {olc::BLACK,
            //view 1  FAR CCC CCC
            olc::RED, olc::RED, olc::RED, olc::RED, olc::RED, olc::RED, olc::RED, olc::RED,
            olc::RED, olc::RED, olc::RED, olc::RED, olc::CYAN, olc::DARK_CYAN, olc::VERY_DARK_CYAN,

            //view 2  FAR CCO OCC
            olc::RED, olc::RED, olc::VERY_DARK_CYAN, olc::RED, olc::RED, olc::VERY_DARK_CYAN, olc::RED, olc::RED,
            olc::DARK_RED, olc::RED, olc::RED, olc::DARK_RED, olc::CYAN, olc::DARK_CYAN, olc::VERY_DARK_CYAN,

            //view 3  FAR COC COC
            olc::RED, olc::DARK_CYAN, olc::RED, olc::RED, olc::DARK_CYAN, olc::RED, olc::RED,
            olc::DARK_RED, olc::RED, olc::RED, olc::DARK_RED, olc::RED, olc::CYAN, olc::DARK_CYAN,
            olc::VERY_DARK_CYAN,

            //view 4  FAR COO OOC
            olc::RED, olc::DARK_CYAN, olc::VERY_DARK_CYAN, olc::RED, olc::DARK_CYAN, olc::VERY_DARK_CYAN, olc::RED,
            olc::DARK_RED, olc::DARK_RED, olc::RED, olc::DARK_RED, olc::DARK_RED, olc::CYAN,
            olc::DARK_CYAN, olc::VERY_DARK_CYAN,

            //view 5  FAR OCC CCO
            olc::CYAN, olc::RED, olc::RED, olc::CYAN, olc::RED, olc::RED, olc::DARK_RED,
            olc::RED, olc::RED, olc::DARK_RED, olc::RED, olc::RED, olc::CYAN, olc::DARK_CYAN,
            olc::VERY_DARK_CYAN,

            //view 6  FAR OCO OCO
            olc::CYAN, olc::RED, olc::VERY_DARK_CYAN, olc::CYAN, olc::RED, olc::VERY_DARK_CYAN, olc::DARK_RED,
            olc::RED, olc::DARK_RED, olc::DARK_RED, olc::RED, olc::DARK_RED, olc::CYAN,
            olc::DARK_CYAN, olc::VERY_DARK_CYAN,

            //view 7  FAR OOC COO
            olc::CYAN, olc::DARK_CYAN, olc::RED, olc::CYAN, olc::DARK_CYAN, olc::RED, olc::RED,
            olc::DARK_RED, olc::RED, olc::RED, olc::DARK_RED, olc::RED, olc::CYAN,
            olc::DARK_CYAN, olc::VERY_DARK_CYAN,

            //view 8  FAR OOO OOO
            olc::CYAN, olc::DARK_CYAN, olc::VERY_DARK_CYAN, olc::CYAN, olc::DARK_CYAN, olc::VERY_DARK_CYAN,
            olc::DARK_RED, olc::RED, olc::DARK_RED, olc::DARK_RED, olc::RED, olc::DARK_RED,
            olc::CYAN, olc::DARK_CYAN, olc::VERY_DARK_CYAN,

            //view 9  MID  CC CC
            olc::RED, olc::RED, olc::DARK_RED, olc::RED, olc::RED, olc::DARK_RED, olc::RED,
            olc::RED, olc::DARK_RED, olc::RED, olc::RED, olc::DARK_RED, olc::CYAN,
            olc::DARK_CYAN, olc::DARK_RED,

            //view 10 MID  CO OC
            olc::RED, olc::DARK_CYAN, olc::DARK_RED, olc::RED, olc::DARK_CYAN, olc::DARK_RED,
            olc::RED, olc::DARK_RED, olc::DARK_RED, olc::RED, olc::DARK_RED, olc::DARK_RED,
            olc::CYAN, olc::DARK_CYAN, olc::DARK_RED,

            //view 11 MID  OC CO
            olc::CYAN, olc::RED, olc::DARK_RED, olc::CYAN, olc::RED, olc::DARK_RED, olc::DARK_RED,
            olc::RED, olc::DARK_RED, olc::DARK_RED, olc::RED, olc::DARK_RED, olc::CYAN,
            olc::DARK_CYAN, olc::DARK_RED,

            //view 12 MID  OO OO
            olc::CYAN, olc::DARK_CYAN, olc::DARK_RED, olc::CYAN, olc::DARK_CYAN, olc::DARK_RED,
            olc::RED, olc::DARK_RED, olc::DARK_RED, olc::RED, olc::DARK_RED, olc::DARK_RED,
            olc::CYAN, olc::DARK_CYAN, olc::DARK_RED,

            //view 13 NEAR  C C
            olc::RED, olc::DARK_RED, olc::DARK_RED, olc::RED, olc::DARK_RED, olc::DARK_RED,
            olc::RED, olc::DARK_RED, olc::DARK_RED, olc::RED, olc::DARK_RED, olc::DARK_RED,
            olc::CYAN, olc::DARK_RED, olc::DARK_RED,

            //view 13 NEAR  O O
            olc::CYAN, olc::DARK_RED, olc::DARK_RED, olc::CYAN, olc::DARK_RED, olc::DARK_RED,
            olc::DARK_RED, olc::DARK_RED, olc::DARK_RED, olc::DARK_RED, olc::DARK_RED,
            olc::DARK_RED, olc::CYAN, olc::DARK_RED, olc::DARK_RED
        };

        // Map the colours to the pen
        for (int i = 1; i <= 15; i++)
        {
            if (i > 3 && i < 10)   //use view right index
                pen[i] = views[(vr-1)*15+i];
            else
                pen[i] = views[(vl-1)*15+i];
        }
    }

public:
    bool OnUserCreate() override
    {

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        int displacement, vl = 1, vr = 1, a=1, orig_pos = pos, look_ahead, dx, dy;
        //set view
        if (view == WEST) displacement = -1;
        if (view == NORTH) displacement = -11;
        if (view == EAST) displacement = 1;
        if (view == SOUTH) displacement = 11;
        //work out view left and view right based on 2 wall depths..  If depth is 0, previous
        //depth colours get ignored.
        for (int depth = 2; depth >= 0; depth--)
        {
            look_ahead = depth * displacement;
            // if depth is still within the maze
            if ((orig_pos + look_ahead) >= 0 && (orig_pos + look_ahead) <= 54)
            {
                //check exits at that depth
                pos = orig_pos + look_ahead;
                bool bN = false, bE = false, bS = false, bW = false;
                if ((nMaze[pos] & nHasNorth) == 8) bN = true;
                if ((nMaze[pos] & nHasSouth) == 4) bS = true;
                if ((nMaze[pos] & nHasEast) == 2) bE = true;
                if ((nMaze[pos] & nHasWest) == 1) bW = true;
                bool bDown = false, bLeft = false, bRight = false;
                if (view == WEST)
                {
                    bDown = bW;
                    bLeft = bS;
                    bRight = bN;
                }
                if (view == NORTH)
                {
                    bDown = bN;
                    bLeft = bW;
                    bRight = bE;
                }
                if (view == EAST)
                {
                    bDown = bE;
                    bLeft = bN;
                    bRight = bS;
                }
                if (view == SOUTH)
                {
                    bDown = bS;
                    bLeft = bE;
                    bRight = bW;
                }
                // if facing wall and no exits.  Bump up vl/vr to 13,1 or 9 which have no exits.
                if (!bDown)  //facing wall
                {
                    vl = 13;
                    a = 1;
                    if (depth == 2) vl = 1;
                    if (depth == 1) vl = 9;
                    vr = vl;
                }
                // add path multiplier if exit exists
                if (bLeft) vl += a;
                if (bRight) vr += a;
                a *= 2;
            }
        }
        // reset pos
        pos = orig_pos;

        // update the pen colour pallet
        SetView(vl,vr);

        // movement
        if (GetKey(olc::Key::UP).bPressed)
        {
            //If no wall ahead, then move into the next postions based on displacement.
            //set map move.
            if (vl < 13)  //not facing a wall
            {
                int sx, sy, ex, ey;
                sx = pos % 11;
                sy = pos / 11;

                pos += displacement;

                ex = pos % 11;
                ey = pos / 11;

                vMap.push_back(std::make_pair(olc::vi2d(sx, sy), olc::vi2d(ex, ey)));
                nMoves++;
            }
        }
        if (GetKey(olc::Key::DOWN).bPressed)
        {
            //look behind to see if exit exits.  If it does, move to that position, while 
            //maintaining current view.  Update map also.
            int backview, backpos, backdisplacement, backexit;
            backview = view + 2;
            if (backview > 4) backview -= 4;  // adjust for wrap around
            backdisplacement = displacement * -1;
            backpos = pos + backdisplacement;
            if (backview == NORTH) backexit = 8;
            if (backview == SOUTH) backexit = 4;
            if (backview == EAST) backexit = 2;
            if (backview == WEST) backexit = 1;
            if (backpos >= 0 && backpos <= 54)  //still on map
            {   
                if ((nMaze[pos] & backexit) != 0) {
                    int sx, sy, ex, ey;
                    sx = pos % 11;
                    sy = pos / 11;

                    pos = backpos;
                    
                    ex = pos % 11;
                    ey = pos / 11;

                    vMap.push_back(std::make_pair(olc::vi2d(sx, sy), olc::vi2d(ex, ey)));
                    nMoves++;
                }
            }
        }
        // change view.
        if (GetKey(olc::Key::LEFT).bPressed)
        {
            view -= 1;
            if (view == 0) view = 4;
        }
        if (GetKey(olc::Key::RIGHT).bPressed)
        {
            view += 1;
            if (view == 5) view = 1;
        }
        // restart game if in cell 53 and enter pressed.
        if (GetKey(olc::Key::ENTER).bPressed && pos == 53)
        {
            pos = 0;
            view = SOUTH;
            nMoves = 0;
            vMap.clear();
        }


        // Draw Maze with correct view
        Clear(olc::BLACK);

        //near top/bottom walls
        FillRect(0,0,640,40,pen[13]);
        FillRect(0,280,640,40,pen[13]);
        //mid top/bottom walls
        FillRect(80,40,480,40,pen[14]);
        FillRect(80,240,480,40,pen[14]);
        //far top/bottom walls
        FillRect(160,80,320,40,pen[15]);
        FillRect(160,200,320,40,pen[15]);  
        //end wall
        FillRect(240,120,160,80,pen[0]);
        //near side walls
        FillRect(0,40,80,240,pen[10]);
        FillRect(560,40,80,240,pen[7]);
        //mid side walls
        FillRect(80,80,80,160,pen[11]);
        FillRect(480,80,80,160,pen[8]);
        //far side walls
        FillRect(160,120,80,80,pen[12]);
        FillRect(400,120,80,80,pen[9]);
        //near corners
        FillTriangle(0,0,0,40,80,40,pen[1]);
        FillTriangle(0,280,0,320,80,280,pen[1]);
        FillTriangle(640,0,640,40,560,40,pen[4]);
        FillTriangle(640,280,640,320,560,280,pen[4]);
        //mid corners
        FillTriangle(80,40,80,80,160,80,pen[2]);
        FillTriangle(80,240,80,280,160,240,pen[2]);
        FillTriangle(560,40,560,80,480,80,pen[5]);
        FillTriangle(560,240,560,280,480,240,pen[5]);
        //far corners
        FillTriangle(160,80,160,120,240,120,pen[3]);
        FillTriangle(160,200,160,240,240,200,pen[3]);
        FillTriangle(480,80,480,120,400,120,pen[6]);
        FillTriangle(480,200,480,240,400,200,pen[6]);
        // angles
        DrawLine(0,320,240,200,pen[0]);
        DrawLine(1,320,241,200,pen[0]);
        DrawLine(640,320,400,200,pen[0]);
        DrawLine(639,320,399,200,pen[0]);
        // downs
        FillRect(240,120,2,80,pen[15]);
        FillRect(398,120,2,80,pen[15]);
        FillRect(160,80,3,160,pen[14]);
        FillRect(480,78,3,164,pen[14]);
        FillRect(80,40,5,280,pen[13]);
        FillRect(555,40,5,280,pen[13]);

        // bottom grid
        DrawLine(0,324,639,324,olc::CYAN);
        DrawLine(0,328,639,328,olc::CYAN);
        DrawLine(0,398,639,398,olc::CYAN);
        DrawLine(0,394,639,394,olc::CYAN);
        DrawLine(158,400,158,322,olc::CYAN);
        DrawLine(150,400,150,322,olc::CYAN);
        DrawLine(512,400,512,322,olc::CYAN);
        DrawLine(520,400,520,322,olc::CYAN);

        DrawString(10,340,"Goal 35",olc::CYAN,2);
        DrawString(530,335,"Navigatge to\nthe ");
        DrawString(560,343,"x",olc::RED);
        DrawString(575,343,"using");
        DrawString(530,352,"the arrow\nkeys to win!!");


        // Draw Map
        FillRect(159,320,353,80,olc::BLACK);
        // Draw Lines... maybe could have optimised this.  Works out if moving up/down or left/right
        // Then draws a rectangle. between the two points.
        for (auto lines : vMap)
        {
            dx = abs(lines.first.x - lines.second.x) * 30;
            dy = abs(lines.first.y - lines.second.y) * 10;
            if (dx == 0) // n / s line
            {
                if (lines.first.y < lines.second.y)
                    FillRect(183+(lines.first.x*30), 340+(lines.first.y*10)-2, 5, dy+5, olc::WHITE);
                else
                    FillRect(183+(lines.second.x*30), 340+(lines.second.y*10)-2, 5, dy+5, olc::WHITE);
            }
            else           // e / w line
            {
                if (lines.first.x < lines.second.x)
                    FillRect(185+(lines.first.x*30)-2, 338+(lines.first.y*10), dx+5, 5, olc::WHITE);
                else
                    FillRect(185+(lines.second.x*30)-2, 338+(lines.second.y*10), dx+5, 5, olc::WHITE);
            }
        }
        // draw mid line on top of map rectangles for look
        for (auto lines : vMap)
        {
            DrawLine(185+(lines.first.x*30), 340+(lines.first.y*10), 
                     185+(lines.second.x*30), 340+(lines.second.y*10), olc::BLACK);
        }
        // Draw Arrow .. Just a simple ascii arrow.  Could be done using sprites or drawing methods.
        std::string arrow;
        if (view == WEST) arrow = "<";
        if (view == NORTH) arrow = "^";
        if (view == EAST) arrow = ">";
        if (view == SOUTH) arrow = "V";
        DrawString(180+(30*(pos % 11)),335+(10*(pos / 11)),arrow,olc::CYAN,2);
        DrawString(452,376,"x",olc::RED);

        // Update moves...
        DrawString(10,370,"Move " + std::to_string(nMoves),olc::CYAN,2);

        // Winner!
        if (pos == 53)
        {
            DrawString(32,192,"You Win!! Press Enter to start again",olc::WHITE,2);
        }

        return true;
        
    }
};

int main()
{
    MazeManic game;
    if (game.Construct(640, 400, 2, 2))
        game.Start();
    return 0;
}