#include <nds.h>
#include <stdio.h>
#include "Font.h"
#include "scene.h"
#include "startscreen.h"
Scene* sc;
void scenechange(){
	if (sc->nxtsc!=NULL){
		Scene* nw = sc->nxtsc;
		delete sc;
		sc = nw;
		swiWaitForVBlank(); //Wait a frame before starting
		sc->start();
	}
}
int main(void)
{
	initFonts();
	sc = new startscreen();
	sc->start();
	while(1) {
		scenechange();
		sc->update();
		swiWaitForVBlank();
	}
}
