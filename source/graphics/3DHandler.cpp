#include <nds.h>
#include <vector>
#include "3DHandler.h"

std::vector<ColoredRect> rectangles;

void drawRect(Color color, int x, int y, int sx, int sy)
{
	rectangles.emplace_back(color, Rectangle{Coord(x, y), Coord(x + sx, y), Coord(x + sx, y + sy), Coord(x, y + sy)});
}

void init3D()
{
	glInit();
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG0 | BLEND_DST_BG2;

	glEnable(GL_BLEND); //DISP3DCNT_ENUM

	glClearColor(0, 0, 0, 0);
	glClearPolyID(0);
	glClearDepth(0x7FFF);
	glViewport(0, 0, 255, 191);

	//Any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION); //GL_MATRIX_MODE_ENUM
	glLoadIdentity();
	glOrthof32(0, 256, 192, 0, -100000, 100000);
}

void update3D()
{
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolyFmt(POLY_ALPHA(10) | POLY_CULL_NONE);

	for (auto &i : rectangles)
	{
		glBegin(GL_QUADS);
		glColor3b(i.first[0], i.first[1], i.first[2]);
		for (auto &j : i.second)
			glVertex3v16(j.first, j.second, 0);
	}
	rectangles.clear();

	glPopMatrix(1);

	glFlush(0);
}
