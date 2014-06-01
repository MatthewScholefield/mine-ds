#include <fat.h>
#include <nds.h>
#include <stdio.h>
//#include <sys/dir.h>
#include <string>
#include "world.h"
#include "inventory.h"

FILE* data;

bool init = false;

void stopNow()
{
    while(1)
        swiWaitForVBlank();
}

void closeFiles()
{
    fclose(data);
}

bool endOfFile()
{
    return feof(data);
}

void openFiles()
{
    if (!init)
    {
        fatInitDefault();
        init = true;
    }
    data = fopen ("/mineds/data.txt", "rb");//Opens file in "rb" mode
    if (!data)
    {
        iprintf("Error opening /MineDS/data.txt");
        stopNow();
        return;
    }
    
    /*while (fgets(buf,1000,data)!=NULL)
        iprintf("%s",buf);
    closeFiles();
    stopNow();*/
}
void openFiles(const char* mode)
{
    if (!init)
    {
        fatInitDefault();
        init = true;
    }
    data = fopen ("/mineds/data.txt", mode);
    if (!data)
    {
        iprintf("Error opening /MineDS/data.txt");
        stopNow();
        return;
    }
    
    /*while (fgets(buf,1000,data)!=NULL)
        iprintf("%s",buf);
    closeFiles();
    stopNow();*/
}

std::string getLine()
{
    char buf[1000];
    fgets(buf,1000,data);
    std::string temp(buf);
    return temp;
}

const char getChar()
{
    return fgetc(data);
}

void writeString(std::string write)
{
    for (char i:write)
        fputc(i,data);
}

void saveWorld(worldObject* world)
{
    openFiles("wb");
    //world is the world object
    //write the world object
    fwrite(world,sizeof(*world),1,data);
    saveInventory(data);
    closeFiles();
}
void loadWorld(worldObject* world)
{
    openFiles("rb");
    //world is the world object
    fread(world,sizeof(*world),1,data);
    loadInventory(data);
    closeFiles();
}
