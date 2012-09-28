#include <nds.h>
#include <time.h>
#include <stdio.h>
#include "nifi.h"
#include "communications.h"
#include "mainGame.h"
#include "stage.h"
int main()
{
	consoleDemoInit();
	nifiInit();
	nifiEnable();
	stageInit();
	initMainGame();
	srand(time(NULL));
	while(1)
	{
		mainGame();
	}
	return 1;
}
