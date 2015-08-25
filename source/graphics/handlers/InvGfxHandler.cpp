#include "InvGfxHandler.h"

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