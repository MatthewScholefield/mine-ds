/* _____________________________________________
  | 						|
  |		worldgen.cpp			|
  |     Part of Mine DS , by CoolAs and Ray	|
  |		Thanks to Dirbaio!		|
  |_____________________________________________|
*/

#include "world.h"
#include "blockID.h"
#include <stdio.h>
#include <nds.h>	
#define GRAVEL_START WORLD_HEIGHT/4
#define GRAVEL_RARENESS 20
#define COAL_START WORLD_HEIGHT/4
#define COAL_RARENESS 48
#define IRON_START WORLD_HEIGHT/3
#define IRON_RARENESS 64
#define GOLD_START WORLD_HEIGHT/3*2
#define GOLD_RARENESS 68
#define REDSTONE_START WORLD_HEIGHT/3*2
#define REDSTONE_RARENESS 48
#define DIAMOND_START WORLD_HEIGHT/3*2
#define DIAMOND_RARENESS 186
#define LAPIS_START WORLD_HEIGHT/3*2
#define LAPIS_RARENESS 88

/// @breif Enforces that the input value is above zero.
/// If it is below zero, it is multiplied by -1.
/// Equivalent to abs(i) in the <math.h> library.
int noNeg(int i)
{
	//I don't want to include the math.h libary
	if (i<0) i*-1;
	return i;
}
///@breif A function that grows Grass.
///It turns every grass block into dirt then it turns the first dirt block it sees (from the top of the map) into grass. 
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
///@breif A function that helps snowbiomes grow grass properly.
///It uses a small HAX which is kinda needed because it is hard to tell whether to grow snow grass blocks or normal grass blocks.
void fixsnowgrass(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
			if (world->blocks[i][j]==SNOW_GRASS) world->blocks[i][j]=1207; //Change all the SNOW_GRASS blocks into SNOW_DIRT blocks
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==1207){
				world->blocks[i][j]=SNOW_GRASS; //Then if a block should be grass make it grass
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
			else if (world->blocks[i][j]!=AIR && world->blocks[i][j]!=LEAF && world->blocks[i][j]!=DARK_WOOD){ //And If we have not encountered dirt and we are at a different block
				j=WORLD_HEIGHT+1;			    //Exit this X
			}
		}
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
		{
			if (world->blocks[i][j]==1207) world->blocks[i][j]=DIRT; //Change all the SNOW_GRASS blocks into <bugfixer> blocks
			if (world->blocks[i][j]==1208) world->blocks[i][j]=DIRT; //Change all the SNOW_GRASS blocks into <bugfixer> blocks
		}
	swiWaitForVBlank();
}
///@breif This function adds stone from the x,y point all the way down to bedrock.
///It doesn't remove the bedrock though.
void rockwall(worldObject* world,int x,int y){
	int i;
	for (i=y;i<WORLD_HEIGHT;i++){
		world->blocks[x][i]=STONE;
	}
}
///@breif This function adds ores to the world.
/// It is about to get the scrap because it doesn't create big chunks of ore.
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
	//swiWaitForVBlank();
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
	//swiWaitForVBlank();
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
	//swiWaitForVBlank();
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
	//swiWaitForVBlank();
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
///@breif this function grows a tree at x,y.
///The x,y coords should point to the ground block to plant the tree on, not where the tree should start from.
///
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
///@breif This function adds STONE after 3+rand() blocks from the first dirt block it finds from the top of the world. [INFO NEEDED]
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

	//swiWaitForVBlank();
}
///@breif This function adds a cactus to the ground at x,y
///The x,y value should point to the actual ground, not where the cactus should be.
void addCACTUS(worldObject* world,int x,int y){
	int height=rand() %1+1;
	int i;
	for (i=0;i<=height;i++){
		y--;
		world->blocks[x][y]=CACTUS;
	}
}
///@breif This function adds a flower onto the ground at x,y-1.
///y to point to the surface of which the flower should be placed.
///The x,y value should point to the actual ground, not where the plant should be.
void addFLOWER_RED(worldObject* world,int x,int y){
        world->blocks[x][y-1]=FLOWER_RED;
}

///@breif This function adds a flower onto the ground at x,y-1.
///y to point to the surface of which the flower should be placed.
///The x,y value should point to the actual ground, not where the plant should be.
void addFLOWER_YELLOW(worldObject* world,int x,int y){
        world->blocks[x][y-1]=FLOWER_YELLOW;
}
///@breif This function adds all of the gravel into the map...
void addGRAVEL(worldObject* world){
	int y=GRAVEL_START;
	int i;
	int id=GRAVEL;
	int rareness=GRAVEL_RARENESS*2;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==GRAVEL) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
}
///@breif Plants trees on Grass blocks.
///It runs addtree every 5+rand() grass blocks to generate a tree on the ground.
void addTrees(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i+=rand() % 5+5)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if (world->blocks[i][j]==GRASS && (world->blocks[i-1][j]==GRASS || world->blocks[i+1][j]==GRASS)) addtree(world,i,j,0);
			else if (world->blocks[i][j]==SAND) addCACTUS(world,i,j);	
            else if (world->blocks[i][j]==SNOW_GRASS && (world->blocks[i-1][j]==SNOW_GRASS || world->blocks[i+1][j]==SNOW_GRASS)) addtree(world,i,j,1); //1 makes the trees have darker wood...
		}
	//swiWaitForVBlank();
}
///@breif This function adds flowers to the game. [INFO NEEDED]
///Ray programed it and I can't be bothered to look into how it works...
void addFlowers(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i+=rand() % 10+2)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if ((world->blocks[i][j]==GRASS || world->blocks[i][j]==SNOW_GRASS) && rand() % 2) addFLOWER_RED(world,i,j);	//rand() %2 makes a 1/2 chance of running addFLOWER_RED only if the addFLOWER_RED was NOT run, will the addFLOWER_YELLOW run... you need chance of the addFLOW_RED not running for the YELLOW to run..
			else if (world->blocks[i][j]==GRASS) addFLOWER_YELLOW(world,i,j);	
		}
	//swiWaitForVBlank();
}
///@breif A function that modifies the world, by planting trees and growing grasses and flowers.
///It basically calls a whole heap of different functions that does its job for it.
///However it does generate bedrock for you.
///It is called by generateWorld(world);
void modifyWorld(worldObject* world){

	swiWaitForVBlank();
	int x;
	fixgrass(world);
	fixsnowgrass(world);
	addrock(world);
	addore(world);
	addFlowers(world);
	addTrees(world);
	addGRAVEL(world);
	for (x=0;x<=WORLD_WIDTH;x++){
		world->blocks[x][WORLD_HEIGHT]=BEDROCK;	
	}
	//swiWaitForVBlank();
	world->version=2;
}
///@breif A general mountain biome, about to get the scrap!
///It doesn't look too much like a mountain, more like random landscape.
int mountainbiome(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	for (int x=startx;x<endx;x++){
		y+=rand() % 5-2;
		if (y<5) y-=rand() %3-3;
		if (y>WORLD_HEIGHT/3) y-=rand()%3;
		int i;
		//iprintf("%d\n",y);
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=DIRT;
	}
	return y;
}
///@breif A general flat biome.
///It picks whether to move up or down a block and draws a straight line from that point.
///Every now and again it move up or down a block again.
int flatbiome(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	int x;
	int changey=0;
	int i;
	int times=rand()%3+1;
	for (x=startx;x<endx;x++){
		if (changey==0){
			y+=rand() % 3-1;	
			times++;	
			if (y<5) y-=rand() %2-2;
			if (y>WORLD_HEIGHT/3) y-=rand()%2;	
			changey=rand() % 5+2;
			//swiWaitForVBlank();
		}
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=DIRT;
		changey--;
	}
	return y;
}
///@breif A general desert biome, about to get the scrap!
int sandbiome(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	int x;
	int changey=0;
	int i;
	int times=rand()%3+1;
	for (x=startx;x<endx;x++){
		if (changey==0){
			y+=rand() % 3-1;	
			times++;	
			if (y<5) y-=rand() %2-2;
			if (y>WORLD_HEIGHT/3) y-=rand()%2;	
			changey=4+rand() % 2;
		}
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=SAND;
		changey--;
	}
	return y;
}
///@breif A general snow biome, about to get the scrap!
int snowbiome(worldObject* world,int startx,int starty,int endx){
	int y=starty;
	int x;
	int changey=0;
	int i;
	int times=rand()%3+1;
	for (x=startx;x<endx;x++){
		if (changey==0){
			y+=rand() % 3-1;	
			times++;	
			if (y<5) y-=rand() %2-2;
			if (y>WORLD_HEIGHT/3) y-=rand()%2;	
			changey=4+rand() % 2;
		}
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=SNOW_GRASS;
		changey--;
	}
	return y;
}
void drawLineThing(worldObject*,int x1,int y1,int x2,int y2,int blockType);
///@breifA biome that uses fractals to make awesome landscape.
///Every 4 + rand() blocks it chooses a EndY amount, which it draws a line of blocks to.
///At the end of the biome It draws a line to the final EndY amount, which is currently randomly chosen.
int mountainSomethingBiome(worldObject* world,int startx,int starty,int endx)
{
	int y=starty;
	int x=startx;
	//We need to find a end y!
	///This can be defined!
	int endy=starty+(rand()%23-11);
	if (endy<0) endy=16;
	else if (endy>WORLD_HEIGHT/2) endy=WORLD_HEIGHT/2;
	while (x<endx-8)
	{
		int x1=x;
		x+=(rand() % 6)+4;
		int x2=x;
		int y1=y;
		if (y<=endy+12 && y>=endy-12) y+=4-(rand() % 9);
		else if (y>=endy+12) y=endy+8;
		else if (y<=endy-12) y=endy-8;
		if (y<8) y=8;
		int y2=y;
		drawLineThing(world,x1,y1,x2,y2,DIRT);
	}
	drawLineThing(world,x,y,endx,endy,DIRT);
	return endy;
}

///@breif The main world Generator.
///It picks a different biome every 16 + rand() blocks.
///After picking a biome it then runs the biomeGenerate function.
///It also calls modifyWorld(world); which plants trees and flowers, generates bedrock and adds ores.
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
		biometype=rand() %5;
		//biometype=4; //Used to test certain biomes.
		if (biometype==0) y2=mountainbiome(world,x,y,endx);
		else if (biometype==1) y2=flatbiome(world,x,y,endx);
		else if (biometype==2) y2=sandbiome(world,x,y,endx);
		else if (biometype==3) y2=snowbiome(world,x,y,endx);
		else if (biometype==4) y2=mountainSomethingBiome(world,x,y,endx);
		x=endx;
		y=y2;
	}
	modifyWorld(world);
}
