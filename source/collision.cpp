#include "general.h"

int spriteCol(int x1, int y1, int x2, int y2, int x1Size, int y1Size, int x2Size, int y2Size)
{
	--x1Size;
	--x2Size;
	--y1Size;
	--y2Size;
	if ((x1 + x1Size >= x2) && (x1 <= x2 + x2Size) && (y2 + y2Size >= y1) && (y2 <= y1 + y1Size))
		return 1;
	else
		return 0;
}
