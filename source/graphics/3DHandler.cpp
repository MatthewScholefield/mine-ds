#include <nds.h>
#include <vector>
#include "3DHandler.h"
#define 	BLEND_CR   (*(vuint16*)0x04000050)
#define 	BLEND_AB   (*(vuint16*)0x04000052)
#define 	BLEND_Y   (*(vuint16*)0x04000054)

std::vector<Pair3<std::pair<int, int> > > triangles;

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	triangles.emplace_back(Pair3<std::pair<int, int> >(std::pair<int, int>(x1, y1), std::pair<int, int>(x2, y2), std::pair<int, int>(x3, y3)));
}

void drawRect(int x, int y, int sx, int sy)
{
	drawTriangle(x, y, x + sx, y, x + sx, y + sy);
	drawTriangle(x, y, x + sx, y + sy, x, y + sy);
}

void init3D()
{
	// initialize gl
	glInit();
  BLEND_AB = 0x1010;
  BLEND_CR = BLEND_ALPHA | BLEND_SRC_BG0 |BLEND_DST_BG2;	
	// enable antialiasing
	glEnable(GL_BLEND); //DISP3DCNT_ENUM
 
	// setup the rear plane
	glClearColor(0, 0, 0, 0); // BG must be opaque for AA to work
  glClearPolyID(0);
	glClearDepth(0x7FFF);
	//this should work the same as the normal gl call
	glViewport(0, 0, 255, 191);
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glOrthof32(0,256,192,0,-100000,100000);
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE ); //*/
}

void update3D()
{
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(7) | POLY_CULL_NONE);

	for (auto &i : triangles)
	{
		glBegin(GL_TRIANGLE);
		glColor3b(0, 192, 255);
		for (auto &j : i.elements)
			glVertex3v16(j.first, j.second, 0);
	}

	glPopMatrix(1);

	glFlush(0);
	triangles.clear();
}
