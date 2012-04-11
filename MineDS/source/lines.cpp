#include <cstdlib>
#include "world.h"
////////////////////////////////////////////////////////////////////////////////

///@breif Draws a line from x1,y1 to x2,y2 with blockId in the worldObject* world
///Copied from devscene and modified a tiny bit, probably.
void drawLineDown(worldObject* world,int x, int y, int blockId)
{
	int i;
	for (i=y;i<WORLD_HEIGHT;i++)
	{
		world->blocks[x][i]=blockId;
	}
}
void drawLineThing(worldObject* world,int x1,int y1,int x2,int y2,int blockId)
{
    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    int delta_x(x2 - x1);
    signed char ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;
 
    int delta_y(y2 - y1);
    signed char iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;
 
    drawLineDown(world,x1,y1,blockId);
 
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
 
        while (x1 != x2)
        {
            if (error >= 0)
            {
                if (error || (ix > 0))
                {
                    y1 += iy;
                    error -= delta_x;
                }
                // else do nothing
            }
            // else do nothing
 
            x1 += ix;
            error += delta_y;
 
			drawLineDown(world,x1,y1,blockId);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
 
        while (y1 != y2)
        {
            if (error >= 0)
            {
                if (error || (iy > 0))
                {
                    x1 += ix;
                    error -= delta_y;
                }
                // else do nothing
            }
            // else do nothing
 
            y1 += iy;
            error += delta_x;
 
			drawLineDown(world,x1,y1,blockId);
        }
    }
}


