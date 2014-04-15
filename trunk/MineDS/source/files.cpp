#include <fat.h>
#include <nds.h>
#include <stdio.h>
//#include <sys/dir.h>
#include <string>
#include "world.h"

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
    openFiles();
    for (int j=0; j<WORLD_HEIGHT-4;j++)
    {
        for (int i=0; i<WORLD_WIDTH;i++)
        {
            writeString(""+world->blocks[i][j]);
            writeString(".");
        }
        writeString("\n");
    }
    closeFiles();
}