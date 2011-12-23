#include "world.h"
#include "blockID.h"
#include <stdio.h>
#include <nds.h>	
#define COAL_START WORLD_HEIGHT/8
#define COAL_RARENESS 48
#define IRON_START WORLD_HEIGHT/3
#define IRON_RARENESS 64
#define GOLD_START WORLD_HEIGHT/3+20
#define GOLD_RARENESS 68
#define REDSTONE_START WORLD_HEIGHT/2
#define REDSTONE_RARENESS 48
#define DIAMOND_START WORLD_HEIGHT/3*2
#define DIAMOND_RARENESS 186
#define LAPIS_START WORLD_HEIGHT/3*2
#define LAPIS_RARENESS 88
void fixgrass(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
			if (world->blocks[i][j]==GRASS) world->blocks[i][j]=DIRT; //Change all the GRASS blocks into dirt blocks
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==DIRT){
				world->blocks[i][j]=GRASS; //Then if a block should be grass make it grass
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
			else if (world->blocks[i][j]!=AIR && world->blocks[i][j]!=LEAF && world->blocks[i][j]!=LOG){ //And If we have not encountered dirt and we are at a different block
				j=WORLD_HEIGHT+1;			    //Exit this X
			}
		}
	swiWaitForVBlank();
}
void fixsnowgrass(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
			if (world->blocks[i][j]==SNOW_GRASS) world->blocks[i][j]=1207; //Change all the SNOW_GRASS blocks into <bugfixer> blocks
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==1207){
				world->blocks[i][j]=SNOW_GRASS; //Then if a block should be grass make it grass
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
			else if (world->blocks[i][j]!=AIR && world->blocks[i][j]!=LEAF && world->blocks[i][j]!=LOG){ //And If we have not encountered dirt and we are at a different block
				j=WORLD_HEIGHT+1;			    //Exit this X
			}
		}
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
			if (world->blocks[i][j]==1207) world->blocks[i][j]=DIRT; //Change all the SNOW_GRASS blocks into <bugfixer> blocks
	swiWaitForVBlank();
}
void rockwall(worldObject* world,int x,int y){
	int i;
	for (i=y;i<=WORLD_HEIGHT;i++){
		world->blocks[x][i]=STONE;
	}
}
void addore(worldObject* world){
	int y=COAL_START;
	int i;
	int id=COAL_ORE;
	int rareness=COAL_RARENESS*2;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	swiWaitForVBlank();
	y=IRON_START;
	id=IRON_ORE;
	rareness=IRON_RARENESS*2;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	swiWaitForVBlank();
	y=GOLD_START;
	id=GOLD_ORE;
	rareness=GOLD_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	swiWaitForVBlank();
	y=REDSTONE_START;
	id=REDSTONE_ORE;
	rareness=REDSTONE_RARENESS*2;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	swiWaitForVBlank();
	y=DIAMOND_START;
	id=DIAMOND_ORE;
	rareness=DIAMOND_RARENESS*2;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	y=LAPIS_START;
	id=LAPIS_ORE;
	rareness=LAPIS_RARENESS*2;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
}
void addtree(worldObject* world,int x,int y,int type){
	int id;
	if (type==0) id=LOG;
	else if (type==1) id=DARK_WOOD;
	else if (type==2) id=WHITE_WOOD;
	int height=rand() % 2+2;
	int i;
	for(i=0;i<=height;i++){
		y--;
		world->blocks[x][y]=id;
	}
	y--;
	height=rand() %2+2;
	x-=height;
	for (i=0;i<=height;i++){
		x++;
		if (world->blocks[x][y]==AIR) world->blocks[x][y]=LEAF;	
	}

	x-=height;
	height-=2;
	y--;
	x-=height/2;
	for (i=0;i<=height;i++){
		x++;
		if (world->blocks[x][y]==AIR) world->blocks[x][y]=LEAF;	
	}
}
void addrock(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==DIRT){
				rockwall(world,i,j+rand()%(3)+3);
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
			else if(world->blocks[i][j]==SAND){
				int startrock = j+rand() % 3+3;
				int k;
				for (k=j+rand() % 2+1;k<=startrock;k++) world->blocks[i][k]=SANDSTONE;
				rockwall(world,i,startrock);
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
		}

	swiWaitForVBlank();
}
void addCACTUS(worldObject* world,int x,int y){
	int height=rand() %1+1;
	int i;
	for (i=0;i<=height;i++){
		y--;
		world->blocks[x][y]=CACTUS;
	}
}

void addFLOWER_RED(worldObject* world,int x,int y){
	int height=rand() %2+2;
	int i;
	for (i=0;i<=height;i++){
		y--;
        world->blocks[x][y]=FLOWER_RED;
		}
}

void addFLOWER_YELLOW(worldObject* world,int x,int y){
	int height=rand() %2+2;
	int i;
	for (i=0;i<=height;i++){
		y--;
        world->blocks[x][y]=FLOWER_YELLOW;
		}
}

void addTrees(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i+=rand() % 5+5)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if (world->blocks[i][j]==GRASS && (world->blocks[i-1][j]==GRASS || world->blocks[i+1][j]==GRASS)) addtree(world,i,j,0);
			else if (world->blocks[i][j]==SAND) addCACTUS(world,i,j);	
            else if (world->blocks[i][j]==SNOW_GRASS && (world->blocks[i-1][j]==SNOW_GRASS || world->blocks[i+1][j]==SNOW_GRASS)) addtree(world,i,j,1); //1 makes the trees have darker wood...
			else if (world->blocks[i][j]==GRASS) addFLOWER_RED(world,i,j);	
			else if (world->blocks[i][j]==GRASS) addFLOWER_YELLOW(world,i,j);	
		}
	swiWaitForVBlank();
}

void modifyWorld(worldObject* world){

	swiWaitForVBlank();
	int x;
	fixgrass(world);
	fixsnowgrass(world);
	addrock(world);
	addore(world);
	addTrees(world);
	for (x=0;x<=WORLD_WIDTH;x++){
		world->blocks[x][WORLD_HEIGHT]=BEDROCK;	
	}
	swiWaitForVBlank();
	world->version=2;
}
int mountainbiome(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	for (int x=startx;x<=endx;x++){
		y+=rand() % 5-2;
		if (y<5) y-=rand() %3-3;
		if (y>WORLD_HEIGHT/3) y-=rand()%3;
		int i;
		//iprintf("%d\n",y);
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=DIRT;
	}
	return y;
}
int flatbiome(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	int x;
	int changey=0;
	int i;
	int times=rand()%3+1;
	for (x=startx;x<=endx;x++){
		if (changey==0){
			y+=rand() % 3-1;	
			times++;	
			if (y<5) y-=rand() %2-2;
			if (y>WORLD_HEIGHT/3) y-=rand()%2;	
			changey=rand() % 5+2;
			swiWaitForVBlank();
		}
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=DIRT;
		changey--;
	}
	return y;
}
int sandbiome(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	int x;
	int changey=0;
	int i;
	int times=rand()%3+1;
	for (x=startx;x<=endx;x++){
		if (changey==0){
			y+=rand() % 3-1;	
			times++;	
			if (y<5) y-=rand() %2-2;
			if (y>WORLD_HEIGHT/3) y-=rand()%2;	
			changey=4+rand() % 2;
			swiWaitForVBlank();
		}
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=SAND;
		changey--;
	}
	return y;
}

int snowbiome(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	int x;
	int changey=0;
	int i;
	int times=rand()%3+1;
	for (x=startx;x<=endx;x++){
		if (changey==0){
			y+=rand() % 3-1;	
			times++;	
			if (y<5) y-=rand() %2-2;
			if (y>WORLD_HEIGHT/3) y-=rand()%2;	
			changey=4+rand() % 2;
			swiWaitForVBlank();
		}
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=SNOW_GRASS;
		changey--;
	}
	return y;
}

void generateWorld(worldObject* world){
	int x=0;
	int y=rand() % (WORLD_HEIGHT/3-8)+5;
	int i;
	for(i=0;i<=WORLD_WIDTH;i++)
		for (x=0;x<=WORLD_HEIGHT;x++) world->blocks[i][x]=AIR;
	x=0;
	int sizex=0;
	int endx=0;
	int biometype=0;
	int y2;
	while(x!=WORLD_WIDTH){
		sizex=rand() % 16+16;
		endx=x+sizex;
		if (endx>WORLD_WIDTH) endx=WORLD_WIDTH;
		biometype=rand() %4;
		//biometype=1; //testing stuff
		if (biometype==0) y2=mountainbiome(world,x,y,endx);
		else if (biometype==1) y2=flatbiome(world,x,y,endx);
		else if (biometype==2) y2=sandbiome(world,x,y,endx);
		else if (biometype==3) y2=snowbiome(world,x,y,endx);
		x=endx; //Plus 1 x to move one block between biomes
		y=y2;
		//x++;
		//swiWaitForVBlank();
	}
	modifyWorld(world);
}
void randGenerate(worldObject* world){
	int i,j;
	for(i=0;i<=WORLD_WIDTH;i++){
		for(j=0;j<=WORLD_HEIGHT;j++){
			world->blocks[WORLD_WIDTH][j]=rand()%23;
		}
	}
}
