#include "InvGfxHandler.h"
#include "../subBgHandler.h"
#include "../graphics.h"
#include "../../general.h"
#include "../UI.h"

void InvGfxHandler::drawSlots(bool drawSelected)
{
	int slot = 0;
	for (int j = 0; j < yCount; ++j)
		for (int i = 0; i < xCount; ++i)
		{
			int tile = 154;
			if (slot == inv.hand && drawSelected)
				tile = 155;
			drawInvSlot(startX + i*xDist, startY + j*yDist, tile);
			++slot;
		}
}

void InvGfxHandler::drawQuantities()
{
	for (int i = 0; i < xCount; ++i)
		for (int j = 0; j < yCount; ++j)
			if (inv.blocks[j * xCount + i].amount != 0 && inv.blocks[j * xCount + i].ID != 0)
			{
				printXY(startX + i * xDist, startY + 1 + j * yDist, inv.blocks[j * xCount + i].amount);
				if (inv.blocks[j * 15 + i].amount < 10)
					printXY(startX + i * xDist + 1, startY + 1 + j * yDist, " ");
			}
			else
				printXY(startX + i * xDist, startY + 1 + j * yDist, "  ");
}

void InvGfxHandler::update()
{
	int a = 0;
	for (int i = 0; i < yCount; ++i)
		for (int j = 0; j < xCount; ++j)
		{
			if (inv.blocks[a].ID != loadedID[a])
			{
				gfx[a].reload(GraphicType::BLOCK, inv.blocks[a].ID, false);
				loadedID[a] = inv.blocks[a].ID;
			}
			gfx[a].draw(startX * 8 + j * 8 * xDist, startY * 8 + i * 8 * yDist, false, 2);
			++a;
		}
}