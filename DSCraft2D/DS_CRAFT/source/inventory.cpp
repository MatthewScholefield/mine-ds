#include "inventory.h"
#include "blockID.h"
#include "allblocks.h"
#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "saver.h" //Saver Loades the numbers into VRAM...
char buffer[4];	
u16* x;
inventoryStruct inventory;
void inventoryInit(){
	int i;
	for (i=0;i<=35;i++){
		inventory.blockID[i]=255;
		inventory.blockAmount[i]=0;
	}
	x=numberReturn(10); //10 is the "x" Symbol... (Yeah I know suckish implementation...)
}
int inventoryAdd(int blockID){
	int i;
	int found=0;
	int foundID=0;
	for (i=0;i<=35;i++){
	//Find an entry of "blockID"
		if (inventory.blockID[i]==blockID){
			foundID=i;//Keep the entry """address""" in the variable foundID...
			inventory.blockAmount[foundID]++; //Add one to the amount of those blocks...
			i=37;
			return 1;
		}
		else if (inventory.blockID[i]==255){
			//We have found a AIR block which can serve as an empty space...
			foundID=i;
			inventory.blockID[foundID]=blockID;
			inventory.blockAmount[foundID]=0;
			i=37;	
			return 1;
		}
	}
	return 0;
}
int inventoryRemove(int blockID){
	int i;
	int found=0;
	int foundID=0;
	for (i=0;i<=35;i++){
	//Find an entry of "blockID"
		if (inventory.blockID[i]==blockID){
			found=1;
			foundID=i;//Keep the entry """address""" in the variable foundID...
			i=37;
		}
	}
	if (found==0) return 0;
	else if (found==1){
		if (inventory.blockAmount[foundID]>0) inventory.blockAmount[foundID]--;
		else{
			inventory.blockID[foundID]=255;
			inventory.blockAmount[foundID]=0;
		}
	}
	return 1;
}
void DrawAmountNum(int blockID){
	int i;
	int foundID;
	bool found=false;
	for (i=0;i<=35;i++){
	//Find an entry of "blockID"
		if (inventory.blockID[i]==blockID){
			foundID=i;//Keep the entry """address""" in the variable foundID...
			i=37;
			found=true;
		}
	}
	oamClear(&oamSub,19,25);
	if (found){
		if (blockID!=255) sprintf (buffer, "%daa",inventory.blockAmount[foundID]+1);
		else sprintf(buffer,"0aaa");// aa is displayed as a blank letter...
		int number1=buffer[0]-48;
		int number2=buffer[1]-48;
		if (number1!=0)oamSet(&oamSub,20,70,64,0,3,SpriteSize_16x16,SpriteColorFormat_256Color,numberReturn(10),-1,false,false,false, false,false); 
		if (number1!=0)oamSet(&oamSub,21,86,64,0,3,SpriteSize_16x16,SpriteColorFormat_256Color,numberReturn(number1),-1,false,false,false, false,false); 
		if (number2<10)oamSet(&oamSub,22,102,64,0,3,SpriteSize_16x16,SpriteColorFormat_256Color,numberReturn(number2),-1,false,false,false, false,false); 
	}
}
void DrawAmount(worldObject* world){
	int blockID=world->ChoosedBlock;
	int i;
	int foundID;
	bool found=false;
	for (i=0;i<=35;i++){
	//Find an entry of "blockID"
		if (inventory.blockID[i]==blockID){
			i=37;
			found=true;
		}
	}
	if (!found){
		world->ChoosedBlock=AIR;		
	}
	DrawAmountNum(blockID);
}
void invSave(FILE* save_file){
        fwrite(&inventory, 1, sizeof(inventory), save_file);
}
void invLoad(FILE* save_file){
        fread(&inventory, 1, sizeof(inventory), save_file);
}
bool invHave(int blockID){
	int i;
	int found=0;
	int foundID=0;
	for (i=0;i<=35;i++){
	//Find an entry of "blockID"
		if (inventory.blockID[i]==blockID){
			found=1;
			foundID=i;//Keep the entry """address""" in the variable foundID...
			i=37;
		}
	}
	if (found==0) return 0;
	else return 1;
	return 0;
}
bool invHaveAmount(int blockID,int amount){
	int i;
	int found=0;
	int foundID=0;
	for (i=0;i<=35;i++){
	//Find an entry of "blockID"
		if (inventory.blockID[i]==blockID){
			found=1;
			foundID=i;//Keep the entry """address""" in the variable foundID...
			i=37;
		}
	}
	if (inventory.blockAmount[foundID]<amount) return 0;
	else return 1;
	return 0;
}
