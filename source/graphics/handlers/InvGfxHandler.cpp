#include "InvGfxHandler.h"
#include "../subBgHandler.h"
#include "../graphics.h"

void InvGfxHandler::drawSlots(int selectedSlot, int startX, int startY, int xCount, int yCount, int xSpace, int ySpace)
{
	int slot = 0;
	for (int j = 0; j < yCount; ++j)
		for (int i = 0; i < xCount; ++i)
		{
			int tile = 154;
			if (slot == selectedSlot)
				tile = 155;
			setSubBgTile(startX + i*xSpace, startY + j*ySpace, tile);
			setSubBgTile(startX + i*xSpace, startY + j * ySpace + 1, tile, V_FLIP);
			setSubBgTile(startX + i * xSpace + 1, startY + j*ySpace, tile, H_FLIP);
			setSubBgTile(startX + i * xSpace + 1, startY + j * ySpace + 1, tile, BOTH_FLIP);
			++slot;
		}
}

void InvGfxHandler::update()
{
	int a = 0;
	for (int j = 0; j < xCount; ++j)
		for (int i = 0; i < yCount; ++i)
		{
			if (inv.blocks[a].blockId == AIR && loadedInvGfx[a])
			{
				unloadGraphic(&invBlockGfx[a]);
				loadedInvGfx[a] = false;
				loadedInvID[a] = AIR;
			}
			else if (inv.blocks[a].blockId != loadedInvID[a])
			{
				if (loadedInvGfx[a])
					unloadGraphic(&invBlockGfx[a]);
				else
					loadedInvGfx[a] = true;
				loadGraphicSub(&invBlockGfx[a], GRAPHIC_BLOCK, inv.blocks[a].blockId);
				loadedInvID[a] = inv.blocks[a].blockId;
			}
			if (loadedInvGfx[a])
				showGraphic(&invBlockGfx[a], startX * 8 + i * 8 * xDist, startY * 8 + j * 8 * yDist, false, 2);
			++a;
		}
}