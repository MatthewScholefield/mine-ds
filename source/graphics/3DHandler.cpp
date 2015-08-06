#include <nds.h>
#include <vector>
#include "3DHandler.h"

// Vector of (Color R,G,B) and (3 Coords X,Y,Z)
std::vector<std::pair<Pair3<int>, Pair3<std::pair<int, int> > > > triangles;

void drawTriangle(Pair3<int> color, int x1, int y1, int x2, int y2, int x3, int y3)
{
	triangles.emplace_back(color, Pair3 <std::pair<int, int> >(std::pair<int, int>(x1, y1), std::pair<int, int>(x2, y2), std::pair<int, int>(x3, y3)));
}

void drawRect(Pair3<int> color, int x, int y, int sx, int sy)
{
	drawTriangle(color, x, y, x + sx, y, x + sx, y + sy);
	drawTriangle(color, x, y, x + sx, y + sy, x, y + sy);
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
	glPolyFmt(POLY_ALPHA(7) | POLY_CULL_NONE);

	for (auto &i : triangles)
	{
		glBegin(GL_TRIANGLE);
		glColor3b(i.first.elements[0], i.first.elements[1], i.first.elements[2]);
		for (auto &j : i.second.elements)
			glVertex3v16(j.first, j.second, 0);
	}
	triangles.clear();

	glPopMatrix(1);

	glFlush(0);
}
